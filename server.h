#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "common_mapa_bloq.h"
#include "common_contador_bloq.h"

#include <string>

class Servidor {
public:
    /*Inicializa un servidor*/
    Servidor();
    
    /*Destruye un servidor*/
    ~Servidor();

    /*
    PRE: Recibe el nombre (std::string &) del archivo donde guardar el proximo
    numero de serie a utilizar e informacion sobre los certificados
    creados; el contador (ContadorBloq &) de numeros de serie, y el
    mapa (MapaBloq &) de sujetos y claves de certificados creados.
    POST: Sobreescribe el archivo la ultima version del proximo numero
    de serie y sujetos y claves de certificados aun vigentes.
    Levanta OSError en caso de error. 
    */
    void guardar_contador_mapa(std::string &nombreArchIndice, 
    ContadorBloq &contador, MapaBloq &sujetosClaves);

    /*
    PRE: Recibe un 3 cadenas de caracteres (std::string &): el puerto
    desde donde escuchar la entrada de clientes, el nombre del archivo
    donde se encuentran las claves del servidor, y el nombre del archivo
    tipo index donde esta el proximo numero de serie, y la informacion 
    de certificados creados.
    POST: Ejecuta un servidor que crea y revoca certificados
    */
    void ejecutar(std::string &nombrePuerto, std::string &nombreArchivoClaves, 
    std::string &nombreArchivoIndice);
};

#endif // SERVIDOR_H
