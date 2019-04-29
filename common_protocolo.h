#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <cstdint>
#include <string>
#include "common_socket.h"

class Protocolo {
    private: 
    Socket &skt;

    /*
    PRE: Recibe un buffer con el mensaje (const char *) a enviar, 
    el largo (uint32_t) del mensaje a enviar.
    POST: Envia todo el mensaje recibido.
    Levanta OSError en caso de error, si no se logro enviar todo 
    el mensaje.
    */
    void _enviar(const char* buffer, uint32_t largo);

    /*
    PRE: Recibe un buffer donde se guardara el mensaje (char *) a 
    recibir, el largo (uint32_t) del mensaje a recibir.
    POST: Recibe y guardar el mensaje recibido en el buffer recibido.
    Levanta OSError en caso de error, en caso de que no se haya recibido
    el largo esperado.
    */
    void _recibir(char *buffer, uint32_t largo);

    public:
    /*
    PRE: Recibe una referencia al socket de comunicacion.
    POST: Inicializa un protocolo de comunicacion.
    */
    explicit Protocolo(Socket &skt);

    /*Destruye el protocolo*/
    ~Protocolo();

    /*
    PRE: Un mensaje a enviar (std::string &).
    POST: Envia el mensaje, primero enviando la longitud
    del mismo, seguido del mensaje.
    Levanta OSError en caso de error.
    */
    void enviar_mensaje(std::string &mensaje); 

    /*
    PRE: Recibe un valor (uint32_t) y una cantidad de bytes 
    a enviar:
    1: un byte
    2: dos bytes
    4: cuatro bytes
    POST: Envia lo bytes (recibidos) menos significativos del valor
    recibido. 
    Si bytes es 2 o 4, se envia en formato big endian.
    */
    void enviar_bytes(uint32_t valor, size_t bytes);

    /*
    Devuelve un mensaje recibido (std::string).
    Levanta OSError en caso de error. 
    */
    std::string recibir_mensaje();

    /*
    Recibe 1 byte y lo devuelve (uint8_t).
    Levante OSError en caso de error.
    */
    uint8_t recibir_un_byte();

    /*
    Recibe 2 bytes y los devuelve (uint16_t).
    Levante OSError en caso de error.
    */
    uint16_t recibir_dos_bytes();

    /*
    Recibe 4 bytes y los devuelve (uint32_t).
    Levante OSError en caso de error.
    */
    uint32_t recibir_cuatro_bytes();
};
#endif //PROTOCOLO_H
