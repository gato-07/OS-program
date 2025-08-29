#include "menu.h"
#include "usuario.h"
#include "utils.h"
#include <iostream>

std::vector<OpcionMenu> opcionesMenu;
bool sistemaActivo = true;

void iniciarMenu() {
    opcionesMenu = {
        {1, "Crear usuario", crearUsuario, true},
        {2, "Listar usuarios", listarUsuarios, true},
        {3, "Eliminar usuario", eliminarUsuario, true},
        {0, "Salir", salir, true}
    };
    std::cout << "Todos los permisos por ahora" << std::endl;
}

void mostrarMenu() {
    limpiarPantalla();
    std::cout << "Gestor de Usuarios:" << std::endl;
    for (const auto& opcion : opcionesMenu) {
        std::cout << opcion.id << ". " << opcion.descripcion << std::endl;
    }
    std::cout << "Ingrese una opción: " << std::endl;
}

void procesarOpcion(int opcion) {
    for (const auto& opcionMenu : opcionesMenu) {
        if (opcionMenu.id == opcion && opcionMenu.activa) {
            opcionMenu.funcion();
            return;
        }
    }
    std::cout << "Opción no válida, vuelve a intentar" << std::endl;
}

void salir() {
    std::cout << "Saliendo del sistema..." << std::endl;
    sistemaActivo = false;
}
