#include <iostream>
#include <ctime>
#include <cstdint>
#include <arpa/inet.h>
#include "client.h"
#define LARGO_TIEMPO_MAXIMO 20
#define DIAS_SUMAR_INICIO_CERTIFICADO 30

typedef std::vector<std::string> stringVector;

/*Inicializa el tiempo en el horario actual.*/
Tiempo::Tiempo(){
    time_t tiempo;
    *(this->tiempo) = localtime(tiempo); 
}

/*Destruye el tiempo*/
Tiempo::~Tiempo(){}

/*Devuelve true, si el año del tiempo es bisiesto.*/
bool Tiempo::es_bisiesto(){
    size_t anio = this->tiempo-tm_year;
    return ( (anio % 4 == 0) && ( (! (anio % 100 == 0)) || (anio % 400 == 0) ) );
}
/*Devuelve la cantidad (size_t) de dias del mes del tiempo actual.*/
size_t Tiempo::cuantos_dias_mes(){
    size_t mes = this->tiempo.tm_month; 
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

/*
PRE: Recibe una cantidad (size_t) de dias a sumar.
POST: Suma la cantidad recibida al tiempo actual
*/
void Tiempo::sumar_dias(size_t dias){
    size_t diasMesMaximo = this->cuantos_dias_mes();
    size_t diasActuales = this->tiempo.tm_days;
    size_t sumaDias = diasActuales + dias;
    if (sumaDias > diasMesMaximo){
        this->tiempo.tm_days = sumaDias - diasMesMaximo;
        this->tiempo.tm_month += 1;
    } else {
        this->tiempo.tm_days = sumaDias;
    }
    if (this->tiempo.tm_month > 11){ //0 a 11 meses (12)
        this->tiempo.tm_month = 0;
        this->tiempo.tm_year += 1;
    }
}

/*
PRE: Recibe una referencia a una string (string &).
POST: Modifica el contenido de la string, 
reemplazandolo por una representacion del tiempo actual:
MM DD hh:mm:ss YYYY
*/
void Tiempo::representar(std::string &representacion){
    char tiempoBuff[LARGO_TIEMPO_MAXIMO]; //+1
    strftime(tiempoBuff, sizeof(tiempoActual), "%b %d %T %Y", this->tiempo);
    //representacion.erase(0, std::string::npos);
    representacion = tiempoBuff; 
}

/*
PRE: Recibe un socket ya conectado con el servidor 
de la autoridad certificante.
POST: Inicializa un certificado
*/
Ceertificado::Certificado(Socket &skt) : skt(skt){}

/*
Destruye un certificado
*/
Certificado::~Certificado(){}

/*
PRE: Recibe el nombre (string &) de un archivo que contengo 
informacion del certificado a crear : subject, fecha de inicio
fecha de finalizacion; y una referencia a un vector de strings 
(std::vector<std::string>).
POST: Carga el la informacion del archivo en el vector de 
recibido.
Devuelve true si logro lo anterior, false en caso contrario.
*/
bool Certificado::cargar_info(std::string &arch, stringVector &info){
    std::string contenido = 0;
    bool todoOK = cargarArchivo(nombreArch, contenido);
    if (! todoOK){
        return false;
    }
    split(contenido, "\n", info);
    if (info.size() < 1){
        //Archivo invalido
        return false;
    }
    if (info.size() < 2){
        //Suponemos que solo puede pasar esto, y no que puede estar la 
        //fecha de inicio, y no la de final
        Tiempo tiempoActual(tiempoActual);
        string fechaTemp = 0;
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
        tiempoActual.sumar_dias(30);
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
    }
    return true;

}

bool Certificado::enviar_info_creacion(stringVector &info, ClaveRSA &clvClnt){
    Protocolo proto(this->skt);
    
    //Enviamos el comando de creacion
    todoOK = proto.enviar_bytes(0,1);
    if (!todoOK) {
        return false;
    }
    //Enviamos el subject
    std::string &subject = info[0];
    uint32_t largoSubject = subject.size();
    const char* subjetoBuff = subject.data();
    todoOK = proto.enviar_mensaje(subjetoBuff, largoSubject);
    if (!todoOK) {
        return false;
    }

    ///Enviamos el modulo del cliente
    uint16_t modulo = claveCliente->modulo; 
    todoOK = proto.enviar_bytes(modulo, 2);
    //2 bytes
    if (!todoOK) {
        return false;
    }

    //Enviamos el exponente publico del cliente
    uint8_t expPublico = claveCliente->expPublico;
    todoOK = proto.enviar_bytes(expPublico, 1);
    //1 byte
    if (!todoOK) {
        return false;
    }
    //Enviamos la fecha de inicio del certificado
    std::string &fechaInicio = info[1];
    uint32_t largoFechaInicio = fechaInicio.size();
    const char *fechaInicioBuff = fechaInicio.data();
    todoOK = proto.enviar_mensaje(fechaInicioBuff, largoFechaInicio);
    if (!todoOK) {
        return false;
    }

    //Enviamos fecha finalizacion del certificado 
    std:string &fechaFin = info[2];
    uint32_t largoFechaFin = fechaFin.size();
    const char *fechaFinBuff = fechaFin.data();
    todoOK = proto.enviar_mensaje(fechaFinBuff, largoFechaFin);

    return todoOK;
}

bool Certificado::crear(std::string &clvsClnt, std::string &publSvr, std::string &infoCert){
    bool todoOK = true;
    stringVector infoVec(0);
    todoOK = this->cargar_info(infoCertif, infoVec);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveCliente(0,0,0);
    todoOK = claveCliente.cargar_claves(clvsClnt);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveServidor(0,0,0);
    todoOK = claveServidor.cargar_claves(publSvr);
    if (! todoOK){
        return false;
    }
    todoOK = this->enviar_info_creacion(infoVec, claveCliente);
    if (! todoOk){
        return false;
    }
    todoOK = this->recibir_certificado(claveCliente, claveServidor, infoVec);
    if (! todoOK){
        return false;
    }

    

}
//Mar 28 21:33:04 2019v