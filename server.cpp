#include "server.h"

#include "server_hilo_aceptador.h"

#include "common_socket.h"
#include "common_claves.h"
#include "common_error.h"
#include "common_contador_bloq.h"
#include "common_mapa_bloq.h"

#include <fstream>
#include <iostream>
#include <string>

#define CANTIDAD_CLIENTES_ESCUCHAR 20

/*Inicializa un servidor*/
Servidor::Servidor(){}

/*Destruye un servidor*/
Servidor::~Servidor(){}

/*
PRE: Recibe el nombre (std::string) del archivo donde guardar el proximo
numero de serie a utilizar e informacion sobre los certificados
creados; el contador (ContadorBloq &) de numeros de serie, y el
mapa (MpaBloq &) de sujetos y claves de certificados creados.
POST: Sobreescribe el archivo la ultima version del proximo numero
de serie y sujetos y claves de certificados aun vigentes.
Levanta OSError en caso de error. 
*/
void Servidor::guardar_contador_mapa(std::string &nombreArchivoIndice, 
ContadorBloq &contador, MapaBloq &sujetosClaves){
    std::ofstream out(nombreArchivoIndice, std::ios::out);
    if (! out.is_open()){
        std::string err = "Error al abrir archivo de indices.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    out << contador;
    out << sujetosClaves;
}

/*
PRE: Recibe un 3 cadenas de caracteres (const char *): el puerto
desde donde escuchar la entrada de clientes, el nombre del archivo
donde se encuentran las claves del servidor, y el nombre del archivo
tipo index donde esta el proximo numero de serie, y la informacion 
de certificados creados.
POST: Ejecuta un servidor que crea y revoca certificados
Levanta OSError en caso de error.
*/
void Servidor::ejecutar(std::string &nombrePuerto, 
std::string &nombreArchivoClaves, std::string &nombreArchivoIndice) {
    try{
        Socket sktPasivo(nombrePuerto.data());
        sktPasivo.escuchar(CANTIDAD_CLIENTES_ESCUCHAR);
        std::ifstream in(nombreArchivoIndice, std::ios::out);
        if (! in.is_open()){
            std::string err = "Error al abrir archivo de indices.";
            throw OSError(__FILE__,__LINE__,err.data());
        }
        ContadorBloq contador(in);
        MapaBloq sujetosClaves(in);
        ClaveRSA claveSvr(nombreArchivoClaves);
        HAceptador hiloAceptador(sktPasivo, contador, sujetosClaves, claveSvr);
        hiloAceptador.start();
        char caracterSalida = 0;
        std::cin.get(caracterSalida);
        while (caracterSalida != 'q'){
            std::cin.get(caracterSalida);
        }
        hiloAceptador.stop();
        hiloAceptador.join();
        this->guardar_contador_mapa(nombreArchivoIndice, contador,
            sujetosClaves);
    } catch (OSError &error){
        std::string err = "Error al ejecutar servidor.";
        throw OSError(__FILE__,__LINE__, err.data());
    }
    return;
}

int main(int argc, const char* argv[]) {
    if (argc != 4) {
        return 1;
    }
    std::string puerto = argv[1];
    std::string claves = argv[2];
    std::string indice = argv[3];
    try {
        Servidor servidor;
        servidor.ejecutar(puerto, claves, indice);
    } catch (OSError &error) {
        return 1;
    }
    return 0;
}

