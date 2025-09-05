#include "menu.h"
#include "archivo.h"
#include "usuario.h"
#include "utils.h"
#include <iostream>
#include <limits>

int main(int argc, char **argv) {
    // cargar configuración y datos
    cargarVariablesEntorno();
    std::cout << "Cargando usuarios..." << std::endl;
    cargarUsuariosMemoria();

    iniciarMenu();

    // Bucle principal del menú
    while (sistemaActivo) {
        mostrarMenu();
        int opcion;

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            procesarOpcion(opcion);
        }
    }

    // Guardar cambios antes de salir
    if (listaUsuarios.enMemoria) {
        std::cout << "Guardando cambios..." << std::endl;
        guardarUsuariosArchivo();
    }

    limpiarPantalla();
    std::cout << "Gestor de usuarios cerrado exitosamente" << std::endl;
    return 0;
}
