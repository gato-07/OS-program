#ifndef PERFILES_H
#define PERFILES_H

#include "estructuras_menu.h"
#include <vector>
#include <string>
#include <map>

// Variables globales externas
extern SesionUsuario sesion;
extern std::map<std::string, Perfil> perfilesDisponibles;

// Funciones de gestión de perfiles
bool cargarPerfiles();
bool asignarPerfilUsuario();
void mostrarPermisosUsuario();
Perfil obtenerPerfilPorNombre(const std::string& nombre);

// Validaciones
bool validarOpcionPermitida(int opcion);

#endif
