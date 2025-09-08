#include "../include/matriz.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        mostrarAyuda(argv[0]);
        return 1;
    }

    std::string archivoA = argv[1];
    std::string archivoB = argv[2];
    char separador = argv[3][0];

    std::cout << "=== MULTIPLICADOR DE MATRICES ===" << std::endl;
    std::cout << "Archivo A: " << archivoA << std::endl;
    std::cout << "Archivo B: " << archivoB << std::endl;
    std::cout << "Separador: '" << separador << "'" << std::endl << std::endl;

    // Crear las matrices usando inicialización por defecto
    Matriz matrizA = {std::vector<std::vector<double>>(), 0, 0};
    Matriz matrizB = {std::vector<std::vector<double>>(), 0, 0};
    Matriz resultado = {std::vector<std::vector<double>>(), 0, 0};

    // Cargar primera matriz
    if (!cargarMatriz(archivoA, separador, matrizA)) {
        std::cerr << "Error: No se pudo cargar la matriz A" << std::endl;
        return 1;
    }

    // Cargar segunda matriz
    if (!cargarMatriz(archivoB, separador, matrizB)) {
        std::cerr << "Error: No se pudo cargar la matriz B" << std::endl;
        return 1;
    }

    // Validar que las matrices no estén vacías
    if (estaVacia(matrizA) || estaVacia(matrizB)) {
        std::cerr << "Error: Una o ambas matrices están vacías" << std::endl;
        return 1;
    }

    // Realizar multiplicación
    if (!multiplicarMatrices(matrizA, matrizB, resultado)) {
        std::cerr << "Error: Falló la multiplicación de matrices" << std::endl;
        return 1;
    }

    // Mostrar resultados usando funciones globales
    mostrarMatriz(matrizA, "MATRIZ A");
    mostrarMatriz(matrizB, "MATRIZ B");
    mostrarMatriz(resultado, "RESULTADO (A × B)");

    // Guardar resultado
    std::string archivoResultado = "resultado_multiplicacion.txt";
    if (guardarMatriz(resultado, archivoResultado, separador)) {
        std::cout << "\nResultado guardado en: " << archivoResultado << std::endl;
    } else {
        std::cout << "\nAdvertencia: No se pudo guardar el resultado" << std::endl;
    }

    std::cout << "\nMultiplicación completada exitosamente." << std::endl;

    return 0;
}
