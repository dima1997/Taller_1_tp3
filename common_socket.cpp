#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>  
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <utility>
#include "common_error.h"
#include "common_socket.h"

/*Inicializa un Socket por default en estado invalido*/
Socket::Socket(){
    this->skt = -1;
}

/*
PRE: Recibe un codigo de socket (int).
POST: Inicializa un socket con dicho 
codigo.
*/
Socket::Socket(int skt){
    this->skt = skt;
}

/*Destruye un socket.*/
Socket::~Socket(){
    if (this->skt != -1){
        shutdown(this->skt, SHUT_RDWR);
        close(this->skt);
    }
}

/*
PRE: Recibe una doble referencia a otro socket (Socket &&).
POST: Crea un nuevo socket por movimiento semantico.
El socket recibido que en estado nulo.
*/
Socket::Socket(Socket&& otroSkt){
    this->skt = otroSkt.skt;
    otroSkt.skt = -1;
}

/*
PRE: Recibe una doble referencia a otro socket (Socket &&).
POST: Mueve semanticamente los atributos del socket recido
al actual, y devuelve una referencia a este ultimo.
El socket recibido que en estado nulo.
*/
Socket& Socket::operator=(Socket&& otroSkt){
    if (this == &otroSkt){
        return *this;
    }
    this->skt = otroSkt.skt;
    otroSkt.skt = -1;
    return *this;
}

/*
Constructor para cliente.
PRE: Recibe los nombres (const char*) del host y 
puerto al que se desea conectar el socket.
POST: Inicializa un socket conectado al host y puerto 
recibidos.
Levanta OSError en caso de error de conexion.
*/
Socket::Socket(const char *host, const char *puerto){
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
        throw OSError(__FILE__,__LINE__,"Error en getaddrinfo: ", 
        gai_strerror(estado));
    }
    sig = direcciones;
    while (sig != NULL) {
        this->skt = socket(sig->ai_family, sig->ai_socktype, sig->ai_protocol);
        if (this->skt == -1) {
            throw OSError(__FILE__,__LINE__,"Error al conectar socket.");
        } else {
            estado = connect(this->skt, sig->ai_addr, sig->ai_addrlen);
            if (estado == -1) {
                close(this->skt);
                throw OSError(__FILE__,__LINE__,"Error al conectar socket.");
            }
            estamosConectados = (estado != -1);
        }
        sig = sig->ai_next;
    }
    freeaddrinfo(direcciones);
    if (! estamosConectados){
        throw OSError(__FILE__,__LINE__,"Error al conectar socket.");
    }
}

/*
Constructor para Servidor.
PRE: Recibe el nombre (char *) de un puerto al 
cual enlazar el socket.
POST: Inicializa un socket para que funcione de 
forma PASIVA, es decir, se lo enlaza al puerto de
nombre recibido. 
Levanta OSError en caso de error de enlazamiento.
*/
Socket::Socket(const char *puerto){
    int estado = 0;
    this->skt = 0;
    struct addrinfo hints;
    struct addrinfo *direccion;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE; //AI_PASSIVE para servidor
    estado = getaddrinfo(NULL, puerto, &hints, &direccion);
    if (estado != 0) {
        throw OSError(__FILE__,__LINE__,"Error en getaddrinfo: ", 
        gai_strerror(estado));
    }
    struct addrinfo *dir = direccion;
    this->skt = socket(dir->ai_family, dir->ai_socktype, dir->ai_protocol);
    if (this->skt == -1) {
        freeaddrinfo(direccion);
        throw OSError(__FILE__,__LINE__,"Error al enlazar socket.");
    }
    // Evita que le servidor falle al abrirlo y cerrarlo en poco tiempo
    int val = 1;
    estado = setsockopt(this->skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (estado == -1) {
        close(this->skt);
        freeaddrinfo(direccion);
        throw OSError(__FILE__,__LINE__,"Error al enlazar socket.");
    }
    estado = bind(this->skt, dir->ai_addr, dir->ai_addrlen);
    freeaddrinfo(direccion);
    if (estado == -1) {
        close(this->skt);
        throw OSError(__FILE__,__LINE__,"Error al enlazar socket.");
    }
}

/*
Metodo para Servidor.
PRE: Recibe la cantidad de sockets entrantes a escuchar.
El socket ya esta enlazado a algun puerto.
POST: Pone a escuchar la cantidad de recibida de sockets 
entrantes.
Levanta OSError en caso de error.
*/
void Socket::escuchar(size_t cuantosEscuchar){
    int estado = listen(this->skt, cuantosEscuchar);
    if (estado == -1) {
        throw OSError(__FILE__,__LINE__,strerror(errno));
    }
}

/*
Metodo para Servidor.
Devuelve un nuevo socket (Socket) obtenido de aceptar
una conexion. 
Levanta OSError en caso de error de aceptacion.
*/
Socket Socket::aceptar(){
    int skt = accept(this->skt, NULL, NULL);
    if (skt == -1) {
        throw OSError(__FILE__,__LINE__,"Error al aceptar socket.");
    }
    return std::move(Socket(skt)); 
}
    
/*
PRE: Recibe el modo (ModoCierre) en que se desea cerrar al socket:
ModoCierre.CERRAR_LECTURA,
ModoCierre.CERRAR_ESCRITURA,
ModoCierre.CERRAR_TODO,
POST: Cierra el socket. 
*/
void Socket::cerrar_canal(ModoCierre modoCierre){
    if (modoCierre == CERRAR_LECTURA){
        shutdown(this->skt, SHUT_RD);
    }
    if (modoCierre == CERRAR_ESCRITURA){
        shutdown(this->skt, SHUT_WR);
    }
    if (modoCierre == CERRAR_TODO){
        shutdown(this->skt, SHUT_RDWR);
    }
}

/*Cierra el socket*/
void Socket::cerrar(){
    close(this->skt);
}
    
/*
PRE: Recibe un mensaje (const char*) a enviar, y su 
longitud (size_t).
POST: Envia el mensaje, y devuelve la cantidad de 
bytes enviados. Si esta cantidad es menor al largo 
recibido es porque se cerro el canal de escritura.
Levanta OSError en caso de error.
*/
int Socket::enviar_todo(const char *msj, size_t largo){
    int largoMsj = largo;
    int estado = 0;
    int bytesEnviados = 0;
    bool seguirEnviando = true;
    while (largoMsj > bytesEnviados && seguirEnviando){
        int i = bytesEnviados;
        int largoMaximo = largoMsj - bytesEnviados;
        estado = send(this->skt, &msj[i], largoMaximo, MSG_NOSIGNAL);
        if (estado < 0) {
            throw OSError(__FILE__,__LINE__,"Error al enviar mensaje.");
        } else if (estado == 0) {
            //Dejamos que esto se maneje desde afuera.
            seguirEnviando = false; 
        } else {
            bytesEnviados += estado; 
        }
    }
    return bytesEnviados;
}

/*
PRE: Recibe un buffer (char *) donde guardar los 
bytes recibidos y el largo a llenar maximo (size_t) 
del mismo.
POST: Devuelve la cantidad de bytes recibidos.
Si esta cantidad es menor al largo del buffer es 
poque se cerro el canal de lectura.
Levanta OSError en caso de error.
*/
int Socket::recibir_algo(char *buffer, size_t largo){
    int largoBuff = largo;
    int estado = 0;
    bool seguirRecibiendo = true;
    int bytesRecibidos = 0;
    while (largoBuff > bytesRecibidos && seguirRecibiendo) { 
        size_t i = bytesRecibidos;
        size_t largoMaximo = largoBuff - bytesRecibidos; 
        estado = recv(this->skt, &buffer[i], largoMaximo, MSG_NOSIGNAL);
        if (estado < 0) {
            throw OSError(__FILE__,__LINE__,"Error al recibir mensaje.");
        } else if (estado == 0) {
            //Dejamos que esto se maneje desde afuera.
            seguirRecibiendo = false;
        } else {
            bytesRecibidos += estado;
        }
    }
    return bytesRecibidos;
}
