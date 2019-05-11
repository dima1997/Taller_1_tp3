#ifndef CLIENTE_COMANDO_REVOCAR_H
#define CLIENTE_COMANDO_REVOCAR_H

#include "client_error.h"

#include "common_error.h"
#include "common_protocolo.h"
#include "common_claves.h"
#include "common_certificado.h"

#include <cstdint>
#include <iostream>

class ClienteComandoRevocar {
private:
    Protocolo &proto;
    ClaveRSA &privCliente;
    ClaveRSA &pubServidor;
    Certificado &certif;
public:
    /*
    PRE: Recibe un protocolo de comunicacion (Protocolo &), 
    las claves (ClaveRSA) privadas del cliente, y publicas del
    servidor, y el certificado a revocar (Certificado &).
    POST: Inicializa un comando revocador de certificados del 
    cliente.
    */
    ClienteComandoRevocar(Protocolo &proto, 
    ClaveRSA &privClnt, ClaveRSA &pubSvr, Certificado &certif);

    /*
    Ejecuta el comando revocar certificado del cliente.
    Levanta ClienteError en caso de un error esperado en el 
    comportamiento del cliente.
    Levanta OSError en caso de otro error.
    */
    void ejecutar();

    /*
    PRE: Recibe 3 enteros sin signo de 4 bytes (uint32_t): la huella enviada al servidor,
    el hash encriptado con la clave privada del cliente y el hash calculado del certificado
    a revocar.
    POST: Imprime por pantalla los valores recibidos de la forma:
    "Hash calculado: <hash calculado>"
    "Hash encriptado con la clave privada: <hash encriptado>"
    "Huella enviada: <huella enviada>" 
    */
    void imprimir_huellas_hashes(uint32_t huellaEnviada, 
    uint32_t hashEncriptado, uint32_t hashCalculado);

    /*Destruye el comando revocar del cliente.*/
    ~ClienteComandoRevocar();      
};
#endif // CLIENTE_COMANDO_REVOCAR_H 