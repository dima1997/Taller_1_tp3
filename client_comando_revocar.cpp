#include "client_comando_revocar.h"

#include "client_error.h"

#include "common_error.h"
#include "common_protocolo.h"
#include "common_claves.h"
#include "common_certificado.h"

#include <cstdint>
#include <iostream>

/*
PRE: Recibe un protocolo de comunicacion (Protocolo &), 
las claves (ClaveRSA) privadas del cliente, y publicas del
servidor, y el certificado a revocar (Certificado &).
POST: Inicializa un comando revocador de certificados del 
cliente.
*/
ClienteComandoRevocar::ClienteComandoRevocar(Protocolo &proto, 
ClaveRSA &privClnt, ClaveRSA &pubSvr, Certificado &certif) 
: proto(proto), privCliente(privClnt), pubServidor(pubSvr),
certif(certif) {}

/*Ejecuta el comando revocar certificado del cliente.*/
ClienteComandoRevocar::ejecutar() {
    try {
        this->proto.enviar_bytes(1,1);
        this->certif.enviar(proto);
        uint32_t hashCalculado = certif.hashear();
        uint32_t hashEncriptado;
        hashEncriptado = this->privClnt.encriptar_privado(hashCalculado);
        uint32_t huellaEnviada;
        huellaEnviada = this->pubServidor.encriptar_publico(hashEncriptado);
        this->imprimir_huellas_hashes(huellaEnviada, hashEncriptado, 
            hashCalculado);
        proto.enviar_bytes(huellaCliente, 4);
        uint8_t respuesta = proto.recibir_un_byte();
        if (respuesta == 1){
            //No hay certificado registrado
            std::string err = "Error: usuario no registrado.";
            throw ClienteError(err);
        }
        if (respuesta == 2){
            //Los hashes no coinciden
            std::string err = "Error: los hashes no coinciden.";
            throw ClienteError(err);
        }
    } catch (OSError &error){
        std::string err = "Error en comando revocar del cliente.";
        throw OSError(__FILE__,__LINE__,err.data()); 
    }
}

ClienteComandoRevocar::imprimir_huellas_hashes(uint32_t huellaEnviada, 
uint32_t hashEncriptado, uint32_t hashCalculado) {
    std::cout << "Hash calculado: ";
    std::cout << std::dec << hashCalculado << "\n";
    std::cout << "Hash encriptado con la clave privada: ";
    std::cout << std::dec << hashEncriptado << "\n";
    std::cout << "Huella enviada ";
    std::cout << std::dec << huellaCliente << "\n";
}

/*Destruye el comando revocar del cliente.*/
ClienteComandoRevocar::~ClienteComandoRevocar() {}
