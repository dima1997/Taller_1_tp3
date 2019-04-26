#include <vector>
#include <string>
#include "spliter.h"

//Spliter

/*Crea un spliter (un separador de strings)*/
Spliter::Spliter(){}

/*Destruye un spliter*/
Spliter::~Spliter(){}

/*
PRE: Recibe un dos referencias a string (std::string &): 
una cadena, y un separador. 
POST: Devuelve un vector de strings (std::vector<std::string>)
donde cada string es una parte de la cadena recibidida que se 
separa del resto de la misma, donde aparece el separador 
recibido (sin incluirlo). 
*/
std::vector<std::string> Spliter::split(std::string &cad, std::string &sep){
    std::vector<std::string> splitVec(0);
    std::string copiaCad(cad);
    std::string parteCadena;
    size_t pos;
    while ((pos = copiaCad.find(sep)) != std::string::npos) {
        parteCadena = copiaCad.substr(0, pos);
        splitVec.push_back(parteCadena);
        copiaCad.erase(0, pos + sep.size());
    }
    splitVec.push_back(copiaCad); 
    return std::move(splitVec);
}