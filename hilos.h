#ifndef HILOS_H
#define HILOS_H
#include "claves.h"

class MapaBloq {
    public:
    std::map<std::string,ClaveRSA> mapa;
    std::mutex centinela;
    /*
    Crea un mapa bloqueante ordenado alfabeticamente, 
    cuyas claves son los sujetos de certificados creados 
    y sus valores asociados son ClaveRSA publicas de dichos
    sujetos
    */
    MapaBloq();

    /*Destruye un mapa bloqueante*/
    ~MapaBloq();

    /*
    PRE: Recibe un vector de informacion
    donde cada elemento es un string de la forma
    <sujeto>; <clave publica>
    POST: el mapa con dichos valores.  
    */
    //cargar(std::vector<std::string> &info);
    
    /*
    Guardar el mapa bloqueante en forma de:
    <sujeto>; <clave publica>, al final del 
    archivo recibido.
    */
    //void guardar(std::ostream &out);

    /*
    PRE: Recibe un clave (std::string), y una clave publica
    (ClaveRSA &).
    POST: Devuelve true si la tupla clave,valor recibida no 
    estaba y se agrego al mapa; o false si ya habia un elemento
    con dicha clave, y por ende no se agrego.
    */
    //bool agregarSiNoEsta(std::string &clave, &ClaveRSA valor);

    /*
    PRE: Recibe una clave (std::string) y un
    */
    //bool obtenerSiNoEsta(std::string &clave, ClaveRSA &valor);

    /*
    PRE: Recibe una clave (std:string &).
    POST: Borra la clave y su valor asociado si 
    es que existen en el mapa.
    */
    //void borrar(std::string &clave);
};
#endif //HILOS_H