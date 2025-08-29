#include "sistema.h"
#include "archivo.h"
#include "menu.h"
#include <iostream>
#include <limits>

void ejecutarSistema() {
    cargarVariablesEntorno();
    iniciarMenu();

    while (sistemaActivo) {
        mostrarMenu();
        int opcion;

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            procesarOpcion(opcion);
        }
    }
    std::cout << "Cerrado" << std::endl;
}
