#include <string>
#include "common_error.h"
#include "common_protocolo.h"

/*
PRE: Recibe una referencia al socket de comunicacion.
POST: Inicializa un protocolo de comunicacion.
*/
Protocolo::Protocolo(Socket &skt) : skt(skt) {}

/*Destruye el protocolo*/
Protocolo::~Protocolo(){}

/*
PRE: Recibe un buffer con el mensaje (const char *) a enviar, 
el largo (uint32_t) del mensaje a enviar.
POST: Envia todo el mensaje recibido.
Levanta OSError en caso de error, si no se logro enviar todo 
el mensaje.
*/
void Protocolo::_enviar(const char* buffer, uint32_t largo){
    int bytesEnviados = 0;
    bytesEnviados = this->skt.enviar_todo(buffer, largo);
    if (bytesEnviados != (int)largo){
        std::string err = "Error: No se enviaron todos los bytes.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe un buffer donde se guardara el mensaje (char *) a 
recibir, el largo (uint32_t) del mensaje a recibir.
POST: Recibe y guardar el mensaje recibido en el buffer recibido.
Levanta OSError en caso de error, en caso de que no se haya recibido
el largo esperado.
*/
void Protocolo::_recibir(char *buffer, uint32_t largo){
    int bytesRecibidos = 0;
    bytesRecibidos = this->skt.recibir_algo(buffer, largo);
    if (bytesRecibidos !=  (int)largo){
        std::string err = "Error: No se recibieron todos los bytes.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Un mensaje a enviar (std::string &).
POST: Envia el mensaje, primero enviando la longitud
del mismo, seguido del mensaje.
Levanta OSError en caso de error.
*/
void Protocolo::enviar_mensaje(std::string &mensaje){ 
    size_t bytesLargo = 4;
    uint32_t largo = mensaje.size(); 
    uint32_t largoBE = htobe32(largo);
    char *buffer = (char*) &largoBE;
    this->_enviar(buffer, bytesLargo);
    this->_enviar(mensaje.data(), largo);
}

/*
PRE: Recibe un valor (uint32_t) y una cantidad de bytes 
a enviar:
1: un byte
2: dos bytes
4: cuatro bytes
POST: Envia lo bytes (recibidos) menos significativos del valor
recibido. 
Si bytes es 2 o 4, se envia en formato big endian.
*/
void Protocolo::enviar_bytes(uint32_t valor, size_t bytes){
    size_t unByte = 1;
    size_t dosBytes = 2;
    size_t cuatroBytes = 4;
    if (bytes == unByte){
        uint8_t valorEnviar = (valor & 0x000000FF);
        const char *buffer = (char *) &valorEnviar;
        this->_enviar(buffer, unByte);
    }
    if (bytes == dosBytes){
        uint16_t valorEnviar = (valor & 0x0000FFFF);
        uint16_t valorEnviarBE = htobe16(valorEnviar);
        const char *buffer = (char*) &valorEnviarBE;
        this->_enviar(buffer, dosBytes); 
    }
    if (bytes == cuatroBytes){
        uint32_t valorEnviar = valor;
        uint32_t valorEnviarBE = htobe32(valorEnviar);
        const char *buffer = (char *) &valorEnviarBE;
        this->_enviar(buffer, cuatroBytes); 
    }
}

/*
Devuelve un mensaje recibido (std::string).
Levanta OSError en caso de error. 
*/
std::string Protocolo::recibir_mensaje(){
    std::string mensaje;
    uint32_t largoMensaje = this->recibir_cuatro_bytes();
    char *buffer = new char[largoMensaje+1]; // +\0
    try {
        this->_recibir(buffer, largoMensaje);
    } catch (OSError &error){
        delete [] buffer;
        throw error;
    }
    buffer[largoMensaje] = 0;
    mensaje = buffer;
    delete [] buffer;
    return std::move(mensaje);
}

/*
Recibe 1 byte y lo devuelve (uint8_t).
Levante OSError en caso de error.
*/
uint8_t Protocolo::recibir_un_byte(){
    uint8_t valorRecibido = 0;
    this->_recibir((char*)&valorRecibido, 1);
    return valorRecibido;
}

/*
Recibe 2 bytes y los devuelve (uint16_t).
Levante OSError en caso de error.
*/
uint16_t Protocolo::recibir_dos_bytes(){
    uint16_t valorBE = 0;
    this->_recibir((char*)&valorBE, 2);
    return be16toh(valorBE);
}

/*
Recibe 4 bytes y los devuelve (uint32_t).
Levante OSError en caso de error.
*/
uint32_t Protocolo::recibir_cuatro_bytes(){
    uint32_t valorBE = 0;
    this->_recibir((char*)&valorBE, 4);
    return be32toh(valorBE);
}
