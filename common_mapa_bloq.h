#ifndef MAPA_BLOQ_H
#define MAPA_BLOQ_H

#include "common_claves.h"

#include <string>
#include <mutex>
#include <map>

typedef std::map<std::string,ClaveRSA> mapaStrRSA_t;

class MapaBloq : public Archivable {
    public:
    mapaStrRSA_t mapa;
    mutable std::mutex centinela;
    /*
    Crea un mapa bloqueante ordenado alfabeticamente, 
    cuyas claves son los sujetos de certificados creados 
    y sus valores asociados son ClaveRSA publicas de dichos
    sujetos
    */
    MapaBloq();

    /*
    PRE: Recibe un flujo de entrada (istream &), 
    cuya lineas sean de la forma:
    <sujeto 1>; <clave publica 1>\n
    <sujeto 2>; <clave publica 2>\n
    .
    .
    POST: Inicializa mapa bloqueante con la informacion 
    provienente del flujo.  
    El puntero al flujo recibido quedara al final
    del mismo.
    */
    MapaBloq(std::istream &in);

    /*Destruye un mapa bloqueante*/
    ~MapaBloq();
    
    /*
    Guardar el mapa bloqueante en forma de:
    <sujeto>; <clave publica>, al final del 
    archivo recibido.
    */
    void guardar(std::ostream &out) const;

    /*
    PRE: Recibe un clave (std::string), y una clave publica
    (ClaveRSA &).
    POST: Devuelve true si la tupla clave,valor recibida no 
    estaba y se agrego al mapa; o false si ya habia un elemento
    con dicha clave, y por ende no se agrego.
    */
    bool agregar_si_no_esta(std::string &clave, ClaveRSA &valor);

    /*
    PRE: Recibe una clave (std::string) y un
    */
    bool obtener_si_esta(std::string &clave, ClaveRSA &copia);

    /*
    PRE: Recibe una clave (std:string &).
    POST: Borra la clave y su valor asociado si 
    es que existen en el mapa.
    */
    void borrar(std::string &clave);
};

/*Sobrecarga del operador << de ostream para clase MapaBloq*/
std::ostream& operator<<(std::ostream &out, const MapaBloq &mapa);

#endif // MAPA_BLOQ_H