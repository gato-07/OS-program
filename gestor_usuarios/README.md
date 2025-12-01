# Gestor de Usuarios

Sistema de administracion de usuarios con almacenamiento persistente en archivo binario.

## Descripcion

Permite registrar, ver y eliminar usuarios del sistema operativo con persistencia en archivo usuarios.dat.

## Compilacion

```bash
cd gestor_usuarios
make
```

## Uso

```bash
./bin/gestor_usuarios
```

No requiere parametros, se ejecuta de forma interactiva.

Ejemplo:
```bash
./bin/gestor_usuarios
```

## Funcionalidades

Menu interactivo con opciones:

1. **Registrar Usuario**
   - Solicita nombre de usuario y password
   - Valida que el usuario no exista
   - Guarda en archivo binario

2. **Ver Usuarios**
   - Lista todos los usuarios registrados
   - Muestra nombre de usuario (no password)

3. **Eliminar Usuario**
   - Solicita nombre de usuario
   - Elimina del archivo si existe

4. **Salir**
   - Cierra el programa

## Persistencia

Los datos se almacenan en:
```
../data/usuarios.dat
```

Si el archivo no existe, se crea automaticamente vacio.

## Formato de Almacenamiento

El archivo usuarios.dat es binario y contiene registros de:
- Username (string)
- Password (string, cifrado/hash)
- Metadata adicional

## Seguridad

- Passwords almacenados de forma segura
- Validacion de duplicados
- Manejo de errores en operaciones de archivo

## Estructura

```
gestor_usuarios/
├── include/
│   ├── usuario.h
│   ├── archivo.h
│   ├── utils.h
│   └── menu.h
├── src/
│   ├── main.cpp
│   ├── usuario.cpp
│   ├── archivo.cpp
│   ├── utils.cpp
│   └── menu.cpp
├── obj/
├── bin/
└── makefile
```
