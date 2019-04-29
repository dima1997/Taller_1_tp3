#include <string>
#include <vector>
#include <iostream>
#include "common_spliter.h"
#include "common_error.h"
#include "common_hash.h"
#include "common_tiempo.h"
#include "common_certificado.h"

/*
PRE: Recibe un entero sin signo de 4 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa32_string(const uint32_t &valor){
    char buffer[11]; //+0x + 8 letras +\0
    size_t largo = sizeof(buffer);
    snprintf(buffer, largo, "0x%08x", valor);
    return std::string(buffer);
}

/*
PRE: Recibe un entero sin signo de 2 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa16_string(const uint16_t &valor){
    char buffer[7]; //+0x + 4 letras + \0
    size_t largo = sizeof(buffer);
    snprintf(buffer, largo, "0x%04x", valor);
    return std::string(buffer);
}


/*
PRE: Recibe un entero sin signo de 1 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa8_string(const uint8_t &valor){
    char buffer[5]; //+0x + 2 letras + \0
    size_t largo = sizeof(buffer);
    snprintf(buffer, largo, "0x%02x", valor);
    return std::string(buffer);
}

/*Inicializa un certificado sin informacion.*/
Certificado::Certificado(){
    this->numeroSerie = 0;
    this->asunto = "";
    this->sujeto = "";
    this->inicio = "";
    this->fin = "";
    this->exp = 0;
    this->mod = 0;
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
Certificado::Certificado(Certificado &&otroCertif){
    this->numeroSerie = otroCertif.numeroSerie;
    this->asunto = otroCertif.asunto;
    this->sujeto = otroCertif.sujeto;
    this->inicio = otroCertif.inicio;
    this->fin = otroCertif.fin;
    this->exp = otroCertif.exp;
    this->mod = otroCertif.mod;
    otroCertif.numeroSerie = 0;
    otroCertif.asunto = "";
    otroCertif.sujeto = "";
    otroCertif.inicio = "";
    otroCertif.fin = "";
    otroCertif.exp = 0;
    otroCertif.mod = 0; 
}

/*
PRE: Recibe una doble referencia a otro certificado
(Certificado &&).
POST: Mueve semanticamente todos los atributos del certificado
recibido al actual.
El certificado recibido queda en estado nulo. 
*/
Certificado& Certificado::operator=(Certificado &&otroCertif){
    if (this== &otroCertif){
        return *this;
    }
    this->numeroSerie = otroCertif.numeroSerie;
    this->asunto = otroCertif.asunto;
    this->sujeto = otroCertif.sujeto;
    this->inicio = otroCertif.inicio;
    this->fin = otroCertif.fin;
    this->exp = otroCertif.exp;
    this->mod = otroCertif.mod;
    otroCertif.numeroSerie = 0;
    otroCertif.asunto = "";
    otroCertif.sujeto = "";
    otroCertif.inicio = "";
    otroCertif.fin = "";
    otroCertif.exp = 0;
    otroCertif.mod = 0; 
    return *this;
}

/*
PRE: Recibe un numero serie (uint32_t).
POST Setea en el certificaod el numero de serie recibido.
*/
void Certificado::setNumeroSerie(uint32_t valor){
    this->numeroSerie = valor;
}

/*
PRE: Recibe un asunto (std::string &).
POST: Setea en el certificado el asunto recibido.
*/
void Certificado::setAsunto(std::string &asunto){
    this->asunto = asunto;
}

/*
PRE: Recibe las claves publicas del cliente (ClaveRSA &).
POST: Actualiza la claves publicas del certificado 
*/
void Certificado::setClave(ClaveRSA &clave){
    this->exp = clave.getExpPublico();
    this->mod = clave.getModulo();
}

/*Devuelve el sujeto (std::string) del certificado*/
std::string Certificado::getSujeto(){
    std::string sujeto = this->sujeto;
    return std::move(sujeto);
}

/*Devuelve la clave publica del certificado*/
ClaveRSA Certificado::getClave(){
    ClaveRSA clave(this->exp, 0, this->mod);
    return std::move(clave);
}

/*Devuelve una representacion (std::string) del certificado actual.*/
std::string Certificado::a_string() const{
    std::string certificado = "certificate:\n";
    certificado += "\tserial number: " + std::to_string(this->numeroSerie); 
    certificado += " (" + a_hexa32_string(this->numeroSerie) + ")" + "\n"; 
    certificado += "\tsubject: " + this->sujeto + "\n";
    certificado += "\tissuer: " + this->asunto + "\n";
    certificado += "\tvalidity:\n";
    certificado += "\t\tnot before: " + this->inicio + "\n";
    certificado += "\t\tnot after: " + this->fin + "\n";
    certificado += "\tsubject public key info:\n";
    certificado += "\t\tmodulus: " + std::to_string(this->mod);
    certificado += " (" + a_hexa16_string(this->mod) + ")" + "\n"; 
    certificado += "\t\texponent: " + std::to_string(this->exp); 
    certificado += " (" + a_hexa8_string(this->exp) + ")"; 
    return std::move(certificado);
}

/*Devuelve el hash (uint32_t) de certificado actual*/
uint32_t Certificado::hashear(){
    std::string certificado = this->a_string();;
    Hash hash;
    return hash.hashear(certificado, certificado.size());
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
        proto.recibir_cuatro_bytes(this->numeroSerie);
        proto.recibir_mensaje(this->asunto);
        proto.recibir_mensaje(this->sujeto);
        proto.recibir_mensaje(this->inicio);
        proto.recibir_mensaje(this->fin);
        proto.recibir_dos_bytes(this->mod);
        proto.recibir_un_byte(this->exp);
    } catch (OSError &e){
        std::string err = "Error al recibir certificado.";
        throw OSError(err.data());
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
        proto.enviar_mensaje(this->asunto);
        proto.enviar_mensaje(this->sujeto);
        proto.enviar_mensaje(this->inicio);
        proto.enviar_mensaje(this->fin);
        proto.enviar_bytes(this->mod, 2);
        proto.enviar_bytes(this->exp, 1);
    } catch (OSError &e){
        std::string err = "Error al enviar certificado";
        throw OSError(err.data());
    }
}

/*
PRE: Recibe un protocolo (Protocolo &) creado 
para recibir informacion de un cliente que 
desee crear un certificado.
POST: Recibe los parametros para crear el certificado
en el orden que los envia el metodo enviar_parametros.
Levanta OSError en caso de error.
*/
void Certificado::recibir_parametros(Protocolo &proto){
    try {
        proto.recibir_mensaje(this->sujeto);
        proto.recibir_dos_bytes(this->mod);
        proto.recibir_un_byte(this->exp);
        proto.recibir_mensaje(this->inicio);
        proto.recibir_mensaje(this->fin);
    } catch (OSError &e){
        std::string err = "Error al recibir parametros de creacion";
        throw OSError(err.data());
    }
}

/*
PRE: Recibe un protocolo (Protocolo &) creado 
para enviar informacion una autoridad certificante.
POST: Envia los parametros que la autoridad necesita
del certificado actual para certificarlo, en el orden
que la autoridad certificante los espera.
Levanta OSError en caso de error. 
*/
void Certificado::enviar_parametros(Protocolo &proto){
    try {
        proto.enviar_mensaje(this->sujeto);
        proto.enviar_bytes(this->mod, 2);
        proto.enviar_bytes(this->exp, 1);
        proto.enviar_mensaje(this->inicio);
        proto.enviar_mensaje(this->fin);
    } catch (OSError &e){
        std::string err = "Erroa al enviar parametros del certificado.";
        throw OSError(err.data());
    }
}

/*
PRE: Recibe un flujo de entrada que contenga informacion del 
certificado a crear : subject, fecha de inicio, fecha de finalizacion.
POST: Carga en el certificado actual los datos anteriores.
*/
void Certificado::cargar_info(std::istream &in){
    std::string linea;
    std::getline(in, linea);
    this->sujeto = std::move(linea);
    std::getline(in, linea);
    if (! in.good()){
        //No hay fecha de inicio, ni final
        Tiempo tiempo;
        this->inicio = tiempo.representar();
        tiempo.sumar_dias(30);
        this->fin = tiempo.representar();
        return;
    }
    //Suponemos que abra tanto inicio como final
    this->inicio = std::move(linea);
    std::getline(in, linea);
    this->fin = std::move(linea);
    return;
}

/*
PRE: Recibe una referencia a una linea (std::string &)
de un archivo que contiene un certificado.
POST: Procesa la linea guardando informacion de ella
segun corresponda, para modelizar el certificado del
archivo en la clase actual.
*/
void Certificado::_procesar_linea(std::string &linea){
    size_t pos = 0;
    std::string lineaSinTabs = linea; 
    while ((pos = lineaSinTabs.find('\t')) != std::string::npos) {
        lineaSinTabs = lineaSinTabs.substr(pos+1);
    }
    Spliter spliter;
    std::vector<std::string> lineaSplit;
    std::string separador = ": ";
    lineaSplit = spliter.split(lineaSinTabs, separador);
    if (lineaSplit.size() < 2){
        return;
    }
    std::string campo = lineaSplit[0];
    std::string valor = lineaSplit[1];
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
    if (campo == "modulus"){
        std::vector<std::string> modulo;
        separador = " ";
        modulo = spliter.split(valor, separador);
        this->mod = (uint16_t) atoi(modulo[0].data());
        return;
    }
    if (campo == "exponent"){
        std::vector<std::string> exponente;
        separador = " ";
        exponente = spliter.split(valor, separador);
        this->exp = (uint8_t) atoi(exponente[0].data());
        return;
    }
    //Cualquier otra cosa la ignoramos
    //Suponemos que el certificado no va a tener errores.
    return;
} 

/*
PRE: Recibe un flujo de entrada (istream &) que contenga
un certificado ya creado.
POST: Carga el certificado actual con la informacion del 
flujo.
*/
void Certificado::cargar(std::istream &in){
    std::string linea;
    while (in.good()){
        std::getline(in, linea);
        this->_procesar_linea(linea);
    }
}

/*
PRE: Recibe un flujo de salida (ostream &).
POST: Escribe en flujo de salida, una representacion del 
certificado.
*/
void Certificado::guardar(std::ostream &out) const { 
    out << this->a_string();
}

/*Sobrecarga del operador >> de istream para clase Certificado*/
std::istream& operator>>(std::istream &in, Certificado &certif){
    certif.cargar(in);
    return in;
}

/*Sobrecarga del operador << de ostream para clase Certificado*/
std::ostream& operator<<(std::ostream &out, const Certificado &certif){
    certif.guardar(out);
    return out;
}

