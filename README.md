# Sistemas Operativos
El objetivo de este proyecto es desarrollar un Sistema Operativo (SO) que permita la ejecución de otros programas de una forma eficiente.

## Caracteristicas
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
  ### Descripcion
    El juego se basa en combate por turnos, por cada
    comando (atacar <nombre enemigo> o defender <nombre enemigo>) 
    se lanza un dado y golpea por el valor. El juego se acaba cuando
    se mueren todos los integrantes del equipo.
  ### Requerimientos
    pip install dotenv
    sudo apt-get install konsole (valido tambien instalar xterm, este paso es por si no hay un paquete que maneje consolas externas)
  ### Ejecucion
    La opcion 9 (juego) deja escoger entre HOST y CLIENTE (jugador).
    El cliente pide ingresar nombre y nombre del equipo, asegurarse de poner el mismo nombre exacto de equipo para hacer teams (2 TEAMS).
  ### Parametros
    Los parametros esenciales se pueden manejar desde el .env.

    SERVER_HOST: Direccion IP, local o Ipv4 (por defecto local).
    SERVER_PORT: Puerto (por defecto 9999, recomendado usar algun valor cercano a este).
    GAME_MAX_PLAYERS_PER_TEAM: Maximo de jugadores por equipo.
    GAME_DIE_SIDES_R: Tamaño del dado que se usa para atacar, el numero que sale es el valor de tu ataque.
    PLAYER_MAX_HP: Vida maxima de cada jugador.
  ### Graficos
    Los graficos ocupan los siguientes estadisticos desde un csv: timestamp,match_id,player_name,team_name,is_winner,damage_dealt,attacks_made,defenses_made. Los graficos que se generan para cada partida individual son: Acciones por partida, Daño total por jugador y Contribucion de daño por equipo (daño porcentual de toda la partida). 

## Grafico rendimiento
  Programa que incluye un módulo de benchmarking automatizado diseñado para evaluar la eficiencia del algoritmo de índice invertido paralelo.

  ### Requisitos
    * Python 3 instalado en el sistema.
    * Librería `matplotlib` para la generación de gráficos.

  ### Parametros
    Los parametros esenciales se pueden manejar desde el .env

    CANT_THREADS=["1", "2", "3", "4", "8", "12"] (Para definir la cantidad de hilos a evaluar)

# Compilación y ejecución menu principal
  Cada carpeta de app cuenta con su makefile, los cuales son requeridos para el funcionamiento del menu principal. Desde cada carpeta en terminal: 
  
  Compilar Gestor de Usuarios
  
    cd OS-program/gestor_usuarios
    make

  Compilar Multiplicador de Matrices
  
    cd OS-program/multiplicador_matrices
    make

  Compilar Índice Invertido
  
    cd OS-program/crea_indice
    make

  Compilar Índice Invertido Paralelo
  
    cd OS-program/crea_indice_paralelo
    make

  Compilar Grafica de rendimiento
  
    cd OS-program/grafica_rendimiento_paralela
    make

  Ir al menu principal
  
    cd OS-program/menu_principal
      make
      ./bin/menu_principal -u usuario -p password [-f archivo]

# Configuración Indice Invertido Paralelo
  El módulo crea_indice_paralelo se puede configurar usando las siguientes variables de entorno. Si no se definen, se usarán valores por defecto.

  N_THREADS: Especifica el número de hilos de procesamiento.

    Por defecto: 4

  N_LOTE: (Aquí debes explicar qué hace esta variable. Ej: "Define cuántos libros procesa cada hilo a la vez").

    Por defecto: 10

  MAPA_LIBROS: Define la ruta donde se guardará el archivo mapa_libros.txt.

    Por defecto: data/mapa_libros.txt
