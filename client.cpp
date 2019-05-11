#include "client.h"

#include "client_comando_crear.h"
#include "client_comando_revocar.h"

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
PRE: Recibe el nombre (const char *) del archivo donde esta 
la informacion con la cual solicitar la creacion de un 
certificado, del archivo con las claves del cliente, y el
de la claves publicas del servidor.
POST: Ejecuta el procedimiento para crear un certificado.
Levanta ClienteError en caso del algun error esperador en el 
comportamiento del cliente.
Levanta OSError en caso de error.
*/
void Cliente::crear_certif(std::string &nombreInfoCertif, 
std::string &nombreClavesClnt, std::string &nombreClavesSvr){
    try {
        ClaveRSA clavesClnt(nombreClavesClnt);
        ClaveRSA clavesClntCopia = std::move(clavesClnt.copiar());
        //Paso una copia porque no quiero perder los atributos de clavesClnt
        GeneradorCertificados genCertif(nombreInfoCertif, clavesClntCopia);
        ClaveRSA clavesSvr(nombreClavesSvr);
        Protocolo proto(this->skt);
        ClienteComandoCrear cmdCrear(proto,clavesClnt, clavesSvr, genCertif);
        cmdCrear.ejecutar();
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
        ClienteComandoRevocar cmdRevocar(proto, clavesClnt, clavesSvr, certif);
        cmdRevocar.ejecutar();
    } catch (OSError &error){
        std::string err = "Error al revocar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

int main(int argc, const char* argv[]){
    try {
        if (argc != 7){
            std::string err = "Error: argumentos invalidos.";
            throw ClienteError(err);
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
            throw ClienteError(err);
        }
    } catch (ClienteError &error){
        std::cout << error.what() << "\n";
        return 0;
    } catch (OSError &error){
        return 1;
    }
    return 0;
}
