#include "../include/autenticacion.h"
#include "../include/perfiles.h"
#include "../include/estructuras_menu.h"
#include "../include/utils.h"
#include <iostream>
#include <cstdlib>

void mostrarMenu();
void procesarOpcion(int opcion);
bool inicializarSistema(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    limpiarPantalla();  // Limpiar al inicio
    std::cout << "=== SISTEMA DE MENÚ PRINCIPAL ===" << std::endl;

    // Inicializar sistema
    if (!inicializarSistema(argc, argv)) {
        std::cerr << "Error en la inicialización del sistema" << std::endl;
        pausarPantalla();  // Pausar antes de salir por error
        return 1;
    }

    std::cout << "\nSistema inicializado correctamente" << std::endl;
    mostrarInfoSesion();
    pausarPantalla();  // Pausar después de mostrar info de sesión

    // Bucle principal del menú
    int opcion;
    do {
        mostrarMenu();
        std::cout << "\nSeleccione una opción: ";
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Error: Ingrese un número válido" << std::endl;
            pausarPantalla();  // Pausar en caso de error
            continue;
        }

        procesarOpcion(opcion);

    } while (opcion != 0);

    limpiarPantalla();  // Limpiar antes de salir
    std::cout << "\n¡Gracias por usar el sistema!" << std::endl;
    cerrarSesion();
    return 0;
}

bool inicializarSistema(int argc, char* argv[]) {
    // Cargar variables de entorno
    std::cout << "Cargando configuración..." << std::endl;
    if (!cargarVariablesEntorno()) {
        std::cout << "Advertencia: No se pudieron cargar variables de entorno" << std::endl;
        std::cout << "Usando rutas por defecto..." << std::endl;
    }

    // Procesar argumentos de línea de comandos
    if (!procesarArgumentos(argc, argv)) {
        return false;
    }

    // Cargar perfiles
    if (!cargarPerfiles()) {
        return false;
    }

    // Asignar perfil al usuario autenticado
    if (!asignarPerfilUsuario()) {
        return false;
    }

    return true;
}
