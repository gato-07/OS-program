# Menu Principal

Sistema operativo integrado que proporciona acceso a todas las aplicaciones mediante un menu centralizado con autenticacion de usuarios y perfiles.

## Descripcion

Menu principal con autenticacion que ejecuta todos los modulos del sistema, controlando permisos mediante perfiles de usuario.

## Compilacion

```bash
cd menu_principal
make
```

## Uso

```bash
./bin/menu_principal -u <usuario> -p <password> [-f <archivo>]
```

Parametros:
- `-u usuario`: Nombre de usuario (requerido)
- `-p password`: Password del usuario (requerido)
- `-f archivo`: Archivo de trabajo opcional

Ejemplo:
```bash
./bin/menu_principal -u admin -p admin123
./bin/menu_principal -u user1 -p pass123 -f ../data/documento.txt
```

## Sistema de Perfiles

Los perfiles definen que opciones del menu puede acceder cada usuario. Se configuran en:
```
data/PERFILES.txt
```

Formato:
```
nombre_perfil:opcion1,opcion2,opcion3
```

## Opciones del Menu

1. Admin de usuarios y perfiles
2. Multiplicacion de matrices NxN
3. Juego
4. Validador de palindromos
5. Calcular f(x)=x²+2x+8
6. Contador de texto
7. Crear indice invertido
8. Crear indice invertido paralelo
9. Juego (multijugador)
10. Graficar rendimiento paralelo
11. Buscador de libros (Cache + Motor)
0. Salir

## Configuracion (.env)

El sistema requiere configuracion de variables de entorno:

```bash
# Ejecutables
ADMIN_SYS=../gestor_usuarios/bin/gestor_usuarios
MULTI_M=../multiplicador_matrices/bin/multiplicador_matrices
CREATE_INDEX=../crea_indice/bin/crea_indice
INDICE_INVERT_PARALELO=../crea_indice_paralelo/bin/crea_indice_paralelo
CACHE_SERVIDOR=../cache/bin/cache_servidor
MOTOR_SERVIDOR=../motor/bin/motor_servidor
BUSCADOR_CLIENTE=../buscador/bin/buscador

# Archivos de datos
USER_FILE=../data/usuarios.dat
PERFILES_FILE=data/PERFILES.txt
INDICE_FILE=../menu_principal/indice.idx
MAPA_LIBROS=../data/mapa_libros.txt

# Configuracion paralela
N_THREADS=8
N_LOTE=10
```

## Funcionalidades Integradas

- Autenticacion de usuarios
- Control de acceso basado en perfiles
- Ejecucion de aplicaciones externas
- Manejo de archivos de trabajo
- Visualizacion de PID del proceso

## Estructura

```
menu_principal/
├── include/
│   ├── autenticacion.h
│   ├── perfiles.h
│   ├── utils.h
│   ├── menu.h
│   └── funciones_menu.h
├── src/
│   ├── main.cpp
│   ├── menu.cpp
│   ├── autenticacion.cpp
│   ├── perfiles.cpp
│   ├── utils.cpp
│   └── funciones_menu.cpp
├── data/
│   └── PERFILES.txt
├── logs/
├── obj/
├── bin/
└── makefile
```
