#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include <vector>

struct Usuario {
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

struct UsuarioTemporal {
    int id;
    char nombre[25];
    char username[17];
    char password[12];
    char perfil[8];
};

struct ListaUsuarios {
    std::vector<Usuario> usuarios;
    bool enMemoria;
};

struct OpcionMenu {
    int id;
    std::string descripcion;
    void (*funcion)();
    bool activa;
};

#endif
