#ifndef ESTRUCTURAS_GESTOR_H
#define ESTRUCTURAS_GESTOR_H

#include "../../common/include/estructuras_usuarios.h"
#include <vector>

// Estructuras específicas del gestor de usuarios
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
