#include <cstring>
#include <string>

// estructuras de datos
struct usuario 
{
    int id;
    char nombre[50];
    char username[50];
    char password[50];
    char perfil[50];
};

// metodos

usuario crearUsuario(int id, const char* nombre, const char* perfil, const char* username, const char* password) {
    usuario nuevoUsuario;
    
    nuevoUsuario.id = id;
    strncpy(nuevoUsuario.nombre, nombre, sizeof(nuevoUsuario.nombre) - 1);
    nuevoUsuario.nombre[sizeof(nuevoUsuario.nombre) - 1] = '\0';
    strncpy(nuevoUsuario.perfil, perfil, sizeof(nuevoUsuario.perfil) - 1);
    nuevoUsuario.perfil[sizeof(nuevoUsuario.perfil) - 1];
    strncpy(nuevoUsuario.username, username, sizeof(nuevoUsuario.username) - 1);
    nuevoUsuario.username[sizeof(nuevoUsuario.username) - 1] = '\0';
    strncpy(nuevoUsuario.password, password, sizeof(nuevoUsuario.password) - 1);
    nuevoUsuario.password[sizeof(nuevoUsuario.password) - 1] = '\0';

    return nuevoUsuario;
}

// la funcion de menu funciona como arreglo, para que seafaicl manejar los permisos