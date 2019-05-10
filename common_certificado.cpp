#include "common_certificado.h"

#include "common_error.h"
#include "common_hash.h"
#include "common_encriptador.h"

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

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
Certificado::Certificado(uint32_t numeroSerie, std::string &sujeto, 
std::string &asunto, std::string &fechaInicio, std::string &fechaFin, 
ClaveRSA &clavesCliente) : numeroSerie(numeroSerie), sujeto(sujeto),
asunto(asunto), inicio(fechaInicio), fin(fechaFin), 
clavesCliente(std::move(clavesCliente)) {}

/*
PRE: Recibe el nombre de un archivo que contenga una representacion 
completa de un certificado.
POST: Inicializa un certificado a partir de dicho archivo.
Levanta OSError en caso de error. 
*/
Certificado::Certificado(std::string &nombreArchivoCertif){
    std::ifstream in(nombreArchivoCertif, std::ios::in);
    if (! in.is_open()){
        std::string err = "Error al abrir archivo de certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    std::string linea;
    uint16_t modulo;
    uint8_t exp;
    while (in.good()){
        std::getline(in, linea);
        this->_procesar_linea(linea, modulo, exp);
    }
    this->clavesCliente = std::move(ClaveRSA(exp, 0, modulo));
}

/*Inicializa un certificado sin informacion.*/
Certificado::Certificado(){
    this->numeroSerie = 0;
    this->sujeto = "";
    this->asunto = "";
    this->inicio = "";
    this->fin = "";
}

/*Destruye un certificado*/
Certificado::~Certificado(){}

/*
PRE: Recibe una doble referencia a otro certificado
(Certificado &&).
POST: Construye un nuevo certificado por movimiento 
semantico de los atributos del recibido.
El certificado recibidido queda en estado nulo 
*/
Certificado::Certificado(Certificado &&otroCertif) 
: clavesCliente(std::move(otroCertif.clavesCliente)){
    this->numeroSerie = otroCertif.numeroSerie;
    this->sujeto = otroCertif.sujeto;
    this->asunto = otroCertif.asunto;
    this->inicio = otroCertif.inicio;
    this->fin = otroCertif.fin;
    otroCertif.numeroSerie = 0;
    otroCertif.sujeto = "";
    otroCertif.asunto = "";
    otroCertif.inicio = "";
    otroCertif.fin = "";
}

/*
PRE: Recibe una doble referencia a otro certificado
(Certificado &&).
POST: Mueve semanticamente todos los atributos del certificado
recibido al actual.
El certificado recibido queda en estado nulo.
Devuelve una referencia al actual certificado 
(Certificado &). 
*/
Certificado& Certificado::operator=(Certificado &&otroCertif){
    if (this== &otroCertif){
        return *this;
    }
    this->numeroSerie = otroCertif.numeroSerie;
    this->sujeto = otroCertif.sujeto;
    this->asunto = otroCertif.asunto;
    this->inicio = otroCertif.inicio;
    this->fin = otroCertif.fin;
    this->clavesCliente = std::move(otroCertif.clavesCliente);
    otroCertif.numeroSerie = 0;
    otroCertif.sujeto = "";
    otroCertif.asunto = "";
    otroCertif.inicio = "";
    otroCertif.fin = "";
    return *this;
}

/*Devuelve una representacion (std::string) del certificado actual.*/
std::string Certificado::a_string() const{
    std::stringstream representacion;
    representacion << "certificate:\n";
    representacion << "\tserial number: " << std::dec << this->numeroSerie;
    representacion << " " << "("; 
    representacion << std::hex << std::setfill('0') << std::setw(8);
    representacion << this->numeroSerie << ")" << "\n";
    representacion << "\tsubject: " << this->sujeto << "\n";
    representacion << "\tissuer: " << this->asunto << "\n";
    representacion << "\tvalidity:\n";
    representacion << "\t\tnot before: " << this->inicio << "\n"; 
    representacion << "\t\tnot after: " << this->fin << "\n";
    representacion << "\tsubject public key info:\n";
    representacion << "\t\tmodulus: ";
    representacion << this->clavesCliente.representar_modulo() << "\n";
    representacion << "\t\texponent: ";
    representacion << this->clavesCliente.representar_exp_publico();
    return std::move(representacion.str());
}

/*Devuelve el hash (uint32_t) de certificado actual*/
uint32_t Certificado::hashear(){
    std::string certificado = this->a_string();;
    Hash hash;
    return hash.hashear(certificado, certificado.size());
}

/*
PRE: Recibe un enteros sin signo de 4 bytes (uint32_t).
POST: Devuelve una encriptacion (uint32_t) del valor recibido 
a partir de las claves publicas del cliente que estan en 
el certificado.
*/
uint32_t Certificado::encriptar(uint32_t valor){
    return this->clavesCliente.encriptar_publico(valor);
}

/*
PRE: Recibe un protocolo (Protocolo &) ya creadp.
POST: Recibe y almacena la informacion de un 
certificado en el orden en que el metodo enviar
la envia, a traves del protocolo
Levanta OSError en caso de error.
*/
void Certificado::recibir(Protocolo &proto){
    try {
        this->numeroSerie = proto.recibir_cuatro_bytes();
        this->sujeto = proto.recibir_mensaje();
        this->asunto = proto.recibir_mensaje();
        this->inicio = proto.recibir_mensaje();
        this->fin = proto.recibir_mensaje();
        this->clavesCliente.recibir_publico(proto);
    } catch (OSError &error){
        std::string err = "Error al recibir certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe un protocolo (Protocolo &) ya creado.
POST: Envia cada campo del certificado en el orden que 
aparecen de arriba hacia abajo un su representacion,
a traves del protocolo recibido.
Levanta OSError en caso de error.
*/
void Certificado::enviar(Protocolo &proto){
    try {
        proto.enviar_bytes(this->numeroSerie, 4);
        proto.enviar_mensaje(this->sujeto);
        proto.enviar_mensaje(this->asunto);
        proto.enviar_mensaje(this->inicio);
        proto.enviar_mensaje(this->fin);
        this->clavesCliente.enviar_publico(proto);
    } catch (OSError &error){
        std::string err = "Error al enviar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

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
void Certificado::_procesar_linea(std::string &linea, uint16_t &mod, uint8_t &exp){
    size_t pos = 0;
    std::string lineaSinTabs = linea; 
    while ((pos = lineaSinTabs.find('\t')) != std::string::npos) {
        lineaSinTabs = lineaSinTabs.substr(pos+1);
    }
    //

    std::stringstream lineaStream;
    lineaStream.str(lineaSinTabs);
    std::string campo;
    char separador = ':';
    std::getline(lineaStream, campo, separador);
    if (! lineaStream.good()){
        return;
    }
    std::string valor = lineaStream.str().substr(1); 
    // Para eliminar el espacio al ppio.

    //
    /*
    Spliter spliter;
    std::vector<std::string> lineaSplit;
    std::string separador = ": ";
    lineaSplit = spliter.split(lineaSinTabs, separador);
    if (lineaSplit.size() < 2){
        return;
    }
    std::string campo = lineaSplit[0];
    std::string valor = lineaSplit[1];
    */
    if (campo == "serial number"){
        this->numeroSerie = (uint32_t) atoi(valor.data());
        return;
    }
    if (campo == "issuer"){
        this->asunto = valor;
        return;
    }
    if (campo == "subject"){
        this->sujeto = valor;
        return;
    }
    if (campo == "not before"){
        this->inicio = valor;
        return;
    }
    if (campo == "not after"){
        this->fin = valor;
        return;
    }
    std::stringstream lineaValorStream;
    lineaValorStream.str(valor);
    char sep = ' ';
    if (campo == "modulus"){
        /*
        std::vector<std::string> modulo;
        separador = " ";
        modulo = spliter.split(valor, separador);
        */
        //
        std::string modulo;
        std::getline(lineaValorStream, modulo, sep);
        mod = (uint16_t) atoi(modulo.data());
        return;
    }
    if (campo == "exponent"){
        /*
        std::vector<std::string> exponente;
        separador = " ";
        exponente = spliter.split(valor, separador);
        */
        std::string exponente;
        std::getline(lineaValorStream, exponente, sep);
        exp = (uint8_t) atoi(exponente.data());
        return;
    }
    //Cualquier otra cosa la ignoramos
    //Suponemos que el certificado no va a tener errores.
    return;
} 

/*
Guardar una representacion del certificado en un archivo de nombre 
"<sujeto del certificado>.cert"
Levanta OSError en caso de error.
*/
void Certificado::guardar() const {
    std::string nombreArchivoCertif = this->sujeto + ".cert";
    std::ofstream out(nombreArchivoCertif, std::ios::out);
    if (! out.is_open()){
        std::string err = "Error al abrir archivo donde guardar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    out << this->a_string();   
}

/*
PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados a 
claves RSA; y una referencia a una claveRSA.
POST: Devuelve true si existe un el sujeto del certificado en el mapa, 
en cuyo caso copia, en la referencia recibida, la clave asociada; o false
si no existe en el mapa.
*/
bool Certificado::obtener_sujeto_mapa(MapaBloq &sujetosClaves, ClaveRSA &copia){
    return sujetosClaves.obtener_si_esta(this->sujeto, copia);
}

/*
PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados a 
claves RSA.
POST: Borra al sujeto del certificado del mapa bloqueante.
*/
void Certificado::borrar_sujeto_mapa(MapaBloq &sujetosClaves){
    sujetosClaves.borrar(this->sujeto);
}
