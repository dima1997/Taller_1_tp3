#ifndef HILOS_H
#define HILOS_H
#include <thread>
#include "common_socket.h"
#include "common_protocolo.h"
#include "server_hilos_aux.h"

//class Thread 

class Thread {
    private:
    std::thread thread;

    public:
    /*Inicializa un Thread*/
    Thread() {}
    
    /*Inicia un nuevo hilo de ejecucion*/
    void start() {
        thread = std::thread(&Thread::run, this);
    }
    
    /*Espera a que el hilo en ejecucion termine*/
    void join() {
        thread.join();
    }
        
    /*Ejecuta la funcion del Thread*/
    virtual void run() = 0;
        
    /*Destruye el Thread*/
    virtual ~Thread() {}
        
    /*Impide que se copien hilos por parametro*/
    Thread(const Thread&) = delete;

    /*Evita que se copien hilos por asignacion*/
    Thread& operator=(const Thread&) = delete;

    /*
    Mueve semanticamente los recursos de un Thread a otro,
    por parametro.
    */
    Thread(Thread&& other) {
        this->thread = std::move(other.thread);
    }
        
    /*
    Mueve semanticamente los recursos de un Thread a otro, 
    por asignacion.
    */
    Thread& operator=(Thread&& other) {
        this->thread = std::move(other.thread);
        return *this;
    }

    virtual bool is_dead() = 0;
};

class HCertfcdor : public Thread {
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
    HCertfcdor(Socket &skt, ContadorBloq &contador, 
    MapaBloq &mapa, ClaveRSA &clave);
    
    /*Destruye un Hilo Certificador*/
    ~HCertfcdor();

    /*Ejecuta el hilo certificador*/
    virtual void run() override;
    
    /*Devuelve true si el hilo certificador termino de ejecutar*/
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
    
    /*Devuelve true si el hilo aceptador termino de ejecutar*/
    virtual bool is_dead() override;

    /*Le indica al hilo aceptardor que debe dejar ejecutarse*/
    void stop();
};

#endif //HILO_CERTIFICADOR_H
