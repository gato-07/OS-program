#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>
#include "estructuras_gestor.h"

extern std::string rutaArchivo;

// solo estas funciones involucran el manejo de archivos
void cargarVariablesEntorno();
void cargarUsuariosMemoria();
void guardarUsuariosArchivo();

#endif
