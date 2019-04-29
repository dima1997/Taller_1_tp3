#include <iostream>
#include <vector>
#include <string>
#include "common_hash.h"
#include "common_claves.h"
#include "common_spliter.h"

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
Devuelve una representacion (std::string) de la clave de la forma
"<exponente publico> <exponente privado> <modulo>"
*/
std::string ClaveRSA::a_string() const{
    std::string representacion;
    representacion += std::to_string(this->expPublico) + " ";
    representacion += std::to_string(this->expPrivado) + " ";
    representacion += std::to_string(this->modulo);
    return std::move(representacion);
}

/*
Devuelve una representacion (std::string) de la clave publica de 
la forma: "<exponente publico> <modulo>"
*/
std::string ClaveRSA::a_string_publica() const{
    std::string representacion;
    representacion += std::to_string(this->expPublico) + " ";
    representacion += std::to_string(this->modulo);
    return std::move(representacion);
}

/*
PRE: Recibe una cadena de caracteres (std::string &) que 
contiene las claves publicas y/o privadas de una entidad, 
de la forma:
<exponente-publico> <exponente-privado> <modulo>
o
<exponente-publico> <modulo>
POST: Actualiza los valores de la claves rsa, con los 
encontrados en el archivo. En el segundo caso, el exponente 
privado se actualiza a valor nulo.
*/
void ClaveRSA::actualizar(std::string &claveCadena){
    std::vector<std::string> partesClave;
    std::string separador = " ";
    Spliter spliter;
    partesClave = spliter.split(claveCadena, separador);
    this->expPublico = atoi(partesClave[0].data());
    if (partesClave.size() < 3){
        this->modulo = atoi(partesClave[1].data());
    } else {
        this->expPrivado = atoi(partesClave[1].data());
        this->modulo = atoi(partesClave[2].data());
    }
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


/*
PRE: Recibe el nombre de un flujo de entrada (std::istream &) 
que contiene las claves publicas y/o privadas de una entidad, 
de la forma:
<exponente-publico> <exponente-privado> <modulo>
o
<exponente-publico> <modulo>
POST: Actualiza los valores de la claves rsa, con los 
encontrados en el archivo.
*/
void ClaveRSA::cargar(std::istream &in){
    std::string linea;
    std::getline(in, linea);
    this->actualizar(linea);
}

/*
PRE: Recibe un flujo de salida (std::ostream &).
POST: Escribe la representacion completa de la clave
en el flujo:
<exponente publico> <exponente privado> <modulo>
*/
void ClaveRSA::guardar(std::ostream &out) const {
    out << this->a_string();
}

/*
Devuelve el exponente publico (uint8_t) de 
la clave
*/
uint8_t ClaveRSA::getExpPublico(){
    return this->expPublico;
}

/*Devuelve el modulo (uint16_t) de la clave*/
uint16_t ClaveRSA::getModulo(){
    return this->modulo;
}

/*Sobrecarga del operador >> para ClaveRSA*/
std::istream& operator>>(std::istream &in, ClaveRSA &clave){
    clave.cargar(in);
    return in;
}

/*Sobrecarga del operador << para ClaveRSA*/
std::ostream& operator<<(std::ostream &out, ClaveRSA &clave){
    clave.guardar(out);
    return out;
}