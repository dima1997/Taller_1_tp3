#include <string>
#include "protocolo.h"
#include "error.h"

/*
PRE: Recibe una referencia al socket de comunicacion.
POST: Inicializa un protocolo de comunicacion.
*/
Protocolo::Protocolo(Socket &skt) : skt(skt) {}

/*Destruye el protocolo*/
Protocolo::~Protocolo(){}

/*
PRE: Recibe un buffer con el mensaje (const char *) a enviar, 
el largo (uint32_t) del mensaje a enviar, y una pequeña 
descripcion (const char *) de lo que se quiera mostrar en caso
de error
POST: Envia todo el mensaje recibido.
Levanta OSError en caso de error, si no se logro enviar todo 
el mensaje.
*/
void Protocolo::_enviar(const char* buffer, uint32_t largo, const char* err){
    int bytesEnviados = 0;
    try {
        bytesEnviados = this->skt.enviar_todo(buffer, largo);
    } catch (OSError &e){
        throw OSError(err);
    }
    if (bytesEnviados != (int)largo){
        throw OSError(err);
    }
}

/*
PRE: Recibe un buffer donde se guardara el mensaje (char *) a 
recibir, el largo (uint32_t) del mensaje a recibir, y una pequeña 
descripcion (const char *) de lo que se quiera mostrar en caso
de error.
POST: Recibe y guardar en el mensaje recibido en el buffer recibido.
Levanta OSError en caso de error, en caso de que no se haya recibido
el largo esperado.
*/
void Protocolo::_recibir(char *buffer, uint32_t largo, const char* err){
    int bytesRecibidos = 0;
    try {
        bytesRecibidos = this->skt.recibir_algo(buffer, largo);
    } catch (OSError &e){
        throw OSError(err);
    }
    if (bytesRecibidos !=  (int)largo){
        throw OSError(err);
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
    this->_enviar(buffer, bytesLargo, "largo del mensaje");
    this->_enviar(mensaje.data(), largo, "mensaje");
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
    std::string err = "Error al enviar ";
    if (bytes == 1){
        uint8_t valorEnviar = (valor & 0x000000FF);
        const char *buffer = (char *) &valorEnviar;
        err += "un byte";
        this->_enviar(buffer, 1, err.data());
    }
    if (bytes == 2){
        uint16_t valorEnviar = (valor & 0x0000FFFF);
        uint16_t valorEnviarBE = htobe16(valorEnviar);
        const char *buffer = (char*) &valorEnviarBE;
        err += "dos bytes";
        this->_enviar(buffer, 2, err.data()); 
    }
    if (bytes == 4){
        uint32_t valorEnviar = valor;
        uint32_t valorEnviarBE = htobe32(valorEnviar);
        const char *buffer = (char *) &valorEnviarBE;
        err += "cuatro bytes"; 
        this->_enviar(buffer, 4, err.data()); 
    }
}

/*
PRE: Recibe una referencia a una string (std::string &).
POST: Devuelve el largo del mensaje recibido, si logro
correctamente recibir y guardar el mensaje recibido en
la referencia recibida.
Levanta OSError en caso de error. 
*/
uint32_t Protocolo::recibir_mensaje(std::string &mensaje){
    std::string err = "Error al recibir mensaje";
    uint32_t largoMensaje = 0;
    this->recibir_cuatro_bytes(largoMensaje);
    char *buffer = new char[largoMensaje];
    try {
        this->_recibir(buffer, largoMensaje, err.data());
    } catch (OSError &e){
        delete buffer;
        throw e;
    }
    mensaje = buffer;
    delete buffer;
    return largoMensaje;
}

/*
PRE: Recibe una referencia a un entero sin signo de 1 bytes
(uint8_t &).
POST: Recibe 1 byte y lo guarda en la referencia recibida.
Levante OSError en caso de error.
*/
void Protocolo::recibir_un_byte(uint8_t &valor){
    std::string err = "Error al recibir un byte";
    uint8_t valorRecibido = 0;
    this->_recibir((char*)&valorRecibido, 1, err.data());
    valor = valorRecibido;
}

/*
PRE: Recibe una referencia a un entero sin signo de 2 bytes
(uint16_t &).
POST: Recibe 2 bytes y lo guarda en la referencia recibida.
Levante OSError en caso de error.
*/
void Protocolo::recibir_dos_bytes(uint16_t &valor){
    std::string err = "Error al recibir dos bytes";
    uint16_t valorBE = 0;
    this->_recibir((char*)&valorBE, 2, err.data());
    valor = be16toh(valorBE);
}

/*
PRE: Recibe una referencia a un entero sin signo de 4 bytes
(uint32_t &).
POST: Recibe 4 bytes y lo guarda en la referencia recibida.
Levante OSError en caso de error.
*/
void Protocolo::recibir_cuatro_bytes(uint32_t &valor){
    std::string err = "Error al recibir cuatro bytes";
    uint32_t valorBE = 0;
    this->_recibir((char*)&valorBE, 4, err.data());
    valor = be32toh(valorBE);
}