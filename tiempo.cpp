#include <time.h>
#include <string>
#include "tiempo.h"


/*Inicializa el tiempo en el horario actual.*/
Tiempo::Tiempo(){
    time(&this->tiempo);
}

/*Destruye el tiempo*/
Tiempo::~Tiempo(){}

/*Devuelve true, si el año del tiempo es bisiesto.*/
/*
bool Tiempo::es_bisiesto(){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    size_t anio = tiempoInfo->tm_year;
    return ( (anio % 4 == 0) && ( (! (anio % 100 == 0)) || (anio % 400 == 0) ) );
}
*/
/*Devuelve la cantidad (size_t) de dias del mes del tiempo actual.*/
/*
size_t Tiempo::cuantos_dias_mes(){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    size_t mes = tiempoInfo->tm_mon; 
    if (mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12){
        return 31;
    }
    if (mes==4 || mes==6 || mes==9 || mes==11){
        return 30;
    }
    // Es febrero: 2
    if (this->es_bisiesto()){
        return 29;
    }
    return 28;
}
*/
/*
PRE: Recibe una cantidad (size_t) de dias a sumar.
POST: Suma la cantidad recibida al tiempo actual
*/
void Tiempo::sumar_dias(size_t dias){
    struct tm tiempoInfo; //*
    //tiempoInfo = gmtime(&this->tiempo);
    gmtime_r(&this->tiempo, &tiempoInfo);
    /*
    size_t diasMesMaximo = this->cuantos_dias_mes();
    size_t diasActuales = tiempoInfo->tm_mday;
    size_t sumaDias = diasActuales + dias;
    if (sumaDias > diasMesMaximo){
        tiempoInfo->tm_mday = sumaDias - diasMesMaximo;
        tiempoInfo->tm_mon += 1;
    } else {
        tiempoInfo->tm_mday = sumaDias;
    }
    if (tiempoInfo->tm_mon > 11){ //0 a 11 meses (12)
        tiempoInfo->tm_mon = 0;
        tiempoInfo->tm_year += 1;
    }
    */
    tiempoInfo->tm_mday += dias;
    this->tiempo = mktime(tiempoInfo);
}

/*
Devuelve una representacion (std::string) del tiempo actual:
MM DD hh:mm:ss YYYY
*/
std::string Tiempo::representar(){
    std::string representacion;
    struct tm tiempoInfo; //*
    //tiempoInfo = gmtime(&this->tiempo);
    gmtime_r(&this->tiempo, &tiempoInfo);
    char tiempoBuff[LARGO_TIEMPO_MAXIMO]; //+1
    strftime(tiempoBuff, sizeof(tiempoBuff), "%b %d %T %Y", tiempoInfo);
    representacion = tiempoInfo;
    return std::move(representacion);
}
