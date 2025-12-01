# Creador de Indice Invertido Paralelo

Programa que genera un indice invertido usando procesamiento paralelo con multiples hilos.

## Descripcion

Version optimizada que procesa documentos en paralelo usando threads, generando un indice invertido y un archivo de mapeo de IDs a nombres de libros.

## Compilacion

```bash
cd crea_indice_paralelo
make
```

## Uso

```bash
./bin/crea_indice_paralelo <archivo_salida.idx> <carpeta_libros>
```

Parametros:
- `archivo_salida.idx`: Ruta donde se guardara el indice invertido
- `carpeta_libros`: Ruta a la carpeta con archivos .txt

Ejemplo:
```bash
./bin/crea_indice_paralelo indice.idx ../data/libros
```

## Variables de Entorno

Configura el procesamiento paralelo en el archivo .env:

```bash
N_THREADS=8              # Numero de hilos (default: 4)
N_LOTE=10               # Archivos procesados por lote (default: 10)
MAPA_LIBROS=../data/mapa_libros.txt  # Ruta del archivo de mapeo
```

## Archivos Generados

1. **indice.idx**: Indice invertido con formato:
   ```
   palabra;(archivo1.txt,freq1);(archivo2.txt,freq2);...
   ```

2. **mapa_libros.txt**: Mapeo de IDs numericos a nombres de archivos:
   ```
   1,El_Quijote.txt
   2,Don_Juan_Tenorio.txt
   3,Cien_Anos_de_Soledad.txt
   ```

## Ventajas sobre Version Secuencial

- Procesamiento paralelo con N threads
- Hasta 8x mas rapido en CPUs de 8 nucleos
- Procesamiento por lotes para eficiencia de memoria
- Logs detallados de progreso

## Logs

Durante la ejecucion muestra:
```
[INFO] Archivos encontrados: 35
[INFO] Procesando lote 1/4 (10 archivos)
[THREAD 0] Procesando libro1.txt
[THREAD 1] Procesando libro2.txt
[INFO] Lote 1 completado
[INFO] Indice invertido generado: indice.idx
[INFO] Mapa de libros generado: mapa_libros.txt
```

## Estructura

```
crea_indice_paralelo/
├── include/
├── src/
│   └── main.cpp
├── logs/
├── data/
├── obj/
├── bin/
└── makefile
```
