# Makefile para Creador de Índice Invertido Paralelo

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
INCLUDES = -Iinclude
TARGET = bin/crea_indice_paralelo
OBJDIR = obj
SRCDIR = src

# Archivos fuente
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Regla principal
all: $(TARGET)

# Crear directorios si no existen
$(OBJDIR):
	mkdir -p $(OBJDIR)

bin:
	mkdir -p bin

# Compilar el ejecutable
$(TARGET): $(OBJECTS) | bin
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "✓ Compilación exitosa: $(TARGET)"

# Compilar archivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) bin
	@echo "✓ Archivos compilados eliminados"

# Ejecutar con valores por defecto
run: $(TARGET)
	@echo "Ejecutando con valores por defecto..."
	@export N-THREADS=4 && export N-LOTE=10 && export MAPA-LIBROS=data/mapa_libros.txt && $(TARGET) test.idx test_libros/

# Mostrar ayuda
help:
	@echo "Makefile para Creador de Índice Invertido Paralelo"
	@echo ""
	@echo "Uso:"
	@echo "  make          - Compilar el programa"
	@echo "  make run      - Compilar y ejecutar con valores por defecto"
	@echo "  make clean    - Limpiar archivos compilados"
	@echo "  make help     - Mostrar esta ayuda"
	@echo ""
	@echo "Variables de entorno:"
	@echo "  N-THREADS     - Número de threads (default: 4)"
	@echo "  N-LOTE        - Tamaño de lote (default: 10)"
	@echo "  MAPA-LIBROS   - Ruta al archivo de mapeo (default: data/mapa_libros.txt)"

.PHONY: all clean run help
