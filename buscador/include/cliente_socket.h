#ifndef CLIENTE_SOCKET_H
#define CLIENTE_SOCKET_H

#include <string>

class ClienteSocket {
private:
    std::string host;
    int puerto;

public:
    ClienteSocket(const std::string& h, int p);
    bool enviarComando(const std::string& comando, std::string& respuesta, int timeoutSeg = 5);
};

#endif // CLIENTE_SOCKET_H
