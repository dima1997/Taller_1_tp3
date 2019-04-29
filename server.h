#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "server.h"

class Servidor {
    public:
    /*Inicializa un servidor*/
    Servidor();
    
    /*Destruye un servidor*/
    ~Servidor();
    
    /*
    PRE: Recibe el nombre (const char *) de un archivo tipo index, que
    contiene informacion sobre el proximo numero de serie a utilizar y
    sobre los certificados ya creados; un contador (ContadorBloq &) y
    un mapa (MapaBloq &) de sujetos y claves de certificados creados.
    POST: Carga el contador y mapa recibios con la informacion del 
    archivo.
    Levanta OSError en caso de error. 
    */
    void cargar_contador_mapa(const char* nombreArchIndice, 
    ContadorBloq &contador, MapaBloq &sujetosClaves);

    /*
    PRE: Recibe el nombre del archivo donde guardar el proximo
    numero de serie a utilizar e informacion sobre los certificados
    creados; el contador (ContadorBloq &) de numeros de serie, y el
    mapa de sujetos y claves de certificados creados.
    POST: Sobreescribe el archivo la ultima version del proximo numero
    de serie y sujetos y claves de certificados aun vigentes.
    Levanta OSError en caso de error. 
    */
    void guardar_contador_mapa(const char* nombreArchIndice, 
    ContadorBloq &contador, MapaBloq &sujetosClaves);

    /*
    PRE: Recibe el nombre de un archivo que contenga claves 
    publicas y/o privadas.
    POST: Devuelve una clave (ClaveRSA) cargado con la 
    informacion del archivo.
    Levanta OSError en caso de error. 
    */
    ClaveRSA cargar_claves(const char* nombreClaves);

    /*
    PRE: Recibe un 3 cadenas de caracteres (const char *): el puerto
    desde donde escuchar la entrada de clientes, el nombre del archivo
    donde se encuentran las claves del servidor, y el nombre del archivo
    tipo index donde esta el proximo numero de serie, y la informacion 
    de certificados creados.
    POST: Ejecuta un servidor que crea y revoca certificados
    */
    void ejecutar(const char* puerto, const char* claves, const char* indice);
};

#endif // SERVIDOR_H
