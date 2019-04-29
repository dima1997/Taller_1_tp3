#include <string>
#include <vector>
#include "common_protocolo.h"
#include "common_certificado.h"
#include "common_error.h"
#include "server_hilos.h"

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
        certif.recibir_parametros(proto);
        std::string sujeto = certif.getSujeto();
        ClaveRSA claveCliente = certif.getClave();
        if (! this->sujetosClaves.agregarSiNoEsta(sujeto, claveCliente)){
            // Ya tiene certificado vigente
            proto.enviar_bytes(1,1);
            return;
        }
        proto.enviar_bytes(0,1); 
        certif.setNumeroSerie(this->contador.obtener_y_sumar_uno());
        std::string asunto = "Taller de programacion 1"; 
        certif.setAsunto(asunto);
        certif.enviar(proto);
        uint32_t hashCertif = certif.hashear();
        uint32_t huellaCertif;
        huellaCertif = this->claveServidor.encriptar_privado(hashCertif);
        huellaCertif = claveCliente.encriptar_publico(huellaCertif);
        proto.enviar_bytes(huellaCertif,4);
        uint8_t seRecibioCorrectamente = proto.recibir_un_byte();
        if (seRecibioCorrectamente == 1){
            //El cliente no recibio la huella correcta
            this->sujetosClaves.borrar(sujeto);
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
    Certificado certif;
    try {
        certif.recibir(proto);
        uint32_t huellaCliente = proto.recibir_cuatro_bytes();
        std::string sujeto = certif.getSujeto();
        ClaveRSA claveCliente;
        if (! this->sujetosClaves.obtenerSiEsta(sujeto, claveCliente)){
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
        this->sujetosClaves.borrar(sujeto);
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

/*
PRE: Recibe un socket (Socket &) configurado de forma pasiva
para comunicarse escuchar clientes de cierto puerto, el 
contador (ContadorBloq &) que indica que el proximo numero 
de serie a ejecutar, un mapa (MapaBloq &) con los sujetos y 
claves publicas asociadas de certificados creados, y las 
claves (ClaveRSA &) del servidor.
POST: Se inicializa un hilo aceptador.
El Socket recibido queda en estado nulo.  
*/
HAceptador::HAceptador(Socket &skt, ContadorBloq &contador, 
MapaBloq &mapa, ClaveRSA &claveSvr) : skt(std::move(skt)), 
estaMuerto(false), contador(contador), sujetosClaves(mapa), 
claveSvr(claveSvr) {}

/*Destruye un hilo aceptador*/
HAceptador::~HAceptador(){}

/*Ejecuta un hilo aceptardor*/
void HAceptador::run(){
    std::vector<Thread*> hilos;
    while (! this->estaMuerto){
        Socket sktActivo;
        try {
            sktActivo = this->skt.aceptar();
        } catch (OSError &error){
            //O bien se corto desde afuera y debo salir
            //O bien ocurrio un error y debo terminar 
            //la ejecucion del hilo
            break;
        }
        HCertificador *hiloCertfcdor;
        ContadorBloq &contador = this->contador;
        MapaBloq &mapa = this->sujetosClaves; 
        ClaveRSA &claveSvr = this->claveSvr;
        hiloCertfcdor = new HCertificador(sktActivo, contador, mapa, claveSvr);
        hiloCertfcdor->start();
        hilos.push_back(hiloCertfcdor);
        std::vector<Thread*> temp; //aux para iterar
        for (size_t i = 0; i < hilos.size(); ++i){
            if (hilos[i]->is_dead()){
                hilos[i]->join();
                delete hilos[i];
            } else {
                temp.push_back(hilos[i]);
            }
        }
        hilos = std::move(temp);
        //Ahora solo quedan los hilos que siguen vivos
    }
    for (size_t i = 0; i < hilos.size(); ++i){
        hilos[i]->join();
        delete hilos[i];
    }
    this->estaMuerto = true;
    return;
}

/*
Devuelve true si el hilo aceptador termino 
de ejecutar, false en caso contrario.
*/
bool HAceptador::is_dead(){
    return this->estaMuerto;
}

/*Detiene la ejecucion del hilo*/
void HAceptador::stop(){
    this->estaMuerto = true;
    this->skt.cerrar_canal(CERRAR_TODO);
    this->skt.cerrar();
}
