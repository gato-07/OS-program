#ifndef SERVIDOR_MOTOR_H
#define SERVIDOR_MOTOR_H

#include "indice_invertido.h"
#include <string>

#define PUERTO_MOTOR 8002
#define TAMANIO_BUFFER 8192

class ServidorMotor {
private:
    IndiceInvertido indice;
    int socketServidor;
    
    void procesarComando(int socketCliente, const std::string& comando);
    void enviarRespuesta(int socketCliente, const std::string& respuesta);
    std::string serializarResultados(const std::vector<Resultado>& resultados);

public:
    ServidorMotor();
    ~ServidorMotor();
    
    bool cargarIndice(const std::string& archivoIndice);
    bool iniciar();
    void ejecutar();
};

#endif // SERVIDOR_MOTOR_H
