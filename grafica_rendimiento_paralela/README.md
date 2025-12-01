# Benchmark de Rendimiento Paralelo

Programa de benchmarking que evalua el rendimiento del creador de indice invertido paralelo con diferentes configuraciones de hilos.

## Descripcion

Ejecuta multiples pruebas del indice invertido paralelo variando el numero de threads y genera graficas comparativas de rendimiento.

## Compilacion

```bash
cd grafica_rendimiento_paralela
make
```

## Uso

```bash
./bin/benchmark <carpeta_libros> "<lista_hilos>"
```

Parametros:
- `carpeta_libros`: Ruta a la carpeta con archivos .txt
- `lista_hilos`: Numeros de hilos a probar separados por espacio

Ejemplo:
```bash
./bin/benchmark ../data/libros "1 2 4 8"
```

## Variables de Entorno

```bash
MAX_BENCHMARK_TESTS=5   # Limite de pruebas simultaneas (default: 5)
```

## Funcionamiento

1. Ejecuta crea_indice_paralelo con cada configuracion de hilos
2. Mide tiempo de ejecucion de cada prueba
3. Genera graficas de rendimiento con matplotlib
4. Guarda resultados en formato CSV

## Salida

El programa genera:
- **benchmark_results.csv**: Datos de tiempo por configuracion
- **rendimiento_paralelo.png**: Grafica de tiempo vs numero de hilos
- **speedup.png**: Grafica de aceleracion relativa

## Requisitos

- Python 3
- matplotlib (`pip install matplotlib`)
- Indice invertido paralelo compilado

## Ejemplo de Grafica

La grafica muestra:
- Eje X: Numero de hilos (1, 2, 4, 8)
- Eje Y: Tiempo de ejecucion (segundos)
- Linea de tendencia mostrando mejora de rendimiento

## Estructura

```
grafica_rendimiento_paralela/
├── src/
│   └── benchmark.cpp
├── bin/
└── makefile
```
