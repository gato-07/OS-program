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

// Declaraciones de funciones de las opciones del menú
void adminUsuariosPerfiles();
void multiplicacionMatrices();
void juego();
void validadorPalindromos();
void calculadoraFuncion();
void contadorTexto();
void indiceInvertido();
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

void juego() {
    std::cout << "PID del proceso: " << getpid() << std::endl;
    std::cout << "=== JUEGO ===" << std::endl;
    std::cout << "\nFuncionalidad en construcción..." << std::endl;
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

std::filesystem::path getRutaEjecutable(const std::string& var) {
    const char* ruta = std::getenv(var.c_str());
    if (!ruta) return "";
    return std::filesystem::absolute(std::filesystem::path(ruta));
}

void salir() {
    std::cout << "Cerrando sesión y saliendo del sistema..." << std::endl;
}
