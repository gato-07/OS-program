#include "usuario.h"
#include "archivo.h"
#include "utils.h"
#include <iostream>
#include <limits>

ListaUsuarios listaUsuarios = {{}, false};

int buscarUsuario(int id) {
    for (size_t i = 0; i < listaUsuarios.usuarios.size(); i++) {
        if (listaUsuarios.usuarios[i].id == id) {
            return i;
        }
    }
    return -1;
}

void crearUsuario() {
    limpiarPantalla();
    cargarUsuariosMemoria();

    std::string nombre, username, password, perfil;
    int opcionPerfil;
    bool entradaValida = false;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese nombre: ";
    std::getline(std::cin, nombre);
    std::cout << "Ingrese username: ";
    std::getline(std::cin, username);
    std::cout << "Ingrese la contraseña: ";
    std::getline(std::cin, password);

    // Validar campos vacíos
    if (nombre.empty() || username.empty() || password.empty()) {
        std::cout << "Error: Todos los campos son obligatorios." << std::endl;
        pausarPantalla();
        return;
    }
    // validar longitud de los campos
    if (!validarLongitudesUsuario(nombre, username, password)) {
        pausarPantalla();
        return;
    }

    // Validar username duplicado
    if (existeUsername(username)) {
        std::cout << "Error: El username '" << username << "' ya existe." << std::endl;
        pausarPantalla();
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
                std::cout << "Opción inválida. Por favor, ingrese 1 o 2." << std::endl;
            }
        } else {
            std::cout << "Entrada inválida. Por favor, ingrese un número." << std::endl;
            std::cin.clear();
        }
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

    std::cout << "Usuario '" << username << "' (ID: " << nuevoUsuario.id << ") creado exitosamente" << std::endl;
    pausarPantalla();
}

void eliminarUsuario() {
    limpiarPantalla();
    cargarUsuariosMemoria();

    if (listaUsuarios.usuarios.empty()) {
        std::cout << "No hay usuarios para eliminar." << std::endl;
        pausarPantalla();
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
        pausarPantalla();
        return;
    }

    int indice = buscarUsuario(idEliminar);

    if (indice == -1) {
        std::cout << "Usuario con ID " << idEliminar << " no encontrado." << std::endl;
        pausarPantalla();
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
            pausarPantalla();
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

    pausarPantalla();
}

void listarUsuarios() {
    limpiarPantalla();
    cargarUsuariosMemoria();

    std::cout << "Id\tNombre\t\tUsername\tPerfil" << std::endl;

    if(listaUsuarios.usuarios.empty()) {
        std::cerr << "Aún no hay usuarios" << std::endl;
    }

    for (const auto& usuario : listaUsuarios.usuarios) {
        std::cout << usuario.id << "\t"
                  << usuario.nombre << "\t\t"
                  << usuario.username << "\t\t"
                  << usuario.perfil << std::endl;
    }

    pausarPantalla();
}

void agregarUsuarioMemoria(const Usuario& usuario) {
    listaUsuarios.usuarios.push_back(usuario);
}

int obtenerProximoId() {
    int maxId = 0;
    for (const auto& usuario : listaUsuarios.usuarios) {
        if (usuario.id > maxId) {
            maxId = usuario.id;
        }
    }
    return maxId + 1;
}

bool existeUsername(const std::string& username) {
    for (const auto& usuario : listaUsuarios.usuarios) {
        if (usuario.username == username) {
            return true;
        }
    }
    return false;
}
