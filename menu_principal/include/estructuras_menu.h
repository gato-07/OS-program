#ifndef ESTRUCTURAS_MENU_H
#define ESTRUCTURAS_MENU_H

#include "../../common/include/estructuras_usuarios.h"
#include <set>

// estructuras para el menu
struct Perfil {
    std::string nombre;
    std::set<int> opcionesPermitidas;
};

struct OpcionMenuPrincipal {
    int id;
    std::string descripcion;
    void (*funcion)();
};

struct SesionUsuario {
    bool autenticado;
    Usuario usuarioActual;
    Perfil perfilActual;
    std::string archivoTrabajo;
};

#endif
