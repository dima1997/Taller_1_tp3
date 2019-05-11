#ifndef CERTIFICADO_H
#define CERTIFICADO_H

#include "common_protocolo.h"
#include "common_claves.h"
#include "common_mapa_bloq.h"

#include <string>
#include <fstream>

class Certificado {
private:
    uint32_t numeroSerie;
    std::string sujeto;
    std::string asunto;
    std::string inicio;
    std::string fin;
    ClaveRSA clavesCliente;
    
    /*
    PRE: Recibe una referencia a una linea (std::string &)
    de un archivo que contiene un certificado, una referencia
    a un moduolo (uint16_t &) y una referencia a un exponente 
    (uin8_t &).
    POST: Procesa la linea guardando informacion de ella
    segun corresponda, para modelizar el certificado del
    archivo en la clase actual.
    Las lineas que correspondan al modulo y exponenete las procesa
    y guarda su valor en las referencias recibidas.
    */
    void _procesar_linea(std::string &linea, uint16_t &mod, uint8_t &exp);

public:
    /*
    PRE: Recibe :
    numero de serie (uint32_t)
    sujeto (std::string &)
    asunto (std::string &)
    fecha de inicio (std::string &)
    fecha de fin (std::string &)
    clave rsa publica (ClaveRSA &)
    POST: Inicializa un certificado.
    La clave rsa recibida queda en estado nulo.
    */
    Certificado(uint32_t numeroSerie, std::string &sujeto, std::string &asunto, 
    std::string &fechaInicio, std::string &fechaFin, ClaveRSA &clavesCliente);

    /*
    PRE: Recibe el nombre de un archivo que contenga una representacion 
    completa de un certificado.
    POST: Inicializa un certificado a partir de dicho archivo.
    Levanta OSError en caso de error. 
    */
    explicit Certificado(std::string &nombreArchivoCertif);

    /*
    PRE: Recibe un protocolo de comunicacion (Protocolo &).
    POST: Inicializa un Certificado a partir de la informacion
    recibida a traves del protocolo, enviada segun el metodo 
    enviar() de esta misma clase.
    */
    explicit Certificado(Protocolo &proto);

    /*Crea un certificado con todos sus atributos nulos*/
    Certificado();

    /*Destruye un certificado*/
    ~Certificado();

    // Move semantics

    /*
    PRE: Recibe una doble referencia a otro certificado
    (Certificado &&).
    POST: Construye un nuevo certificado por movimiento 
    semantico de los atributos del recibido.
    El certificado recibidido queda en estado nulo 
    */
    Certificado(Certificado &&otroCertif);

    /*
    PRE: Recibe una doble referencia a otro certificado
    (Certificado &&).
    POST: Mueve semanticamente todos los atributos del certificado
    recibido al actual.
    El certificado recibido queda en estado nulo. 
    */
    Certificado& operator=(Certificado &&otroCertif);

    // Copias

    /*
    PRE: Recibe otro certificado (const Certificado &).
    POST: Crea un nuevo certificado por copia.
    */
    Certificado(const Certificado &otroCertif) = delete;

    /*
    PRE: Recibe otro certificado (const Certificado &).
    POST: Asigna por copia los atributos del certificado 
    recibido al actual.
    Devuelve una referencia al actual certificado 
    (Certificado &).
    */
    Certificado& operator=(const Certificado &otroCertif) = delete;

    /*Devuelve una representacion (std::string) del certificado actual.*/
    std::string a_string() const;
    
    /*Devuelve el hash (uint32_t) de certificado actual*/
    uint32_t hashear();

    /*
    PRE: Recibe un enteros sin signo de 4 bytes (uint32_t).
    POST: Devuelve una encriptacion (uint32_t) del valor recibido 
    a partir de las claves publicas del cliente que estan en 
    el certificado.
    */
    uint32_t encriptar(uint32_t valor);

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
    Guardar una representacion del certificado en un archivo de nombre 
    "<sujeto del certificado>.cert"
    Levanta OSError en caso de error.
    */
    void guardar() const;

    /*
    PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados a 
    claves RSA; y una referencia a una claveRSA.
    POST: Devuelve true si existe un el sujeto del certificado en el mapa, 
    en cuyo caso copia, en la referencia recibida, la clave asociada; o false
    si no existe en el mapa.
    */
    bool obtener_sujeto_mapa(MapaBloq &sujetosClaves, ClaveRSA &copia);

    /*
    PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados a 
    claves RSA.
    POST: Borra al sujeto del certificado del mapa bloqueante.
    */
    void borrar_sujeto_mapa(MapaBloq &sujetosClaves);
};

#endif // CERTIFICADO_H
