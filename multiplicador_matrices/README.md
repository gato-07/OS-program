# Multiplicador de Matrices

Programa que realiza la multiplicacion de dos matrices NxN.

## Descripcion

Lee dos matrices desde archivos de texto y calcula su producto, mostrando el resultado y guardandolo en un archivo.

## Compilacion

```bash
cd multiplicador_matrices
make
```

## Uso

```bash
./bin/multiplicador_matrices <matrizA> <matrizB> <separador>
```

Parametros:
- `matrizA`: Ruta al archivo de la primera matriz
- `matrizB`: Ruta al archivo de la segunda matriz
- `separador`: Caracter separador usado en los archivos (ej: , ; espacio)

Ejemplo:
```bash
./bin/multiplicador_matrices ../data/matrices/matrizA.txt ../data/matrices/matrizB.txt ,
```

## Formato de Archivos

Las matrices deben estar en archivos de texto con el siguiente formato:

```
1,2,3
4,5,6
7,8,9
```

La primera linea indica las dimensiones (NxM), seguida de N lineas con M valores separados por el delimitador especificado.

## Restricciones

- Las matrices deben ser compatibles para multiplicacion (columnas de A = filas de B)
- Solo soporta matrices cuadradas o rectangulares

## Salida

El programa muestra:
1. Matriz A
2. Matriz B
3. Matriz resultado (A x B)
4. Guarda el resultado en un archivo

## Estructura

```
multiplicador_matrices/
├── include/
│   └── matriz.h
├── src/
│   ├── main.cpp
│   └── matriz.cpp
├── obj/
├── bin/
└── makefile
```
