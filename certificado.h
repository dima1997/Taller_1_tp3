#ifndef CERTIFICADO_H
#define CERTIFICADO_H
#include <string>
#include <vector>
#include <fstream>
#include "protocolo.h"
#include "claves.h"
#include "hash.h"
#include "spliter.h"

/*
PRE: Recibe un entero sin signo de 4 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa32_string(uint32_t &valor);

/*
PRE: Recibe un entero sin signo de 2 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa16_string(uint16_t &valor);


/*
PRE: Recibe un entero sin signo de 1 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa8_string(uint8_t &valor);

class Certificado {
    public:
    uint32_t numeroSerie;
    std::string asunto;
    std::string sujeto;
    std::string inicio;
    std::string fin;
    uint8_t exp;
    uint16_t mod;

    /*Crea un certificado con todos sus atributos nulos*/
    Certificado();

    /*
    PRE: Recibe las claves publicas del cliente (ClaveRSA &, con los 
    expPublico y modulo correctos), y un vector de strings con informacion
    de el sujeto del certificado, la fecha de inicio, y de final 
    (en ese orden).
    POST: Inicializa un certificado con dicha informacion. 
    */
    Certificado(ClaveRSA &clvClnt, std::vector<std::string> &info);

    /*Destruye un certificado*/
    ~Certificado();

    /*
    PRE: Recibe una referencia a una linea (std::string &)
    de un archivo que contiene un certificado.
    POST: Procesa la linea guardando informacion de ella
    segun corresponda, para modelizar el certificado del
    archivo en la clase actual.
    */
    void _procesar_linea(std::string &linea);

    /*
    PRE: Recibe el nombre (std::string &) a un archivo que
    tiene un certificado ya creado.
    POST: Carga el certificado con la informacion del 
    archivo.
    Devuelve true si logro lo anterior, false en caso 
    contrario.
    */
    //bool cargar(std::string &nombreArchivo);

    /*
    Crea un archivo <sujeto>.cert y guarda en el certificado actual.
    */
    //bool guardar();

    /*
    PRE: Recibe una referencia a una string (std::string &).
    POST: Copia en la referencia la representacion del certificado 
    actual.
    */
    void a_string(std::string &cadena);
    
    /*Devuelve el hash (uint32_t) de certificado actual*/
    uint32_t hashear();

    /*
    PRE: Recibe un protocolo (Protocolo &) ya creadp.
    POST: Recibe y almacena la informacion de un 
    certificado en el orden en que el metodo enviar
    la envia, a traves del protocolo
    Devuelve true, si logro lo anterior con exito,
    false en caso contrario.
    */
    bool recibir(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) ya creado.
    POST: Envia cada campo del certificado en el orden que 
    aparecen de arriba hacia abajo un su representacion.,
    a traves del protocolo recibido.
    */
    bool enviar(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para recibir informacion de un cliente que 
    desee crear un certificado.
    POST: Recibe los parametros para crear el certificado
    en el orden que los envia el metodo enviar_parametros.
    Devuelve true si logro recibir y guardar en si mismo
    todos estos parametros recibidos; false en caso 
    contrario.
    */
    bool recibir_parametros(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para enviar informacion una autoridad certificante.
    POST: Envia los parametros que la autoridad necesita
    del certificado actual para certificarlo, en el orden
    que la autoridad certificante los espera.
    Devuelve true si logro lo anterior; false en caso
    contrario. 
    */
    bool enviar_parametros(Protocolo &proto);
};
#endif // CERTIFICADO_H 