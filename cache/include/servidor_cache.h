#ifndef SERVIDOR_CACHE_H
#define SERVIDOR_CACHE_H

#include "cache_fifo.h"
#include <string>

#define PUERTO_CACHE 8001
#define TAMANIO_BUFFER 4096

class ServidorCache {
private:
    CacheFIFO cache;
    int socketServidor;
    
    void procesarComando(int socketCliente, const std::string& comando);
    void enviarRespuesta(int socketCliente, const std::string& respuesta);
    
    // Serialización
    std::string serializarResultados(const std::vector<Resultado>& resultados);
    std::vector<Resultado> deserializarResultados(const std::string& datos);

public:
    ServidorCache(size_t tamanioCache);
    ~ServidorCache();
    
    bool iniciar();
    void ejecutar();
};

#endif // SERVIDOR_CACHE_H
