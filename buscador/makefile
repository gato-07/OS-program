# Makefile para Cliente Buscador

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
INCLUDES = -Iinclude
TARGET = bin/buscador
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

# Ejecutar el buscador
run: $(TARGET)
	@echo "Ejecutando buscador..."
	$(TARGET)

.PHONY: all clean run
