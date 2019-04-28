#include "hilos.h"
#include "protocolo.h"

/*
PRE: Recibe un socket (Socket &) configurado para comunicarse 
con un cliente entrante, un contador (ContadorBloq) con el 
proximo numero de serie para el siguiente certificado a crear, 
y un mapa (MapaBloq) cuyas claves sean los sujetos de certificados
creados, y su valores asociados sean sus claves rsa publicas.
POST: Se inicializa un hilo certificador. 
El socket recibido queda en estado nulo. 
*/
HCertfcdor::HCertfcdor(Socket &skt, ContadorBloq &contador, MapaBloq &mapa,
ClaveRSA &clave) : skt(std::move(skt)), contador(contador), sujetosClaves(mapa), 
claveServidor(clave) {}

/*Destruye un Hilo Certificador*/
HCertfcdor::~HCertfcdor(){}

/*Ejecuta el hilo certificador*/
void HCertfcdor::run(){
    Protocolo proto(this->skt);
    //Recibo comando
    uint8_t comando = 2;
    proto.recibir_un_byte(comando);
    if (comando == 0){
        this->crear(proto);
    } else if (comando == 1){
        this->revocar(proto);
    }
}

/*
PRE: Recibe un protocolo (Protocolo &) de comunicacion con
un cliente entrante.
POST: Ejecuta el procedimiento para la creacion de un 
certificado.
*/
void HCertfcdor::crear(Protocolo &proto){
    Certificado certif;
    try {
        certif.recibir_parametros(proto);
        std::string sujeto = certif.getSujeto();
        ClaveRSA claveCliente = certif.getClave();
        if (! this->sujetosClaves.agregarSiNoEsta(sujeto, claveCliente)){
            // Ya tiene certificado vigente
            proto.enviar_un_byte(1);
            return;
        }
        proto.enviar_un_byte(0); 
        certif.setNumeroSerie(this->contador++);
        std::string asunto = "Taller de programacion 1"; 
        certif.setAsunto(asunto);
        certif.enviar(proto);
        uint32_t hashCertif = certif.hashear();
        uint32_t huellaCertif = this->claveServidor.encriptar_privado(hashCertif);
        huellaCertif = claveCliente.encriptar_publico(huellaCertif);
        proto.enviar_cuatro_bytes(huellaCertif);
        uint8_t seRecibioCorrectamente;
        proto.recibir_un_byte(seRecibioCorrectamente);
    } catch (OSError &e){
        std::string err = "Error al crear certificado";
        throw OSError(err.data());
    }
    if (seRecibioCorrectamente == 1){
        //El cliente no recibio la huella correcta
        this->sujetosClaves.borrar(sujeto);
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
void HCertfcdor::revocar(Protocolo &proto){
    Certificado certif;
    certif.recibir(proto);
    uint32_t huellaCliente;
    proto.recibir_cuatro_bytes(huellaCliente);
    std::string sujeto = certif.getSujeto();
    ClaveRSA claveCliente;
    if (! this->sujetosClaves.obtenerSiEsta(sujeto, claveCliente)){
        //El certificado no esta registrado.
        proto.enviar_un_byte(1);
        return;
    }
    uint32_t hashServidor; 
    hashServidor = this->claveServidor.encriptar_privado(huellaCliente);
    hashServidor = claveCliente.encriptar_publico(hashServidor);
    //CONTINUARA...
}