#include "../include/servidor_motor.h"
#include "../include/indice_invertido.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

// Implementación de IndiceInvertido
bool IndiceInvertido::cargarIndice(const std::string& archivoIndice) {
    std::ifstream archivo(archivoIndice);
    if (!archivo.is_open()) {
        std::cerr << "ERROR: No se pudo abrir " << archivoIndice << "\n";
        return false;
    }
    
    std::string linea;
    while (std::getline(archivo, linea)) {
        // Formato: palabra;(nombre_archivo.txt,freq1);(nombre_archivo2.txt,freq2);...
        std::istringstream iss(linea);
        std::string palabra;
        std::getline(iss, palabra, ';');
        
        std::vector<std::pair<std::string, int>> documentos;
        std::string item;
        
        while (std::getline(iss, item, ';')) {
            // Parsear (nombre,freq)
            size_t coma = item.find(',');
            if (coma != std::string::npos) {
                std::string nombre = item.substr(1, coma - 1); // Saltar '('
                int freq = std::stoi(item.substr(coma + 1, item.length() - coma - 2)); // Hasta ')'
                documentos.push_back({nombre, freq});
            }
        }
        
        indice[palabra] = documentos;
    }
    
    std::cout << "[MOTOR] Índice cargado: " << indice.size() << " palabras\n";
    return true;
}

std::vector<Resultado> IndiceInvertido::buscarTopK(const std::string& consulta, int k) {
    // Tokenizar consulta
    std::istringstream iss(consulta);
    std::vector<std::string> palabras;
    std::string palabra;
    
    while (iss >> palabra) {
        // Convertir a minúsculas
        std::transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
        palabras.push_back(palabra);
    }
    
    // Acumular scores por documento
    std::map<std::string, int> scoresPorNombre;
    
    for (const auto& palabra : palabras) {
        auto it = indice.find(palabra);
        if (it != indice.end()) {
            for (const auto& [nombreLibro, freq] : it->second) {
                scoresPorNombre[nombreLibro] += freq;
            }
        }
    }
    
    // Convertir a vector de resultados
    std::vector<Resultado> resultados;
    for (const auto& [nombreLibro, score] : scoresPorNombre) {
        // Limpiar el nombre (remover .txt si existe)
        std::string nombreLimpio = nombreLibro;
        if (nombreLimpio.size() > 4 && nombreLimpio.substr(nombreLimpio.size() - 4) == ".txt") {
            nombreLimpio = nombreLimpio.substr(0, nombreLimpio.size() - 4);
        }
        resultados.push_back({nombreLimpio, score});
    }
    
    // Ordenar por score descendente
    std::sort(resultados.begin(), resultados.end(),
              [](const Resultado& a, const Resultado& b) {
                  return a.score > b.score;
              });
    
    // Retornar solo TopK
    if (resultados.size() > (size_t)k) {
        resultados.resize(k);
    }
    
    return resultados;
}

// Implementación de ServidorMotor
ServidorMotor::ServidorMotor() : socketServidor(-1) {
}

ServidorMotor::~ServidorMotor() {
    if (socketServidor != -1) {
        close(socketServidor);
    }
}

bool ServidorMotor::cargarIndice(const std::string& archivoIndice) {
    return indice.cargarIndice(archivoIndice);
}

bool ServidorMotor::iniciar() {
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor == -1) {
        std::cerr << "ERROR: No se pudo crear socket del motor\n";
        return false;
    }
    
    int opcion = 1;
    setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion));
    
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO_MOTOR);
    
    if (bind(socketServidor, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        std::cerr << "ERROR: Bind falló en puerto " << PUERTO_MOTOR << "\n";
        return false;
    }
    
    if (listen(socketServidor, 5) < 0) {
        std::cerr << "ERROR: Listen falló\n";
        return false;
    }
    
    std::cout << "[MOTOR] Servidor iniciado en puerto " << PUERTO_MOTOR << "\n";
    return true;
}

void ServidorMotor::ejecutar() {
    std::cout << "[MOTOR] Esperando conexiones...\n";
    
    while (true) {
        struct sockaddr_in direccionCliente;
        socklen_t longitudCliente = sizeof(direccionCliente);
        
        int socketCliente = accept(socketServidor, (struct sockaddr*)&direccionCliente, &longitudCliente);
        if (socketCliente < 0) {
            std::cerr << "[MOTOR] Error aceptando conexión\n";
            continue;
        }
        
        char buffer[TAMANIO_BUFFER] = {0};
        int bytesLeidos = read(socketCliente, buffer, TAMANIO_BUFFER - 1);
        
        if (bytesLeidos > 0) {
            std::string comando(buffer);
            procesarComando(socketCliente, comando);
        }
        
        close(socketCliente);
    }
}

void ServidorMotor::procesarComando(int socketCliente, const std::string& comando) {
    std::istringstream iss(comando);
    std::string cmd;
    std::getline(iss, cmd, ':');
    
    if (cmd == "SEARCH") {
        std::string consulta;
        std::getline(iss, consulta, ':');
        
        std::string topkStr;
        std::getline(iss, topkStr);
        int topk = std::stoi(topkStr);
        
        std::cout << "[MOTOR] SEARCH \"" << consulta << "\" (topK=" << topk << ")";
        std::vector<Resultado> resultados = indice.buscarTopK(consulta, topk);
        std::cout << " -> " << resultados.size() << " resultados encontrados\n";
        
        std::string respuesta = "OK:" + serializarResultados(resultados);
        enviarRespuesta(socketCliente, respuesta);
        
    } else {
        std::cout << "[MOTOR] ERROR -> Comando desconocido: \"" << cmd << "\"\n";
        enviarRespuesta(socketCliente, "ERROR:Comando desconocido");
    }
}

void ServidorMotor::enviarRespuesta(int socketCliente, const std::string& respuesta) {
    send(socketCliente, respuesta.c_str(), respuesta.length(), 0);
}

std::string ServidorMotor::serializarResultados(const std::vector<Resultado>& resultados) {
    if (resultados.empty()) {
        return "VACIO";
    }
    
    std::ostringstream oss;
    for (size_t i = 0; i < resultados.size(); i++) {
        oss << resultados[i].libro << "," << resultados[i].score;
        if (i < resultados.size() - 1) oss << ";";
    }
    return oss.str();
}

// MAIN
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_indice.idx>\n";
        std::cerr << "\nEjemplo:\n";
        std::cerr << "  " << argv[0] << " indice.idx\n";
        return 1;
    }
    
    std::string archivoIndice = argv[1];
    
    std::cout << "=== SERVIDOR MOTOR DE BÚSQUEDA ===\n";
    std::cout << "Archivo índice: " << archivoIndice << "\n\n";
    
    ServidorMotor servidor;
    
    if (!servidor.cargarIndice(archivoIndice)) {
        std::cerr << "ERROR CRÍTICO: No se pudo cargar el índice invertido\n";
        return 1;
    }
    
    if (!servidor.iniciar()) {
        std::cerr << "ERROR CRÍTICO: No se pudo iniciar el servidor motor\n";
        return 1;
    }
    
    servidor.ejecutar();
    
    return 0;
}
