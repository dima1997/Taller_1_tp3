#include "server_hilo_certificador.h"

#include "common_socket.h"
#include "common_protocolo.h"
#include "common_error.h"
#include "common_claves.h"
#include "common_generador_certificados.h"
#include "common_certificado.h"
#include "common_contador_bloq.h"
#include "common_mapa_bloq.h"

#include <string>

/*
PRE: Recibe un socket (Socket &) configurado para comunicarse 
con un cliente entrante, un contador (ContadorBloq) con el 
proximo numero de serie para el siguiente certificado a crear, 
y un mapa (MapaBloq) cuyas claves sean los sujetos de certificados
creados, y su valores asociados sean sus claves rsa publicas.
POST: Se inicializa un hilo certificador. 
El socket recibido queda en estado nulo. 
*/
HCertificador::HCertificador(Socket &skt, ContadorBloq &contador, 
MapaBloq &mapa, ClaveRSA &clave) : skt(std::move(skt)), estaMuerto(false), 
claveServidor(clave), contador(contador), sujetosClaves(mapa) {}

/*Destruye un Hilo Certificador*/
HCertificador::~HCertificador(){}

/*Ejecuta el hilo certificador*/
void HCertificador::run(){
    Protocolo proto(this->skt);
    uint8_t comando = proto.recibir_un_byte(); 
    if (comando == 0){
        this->crear(proto);
    } 
    if (comando == 1){
        this->revocar(proto);
    }
    this->estaMuerto = true;
    return;
}

/*
PRE: Recibe un protocolo (Protocolo &) de comunicacion con
un cliente entrante.
POST: Ejecuta el procedimiento para la creacion de un 
certificado.
*/
void HCertificador::crear(Protocolo &proto){
    Certificado certif;
    try {
        GeneradorCertificados genCertif(proto);
        if (! genCertif.agregar_sujeto_clave_mapa(this->sujetosClaves)){
            // Ya tiene certificado vigente
            proto.enviar_bytes(1,1);
            return;
        }

        proto.enviar_bytes(0,1); 
        std::string asunto = "Taller de programacion 1";
        Certificado certif = genCertif.generar(this->contador, asunto);
        certif.enviar(proto);
        uint32_t hashCertif = certif.hashear();
        uint32_t huellaCertif;
        huellaCertif = this->claveServidor.encriptar_privado(hashCertif);
        huellaCertif = certif.encriptar(huellaCertif);
        proto.enviar_bytes(huellaCertif,4);
        uint8_t seRecibioCorrectamente = proto.recibir_un_byte();
        if (seRecibioCorrectamente == 1){
            //El cliente no recibio la huella correcta
            certif.borrar_sujeto_mapa(this->sujetosClaves);
        }
    } catch (OSError &error){
        std::string err = "Error al crear certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    //Si recibo 0, no hago nada
    //Supongo que no voy a recibir ningun otro valor
    return;
}

/*
PRE: Recibe un protocolo (Protocolo &) de comunicacion con
un cliente entrante.
POST: Ejecuta el procedimiento para la revocacion de un 
certificado.
*/
void HCertificador::revocar(Protocolo &proto){
    try {
        Certificado certif(proto);
        uint32_t huellaCliente = proto.recibir_cuatro_bytes();
        ClaveRSA claveCliente;
        if (! certif.obtener_sujeto_mapa(this->sujetosClaves, claveCliente)){
            //El certificado no esta registrado.
            proto.enviar_bytes(1,1);
            return;
        }
        uint32_t hashCertif = certif.hashear();
        uint32_t hashCliente; 
        hashCliente = this->claveServidor.encriptar_privado(huellaCliente);
        hashCliente = claveCliente.encriptar_publico(hashCliente);
        if (hashCertif != hashCliente){
            //Los hashes no coinciden
            proto.enviar_bytes(2,1);
            return;
        }
        certif.borrar_sujeto_mapa(this->sujetosClaves);
        proto.enviar_bytes(0,1);
    } catch (OSError &error){
        std::string err = "Error al revocar certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    return;
}

/*
Devuelve true si el hilo ejecutor termino de 
ejecutar, false en caso contrario
*/
bool HCertificador::is_dead(){
    return this->estaMuerto;
}

/*Detiene la ejecucion del hilo*/
void HCertificador::stop(){
    //No hace nada
    //El hilo termina su ejecucion por si solo 
}
