#include "common_generador_certificados.h"

#include "common_certificado.h"
#include "common_mapa_bloq.h"
#include "common_contador_bloq.h"
#include "common_tiempo.h"
#include "common_claves.h"
#include "common_protocolo.h"
#include "common_error.h"

#include <fstream>
#include <iostream>

#define VIGENCIA_CERTIFICADO 30

/*
Crea un generador de certificados por defecto, 
con sujeto, fecha de inicio, de fin, y clave rsa 
nulos.
*/
GeneradorCertificados::GeneradorCertificados(){
    this->sujeto = "";
    this->fechaInicio = "";
    this->fechaFin = "";
}

/*
PRE: Recibe un archivo de entrada (std::istream &) que 
contenga informacion para generar un certificado de la 
forma:
<sujeto>
<fecha de inicio del certificado>
<fecha de finalizacion del certificado>
o
<sujeto>
Y las claves publicas (ClaveRSA &) del cliente que desea 
generar el certificado.
POST: Crea un generador de certificados. 
Si no se especificado fecha de inicio y finalizacion del 
certificado se tomara la fecha actual como inicio, la 30
dias despues la de finalizacion. 
*/
GeneradorCertificados::GeneradorCertificados(const char *nombreArchivoInfo, 
ClaveRSA &clavesCliente) : clavesCliente(std::move(clavesCliente)) { 
        std::ifstream in(nombreArchivoInfo, std::ios::in);
        if (! in.is_open()){
            std::string err = "Error al abrir archivo en generador.";
            throw OSError(__FILE__,__LINE__,err.data());
        }
        std::string linea;
        std::getline(in, linea);
        this->sujeto = std::move(linea);
        std::getline(in, linea);
        if (! in.good()){
            //No hay fecha de inicio, ni final
            Tiempo tiempo;
            this->fechaInicio = std::move(tiempo.representar());
            tiempo.sumar_dias(VIGENCIA_CERTIFICADO);
            this->fechaFin = std::move(tiempo.representar());
            return;
        }
        //Suponemos que abra tanto inicio como final
        this->fechaInicio = std::move(linea);
        std::getline(in, linea);
        this->fechaFin = std::move(linea);
}

/*Destruye un generador de certificados.*/
GeneradorCertificados::~GeneradorCertificados() {}

/*
PRE: Recibe una doble referencia a otro generador de 
certificados (GeneradorCertificados &&).
POST: Construye un nuevo generador de certificados por 
movimiento semantico de los atributos del recibido.
El generador de certificados recibidido queda en estado nulo. 
*/
GeneradorCertificados::GeneradorCertificados(GeneradorCertificados &&otroGenerador) 
: clavesCliente(std::move(otroGenerador.clavesCliente)) {
    this->sujeto = std::move(otroGenerador.sujeto);
    this->fechaInicio = std::move(otroGenerador.fechaInicio);
    this->fechaFin = std::move(otroGenerador.fechaFin);
    otroGenerador.sujeto = "";
    otroGenerador.fechaInicio = "";
    otroGenerador.fechaFin = "";
    otroGenerador.clavesCliente = std::move(ClaveRSA()); //Tal ves no sea necesario
}

/*
PRE: Recibe una doble referencia a otro generador de certificados 
(GeneradorCertificados &&).
POST: Mueve semanticamente todos los atributos del generador
recibido al actual.
El generador recibido queda en estado nulo.
Devuelve una referencia al actual generador 
(GeneradorCertificados &). 
*/
GeneradorCertificados& GeneradorCertificados::operator=(
GeneradorCertificados &&otroGenerador) {
    if (this == &otroGenerador) {
        return *this;
    }
    this->sujeto = std::move(otroGenerador.sujeto);
    this->fechaInicio = std::move(otroGenerador.fechaInicio);
    this->fechaFin = std::move(otroGenerador.fechaFin);
    this->clavesCliente = std::move(clavesCliente);
    otroGenerador.sujeto = "";
    otroGenerador.fechaInicio = "";
    otroGenerador.fechaFin = "";
    otroGenerador.clavesCliente = std::move(ClaveRSA()); //Tal ves no sea necesario
    return *this;
}

/*
PRE: Recibe un protocolo (Protocolo &) creado 
para enviar informacion a una autoridad certificante.
POST: Envia los parametros que la autoridad certificadora 
necesita para generar un certificado
Levanta OSError en caso de error. 
*/
void GeneradorCertificados::enviar_parametros(Protocolo &proto) {
    try {
        proto.enviar_mensaje(this->sujeto);
        this->clavesCliente.enviar_publico(proto);
        proto.enviar_mensaje(this->fechaInicio);
        proto.enviar_mensaje(this->fechaFin);
    } catch (OSError &e){
        std::string err = "Error al enviar parametros del certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe un protocolo (Protocolo &) creado 
para enviar informacion a una autoridad certificante.
POST: Recibe los parametros que la autoridad certifiadora 
necesita para generar un certificado
Levanta OSError en caso de error. 
*/
void GeneradorCertificados::recibir_parametros(Protocolo &proto) {
    try {
        this->sujeto = proto.recibir_mensaje();
        this->clavesCliente.recibir_publico(proto);
        this->fechaInicio = proto.recibir_mensaje();
        this->fechaFin = proto.recibir_mensaje();
    } catch (OSError &e){
        std::string err = "Error al recibir parametros del certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados claves RSA.
POST: Devuelve true si agrego un nuevo (que no existia antes) sujeto y clave 
al mapa; false en caso contrario (y no se agrego nada). 
*/
bool GeneradorCertificados::agregar_sujeto_clave_mapa(MapaBloq &sujetosClaves){
    return sujetosClaves.agregar_si_no_esta(this->sujeto, this->clavesCliente);
    //pasar a snake_case en mapBloq
}

/*
PRE: Recibe un contador (ContadorBloq &) de numeros de serie, 
y un asunto (std::string &) para el certificado.
POST: Genera  y devuelve un certificado (Certificado).
*/
Certificado GeneradorCertificados::generar(ContadorBloq &contador, std::string &asunto) {
    uint32_t numeroSerie = contador.obtener_y_sumar_uno();
    Certificado certif(numeroSerie, this->sujeto, asunto, this->fechaInicio,
        this->fechaFin, this->clavesCliente);
    return std::move(certif);
}

/*
PRE: Recibe un mapa bloqueante de sujetos y claves.
POST: Elimina del mapa al sujeto del ceritificado si es que esta.
*/
void GeneradorCertificados::borrar_sujeto_mapa(MapaBloq &sujetosClaves) {
    sujetosClaves.borrar(this->sujeto);
}