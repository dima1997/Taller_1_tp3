#ifndef CLIENTE_COMANDO_CREAR_H
#define CLIENTE_COMANDO_CREAR_H

#include "common_protocolo.h"
#include "common_generador_certificados.h"
#include "common_claves.h"

#include <cstdint>

class ClienteComandoCrear {
private:
    Protocolo &proto;
    ClaveRSA &privCliente;
    ClaveRSA &pubServidor;
    GeneradorCertificados &genCertif;

public:
    /*
    PRE: Recibe un protocolo de comunicacion (Protocolo &), 
    las claves (ClaveRSA &) privadas del cliente
    las claves publicas del servidor, y un generador de certificados 
    (GeneradorCertificados &).
    POST: Inicializa un comando creador de certificados del cliente.
    */
    ClienteComandoCrear(Protocolo &proto, ClaveRSA &privCliente, 
    ClaveRSA &pubSvr, GeneradorCertificados &gen);

    /*
    Ejecuta el comando crear del cliente.
    Levanta ClienteError en caso de un error esperado 
    en el comportamiento del cliente.
    Levanta OSError en caso de otro error. 
    */
    void ejecutar();

    /*
    PRE: Recibe 3 enteros sin signo de 4 bytes (uint32_t):
    la huella del servidor, el hash del servidor, y el hash 
    calculado.
    POST: Imprime los valor recibidos de la forma:
    "Huella del servidor: " <huellaSvr>
    "Hash del servidor: " <hashSvr>
    "Hash calculado " <hashCalculado>
    */
    void imprimir_huellas_hashes(uint32_t huellaSvr, 
    uint32_t hashSvr, uint32_t hashCalculado);

    /*Destruye el comando crear del cliente*/
    ~ClienteComandoCrear();    
};

#endif // CLIENTE_COMANDO_CREAR_H
