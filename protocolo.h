#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <cstdint>
#include <string>
#include "socket.h"

class Protocolo {
    private: 
    Socket &skt;

    /*
    PRE: Recibe un buffer con el mensaje (const char *) a enviar, 
    el largo (uint32_t) del mensaje a enviar, y una pequeña 
    descripcion (const char *) de lo que se quiera mostrar en caso
    de error
    POST: Envia todo el mensaje recibido.
    Levanta OSError en caso de error, si no se logro enviar todo 
    el mensaje.
    */
    void _enviar(const char* buffer, uint32_t largo, const char* err);

    /*
    PRE: Recibe un buffer donde se guardara el mensaje (char *) a 
    recibir, el largo (uint32_t) del mensaje a recibir, y una pequeña 
    descripcion (const char *) de lo que se quiera mostrar en caso
    de error.
    POST: Recibe y guardar en el mensaje recibido en el buffer recibido.
    Levanta OSError en caso de error, en caso de que no se haya recibido
    el largo esperado.
    */
    void _recibir(char *buffer, uint32_t largo, const char* err);

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
    PRE: Recibe una referencia a una string (std::string &).
    POST: Devuelve el largo del mensaje recibido, si logro
    correctamente recibir y guardar el mensaje recibido en
    la referencia recibida.
    Levanta OSError en caso de error. 
    */
    uint32_t recibir_mensaje(std::string &mensaje);

    /*
    PRE: Recibe una referencia a un entero sin signo de 1 bytes
    (uint8_t &).
    POST: Recibe 1 byte y lo guarda en la referencia recibida.
    Levante OSError en caso de error.
    */
    void recibir_un_byte(uint8_t &valor);

    /*
    PRE: Recibe una referencia a un entero sin signo de 2 bytes
    (uint16_t &).
    POST: Recibe 2 bytes y lo guarda en la referencia recibida.
    Levante OSError en caso de error.
    */
    void recibir_dos_bytes(uint16_t &valor);

    /*
    PRE: Recibe una referencia a un entero sin signo de 4 bytes
    (uint32_t &).
    POST: Recibe 4 bytes y lo guarda en la referencia recibida.
    Levante OSError en caso de error.
    */
    void recibir_cuatro_bytes(uint32_t &valor);
};
#endif //PROTOCOLO_H
