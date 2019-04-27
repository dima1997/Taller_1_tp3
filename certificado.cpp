#include "certificado.h"
#include "spliter.h"

/*
PRE: Recibe un entero sin signo de 4 bytes.
POST: Devuelve una representacion (std::string) 
hexagesimal del valor recibido.
*/
std::string a_hexa32_string(uint32_t &valor){
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
std::string a_hexa16_string(uint16_t &valor){
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
std::string a_hexa8_string(uint8_t &valor){
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

/*
PRE: Recibe las claves publicas del cliente (ClaveRSA &, con los 
expPublico y modulo correctos), y un vector de strings con informacion
de el sujeto del certificado, la fecha de inicio, y de final 
(en ese orden).
POST: Inicializa un certificado con dicha informacion. 
*/
Certificado::Certificado(ClaveRSA &clvClnt, std::vector<std::string> &info){
    this->numeroSerie = 0;
    this->asunto = "";
    this->sujeto = info[0];
    this->inicio = info[1];
    this->fin = info[2];
    this->exp = clvClnt.expPublico;
    this->mod = clvClnt.modulo;
}

/*Destruye un certificado*/
Certificado::~Certificado(){}

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
    while ((pos = linea.find('\t')) != std::string::npos) {
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
PRE: Recibe el nombre (std::string &) a un archivo que
tiene un certificado ya creado.
POST: Carga el certificado con la informacion del 
archivo.
Devuelve true si logro lo anterior, false en caso 
contrario.
*/
/*
bool Certificado::cargar(std::string &nombreArchivo){
    std::ifstream archCertif;
    archCertif.open(nombreArchivo);
    if (! archCertif.is_open()){
        return false;
    }
    std::string linea = 0;
    std::getline(archCertif, linea);
    while (archCertif.good()){
        this->_procesar_linea(linea);
        std::getline(archCertif, linea);
    }
    return true;
}
*/
/*
Crea un archivo <sujeto>.cert y guarda en el certificado actual.
*/
/*
bool Certificado::guardar(){ //Sobrecargar << , en esta clase o en una clase Archivo
    std::string certificado = 0;
    this->a_string(certificado);
    std::string terminacion = ".cert";
    return escribirArchivo(this->sujeto, terminacion, certificado);
}
*/
/*
PRE: Recibe una referencia a una string (std::string &).
POST: Copia en la referencia la representacion del certificado 
actual.
*/
void Certificado::a_string(std::string &cadena){
    std::string certificado = "certificate:\n";
    certificado += "\tserial number: " + this->numeroSerie; 
    certificado += a_hexa32_string(this->numeroSerie) + "\n"; 
    certificado += "\tsubject: " + this->sujeto + "\n";
    certificado += "\tissuer: " + this->asunto + "\n"; //Taller de programacion 1
    certificado += "\t​validity:​\n";
    certificado += "\t\tnot before: " + this->inicio + "\n";
    certificado += "\t\tnot after: " + this->fin + "\n";
    certificado += "\tsubject public key info:\n";
    certificado += "\t\tmodulus: " + std::to_string(this->mod);
    certificado += a_hexa16_string(this->mod) + "\n"; 
    certificado += "\t\texponent: " + std::to_string(this->exp); 
    certificado += a_hexa8_string(this->exp) + "\n"; 
    cadena = certificado;
}

/*Devuelve el hash (uint32_t) de certificado actual*/
uint32_t Certificado::hashear(){
    std::string certificado = 0;
    this->a_string(certificado);
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
        proto.recibir_mensaje(this->inicio);
        proto.recibir_dos_bytes(this->mod);
        proto.recibir_un_byte(this->exp);
    } catch (OSError &e){
        std::string err = "Error al recibir certificado."
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
        throw OSError(err);
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
Devuelve true si logro lo anterior; false en caso
contrario. 
*/
void Certificado::enviar_parametros(Protocolo &proto){
    //Enviamos el subject
    todoOK = proto.enviar_mensaje(this->sujeto);
    if (!todoOK) {
        return false;
    }

    ///Enviamos el modulo del cliente
    todoOK = proto.enviar_bytes(this->mod, 2);
    //2 bytes
    if (!todoOK) {
        return false;
    }

    //Enviamos el exponente publico del cliente
    todoOK = proto.enviar_bytes(this->exp, 1);
    //1 byte
    if (!todoOK) {
        return false;
    }
    //Enviamos la fecha de inicio del certificado
    todoOK = proto.enviar_mensaje(this->inicio);
    if (!todoOK) {
        return false;
    }

    //Enviamos fecha finalizacion del certificado 
    todoOK = proto.enviar_mensaje(this->fin);

    return todoOK;
}
