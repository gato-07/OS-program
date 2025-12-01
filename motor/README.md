# Motor de Busqueda

Servidor de busqueda que carga un indice invertido y procesa consultas usando algoritmo TopK.

## Descripcion

Carga el indice invertido generado por crea_indice_paralelo y realiza busquedas por terminos con ranking de documentos basado en frecuencia.

## Compilacion

```bash
cd motor
make
```

## Uso

```bash
./bin/motor_servidor <archivo_indice.idx>
```

Parametros:
- `archivo_indice.idx`: Ruta al archivo de indice invertido

Ejemplo:
```bash
./bin/motor_servidor ../menu_principal/indice.idx
```

## Protocolo

El servidor escucha en puerto 8002 y acepta comandos via TCP:

- `SEARCH:<consulta>:<topK>` - Buscar documentos
  - Respuesta: `OK:<datos>` o `OK:VACIO`

Formato de datos: `libro1,score1;libro2,score2;...`

## Algoritmo

1. Tokeniza la consulta en palabras
2. Busca cada palabra en el indice invertido
3. Acumula scores por documento (suma de frecuencias)
4. Ordena por score descendente
5. Retorna los TopK resultados

## Logs

El servidor muestra logs en tiempo real:
```
[MOTOR] SEARCH "python machine learning" (topK=3) -> 3 resultados encontrados
[MOTOR] SEARCH "test" (topK=3) -> 0 resultados encontrados
```

## Formato del Indice

El archivo .idx debe tener formato:
```
palabra;(archivo1.txt,freq1);(archivo2.txt,freq2);...
```

Ejemplo:
```
python;(libro1.txt,5);(libro2.txt,3)
machine;(libro1.txt,2);(libro3.txt,1)
learning;(libro1.txt,3);(libro2.txt,2)
```

## Estructura

```
motor/
├── include/
│   ├── indice_invertido.h
│   └── servidor_motor.h
├── src/
│   └── main.cpp
├── obj/
├── bin/
└── makefile
```
