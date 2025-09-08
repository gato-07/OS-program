# multiplicador_matrices/makefile
# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(INCDIR)

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DATADIR = ../data/matrices

# Archivos fuente y objetos
SOURCES = main.cpp matriz.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/multiplicador_matrices

# Regla principal
all: directories $(TARGET) matrices-ejemplo

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(DATADIR)

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Compilación exitosa! Ejecutable creado en $(TARGET)"

# Regla genérica para compilar archivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencias específicas
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/matriz.h
$(OBJDIR)/matriz.o: $(SRCDIR)/matriz.cpp $(INCDIR)/matriz.h

# Crear matrices de ejemplo
matrices-ejemplo: directories
	@echo "Creando matrices de ejemplo..."
	@echo "Matriz A (3x3) en $(DATADIR)/matrizA.txt"
	@echo "2,4,6" > $(DATADIR)/matrizA.txt
	@echo "1,3,5" >> $(DATADIR)/matrizA.txt
	@echo "7,8,9" >> $(DATADIR)/matrizA.txt

	@echo "Matriz B (3x3) en $(DATADIR)/matrizB.txt"
	@echo "1,0,2" > $(DATADIR)/matrizB.txt
	@echo "3,1,0" >> $(DATADIR)/matrizB.txt
	@echo "0,2,1" >> $(DATADIR)/matrizB.txt

	@echo "Matriz C (4x4) en $(DATADIR)/matrizC.txt"
	@echo "1.5,2.3,4.1,0.5" > $(DATADIR)/matrizC.txt
	@echo "3.2,1.7,2.8,1.1" >> $(DATADIR)/matrizC.txt
	@echo "0.9,4.5,3.3,2.4" >> $(DATADIR)/matrizC.txt
	@echo "2.1,1.8,0.7,3.9" >> $(DATADIR)/matrizC.txt

	@echo "Matriz D (4x4) en $(DATADIR)/matrizD.txt"
	@echo "2.0,0.0,1.0,3.0" > $(DATADIR)/matrizD.txt
	@echo "1.0,2.0,0.0,1.0" >> $(DATADIR)/matrizD.txt
	@echo "0.0,1.0,2.0,0.0" >> $(DATADIR)/matrizD.txt
	@echo "3.0,0.0,1.0,2.0" >> $(DATADIR)/matrizD.txt

	@echo "Matrices con separador ';' en $(DATADIR)/matrizE.txt y matrizF.txt"
	@echo "10;20;30" > $(DATADIR)/matrizE.txt
	@echo "40;50;60" >> $(DATADIR)/matrizE.txt
	@echo "70;80;90" >> $(DATADIR)/matrizE.txt

	@echo "2;4;6" > $(DATADIR)/matrizF.txt
	@echo "8;10;12" >> $(DATADIR)/matrizF.txt
	@echo "14;16;18" >> $(DATADIR)/matrizF.txt

	@echo "Matrices con separador espacio en $(DATADIR)/matrizG.txt y matrizH.txt"
	@echo "1 2 3 4 5" > $(DATADIR)/matrizG.txt
	@echo "6 7 8 9 10" >> $(DATADIR)/matrizG.txt
	@echo "11 12 13 14 15" >> $(DATADIR)/matrizG.txt
	@echo "16 17 18 19 20" >> $(DATADIR)/matrizG.txt
	@echo "21 22 23 24 25" >> $(DATADIR)/matrizG.txt

	@echo "1 0 0 0 0" > $(DATADIR)/matrizH.txt
	@echo "0 1 0 0 0" >> $(DATADIR)/matrizH.txt
	@echo "0 0 1 0 0" >> $(DATADIR)/matrizH.txt
	@echo "0 0 0 1 0" >> $(DATADIR)/matrizH.txt
	@echo "0 0 0 0 1" >> $(DATADIR)/matrizH.txt

	@echo "Matrices de ejemplo creadas exitosamente!"

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Archivos compilados eliminados"

# Limpiar todo incluyendo matrices de ejemplo
clean-all: clean
	rm -rf $(DATADIR)
	@echo "Archivos compilados y matrices de ejemplo eliminados"

# Ejecutar el programa con las matrices de ejemplo
test: all
	@echo "=== PRUEBA CON MATRICES 3x3 (separador ,) ==="
	./$(TARGET) $(DATADIR)/matrizA.txt $(DATADIR)/matrizB.txt ","
	@echo ""
	@echo "=== PRUEBA CON MATRICES 4x4 (separador ,) ==="
	./$(TARGET) $(DATADIR)/matrizC.txt $(DATADIR)/matrizD.txt ","
	@echo ""
	@echo "=== PRUEBA CON MATRICES 3x3 (separador ;) ==="
	./$(TARGET) $(DATADIR)/matrizE.txt $(DATADIR)/matrizF.txt ";"

# Probar con matrices incompatibles (debe dar error)
test-error: all
	@echo "=== PRUEBA DE ERROR: Matrices incompatibles ==="
	@echo "Intentando multiplicar 3x3 con 4x4 (debe fallar):"
	-./$(TARGET) $(DATADIR)/matrizA.txt $(DATADIR)/matrizC.txt ","

# Ejecutar con argumentos personalizados
run: all
	@echo "Uso: make run ARGS='archivo1 archivo2 separador'"
	@echo "Ejemplo: make run ARGS='$(DATADIR)/matrizA.txt $(DATADIR)/matrizB.txt ,'"
	@if [ -z "$(ARGS)" ]; then \
		echo "Error: Debe proporcionar argumentos con ARGS='...'"; \
		exit 1; \
	else \
		./$(TARGET) $(ARGS); \
	fi

# Recompilar todo desde cero
rebuild: clean all

# Mostrar información del proyecto
info:
	@echo "=== MULTIPLICADOR DE MATRICES ==="
	@echo "Compilador: $(CXX) $(CXXFLAGS)"
	@echo "Ejecutable: $(TARGET)"
	@echo "Directorio de matrices: $(DATADIR)"
	@echo ""
	@echo "Matrices de ejemplo disponibles:"
	@echo "  - matrizA.txt, matrizB.txt (3x3, separador ,)"
	@echo "  - matrizC.txt, matrizD.txt (4x4, separador ,)"
	@echo "  - matrizE.txt, matrizF.txt (3x3, separador ;)"
	@echo "  - matrizG.txt, matrizH.txt (5x5, separador espacio)"

# Mostrar ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make all          - Compilar el programa y crear matrices de ejemplo"
	@echo "  make test         - Ejecutar pruebas con matrices de ejemplo"
	@echo "  make test-error   - Probar con matrices incompatibles"
	@echo "  make run ARGS=... - Ejecutar con argumentos personalizados"
	@echo "  make matrices-ejemplo - Solo crear matrices de ejemplo"
	@echo "  make clean        - Limpiar archivos compilados"
	@echo "  make clean-all    - Limpiar todo incluyendo matrices"
	@echo "  make rebuild      - Recompilar desde cero"
	@echo "  make info         - Mostrar información del proyecto"
	@echo "  make help         - Mostrar esta ayuda"

.PHONY: all clean clean-all test test-error run rebuild info help directories matrices-ejemplo
