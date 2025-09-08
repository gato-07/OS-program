# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(INCDIR) -I../common/include

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Archivos fuente y objetos
SOURCES = main.cpp menu.cpp autenticacion.cpp perfiles.cpp funciones_menu.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/menu_principal

# Regla principal
all: directories $(TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p ../data
	@mkdir -p ../logs

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilación exitosa! Ejecutable creado en $(TARGET)"

# Regla genérica para compilar archivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Archivos compilados eliminados"

# Ejecutar programa de prueba
test: all
	@echo "=== PRUEBA DEL MENÚ PRINCIPAL ==="
	@echo "Uso: ./$(TARGET) -u usuario -p password [-f archivo]"
	@echo "Ejemplo: ./$(TARGET) -u admin -p admin"

# Ejecutar con usuario admin
test-admin: all
	./$(TARGET) -u admin -p admin

# Ejecutar con usuario general (si existe)
test-general: all
	./$(TARGET) -u general -p general

# Verificar estructura de archivos necesarios
check-files:
	@echo "Verificando archivos necesarios..."
	@test -f ../data/perfiles.txt || echo "Advertencia: ../data/perfiles.txt no existe"
	@test -f ../data/usuarios.dat || echo "Advertencia: ../data/usuarios.dat no existe"
	@test -f ../.env || echo "Advertencia: ../.env no existe"
	@echo "Verificación completada"

# Crear archivos básicos de prueba si no existen
setup: directories
	@echo "Configurando archivos básicos..."
	@if [ ! -f ../data/perfiles.txt ]; then \
		echo "ADMIN;0,1,2,3,4,5,6" > ../data/perfiles.txt; \
		echo "GENERAL;0,2,3,4,5,6" >> ../data/perfiles.txt; \
		echo "Archivo perfiles.txt creado"; \
	fi
	@echo "Configuración completada"

# Recompilar todo desde cero
rebuild: clean all

# Mostrar información del proyecto
info:
	@echo "=== MENÚ PRINCIPAL ==="
	@echo "Compilador: $(CXX) $(CXXFLAGS)"
	@echo "Ejecutable: $(TARGET)"
	@echo "Archivos fuente: $(SOURCES)"

# Mostrar ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make all         - Compilar el programa"
	@echo "  make test        - Mostrar cómo ejecutar pruebas"
	@echo "  make test-admin  - Ejecutar con usuario admin"
	@echo "  make test-general- Ejecutar con usuario general"
	@echo "  make check-files - Verificar archivos necesarios"
	@echo "  make setup       - Crear archivos básicos de prueba"
	@echo "  make clean       - Limpiar archivos compilados"
	@echo "  make rebuild     - Recompilar desde cero"
	@echo "  make info        - Mostrar información del proyecto"
	@echo "  make help        - Mostrar esta ayuda"

.PHONY: all clean test test-admin test-general check-files setup rebuild info help directories
