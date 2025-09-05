#ifndef ESTRUCTURAS_USUARIOS_H
#define ESTRUCTURAS_USUARIOS_H
#include <string>


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

#endif
