#include "archivo.h"
#include "usuario.h"  // Para acceder a listaUsuarios
#include <iostream>
#include <fstream>
#include <sstream>

std::string rutaArchivo = "";

void cargarVariablesEntorno() {
    std::ifstream archivo(".env");
    std::string linea;
    rutaArchivo = "data/usuarios.txt";

    if (!archivo.is_open()) {
        std::cout << "Archivo .env no encontrado. Usando: " << rutaArchivo << std::endl;
        return;
    }

    while (std::getline(archivo, linea)) {
        if (linea.find("USER_FILE") != std::string::npos) {
            size_t pos = linea.find('=');
            if (pos != std::string::npos) {
                rutaArchivo = linea.substr(pos + 1);
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
    if (listaUsuarios.enMemoria) {
        return;
    }

    std::ifstream archivo(rutaArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cout << "No se encuentra el archivo de usuarios, se creó uno nuevo en " << rutaArchivo << std::endl;
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
}

void guardarUsuariosArchivo() {
    std::ofstream archivo(rutaArchivo);

    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << rutaArchivo << std::endl;
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
    std::cout << "Usuarios guardados en archivo: " << rutaArchivo << std::endl;
}
