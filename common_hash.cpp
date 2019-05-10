#include "common_hash.h"

#include <cstdint>
#include <string>

//TDA Hash

/*Crea un Hash*/
Hash::Hash(){}

/*Destruye un Hash*/
Hash::~Hash(){}

/*
PRE: Recibe una cadena de caracteres (std::string &),
y su largo (size_t).
POST: Devuelve la suma (uint16_t) de todos los caracteres 
de la cadena recibida, expresados como enteros sin signo 
de 2 bytes.
*/
uint16_t Hash::hashear(std::string &cadena, size_t largo){
    uint16_t resultado = 0;
    for (size_t i = 0; i < largo; ++i) {
        resultado += (uint8_t)cadena[i];
    }
    return resultado;
}
