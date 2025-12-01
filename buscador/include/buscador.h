#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "cliente_socket.h"
#include <vector>
#include <string>

#define PUERTO_CACHE 8001
#define PUERTO_MOTOR 8002

// Estructura para resultados
struct Resultado {
    std::string libro;
    int score;
};

// Estructura para métricas de tiempo
struct Tiempos {
    double cacheMs;
    double memoriaMs;
    double totalMs;
};

class Buscador {
private:
    ClienteSocket clienteCache;
    ClienteSocket clienteMotor;
    int topkPorDefecto;
    
    std::vector<Resultado> deserializarResultados(const std::string& datos);
    void mostrarJSON(const std::string& consulta, const std::string& origen,
                     const std::vector<Resultado>& resultados, const Tiempos& tiempos, int topk);

public:
    Buscador(const std::string& host, int topk = 3);
    
    bool verificarServicios();
    void buscar(const std::string& consulta, int topk = -1);
    void mostrarEstadisticasCache();
};

#endif // BUSCADOR_H
