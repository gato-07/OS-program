#include "../include/funciones_menu.h"
#include <iostream>
#include <algorithm>
#include <cctype>


bool esPalindromo(const std::string& texto) {
    std::string textoLimpio;

    // Limpiar texto: solo letras y números, convertir a minúsculas
    for (char c : texto) {
        if (std::isalnum(c)) {
            textoLimpio += std::tolower(c);
        }
    }

    if (textoLimpio.empty()) {
        return false;
    }

    // Verificar si es palíndromo
    std::string textoInvertido = textoLimpio;
    std::reverse(textoInvertido.begin(), textoInvertido.end());

    return textoLimpio == textoInvertido;
}

// Función para analizar texto
void analizarTexto(const std::string& texto, int& vocales, int& consonantes,
                   int& especiales, int& palabras) {
    vocales = consonantes = especiales = palabras = 0;
    bool enPalabra = false;

    for (size_t i = 0; i < texto.length(); i++) {
        char c = texto[i];

        if (std::isalpha(c)) {
            char minuscula = std::tolower(c);
            if (minuscula == 'a' || minuscula == 'e' || minuscula == 'i' ||
                minuscula == 'o' || minuscula == 'u') {
                vocales++;
            } else {
                consonantes++;
            }

            if (!enPalabra) {
                palabras++;
                enPalabra = true;
            }
        } else if (std::isspace(c)) {
            enPalabra = false;
        } else {
            especiales++;
            enPalabra = false;
        }
    }
}

// Función para mostrar opciones de VOLVER
bool mostrarOpcionVolver() {
    char opcion;
    std::cout << "\n¿Desea realizar otra operación? (s)í / (n)o - VOLVER: ";
    std::cin >> opcion;

    return (opcion == 's' || opcion == 'S');
}

// Función para validar entrada numérica
bool leerNumeroReal(double& numero) {
    std::cin >> numero;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Error: Debe ingresar un número válido" << std::endl;
        return false;
    }

    return true;
}
