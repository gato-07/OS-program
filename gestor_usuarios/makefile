# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(INCDIR)

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DATADIR = ../data

# Archivos fuente y objetos
SOURCES = main.cpp menu.cpp usuario.cpp archivo.cpp utils.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/gestor_usuarios

# Regla principal
all: directories $(TARGET)

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

# Dependencias específicas (para los headers)
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/menu.h $(INCDIR)/archivo.h $(INCDIR)/utils.h
$(OBJDIR)/menu.o: $(SRCDIR)/menu.cpp $(INCDIR)/menu.h $(INCDIR)/usuario.h $(INCDIR)/utils.h
$(OBJDIR)/usuario.o: $(SRCDIR)/usuario.cpp $(INCDIR)/usuario.h $(INCDIR)/archivo.h $(INCDIR)/utils.h
$(OBJDIR)/archivo.o: $(SRCDIR)/archivo.cpp $(INCDIR)/archivo.h $(INCDIR)/usuario.h
$(OBJDIR)/utils.o: $(SRCDIR)/utils.cpp $(INCDIR)/utils.h $(INCDIR)/estructuras.h

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Archivos compilados eliminados"

# Ejecutar el programa
run: all
	./$(TARGET)

# Recompilar todo desde cero
rebuild: clean all

.PHONY: all clean run rebuild directories
