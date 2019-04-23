#include "server.h"
#include "common.h"

MapaBloq::MapaBloq(){}
MapaBloq::~MapaBloq


HiloFin::HiloFin(bool &seguirEjecutando) : seguirEjecutando(seguirEjecutando){}

HiloFin::~HiloFin(){} 

void HiloFin::run(){
    char caracter = 0;
    while (caracter != 'q' && seguirEjecutando){
        cin.get(caracter);
    }
    this->seguirEjecutando = false;
    return;
}

int main(int argc, const char* argv[]){
    //./server 2000 server.keys index.txt
    if (argc != 4){
        return 1;
    }
    std::string puerto = argv[1];
    std::string rutaClaves = argv[2];
    std::string rutaIndice = argv[3];
    Servidor svr(puerto, rutaClaves, rutaIndice);
    return svr.ejecutar();
}

Servidor::Servidor(std::string &prto, std::string &rtaClv, std::string &rtaInd){
    this->skt = new Socket();
    this->skt.enlazar(prto.data());

}

Servidor::~Servidor(){
    delete this->skt;
}

/*
Mientras no se reciba una letra 'q' por entrada estandar,
el servidor aceptar cliente y se comunica con ellos
creando o revocando certificados. 
*/
int Servidor::ejecutar(){
    bool seguirEjecutando = true;
    HiloFin hiloFin(seguirEjecutando);
    hiloFin.start();
    bool todoOK = true;
    while (seguirEjecutando ){
        Socket sktActivo();
        todoOK = this->skt.aceptar(sktActivo);
        if(! todoOK){
            break;
        }
        AutoridadCertif(sktActivo);
        AutoridadCertif.ejecutar();

    }  
    seguirEjecutando = false;
    hiloFin.join();
    return todoOK ? 0 : 1;
}