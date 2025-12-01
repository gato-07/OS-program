# Buscador de Libros

Cliente interactivo que orquesta la comunicacion entre Cache y Motor para realizar busquedas de libros.

## Descripcion

Interfaz de usuario que coordina busquedas consultando primero el cache y luego el motor si es necesario, midiendo tiempos de respuesta.

## Compilacion

```bash
cd buscador
make
```

## Uso

```bash
./bin/buscador [-h host] [-k topK]
```

Parametros:
- `-h host`: Direccion IP de los servidores (default: 127.0.0.1)
- `-k topK`: Numero de resultados a retornar (default: 3)

Ejemplo:
```bash
./bin/buscador -h 127.0.0.1 -k 5
```

## Requisitos

Antes de ejecutar el buscador, deben estar activos:
1. Servidor Cache en puerto 8001
2. Servidor Motor en puerto 8002

## Flujo de Busqueda

```
1. Consultar Cache (puerto 8001)
   - Si HIT: mostrar resultados
   - Si MISS: continuar al paso 2

2. Consultar Motor (puerto 8002)
   - Buscar en indice invertido
   - Obtener TopK resultados

3. Guardar en Cache
   - Almacenar resultados para futuras consultas
```

## Comandos Disponibles

```
buscador> <consulta>   - Buscar libros
buscador> stats        - Ver estadisticas del cache
buscador> salir        - Salir del programa
```

## Formato de Salida

```json
{
  "Mensaje": "python machine learning",
  "origen_respuesta": "cache",
  "tiempo_cache": 2.45 ms,
  "tiempo_M_total": 0 ms,
  "tiempo_total": 3.12 ms,
  "topK": 3,
  "Respuesta": [
    {"Libro": "Introduccion a Python", "Score": 15},
    {"Libro": "Machine Learning Basico", "Score": 8},
    {"Libro": "Programacion Avanzada", "Score": 3}
  ]
}
```

## Metricas

- `tiempo_cache`: Tiempo de consulta al cache
- `tiempo_M_total`: Tiempo de busqueda en memoria (motor)
- `tiempo_total`: Tiempo total de la operacion
- `origen_respuesta`: "cache" o "memoria"

## Estructura

```
buscador/
├── include/
│   ├── buscador.h
│   └── cliente_socket.h
├── src/
│   └── main.cpp
├── obj/
├── bin/
└── makefile
```
