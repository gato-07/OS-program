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

## Buscador de Libros (Cache + Motor)
  Sistema de búsqueda de libros implementado con arquitectura cliente-servidor usando sockets TCP. Consta de 3 componentes independientes:
  
  ### Componentes
  
  **1. Servidor Cache (Puerto 8001)**
  - Implementa una caché FIFO con HashMap para O(1) lookup
  - Almacena resultados de búsquedas recientes
  - Estadísticas de hits/misses
  - Política de evicción FIFO cuando está lleno
  
  **2. Servidor Motor (Puerto 8002)**
  - Carga el índice invertido generado por `crea_indice_paralelo`
  - Búsqueda en índice invertido con algoritmo TopK
  - Ranking de documentos por score (frecuencia de términos)
  
  **3. Cliente Buscador**
  - Interfaz de usuario para realizar búsquedas
  - Orquesta la comunicación entre Cache y Motor
  - Mide tiempos de respuesta (cache, memoria, total)
  - Muestra resultados en formato JSON
  
  ### Flujo de Búsqueda
  ```
  Usuario ingresa query
    ↓
  [1] Buscador → Cache: ¿Está en caché?
    ↓
  SI: Cache → Buscador (HIT)
  NO: [2] Buscador → Motor: Buscar en índice
        ↓
      Motor calcula TopK
        ↓
      Motor → Buscador: Resultados
        ↓
      [3] Buscador → Cache: Guardar resultados
        ↓
      Buscador muestra JSON con métricas
  ```
  
  ### Formato de Respuesta JSON
  ```json
  {
    "Mensaje": "danilo rojo",
    "origen_respuesta": "cache",
    "tiempo_cache": 2.45 ms,
    "tiempo_M_total": 0 ms,
    "tiempo_total": 3.12 ms,
    "topK": 3,
    "Respuesta": [
      {"Libro": "El Sombrio Anido", "Score": 3},
      {"Libro": "Militio", "Score": 2},
      {"Libro": "Ojos Rojos", "Score": 1}
    ]
  }
  ```
  
  ### Protocolo de Comunicación
  
  **Cache (8001):**
  - `GET:<query>` → `HIT:<datos>` o `MISS`
  - `PUT:<query>:<datos>` → `OK`
  - `STATS` → `HITS:X|MISSES:Y|TASA:Z%`
  - `CLEAR` → `OK`
  
  **Motor (8002):**
  - `SEARCH:<query>:<topK>` → `OK:<datos>` o `OK:VACIO`
  
  Formato de datos: `libro1,score1;libro2,score2;...`
  
  ### Requisitos
  - Índice invertido generado (ejecutar opción 8 del menú)
  - Archivo `mapa_libros.txt` generado
  - Terminal con soporte para xterm, gnome-terminal o konsole
  
  ### Parámetros (.env)
  ```bash
  CACHE_SERVIDOR=../cache/bin/cache_servidor
  MOTOR_SERVIDOR=../motor/bin/motor_servidor
  BUSCADOR_CLIENTE=../buscador/bin/buscador
  INDICE_FILE=../crea_indice_paralelo/indice.idx
  MAPA_LIBROS=../data/mapa_libros.txt
  CACHE_SIZE=100
  TOPK_DEFAULT=3
  ```
  
  ### Manejo de Errores
  - El Buscador verifica que Cache y Motor estén activos antes de aceptar búsquedas
  - Timeouts de 5 segundos en conexiones
  - Mensajes descriptivos indicando qué servicio falta y cómo iniciarlo
  - Validación de respuestas en cada comunicación
  
  ### Uso desde el Menú
  Opción 11: "Buscador de libros (Cache + Motor)"
  
  Submenú:
  1. Iniciar Cache (Puerto 8001) - Abre nueva ventana
  2. Iniciar Motor (Puerto 8002) - Abre nueva ventana
  3. Ejecutar Buscador (Cliente) - En la terminal actual
  4. Iniciar todo el sistema - Automatiza 1, 2 y 3

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

  Compilar Cache (Servidor de Caché FIFO)
  
    cd OS-program/cache
    make

  Compilar Motor (Servidor de Búsqueda)
  
    cd OS-program/motor
    make

  Compilar Buscador (Cliente)
  
    cd OS-program/buscador
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
