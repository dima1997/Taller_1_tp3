#define _POSIX_C_SOURCE 200112L
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>  
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <endian.h> 
#include "common.h"

/*
g++ -std=c++11 -ggdb -DDEBUG -fno-inline -Wall -Werror -pedantic -pedantic-errors common.cpp -o prueba
*/

//Funciones auxiliares

/*
PRE: Recibe una cadena de caracteres (string &), un separador 
(string &), y un vector de strings (vector<string> &).
POST: LLena el vector recibido (en sus ultimas posiciones), con
las partes de la cadena recibida que esten separados por el separador
recibido.
*/
void split(std::string &cad, std::string &sep, std::vector<std::string> &vSplt){
    std::string copiaCad(cad);
    std::string parteCadena;
    size_t pos;
    while ((pos = copiaCad.find(sep)) != std::string::npos) {
        parteCadena = copiaCad.substr(0, pos);
        vSplt.push_back(parteCadena);
        copiaCad.erase(0, pos + sep.size());
    }
    vSplt.push_back(copiaCad); 
    return;
}

/*
PRE: Recibe el nombre de un archivo (string &), y una string 
(string &) donde se guardara el contenido del archivo.
POST: Se carga todo el archivo en la string contenido, 
recibida.
Cualquier contenido anterior en segunda string recibida queda 
borrado.  
*/
bool cargarArchivo(std::string &nombre, std::string &contenido){
    std::ifstream archivo.open(nombre);
    if (! archivo.is_open()){
        return false;
    }
    contenido.erase(0,std::string::npos)
    char caracter;
    archivo.get(caracter);
    while (archivo.good()){
        contenido.push_back(caracter);
        archivo.get(caracter);
    }
    archivo.close();
    return true;
}

/*
PRE: Recibe el nombre (std::string &) de un archivo, la terminacion (std::string &)
que se le quiera agregar al nombre del archivo, y un texto (string &) que sera lo
que se escribira en el archivo.
*/
bool escribirArchivo(std::string &nombre, std::string &fin, std::string &texto){
    std::string nombreArchivo = nombre + fin;
    std::ofstream archivo.open(nombreArchivo);
    if (! archivo.is_open){
        return false;
    }
    archivo << texto;
    archivo.close();
}

//TDA Hash

/*Crea un Hash*/
Hash::Hash(){}

/*Destruye un Hash*/
Hash::~Hash(){}

/*
PRE: Recibe una cadena de caracteres (std::string &),
y su largo (size_t).
POST: Devuelve la suma (uint16_t) de todos los caracteres 
de la cadena recibida, expresados como enteros sin signo 
de 2 bytes.
*/
uint16_t Hash::hashear(std::string &cadena, size_t largo){
    uint16_t resultado = 0;
    for (size_t i = 0; i < largo; ++i) {
        resultado += (uint8_t)cadena[i]; //entero sin signo de un byte
    }
    return resultado;
}

//TDA Encriptador

/*Crea un encriptador*/
Encriptador::Encriptador(){}

/*Destruye un encriptador*/
Encriptador::~Encriptador(){}


/*
PRE: Recibe un huella: entero sin signo de 4 bytes 
(uint32_t); un exponente: entero sin signo de 1 bytes 
(uint8_t); y un modulo: entero sin signo de 2 bytes
(uint16_t).
POST: Devuelve la encriptacion de la huella recibida:
entero sin signo de 4 bytes (uint32_t), en base al 
exponente y modulo recibidos. 
*/
uint32_t Encriptador::encriptar(uint32_t huella, uint8_t exp, uint16_t mod){
    //El retorno es un entero de 4 bytes sin signo.
    uint32_t retorno = 0;
    //Recorro los bytes del mensaje a encriptar (que
    //es un número de 4 bytes sin signo).
    //Este mensaje puede ser el hash recibido o un 
    //valor previamente encriptado con otra clave.
    for (int i = 0; i < 4; ++i) {
        uint32_t resultado = (huella >> (i * 8)) & 0xff;;
        //Elevo el valor de la base (el valor del byte) 
        //al exponente público de la clave.
        uint32_t base = resultado;
        for (int j = 1; j < exp; ++j) { //+1
            resultado = (resultado * base) % mod;
        }
        retorno = retorno + (resultado << (i * 8));
    }
    return retorno;
}


//Imprimir en hexagesimal : printf("Numero hexagesimal %x", numeroEntero)

//string.data() => char *

    /*
    Inicializa el socket dejandolo en un estado
    invalido hasta tanto no llamar a otros metodos del 
    socket.
    */
    Socket::Socket(){
        this->skt = -1;
    }

    /*Destruye un socket.*/
    Socket::~Socket(){
        if (this->skt != -1){
            shutdown(this->skt, SHUT_RDWR);
            close(this->skt);
        }
    }

    /*
    Metodo para cliente.
    PRE: Recibe los nombres (const char*) del host y 
    puerto al que se desea conectar el socket.
    */
    bool Socket::conectar(const char *host, const char *puerto){
        int estado = 0;
        bool estamosConectados = false;
        struct addrinfo hints;
        struct addrinfo *direcciones, *sig; //siguiente
        this->skt = 0;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM; //TCP
        hints.ai_flags = 0;
        estado = getaddrinfo(host, puerto, &hints, &direcciones);
        if (estado != 0) {
            printf("Error in getaddrinfo: %s\n", gai_strerror(estado));
            return false;
        }
        sig = direcciones;
        while (sig != NULL) {
            this->skt = socket(sig->ai_family, sig->ai_socktype, sig->ai_protocol);
            if (this->skt == -1) {
                printf("Error: %s\n", std::strerror(errno));
            } else {
                estado = connect(this->skt, sig->ai_addr, sig->ai_addrlen);
                if (estado == -1) {
                    printf("Error: %s\n", std::strerror(errno));
                    close(this->skt);
                }
                estamosConectados = (estado != -1);
            }
            sig = sig->ai_next;
        }
        freeaddrinfo(direcciones);
        return estamosConectados;
    }

    /*
    Metodo para Servidor.
    PRE: Recibe un socket (skt_t *) ya creado, y el
    nombre (char *) de un puerto al cual enlazarlo.
    POST: Configura al socket para que funcion de 
    forma PASIVA, es decir, se lo enlaza al puerto de
    nombre recibido. 
    Devuelve true, si logro lo anterior, false en caso
    contrario.
    */
    bool Socket::enlazar(const char *puerto){
        int estado = 0;
        struct addrinfo hints;
        struct addrinfo *direccion;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM; //TCP
        hints.ai_flags = AI_PASSIVE; //AI_PASSIVE para servidor
        estado = getaddrinfo(NULL, puerto, &hints, &direccion);
        if (estado != 0) { 
            printf("Error in getaddrinfo: %s\n", gai_strerror(estado));
            return false;
        }
        struct addrinfo *dir = direccion;
        this->skt = socket(dir->ai_family, dir->ai_socktype, dir->ai_protocol);
        if (this->skt == -1) {
            printf("Error: %s\n", std::strerror(errno));
            freeaddrinfo(direccion);
            return false;
        }
        // Evita que le servidor falle al abrirlo y cerrarlo en poco tiempo
        int val = 1;
        estado = setsockopt(this->skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
        if (estado == -1) {
            printf("Error: %s\n", std::strerror(errno));
            close(this->skt);
            freeaddrinfo(direccion);
            return false;
        }
        estado = bind(this->skt, dir->ai_addr, dir->ai_addrlen);
        freeaddrinfo(direccion);
        if (estado == -1) {
            printf("Error: %s\n", std::strerror(errno));
            close(this->skt);
            return false;
        }
        return true;
    }

    /*
    Metodo para Servidor.
    PRE: Recibe la cantidad de sockets entrantes a escuchar.
    El socket ya esta enlazado a algun puerto.
    POST: Pone a escuchar la cantidad de recibida de sockets 
    entrantes.
    Devuelve true, si logro lo anterior, false en caso contrario.
    */
    bool Socket::escuchar(size_t cuantosEscuchar){
        int estado = listen(this->skt, cuantosEscuchar);
        if (estado == -1) {
            printf("Error: %s\n", std::strerror(errno));
            close(this->skt);
            return false;
        }
        return true;  
    }

    /*
    Metodo para Servidor.
    PRE: Recibe un socket (Socket &) para ser configurado como 
    ACTIVO. 
    POST: Devuelve true si logro aceptar una nueva comunicacion y
    configurar al socket ACTIVO para la misma, o false en caso 
    contrario. 
    */
    bool Socket::aceptar(Socket &sktActivo){
        sktActivo.skt = accept(this->skt, NULL, NULL);
        if (sktActivo.skt == -1) {
            printf("Error: %s\n", std::strerror(errno));
            return false;
        }
        return true; 
    }
    
    /*
    PRE: Recibe el modo en que se desea cerrar al socket:
    SHUT_RD, SHUT_WR, SHUT_RDWR
    POST: Cierra el socket. 
    */
    void Socket::cerrar_canal(int modoCierre){
        shutdown(this->skt, modoCierre);
    }
    
    /*
    PRE: Recibe un socket ya conectado (skt_t*), un 
    mensaje a enviar, y su longitud.
    POST: Devuelve true si logro enviar todo el mensaje, 
    false en caso contrario, dado algun error.
    */
    bool Socket::enviar_todo(const void *msj, size_t largo){
        int largoMsj = largo;
        int estado = 0;
        int bytesEnviados = 0;
        bool hayError = false; //todoOK = true;
        while (largoMsj > bytesEnviados && ! hayError){
            int i = bytesEnviados;
            int largoMaximo = largoMsj - bytesEnviados;
            estado = send(this->skt, &msj[i], largoMaximo, MSG_NOSIGNAL);
            if (estado < 0) {
                printf("Error: %s\n", std::strerror(errno));
                hayError = true; //todoOK = false;
            } else if (estado == 0) {
                printf("Error: el canal de escritura fue cerrado\n");
                hayError = true; //todoOK = false;
            } else {
                bytesEnviados = estado;
            }
        }
        return ! hayError; //todoOK
    }

    /*
    PRE: Recibe un buffer (string &) donde guardar los 
    bytes recibidos y el largo a llenar (int) del mismo.
    POST: Devuelve la cantidad de bytes recibidos.
    Si esta cantidad es negativa, entonces significa 
    que hubo algun error de socket. Si la cantidad es 
    igual a cero, significa que ya no queda nada mas 
    que recibir.
    */
    int Socket::recibir_algo(void *buffer, size_t largo){
        int largoBuff = largo;
        int estado = 0;
        bool hayError = false; //todoOK = true;
        bool seguirRecibiendo = true;
        int bytesRecibidos = 0;
        while (largoBuff > bytesRecibidos && ! hayError && seguirRecibiendo) { //&& todoOK
            size_t i = bytesRecibidos;
            size_t largoMaximo = largoBuff - bytesRecibidos; 
            estado = recv(this->skt, &buffer[i], largoMaximo-1, MSG_NOSIGNAL);
            if (estado < 0) {
                printf("Error: %s\n", std::strerror(errno));
                hayError = true; //todoOK = false;
            } else if (estado == 0) {
                seguirRecibiendo = false; //todoOK = false;
            } else {
                bytesRecibidos += estado; 
                buffer[bytesRecibidos] = 0;
            }
        }
        if (hayError) { //! todoOK
            return -1;
        }
        return bytesRecibidos;
    }


/*
PRE: REcibe un exponente publico: entero sin 
signo de 1 byte (uint8_t) entero sin signo de 
2 bytes, un exponente privado: entero sin 
signo de 1 bytes (uint8_t) y de un modulo: 
entero sin signo de 2 bytes (uint16_t). 
POST: Crea una clave rsa.
*/
Clave::ClaveRSA(uint8_t expPublico, uint8_t expPrivado, uint16_t modulo)
: expPublico(expPublico), expPrivado(expPrivado), modulo(modulo){}

/*Destruye la clave rsa*/
ClaveRSA::~ClaveRSA(){}

/*
PRE: Recibe el nombre de un archivo que contiene las claves
publicas y/o privadas de una entidad, de la forma:
<exponente-publico> <exponente-privado> <modulo>
o
<exponente-publico> <modulo>
POST: Actualiza los valores de la claves rsa, con los 
encontrados en el archivo.
*/
bool ClaveRSA::cargar_claves(std::string &nombreArchivo){
    std::string todasClaves = 0;
    bool todoOK = true;
    todoOK = cargarArchivo(nombreArchivo, todasClaves);
    if (! todoOK){
        return false;
    }
    std::std::vector<std::string> partesClaves;
    split(todasClaves, " ", partesClaves);
    this->expPublico = atoi(partesClaves[0].data());
    if (partesClaves.size() < 3){
        this->modulo = atoi(partesClaves[1].data());
    } else {
        this->expPrivado = atoi(partesClaves[1].data());
        this->modulo = atoi(partesClaves[2].data());
    }
    return true;
}

/*
PRE: Recibe una referencia al socket de comunicacion.
POST: Inicializa un protocolo de comunicacion.
*/
Protocolo::Protocolo(Socket &skt) : skt(skt) {}

/*Destruye el protocolo*/
Protocolo::~Protocolo(){}

/*
PRE: Recibe el largo/cantidad de bytes (uint32_t) 
de un mensaje a enviar. 
y el mensaje a enviar (const void *).
POST: Envia el mensaje, primero enviando la longitud
del mismo, seguido del mensaje.
Devuelve true si logro enviar el mensaje, false en caso 
contrario.
*/
bool Protocolo::enviar_mensaje(std::string &mensaje){ //const char *mensaje, uint32_t largo
    uint32_t largo = mensaje.size(); 
    bool seEnvio = true;
    uint32_t largoBE = htobe32(largo);
    char *buffer = (char*) &largoBE;
    seEnvio = this->skt.enviar_todo(buffer, 4); 
    // las longitudes se envian en 4 bytes
    if (! seEnvio){
        return false;
    }
    seEnvio = this->skt.enviar_todo(mensaje.data(), largo);
    return seEnvio;
}

/*
PRE: Recibe una referencia a una string (std::string &).
POST: Devuelve el largo del mensaje recibido, si logro
correctamente recibir y guardar el mensaje recibido en
la referencia recibida.
Si ocurrio algun error devuelve 0;  
*/
uint32_t Protocolo::recibir_mensaje(std::string &mensaje){
    uint32_t largoMensaje = 0;
    bool todoOK = true;
    todoOK = this->recibir_cuatro_bytes(largoMensaje);
    if (! todoOK){
        return 0;
    }
    char *buffer = new char[largoMensaje];
    int bytesRecibidos = this->skt.recibir_algo(buffer, largoMensaje);
    if (bytesRecibidos != largoMensaje){
        delete buffer;
        return 0;
    }
    mensaje = buffer;
    delete buffer;
    return largoMensaje;
}

/*
PRE: Recibe un valor (uint32_t) y una cantidad de bytes 
a enviar:
1: un byte
2: dos bytes
4: cuatro bytes
POST: lo bytes (recibidos) menos significativos del valor
recibido. 
Si bytes es 2 o 4, se envia en formato big endian.
Devuelve true si logro enviar los bytes del valor,
false en caso contrario.
*/
bool Protocolo::enviar_bytes(uint32_t valor, size_t bytes){
    bool todoOK = false;
    if (bytes == 1){
        uint8_t valorEnviar = (valor & 0x000000FF);
        char *buffer = (char *) &valorEnviar;
        todoOK = this->skt.enviar_todo(&valorEnviar, 1);
    }
    if (bytes == 2){
        uint16_t valorEnviar = (valor & 0x0000FFFF);
        uint16_t valorEnviarBE = htobe16(valorEnviar);
        char *buffer = (char*) &valorEnviarBE

        todoOK = this->skt.enviar_todo(buffer, 2); 
    }
    if (bytes == 4){
        uint32_t valorEnviar = valor;
        uint32_t valorEnviarBE = htobe32(valorEnviar);
        const char *buffer = (char *) &valorEnviarBE;
        todoOK = this->skt.enviar_todo(buffer, 4); 
    }
    return todoOK;
}

/*
PRE: Recibe una referencia a un entero sin signo de 1 bytes
(uint8_t &).
POST: Devuelve true si logro recibir correctamente 1 bytes
y guardalo en la referencia recibida; false en caso 
contrario.
*/
bool Protocolo::recibir_un_byte(uint8_t &valor){
    char buffer[1];
    int bytesRecibidos = this->skt.recibir_algo(buffer, 1);
    if (bytesRecibidos != 1){
        return false;
    }
    valor = *(uint8_t*)buffer;
    return true;
}

/*
PRE: Recibe una referencia a un entero sin signo de 2 bytes
(uint16_t &).
POST: Devuelve true si logro recibir correctamente 2 bytes
en big endian, y escribirlos en el endianess local en la
referencia recibida; false en caso contrario.
*/
bool Protocolo::recibir_dos_bytes(uint16_t &valor){
    char buffer[2];
    int bytesRecibidos = this->skt.recibir_algo(buffer, 2);
    if (bytesRecibidos != 2){
        return false;
    }
    uint16_t valorBE = *(uint16_t*)buffer;
    valor = betoh(valorBE);
    return true;
}

/*
PRE: Recibe una referencia a un entero sin signo de 4 bytes
(uint32_t &).
POST: Devuelve true si logro recibir correctametne 4 bytes
en big endian, y escribirlos en el endianess local en la 
referencia recibida; false en caso contrario.
*/
bool Protocolo::recibir_cuatro_bytes(uint32_t &valor){
    char buffer[2];
    int bytesRecibidos = this->skt.recibir_algo(buffer, 2);
    if (bytesRecibidos != 2){
        return false;
    }
    uint16_t valorBE = *(uint16_t*)buffer;
    valor = betoh(valorBE);
    return true;
}

std::string a_hexa32_string(uint32_t &valor){
    char buffer[11] //+0x + 8 letras +\0
    size_t largo = sizeof(buffer);
    snprintf(buffer, largo, "0x%08x", valor);
    return std::string(buffer);
}

std::string a_hexa16_string(uint16_t &valor){
    char buffer[7]; //+0x + 4 letras + \0
    size_t largo = sizeof(buffer);
    snprintf(buffer, largo, "0x%04x", valor);
    return std::string(buffer);
}

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
PRE: Recibe el nombre (std::string &) a un archivo que
tiene un certificado ya creado.
POST: Carga el certificado con la informacion del 
archivo.
Devuelve true si logro lo anterior, false en caso 
contrario.
*/
/*
bool Certificado::cargar(std::string &nombreArchivo){
    ifstream archCertif.open(nombreArchivo);
    if (! archCertif.is_open()){
        return false;
    }
    std::string linea = 0;
    std::getline(linea, );
    while (archCertif.is_good()){
        std::vector<std::string> lineaSplit(0);
        split(linea, "\t", lineaSplit);
    }
}
*/
/*
PRE: Recibe las claves publicas del cliente (ClaveRSA &, con los 
expPublico y modulo correctos), y un vector de strings con informacion
de el sujeto del certificado, la fecha de inicio, y de final 
(en ese orden).
POST: Inicializa un certificado con dicha informacion. 
*/
Certificado::Certificado(ClaveRSA &clvClnt, std::vector<std::string> &info){
    this->numeroSerie = 0;
    this->asunto = ""
    this->sujeto = info[0];
    this->inicio = info[1];
    this->fin = info[2];
    this->exp = clvClnt->expPublico;
    this->mod = clvClnt->mod;
}

/*Destruye un certificado*/
Certificado::~Certificado(){}

/*
PRE: Recibe un socket conectado con otro socket.
POST: Recibe y almacena la informacion de un 
certificado en el orden en que el metodo enviar
la envia.
Devuelve true, si logro lo anterior con exito,
false en caso contrario.
*/
bool recibir(Socket &skt){
    Protocolo proto(skt);
    uint32_t bytesRecibidos = 0;
    bool todoOK = true;
    todoOK = proto.recibir_cuatro_bytes(this->numeroSerie);
    if (! todoOK){
        return false;
    }

    bytesRecibidos = proto.recibir_mensaje(this->asunto);
    if (bytesRecibidos == 0){
        return false;
    }

    bytesRecibidos = proto.recibir_mensaje(this->sujeto);
    if (bytesRecibidos == 0){
        return false;
    }

    bytesRecibidos = proto.recibir_mensaje(this->inicio);
    if (bytesRecibidos == 0){
        return false;
    }

    bytesRecibidos = proto.recibir_mensaje(this->fin);
    if (bytesRecibidos == 0){
        return false;
    }
    todoOK = proto.recibir_dos_bytes(this->mod);
    if (! todoOK){
        return false;
    }
    return proto.recibir_un_byte(this->exp);
}

bool Certificado::enviar(Socket &skt){
    Protocolo proto(skt);
    bool todoOK = true;
    todoOK = proto.enviar_bytes(this->numeroSerie, 4);
    if (! todoOK){
        return false;
    }

    todoOK = proto.enviar_mensaje(this->asunto);
    if (! todoOK){
        return false;
    }

    todoOK = proto.enviar_mensaje(this->sujeto);
    if (! todoOK){
        return false;
    }

    todoOK = proto.enviar_mensaje(this->inicio);
    if (! todoOK){
        return false;
    }

    todoOK = proto.enviar_mensaje(this->fin);
    if (! todoOK){
        return false;
    }
    
    todoOK = proto.enviar_bytes(this->mod, 2);
    if (! todoOK){
        return false;
    }
    return proto.enviar_bytes(this->exp, 1);
}

/*
PRE: Recibe un socket conectado con un cliente que 
desee crear un certificado.
POST: Recibe los parametros para crear el certificado
en el orden que los envia el metodo enviar_parametros.
Devuelve true si logro recibir y guardar en si mismo
todos estos parametros recibidos; false en caso 
contrario.
*/
bool Certificado::recibir_parametros(Socket &skt){
    Protocolo proto(this->skt);
    bool todoOK = true;
    uint32_t bytesRecibidos = 0;
    //Recibimos el subject
    bytesRecibidos = proto.recibir_mensaje(this->subject);
    if (bytesRecibidos == 0){
        return false;
    }

    //Recibimos modulo
    todoOK = proto.recibir_dos_bytes(this->modulo);
    if (! todoOK){
        return false;
    }

    //Recibimos exponente
    todoOK = proto.recibir_un_byte(this->exp);
    if (! todoOK){
        return false;
    }

    //Recibimos fecha de inicio
    bytesRecibidos = proto.recibir_mensaje(this->inicio);
    if (bytesRecibidos == 0){
        return false;
    }

    //Recibimos fecha de fin
    bytesRecibidos = proto.recibir_mensaje(this->fin);
    
    return (bytesRecibidos == 0);
}

/*
PRE: Recibe un socket conectado con el servidor de 
una autoridad certificante.
POST: Envia los parametros que la autoridad necesita
del certificado actual para certificarlo, en el orden
que la autoridad certificante los espera.
Devuelve true si logro lo anterior; false en caso
contrario. 
*/
bool Certificado::enviar_parametros(Socket &skt){
    Protocolo proto(this->skt);
    bool todoOK = true;

    //Enviamos el subject
    todoOK = proto.enviar_mensaje(this->sujeto);
    if (!todoOK) {
        return false;
    }

    ///Enviamos el modulo del cliente
    todoOK = proto.enviar_bytes(this->modulo, 2);
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

/*
PRE: Recibe una referencia a una string (std::string &).
POST: Copia en la referencia la representacion del certificado 
actual.
*/
void a_string(std::string &cadena){
    std::string certificado = "certificate:\n";
    certificado += "\tserial number: " + this->numeroSerie; 
    certificado += a_hexa32_string(this->numeroSerie) + "\n"; 
    certificado += "\tsubject: " + this->sujeto + "\n";
    certificado += "\tissuer: " + this->asunto + "\n"; //Taller de programacion 1
    certificado += "\t​validity:​\n";
    certificado += "\t\tnot before: " + this->inicio + "\n";
    certificado += "\t\tnot after: " + this->fin + "\n";
    certificado += "\tsubject public key info:\n";
    certificado += "\t\tmodulus: " + std::string(this->mod);
    certificado += a_hexa16_string(this->mod) + "\n"; 
    certificado += "\t\texponent: " + std::string(this->exp); 
    certificado += a_hexa8_string(this->exp) + "\n"; 
    cadena = certificado;
}

/*Devuelve el hash de certificado actual*/
uint32_t hashear(){
    std::string certificado = 0;
    this->a_string(certificado);
    Hash hash;
    return hash.hashear(certificado, certificado.size());
}

/*
Crea un archivo <sujeto>.cert y guarda en el certificado actual.
*/
bool guardar(){
    std::string certificado = 0;
    this->a_string(certificado);
    return escribirArchivo(this->subject, ".cert", certificado);
}