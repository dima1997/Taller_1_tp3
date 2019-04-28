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