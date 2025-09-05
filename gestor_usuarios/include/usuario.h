#ifndef USUARIO_H
#define USUARIO_H

#include "estructuras_gestor.h"

extern ListaUsuarios listaUsuarios;

// funciones para el "crud" de usuarios
int buscarUsuario(int id);
void crearUsuario();
void eliminarUsuario();
void listarUsuarios();

void agregarUsuarioMemoria(const Usuario& usuario);
int obtenerProximoId();
bool existeUsername(const std::string& username);

#endif
