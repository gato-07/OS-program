#include "../include/autenticacion.h"
#include "../include/perfiles.h"
#include "../include/utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cctype>

// Declaraciones de funciones de las opciones del menú
void adminUsuariosPerfiles();
void multiplicacionMatrices();
void juego();
void validadorPalindromos();
void calculadoraFuncion();
void contadorTexto();
void salir();

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
    {0, "Salir", salir}
};

const int NUM_OPCIONES = sizeof(opcionesMenu) / sizeof(OpcionMenu);

void mostrarMenu() {
    limpiarPantalla();
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
    // Buscar la opción en el array
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

    // Verificar permisos (excepto para salir)
    if (opcion != 0 && !validarOpcionPermitida(opcion)) {
        std::cout << "Error: No tiene permisos para acceder a esta opción" << std::endl;
        pausarPantalla();
        return;
    }

    // Limpiar pantalla antes de ejecutar la opción
    limpiarPantalla();

    // Ejecutar la función correspondiente
    if (opcionSeleccionada->funcion != nullptr) {
        std::cout << "\n--- " << opcionSeleccionada->descripcion << " ---" << std::endl;
        opcionSeleccionada->funcion();
    }
}

// Implementaciones de las funciones del menú
void adminUsuariosPerfiles() {
    std::cout << "=== ADMINISTRACIÓN DE USUARIOS Y PERFILES ===" << std::endl;
    std::cout << "Esta opción solo está disponible para usuarios con perfil ADMIN" << std::endl;
    std::cout << "\nFuncionalidad en construcción..." << std::endl;
    pausarPantalla();
}

void multiplicacionMatrices() {
    std::cout << "=== MULTIPLICACIÓN DE MATRICES NxN ===" << std::endl;
    std::cout << "Esta funcionalidad está disponible como programa independiente." << std::endl;
    std::cout << "\nUse: ./multiplicador_matrices/bin/multiplicador_matrices archivo_a archivo_b separador" << std::endl;
    std::cout << "\nFuncionalidad en construcción..." << std::endl;
    pausarPantalla();
}

void juego() {
    std::cout << "=== JUEGO ===" << std::endl;
    std::cout << "\nFuncionalidad en construcción..." << std::endl;
    pausarPantalla();
}

void validadorPalindromos() {
    bool continuar = true;

    while (continuar) {
        limpiarPantalla();
        std::string texto;
        char opcion;

        std::cout << "\n=== VALIDADOR DE PALÍNDROMOS ===" << std::endl;
        std::cout << "Ingrese el texto a validar: ";
        std::cin.ignore(); // Limpiar buffer
        std::getline(std::cin, texto);

        if (texto.empty()) {
            std::cout << "Error: No se ingresó ningún texto" << std::endl;
            pausarPantalla();
            continue;
        }

        std::cout << "\nTexto ingresado: '" << texto << "'" << std::endl;
        std::cout << "\nOpciones disponibles:" << std::endl;
        std::cout << "(v) Validar" << std::endl;
        std::cout << "(c) Cancelar" << std::endl;
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        if (opcion == 'v' || opcion == 'V') {
            bool resultado = esPalindromo(texto);

            std::cout << "\n" << std::string(50, '-') << std::endl;
            std::cout << "RESULTADO DE VALIDACIÓN:" << std::endl;
            std::cout << "Texto: '" << texto << "'" << std::endl;

            if (resultado) {
                std::cout << "✓ SÍ es un palíndromo" << std::endl;
            } else {
                std::cout << "✗ NO es un palíndromo" << std::endl;
            }
            std::cout << std::string(50, '-') << std::endl;

        } else if (opcion == 'c' || opcion == 'C') {
            std::cout << "Operación cancelada" << std::endl;
        } else {
            std::cout << "Opción no válida" << std::endl;
            pausarPantalla();
            continue;
        }

        // Preguntar si desea continuar
        continuar = mostrarOpcionVolver();
    }

    std::cout << "Volviendo al menú principal..." << std::endl;
    pausarPantalla();
}

void calculadoraFuncion() {
    bool continuar = true;

    while (continuar) {
        limpiarPantalla();
        double x, resultado;

        std::cout << "\n=== CALCULADORA f(x) = x² + 2x + 8 ===" << std::endl;
        std::cout << "Esta función calcula el valor de f(x) = x² + 2x + 8 para números reales" << std::endl;
        std::cout << "\nIngrese el valor de x: ";

        if (!leerNumeroReal(x)) {
            pausarPantalla();
            continue;
        }

        // Calcular f(x) = x² + 2x + 8
        resultado = x * x + 2 * x + 8;

        // Mostrar resultado con especificación detallada
        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "CÁLCULO DE LA FUNCIÓN:" << std::endl;
        std::cout << "f(x) = x² + 2x + 8" << std::endl;
        std::cout << "f(" << std::fixed << std::setprecision(2) << x << ") = "
                  << "(" << x << ")² + 2(" << x << ") + 8" << std::endl;
        std::cout << "f(" << x << ") = " << (x*x) << " + " << (2*x) << " + 8" << std::endl;
        std::cout << "f(" << x << ") = " << std::setprecision(4) << resultado << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        // Preguntar si desea continuar (opción VOLVER)
        continuar = mostrarOpcionVolver();
    }

    std::cout << "Volviendo al menú principal..." << std::endl;
    pausarPantalla();
}

void contadorTexto() {
    bool continuar = true;

    while (continuar) {
        limpiarPantalla();
        std::string texto;
        int vocales, consonantes, especiales, palabras;

        std::cout << "\n=== CONTADOR DE TEXTO ===" << std::endl;
        std::cout << "Esta función analiza un texto y cuenta diferentes tipos de caracteres" << std::endl;

        // Verificar si hay archivo de trabajo
        if (!sesion.archivoTrabajo.empty()) {
            std::cout << "\nArchivo de trabajo disponible: " << sesion.archivoTrabajo << std::endl;
            std::cout << "¿Desea usar el archivo de trabajo? (s/n): ";
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
                    std::cout << "Archivo cargado exitosamente." << std::endl;
                } else {
                    std::cout << "Error: No se pudo abrir el archivo." << std::endl;
                    std::cout << "\nIngrese el texto a analizar manualmente: ";
                    std::getline(std::cin, texto);
                }
            } else {
                std::cout << "\nIngrese el texto a analizar: ";
                std::getline(std::cin, texto);
            }
        } else {
            std::cout << "\nIngrese el texto a analizar: ";
            std::cin.ignore(); // Limpiar buffer
            std::getline(std::cin, texto);
        }

        if (texto.empty()) {
            std::cout << "Error: No se ingresó ningún texto" << std::endl;
            pausarPantalla();
            continue;
        }

        // Analizar el texto
        analizarTexto(texto, vocales, consonantes, especiales, palabras);

        // Mostrar resumen de conteo
        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "RESUMEN DE CONTEO:" << std::endl;

        // Mostrar solo los primeros 100 caracteres del texto si es muy largo
        if (texto.length() > 100) {
            std::cout << "Texto analizado: '" << texto.substr(0, 100) << "...'" << std::endl;
        } else {
            std::cout << "Texto analizado: '" << texto << "'" << std::endl;
        }

        std::cout << "Longitud total: " << texto.length() << " caracteres" << std::endl;
        std::cout << std::string(30, '-') << std::endl;
        std::cout << "• Cantidad de vocales: " << vocales << std::endl;
        std::cout << "• Cantidad de consonantes: " << consonantes << std::endl;
        std::cout << "• Cantidad de caracteres especiales: " << especiales << std::endl;
        std::cout << "• Cantidad de palabras: " << palabras << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        // Preguntar si desea continuar (opción VOLVER)
        continuar = mostrarOpcionVolver();
    }

    std::cout << "Volviendo al menú principal..." << std::endl;
    pausarPantalla();
}

void salir() {
    std::cout << "Cerrando sesión y saliendo del sistema..." << std::endl;
    // No necesita pausarPantalla porque el programa termina
}
