#ifndef MENU_H
#define MENU_H

#include "estructuras.h"
#include <vector>

extern std::vector<OpcionMenu> opcionesMenu;
extern bool sistemaActivo;

void iniciarMenu();
void mostrarMenu();
void procesarOpcion(int opcion);
void salir();

#endif
