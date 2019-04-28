#ifndef HILOS_AUX_H
#define HILOS_AUX_H
#include <string>
#include <mutex>
#include <map>
#include "common_claves.h"

typedef std::map<std::string,ClaveRSA> mapaStrRSA_t;

class MapaBloq {
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

    /*Destruye un mapa bloqueante*/
    ~MapaBloq();

    /*
    PRE: Recibe un flujo de entrada (istream &), 
    cuya lineas sean de la forma:
    <sujeto 1>; <clave publica 1>\n
    <sujeto 2>; <clave publica 2>\n
    .
    .
    POST: Carga el mapa con la informacion 
    provienente del flujo.  
    */
    void cargar(std::istream& in);
    
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
    bool agregarSiNoEsta(std::string &clave, ClaveRSA &valor);

    /*
    PRE: Recibe una clave (std::string) y un
    */
    bool obtenerSiEsta(std::string &clave, ClaveRSA &copia);

    /*
    PRE: Recibe una clave (std:string &).
    POST: Borra la clave y su valor asociado si 
    es que existen en el mapa.
    */
    void borrar(std::string &clave);
};

/*Sobrecarga del operador >> de istream para clase MapaBloq*/
std::istream& operator>>(std::istream &in, MapaBloq &mapa);

/*Sobrecarga del operador << de ostream para clase MapaBloq*/
std::ostream& operator<<(std::ostream &out, const MapaBloq &mapa);

class ContadorBloq {
    public:
    uint32_t cuenta;
    mutable std::mutex centinela;
    /*Crea un contador bloqueanteinicializado en cero.*/
    ContadorBloq();

    /*Destruye un contador bloqueante.*/
    ~ContadorBloq();

    /*
    Suma 1 al contador y devuelve el valor anterior 
    como un entero sin signo de 4 bytes(uint32_t).
    */
    uint32_t obtener_y_sumar_uno();

    /*
    PRE: Recibe un flujo de salida (ostream &).
    POST: Escribe en el flujo de salida una linea
    con valor del contador actual.
    */
    void guardar(std::ostream &out) const;

    /*
    PRE: Recibe un flujo de entrada (istream &), tal que su
    primer linea sea un numero entero si signo de 4 bytes.
    POST: Actualiza el valor de cuenta del contador con dicha 
    informacion. 
    El puntero al flujo recibido quedara en la siguiente linea
    a la recibida.
    */
    void cargar(std::istream &in); 
};

/*Sobrecarga del operador >> de istream para clase ContadorBloq*/
std::istream& operator>>(std::istream &in, ContadorBloq &contador);

/*Sobrecarga del operador << de ostream para clase ContadorBloq*/
std::ostream& operator<<(std::ostream &out, const ContadorBloq &contador);

#endif //HILOS_AUX_H
