# Cache FIFO

Servidor de cache con politica de eviccion FIFO (First In, First Out) para el sistema de busqueda de libros.

## Descripcion

Almacena resultados de busquedas recientes usando un HashMap para acceso O(1) y una cola FIFO para eviccion cuando se alcanza la capacidad maxima.

## Compilacion

```bash
cd cache
make
```

## Uso

```bash
./bin/cache_servidor [tamanio]
```

Parametros:
- `tamanio`: Numero maximo de consultas en cache (default: 100)

Ejemplo:
```bash
./bin/cache_servidor 100
```

## Protocolo

El servidor escucha en puerto 8001 y acepta comandos via TCP:

- `GET:<consulta>` - Buscar en cache
  - Respuesta: `HIT:<datos>` o `MISS`
  
- `PUT:<consulta>:<datos>` - Guardar en cache
  - Respuesta: `OK`
  
- `STATS` - Obtener estadisticas
  - Respuesta: `HITS:X|MISSES:Y|TASA:Z%`
  
- `CLEAR` - Limpiar cache
  - Respuesta: `OK`

Formato de datos: `libro1,score1;libro2,score2;...`

## Logs

El servidor muestra logs en tiempo real:
```
[CACHE] GET "python machine learning" -> MISS
[CACHE] PUT "python machine learning" -> Guardado (3 resultados)
[CACHE] GET "python machine learning" -> HIT (devolviendo 3 resultados)
[CACHE] STATS -> HITS:1|MISSES:1|TASA:50%
```

## Estructura

```
cache/
├── include/
│   ├── cache_fifo.h
│   └── servidor_cache.h
├── src/
│   └── main.cpp
├── obj/
├── bin/
└── makefile
```
