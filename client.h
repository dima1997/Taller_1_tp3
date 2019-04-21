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


class Certificado {
    public:
    Socket &skt;
    Certificado(Socket &skt);
    ~Certificado();

    crear(std::string &clvsClnt, std::string &publSvr, std::string &infoCert);
    
};

#endif // CLIENT_H
