#include "common_mapa_bloq.h"

#include <string>
#include <mutex>
#include <iostream>
#include <fstream>

/*
Crea un mapa bloqueante ordenado alfabeticamente, 
cuyas claves son los sujetos de certificados creados 
y sus valores asociados son ClaveRSA publicas de dichos
sujetos
*/
MapaBloq::MapaBloq(){}

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
MapaBloq::MapaBloq(std::istream &in) {
    std::string linea;
    std::string sepSujetoClave = "; ";
    //Spliter spliter;
    std::stringstream lineaStream;
    while (in.good()) {
        std::getline(in,linea);
        /*
        std::vector<std::string> sujetoClave;
        sujetoClave = spliter.split(linea, sepSujetoClave);
        */
        lineaStream.str(linea);
        std::string sujeto;
        std::getline(lineaStream, sujeto, sepSujetoClave);
        if (! lineaStream.good()){
            continue;
        }
        std::string claveCad = lineaStream.str();
        /*
        if (sujetoClave.size() != 2){
            continue;
        }
        */

        //std::string sujeto = std::move(sujetoClave[0]);
        //std::string claveCad = std::move(sujetoClave[1]);
        ClaveRSA clave;
        clave.actualizar(claveCad);
        this->agregar_si_no_esta(sujeto,clave);
    }
}

/*Destruye un mapa bloqueante*/
MapaBloq::~MapaBloq(){}

/*
PRE: Recibe un clave (std::string), y una clave publica
(ClaveRSA &).
POST: Devuelve true si la tupla clave,valor recibida no 
estaba y se agrego al mapa; o false si ya habia un elemento
con dicha clave, y por ende no se agrego.
*/
bool MapaBloq::agregar_si_no_esta(std::string &clave, ClaveRSA &valor){
    std::lock_guard<std::mutex> lock(this->centinela);
    if (this->mapa.find(clave) == this->mapa.end()){
        this->mapa[clave] = valor;
        return true;
    }
    return false;
}

/*
PRE: Recibe una clave (std::string) y un copia (ClaveRSA &)
POST: Devuelve true si la clave esta en el mapa y se si se 
copio a la referencia recibida, el valor asociado; o false
en caso de que no este.
*/
bool MapaBloq::obtener_si_esta(std::string &clave, ClaveRSA &copia){
    std::lock_guard<std::mutex> lock(this->centinela);
    if (this->mapa.find(clave) == this->mapa.end()){
        return false;
    }
    copia.copiarDesde(this->mapa[clave]); //Aqui hago una copia
    return true; 
}

/*
PRE: Recibe una clave (std:string &).
POST: Borra la clave y su valor asociado si 
es que existen en el mapa.
*/
void MapaBloq::borrar(std::string &clave){
    std::lock_guard<std::mutex> lock(this->centinela);
    this->mapa.erase(clave);
}

/*
PRE: Recibe un flujo de salida (ostream &out).
POST: Escribe en el flujo recibida, lineas del 
forma:
<sujeto>; <clave publica>\n
con todo la informacion del mapa bloqueante 
ordenando los sujetos alfabeticamente.
*/
void MapaBloq::guardar(std::ostream &out) const {
    std::lock_guard<std::mutex> lock(this->centinela);
    std::string separador = "; ";
    mapaStrRSA_t::const_iterator it;
    for (it = this->mapa.begin(); it != this->mapa.end(); ++it){
        out << it->first << separador << it->second.a_string_publica() << "\n";
    }
}

/*Sobrecarga del operador << de ostream para clase MapaBloq*/
std::ostream& operator<<(std::ostream &out, const MapaBloq &mapa){
    mapa.guardar(out);
    return out;
}
