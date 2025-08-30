#include "utils.h"
#include "estructuras.h"
#include <cstdlib>
#include <iostream>
#include <limits>

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausarPantalla() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool validarLongitudesUsuario(const std::string& nombre, const std::string& username,
                             const std::string& password) {

    const size_t MAX_NOMBRE = sizeof(UsuarioTemporal{}.nombre) - 1;
    const size_t MAX_USERNAME = sizeof(UsuarioTemporal{}.username) - 1;
    const size_t MAX_PASSWORD = sizeof(UsuarioTemporal{}.password) - 1;

    if (nombre.length() > MAX_NOMBRE) {
        std::cerr << "Error: El nombre es demasiado largo (máximo " << MAX_NOMBRE << " caracteres)" << std::endl;
        return false;
    }

    if (username.length() > MAX_USERNAME) {
        std::cerr << "Error: El username es demasiado largo (máximo " << MAX_USERNAME << " caracteres)" << std::endl;
        return false;
    }

    if (password.length() > MAX_PASSWORD) {
        std::cerr << "Error: La contraseña es demasiado larga (máximo " << MAX_PASSWORD << " caracteres)" << std::endl;
        return false;
    }

    return true;
}
