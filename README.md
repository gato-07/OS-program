# Sistemas Operativos
El objetivo de este proyecto es desarrollar un Sistema Operativo (SO) que permita la ejecución de otros programas de una forma eficiente.

# Caracteristicas
  El programa cuenta con un menu principal que da acceso a todas las aplicaciones del SO.

## Menu
  Menu con diferentes modulos que trabaja con perfiles de usuario, recibiendo argumentos de ejecución y autenticación de usuario.

## Gestor de usuarios
 Este programa despliega un menu para Registrar, Ver y Eliminar usuarios del SO.

## Multiplicador de matrices
  Programa que recibe 2 matrices y realiza la multiplicación de estas.

## Indice ivertido
  Programa que genera un indice invertido a partir de una carpeta con libros en formato .txt.

## Indice invertido paralelo 
  Programa que genera un indice invertido a partir de una carpeta con libros en formato .txt trabajando con multiples hilos de forma paralela.

## Juego
  Construcción..

## Compilación y ejecución menu principal
  Cada carpeta de app cuenta con su makefile, los cuales son requeridos para el funcionamiento del menu principal. Desde cada carpeta en terminal: 
  # Compilar Gestor de Usuarios
  cd OS-program/gestor_usuarios
  make

  # Compilar Multiplicador de Matrices
  cd OS-program/multiplicador_matrices
  make

  # Compilar Índice Invertido
  cd OS-program/crea_indice
  make

  # Compilar Índice Invertido Paralelo
  cd OS-program/crea_indice_paralelo
  make

  # Ir al menu principal
  cd OS-program/menu_principal
      make
      ./bin/menu_principal -u usuario -p password [-f archivo]
