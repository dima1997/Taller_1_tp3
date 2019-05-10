#ifndef HASH_H
#define HASH_H
#include <cstdint>
#include <string>

class Hash {
public:
    /*Crea un Hash*/
    Hash();

    /*Destruye un Hash*/
    ~Hash();

    /*
    PRE: Recibe una cadena de caracteres (std::string &),
    y su largo (size_t).
    POST: Devuelve la suma (uint16_t) de todos los caracteres 
    de la cadena recibida, expresados como enteros sin signo 
    de 2 bytes.
    */
    uint16_t hashear(std::string &cadena, size_t largo);  
};

#endif //HASH_H
