#ifndef CLIENT_H
#define CLIENT_H
#include <ctime>
#include "common.h"

class Tiempo {
    public:
    struct tm *tiempo;
    /*Inicializa el tiempo en el horario actual.*/
    Tiempo(time_t tiempo);

    /*Destruye el tiempo*/
    ~Tiempo();

    /*Devuelve true, si el año del tiempo es bisiesto.*/
    bool es_bisiesto();

    /*Devuelve la cantidad (size_t) de dias del mes del tiempo actual.*/
    size_t cuantos_dias_mes();

    /*
    PRE: Recibe una cantidad (size_t) de dias a sumar.
    POST: Suma la cantidad recibida al tiempo actual
    */
    void sumar_dias(size_t dias);

    /*
    PRE: Recibe una referencia a una string (string &).
    POST: Modifica el contenido de la string, 
    reemplazandolo por una representacion del tiempo actual:
    MM DD hh:mm:ss YYYY
    */
    void Tiempo::representar(std:string &representacion);
};

class Cliente {
    public:
    Socket *skt;
    /*
    PRE: Recibe los nombres (std::string &) del host y
    puerto al que se conectara. 
    POST: Inicializa un cliente que pide crear y revocar
    certificados.
    */
    Cliente(str &host, str &puerto);
    
    /*Destruye al cliente.*/
    ~Cliente();
    
    /*
    PRE: Recibe el nombre (std::string &) del archivo donde estan 
    las claves del cliente , el nombre (std::string &) del archivo 
    donde estan las claves publicas del servidor de la autoridad 
    certificante, y el nombre (std::string &) del archivo donde 
    esta la informacion para crear el certificado.
    POST: Devuelve true, si logro crear un certificado 
    correctamente, false en caso contrario.
    */
    bool Cliente::crear(str &ifCert, str &clvsClnt, str &pblSvr);

    /*
    PRE: Recibe un vector de strings con la informacion
    para crear el certificado (subject, fecha de inicio, 
    fecha de finalizacion) (stringVector &); y una clave
    rsa (ClaveRSA &).
    POST: Envia los parametros necesarios, en el orden 
    correcto, al servidor para crear un certificado.
    */
    bool Cliente::enviar_info_creacion(strVec &info, ClaveRSA &clvClnt);
    
    /*
    PRE: Recibe las claves del cliente y el servidor (ClaveRSA &), y el subject
    (std::string &) del certificado usado para crearlo.
    POST: Devuelve true si logro recibir y procesar las respuesta del servidor
    a la creacion de un certificado (comprueba hashes y guarda certificado); 
    devuelve false en caso de algun error ajeno los esperados.
    */
    bool Cliente::recibir_certif(ClaveRSA &clvClt, ClaveRSA &clvSvr, str &sbjct);
    
    bool revocar(str &nombreCertif, str &nombreClvClnt, str &nombreClvSvr)
}

#endif // CLIENT_H
