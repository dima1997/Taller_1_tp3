#ifndef CLAVES_H
#define CLAVES_H

#include "common_protocolo.h"

#include <cstdint>
#include <string>

struct ClaveRSA {
private:
    uint8_t expPublico;
    uint8_t expPrivado;
    uint16_t modulo;
public:
    //Constructor por default

    /*Crea una clave por default, con todos sus atributos nulos.*/
    ClaveRSA();

    //Constructor completo

    /*
    PRE: Recibe un exponente publico: entero sin 
    signo de 1 byte (uint8_t) entero sin signo de 
    2 bytes, un exponente privado: entero sin 
    signo de 1 bytes (uint8_t) y de un modulo: 
    entero sin signo de 2 bytes (uint16_t). 
    POST: Crea una clave rsa.
    */
    ClaveRSA(uint8_t expPublico, uint8_t expPrivado, uint16_t modulo);

    /*
    PRE: Recibe el nombre de un archivo que contenga claves de la forma:
    <exponente publico> <exponente privado> <modulo>
    o
    <exponente publico> <modulo>
    POST: Inicializa una clave RSA a partir de la informacion del archivo.
    */
    ClaveRSA(std::string &nombreArchivoClaves);

    //Destructor

    /*Destruye la clave rsa*/
    ~ClaveRSA();

    //No permite copias implicitas

    ClaveRSA(const ClaveRSA &otraClave) = delete;

    ClaveRSA& operator=(const ClaveRSA &otraClave) = delete;

    /*
    Copia explicita.
    PRE: Recibe una referencia a otra clave (ClaveRSA &).
    POST: Copia todos los atributos de clave recibida, a 
    la clave actual.
    */
    void copiarDesde(const ClaveRSA &otraClave);

    //Constructor, operador =  por moviemiento semantico

    /*
    PRE: Recibe una doble referencia a otra clave 
    (ClaveRSA &&).
    POST: Crea una nueva clave por movimiento semantico,
    dejando a la recibida con atributos nulos.
    */
    ClaveRSA(ClaveRSA&& otraClave);

    /*
    PRE: Recibe una doble referencias a otra clave (ClaveRSA &&).
    POST: Asigna por movimiento semantico todos los atributos de 
    la clave recibida, a la clave actual. La clave recibida queda
    con atributos nulos.
    Devuelve una referencia a la clave actual.
    */
    ClaveRSA& operator=(ClaveRSA&& otraClave);

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
    void actualizar(std::string &claveCadena);

    /*
    Devuelve una representacion (std::string) de la clave de la forma
    "<exponente publico> <exponente privado> <modulo>"
    */
    std::string a_string() const;

    /*
    Devuelve una representacion (std::string) de la clave publica de 
    la forma: "<exponente publico> <modulo>"
    */
    std::string a_string_publica() const;

    /*
    PRE: Recibe un valor entero sin signo de 4 bytes
    POST: Devuelve una huella (uint32_t) del valor recibido
    encriptado con el exponente publico.
    */
    uint32_t encriptar_publico(uint32_t valor);

    /*
    PRE: Recibe un valor entero sin signo de 4 bytes
    POST: Devuelve una huella (uint32_t) del valor recibido
    encriptado con el exponente privado.
    */
    uint32_t encriptar_privado(uint32_t valor);

    /*
    PRE: Recibe un protocolo de comunicacion (Protocolo &).
    POST: Envia a traves del protocolo :
        1°) Su modulo, como entero sin signo de 2 bytes
        2°) Su exponente, como entero sin signo de 1 byte.
    Levanta OSError en caso de error. 
    */
    void enviar_publico(Protocolo &proto);

    /*
    PRE: Recibe un protocolo de comunicacion (Protocolo &).
    POST: Recibe (y se carga), a traves del protocolo : 
        1°) Un modulo, como entero sin signo de 2 bytes
        2°) Un exponente publico, como entero sin signo de 1 byte.
    Levanta OSError en caso de error. 
    */
    void recibir_publico(Protocolo &proto);

    /*
    Devuelve una representacion del modulo de la clave:
    <modulo en decimal> (<modulo en hexagesimal>)
    */
    std::string representar_modulo() const;

    /*
    Devuelve una representacion del exponente publico de la clave:
    <exponente publico en decimal> (<exponente publico en hexagesimal>)
    */
    std::string representar_exp_publico() const;
};

#endif // CLAVES_H

