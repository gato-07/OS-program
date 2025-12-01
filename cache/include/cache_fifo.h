#ifndef CACHE_FIFO_H
#define CACHE_FIFO_H

#include <unordered_map>
#include <queue>
#include <string>
#include <vector>

// Estructura para un resultado de búsqueda
struct Resultado {
    std::string libro;
    int score;
};

// Clase Cache FIFO con HashMap
class CacheFIFO {
private:
    std::unordered_map<std::string, std::vector<Resultado>> mapaCache;
    std::queue<std::string> colaFIFO;
    size_t tamanioMaximo;
    
    // Estadísticas
    int hits;
    int misses;

public:
    CacheFIFO(size_t tamanio);
    
    // Operaciones principales
    bool obtener(const std::string& consulta, std::vector<Resultado>& resultados);
    void insertar(const std::string& consulta, const std::vector<Resultado>& resultados);
    std::string obtenerEstadisticas();
    void limpiar();
};

#endif // CACHE_FIFO_H
