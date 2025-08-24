#ifndef GESTOR_H
#define GESTOR_H

#include <string>
#include <vector>
// estructuras de datos
struct Usuario
{
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

struct ListaUsuarios{
    std::vector<Usuario> usuarios;
    bool enMemoria;
};

//
struct OpcionMenu {
    int id;
    std::string descripcion;
    void (*funcion)();  // puntero a la funcion de menu que ejecuta
    bool activa;
};

// variables globales
extern ListaUsuarios listaUsuarios;
extern std::string rutaArchivo;
extern std::vector<OpcionMenu> opcionesMenu;
extern bool sistemaActivo;

void cargarVariablesEntorno();
void iniciarMenu();
void mostrarMenu();
void procesarOpcion(int opcion);
void ejecutarSistema();
void limpiarPantalla();

int buscarUsuario(int id);
void crearUsuario();
void eliminarUsuario();
void listarUsuarios();
void salir();

void cargarUsuariosMemoria();
void guardarUsuariosArchivo();
void agregarUsuarioMemoria(const Usuario& usuario);
int obtenerProximoId();
bool existeUsername(const std::string& username);
#endif
