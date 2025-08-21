#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

void window_callback(Fl_Widget* widget, void* data) {
    Fl_Window* window = static_cast<Fl_Window*>(data);
    window->hide(); // Oculta la ventana al hacer clic en el botón
}