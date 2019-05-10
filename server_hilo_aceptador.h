#ifndef HILO_ACEPTADOR_H
#define HILO_ACEPTADOR_H

#include "common_thread.h"
#include "common_socket.h"
#include "common_protocolo.h"
#include "common_contador_bloq.h"
#include "common_mapa_bloq.h"
#include "common_claves.h"

class HAceptador : public Thread {
    public:
    Socket skt;
    bool estaMuerto;
    ContadorBloq &contador;
    MapaBloq &sujetosClaves;
    ClaveRSA &claveSvr;

    /*
    PRE: Recibe un socket (Socket &) configurado de forma pasiva
    para comunicarse escuchar clientes de cierto puerto.
    POST: Se inicializa un hilo aceptador.
    El Socket recibido queda en estado nulo.  
    */
    HAceptador(Socket &skt, ContadorBloq &contador, 
    MapaBloq &mapa, ClaveRSA &claveSvr); 

    /*Destruye un hilo aceptador*/
    ~HAceptador();

    /*Ejecuta el hilo aceptador*/
    virtual void run() override;
    
    /*Detiene la ejecucion del hilo*/
    virtual void stop() override;

    /*
    Devuelve true si el hilo aceptador termino de ejecutar, 
    false en caso contrario.
    */
    virtual bool is_dead() override;
};

#endif //HILO_ACEPTADOR_H
