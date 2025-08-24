#include "gestor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>


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

    std::string nombre, username, password, perfil;
    int opcionPerfil;
    bool entradaValida = false;
    //Duda sobre la siguiente linea del codigo, preguntar despues si hay alguna otra manera de hacerlo
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese nombre: "; 
    std::getline(std::cin, nombre);
    std::cout << "Ingrese username: ";
    std::getline(std::cin, username);
    std::cout << "Ingrese la contraseña: ";
    std::getline(std::cin, password);
    while (!entradaValida) {
        std::cout << "Ingrese perfil (1.admin o 2.general): ";
        if (std::cin >> opcionPerfil) {
            if (opcionPerfil == 1) {
                perfil = "admin";
                entradaValida = true;
            } else if (opcionPerfil == 2) {
                perfil = "general";
                entradaValida = true;
            } else {
                std::cout << "Opcion invalida. Por favor, ingrese 1 o 2." << std::endl;
            }
        } else {
            // Entra si lee un string
            std::cout << "Entrada invalida. Por favor, ingrese un numero." << std::endl;
            std::cin.clear(); 
        }
        //Revisar bien lo que hace esta linea
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    std::ofstream archivo("data/usuarios_ejemplo.txt", std::ios::app);

    int Id = obtenerUltimoId() + 1;
    
    if (archivo.is_open()) {
        archivo << Id << "," << nombre << "," << username << "," << password << "," << perfil << std::endl;
        archivo.close();
        std::cout << "Usuario '" << username << "' creado y guardado exitosamente." << std::endl;
    } else {
        std::cerr << "Error: No se pudo abrir el archivo" << std::endl;
    }
}

void listarUsuarios(){
    limpiarPantalla();
    std::cout << "por implementar" << std::endl;
}

void salir(){
 // debe salir del programa (hay q discutir si es mejor como esta ahora o agregar una )
}

//Obtiene el id del ultimo usuario del txt 
int obtenerUltimoId() {
    int ultimoId = 0;
    std::string linea;
    std::ifstream archivo("data/usuarios_ejemplo.txt");

    if (!archivo.is_open()) {
        return 0; 
    }

    //Lee las lineas del archivo
    while (std::getline(archivo, linea)) {
        if (!linea.empty()) {
            std::stringstream ss(linea);
            std::string idString;
            std::getline(ss, idString, ',');

            try {
                int id = std::stoi(idString);
                if (id > ultimoId) {
                    ultimoId = id;
                }
            } catch (const std::exception& e) {
            }
        }
    }
    archivo.close();

    return ultimoId;
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
