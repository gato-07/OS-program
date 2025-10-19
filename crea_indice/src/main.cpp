#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <filesystem>
#include <cctype>

namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    if(argc != 3){
        std:: cerr << "Uso: " << argv[0] << "<archivo_salida.idx> <carpeta_libros>" << std::endl;
        return 1;
    }


    std::string archivoSalida = argv[1];
    std::string carpetaLibros = argv[2];


    //Validar .idx
    if(archivoSalida.size() < 4 || archivoSalida.substr(archivoSalida.size() - 4) != ".idx"){
        std::cerr << "Error: el archivo de salida debe tener extension .idx" << std::endl;
        return 1;        
    }
    
    //El primer string almacena la palabra en minuscula, y el siguiente map<documento, cantidad de veces>
    std::map<std::string, std::map<std::string, int>> indice;

    
    //Recorre todos los archivos de la carpeta
    for (const auto& entry : fs::directory_iterator(carpetaLibros)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::string nombreArchivo = entry.path().filename().string();
            std::cout << "Leyendo libro: " << nombreArchivo << "..." << std::endl;
            std::ifstream archivo(entry.path());

            if (!archivo.is_open()) {
                std::cerr << "No se pudo abrir: " << nombreArchivo << std::endl;
                continue;
            }

            std::string palabra;
            while (archivo >> palabra) {
                // Limpiar signos de puntuación y pasar a minúsculas
                std::string limpia;
                for (char c : palabra) {
                    if (std::isalnum(static_cast<unsigned char>(c))) {
                        limpia += std::tolower(static_cast<unsigned char>(c));
                    }
                }

                if (!limpia.empty()) {
                    indice[limpia][nombreArchivo]++;
                }
            }
            archivo.close();
        }
    }


    //Guardar indice en archivo
    std::ofstream salida(archivoSalida);
    if(!salida.is_open()){
        std::cerr << "Error: no se pudo crear el archivo de salida" << std::endl;
        return 1;
    }

     for (const auto& [palabra, docs] : indice) {
        salida << palabra;
        for (const auto& [doc, cant] : docs) {
            salida << ";(" << doc << "," << cant << ")";
        }
        salida << "\n";
    }

    salida.close();
    std::cout << "Indice invertido creado en: " << archivoSalida << std::endl;

    return 0;


}