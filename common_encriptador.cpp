#include "common_encriptador.h"

#include <cstdint>

/*Crea un encriptador*/
Encriptador::Encriptador(){}

/*Destruye un encriptador*/
Encriptador::~Encriptador(){}

/*
PRE: Recibe un huella: entero sin signo de 4 bytes 
(uint32_t); un exponente: entero sin signo de 1 bytes 
(uint8_t); y un modulo: entero sin signo de 2 bytes
(uint16_t).
POST: Devuelve la encriptacion de la huella recibida:
entero sin signo de 4 bytes (uint32_t), en base al 
exponente y modulo recibidos. 
*/
uint32_t Encriptador::encriptar(uint32_t huella, uint8_t exp, uint16_t mod){
    //El retorno es un entero de 4 bytes sin signo.
    uint32_t retorno = 0;
    //Recorro los bytes del mensaje a encriptar (que
    //es un número de 4 bytes sin signo).
    //Este mensaje puede ser el hash recibido o un 
    //valor previamente encriptado con otra clave.
    for (int i = 0; i < 4; ++i) {
        uint32_t resultado = (huella >> (i * 8)) & 0xff;;
        //Elevo el valor de la base (el valor del byte) 
        //al exponente público de la clave.
        uint32_t base = resultado;
        for (int j = 1; j < exp; ++j) {
            resultado = (resultado * base) % mod;
        }
        retorno = retorno + (resultado << (i * 8));
    }
    return retorno;
}
