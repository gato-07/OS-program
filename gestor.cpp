#include "gestor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>


ListaUsuarios listaUsuarios = {{},false};
std::string rutaArchivo = "";
std::vector<OpcionMenu> opcionesMenu;
bool sistemaActivo = true;

void cargarVariablesEntorno() {
    std::ifstream archivo(".env");
    std::string linea;
    rutaArchivo = "data/usuarios.txt";

    if (!archivo.is_open()) {
        std::cout << "Archivo .env no encontrado. Usando: " << rutaArchivo << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        // buscar USER_FILE
        if (linea.find("USER_FILE") != std::string::npos) {
            size_t pos = linea.find('=');
            if (pos != std::string::npos) {
                rutaArchivo = linea.substr(pos + 1);
                // remover comillas si existen
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"' && rutaArchivo.back() == '"') {
                    rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                }
                break;
            }
        }
    }
    archivo.close();
    std::cout << "Ruta del archivo usuarios: " << rutaArchivo << std::endl;
}

void cargarUsuariosMemoria() {
    if (listaUsuarios.enMemoria){
        return; // si los usuarios ya estan cargados no es necesario hacer nada
    }

    std::ifstream archivo(rutaArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cout << "no se encuentra el archivo de usuarios, se creo uno nuevo en " << rutaArchivo << std::endl;
        listaUsuarios.enMemoria = true;
        return;
    }

    listaUsuarios.usuarios.clear();

    while (std::getline(archivo, linea)) {
        if (!linea.empty()) {
            std::stringstream ss(linea);
            std::string campo;
            std::vector<std::string> campos;

            while (std::getline(ss, campo, ',')) {
                campos.push_back(campo);
            }

            if (campos.size() >= 5) {
                Usuario usuario;
                usuario.id = std::stoi(campos[0]);
                usuario.nombre = campos[1];
                usuario.username = campos[2];
                usuario.password = campos[3];
                usuario.perfil = campos[4];

                listaUsuarios.usuarios.push_back(usuario);
            }
        }
    }
    archivo.close();
    listaUsuarios.enMemoria = true;
    // std::cout << "usuarios cargados en memoria" << std::endl;
}

void guardarUsuariosArchivo() {
    std::ofstream archivo(rutaArchivo);

    if (!archivo.is_open()) {
        std::cerr << "no se pudo abrir el archivo: " << rutaArchivo << std::endl;
        return;
    }

    for (const auto& usuario : listaUsuarios.usuarios) {
        archivo << usuario.id << ","
                << usuario.nombre << ","
                << usuario.username << ","
                << usuario.password << ","
                << usuario.perfil << std::endl;
    }

    archivo.close();
    std::cout << "usuarios guardados en archivo: " << rutaArchivo << std::endl;
}

void agregarUsuarioMemoria(const Usuario &usuario) {
    cargarUsuariosMemoria(); // revisar si ya existen
    listaUsuarios.usuarios.push_back(usuario);
}

bool existeUsername(const std::string& username) {
    cargarUsuariosMemoria();
    for (const auto& usuario : listaUsuarios.usuarios) {
        if (usuario.username == username) {
            return true;
        }
    }
    return false;
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
    std::cout << "Opcion no valida, vuelve a intentar" << std::endl;
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// estas son las funciones a las que deben apuntar cada elemento del tipo

int buscarUsuario(int id){
    cargarUsuariosMemoria();

    for (size_t i = 0; i < listaUsuarios.usuarios.size(); i++) {
        if (listaUsuarios.usuarios[i].id == id) {
            return i; // Retorna el índice donde está el usuario
        }
    }
    return -1; // No encontrado
}

void eliminarUsuario(){
    limpiarPantalla();
    cargarUsuariosMemoria();

    if (listaUsuarios.usuarios.empty()) {
        std::cout << "No hay usuarios para eliminar." << std::endl;
        return;
    }

    // Mostrar usuarios disponibles
    std::cout << "Lista de usuarios disponibles:" << std::endl;
    std::cout << "Id\tNombre\t\tUsername\tPerfil" << std::endl;

    for (const auto& usuario : listaUsuarios.usuarios) {
        std::cout << usuario.id << "\t"
                  << usuario.nombre << "\t\t"
                  << usuario.username << "\t\t"
                  << usuario.perfil << std::endl;
    }

    int idEliminar;
    std::cout << "Ingrese el ID del usuario a eliminar: ";

    if (!(std::cin >> idEliminar)) {
        std::cout << "ID inválido." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    // buscar el id a eliminar
    int indice = buscarUsuario(idEliminar);

    if (indice == -1) {
        std::cout << "Usuario con ID " << idEliminar << " no encontrado." << std::endl;
        return;
    }

    Usuario usuarioAEliminar = listaUsuarios.usuarios[indice];

    // Alerta si es usuario ADMIN
    if (usuarioAEliminar.perfil == "admin") {
        std::cout << "\n¡ALERTA!" << std::endl;
        std::cout << "Está intentando eliminar un usuario ADMIN: " << usuarioAEliminar.nombre << std::endl;
        std::cout << "Esto podría causar problemas en el sistema." << std::endl;

        char confirmacion;
        std::cout << "¿Está seguro de continuar? (s/n): ";
        std::cin >> confirmacion;

        if (confirmacion != 's' && confirmacion != 'S') {
            std::cout << "Operación cancelada." << std::endl;
            return;
        }
    }

    // Confirmar eliminación
    char confirmar;
    std::cout << "\n¿Confirma eliminar al usuario '" << usuarioAEliminar.nombre
              << "' (ID: " << usuarioAEliminar.id << ")? (s/n): ";
    std::cin >> confirmar;

    if (confirmar != 's' && confirmar != 'S') {
        std::cout << "Operación cancelada." << std::endl;
        return;
    }

    // Eliminar usuario de la lista
    listaUsuarios.usuarios.erase(listaUsuarios.usuarios.begin() + indice);

    // Guardar cambios en archivo
    guardarUsuariosArchivo();

    std::cout << "\nUsuario '" << usuarioAEliminar.nombre << "' eliminado exitosamente." << std::endl;
}

void crearUsuario(){
    limpiarPantalla();

    std::string nombre, username, password, perfil;
    int opcionPerfil;
    bool entradaValida = false;
    //Duda sobre la siguiente linea del codigo, preguntar despues si hay alguna otra manera de hacerlo
    // no, no se puede xd
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese nombre: ";
    std::getline(std::cin, nombre);
    std::cout << "Ingrese username: ";
    std::getline(std::cin, username);
    std::cout << "Ingrese la contraseña: ";
    std::getline(std::cin, password);

    // validar campos vacíos
    if (nombre.empty() || username.empty() || password.empty()) {
        std::cout << "Error: Todos los campos son obligatorios." << std::endl;
        return;
    }

    // validar username duplicado
    if (existeUsername(username)) {
        std::cout << "Error: El username '" << username << "' ya existe." << std::endl;
        return;
    }
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


    Usuario nuevoUsuario;
    nuevoUsuario.id = obtenerProximoId();
    nuevoUsuario.nombre = nombre;
    nuevoUsuario.username = username;
    nuevoUsuario.password = password;
    nuevoUsuario.perfil = perfil;

    agregarUsuarioMemoria(nuevoUsuario);
    guardarUsuariosArchivo();
    std::cout << "usuario '" << username << "' (ID: " << nuevoUsuario.id << ") creado exitosamente" << std::endl;
}

void listarUsuarios(){
    limpiarPantalla();
    cargarUsuariosMemoria();

    std::string linea;

    std::cout << "Id\tNombre\t\tUsername\tPerfil" << std::endl;

    if(listaUsuarios.usuarios.empty()){
        std::cerr << "aun no hay usuarios" << std::endl;
    }

    for (const auto& usuario : listaUsuarios.usuarios) {
        std::cout << usuario.id << "\t"
                  << usuario.nombre << "\t\t"
                  << usuario.username << "\t\t"
                  << usuario.perfil << std::endl;
    }
}

void salir(){
    std::cout << "Saliendo del sistema..." << std::endl;
    sistemaActivo = false;
}

int obtenerProximoId() {
    cargarUsuariosMemoria(); // tienen que haber si o si datos en memoria

    int maxId = 0;
    for (const auto& usuario : listaUsuarios.usuarios) {
        if (usuario.id > maxId) {
            maxId = usuario.id;
        }
    }
    return maxId + 1;
}

void ejecutarSistema() {
    cargarVariablesEntorno();
    iniciarMenu();

    while (sistemaActivo) {
        mostrarMenu();
        int opcion;
        std::cin >> opcion;
        procesarOpcion(opcion);

        if (sistemaActivo) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
    }
    std::cout << "Cerrado" << std::endl;
}
