#ifndef CLIENTE_H
#define CLIENTE_H
#include "common_socket.h"
class Cliente {
    public:
    Socket skt;

    /*
    PRE: Recibe los nombres (std::string &) del host y
    puerto al que se conectara. 
    POST: Inicializa un cliente que pide crear y revocar
    certificados.
    */
    Cliente(const char *host, const char *puerto);
    
    /*Destruye al cliente.*/
    ~Cliente();

    /*
    PRE: Recibe el nombre (const char *) del archivo donde esta 
    la informacion con la cual solicitar la creacion de un 
    certificado, del archivo con las claves del cliente, y el
    de la claves publicas del servidor.
    POST: Ejecuta el procedimiento para crear un certificado.
    Levanta OSError en caso de error.
    */
    void crear_certif(const char *nombreInfoCertif, 
    const char *nombreClavesClnt, const char *nombreClavesSvr);

    
    /*
    PRE: Recibe el nombre de 3 archivos (const char *): del archivo 
    que contiene el certificado a revocar, del archivo que contiene las
    claves del cliente, del archivo que contiene las claves publicas
    del servidor.
    POST: Ejecuta el procedimiento para revocar un certificado.
    Levanta OSError en caso de error.
    */
    void revocar_certif(const char *nombreCertif, 
    const char *nombreClavesClnt, const char *nombreClavesSvr);
};

#endif // CLIENTE_H
