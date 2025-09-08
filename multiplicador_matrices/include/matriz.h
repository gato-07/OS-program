#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <string>


struct Matriz {
    std::vector<std::vector<double>> datos;
    int filas;
    int columnas;
};

bool cargarMatriz(const std::string& archivo, char separador, Matriz& matriz);
bool multiplicarMatrices(const Matriz& a, const Matriz& b, Matriz& resultado);
bool guardarMatriz(const Matriz& matriz, const std::string& archivo, char separador);
void mostrarMatriz(const Matriz& matriz, const std::string& nombre);
void limpiarMatriz(Matriz& matriz);
bool estaVacia(const Matriz& matriz);
void mostrarAyuda(const std::string& programa);

#endif
