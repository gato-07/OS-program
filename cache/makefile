# Makefile para Servidor de Cache FIFO

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
INCLUDES = -Iinclude
TARGET = bin/cache_servidor
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
	@echo "Ejecutando servidor de cache..."
	$(TARGET) 100

.PHONY: all clean run
