#ifndef INDICE_PARALELO_H
#define INDICE_PARALELO_H

#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>
#include <deque>
#include <atomic>

// Estructura para mapear un libro (ID -> nombre)
struct MapaLibro {
    int id;
    std::string nombre;
    std::string ruta;
};

// Estructura para registrar la actividad de cada thread
struct LogEntry {
    int threadId;
    int libroId;
    std::string nombreLibro;
    int cantidadPalabras;
    std::chrono::system_clock::time_point inicio;
    std::chrono::system_clock::time_point fin;
    
    // Calcula la duración en milisegundos
    long long duracionMs() const {
        auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
        return duracion.count();
    }
};

// Tipo para el índice invertido: palabra -> (idLibro -> cantidad)
using IndiceInvertido = std::map<std::string, std::map<int, int>>;

// Estructura para cola de work stealing (similar al resto del código)
struct ColaWorkStealing {
    std::deque<MapaLibro> libros;
    std::mutex mutex;
};

// Variables globales compartidas (protegidas con mutex)
extern IndiceInvertido indiceGlobal;
extern std::mutex mutexIndice;
extern std::mutex mutexLog;
extern std::vector<LogEntry> registrosLog;

// Funciones para work stealing
void agregarTrabajo(ColaWorkStealing* cola, const MapaLibro& libro);
bool tomarTrabajo(ColaWorkStealing* cola, MapaLibro& libro);
bool robarTrabajo(ColaWorkStealing* cola, MapaLibro& libro);
bool colaVacia(ColaWorkStealing* cola);
size_t tamanioCola(ColaWorkStealing* cola);

// Configuración del sistema (variables de entorno)
struct ConfiguracionSistema {
    int nThreads;        // N-THREADS
    int nLote;          // N-LOTE
    std::string rutaMapaLibros;  // MAPA-LIBROS
};

// Funciones principales
bool cargarConfiguracion(ConfiguracionSistema& config);
std::vector<MapaLibro> escanearLibros(const std::string& carpeta);
bool guardarMapaLibros(const std::vector<MapaLibro>& libros, const std::string& rutaArchivo);
void procesarLibro(const MapaLibro& libro, int threadId);
bool guardarIndice(const std::string& archivoSalida);
bool guardarLog(const std::string& archivoLog);

#endif // INDICE_PARALELO_H
