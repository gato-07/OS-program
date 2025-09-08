#include "../include/autenticacion.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <getopt.h>

SesionUsuario sesion = {false, Usuario(), Perfil(), ""};
std::vector<Usuario> listaUsuarios;

bool cargarUsuarios() {
    const char* userFile = getenv("USER_FILE");
    std::string rutaUsuarios = userFile ? userFile : "data/usuarios.dat";
    std::ifstream archivo(rutaUsuarios, std::ios::binary);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de usuarios: " << rutaUsuarios << std::endl;
        std::cerr << "Asegúrese de que existe el archivo o ejecute el gestor de usuarios primero" << std::endl;
        return false;
    }

    listaUsuarios.clear();
    UsuarioTemporal temp;

    // Leer usuarios del archivo binario
    while (archivo.read(reinterpret_cast<char*>(&temp), sizeof(UsuarioTemporal))) {
        Usuario usuario;
        usuario.id = temp.id;
        usuario.nombre = std::string(temp.nombre);
        usuario.username = std::string(temp.username);
        usuario.password = std::string(temp.password);
        usuario.perfil = std::string(temp.perfil);

        listaUsuarios.push_back(usuario);
    }

    archivo.close();

    if (listaUsuarios.empty()) {
        std::cerr << "Error: No hay usuarios en el sistema" << std::endl;
        std::cerr << "Ejecute primero el gestor de usuarios para crear usuarios" << std::endl;
        return false;
    }

    std::cout << "Se cargaron " << listaUsuarios.size() << " usuarios del sistema" << std::endl;
    return true;
}

bool autenticarUsuario(const std::string& username, const std::string& password) {
    for (const auto& usuario : listaUsuarios) {
        if (usuario.username == username && usuario.password == password) {
            sesion.autenticado = true;
            sesion.usuarioActual = usuario;
            return true;
        }
    }
    return false;
}

bool procesarArgumentos(int argc, char* argv[]) {
    // Variables para los argumentos
    std::string username = "";
    std::string password = "";
    std::string archivo = "";

    // Procesar argumentos usando getopt
    int opt;
    while ((opt = getopt(argc, argv, "u:p:f:")) != -1) {
        switch (opt) {
            case 'u':
                username = optarg;
                break;
            case 'p':
                password = optarg;
                break;
            case 'f':
                archivo = optarg;
                break;
            default:
                std::cerr << "Uso: " << argv[0] << " -u usuario -p password [-f archivo]" << std::endl;
                return false;
        }
    }

    // Validar que se proporcionaron usuario y contraseña
    if (username.empty() || password.empty()) {
        std::cerr << "Error: Usuario y contraseña son requeridos" << std::endl;
        std::cerr << "Uso: " << argv[0] << " -u usuario -p password [-f archivo]" << std::endl;
        return false;
    }

    // Cargar usuarios del sistema
    if (!cargarUsuarios()) {
        return false;
    }

    // Intentar autenticar
    if (!autenticarUsuario(username, password)) {
        std::cerr << "Error: Usuario o contraseña incorrectos" << std::endl;
        return false;
    }

    std::cout << "Autenticación exitosa para usuario: " << username << std::endl;

    // Guardar archivo de trabajo si se proporcionó
    if (!archivo.empty()) {
        sesion.archivoTrabajo = archivo;
        std::cout << "Archivo de trabajo establecido: " << archivo << std::endl;
    }

    return true;
}

void mostrarInfoSesion() {
    if (!sesion.autenticado) {
        std::cout << "No hay usuario autenticado" << std::endl;
        return;
    }

    std::cout << "\n=== INFORMACIÓN DE SESIÓN ===" << std::endl;
    std::cout << "Usuario: " << sesion.usuarioActual.nombre
              << " (" << sesion.usuarioActual.username << ")" << std::endl;
    std::cout << "Perfil: " << sesion.perfilActual.nombre << std::endl;

    if (!sesion.archivoTrabajo.empty()) {
        std::cout << "Archivo de trabajo: " << sesion.archivoTrabajo << std::endl;
    }
}

bool usuarioTienePermiso(int opcionId) {
    return sesion.perfilActual.opcionesPermitidas.count(opcionId) > 0;
}

void cerrarSesion() {
    sesion.autenticado = false;
    sesion.usuarioActual = Usuario();
    sesion.perfilActual = Perfil();
    sesion.archivoTrabajo = "";
    listaUsuarios.clear();
    std::cout << "Sesión cerrada correctamente" << std::endl;
}

bool cargarVariablesEntorno() {
    std::ifstream archivo("../.env");
    if (!archivo.is_open()) {
        std::cout << "Advertencia: No se encontró archivo .env" << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        size_t pos = linea.find('=');
        if (pos != std::string::npos) {
            std::string variable = linea.substr(0, pos);
            std::string valor = linea.substr(pos + 1);

            // Establecer variable de entorno
            setenv(variable.c_str(), valor.c_str(), 1);
        }
    }

    archivo.close();
    return true;
}

std::string obtenerVariable(const std::string& nombre) {
    const char* valor = getenv(nombre.c_str());
    return valor ? std::string(valor) : "";
}
