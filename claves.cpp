#include "hash.h"
#include "claves.h"

/*Crea una clave por default, con todos sus atributos nulos.*/
ClaveRSA::ClaveRSA() : expPublico(0), expPrivado(0), modulo(0){}

/*
PRE: REcibe un exponente publico: entero sin 
signo de 1 byte (uint8_t) entero sin signo de 
2 bytes, un exponente privado: entero sin 
signo de 1 bytes (uint8_t) y de un modulo: 
entero sin signo de 2 bytes (uint16_t). 
POST: Crea una clave rsa.
*/
ClaveRSA::ClaveRSA(uint8_t expPublico, uint8_t expPrivado, uint16_t modulo)
: expPublico(expPublico), expPrivado(expPrivado), modulo(modulo){}

/*Destruye la clave rsa*/
ClaveRSA::~ClaveRSA(){}


ClaveRSA::ClaveRSA(const ClaveRSA &otraClave){
    this->expPublico = otraClave.expPublico;
    this->expPrivado = otraClave.expPrivado;
    this->modulo = otraClave.modulo;
}
/*
PRE: Recibe una referencia a otra clave (ClaveRSA &).
POST: Copia todos los atributos de clave recibida, a 
la clave actual.
Devuelve un referencia a la clave actual
*/
ClaveRSA& ClaveRSA::operator=(const ClaveRSA &otraClave){
    if (this == &otraClave){
        return *this;
    }
    this->expPublico = otraClave.expPublico;
    this->expPrivado = otraClave.expPrivado;
    this->modulo = otraClave.modulo;
    return *this;
}

/*
PRE: Recibe una doble referencia a otra clave 
(ClaveRSA &&).
POST: Crea una nueva clave por movimiento semantico,
dejando a la recibida con atributos nulos.
*/
ClaveRSA::ClaveRSA(ClaveRSA&& otraClave){
    this->expPublico = otraClave.expPublico;
    this->expPrivado = otraClave.expPrivado;
    this->modulo = otraClave.modulo;
    otraClave.expPublico = 0;
    otraClave.expPrivado = 0;
    otraClave.modulo = 0;
}

/*
PRE: Recibe una doble referencias a otra clave (ClaveRSA &&).
POST: Asigna por movimiento semantico todos los atributos de 
la clave recibida, a la clave actual. La clave recibida queda
con atributos nulos.
Devuelve una referencia a la clave actual.
*/
ClaveRSA& ClaveRSA::operator=(ClaveRSA&& otraClave){
    if (this == &otraClave){
        return *this;
    }
    this->expPublico = otraClave.expPublico;
    this->expPrivado = otraClave.expPrivado;
    this->modulo = otraClave.modulo;
    otraClave.expPublico = 0;
    otraClave.expPrivado = 0;
    otraClave.modulo = 0;
    return *this;
}

/*
PRE: Recibe el nombre de un archivo que contiene las claves
publicas y/o privadas de una entidad, de la forma:
<exponente-publico> <exponente-privado> <modulo>
o
<exponente-publico> <modulo>
POST: Actualiza los valores de la claves rsa, con los 
encontrados en el archivo.
Devuelve true si logro lo anterior, false en caso contrario.
*/
bool ClaveRSA::cargar_claves(std::string &nombreArchivo){
    std::string todasClaves = 0;
    bool todoOK = true;
    todoOK = cargarArchivo(nombreArchivo, todasClaves);
    if (! todoOK){
        return false;
    }
    std::vector<std::string> partesClaves(0);
    std::string separador = " ";
    split(todasClaves, separador, partesClaves);
    this->expPublico = atoi(partesClaves[0].data());
    if (partesClaves.size() < 3){
        this->modulo = atoi(partesClaves[1].data());
    } else {
        this->expPrivado = atoi(partesClaves[1].data());
        this->modulo = atoi(partesClaves[2].data());
    }
    return true;
}

/*
PRE: Recibe un valor entero sin signo de 4 bytes
POST: Devuelve una huella (uint32_t) del valor recibido
encriptado con el exponente publico.
*/
uint32_t ClaveRSA::encriptar_publico(uint32_t valor){
    Encriptador encrip;
    return encrip.encriptar(valor, this->expPublico, this->modulo);
}

/*
PRE: Recibe un valor entero sin signo de 4 bytes
POST: Devuelve una huella (uint32_t) del valor recibido
encriptado con el exponente privado.
*/
uint32_t ClaveRSA::encriptar_privado(uint32_t valor){
    Encriptador encrip;
    return encrip.encriptar(valor, this->expPrivado, this->modulo);
}