#include "archivo.h"
#include "estructuras.h"
#include "usuario.h"
#include <iostream>
#include <cstring>
#include <fstream>

std::string rutaArchivo = "";

void cargarVariablesEntorno() {
    std::ifstream archivo("../.env");  // Buscar .env en el directorio padre
    std::string linea;
    rutaArchivo = "../data/usuarios.dat";  // Ruta por defecto al directorio data compartido

    if (!archivo.is_open()) {
        std::cout << "Archivo .env no encontrado. Usando ruta por defecto: " << rutaArchivo << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        if (linea.find("USER_FILE") != std::string::npos) {
            size_t pos = linea.find('=');
            if (pos != std::string::npos) {
                std::string valor = linea.substr(pos + 1);
                // Quitar comillas si las tiene
                if (!valor.empty() && valor.front() == '"' && valor.back() == '"') {
                    valor = valor.substr(1, valor.length() - 2);
                }
                // Agregar ../ al inicio si no lo tiene
                if (valor.substr(0, 3) != "../") {
                    rutaArchivo = "../" + valor;
                } else {
                    rutaArchivo = valor;
                }
                break;
            }
        }
    }
    archivo.close();
    std::cout << "Ruta del archivo usuarios: " << rutaArchivo << std::endl;
}

void cargarUsuariosMemoria() {
    if (listaUsuarios.enMemoria) {
        return;
    }

    std::ifstream archivo(rutaArchivo, std::ios::binary);

    if (!archivo.is_open()) {
        std::cout << "No se encuentra el archivo de usuarios, se creará uno nuevo en " << rutaArchivo << std::endl;
        listaUsuarios.enMemoria = true;
        return;
    }

    listaUsuarios.usuarios.clear();
    UsuarioTemporal temp;

    while (archivo.read(reinterpret_cast<char*>(&temp), sizeof(UsuarioTemporal))) {
        Usuario usuario;
        usuario.id = temp.id;
        usuario.nombre = std::string(temp.nombre);
        usuario.username = std::string(temp.username);
        usuario.password = std::string(temp.password);
        usuario.perfil = std::string(temp.perfil);

        listaUsuarios.usuarios.push_back(usuario);
    }

    archivo.close();
    listaUsuarios.enMemoria = true;
    std::cout << "Se cargaron " << listaUsuarios.usuarios.size() << " usuarios desde el archivo usuarios" << std::endl;
}

void guardarUsuariosArchivo() {
    std::ofstream archivo(rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << rutaArchivo << std::endl;
        return;
    }

    for (const auto& usuario : listaUsuarios.usuarios) {
        UsuarioTemporal temp;
        temp.id = usuario.id;

        std::strncpy(temp.nombre, usuario.nombre.c_str(), sizeof(temp.nombre) - 1);
        temp.nombre[sizeof(temp.nombre) - 1] = '\0';

        std::strncpy(temp.username, usuario.username.c_str(), sizeof(temp.username) - 1);
        temp.username[sizeof(temp.username) - 1] = '\0';

        std::strncpy(temp.password, usuario.password.c_str(), sizeof(temp.password) - 1);
        temp.password[sizeof(temp.password) - 1] = '\0';

        std::strncpy(temp.perfil, usuario.perfil.c_str(), sizeof(temp.perfil) - 1);
        temp.perfil[sizeof(temp.perfil) - 1] = '\0';

        // reinterpreta la estructura temporal
        archivo.write(reinterpret_cast<const char*>(&temp), sizeof(UsuarioTemporal));
    }

    archivo.close();
    std::cout << "Usuarios guardados en archivo: " << rutaArchivo << std::endl;
}
