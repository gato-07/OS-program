#include "../include/buscador.h"
#include "../include/cliente_socket.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <limits>

// Implementación ClienteSocket
ClienteSocket::ClienteSocket(const std::string& h, int p) : host(h), puerto(p) {
}

bool ClienteSocket::enviarComando(const std::string& comando, std::string& respuesta, int timeoutSeg) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "ERROR: No se pudo crear socket\n";
        return false;
    }
    
    // Timeout
    struct timeval tv;
    tv.tv_sec = timeoutSeg;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    
    // Configurar servidor
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(puerto);
    
    if (inet_pton(AF_INET, host.c_str(), &direccionServidor.sin_addr) <= 0) {
        std::cerr << "ERROR: Dirección inválida\n";
        close(sock);
        return false;
    }
    
    // Conectar
    if (connect(sock, (struct sockaddr*)&direccionServidor, sizeof(direccionServidor)) < 0) {
        std::cerr << "ERROR: No se pudo conectar a " << host << ":" << puerto << "\n";
        close(sock);
        return false;
    }
    
    // Enviar
    if (send(sock, comando.c_str(), comando.length(), 0) < 0) {
        std::cerr << "ERROR: Fallo al enviar comando\n";
        close(sock);
        return false;
    }
    
    // Recibir
    char buffer[8192] = {0};
    int bytesRecibidos = recv(sock, buffer, 8191, 0);
    
    if (bytesRecibidos < 0) {
        std::cerr << "ERROR: Timeout o error al recibir\n";
        close(sock);
        return false;
    }
    
    respuesta = std::string(buffer, bytesRecibidos);
    close(sock);
    return true;
}

// Implementación Buscador
Buscador::Buscador(const std::string& host, int topk)
    : clienteCache(host, PUERTO_CACHE),
      clienteMotor(host, PUERTO_MOTOR),
      topkPorDefecto(topk) {
}

bool Buscador::verificarServicios() {
    std::cout << "\n[BUSCADOR] Verificando servicios...\n";
    
    std::string respuesta;
    
    // Verificar CACHE
    std::cout << "  - Verificando CACHE (puerto " << PUERTO_CACHE << ")... ";
    if (!clienteCache.enviarComando("STATS", respuesta, 5)) {
        std::cerr << "\n✗ ERROR CRÍTICO: Servicio CACHE no disponible\n";
        std::cerr << "  Inicia el cache en otra terminal con:\n";
        std::cerr << "    cd cache && make && ./bin/cache_servidor 100\n";
        return false;
    }
    std::cout << "✓ OK\n";
    
    // Verificar MOTOR
    std::cout << "  - Verificando MOTOR (puerto " << PUERTO_MOTOR << ")... ";
    if (!clienteMotor.enviarComando("SEARCH:test:1", respuesta, 5)) {
        std::cerr << "\n✗ ERROR CRÍTICO: Servicio MOTOR no disponible\n";
        std::cerr << "  Inicia el motor en otra terminal con:\n";
        std::cerr << "    cd motor && make && ./bin/motor_servidor <indice.idx> <mapa.txt>\n";
        return false;
    }
    std::cout << "✓ OK\n\n";
    
    return true;
}

void Buscador::buscar(const std::string& consulta, int topk) {
    if (topk == -1) topk = topkPorDefecto;
    
    auto inicioTotal = std::chrono::high_resolution_clock::now();
    Tiempos tiempos = {0, 0, 0};
    std::string origen;
    std::vector<Resultado> resultados;
    
    // PASO 1: Buscar en CACHE
    std::cout << "\n[1/3] Buscando en caché...\n";
    auto inicioCache = std::chrono::high_resolution_clock::now();
    
    std::string respuestaCache;
    if (!clienteCache.enviarComando("GET:" + consulta, respuestaCache)) {
        std::cerr << "✗ ERROR: Cache no responde\n";
        return;
    }
    
    auto finCache = std::chrono::high_resolution_clock::now();
    tiempos.cacheMs = std::chrono::duration<double, std::milli>(finCache - inicioCache).count();
    
    // Parsear respuesta cache
    if (respuestaCache.substr(0, 3) == "HIT") {
        std::cout << "  ✓ CACHE HIT\n";
        origen = "cache";
        std::string datos = respuestaCache.substr(4);
        resultados = deserializarResultados(datos);
        
    } else if (respuestaCache == "MISS") {
        std::cout << "  - CACHE MISS\n";
        origen = "memoria";
        
        // PASO 2: Buscar en MOTOR
        std::cout << "[2/3] Buscando en índice invertido...\n";
        auto inicioMemoria = std::chrono::high_resolution_clock::now();
        
        std::string comandoBusqueda = "SEARCH:" + consulta + ":" + std::to_string(topk);
        std::string respuestaMotor;
        
        if (!clienteMotor.enviarComando(comandoBusqueda, respuestaMotor)) {
            std::cerr << "✗ ERROR: Motor no responde\n";
            return;
        }
        
        auto finMemoria = std::chrono::high_resolution_clock::now();
        tiempos.memoriaMs = std::chrono::duration<double, std::milli>(finMemoria - inicioMemoria).count();
        
        // Parsear respuesta motor
        if (respuestaMotor.substr(0, 3) == "OK:") {
            std::string datos = respuestaMotor.substr(3);
            
            if (datos == "VACIO") {
                std::cout << "  - No se encontraron resultados\n";
            } else {
                resultados = deserializarResultados(datos);
                std::cout << "  ✓ Encontrados " << resultados.size() << " resultados\n";
                
                // PASO 3: Guardar en CACHE
                std::cout << "[3/3] Guardando en caché...\n";
                std::string comandoPut = "PUT:" + consulta + ":" + datos;
                std::string respuestaPut;
                
                if (clienteCache.enviarComando(comandoPut, respuestaPut)) {
                    std::cout << "  ✓ Guardado en caché\n";
                }
            }
        }
    }
    
    auto finTotal = std::chrono::high_resolution_clock::now();
    tiempos.totalMs = std::chrono::duration<double, std::milli>(finTotal - inicioTotal).count();
    
    // Mostrar resultado
    mostrarJSON(consulta, origen, resultados, tiempos, topk);
}

std::vector<Resultado> Buscador::deserializarResultados(const std::string& datos) {
    std::vector<Resultado> resultados;
    std::istringstream iss(datos);
    std::string item;
    
    while (std::getline(iss, item, ';')) {
        std::istringstream itemSS(item);
        std::string libro;
        int score;
        
        std::getline(itemSS, libro, ',');
        itemSS >> score;
        
        resultados.push_back({libro, score});
    }
    
    return resultados;
}

void Buscador::mostrarJSON(const std::string& consulta, const std::string& origen,
                          const std::vector<Resultado>& resultados, const Tiempos& tiempos, int topk) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "RESULTADO DE BÚSQUEDA (JSON)\n";
    std::cout << std::string(60, '=') << "\n";
    
    std::cout << "{\n";
    std::cout << "  \"Mensaje\": \"" << consulta << "\",\n";
    std::cout << "  \"origen_respuesta\": \"" << origen << "\",\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  \"tiempo_cache\": " << tiempos.cacheMs << " ms,\n";
    std::cout << "  \"tiempo_M_total\": " << tiempos.memoriaMs << " ms,\n";
    std::cout << "  \"tiempo_total\": " << tiempos.totalMs << " ms,\n";
    std::cout << "  \"topK\": " << topk << ",\n";
    std::cout << "  \"Respuesta\": [\n";
    
    for (size_t i = 0; i < resultados.size(); i++) {
        std::cout << "    {\"Libro\": \"" << resultados[i].libro << "\", \"Score\": " << resultados[i].score << "}";
        if (i < resultados.size() - 1) std::cout << ",";
        std::cout << "\n";
    }
    
    std::cout << "  ]\n";
    std::cout << "}\n";
    std::cout << std::string(60, '=') << "\n\n";
}

void Buscador::mostrarEstadisticasCache() {
    std::string respuesta;
    if (clienteCache.enviarComando("STATS", respuesta)) {
        std::cout << "\n[ESTADÍSTICAS DEL CACHE]\n";
        std::cout << respuesta << "\n\n";
    } else {
        std::cerr << "ERROR: No se pudo obtener estadísticas\n";
    }
}

// MAIN
int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int topk = 3;
    
    // Parsear argumentos
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" && i + 1 < argc) {
            host = argv[++i];
        } else if (arg == "-k" && i + 1 < argc) {
            topk = std::atoi(argv[++i]);
        }
    }
    
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║      SISTEMA DE BÚSQUEDA CON CACHÉ FIFO               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    
    Buscador buscador(host, topk);
    
    // Verificar servicios
    if (!buscador.verificarServicios()) {
        std::cerr << "\n✗ No se pueden realizar búsquedas sin los servicios activos\n";
        return 1;
    }
    
    std::cout << "Comandos disponibles:\n";
    std::cout << "  - <consulta>  : Buscar libros\n";
    std::cout << "  - stats       : Ver estadísticas del caché\n";
    std::cout << "  - salir       : Salir del programa\n\n";
    
    std::string entrada;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    while (true) {
        std::cout << "buscador> ";
        std::getline(std::cin, entrada);
        
        if (entrada.empty()) continue;
        
        if (entrada == "salir" || entrada == "exit") {
            std::cout << "¡Hasta luego!\n";
            break;
        }
        
        if (entrada == "stats") {
            buscador.mostrarEstadisticasCache();
            continue;
        }
        
        buscador.buscar(entrada, topk);
    }
    
    return 0;
}
