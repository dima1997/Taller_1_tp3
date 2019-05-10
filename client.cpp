#include "client.h"

#include "client_error.h"

#include "common_error.h"
#include "common_protocolo.h"
#include "common_claves.h"
#include "common_certificado.h"
#include "common_generador_certificados.h"

#include <fstream>
#include <iostream>
#include <string>

/*
PRE: Recibe los nombres (std::string &) del host y
puerto al que se conectara. 
POST: Inicializa un cliente que pide crear y revocar
certificados.
*/
Cliente::Cliente(std::string &host, std::string &puerto) 
: skt(host.data(), puerto.data()){}

/*Destruye al cliente.*/
Cliente::~Cliente(){}

/*
PRE: Recibe un certificado (Certificado &).
POST: devuelve (uint32_t) e imprime el hash 
del certicado, de la forma:
"Hash calculado: <hash calculado>\n"
*/
uint32_t Cliente::calcular_imprimir_hash(Certificado &certif){
    uint32_t hashCalc = certif.hashear();
    std::cout << "Hash calculado: "; 
    std::cout << std::to_string(hashCalc) << "\n";
    return hashCalc;
}

/*
PRE: Recibe un huella del servidor (uint32_t), las claves
(ClaveRSA) del cliente y la publica del servidor.
POST: Desencripta la huella recibida e imprime en el proceso:
"Huella del servidor: <huella del servidor>"
"Hash del servidor: <huella desencriptada>"
Devuelve la huella desencritada.
*/
uint32_t Cliente::desencrip_imprimir_huella_svr(uint32_t huellaSvr,
ClaveRSA &clvClnt, ClaveRSA &clvSvr){
    std::cout << "Huella del servidor: ";
    std::cout << std::to_string(huellaSvr) << "\n";
    uint32_t hashSvr = clvClnt.encriptar_privado(huellaSvr);
    hashSvr = clvSvr.encriptar_publico(hashSvr);
    std::cout << "Hash del servidor: "; 
    std::cout << std::to_string(hashSvr) << "\n";
    return hashSvr;
}

/*
PRE: Recibe un hash del cliente (uint32_t), las claves
(ClaveRSA) del cliente y la publica del servidor.
POST: Encripta el hash recibido y lo imprime en el proceso:
"Hash encriptado con la clave privada: <hash encriptado con clave...>"
"Huella enviada : <hash completamente encriptado>"
*/
uint32_t Cliente::encrip_imprimir_hash_clnt(uint32_t hashClnt,
ClaveRSA &clvClnt, ClaveRSA &clvSvr){
    uint32_t huellaCliente = clvClnt.encriptar_privado(hashClnt);
    std::cout << "Hash encriptado con la clave privada: ";
    std::cout << std::to_string(huellaCliente) << "\n";
    huellaCliente = clvSvr.encriptar_publico(huellaCliente);
    std::cout << "Huella enviada: ";
    std::cout << std::to_string(huellaCliente) << "\n";
    return huellaCliente;
}

/*
PRE: Recibe el nombre (const char *) del archivo donde esta 
la informacion con la cual solicitar la creacion de un 
certificado, del archivo con las claves del cliente, y el
de la claves publicas del servidor.
POST: Ejecuta el procedimiento para crear un certificado.
Levanta OSError en caso de error.
*/
void Cliente::crear_certif(std::string &nombreInfoCertif, 
std::string &nombreClavesClnt, std::string &nombreClavesSvr){
    try {
        ClaveRSA clavesClnt(nombreClavesClnt);
        GeneradorCertificados genCertif(nombreInfoCertif, clavesClnt);
        ClaveRSA clavesSvr(nombreClavesSvr);
        Protocolo proto(this->skt);
        proto.enviar_bytes(0,1);
        genCertif.enviar_parametros(proto);
        uint8_t respuesta = proto.recibir_un_byte();
        if (respuesta == 1){
            std::string err = "Error: ya existe un certificado.";
            throw ClienteError(err.data());
        }   
        Certificado certif;
        certif.recibir(proto);
        uint32_t huellaSvr = proto.recibir_cuatro_bytes();
        uint32_t hashSvr;
        hashSvr = this->desencrip_imprimir_huella_svr(huellaSvr, 
            clavesClnt, clavesSvr);
        uint32_t hashCalculado = this->calcular_imprimir_hash(certif);
        if (hashSvr != hashCalculado){
            proto.enviar_bytes(1,1);
            std::string err = "Error: los hashes no coinciden.";
            throw ClienteError(err.data());
        }
        proto.enviar_bytes(0,1);
        certif.guardar();
    } catch (OSError &error){
        std::string err = "Error al crear certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe el nombre de 3 archivos (const char *): del archivo 
que contiene el certificado a revocar, del archivo que contiene las
claves del cliente, del archivo que contiene las claves publicas
del servidor.
POST: Ejecuta el procedimiento para revocar un certificado.
Levanta OSError en caso de error.
*/
void Cliente::revocar_certif(std::string &nombreCertif, 
std::string &nombreClavesClnt, std::string &nombreClavesSvr){
    try {
        Certificado certif(nombreCertif);
        ClaveRSA clavesClnt(nombreClavesClnt);
        ClaveRSA clavesSvr(nombreClavesSvr);
        Protocolo proto(this->skt);
        proto.enviar_bytes(1,1);
        certif.enviar(proto);
        uint32_t hashClnt = this->calcular_imprimir_hash(certif);
        uint32_t huellaCliente;
        huellaCliente = this->encrip_imprimir_hash_clnt(hashClnt, 
            clavesClnt, clavesSvr);
        proto.enviar_bytes(huellaCliente, 4);
        uint8_t respuesta = proto.recibir_un_byte();
        if (respuesta == 1){
            //No hay certificado registrado
            std::string err = "Error: usuario no registrado.";
            throw ClienteError(err.data());
        }
        if (respuesta == 2){
            //Los hashes no coinciden
            std::string err = "Error: los hashes no coinciden.";
            throw ClienteError(err.data());
        }
    } catch (OSError &error){
        std::string err = "Error al revocar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

int main(int argc, const char* argv[]){
    try {
        if (argc != 7){
            std::string err = "Error: argumentos invalidos.";
            throw ClienteError(err.data());
        }
        std::string host = argv[1];
        std::string puerto = argv[2];
        Cliente cliente(host, puerto);
        
        std::string comando = argv[3];
        std::string nombreCertif = argv[4];
        std::string nombreClvClnt = argv[5];
        std::string nombreClvSvr = argv[6];

        if (comando == "new"){
            cliente.crear_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else if (comando == "revoke"){
            cliente.revocar_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else {   
            std::string err = "Error: argumentos invalidos.";
            throw ClienteError(err.data());
        }
    } catch (ClienteError &error){
        std::cout << error.what() << "\n";
        return 0;
    } catch (OSError &error){
        return 1;
    }
    return 0;
}
