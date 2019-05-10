#include "server_hilo_aceptador.h"

#include "server_hilo_certificador.h"

#include "common_socket.h"
#include "common_error.h"
#include "common_contador_bloq.h"
#include "common_mapa_bloq.h"
#include "common_claves.h"

#include <string>
#include <vector>

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
        /*
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
        */
        for (auto it = hilos.begin(); it != hilos.end(); ) {
            if (*it->is_dead()) {
                *it->join();
                delete *it;
                it = hilos.erase(it);
            } else {
                ++it;
            }
        }

        //Ahora solo quedan los hilos que siguen vivos
    }
    /*
    for (size_t i = 0; i < hilos.size(); ++i){
        hilos[i]->join();
        delete hilos[i];
    }
    */
    for (auto it = hilos.begin(); it != hilos.end(); ) {
        *it->join();
        delete *it;
        it = hilos.erase(it);
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
