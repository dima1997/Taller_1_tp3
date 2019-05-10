#ifndef HILO_CERTIFICADOR_H
#define HILO_CERTIFICADOR_H

#include "common_thread.h"
#include "common_protocolo.h"
#include "common_socket.h"
#include "common_claves.h"
#include "common_contador_bloq.h"
#include "common_mapa_bloq.h"

class HCertificador : public Thread {
    public:
    Socket skt;
    
    private: 
    bool estaMuerto; 
    ClaveRSA &claveServidor;
    ContadorBloq &contador;
    MapaBloq &sujetosClaves;

    public:
    /*
    PRE: Recibe un socket (Socket &) configurado para comunicarse 
    con un cliente entrante, un contador (ContadorBloq) con el 
    proximo numero de serie para el siguiente certificado a crear, 
    y un mapa (MapaBloq) cuyas claves sean los sujetos de certificados
    creados, y su valores asociados sean sus claves rsa publicas.
    POST: Se inicializa un hilo certificador. 
    El socket recibido queda en estado nulo. 
    */
    HCertificador(Socket &skt, ContadorBloq &contador, 
    MapaBloq &mapa, ClaveRSA &clave);
    
    /*Destruye un Hilo Certificador*/
    ~HCertificador();

    /*Ejecuta el hilo*/
    virtual void run() override;

    /*Detiene la ejecucion del hilo*/
    virtual void stop() override;
    
    /*
    Devuelve true si el hilo  termino de ejecutar, 
    false en caso contrario.
    */
    virtual bool is_dead() override;

    /*
    PRE: Recibe un protocolo (Protocolo &) de comunicacion con
    un cliente entrante.
    POST: Ejecuta el procedimiento para la creacion de un 
    certificado.
    */
    void crear(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) de comunicacion con
    un cliente entrante.
    POST: Ejecuta el procedimiento para la revocacion de un 
    certificado.
    */
    void revocar(Protocolo &proto);
};

#endif  //HILO_CERTIFICADOR_H
