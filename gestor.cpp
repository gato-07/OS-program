#include <fstream>
#include <iostream>
#include "gestor.h"
using namespace std;

int main(int argc, char **argv) {
    
    int n;

    cout << "Bienvenido al gestor de usuarios :3" << endl;
    cout << "Ingrese una opcion:" << endl;
    cout << "1. Crear usuario" << endl;
    cout << "2. Ver usuarios(NO DISPONIBLE)" << endl;
    cout << "3. Eliminar usuario(NO DISPONIBLE)" << endl;

    cin >> n;

    switch (n){
        case 1: {

            int id; char nombre[50], perfil[50];
            cout << "Ingrese el ID del usuario: ";
            cin >> id;
            cout << "Ingrese el nombre del usuario: ";
            cin.ignore(); // Limpiar el buffer de entrada
            cin.getline(nombre, sizeof(nombre));
            cout << "Ingrese el perfil del usuario: ";
            cin.getline(perfil, sizeof(perfil));
            cout << "Ingrese el username del usuario: ";
            char username[50];
            cin.getline(username, sizeof(username));
            cout << "Ingrese la password del usuario: ";
            char password[50];
            cin.getline(password, sizeof(password));
            usuario nuevoUsuario = crearUsuario(id, nombre, perfil, username, password);


            ofstream archivo("usuarios.txt", ios::app);
            if (archivo.is_open()) {
                archivo << nuevoUsuario.id << ","
                        << nuevoUsuario.nombre << ","
                        << nuevoUsuario.username << ","
                        << nuevoUsuario.password << ","
                        << nuevoUsuario.perfil << endl;
                archivo.close();
                cout << "Usuario creado y guardado en el archivo." << endl;
            }
            break;
        }
    }
}