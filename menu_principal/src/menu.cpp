#include "../include/autenticacion.h"
#include "../include/perfiles.h"
#include "../include/utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>

// Declaraciones de funciones de las opciones del menú
void adminUsuariosPerfiles();
void multiplicacionMatrices();
void juego();
void validadorPalindromos();
void calculadoraFuncion();
void contadorTexto();
void indiceInvertido();
void indiceInvertidoParalelo();
void juegoHOST();
void juegoCLIENT();
void juego();
void graficaParalela();
void buscadorLibros();
void salir();
std::filesystem::path getRutaEjecutable(const std::string& var);

// Funciones auxiliares declaradas en funciones_menu.cpp
extern bool esPalindromo(const std::string& texto);
extern void analizarTexto(const std::string& texto, int& vocales, int& consonantes,
                         int& especiales, int& palabras);
extern bool mostrarOpcionVolver();
extern bool leerNumeroReal(double& numero);

struct OpcionMenu {
    int id;
    std::string descripcion;
    void (*funcion)();
};

// Definir las opciones del menú
OpcionMenu opcionesMenu[] = {
    {1, "Admin de usuarios y perfiles", adminUsuariosPerfiles},
    {2, "Multiplicación de matrices NxN", multiplicacionMatrices},
    {3, "Juego", juego},
    {4, "¿Es palíndromo?", validadorPalindromos},
    {5, "Calcular f(x)=x²+2x+8", calculadoraFuncion},
    {6, "Contador de texto", contadorTexto},
    {7, "Crear indice invertido", indiceInvertido},
    {8, "Crear indice invertido paralelo", indiceInvertidoParalelo},
    {9, "juego", juego},
    {10, "Graficar rendimiento ind. inv. paralelo", graficaParalela},
    {11, "Buscador de libros (Cache + Motor)", buscadorLibros},
    {0, "Salir", salir}
};

const int NUM_OPCIONES = sizeof(opcionesMenu) / sizeof(OpcionMenu);

void mostrarMenu() {
    limpiarPantalla();
    std::cout << "PID del proceso: " << getpid() << std::endl;
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "           MENÚ PRINCIPAL DEL SISTEMA" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Usuario: " << sesion.usuarioActual.username
              << " | Perfil: " << sesion.perfilActual.nombre << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Mostrar solo las opciones permitidas para el perfil actual
    for (int i = 0; i < NUM_OPCIONES; i++) {
        OpcionMenu& opcion = opcionesMenu[i];

        // La opción 0 (Salir) siempre está disponible
        if (opcion.id == 0 || validarOpcionPermitida(opcion.id)) {
            std::cout << "(" << opcion.id << ") " << opcion.descripcion << std::endl;
        }
    }
    std::cout << std::string(60, '=') << std::endl;
}

void procesarOpcion(int opcion) {
    OpcionMenu* opcionSeleccionada = nullptr;
    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (opcionesMenu[i].id == opcion) {
            opcionSeleccionada = &opcionesMenu[i];
            break;
        }
    }

    if (opcionSeleccionada == nullptr) {
        std::cout << "Error: Opción no válida" << std::endl;
        pausarPantalla();
        return;
    }

    if (opcion != 0 && !validarOpcionPermitida(opcion)) {
        std::cout << "Error: No tiene permisos para acceder a esta opción" << std::endl;
        pausarPantalla();
        return;
    }

    limpiarPantalla();

    if (opcionSeleccionada->funcion != nullptr) {
        std::cout << "\n--- " << opcionSeleccionada->descripcion << " ---" << std::endl;
        opcionSeleccionada->funcion();
    }
}

// === Implementaciones de las funciones del menú ===

void adminUsuariosPerfiles() {
    const char* ruta = std::getenv("ADMIN_SYS");
    std::cout << "PID del proceso: " << getpid() << std::endl;
    if (!ruta) {
        std::cerr << "Variable de entorno ADMIN_SYS no definida\n";
        return;
    }

    std::filesystem::path fullPath = std::filesystem::path(ruta);
    if (!std::filesystem::exists(fullPath)) {
        std::cerr << "Error: ejecutable no encontrado en " << fullPath << "\n";
        return;
    }

    std::cout << "Ejecutando: " << fullPath << "\n";
    int result = system(fullPath.string().c_str());
    std::cout << "El ejecutable terminó con código: " << result << "\n";
    std::cout << "Volviendo al menú principal...\n";
    pausarPantalla();
}

void multiplicacionMatrices() {
    const char* ruta = std::getenv("MULTI_M");
    std::cout << "PID del proceso: " << getpid() << std::endl;
    if (!ruta) {
        std::cerr << "Variable de entorno MULTI_M no definida\n";
        return;
    }

    std::filesystem::path fullPath = std::filesystem::path(ruta);
    if (!std::filesystem::exists(fullPath)) {
        std::cerr << "Error: ejecutable no encontrado en " << fullPath << "\n";
        return;
    }

    // Preguntar matrices al usuario
    std::string matrizA, matrizB, separador;
    std::cout << "Indique el archivo de la primera matriz (ej: ../data/matrices/matrizA.txt): ";
    std::cin >> matrizA;

    std::cout << "Indique el archivo de la segunda matriz (ej: ../data/matrices/matrizB.txt): ";
    std::cin >> matrizB;

    std::cout << "Indique el separador usado en los archivos (ej: , o ; o espacio): ";
    std::cin >> separador;

    // Armar comando
    std::string command = fullPath.string() + " " + matrizA + " " + matrizB + " " + separador;

    std::cout << "\nEjecutando: " << command << "\n";

    // Decidir si lanzamos la ejecucion o volvemos al menu principal
    char opcion;
    std::cout << "¿Desea ejecutar la multiplicación de matrices? (s/n): ";
    std::cin >> opcion;
    if (opcion != 's' && opcion != 'S') {
        std::cout << "Operación cancelada. Volviendo al menú principal...\n";
        pausarPantalla();
        return;
    }
    int result = system(command.c_str());

    std::cout << "\nEl ejecutable terminó con código: " << result << "\n";
    std::cout << "Volviendo al menú principal...\n";
    pausarPantalla();
}


void validadorPalindromos() {
    bool continuar = true;
    while (continuar) {
        limpiarPantalla();
        std::cout << "PID del proceso: " << getpid() << std::endl;
        std::string texto;
        char opcion;

        std::cout << "\n=== VALIDADOR DE PALÍNDROMOS ===" << std::endl;
        std::cout << "Ingrese el texto a validar: ";
        std::cin.ignore();
        std::getline(std::cin, texto);

        if (texto.empty()) {
            std::cout << "Error: No se ingresó ningún texto" << std::endl;
            pausarPantalla();
            continue;
        }

        std::cout << "\nTexto ingresado: '" << texto << "'" << std::endl;
        std::cout << "(v) Validar | (c) Cancelar\nSeleccione una opción: ";
        std::cin >> opcion;

        if (opcion == 'v' || opcion == 'V') {
            bool resultado = esPalindromo(texto);
            std::cout << (resultado ? "✓ SÍ es un palíndromo" : "✗ NO es un palíndromo") << std::endl;
        } else if (opcion == 'c' || opcion == 'C') {
            std::cout << "Operación cancelada" << std::endl;
        } else {
            std::cout << "Opción no válida" << std::endl;
            pausarPantalla();
            continue;
        }

        continuar = mostrarOpcionVolver();
    }
    std::cout << "Volviendo al menú principal..." << std::endl;
    pausarPantalla();
}

void calculadoraFuncion() {
    bool continuar = true;
    while (continuar) {
        limpiarPantalla();
        std::cout << "PID del proceso: " << getpid() << std::endl;
        double x, resultado;

        std::cout << "\n=== CALCULADORA f(x) = x² + 2x + 8 ===" << std::endl;
        std::cout << "Ingrese el valor de x: ";

        if (!leerNumeroReal(x)) {
            pausarPantalla();
            continue;
        }

        resultado = x * x + 2 * x + 8;
        std::cout << "f(" << x << ") = " << resultado << std::endl;

        continuar = mostrarOpcionVolver();
    }
    pausarPantalla();
}

void contadorTexto() {
    bool continuar = true;
    while (continuar) {
        limpiarPantalla();
        std::cout << "PID del proceso: " << getpid() << std::endl;
        std::string texto;
        int vocales, consonantes, especiales, palabras;

        std::cout << "\n=== CONTADOR DE TEXTO ===" << std::endl;

        if (!sesion.archivoTrabajo.empty()) {
            std::cout << "Archivo de trabajo: " << sesion.archivoTrabajo << std::endl;
            std::cout << "¿Desea usarlo? (s/n): ";
            char usar;
            std::cin >> usar;
            std::cin.ignore();

            if (usar == 's' || usar == 'S') {
                std::ifstream archivo(sesion.archivoTrabajo);
                if (archivo.is_open()) {
                    std::string linea;
                    texto = "";
                    while (std::getline(archivo, linea)) {
                        texto += linea + " ";
                    }
                    archivo.close();
                } else {
                    std::cout << "Error al abrir el archivo. Ingrese texto manual: ";
                    std::getline(std::cin, texto);
                }
            } else {
                std::cout << "Ingrese texto: ";
                std::getline(std::cin, texto);
            }
        } else {
            std::cout << "Ingrese texto: ";
            std::cin.ignore();
            std::getline(std::cin, texto);
        }

        if (texto.empty()) {
            std::cout << "Texto vacío" << std::endl;
            pausarPantalla();
            continue;
        }

        analizarTexto(texto, vocales, consonantes, especiales, palabras);

        std::cout << "• Vocales: " << vocales
                  << " • Consonantes: " << consonantes
                  << " • Especiales: " << especiales
                  << " • Palabras: " << palabras << std::endl;

        continuar = mostrarOpcionVolver();
    }
    pausarPantalla();
}

void indiceInvertido() {
    bool continuar = true;
    while (continuar) {
        limpiarPantalla();
        std::string nombreArchivo, carpetaLibros;
        std::cout << "PID del proceso: " << getpid() << std::endl;

        std::cout << "\n === Crear Indice Invertido ===" << std::endl;
        std::cout << "Ingrese el nombre del archivo (.idx): ";
        std::cin.ignore();
        std::getline(std::cin, nombreArchivo);

        if (nombreArchivo.size() < 4 || nombreArchivo.substr(nombreArchivo.size() - 4) != ".idx") {
            std::cout << "Error: debe terminar en .idx" << std::endl;
            pausarPantalla();
            continue;
        }

        std::cout << "Ingrese carpeta de libros: ";
        std::getline(std::cin, carpetaLibros);

        if (carpetaLibros.empty()) {
            std::cout << "Error: path vacío" << std::endl;
            pausarPantalla();
            continue;
        }

        const char* programa = std::getenv("CREATE_INDEX");
        if (!programa) {
            std::cout << "Error: CREATE_INDEX no definida" << std::endl;
            pausarPantalla();
            return;
        }

        std::string comando = std::string(programa) + " " + nombreArchivo + " " + carpetaLibros;
        std::cout << "Ejecutando: " << comando << std::endl;

        int resultado = system(comando.c_str());
        if (resultado == 0) std::cout << "Indice creado correctamente\n";
        else std::cout << "Error al ejecutar el programa externo\n";

        continuar = mostrarOpcionVolver();
    }
    pausarPantalla();
}

void indiceInvertidoParalelo() {
    bool continuar = true;
    while (continuar) {
        limpiarPantalla();
        std::string nombreArchivo, carpetaLibros;
        std::cout << "PID del proceso: " << getpid() << std::endl;

        std::cout << "\n === Crear Indice Invertido PARALELO ===" << std::endl;
        std::cout << "Esta versión usa procesamiento paralelo con threads" << std::endl;

        // Mostrar configuración actual
        const char* nThreads = std::getenv("N_THREADS");
        const char* nLote = std::getenv("N_LOTE");
        const char* mapaLibros = std::getenv("MAPA_LIBROS");

        std::cout << "\nConfiguración actual:" << std::endl;
        std::cout << "  N_THREADS: " << (nThreads ? nThreads : "4 (default)") << std::endl;
        std::cout << "  N_LOTE: " << (nLote ? nLote : "10 (default)") << std::endl;
        std::cout << "  MAPA_LIBROS: " << (mapaLibros ? mapaLibros : "data/mapa_libros.txt (default)") << std::endl;

        std::cout << "\nIngrese el nombre del archivo (.idx): ";
        std::cin.ignore();
        std::getline(std::cin, nombreArchivo);

        if (nombreArchivo.size() < 4 || nombreArchivo.substr(nombreArchivo.size() - 4) != ".idx") {
            std::cout << "Error: debe terminar en .idx" << std::endl;
            pausarPantalla();
            continue;
        }

        std::cout << "Ingrese carpeta de libros: ";
        std::getline(std::cin, carpetaLibros);

        if (carpetaLibros.empty()) {
            std::cout << "Error: path vacío" << std::endl;
            pausarPantalla();
            continue;
        }

        const char* programa = std::getenv("INDICE_INVERT_PARALELO");
        if (!programa) {
            std::cout << "Error: INDICE_INVERT_PARALELO no definida" << std::endl;
            pausarPantalla();
            return;
        }

        std::string comando = std::string(programa) + " " + nombreArchivo + " " + carpetaLibros;
        std::cout << "\nEjecutando: " << comando << std::endl;

        int resultado = system(comando.c_str());
        if (resultado == 0) {
            std::cout << "\n  Indice paralelo creado correctamente" << std::endl;
            std::cout << "  - Archivo índice: " << nombreArchivo << std::endl;
            std::cout << "  - Mapa de libros: " << (mapaLibros ? mapaLibros : "data/mapa_libros.txt") << std::endl;
        } else {
            std::cout << "\n Error al ejecutar el programa externo (código: " << resultado << ")" << std::endl;
        }

        continuar = mostrarOpcionVolver();
    }
    pausarPantalla();
}

void juegoHOST() {
    std::cout << "Lanzando servidor de juego en nueva ventana..." << std::endl;
#ifdef _WIN32
    // Windows: abre nueva ventana de cmd y ejecuta el servidor
    int rc = system("start cmd /k "cd ../juego && python server.py"");
#else
    // Bash/Linux/macOS: abre nueva terminal (gnome-terminal, xterm, konsole, etc.)
    // Intenta usar la terminal más robusta primero (xterm) y tiene fallbacks.
    int rc = system(
        "cd ../juego && ( "
        "xterm -e 'python3 server.py; echo \"Servidor cerrado. Presiona Enter para cerrar esta ventana.\"; read' || "
        "gnome-terminal -- bash -c 'python3 server.py; echo \"Servidor cerrado. Presiona Enter para cerrar esta ventana.\"; exec bash' || "
        "konsole --noclose -e bash -c 'python3 server.py; echo \"Servidor cerrado. Presiona Enter para cerrar esta ventana.\"; read' || "
        " (echo \"Error: No se encontró una terminal gráfica compatible (xterm, gnome-terminal, konsole).\" && exit 1) "
        ")"
    );
#endif
    if (rc != 0) {
        std::cerr << "Error al lanzar el servidor (código " << rc << ")" << std::endl;
    } else {
        std::cout << "Servidor lanzado en nueva ventana." << std::endl;
    }
    pausarPantalla();
}

void juegoCLIENT() {
    std::cout << "Lanzando cliente de juego..." << std::endl;
    int rc = system("cd ../juego && python3 client.py");
    if (rc != 0) {
        std::cerr << "Error al lanzar el cliente (código " << rc << ")" << std::endl;
    }
    pausarPantalla();
}

void juegoPLOTS() {
    std::cout << "Lanzando script de graficación de juego..." << std::endl;
    int rc = system("cd ../juego && python3 statPlot.py");
    if (rc != 0) {
        std::cerr << "Error al lanzar la graficación (código " << rc << ")" << std::endl;
    }
    pausarPantalla();
}

void juego() {
    while (true) {
        limpiarPantalla();
        std::cout << "=== JUEGO ===" << std::endl;
        std::cout << "1) Hostear partida (servidor)" << std::endl;
        std::cout << "2) Unirse como cliente" << std::endl;
        std::cout << "3) Graficar partida por ID" << std::endl;
        std::cout << "0) Volver al menú principal" << std::endl;
        std::cout << "Seleccione una opción: ";
        int op = 0;
        std::cin >> op;
        switch (op) {
            case 1:
                juegoHOST();
                break;
            case 2:
                juegoCLIENT();
                break;
            case 3:
                juegoPLOTS();
                break;
            case 0:
                return;
            default:
                std::cout << "Opción no válida." << std::endl;
                pausarPantalla();
        }
    }
}

void graficaParalela (){
    bool continuar = true;
    while (continuar) {

        limpiarPantalla();
        std::cout << "PID del proceso: " << getpid() << std::endl;
        std::cout << "\n === Grafica Rendimiento (Benchmark) ===" << std::endl;
        
        std::string carpetaLibros;
        std::string listaHilos;

        // 1. Mostrar límite configurado en el .env 
        const char* maxTests = std::getenv("MAX_BENCHMARK_TESTS");
        std::cout << "\n[INFO] Límite de pruebas simultáneas (.env): " 
                  << (maxTests ? maxTests : "5 (default)") << std::endl;

        // 2. Pedir Carpeta
        std::cout << "\nIngrese carpeta de libros: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar buffer
        std::getline(std::cin, carpetaLibros);

        if (carpetaLibros.empty()) {
            std::cout << "Error: path vacío" << std::endl;
            pausarPantalla();
            continue;
        }

        // 3. Pedir Hilos 
        std::cout << "Ingrese los hilos a probar separados por espacio (Ej: 1 2 4 8): ";
        std::getline(std::cin, listaHilos);

        if (listaHilos.empty()) {
            std::cout << "Error: lista de hilos vacía" << std::endl;
            pausarPantalla();
            continue;
        }

        std::cout << "\nIniciando Benchmark..." << std::endl;
        
        const char* programa = std::getenv("BENCHMARK_RENDIMIENTO");
        if (!programa) {
            std::cout << "Error: BENCHMARK_RENDIMIENTO no definida en .env" << std::endl;
            pausarPantalla();
            return;
        }

        // ./benchmark "carpeta" "1 2 4 8"
        std::string comando = std::string(programa) + " \"" + carpetaLibros + "\" \"" + listaHilos + "\"";
        
        std::cout << "Ejecutando: " << comando << std::endl;

        int resultado = system(comando.c_str());        
        if (resultado != 0) {
            std::cerr << "Error al ejecutar el programa de benchmark." << std::endl;
        }

        continuar = mostrarOpcionVolver();
    }
}
    


void buscadorLibros() {
    while (true) {
        limpiarPantalla();
        std::cout << "PID del proceso: " << getpid() << std::endl;
        std::cout << "\n=== BUSCADOR DE LIBROS (CACHE + MOTOR) ===" << std::endl;
        std::cout << "Sistema de búsqueda con caché FIFO\n" << std::endl;
        std::cout << "1) Iniciar Cache (Puerto 8001)" << std::endl;
        std::cout << "2) Iniciar Motor (Puerto 8002)" << std::endl;
        std::cout << "3) Ejecutar Buscador (Cliente)" << std::endl;
        std::cout << "4) Iniciar todo el sistema (Cache + Motor + Buscador)" << std::endl;
        std::cout << "0) Volver al menú principal" << std::endl;
        std::cout << "\nSeleccione una opción: ";
        
        int opcion;
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opción inválida" << std::endl;
            pausarPantalla();
            continue;
        }
        
        switch (opcion) {
            case 1: {
                // Iniciar Cache
                std::cout << "\nIniciando servidor de Cache en nueva ventana..." << std::endl;
                const char* cacheExec = std::getenv("CACHE_SERVIDOR");
                if (!cacheExec) {
                    std::cerr << "ERROR: Variable CACHE_SERVIDOR no definida en .env" << std::endl;
                    pausarPantalla();
                    break;
                }
                
                std::string comando = "xterm -e 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; read' || "
                                     "gnome-terminal -- bash -c 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; exec bash' || "
                                     "konsole --noclose -e bash -c 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; read' &";
                
                int rc = system(comando.c_str());
                if (rc == 0) {
                    std::cout << "✓ Servidor de Cache lanzado en nueva ventana (Puerto 8001)" << std::endl;
                } else {
                    std::cerr << "✗ Error al lanzar el servidor de Cache" << std::endl;
                }
                pausarPantalla();
                break;
            }
            
            case 2: {
                // Iniciar Motor
                std::cout << "\nIniciando servidor Motor en nueva ventana..." << std::endl;
                const char* motorExec = std::getenv("MOTOR_SERVIDOR");
                const char* indiceFile = std::getenv("INDICE_FILE");
                
                if (!motorExec || !indiceFile) {
                    std::cerr << "ERROR: Variables no definidas en .env (MOTOR_SERVIDOR, INDICE_FILE)" << std::endl;
                    pausarPantalla();
                    break;
                }
                
                std::string comando = "xterm -e 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; read' || "
                                     "gnome-terminal -- bash -c 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; exec bash' || "
                                     "konsole --noclose -e bash -c 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; read' &";
                
                int rc = system(comando.c_str());
                if (rc == 0) {
                    std::cout << "✓ Servidor Motor lanzado en nueva ventana (Puerto 8002)" << std::endl;
                } else {
                    std::cerr << "✗ Error al lanzar el servidor Motor" << std::endl;
                }
                pausarPantalla();
                break;
            }
            
            case 3: {
                // Ejecutar Buscador
                std::cout << "\nEjecutando Buscador (Cliente)..." << std::endl;
                std::cout << "Nota: Asegúrate de que Cache y Motor estén ejecutándose\n" << std::endl;
                
                const char* buscadorExec = std::getenv("BUSCADOR_CLIENTE");
                if (!buscadorExec) {
                    std::cerr << "ERROR: Variable BUSCADOR_CLIENTE no definida en .env" << std::endl;
                    pausarPantalla();
                    break;
                }
                
                std::string comando = "cd ../buscador && " + std::string(buscadorExec);
                int rc = system(comando.c_str());
                
                if (rc != 0) {
                    std::cerr << "\n✗ Error al ejecutar el buscador (código: " << rc << ")" << std::endl;
                }
                pausarPantalla();
                break;
            }
            
            case 4: {
                // Iniciar todo el sistema
                std::cout << "\nIniciando sistema completo (Cache + Motor + Buscador)..." << std::endl;
                std::cout << "Se abrirán 2 ventanas nuevas para Cache y Motor\n" << std::endl;
                
                const char* cacheExec = std::getenv("CACHE_SERVIDOR");
                const char* motorExec = std::getenv("MOTOR_SERVIDOR");
                const char* buscadorExec = std::getenv("BUSCADOR_CLIENTE");
                const char* indiceFile = std::getenv("INDICE_FILE");
                
                if (!cacheExec || !motorExec || !buscadorExec || !indiceFile) {
                    std::cerr << "ERROR: Variables de entorno no definidas en .env" << std::endl;
                    pausarPantalla();
                    break;
                }
                
                // Lanzar Cache
                std::string cmdCache = "xterm -e 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; read' || "
                                      "gnome-terminal -- bash -c 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; exec bash' || "
                                      "konsole --noclose -e bash -c 'cd ../cache && " + std::string(cacheExec) + " 100; echo \"Cache cerrado. Presiona Enter.\"; read' &";
                system(cmdCache.c_str());
                std::cout << "  [1/3] Cache iniciado (Puerto 8001)" << std::endl;
                std::cout << "  Esperando 2 segundos..." << std::endl;
                sleep(2);
                
                // Lanzar Motor
                std::string cmdMotor = "xterm -e 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; read' || "
                                      "gnome-terminal -- bash -c 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; exec bash' || "
                                      "konsole --noclose -e bash -c 'cd ../motor && " + std::string(motorExec) + " " + std::string(indiceFile) + "; echo \"Motor cerrado. Presiona Enter.\"; read' &";
                system(cmdMotor.c_str());
                std::cout << "  [2/3] Motor iniciado (Puerto 8002)" << std::endl;
                std::cout << "  Esperando a que el motor cargue el índice (5 segundos)..." << std::endl;
                sleep(5);
                
                // Ejecutar Buscador
                std::cout << "  [3/3] Iniciando Buscador..." << std::endl;
                std::string cmdBuscador = "cd ../buscador && " + std::string(buscadorExec);
                system(cmdBuscador.c_str());
                
                pausarPantalla();
                break;
            }
            
            case 0:
                return;
                
            default:
                std::cout << "Opción no válida" << std::endl;
                pausarPantalla();
        }
    }
}

std::filesystem::path getRutaEjecutable(const std::string& var) {
    const char* ruta = std::getenv(var.c_str());
    if (!ruta) return "";
    return std::filesystem::absolute(std::filesystem::path(ruta));
    pausarPantalla();
}

void salir() {
    std::cout << "Cerrando sesión y saliendo del sistema..." << std::endl;
}
