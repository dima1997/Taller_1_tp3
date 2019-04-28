#ifndef HILOS_H
#define HILOS_H
#include <thread>
#include "socket.h"
#include "hilos_aux.h"

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
};

class HCertfcdor : public Thread {
    public:
    Socket skt;
    
    private:
    ClaveRSA &claveServidor;
    ContadorBloq &contador;
    MapaBloq &sujetosClaves;

    public:
    HCertfcdor(Socket &sskt, ContadorBloq &contador, 
    MapaBloq &mapa, ClaveRSA &clave);
    ~HCertfcdor();
    virtual void run() override;
};


#endif //HILO_CERTIFICADOR_H