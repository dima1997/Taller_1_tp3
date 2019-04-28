#include <fstream>
#include <iostream>
#include "server.h"
#include "socket.h"
#include "hilos.h"
#include "hilos_aux.h"
#include "claves.h"
#include "error.h"

/*Inicializa un servidor*/
Servidor::Servidor(){}

/*Destruye un servidor*/
Servidor::~Servidor(){}

/*
PRE: Recibe un 3 cadenas de caracteres (const char *): el puerto
desde donde escuchar la entrada de clientes, el nombre del archivo
donde se encuentran las claves del servidor, y el nombre del archivo
tipo index donde esta el proximo numero de serie, y la informacion 
de certificados creados.
POST: Ejecuta un servidor que crea y revoca certificados
*/
void Servidor::ejecutar(const char* puerto, const char* claves, 
const char* indice){
    Socket sktPasivo(puerto);
    sktPasivo.escuchar(20);
    ContadorBloq contador;
    MapaBloq sujetosClaves;

    std::ifstream archIndiceEntrada;
    archIndiceEntrada.open(indice);
    archIndiceEntrada >> contador;
    archIndiceEntrada >> sujetosClaves;
    archIndiceEntrada.close();
    
    ClaveRSA claveSvr;
    
    std::ifstream archClaves;
    archClaves.open(claves);
    archClaves >> claveSvr;
    archClaves.close();

    HAceptador hiloAceptador(sktPasivo, contador, sujetosClaves, claveSvr);
    hiloAceptador.start();
    char caracterSalida = 0;
    std::cin.get(caracterSalida);
    while (caracterSalida != 'q'){
        std::cin.get(caracterSalida);
    }
    hiloAceptador.finalizar();
    hiloAceptador.join();

    std::ofstream archIndiceSalida;
    archIndiceSalida.open(indice);
    archIndiceSalida << contador;
    archIndiceSalida << sujetosClaves;
    archIndiceSalida.close();
    return;
}

int main(int argc, const char *argv[]){
    if (argc != 4){
        return 1; //Aunque no deberia pasar
    }
    const char *puerto = argv[1];
    const char *claves = argv[2];
    const char *indice = argv[3];
    try {
        Servidor servidor;
        servidor.ejecutar(puerto, claves, indice);
    } catch (OSError &e) {
        return 1;
    }
    return 0;
}