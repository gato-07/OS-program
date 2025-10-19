#include "../include/matriz.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

bool cargarMatriz(const std::string& archivo, char separador, Matriz& matriz) {
    std::ifstream file(archivo);
    if (!file.is_open()) {
        std::cerr << "Error: No se puede abrir el archivo " << archivo << std::endl;
        return false;
    }

    std::vector<std::vector<double>> datosTemp;
    std::string linea;
    int numColumnas = -1;

    while (std::getline(file, linea)) {
        // Saltar líneas vacías
        if (linea.empty() || linea.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::vector<double> fila;
        std::stringstream ss(linea);
        std::string valor;

        // Parsear la línea usando el separador
        while (std::getline(ss, valor, separador)) {
            // Limpiar espacios en blanco
            size_t inicio = valor.find_first_not_of(" \t");
            size_t fin = valor.find_last_not_of(" \t");
            if (inicio != std::string::npos && fin != std::string::npos) {
                valor = valor.substr(inicio, fin - inicio + 1);
            }

            try {
                size_t pos;
                double num = std::stod(valor, &pos);
                // Validar que se haya consumido toda la cadena
                if (pos != valor.length()) {
                    std::cerr << "Error: Valor no numérico encontrado: '" << valor
                            << "' en archivo " << archivo << std::endl;
                    return false;
                }
                fila.push_back(num);
            } catch (const std::exception& e) {
                std::cerr << "Error: Valor no numérico encontrado: '" << valor
                        << "' en archivo " << archivo << std::endl;
                return false;
            }
        }

        // Verificar consistencia de columnas
        if (numColumnas == -1) {
            numColumnas = fila.size();
            if (numColumnas == 0) {
                std::cerr << "Error: Fila vacía encontrada en " << archivo << std::endl;
                return false;
            }
        } else if ((int)fila.size() != numColumnas) {
            std::cerr << "Error: Matriz inconsistente en " << archivo << std::endl;
            std::cerr << "Se esperaban " << numColumnas << " columnas, pero se encontraron "
                      << fila.size() << std::endl;
            return false;
        }

        datosTemp.push_back(fila);
    }

    if (datosTemp.empty()) {
        std::cerr << "Error: Archivo vacío o sin datos válidos: " << archivo << std::endl;
        return false;
    }

    // Asignar datos a la matriz
    matriz.filas = datosTemp.size();
    matriz.columnas = numColumnas;
    matriz.datos = datosTemp;

    std::cout << "Matriz cargada desde " << archivo << ": "
              << matriz.filas << "x" << matriz.columnas << std::endl;
    return true;
}

bool multiplicarMatrices(const Matriz& a, const Matriz& b, Matriz& resultado) {
    // Verificar compatibilidad de dimensiones
    if (a.columnas != b.filas) {
        std::cerr << "Error: Las dimensiones no son compatibles para multiplicación" << std::endl;
        std::cerr << "Matriz A: " << a.filas << "x" << a.columnas << std::endl;
        std::cerr << "Matriz B: " << b.filas << "x" << b.columnas << std::endl;
        std::cerr << "Para multiplicar A×B, las columnas de A deben igual a las filas de B" << std::endl;
        return false;
    }

    // Preparar matriz resultado
    resultado.filas = a.filas;
    resultado.columnas = b.columnas;
    resultado.datos.clear();
    resultado.datos.resize(resultado.filas, std::vector<double>(resultado.columnas, 0.0));

    // Realizar multiplicación
    for (int i = 0; i < a.filas; i++) {
        for (int j = 0; j < b.columnas; j++) {
            double suma = 0.0;
            for (int k = 0; k < a.columnas; k++) {
                suma += a.datos[i][k] * b.datos[k][j];
            }
            resultado.datos[i][j] = suma;
        }
    }

    std::cout << "Multiplicación completada: " << resultado.filas
              << "x" << resultado.columnas << std::endl;
    return true;
}

bool guardarMatriz(const Matriz& matriz, const std::string& archivo, char separador) {
    std::ofstream file(archivo);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << archivo << std::endl;
        return false;
    }

    for (int i = 0; i < matriz.filas; i++) {
        for (int j = 0; j < matriz.columnas; j++) {
            file << std::fixed << std::setprecision(2) << matriz.datos[i][j];
            if (j < matriz.columnas - 1) {
                file << separador;
            }
        }
        file << std::endl;
    }

    file.close();
    return true;
}

void mostrarMatriz(const Matriz& matriz, const std::string& nombre) {
    std::cout << "\n=== " << nombre << " ===" << std::endl;
    for (int i = 0; i < matriz.filas; i++) {
        for (int j = 0; j < matriz.columnas; j++) {
            std::cout << std::setw(10) << std::setprecision(2) << std::fixed
                      << matriz.datos[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void limpiarMatriz(Matriz& matriz) {
    matriz.datos.clear();
    matriz.filas = 0;
    matriz.columnas = 0;
}

bool estaVacia(const Matriz& matriz) {
    return matriz.filas == 0 || matriz.columnas == 0 || matriz.datos.empty();
}

void mostrarAyuda(const std::string& programa) {
    std::cout << "Uso: " << programa << " <archivo_matriz_A> <archivo_matriz_B> <separador>" << std::endl;
    std::cout << "\nDescripción:" << std::endl;
    std::cout << "  Multiplica dos matrices NxN leídas desde archivos de texto" << std::endl;
    std::cout << "  y guarda el resultado en 'resultado_multiplicacion.txt'" << std::endl;
    std::cout << "\nArgumentos:" << std::endl;
    std::cout << "  archivo_matriz_A  : Archivo de texto con la primera matriz" << std::endl;
    std::cout << "  archivo_matriz_B  : Archivo de texto con la segunda matriz" << std::endl;
    std::cout << "  separador         : Carácter que separa los elementos (ej: ',' ';' ' ')" << std::endl;
    std::cout << "\nEjemplos:" << std::endl;
    std::cout << "  " << programa << " matriz1.txt matriz2.txt ," << std::endl;
    std::cout << "  " << programa << " datos/A.csv datos/B.csv ;" << std::endl;
    std::cout << "\nFormato de archivos:" << std::endl;
    std::cout << "  Los archivos deben contener matrices rectangulares con" << std::endl;
    std::cout << "  elementos numéricos separados por el carácter especificado" << std::endl;
    std::cout << "  Ejemplo con separador ',':" << std::endl;
    std::cout << "    1.5,2.0,3.2" << std::endl;
    std::cout << "    4.1,5.7,6.8" << std::endl;
    std::cout << "    7.3,8.9,9.1" << std::endl;
}
