#ifndef INDICE_INVERTIDO_H
#define INDICE_INVERTIDO_H

#include <map>
#include <vector>
#include <string>

// Estructura para resultado de búsqueda
struct Resultado {
    std::string libro;
    int score;
};

// Clase para manejar el índice invertido
class IndiceInvertido {
private:
    // palabra -> [(nombreLibro, frecuencia), ...]
    std::map<std::string, std::vector<std::pair<std::string, int>>> indice;

public:
    bool cargarIndice(const std::string& archivoIndice);
    std::vector<Resultado> buscarTopK(const std::string& consulta, int k);
};

#endif // INDICE_INVERTIDO_H
