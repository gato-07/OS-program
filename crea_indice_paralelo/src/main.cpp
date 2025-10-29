#include "../include/indice_paralelo.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <atomic>

namespace fs = std::filesystem;

// Definición de las variables globales
IndiceInvertido indiceGlobal;
std::mutex mutexIndice;
std::mutex mutexLog;
std::vector<LogEntry> registrosLog;

void agregarTrabajo(ColaWorkStealing* cola, const MapaLibro& libro) {
    std::lock_guard<std::mutex> lock(cola->mutex);
    cola->libros.push_back(libro);
}

// Tomar trabajo del final de la cola (thread dueño)
bool tomarTrabajo(ColaWorkStealing* cola, MapaLibro& libro) {
    std::lock_guard<std::mutex> lock(cola->mutex);
    if (cola->libros.empty()) {
        return false;
    }
    libro = cola->libros.back();
    cola->libros.pop_back();
    return true;
}

// Robar trabajo del frente de la cola (otros threads)
bool robarTrabajo(ColaWorkStealing* cola, MapaLibro& libro) {
    std::lock_guard<std::mutex> lock(cola->mutex);
    if (cola->libros.empty()) {
        return false;
    }
    libro = cola->libros.front();
    cola->libros.pop_front();
    return true;
}

// Verificar si la cola está vacía
bool colaVacia(ColaWorkStealing* cola) {
    std::lock_guard<std::mutex> lock(cola->mutex);
    return cola->libros.empty();
}

// Obtener tamaño de la cola
size_t tamanioCola(ColaWorkStealing* cola) {
    std::lock_guard<std::mutex> lock(cola->mutex);
    return cola->libros.size();
}
bool cargarConfiguracion(ConfiguracionSistema& config) {
    std::cout << "Cargando configuración..." << std::endl;

    // Leer N_THREADS (valor por defecto: 4)
    const char* nThreadsEnv = std::getenv("N_THREADS");
    if (nThreadsEnv != nullptr) {
        config.nThreads = std::atoi(nThreadsEnv);
    } else {
        config.nThreads = 4; // valor por defecto
        std::cout << "  N_THREADS no definido, usando valor por defecto: 4" << std::endl;
    }

    // Leer N_LOTE (valor por defecto: 10)
    const char* nLoteEnv = std::getenv("N_LOTE");
    if (nLoteEnv != nullptr) {
        config.nLote = std::atoi(nLoteEnv);
    } else {
        config.nLote = 10; // valor por defecto
        std::cout << "  N_LOTE no definido, usando valor por defecto: 10" << std::endl;
    }

    // Leer MAPA_LIBROS (valor por defecto: data/mapa_libros.txt)
    const char* mapaLibrosEnv = std::getenv("MAPA_LIBROS");
    if (mapaLibrosEnv != nullptr) {
        config.rutaMapaLibros = mapaLibrosEnv;
    } else {
        config.rutaMapaLibros = "data/mapa_libros.txt";
        std::cout << "  MAPA_LIBROS no definido, usando valor por defecto: data/mapa_libros.txt" << std::endl;
    }

    std::cout << "\nConfiguración cargada:" << std::endl;
    std::cout << "  N_THREADS: " << config.nThreads << std::endl;
    std::cout << "  N_LOTE: " << config.nLote << std::endl;
    std::cout << "  MAPA_LIBROS: " << config.rutaMapaLibros << std::endl;

    return true;
}

std::vector<MapaLibro> escanearLibros(const std::string& carpeta) {
    std::vector<MapaLibro> libros;
    int idActual = 1;

    std::cout << "\nEscaneando libros en: " << carpeta << std::endl;

    // Verificar que la carpeta existe
    if (!fs::exists(carpeta)) {
        std::cerr << "Error: la carpeta '" << carpeta << "' no existe" << std::endl;
        return libros;
    }

    if (!fs::is_directory(carpeta)) {
        std::cerr << "Error: '" << carpeta << "' no es una carpeta" << std::endl;
        return libros;
    }

    // Recorrer todos los archivos en la carpeta
    try {
        for (const auto& entry : fs::directory_iterator(carpeta)) {
            // Solo procesar archivos regulares con extensión .txt
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                MapaLibro libro;
                libro.id = idActual++;
                libro.nombre = entry.path().stem().string(); // nombre sin extensión
                libro.ruta = entry.path().string();

                libros.push_back(libro);

                std::cout << "  [" << libro.id << "] " << libro.nombre
                          << " (" << entry.path().filename().string() << ")" << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error al escanear carpeta: " << e.what() << std::endl;
        return libros;
    }

    std::cout << "\nTotal de libros encontrados: " << libros.size() << std::endl;

    return libros;
}


bool guardarMapaLibros(const std::vector<MapaLibro>& libros, const std::string& rutaArchivo) {
    std::cout << "\nGuardando mapa de libros en: " << rutaArchivo << std::endl;

    // Crear directorio si no existe
    fs::path rutaCompleta(rutaArchivo);
    if (rutaCompleta.has_parent_path()) {
        fs::path directorio = rutaCompleta.parent_path();
        if (!fs::exists(directorio)) {
            try {
                fs::create_directories(directorio);
                std::cout << "  Directorio creado: " << directorio << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error al crear directorio: " << e.what() << std::endl;
                return false;
            }
        }
    }

    // Abrir archivo para escritura
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo crear el archivo " << rutaArchivo << std::endl;
        return false;
    }

    // Escribir cada libro en formato: id; nombre
    for (const auto& libro : libros) {
        archivo << libro.id << "; " << libro.nombre << std::endl;
    }

    archivo.close();

    std::cout << "  Mapa de libros guardado exitosamente" << std::endl;
    std::cout << "  Total de entradas: " << libros.size() << std::endl;

    return true;
}

void procesarLibro(const MapaLibro& libro, int threadId) {
    // Crear entrada de log
    LogEntry log;
    log.threadId = threadId;
    log.libroId = libro.id;
    log.nombreLibro = libro.nombre;
    log.cantidadPalabras = 0;
    log.inicio = std::chrono::system_clock::now();

    // Índice local para este libro
    std::map<std::string, int> indiceLocal;

    // Abrir archivo
    std::ifstream archivo(libro.ruta);
    if (!archivo.is_open()) {
        std::cerr << "  [Thread " << threadId << "] Error al abrir: " << libro.ruta << std::endl;
        log.fin = std::chrono::system_clock::now();

        // Registrar log incluso si falla
        std::lock_guard<std::mutex> lockLog(mutexLog);
        registrosLog.push_back(log);
        return;
    }

    // Leer y procesar palabras
    std::string palabra;
    while (archivo >> palabra) {
        // Limpiar signos de puntuación y convertir a minúsculas
        std::string limpia;
        for (char c : palabra) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                limpia += std::tolower(static_cast<unsigned char>(c));
            }
        }

        if (!limpia.empty()) {
            indiceLocal[limpia]++;
            log.cantidadPalabras++;
        }
    }

    archivo.close();

    // Actualizar índice global (sección crítica)
    {
        std::lock_guard<std::mutex> lockIndice(mutexIndice);
        for (const auto& [palabra, cantidad] : indiceLocal) {
            indiceGlobal[palabra][libro.id] = cantidad;
        }
    }

    // Marcar tiempo de fin
    log.fin = std::chrono::system_clock::now();

    // Registrar log (sección crítica)
    {
        std::lock_guard<std::mutex> lockLog(mutexLog);
        registrosLog.push_back(log);
    }

    std::cout << "  [Thread " << threadId << "] Libro " << libro.id
              << " (" << libro.nombre << "): " << log.cantidadPalabras
              << " palabras procesadas en " << log.duracionMs() << " ms" << std::endl;
}

bool guardarIndice(const std::string& archivoSalida) {
    std::cout << "\nGuardando índice invertido en: " << archivoSalida << std::endl;

    // Abrir archivo para escritura
    std::ofstream archivo(archivoSalida);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo crear el archivo " << archivoSalida << std::endl;
        return false;
    }

    for (const auto& [palabra, documentos] : indiceGlobal) {
        archivo << palabra;
        for (const auto& [idLibro, cantidad] : documentos) {
            archivo << ";(" << idLibro << "," << cantidad << ")";
        }
        archivo << "\n";
    }

    archivo.close();

    std::cout << "  Índice invertido guardado exitosamente" << std::endl;
    std::cout << "  Total de palabras: " << indiceGlobal.size() << std::endl;

    return true;
}

bool guardarLog(const std::string& archivoLog) {
    std::cout << "\nGuardando log de procesamiento en: " << archivoLog << std::endl;

    // Crear directorio logs si no existe
    fs::path rutaCompleta(archivoLog);
    if (rutaCompleta.has_parent_path()) {
        fs::path directorio = rutaCompleta.parent_path();
        if (!fs::exists(directorio)) {
            try {
                fs::create_directories(directorio);
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error al crear directorio logs: " << e.what() << std::endl;
                return false;
            }
        }
    }

    // Abrir archivo para escritura
    std::ofstream archivo(archivoLog);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo crear el archivo de log" << std::endl;
        return false;
    }

    // Escribir encabezado
    archivo << "thread_id;libro_id;nombre_libro;cantidad_palabras;timestamp_inicio;timestamp_fin;duracion_ms\n";

    // Escribir cada registro
    for (const auto& log : registrosLog) {
        // Convertir timestamps a formato legible
        auto inicioTime = std::chrono::system_clock::to_time_t(log.inicio);
        auto finTime = std::chrono::system_clock::to_time_t(log.fin);

        archivo << log.threadId << ";"
                << log.libroId << ";"
                << log.nombreLibro << ";"
                << log.cantidadPalabras << ";"
                << std::put_time(std::localtime(&inicioTime), "%Y-%m-%d %H:%M:%S") << ";"
                << std::put_time(std::localtime(&finTime), "%Y-%m-%d %H:%M:%S") << ";"
                << log.duracionMs() << "\n";
    }

    archivo.close();

    std::cout << "  Log guardado exitosamente" << std::endl;
    std::cout << "  Total de registros: " << registrosLog.size() << std::endl;

    return true;
}

int main(int argc, char* argv[]) {
    std::cout << "=== CREADOR DE ÍNDICE INVERTIDO PARALELO ===" << std::endl << std::endl;

    // Verificar argumentos
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <archivo_salida.idx> <carpeta_libros>" << std::endl;
        std::cerr << "\nEjemplo:" << std::endl;
        std::cerr << "  " << argv[0] << " indice.idx carpeta_con_libros/" << std::endl;
        return 1;
    }

    std::string archivoSalida = argv[1];
    std::string carpetaLibros = argv[2];

    // Validar extensión .idx
    if (archivoSalida.size() < 4 || archivoSalida.substr(archivoSalida.size() - 4) != ".idx") {
        std::cerr << "Error: el archivo de salida debe tener extensión .idx" << std::endl;
        return 1;
    }

    // Cargar configuración
    ConfiguracionSistema config;
    if (!cargarConfiguracion(config)) {
        std::cerr << "Error al cargar la configuración" << std::endl;
        return 1;
    }

    std::cout << "\n Configuración cargada exitosamente" << std::endl;
    std::cout << "\nArchivo de salida: " << archivoSalida << std::endl;
    std::cout << "Carpeta de libros: " << carpetaLibros << std::endl;

    std::vector<MapaLibro> libros = escanearLibros(carpetaLibros);
    if (libros.empty()) {
        std::cerr << "\nError: No se encontraron libros (.txt) en la carpeta" << std::endl;
        return 1;
    }

    if (!guardarMapaLibros(libros, config.rutaMapaLibros)) {
        std::cerr << "\nError: No se pudo guardar el mapa de libros" << std::endl;
        return 1;
    }

    // PASO 3: Procesar libros EN PARALELO con planificador WORK STEALING
    std::cout << "\n=== PROCESAMIENTO PARALELO CON WORK STEALING ===" << std::endl;
    std::cout << "Estrategia: Work Stealing con " << config.nThreads << " threads" << std::endl;
    std::cout << "Total de libros a procesar: " << libros.size() << std::endl;

    auto inicioTotal = std::chrono::system_clock::now();

    std::vector<ColaWorkStealing> colasThreads(config.nThreads);

    std::cout << "\nDistribución inicial de trabajo:" << std::endl;
    for (size_t i = 0; i < libros.size(); i++) {
        int threadId = i % config.nThreads;
        agregarTrabajo(&colasThreads[threadId], libros[i]);
    }

    std::cout << "\nCarga inicial por thread:" << std::endl;
    for (int tid = 0; tid < config.nThreads; tid++) {
        std::cout << "  Thread " << tid << ": " << tamanioCola(&colasThreads[tid])
                  << " libros asignados" << std::endl;
    }

    std::vector<std::atomic<int>> librosProceados(config.nThreads);
    std::vector<std::atomic<int>> tareasRobadas(config.nThreads);

    for (int i = 0; i < config.nThreads; i++) {
        librosProceados[i] = 0;
        tareasRobadas[i] = 0;
    }

    std::cout << "\n>>> Iniciando procesamiento paralelo con Work Stealing..." << std::endl;

    std::vector<std::thread> workers;

    // Crear threads workers con lógica de work stealing
    for (int tid = 0; tid < config.nThreads; tid++) {
        workers.push_back(std::thread([tid, config, &colasThreads, &librosProceados, &tareasRobadas]() {
            std::cout << "  [Thread " << tid << "] Iniciado" << std::endl;

            MapaLibro libro;
            int librosLocales = 0;
            int robosExitosos = 0;

            while (true) {
                // PASO 1: Intentar tomar trabajo de mi propia cola
                if (tomarTrabajo(&colasThreads[tid], libro)) {
                    procesarLibro(libro, tid);
                    librosLocales++;
                    librosProceados[tid]++;
                    continue;
                }

                // PASO 2: Mi cola está vacía, intentar robar de otros threads
                bool roboExitoso = false;

                // Intentar robar de cada thread (excepto de mi mismo)
                for (int victima = 0; victima < config.nThreads; victima++) {
                    if (victima == tid) continue; // No robar de mi mismo

                    if (robarTrabajo(&colasThreads[victima], libro)) {
                        // ¡Robo exitoso!
                        std::cout << "  [Thread " << tid << "] Robó trabajo del Thread "
                                  << victima << " (Libro: " << libro.nombre << ")" << std::endl;
                        procesarLibro(libro, tid);
                        librosLocales++;
                        librosProceados[tid]++;
                        robosExitosos++;
                        tareasRobadas[victima]++;
                        roboExitoso = true;
                        break; // Salir del loop de robo
                    }
                }

                // PASO 3: Si no pude robar de nadie, verificar si todos terminaron
                if (!roboExitoso) {
                    // Verificar si todas las colas están vacías (trabajo terminado)
                    bool todasVacias = true;
                    for (int i = 0; i < config.nThreads; i++) {
                        if (!colaVacia(&colasThreads[i])) {
                            todasVacias = false;
                            break;
                        }
                    }

                    if (todasVacias) {
                        // Todo el trabajo está hecho
                        std::cout << "  [Thread " << tid << "] Finalizado - Procesó "
                                  << librosLocales << " libros (" << robosExitosos
                                  << " robados)" << std::endl;
                        break;
                    }

                    // Esperar un poco antes de intentar robar de nuevo
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }));
    }

    // Esperar a que todos los threads terminen
    std::cout << "\nEsperando a que todos los threads terminen..." << std::endl;
    for (auto& worker : workers) {
        worker.join();
    }

    auto finTotal = std::chrono::system_clock::now();
    auto duracionTotal = std::chrono::duration_cast<std::chrono::milliseconds>(finTotal - inicioTotal);

    std::cout << "\n========================================" << std::endl;
    std::cout << " PROCESAMIENTO PARALELO COMPLETADO (WORK STEALING)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Tiempo total: " << duracionTotal.count() << " ms" << std::endl;
    std::cout << "  Total de libros procesados: " << libros.size() << std::endl;
    std::cout << "  Total de palabras únicas en el índice: " << indiceGlobal.size() << std::endl;
    std::cout << "  Total de registros de log: " << registrosLog.size() << std::endl;

    std::cout << "\n  Estadísticas de Work Stealing:" << std::endl;
    int totalRobos = 0;
    for (int tid = 0; tid < config.nThreads; tid++) {
        std::cout << "    Thread " << tid << ": procesó " << librosProceados[tid]
                  << " libros, le robaron " << tareasRobadas[tid] << " tareas" << std::endl;
        totalRobos += tareasRobadas[tid].load();
    }
    std::cout << "  Total de robos exitosos: " << totalRobos << std::endl;
    std::cout << "========================================" << std::endl;

    // PASO 4: Guardar índice invertido
    if (!guardarIndice(archivoSalida)) {
        std::cerr << "\nError: No se pudo guardar el índice" << std::endl;
        return 1;
    }

    // PASO 5: Guardar log de procesamiento
    std::string archivoLog = "logs/procesamiento_paralelo.log";
    if (!guardarLog(archivoLog)) {
        std::cerr << "\nError: No se pudo guardar el log" << std::endl;
        return 1;
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << " PROCESO COMPLETADO EXITOSAMENTE" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Archivos generados:" << std::endl;
    std::cout << "  - Índice: " << archivoSalida << std::endl;
    std::cout << "  - Mapa de libros: " << config.rutaMapaLibros << std::endl;
    std::cout << "  - Log: " << archivoLog << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
