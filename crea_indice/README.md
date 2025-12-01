# Creador de Indice Invertido

Programa que genera un indice invertido a partir de documentos de texto de forma secuencial.

## Descripcion

Procesa archivos de texto en una carpeta y genera un indice invertido que mapea palabras a los documentos donde aparecen y su frecuencia.

## Compilacion

```bash
cd crea_indice
make
```

## Uso

```bash
./bin/crea_indice <archivo_salida.idx> <carpeta_libros>
```

Parametros:
- `archivo_salida.idx`: Ruta donde se guardara el indice invertido
- `carpeta_libros`: Ruta a la carpeta con archivos .txt

Ejemplo:
```bash
./bin/crea_indice indice.idx ../data/libros
```

## Formato de Salida

El archivo .idx generado tiene el formato:
```
palabra;(archivo1.txt,freq1);(archivo2.txt,freq2);...
```

Ejemplo:
```
python;(libro1.txt,5);(libro2.txt,3)
machine;(libro1.txt,2);(libro3.txt,1)
learning;(libro1.txt,3);(libro2.txt,2)
```

## Procesamiento

1. Lee todos los archivos .txt de la carpeta
2. Tokeniza cada documento en palabras
3. Normaliza palabras (minusculas, elimina puntuacion)
4. Cuenta frecuencia de cada palabra por documento
5. Genera indice invertido ordenado alfabeticamente

## Limitaciones

- Version secuencial (un solo hilo)
- Para procesamiento paralelo usar crea_indice_paralelo

## Estructura

```
crea_indice/
├── include/
├── src/
│   └── main.cpp
└── makefile
```
