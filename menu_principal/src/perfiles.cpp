#include "../include/perfiles.h"
#include "../include/autenticacion.h"  // Para acceder a sesion
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::map<std::string, Perfil> perfilesDisponibles;

bool cargarPerfiles() {
    // Obtener ruta del archivo desde variable de entorno
    const char* perfilesFile = getenv("PERFILES_FILE");
    std::string rutaPerfiles = perfilesFile ? perfilesFile : "data/PERFILES.txt";

    std::ifstream archivo(rutaPerfiles);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de perfiles: " << rutaPerfiles << std::endl;
        return false;
    }

    perfilesDisponibles.clear();
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        // Parsear línea: NOMBRE;opcion1,opcion2,opcion3
        size_t pos = linea.find(';');
        if (pos == std::string::npos) continue;

        Perfil perfil;
        perfil.nombre = linea.substr(0, pos);

        // Parsear opciones permitidas
        std::string opciones = linea.substr(pos + 1);
        std::stringstream ss(opciones);
        std::string opcion;

        while (std::getline(ss, opcion, ',')) {
            try {
                int opcionId = std::stoi(opcion);
                perfil.opcionesPermitidas.insert(opcionId);
            } catch (const std::exception& e) {
                std::cerr << "Error parseando opción: " << opcion << std::endl;
            }
        }

        perfilesDisponibles[perfil.nombre] = perfil;
    }

    archivo.close();
    std::cout << "Se cargaron " << perfilesDisponibles.size() << " perfiles" << std::endl;
    return true;
}

bool asignarPerfilUsuario() {
    if (!sesion.autenticado) {
        return false;
    }

    auto it = perfilesDisponibles.find(sesion.usuarioActual.perfil);
    if (it != perfilesDisponibles.end()) {
        sesion.perfilActual = it->second;
        return true;
    }

    std::cerr << "Error: Perfil '" << sesion.usuarioActual.perfil << "' no encontrado" << std::endl;
    return false;
}

void mostrarPermisosUsuario() {
    std::cout << "\n=== PERMISOS DEL USUARIO ===" << std::endl;
    std::cout << "Usuario: " << sesion.usuarioActual.username << std::endl;
    std::cout << "Perfil: " << sesion.perfilActual.nombre << std::endl;
    std::cout << "Opciones permitidas: ";

    for (const auto& opcion : sesion.perfilActual.opcionesPermitidas) {
        std::cout << opcion << " ";
    }
    std::cout << std::endl;
}

Perfil obtenerPerfilPorNombre(const std::string& nombre) {
    auto it = perfilesDisponibles.find(nombre);
    if (it != perfilesDisponibles.end()) {
        return it->second;
    }
    return Perfil(); // Perfil vacío si no se encuentra
}

bool validarOpcionPermitida(int opcion) {
    return sesion.perfilActual.opcionesPermitidas.count(opcion) > 0;
}
