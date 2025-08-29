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
