#ifndef CLIENTE_H
#define CLIENTE_H
#include "common_socket.h"
#include "common_certificado.h"
#include "common_claves.h"

#include <cstdint>
#include <string>

class Cliente {
public:
    Socket skt;

    /*
    PRE: Recibe los nombres (std::string &) del host y
    puerto al que se conectara. 
    POST: Inicializa un cliente que pide crear y revocar
    certificados.
    */
    Cliente(std::string &host, std::string &puerto);
    
    /*Destruye al cliente.*/
    ~Cliente();

    /*
    PRE: Recibe un certificado (Certificado &).
    POST: devuelve (uint32_t) e imprime el hash 
    del certicado, de la forma:
    "Hash calculado: <hash calculado>\n"
    */
    uint32_t calcular_imprimir_hash(Certificado &certif);

    /*
    PRE: Recibe un huella del servidor (uint32_t), las claves
    (ClaveRSA) del cliente y la publica del servidor.
    POST: Desencripta la huella recibida e imprime en el proceso:
    "Huella del servidor: <huella del servidor>"
    "Hash del servidor: <huella desencriptada>"
    Devuelve la huella desencritada.
    */
    uint32_t desencrip_imprimir_huella_svr(uint32_t huellaSvr,
    ClaveRSA &clvClnt, ClaveRSA &clvSvr);

    /*
    PRE: Recibe un hash del cliente (uint32_t), las claves
    (ClaveRSA) del cliente y la publica del servidor.
    POST: Encripta el hash recibido y lo imprime en el proceso:
    "Hash encriptado con la clave privada: <hash encriptado 
    con la clave privada>\n"
    "Huella enviada : <hash completamente encriptado>\n"
    */
    uint32_t encrip_imprimir_hash_clnt(uint32_t hashClnt,
    ClaveRSA &clvClnt, ClaveRSA &clvSvr);

    /*
    PRE: Recibe el nombre (std::string &) del archivo donde esta 
    la informacion con la cual solicitar la creacion de un 
    certificado, del archivo con las claves del cliente, y el
    de la claves publicas del servidor.
    POST: Ejecuta el procedimiento para crear un certificado.
    Levanta OSError en caso de error.
    */
    void crear_certif(std::string &nombreInfoCertif, 
    std::string &nombreClavesClnt, std::string &nombreClavesSvr);

    
    /*
    PRE: Recibe el nombre de 3 archivos (const char *): del archivo 
    que contiene el certificado a revocar, del archivo que contiene las
    claves del cliente, del archivo que contiene las claves publicas
    del servidor.
    POST: Ejecuta el procedimiento para revocar un certificado.
    Levanta OSError en caso de error.
    */
    void revocar_certif(std::string &nombreCertif, 
    std::string &nombreClavesClnt, std::string &nombreClavesSvr);
};

#endif // CLIENTE_H
