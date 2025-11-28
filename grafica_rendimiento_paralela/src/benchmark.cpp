#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

// --- CONFIGURACIÓN ---
const std::string ARCHIVO_SALIDA_TEMP = "temp_benchmark.idx";
const std::string LOG_TIEMPOS = "benchmark_resultados.log"; 
const int DEFAULT_MAX_TESTS = 5;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::vector<int> parsearHilosInput(std::string str) {
    std::vector<int> numeros;
    std::stringstream ss(str);
    int num;
    while (ss >> num) {
        if (num > 0) numeros.push_back(num);
    }
    return numeros;
}

void cargarEnv(const std::string& rutaEnv) {
    std::ifstream file(rutaEnv);
    if (!file.is_open()) return;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            setenv(key.c_str(), value.c_str(), 0);
        }
    }
}

bool inicializarLog(const std::string& rutaArchivo) {
    fs::path rutaCompleta(rutaArchivo);
    if (rutaCompleta.has_parent_path()) {
        fs::path directorio = rutaCompleta.parent_path();
        if (!fs::exists(directorio)) {
            try { fs::create_directories(directorio); } 
            catch (...) { return false; }
        }
    }
    std::ofstream archivo(rutaArchivo, std::ofstream::trunc);
    if (!archivo.is_open()) return false;
    archivo.close();
    return true;
}

int main(int argc, char* argv[]) {
    std::cout << "==================================================" << std::endl;
    std::cout << "     INICIANDO BENCHMARK DE RENDIMIENTO" << std::endl;
    std::cout << "==================================================" << std::endl;

    // --- 1. VALIDACIÓN DE ARGUMENTOS ---
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <carpeta_libros> <lista_hilos>" << std::endl;
        std::cerr << "Ejemplo: " << argv[0] << " ../data/libros \"1 2 4 8\"" << std::endl;
        return 1;
    }
    
    std::string carpeta_libros = argv[1];
    std::string input_hilos = argv[2]; 

    // --- 2. CARGAR CONFIGURACIÓN ---
    if (fs::exists(".env")) cargarEnv(".env");
    else if (fs::exists("../.env")) cargarEnv("../.env");

    const char* env_exe = std::getenv("INDICE_INVERT_PARALELO");
    std::string ruta_ejecutable = env_exe ? env_exe : ""; 

    if (ruta_ejecutable.empty() || !fs::exists(ruta_ejecutable)) {
        std::cerr << "[ERROR FATAL] Ejecutable no encontrado: " << ruta_ejecutable << std::endl;
        return 1;
    }
    if (!fs::exists(carpeta_libros)) {
        std::cerr << "[ERROR FATAL] Carpeta de libros no encontrada: " << carpeta_libros << std::endl;
        return 1;
    }

    // --- 3. PROCESAR HILOS Y APLICAR LÍMITE ---
    
    const char* env_limit = std::getenv("MAX_BENCHMARK_TESTS");
    int max_tests = env_limit ? std::atoi(env_limit) : DEFAULT_MAX_TESTS;
    if (max_tests <= 0) max_tests = DEFAULT_MAX_TESTS;

    std::cout << "  [CONF] Límite de pruebas (.env): " << max_tests << std::endl;

    std::vector<int> cant_threads = parsearHilosInput(input_hilos);

    if (cant_threads.empty()) {
        std::cerr << "[ERROR] No ingresaste números válidos de hilos." << std::endl;
        return 1;
    }

    if (cant_threads.size() > (size_t)max_tests) {
        std::cout << "  [WARN] Ingresaste " << cant_threads.size() << " configuraciones, pero el límite es " << max_tests << "." << std::endl;
        std::cout << "  -> Recortando la lista a los primeros " << max_tests << "." << std::endl;
        cant_threads.resize(max_tests);
    }

    std::cout << "  [INFO] Se ejecutarán pruebas con: ";
    for (int t : cant_threads) std::cout << t << " ";
    std::cout << "hilos." << std::endl;


    // --- 4. PREPARAR LOG ---
    if (!inicializarLog(LOG_TIEMPOS)) return 1;
    std::string log_absoluto = fs::absolute(LOG_TIEMPOS).string();
    setenv("BENCHMARK_OUTPUT", log_absoluto.c_str(), 1);


    // --- 5. EJECUTAR PRUEBAS ---
    for (int n_hilos : cant_threads) {
        std::cout << "\n>>> Probando con " << n_hilos << " threads... ";
        std::cout.flush();

        std::string nStr = std::to_string(n_hilos);
        setenv("N_THREADS", nStr.c_str(), 1);

        std::string comando = ruta_ejecutable + " " + ARCHIVO_SALIDA_TEMP + " \"" + carpeta_libros + "\"";
        
        int retorno = system((comando + " > /dev/null").c_str());

        if (retorno != 0) {
            std::cerr << "\n[FALLO] Código de error: " << retorno << std::endl;
        } else {
            std::cout << "[OK]";
        }
    }

    unsetenv("BENCHMARK_OUTPUT");
    unsetenv("N_THREADS");


    // --- 6. GRAFICAR ---
    std::cout << "\n\n==================================================" << std::endl;
    std::cout << "     GENERANDO GRÁFICO" << std::endl;
    std::cout << "==================================================" << std::endl;

    const char* env_py = std::getenv("GRAPHIC_THREADS");
    std::string script_py_final = env_py ? env_py : "";

    if (script_py_final.empty() || !fs::exists(script_py_final)) {
         std::cerr << "[ERROR] Script Python no encontrado: " << script_py_final << std::endl;
         return 1;
    }

    std::string comandoPy = "python3 " + script_py_final + " " + LOG_TIEMPOS;
    int retPy = system(comandoPy.c_str());

    if (retPy == 0) {
        std::cout << "\n[EXITO] Gráfico guardado: grafico_rendimiento.png" << std::endl;
    } else {
        std::cerr << "\n[ERROR] Falló Python." << std::endl;
    }

    return 0;
}