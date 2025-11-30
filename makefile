# Makefile principal para compilar todos los subproyectos.

# Lista de directorios que contienen otros Makefiles.
SUBDIRS = gestor_usuarios multiplicador_matrices crea_indice crea_indice_paralelo grafica_rendimiento_paralela menu_principal

# El objetivo por defecto 'all' compilará todo.
all: $(SUBDIRS)
	@echo "--- Todos los subproyectos han sido compilados. ---"

$(SUBDIRS):
	@echo "--- Compilando en el directorio: $@ ---"
	$(MAKE) -C $@

clean: clean_subdirs

clean_subdirs:
	@for dir in $(SUBDIRS); do \
		echo "--- Limpiando en el directorio: $$dir ---"; \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: all clean $(SUBDIRS) clean_subdirs