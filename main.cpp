#include<iostream>
#include "gui.h"
int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(340,180, "Ejemplo FLTK");
    Fl_Button *button = new Fl_Button(110, 70, 120, 40, "¡Hola FLTK!");
    button->callback(window_callback, window); // Asigna el callback al botón
    window->end();
    window->show(argc, argv);
    return Fl::run();
}