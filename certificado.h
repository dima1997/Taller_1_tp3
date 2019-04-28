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
std::string a_hexa32_string(const uint32_t &valor);

/*
PRE: Recibe un entero sin signo de 2 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa16_string(const uint16_t &valor);


/*
PRE: Recibe un entero sin signo de 1 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa8_string(const uint8_t &valor);

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
    PRE: Recibe un numero serie (uint32_t).
    POST Setea en el certificaod el numero de serie recibido.
    */
    void setNumeroSerie(uint32_t valor);

    /*
    PRE: Recibe un asunto (std::string &).
    POST: Setea en el certificado el asunto recibido.
    */
    void setAsunto(std::string &asunto);

    /*
    PRE: Recibe las claves publicas del cliente (ClaveRSA &).
    POST: Actualiza la claves publicas del certificado 
    */
    void setClave(ClaveRSA &clave);

    /*Devuelve el sujeto (std::string) del certificado*/
    std::string getSujeto();

    /*Devuelve la clave publica del certificado*/
    ClaveRSA getClave();

    /*Devuelve una representacion (std::string) del certificado actual.*/
    std::string a_string() const;
    
    /*Devuelve el hash (uint32_t) de certificado actual*/
    uint32_t hashear();

    /*
    PRE: Recibe un protocolo (Protocolo &) ya creadp.
    POST: Recibe y almacena la informacion de un 
    certificado en el orden en que el metodo enviar
    la envia, a traves del protocolo
    Levanta OSError en caso de error.
    */
    void recibir(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) ya creado.
    POST: Envia cada campo del certificado en el orden que 
    aparecen de arriba hacia abajo un su representacion,
    a traves del protocolo recibido.
    Levanta OSError en caso de error.
    */
    void enviar(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para recibir informacion de un cliente que 
    desee crear un certificado.
    POST: Recibe los parametros para crear el certificado
    en el orden que los envia el metodo enviar_parametros.
    Levanta OSError en caso de error.
    */
    void recibir_parametros(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para enviar informacion una autoridad certificante.
    POST: Envia los parametros que la autoridad necesita
    del certificado actual para certificarlo, en el orden
    que la autoridad certificante los espera.
    Levanta OSError en caso de error. 
    */
    void enviar_parametros(Protocolo &proto);

    /*
    PRE: Recibe una referencia a una linea (std::string &)
    de un archivo que contiene un certificado.
    POST: Procesa la linea guardando informacion de ella
    segun corresponda, para modelizar el certificado del
    archivo en la clase actual.
    */
    void _procesar_linea(std::string &linea);

    /*
    PRE: Recibe un flujo de entrada que contenga informacion del 
    certificado a crear : subject, fecha de inicio, fecha de finalizacion.
    POST: Carga en el certificado actual los datos anteriores.
    */
    void cargar_info(std::istream &in);

    /*
    PRE: Recibe un flujo de entrada (istream &) que contenga
    un certificado ya creado.
    POST: Carga el certificado actual con la informacion del 
    flujo.
    */
    void cargar(std::istream &in);

    /*
    PRE: Recibe un flujo de salida (ostream &).
    POST: Escribe en flujo de salida, una representacion del 
    certificado.
    */
    void guardar(std::ostream &out) const;
};
#endif // CERTIFICADO_H 