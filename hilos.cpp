#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include "hilos.h"
#include "spliter.h"

/*
Crea un mapa bloqueante ordenado alfabeticamente, 
cuyas claves son los sujetos de certificados creados 
y sus valores asociados son ClaveRSA publicas de dichos
sujetos
*/
MapaBloq::MapaBloq(){}

/*Destruye un mapa bloqueante*/
MapaBloq::~MapaBloq(){}

/*
PRE: Recibe un clave (std::string), y una clave publica
(ClaveRSA &).
POST: Devuelve true si la tupla clave,valor recibida no 
estaba y se agrego al mapa; o false si ya habia un elemento
con dicha clave, y por ende no se agrego.
*/
bool MapaBloq::agregarSiNoEsta(std::string &clave, ClaveRSA &valor){
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
bool MapaBloq::obtenerSiEsta(std::string &clave, ClaveRSA &copia){
    std::lock_guard<std::mutex> lock(this->centinela);
    if (this->mapa.find(clave) == this->mapa.end()){
        return false;
    }
    copia = this->mapa[clave]; //Aqui hago una copia
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
PRE: Recibe un flujo de entrada (istream &), 
cuya lineas sean de la forma:
<sujeto 1>; <clave publica 1>\n
<sujeto 2>; <clave publica 2>\n
.
.
POST: Carga el mapa con la informacion 
provienente del flujo.  
El puntero al flujo recibido quedara al final
del flujo.
*/
void MapaBloq::cargar(std::istream& in) {
    std::string linea;
    std::string sepSujetoClave = "; ";
    std::string sepClave = " ";
    Spliter spliter;
    while (in.good()) {
        std::getline(in,linea);
        std::vector<std::string> sujetoClave;
        sujetoClave = spliter.split(linea, sepSujetoClave);
        if (sujetoClave.size() != 2){
            continue;
        }
        std::string sujeto = std::move(sujetoClave[0]);
        std::string claveCad = std::move(sujetoClave[1]);
        ClaveRSA clave;
        clave.actualizar(claveCad);
        this->agregarSiNoEsta(sujeto,clave);
    }
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
    std::string separador = "; ";
    mapaStrRSA_t::const_iterator it;
    for (it = this->mapa.begin(); it != this->mapa.end(); ++it){
        out << it->first << separador << it->second.a_string_publica() << "\n";
    }
}

/*Sobrecarga del operador >> de istream para clase MapaBloq*/
std::istream& operator>>(std::istream &in, MapaBloq &mapa){
    mapa.cargar(in);
    return in;
}


/*Sobrecarga del operador << de ostream para clase MapaBloq*/
std::ostream& operator<<(std::ostream &out, const MapaBloq &mapa){
    mapa.guardar(out);
    return out;
}

void pruebas_mapa_agregar_obtener(){
    MapaBloq mapa;
    std::string sujeto1 = "sujeto 1";
    ClaveRSA clave(1,2,3);
    bool seAgrego = mapa.agregarSiNoEsta(sujeto1, clave);
    std::cout << "Agrego al mapa: " << sujeto1 << " con clave rsa: " << clave.a_string() << ".\n";
    std::cout << "seAgrego es true (1) : " << seAgrego << "\n";
    
    ClaveRSA claveObtenida;
    bool seObtuvo = mapa.obtenerSiEsta(sujeto1, claveObtenida);
    std::cout << "seObtuvo es true (1) : " << seObtuvo << "\n";
    std::cout << "La clave obtenida es: " << claveObtenida.a_string() << "\n";
    std::cout << "Los atributos de la clave obtenida son correctos ?:\n"; 
    std::cout << "expPublico == 1: (1)" << (claveObtenida.expPublico == 1) << "\n";
    std::cout << "expPrivado == 2: (1)" << (claveObtenida.expPrivado == 2) << "\n";
    std::cout << "modulo == 3: (1)" << (claveObtenida.modulo == 3) << "\n";
    
    ClaveRSA otraClave(4,5,6);
    seAgrego = mapa.agregarSiNoEsta(sujeto1, otraClave);
    std::cout << "Se intenta agregar al mapa: " << sujeto1 << " con otra clave rsa: ";
    std::cout << otraClave.a_string() << "\n";
    std::cout << "seAgrego es false (0)? : " << seAgrego << "\n";
    
    seObtuvo = mapa.obtenerSiEsta(sujeto1, claveObtenida);
    std::cout << "seObtuvo es true (1) : " << seObtuvo << "\n";
    std::cout << "La clave obtenida es: " << claveObtenida.a_string() << "\n";
    std::cout << "Los atributos de la clave obtenida siguen siendo los anteriores ?:\n"; 
    std::cout << "expPublico == 1: (1)" << (claveObtenida.expPublico == 1) << "\n";
    std::cout << "expPrivado == 2: (1)" << (claveObtenida.expPrivado == 2) << "\n";
    std::cout << "modulo == 3: (1)" << (claveObtenida.modulo == 3) << "\n";
}

void prueba_mapa_guardar(){
    MapaBloq mapa;
    ClaveRSA clave1(1,2,3);
    ClaveRSA clave2(4,5,6);
    std::string sujetoA = "A Sujeto";
    std::string sujetoB = "B Sujeto";
    std::cout << "Agrego: " << sujetoB << " con clave: " ;
    std::cout << clave2.a_string_publica() << "\n";
    mapa.agregarSiNoEsta(sujetoB, clave2);
    std::cout << "Agrego: " << sujetoA << " con clave: " ;
    std::cout << clave1.a_string_publica() << "\n";
    mapa.agregarSiNoEsta(sujetoA, clave1);
    std::cout << "Guardo mapa en index_prueba.txt\n";

    std::ofstream archIndice;
    archIndice.open("indice_pruebas.txt");
    archIndice << mapa;
    archIndice.close();
}

void prueba_mapa_cargar(){
    MapaBloq mapa;
    std::ifstream archIndice;
    archIndice.open("indice_pruebas.txt");
    archIndice >> mapa;
    ClaveRSA claveObten;
    std::string sujetoA = "A Sujeto";
    std::string sujetoB = "B Sujeto";
    bool seObtuvo = false;
    seObtuvo = mapa.obtenerSiEsta(sujetoA, claveObten);
    std::cout << "Se obtuvo: " << sujetoA << "(1): " << seObtuvo; ;
    std::cout << " con clave: " << claveObten.a_string_publica() << "\n";
    seObtuvo = mapa.obtenerSiEsta(sujetoB, claveObten);
    std::cout << "Se obtuvo: " << sujetoB << "(1): " << seObtuvo; ;
    std::cout << " con clave: " << claveObten.a_string_publica() << "\n";
}

// tda ContadorBloq

/*Crea un contador bloqueanteinicializado en cero.*/
ContadorBloq::ContadorBloq() : cuenta(0){}

/*Destruye un contador bloqueante.*/
ContadorBloq::~ContadorBloq(){}

/*Suma 1 al contador y devuelve el valor anterior (uint32_t).*/
uint32_t ContadorBloq::operator++(int){
    std::lock_guard<std::mutex> lock(this->centinela);
    uint32_t valorAnterior = this->cuenta;
    this->cuenta += 1;
    return valorAnterior;
}

/*
PRE: Recibe un flujo de salida (ostream &).
POST: Escribe en el flujo de salida una linea
con valor del contador actual.
*/
void ContadorBloq::guardar(std::ostream &out) const {
    std::lock_guard<std::mutex> lock(this->centinela);
    out << std::to_string(this->cuenta) << "\n";
}

/*
PRE: Recibe un flujo de entrada (istream &), tal que su
primer linea sea un numero entero si signo de 4 bytes.
POST: Actualiza el valor de cuenta del contador con dicha 
informacion. 
El puntero al flujo recibido quedara en la siguiente linea
a la recibida.
*/
void ContadorBloq::cargar(std::istream &in){
    if (! in.good()){
        return;
    }
    std::string linea;
    std::getline(in, linea);
    std::lock_guard<std::mutex> lock(this->centinela);
    this->cuenta = (uint32_t) stoi(linea);
}

/*Sobrecarga del operador >> de istream para clase ContadorBloq*/
std::istream& operator>>(std::istream &in, ContadorBloq &contador){
    contador.cargar(in);
    return in;
}

/*Sobrecarga del operador << de ostream para clase ContadorBloq*/
std::ostream& operator<<(std::ostream &out, const ContadorBloq &contador){
    contador.guardar(out);
    return out;
}


int main(){
    //pruebas_mapa();
    //prueba_mapa_guardar();
    //prueba_mapa_cargar();
    return 0;
}


/*
error: no match for ‘operator=’ 
(operand types are 
‘std::map<std::__cxx11::basic_string<char>, ClaveRSA>::iterator 
{aka std::_Rb_tree_iterator<std::pair<const std::__cxx11::basic_string<char>, ClaveRSA> >}’
and 
‘std::map<std::__cxx11::basic_string<char>, ClaveRSA>::const_iterator 
{aka std::_Rb_tree_const_iterator<std::pair<const std::__cxx11::basic_string<char>, ClaveRSA> >}’)


*/