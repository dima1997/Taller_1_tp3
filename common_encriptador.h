#ifndef ENCRIPTADOR_H
#define ENCRIPTADOR_H
#include <cstdint>

class Encriptador {
public:
    /*Creo un encriptador*/
    Encriptador();

    /*Destruye un encriptador*/
    ~Encriptador();

    /*
    PRE: Recibe un huella: entero sin signo de 4 bytes 
    (uint32_t); un exponente: entero sin signo de 1 bytes 
    (uint8_t); y un modulo: entero sin signo de 2 bytes
    (uint16_t).
    POST: Devuelve la encriptacion de la huella recibida:
    entero sin signo de 4 bytes (uint32_t), en base al 
    exponente y modulo recibidos. 
    */
    uint32_t encriptar(uint32_t huella, uint8_t exp, uint16_t mod); 
};

#endif // ENCRIPTADOR_H
