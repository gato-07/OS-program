# Definir el compilador y las banderas
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Definir directorios
SRC_DIR = src
BIN_DIR = bin

# Definir el nombre del ejecutable final
TARGET = $(BIN_DIR)/benchmark

# Definir el archivo fuente
SRC = $(SRC_DIR)/benchmark.cpp

# Regla por defecto (lo que se ejecuta al escribir 'make')
all: directories $(TARGET)

# Regla para crear el directorio bin si no existe
# el flag '-p' evita errores si la carpeta ya existe
directories:
	@mkdir -p $(BIN_DIR)

# Regla para compilar el ejecutable
# Depende de la carpeta bin y del archivo fuente
$(TARGET): $(SRC)
	@echo "Compilando benchmark..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	@echo "Compilación exitosa. Ejecutable creado en: $(TARGET)"

# Regla para limpiar (borrar la carpeta bin y su contenido)
clean:
	@echo "Limpiando archivos compilados..."
	rm -rf $(BIN_DIR)