#include <fstream>
#include <iostream>
#include <string>
#include "common_socket.h"
#include "common_claves.h"
#include "common_error.h"
#include "server_hilos.h"
#include "server_hilos_aux.h"
#include "server.h"
#define CANTIDAD_CLIENTES_ESCUCHAR 20

/*Inicializa un servidor*/
Servidor::Servidor(){}

/*Destruye un servidor*/
Servidor::~Servidor(){}

/*
PRE: Recibe el nombre (const char *) de un archivo tipo index, que
contiene informacion sobre el proximo numero de serie a utilizar y
sobre los certificados ya creados; un contador (ContadorBloq &) y
un mapa (MapaBloq &) de sujetos y claves de certificados creados.
POST: Carga el contador y mapa recibios con la informacion del 
archivo.
Levanta OSError en caso de error. 
*/
void Servidor::cargar_contador_mapa(const char* nombreArchIndice, 
ContadorBloq &contador, MapaBloq &sujetosClaves){
    Archivo arch(nombreArchIndice, 'r');
    arch >> contador;
    arch >> sujetosClaves;
    arch.cerrar();
}

/*
PRE: Recibe el nombre del archivo donde guardar el proximo
numero de serie a utilizar e informacion sobre los certificados
creados; el contador (ContadorBloq &) de numeros de serie, y el
mapa de sujetos y claves de certificados creados.
POST: Sobreescribe el archivo la ultima version del proximo numero
de serie y sujetos y claves de certificados aun vigentes.
Levanta OSError en caso de error. 
*/
void Servidor::guardar_contador_mapa(const char* nombreArchIndice, 
ContadorBloq &contador, MapaBloq &sujetosClaves){
    Archivo arch(nombreArchIndice, 'w');
    arch << contador;
    arch << sujetosClaves;
    arch.cerrar();
}

/*
PRE: Recibe el nombre de un archivo que contenga claves 
publicas y/o privadas.
POST: Devuelve una clave (ClaveRSA) cargado con la 
informacion del archivo.
Levanta OSError en caso de error. 
*/
ClaveRSA Servidor::cargar_claves(const char* nombreClaves){
    ClaveRSA clave;
    Archivo arch(nombreClaves, 'r');
    arch >> clave;
    arch.cerrar();
    return std::move(clave);
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
void Servidor::ejecutar(const char* puerto, const char* claves, 
const char* indice){
    try{
        Socket sktPasivo(puerto);
        sktPasivo.escuchar(CANTIDAD_CLIENTES_ESCUCHAR);
        ContadorBloq contador;
        MapaBloq sujetosClaves;
        this->cargar_contador_mapa(indice, contador, sujetosClaves);
        ClaveRSA claveSvr = std::move(this->cargar_claves(claves));
        HAceptador hiloAceptador(sktPasivo, contador, sujetosClaves, claveSvr);
        hiloAceptador.start();
        char caracterSalida = 0;
        std::cin.get(caracterSalida);
        while (caracterSalida != 'q'){
            std::cin.get(caracterSalida);
        }
        hiloAceptador.stop();
        hiloAceptador.join();
        this->guardar_contador_mapa(indice, contador, sujetosClaves);
    } catch (OSError &e){
        std::string err = "Error al ejecutar servidor";
        throw OSError(__FILE__,__LINE__, err.data());
    }
    return;
}

int main(int argc, const char* argv[]) {
    if (argc != 4) {
        return 1;
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

