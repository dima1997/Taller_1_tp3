#include <cstdint>
#include <iostream>
#include <string>
#include "common.h"

/*
g++ -std=c++11 -ggdb -DDEBUG -fno-inline -Wall -Werror -pedantic -pedantic-errors common.cpp -o prueba
*/

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
        resultado += (uint8_t)cadena[i]; //entero sin signo de un byte
    }
    return resultado;
}

void hash_prueba_hola(){
    std::string hola = "hola\0";
    Hash hash;
    uint16_t resultado = hash.hashear(hola, hola.size());
    uint16_t hash_real = 420;
    bool todoOK = (resultado == hash_real);
    std::cout << hola << "\n";
    std::cout << "hash real: " << hash_real << "\n";
    std::cout << "hash calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";
}

void hash_prueba_enunciado(){
    std::string certificado = "certificate:\n\tserial number: 11 (0x0000000b)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 17 (0x11)\0";
    Hash hash;
    uint16_t resultado = hash.hashear(certificado, certificado.size());
    uint16_t hash_real = 20603;
    bool todoOK = (resultado == hash_real);
    std::cout << certificado << "\n";
    std::cout << "hash real: " << hash_real << "\n";
    std::cout << "hash calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";
}


void hash_prueba_certificados_iguales(){
    std::string certificado1 = "certificate:​\n\t​serial number: 11 (0x0000000b)​\n\t​subject: Federico Manuel Gomez Peter​\n\t​issuer: Taller de programacion 1​\n\t​validity:​\n\t\t​not before: Mar 28 21:33:04 2019​\n\t\t​not after: May 27 21:33:04 2019​\n\t​subject public key info:​\n\t\t​modulus: 253 (0x00fd)​\n\t\t​exponent: 17 (0x11)\0";
    std::string certificado2 = "certificate:\n\tserial number: 11 (0x0000000b)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 17 (0x11)\0";
    std::cout << "certificado 1: " << certificado1 << "\n";
    std::cout << "certificado 2: " << certificado2 << "\n";
    std::cout << "¿Son iguales? : " << (certificado2 == certificado1) << "\n";
    for (size_t i = 0; i < certificado1.size(); ++i){
        if (certificado1[i] != certificado2[i]){
            std::cout << "El caracter diferente es el n° " << i << "\n";
            std::cout << "Caracter diferente certificado 1 es : " << certificado1[i] << "\n";
            std::cout << "Caracter diferente certificado 2 es : " << certificado2[i] << "\n";  
        }
    }
}

//TDA Encriptador

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
        for (int j = 1; j < exp; ++j) { //+1
            resultado = (resultado * base) % mod;
        }
        retorno = retorno + (resultado << (i * 8));
    }
    return retorno;
}

void encriptador_prueba_hola(){
    std::string hola = "hola\0";
    Hash hash;
    uint16_t huella = hash.hashear(hola, hola.size());
    Encriptador encript;
    uint32_t resultado_real = 354;
    uint32_t resultado = encript.encriptar(huella, 19, 253);
    bool todoOK = (resultado == resultado_real);
    std::cout << "encriptado real: " << resultado_real << "\n";
    std::cout << "encriptado calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";

}

void encriptador_doble_prueba_certificado2(){
    std::string certificado2 = "certificate:\n\tserial number: 1 (0x00000001)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 19 (0x13)";
    Hash hash;
    uint16_t huella = hash.hashear(certificado2, certificado2.size());
    uint16_t resultado_hash_real = 20509;
    Encriptador encript; 
    uint32_t resultado1 = encript.encriptar(huella, 17, 253);
    uint32_t resultado1_real = 39039;
    uint32_t resultado2 = encript.encriptar(resultado1, 19, 253);
    uint32_t resultado2_real = 38011;
    std::cout << "hash_real : "<< resultado_hash_real << "\n";
    std::cout << "hash calculado : "<< huella << "\n";
    std::cout << "¿Son iguales hash? : "<< (resultado_hash_real == huella) << "\n";
    std::cout << "encriptado privado 1 real : "<< resultado1_real << "\n";   
    std::cout << "encriptado privado 1 calculado : "<< resultado1 << "\n";
    std::cout << "¿Son iguales encriptaciones 1? : "<< (resultado1_real == resultado1) << "\n";
    std::cout << "encriptado publico 2 real : "<< resultado2_real << "\n";   
    std::cout << "encriptado publico 2 calculado : "<< resultado2 << "\n";
    std::cout << "¿Son iguales encriptaciones 2? : "<< (resultado2_real == resultado2) << "\n";
}   


int main(){
    hash_prueba_hola();
    hash_prueba_enunciado();
    //hash_prueba_certificados_iguales();
    encriptador_prueba_hola();
    encriptador_doble_prueba_certificado2();
    return 0;
}