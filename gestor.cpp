#include "gestor.h"
#include <iostream>
#include <fstream>
#include <vector>

ListaUsuarios listaUsuarios = {{},false};
std::string rutaArchivo = "";
std::vector<OpcionMenu> opcionesMenu;

void cargarVariablesEntorno() {
    std::ifstream archivo(".env");
    std::string linea;

    if (archivo.is_open()) {
        while (std::getline(archivo, linea)) {
            // buscar USER_FILE
            if (linea.find("USER_FILE") != std::string::npos) {
                size_t pos = linea.find('=');
                if (pos != std::string::npos) {
                    rutaArchivo = linea.substr(pos + 1);
                    // remover comillas si existen
                    if (rutaArchivo.front() == '"' && rutaArchivo.back() == '"') {
                        rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                    }
                }
            }
        }
        archivo.close();
    }
    std::cout << "la ruta del archivo usuarios es:" << rutaArchivo << std::endl;
}

void iniciarMenu(){
    // de momento asi hasta que se implementen la gestion de permisos
    opcionesMenu = {
        {1, "Crear usuario", crearUsuario, true},
        {2, "Listar usuarios", listarUsuarios, true},
        {3, "Eliminar usuario", eliminarUsuario, true},
        {0, "Salir", salir, true}
    };

    std::cout << "todos los permisos por ahora" << std::endl;
}

void mostrarMenu(){
    limpiarPantalla();
    std::cout << "Gestor de Usuarios:" << std::endl;
    for (const auto& opcion : opcionesMenu) {
        std::cout << opcion.id << ". " << opcion.descripcion << std::endl;
    }
    std::cout << "Ingrese una opcion: " << std::endl;
}

void procesarOpcion(int opcion){
    for (const auto& opcionMenu : opcionesMenu) {
        if (opcionMenu.id == opcion && opcionMenu.activa) {
            opcionMenu.funcion();
            return;
        }
    }
    std::cout << "Opcion no valida" << std::endl;
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// estas son las funciones a las que deben apuntar cada elemento del tipo

void buscarUsuario(){
 // debe buscar un usuario (usarla para eliminar usuario)
}

void eliminarUsuario(){
    limpiarPantalla();
    std::cout << "por implementar" << std::endl;
}

void crearUsuario(){
    limpiarPantalla();
    std::cout << "por implementar" << std::endl;
}

void listarUsuarios(){
    limpiarPantalla();
    std::cout << "por implementar" << std::endl;
}

void salir(){
 // debe salir del programa (hay q discutir si es mejor como esta ahora o agregar una )
}

void ejecutarSistema() {
    cargarVariablesEntorno();
    iniciarMenu();
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        procesarOpcion(opcion);

        if (opcion != 0) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
    } while (opcion != 0);
}
