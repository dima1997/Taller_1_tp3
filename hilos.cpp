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
HCertfcdor::HCertfcdor(Socket &skt, ContadorBloq &contador, MapaBloq &mapa)
: skt(std::move(skt)), contador(contador), sujetosClaves(mapa) {}

/*Destruye un Hilo Certificador*/
HCertfcdor::~HCertfcdor(){}
void HCertfcdor::run(){
    Protocolo proto(this->skt);
    ///
}