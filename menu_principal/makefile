# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INCDIR) -I../common/include

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Archivos fuente y objetos del menú principal
SOURCES = main.cpp menu.cpp autenticacion.cpp perfiles.cpp funciones_menu.cpp utils.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/menu_principal

# Archivos fuente y objetos de crea_indice
CI_SRCDIR = ../crea_indice/src
CI_OBJDIR = $(OBJDIR)/crea_indice
CI_SOURCES = $(wildcard $(CI_SRCDIR)/*.cpp)
CI_OBJECTS = $(CI_SOURCES:$(CI_SRCDIR)/%.cpp=$(CI_OBJDIR)/%.o)
CI_TARGET = $(BINDIR)/crea_indice


# Regla principal: compila menú y crea_indice
all: directories $(TARGET) $(CI_TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJDIR) $(CI_OBJDIR) $(BINDIR) ../data ../logs

# Compilar menú principal
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilación exitosa! Ejecutable creado en $(TARGET)"

# Regla genérica para compilar objetos del menú
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar crea_indice solo si hay archivos fuente
$(CI_TARGET): $(CI_OBJECTS)
ifeq ($(CI_OBJECTS),)
	@echo "No hay archivos fuente en crea_indice. Saltando compilación."
else
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilación exitosa! Ejecutable crea_indice creado en $(CI_TARGET)"
endif

# Regla genérica para compilar objetos de crea_indice
$(CI_OBJDIR)/%.o: $(CI_SRCDIR)/%.cpp
	@mkdir -p $(CI_OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Archivos compilados eliminados"

# Ejecutar programa de prueba del menú
test: all
	@echo "=== PRUEBA DEL MENÚ PRINCIPAL ==="
	@echo "Uso: ./$(TARGET) -u usuario -p password [-f archivo]"
	@echo "Ejemplo: ./$(TARGET) -u admin -p admin"

test-admin: all
	./$(TARGET) -u admin -p admin

test-general: all
	./$(TARGET) -u general -p general

# Verificar archivos necesarios
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
	@echo "Ejecutable menú: $(TARGET)"
	@echo "Ejecutable crea_indice: $(CI_TARGET)"
	@echo "Archivos fuente menú: $(SOURCES)"
	@echo "Archivos fuente crea_indice: $(CI_SOURCES)"

# Mostrar ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make all           - Compilar menú y crea_indice"
	@echo "  make test          - Mostrar cómo ejecutar pruebas del menú"
	@echo "  make test-admin    - Ejecutar menú con usuario admin"
	@echo "  make test-general  - Ejecutar menú con usuario general"
	@echo "  make check-files   - Verificar archivos necesarios"
	@echo "  make setup         - Crear archivos básicos de prueba"
	@echo "  make clean         - Limpiar archivos compilados"
	@echo "  make rebuild       - Recompilar desde cero"
	@echo "  make info          - Mostrar información del proyecto"
	@echo "  make help          - Mostrar esta ayuda"

# Archivos que no son archivos reales
.PHONY: all clean test test-admin test-general check-files setup rebuild info help directories
