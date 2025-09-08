#ifndef FUNCIONES_MENU_H
#define FUNCIONES_MENU_H

#include <string>

bool esPalindromo(const std::string& texto);
void analizarTexto(const std::string& texto, int& vocales, int& consonantes,
                   int& especiales, int& palabras);
bool mostrarOpcionVolver();
bool leerNumeroReal(double& numero);

#endif
