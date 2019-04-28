#include <time.h>
#include <string>
#include "common_tiempo.h"
#define LARGO_TIEMPO_MAXIMO 21

/*Inicializa el tiempo en el horario actual.*/
Tiempo::Tiempo(){
    time(&this->tiempo);
}

/*Destruye el tiempo*/
Tiempo::~Tiempo(){}
/*
PRE: Recibe una cantidad (size_t) de dias a sumar.
POST: Suma la cantidad recibida al tiempo actual
*/
void Tiempo::sumar_dias(size_t dias){
    struct tm tiempoInfo; 
    gmtime_r(&this->tiempo, &tiempoInfo);
    tiempoInfo.tm_mday += dias; 
    this->tiempo = mktime(&tiempoInfo);
}

/*
Devuelve una representacion (std::string) del tiempo actual:
MM DD hh:mm:ss YYYY
*/
std::string Tiempo::representar(){
    std::string representacion;
    struct tm tiempoInfo; 
    gmtime_r(&this->tiempo, &tiempoInfo);
    char tiempoBuff[LARGO_TIEMPO_MAXIMO];
    strftime(tiempoBuff, sizeof(tiempoBuff), "%b %d %T %Y", &tiempoInfo);
    representacion = tiempoBuff;
    return std::move(representacion);
}