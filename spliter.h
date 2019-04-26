#ifndef SPLITER_H
#define SPLITER_H
#include <vector>
#include <string>

//class Spliter

class Spliter {
    public:
    /*Crea un spliter (un separador de strings)*/
    Spliter();

    /*Destruye un spliter*/
    ~Spliter();

    /*
    PRE: Recibe un dos referencias a string (std::string &): 
    una cadena, y un separador. 
    POST: Devuelve un vector de strings (std::vector<std::string>)
    donde cada string es una parte de la cadena recibidida que se 
    separa del resto de la misma, donde aparece el separador 
    recibido (sin incluirlo). 
    */
    std::vector<std::string> split(std::string &cad, std::string &sep);
};

#endif //SPLITER_H