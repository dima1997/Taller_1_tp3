#include <fstream>
#include <iostream>
#include <string>
#include "common_protocolo.h"
#include "common_claves.h"
#include "common_certificado.h"
#include "common_error.h"
#include "common_archivo.h"

#include "common_generador_certificados.h"
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
PRE: Recibe el nombre de un archivo que contiene informacion
para crear un certificado: sujeto, fecha de inicio, fecha de 
fin.
POST: Devuelve un certificado (Certificado) con cargado
con la informacion del archivo.
Levanta OSError en caso de error.
*/
Certificado Cliente::cargar_info(const char *nombreInfoCertif){
    Certificado certif;
    ArchivoCertif archCertif(nombreInfoCertif);
    archCertif.cargar_info(certif);
    archCertif.cerrar();
    return std::move(certif);        
}

/*
PRE: Recibe el nombre (const char *) de un archivo, 
y un archivable (Archivable &).
POST: Carga el archivable.
Levanta OSError en caso de error.
*/
void Cliente::cargar_archivable(const char* nombre, Archivable &archivable){
    Archivo arch(nombre, 'r');
    arch >> archivable;
    arch.cerrar();
}

/*
PRE: Recibe el nombre (const char *) de un archivo 
que contenga un certificado.
POST: Devuelve un certificado (Certificado)
*/
Certificado Cliente::cargar_certif(const char* nombreArchCertif){
    Certificado certif;
    this->cargar_archivable(nombreArchCertif, certif);
    return std::move(certif);
}
/*
PRE: Recibe el nombre (const char *) de un archivo que 
contiene un clave publica o completa (publica y privada).
POST: Devuelve una clave (ClaveRSA) cargado con la 
informacion del archivo.
Levanta OSError en caso de error.
*/
ClaveRSA Cliente::cargar_claves(const char* nombreClaves){
    ClaveRSA clave;
    this->cargar_archivable(nombreClaves, clave);
    return std::move(clave);
}

/*
PRE: Recibe un certificado (Certificado &).
POST: Guardar el contenido del certificado en un
archivo de nombre: "<sujeto del certificado>.cert"
Levanta OSError en caso de error.
*/
void Cliente::guardar_certif(Certificado &certif){
    std::string nombreArchCertif = certif.getSujeto() + ".cert";
    Archivo arch(nombreArchCertif.data(),'w');
    arch << certif;
    arch.cerrar();
}

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
ClaveRSA &clvClnt, ClaveRSA clvSvr){
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
void Cliente::crear_certif(const char *nombreInfoCertif, 
const char *nombreClavesClnt, const char *nombreClavesSvr){
    try {
        ClaveRSA clavesClnt = std::move(this->cargar_claves(nombreClavesClnt));
        GeneradorCertificados genCertif(nombreInfoCertif, clavesClnt);
        ClaveRSA clavesSvr = std::move(this->cargar_claves(nombreClavesSvr));
        Protocolo proto(this->skt);
        proto.enviar_bytes(0,1);
        genCertif.enviar_parametros(proto);
        uint8_t respuesta = proto.recibir_un_byte();
        if (respuesta == 1){
            std::cout << "Error: ya existe un certificado.\n";
            return;
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
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
        proto.enviar_bytes(0,1);
        this->guardar_certif(certif);
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
void Cliente::revocar_certif(const char *nombreCertif, 
const char *nombreClavesClnt, const char *nombreClavesSvr){
    try {
        Certificado certif = std::move(this->cargar_certif(nombreCertif));
        ClaveRSA clavesClnt = std::move(this->cargar_claves(nombreClavesClnt));
        ClaveRSA clavesSvr = std::move(this->cargar_claves(nombreClavesSvr));
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
            std::cout << "Error: usuario no registrado.\n";
            return;
        }
        if (respuesta == 2){
            //Los hashes no coinciden
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
    } catch (OSError &error){
        std::string err = "Error al revocar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
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
    } catch (OSError &error){
        return 1;
    }
    return 0;
}
