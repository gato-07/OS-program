#include "../include/servidor_cache.h"
#include "../include/cache_fifo.h"
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

// Implementación de CacheFIFO
CacheFIFO::CacheFIFO(size_t tamanio) 
    : tamanioMaximo(tamanio), hits(0), misses(0) {
}

bool CacheFIFO::obtener(const std::string& consulta, std::vector<Resultado>& resultados) {
    auto it = mapaCache.find(consulta);
    if (it != mapaCache.end()) {
        hits++;
        resultados = it->second;
        return true;
    }
    misses++;
    return false;
}

void CacheFIFO::insertar(const std::string& consulta, const std::vector<Resultado>& resultados) {
    // No duplicar si ya existe
    if (mapaCache.find(consulta) != mapaCache.end()) {
        return;
    }
    
    // Evicción FIFO si está lleno
    if (mapaCache.size() >= tamanioMaximo) {
        std::string masAntiguo = colaFIFO.front();
        colaFIFO.pop();
        mapaCache.erase(masAntiguo);
    }
    
    // Insertar nuevo
    mapaCache[consulta] = resultados;
    colaFIFO.push(consulta);
}

std::string CacheFIFO::obtenerEstadisticas() {
    int total = hits + misses;
    double tasaAciertos = total > 0 ? (double)hits / total * 100.0 : 0.0;
    
    std::ostringstream oss;
    oss << "HITS:" << hits 
        << "|MISSES:" << misses 
        << "|TASA:" << tasaAciertos << "%";
    return oss.str();
}

void CacheFIFO::limpiar() {
    mapaCache.clear();
    while (!colaFIFO.empty()) colaFIFO.pop();
    hits = 0;
    misses = 0;
}

// Implementación de ServidorCache
ServidorCache::ServidorCache(size_t tamanioCache) 
    : cache(tamanioCache), socketServidor(-1) {
}

ServidorCache::~ServidorCache() {
    if (socketServidor != -1) {
        close(socketServidor);
    }
}

bool ServidorCache::iniciar() {
    // Crear socket
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor == -1) {
        std::cerr << "ERROR: No se pudo crear socket del cache\n";
        return false;
    }
    
    // Reutilizar puerto
    int opcion = 1;
    setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion));
    
    // Configurar dirección
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO_CACHE);
    
    // Bind
    if (bind(socketServidor, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        std::cerr << "ERROR: Bind falló en puerto " << PUERTO_CACHE << "\n";
        return false;
    }
    
    // Listen
    if (listen(socketServidor, 5) < 0) {
        std::cerr << "ERROR: Listen falló\n";
        return false;
    }
    
    std::cout << "[CACHE] Servidor iniciado en puerto " << PUERTO_CACHE << "\n";
    return true;
}

void ServidorCache::ejecutar() {
    std::cout << "[CACHE] Esperando conexiones...\n";
    
    while (true) {
        struct sockaddr_in direccionCliente;
        socklen_t longitudCliente = sizeof(direccionCliente);
        
        int socketCliente = accept(socketServidor, (struct sockaddr*)&direccionCliente, &longitudCliente);
        if (socketCliente < 0) {
            std::cerr << "[CACHE] Error aceptando conexión\n";
            continue;
        }
        
        // Leer comando
        char buffer[TAMANIO_BUFFER] = {0};
        int bytesLeidos = read(socketCliente, buffer, TAMANIO_BUFFER - 1);
        
        if (bytesLeidos > 0) {
            std::string comando(buffer);
            procesarComando(socketCliente, comando);
        }
        
        close(socketCliente);
    }
}

void ServidorCache::procesarComando(int socketCliente, const std::string& comando) {
    std::istringstream iss(comando);
    std::string cmd;
    std::getline(iss, cmd, ':');
    
    if (cmd == "GET") {
        std::string consulta;
        std::getline(iss, consulta);
        
        std::vector<Resultado> resultados;
        if (cache.obtener(consulta, resultados)) {
            std::cout << "[CACHE] GET \"" << consulta << "\" -> HIT (devolviendo " << resultados.size() << " resultados)\n";
            std::string respuesta = "HIT:" + serializarResultados(resultados);
            enviarRespuesta(socketCliente, respuesta);
        } else {
            std::cout << "[CACHE] GET \"" << consulta << "\" -> MISS\n";
            enviarRespuesta(socketCliente, "MISS");
        }
        
    } else if (cmd == "PUT") {
        std::string consulta;
        std::getline(iss, consulta, ':');
        
        std::string datos;
        std::getline(iss, datos);
        
        std::vector<Resultado> resultados = deserializarResultados(datos);
        cache.insertar(consulta, resultados);
        std::cout << "[CACHE] PUT \"" << consulta << "\" -> Guardado (" << resultados.size() << " resultados)\n";
        enviarRespuesta(socketCliente, "OK");
        
    } else if (cmd == "STATS") {
        std::string stats = cache.obtenerEstadisticas();
        std::cout << "[CACHE] STATS -> " << stats << "\n";
        enviarRespuesta(socketCliente, stats);
        
    } else if (cmd == "CLEAR") {
        cache.limpiar();
        std::cout << "[CACHE] CLEAR -> Cache limpiado\n";
        enviarRespuesta(socketCliente, "OK");
        
    } else {
        std::cout << "[CACHE] ERROR -> Comando desconocido: \"" << cmd << "\"\n";
        enviarRespuesta(socketCliente, "ERROR:Comando desconocido");
    }
}

void ServidorCache::enviarRespuesta(int socketCliente, const std::string& respuesta) {
    send(socketCliente, respuesta.c_str(), respuesta.length(), 0);
}

std::string ServidorCache::serializarResultados(const std::vector<Resultado>& resultados) {
    std::ostringstream oss;
    for (size_t i = 0; i < resultados.size(); i++) {
        oss << resultados[i].libro << "," << resultados[i].score;
        if (i < resultados.size() - 1) oss << ";";
    }
    return oss.str();
}

std::vector<Resultado> ServidorCache::deserializarResultados(const std::string& datos) {
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

// MAIN
int main(int argc, char* argv[]) {
    size_t tamanioCache = 100;
    
    if (argc > 1) {
        tamanioCache = std::atoi(argv[1]);
    }
    
    std::cout << "=== SERVIDOR DE CACHE FIFO ===\n";
    std::cout << "Tamaño del cache: " << tamanioCache << " entradas\n\n";
    
    ServidorCache servidor(tamanioCache);
    
    if (!servidor.iniciar()) {
        std::cerr << "ERROR CRÍTICO: No se pudo iniciar el servidor de cache\n";
        return 1;
    }
    
    servidor.ejecutar();
    
    return 0;
}
