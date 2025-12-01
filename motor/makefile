# Makefile para Servidor Motor de Búsqueda

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
INCLUDES = -Iinclude
TARGET = bin/motor_servidor
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

# Ejecutar el servidor
run: $(TARGET)
	@echo "Ejecutando servidor motor..."
	$(TARGET) ../../crea_indice_paralelo/indice.idx ../../crea_indice_paralelo/data/mapa_libros.txt

.PHONY: all clean run
