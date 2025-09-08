#ifndef AUTENTICACION_H
#define AUTENTICACION_H

#include "estructuras_menu.h"
#include <vector>
#include <string>

// Variable global de sesión
extern SesionUsuario sesion;
extern std::vector<Usuario> listaUsuarios;

// Funciones de autenticación
bool cargarUsuarios();
bool autenticarUsuario(const std::string& username, const std::string& password);
bool procesarArgumentos(int argc, char* argv[]);

// Funciones auxiliares
void mostrarInfoSesion();
bool usuarioTienePermiso(int opcionId);
void cerrarSesion();

// Funciones de manejo de variables de entorno
bool cargarVariablesEntorno();
std::string obtenerVariable(const std::string& nombre);

#endif
