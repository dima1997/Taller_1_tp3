#ifndef CLAVES_H
#define CLAVES_H
#include <cstdint>
#include <string>

struct ClaveRSA{
    public:
    uint8_t expPublico;
    uint8_t expPrivado;
    uint16_t modulo;

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

    //Destructor

    /*Destruye la clave rsa*/
    ~ClaveRSA();

    //Constructor, operador =  por copia

    ClaveRSA(const ClaveRSA &otraClave);
    /*
    PRE: Recibe una referencia a otra clave (ClaveRSA &).
    POST: Copia todos los atributos de clave recibida, a 
    la clave actual.
    Devuelve un referencia a la clave actual
    */
    ClaveRSA& operator=(const ClaveRSA &otraClave);

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
    PRE: Recibe el nombre de un flujo de entrada (std::istream &) 
    que contiene las claves publicas y/o privadas de una entidad, 
    de la forma:
    <exponente-publico> <exponente-privado> <modulo>
    o
    <exponente-publico> <modulo>
    POST: Actualiza los valores de la claves rsa, con los 
    encontrados en el archivo.
    */
    void cargar(std::istream &in);

    /*
    Devuelve el exponente publico (uint8_t) de 
    la clave
    */
    uint8_t getExpPublico();

    /*Devuelve el modulo (uint16_t) de la clave*/
    uint16_t getModulo();
};

/*Sobrecarga del operador >> para ClaveRSA*/
std::istream& operator>>(std::istream &in, ClaveRSA &clave);

#endif // CLAVES_H