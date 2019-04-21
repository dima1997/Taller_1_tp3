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

void hash_prueba_hola(){
    std::string hola = "hola\0";
    Hash hash;
    uint16_t resultado = hash.hashear(hola, hola.size());
    uint16_t hash_real = 420;
    bool todoOK = (resultado == hash_real);
    std::cout << hola << "\n";
    std::cout << "hash real: " << hash_real << "\n";
    std::cout << "hash calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";
}

void hash_prueba_enunciado(){
    std::string certificado = "certificate:\n\tserial number: 11 (0x0000000b)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 17 (0x11)\0";
    Hash hash;
    uint16_t resultado = hash.hashear(certificado, certificado.size());
    uint16_t hash_real = 20603;
    bool todoOK = (resultado == hash_real);
    std::cout << certificado << "\n";
    std::cout << "hash real: " << hash_real << "\n";
    std::cout << "hash calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";
}


void hash_prueba_certificados_iguales(){
    std::string certificado1 = "certificate:​\n\t​serial number: 11 (0x0000000b)​\n\t​subject: Federico Manuel Gomez Peter​\n\t​issuer: Taller de programacion 1​\n\t​validity:​\n\t\t​not before: Mar 28 21:33:04 2019​\n\t\t​not after: May 27 21:33:04 2019​\n\t​subject public key info:​\n\t\t​modulus: 253 (0x00fd)​\n\t\t​exponent: 17 (0x11)\0";
    std::string certificado2 = "certificate:\n\tserial number: 11 (0x0000000b)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 17 (0x11)\0";
    std::cout << "certificado 1: " << certificado1 << "\n";
    std::cout << "certificado 2: " << certificado2 << "\n";
    std::cout << "¿Son iguales? : " << (certificado2 == certificado1) << "\n";
    for (size_t i = 0; i < certificado1.size(); ++i){
        if (certificado1[i] != certificado2[i]){
            std::cout << "El caracter diferente es el n° " << i << "\n";
            std::cout << "Caracter diferente certificado 1 es : " << certificado1[i] << "\n";
            std::cout << "Caracter diferente certificado 2 es : " << certificado2[i] << "\n";  
        }
    }
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

void encriptador_prueba_hola(){
    std::string hola = "hola\0";
    Hash hash;
    uint16_t huella = hash.hashear(hola, hola.size());
    Encriptador encript;
    uint32_t resultado_real = 354;
    uint32_t resultado = encript.encriptar(huella, 19, 253);
    bool todoOK = (resultado == resultado_real);
    std::cout << "encriptado real: " << resultado_real << "\n";
    std::cout << "encriptado calculado: " << resultado << "\n";
    std::cout << "todoOK : " << todoOK << "\n";

}

void encriptador_doble_prueba_certificado2(){
    std::string certificado2 = "certificate:\n\tserial number: 1 (0x00000001)\n\tsubject: Federico Manuel Gomez Peter\n\tissuer: Taller de programacion 1\n\tvalidity:\n\t\tnot before: Mar 28 21:33:04 2019\n\t\tnot after: May 27 21:33:04 2019\n\tsubject public key info:\n\t\tmodulus: 253 (0x00fd)\n\t\texponent: 19 (0x13)";
    Hash hash;
    uint16_t huella = hash.hashear(certificado2, certificado2.size());
    uint16_t resultado_hash_real = 20509;
    Encriptador encript; 
    uint32_t resultado1 = encript.encriptar(huella, 17, 253);
    uint32_t resultado1_real = 39039;
    uint32_t resultado2 = encript.encriptar(resultado1, 19, 253);
    uint32_t resultado2_real = 38011;
    std::cout << "hash_real : "<< resultado_hash_real << "\n";
    std::cout << "hash calculado : "<< huella << "\n";
    std::cout << "¿Son iguales hash? : "<< (resultado_hash_real == huella) << "\n";
    std::cout << "encriptado privado 1 real : "<< resultado1_real << "\n";   
    std::cout << "encriptado privado 1 calculado : "<< resultado1 << "\n";
    std::cout << "¿Son iguales encriptaciones 1? : "<< (resultado1_real == resultado1) << "\n";
    std::cout << "encriptado publico 2 real : "<< resultado2_real << "\n";   
    std::cout << "encriptado publico 2 calculado : "<< resultado2 << "\n";
    std::cout << "¿Son iguales encriptaciones 2? : "<< (resultado2_real == resultado2) << "\n";
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
    int Socket::socket_recibir_algo(void *buffer, size_t largo){
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
POST: Inicializa un protocolo de comunicacion, donde
los mensaje se enviar primero indicando su longitud en
enteros sin signo de 4 bytes big endian, y luego se 
envia el mensaje.
*/
Protocolo::Protocolo(Socket &skt) : skt(skt) {}

/*Destruye el protocolo*/
Protocolo::~Protocolo(){}

/*
PRE: Recibe el largo (uint32_t) de un mensaje a enviar, 
y el mensaje a enviar (const void *).
POST: Envia el mensaje (todo) segun protocolo especifica
*/
Protocolo::enviar_mensaje(const void *mensaje, uint32_t largo){
    bool seEnvio = true;
    uint32_t largoBE = htonl(largo);
    seEnvio = this->skt.enviar_todo(largoBE, 4); 
    // las longitudes se envian en 4 bytes
    if (! seEnvio){
        return false;
    }
    seEnvio = this->skt.enviar_todo(mensaje, largo);
    return seEnvio;
}

int main(){
    hash_prueba_hola();
    hash_prueba_enunciado();
    //hash_prueba_certificados_iguales();
    encriptador_prueba_hola();
    encriptador_doble_prueba_certificado2();
    return 0;
}