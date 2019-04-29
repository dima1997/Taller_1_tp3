#include <fstream>
#include <iostream>
#include <string>
#include "common_protocolo.h"
#include "common_claves.h"
#include "common_certificado.h"
#include "common_error.h"
#include "client.h"

/*
PRE: Recibe los nombres (std::string &) del host y
puerto al que se conectara. 
POST: Inicializa un cliente que pide crear y revocar
certificados.
*/
Cliente::Cliente(const char *host, const char *puerto) 
: skt(host, puerto){}

/*Destruye al cliente.*/
Cliente::~Cliente(){}

/*
PRE: Recibe el nombre (const char *) del archivo donde esta 
la informacion con la cual solicitar la creacion de un 
certificado, del archivo con las claves del cliente, y el
de la claves publicas del servidor.
POST: Ejecuta el procedimiento para crear un certificado.
Levanta OSError en caso de error.
*/
void Cliente::crear_certif(const char *nombreInfoCertif, 
const char *nombreClavesClnt, const char *nombreClavesSvr){
    Certificado certif;
    
    std::ifstream archInfo;
    archInfo.open(nombreInfoCertif);
    certif.cargar_info(archInfo);
    archInfo.close();

    ClaveRSA clavesClnt;
    std::ifstream archClavesCliente;
    archClavesCliente.open(nombreClavesClnt);
    archClavesCliente >> clavesClnt;
    archClavesCliente.close();

    certif.setClave(clavesClnt);

    ClaveRSA clavesSvr;
    std::ifstream archClavesServidor;
    archClavesServidor.open(nombreClavesSvr);
    archClavesServidor >> clavesSvr;
    archClavesServidor.close();
    try {
        Protocolo proto(this->skt);
        proto.enviar_bytes(0,1);
        certif.enviar_parametros(proto);
        uint8_t respuesta = 3;
        proto.recibir_un_byte(respuesta);
        if (respuesta == 1){
            std::cout << "Error: ya existe un certificado.\n";
            return;
        }   
        certif.recibir(proto);
        uint32_t huellaServidor;
        proto.recibir_cuatro_bytes(huellaServidor);
        std::cout << "Huella del servidor: ";
        std::cout << std::to_string(huellaServidor) << "\n";
        uint32_t hashSvr = clavesClnt.encriptar_privado(huellaServidor);
        hashSvr = clavesSvr.encriptar_publico(hashSvr);
        std::cout << "Hash del servidor: "; 
        std::cout << std::to_string(hashSvr) << "\n";
        uint32_t hashCalculado = certif.hashear();
        std::cout << "Hash calculado: "; 
        std::cout << std::to_string(hashCalculado) << "\n";
        if (hashSvr != hashCalculado){
            proto.enviar_bytes(1,1);
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
        proto.enviar_bytes(0,1);
    } catch (OSError &e){
        std::string err = "Error al crear certificado.";
        throw OSError(err.data());
    }
    std::ofstream archCertif;
    std::string nombreArchCertif = certif.getSujeto() + ".cert";
    archCertif.open(nombreArchCertif);
    archCertif << certif;
    archCertif.close();
}

/*
PRE: Recibe el nombre de 3 archivos (const char *): del archivo 
que contiene el certificado a revocar, del archivo que contiene las
claves del cliente, del archivo que contiene las claves publicas
del servidor.
POST: Ejecuta el procedimiento para revocar un certificado.
Levanta OSError en caso de error.
*/
void Cliente::revocar_certif(const char *nombreCertif, 
const char *nombreClavesClnt, const char *nombreClavesSvr){
    Certificado certif;
    
    std::ifstream archCertif;
    archCertif.open(nombreCertif);
    archCertif >> certif;
    archCertif.close();

    ClaveRSA clavesClnt;

    std::ifstream archClavesClnt;
    archClavesClnt.open(nombreClavesClnt);
    archClavesClnt >> clavesClnt;
    archClavesClnt.close();

    ClaveRSA clavesSvr;

    std::ifstream archClavesSvr;
    archClavesSvr.open(nombreClavesSvr);
    archClavesSvr >> clavesSvr;
    archClavesSvr.close();
    try {   
        Protocolo proto(this->skt);
        proto.enviar_bytes(1,1);
        certif.enviar(proto);
        uint32_t huellaCliente = certif.hashear();
        std::cout << "Hash calculado: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        huellaCliente = clavesClnt.encriptar_privado(huellaCliente);
        std::cout << "Hash encriptado con clave privada: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        huellaCliente = clavesSvr.encriptar_publico(huellaCliente);
        std::cout << "Huella enviada: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        proto.enviar_bytes(huellaCliente, 4);
        uint8_t respuesta;
        proto.recibir_un_byte(respuesta);
        if (respuesta == 1){
            //No hay certificado registrado
            std::cout << "Error: usuario no registrado.\n";
            return;
        }
        if (respuesta == 2){
            //Los hashes no coinciden
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
    } catch (OSError &e){
        std::string err = "Error al revocar certificado.";
        throw OSError(err.data());
    }
}

int main(int argc, const char* argv[]){
    if (argc != 7){
        std::cout << "Error: argumentos invalidos.\n";
    }
    try {
        const char *host = argv[1];
        const char *puerto = argv[2];
        Cliente cliente(host, puerto);
        
        std::string comando = argv[3];
        const char *nombreCertif = argv[4];
        const char *nombreClvClnt = argv[5];
        const char *nombreClvSvr = argv[6];

        if (comando == "new"){
            cliente.crear_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else if (comando == "revoke"){
            cliente.revocar_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else {
            std::cout << "Error: argumentos invalidos.\n";   
        }
    } catch (OSError &e){
        return 1;
    }
    return 0;
}
