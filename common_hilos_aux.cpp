#include "commmon_contador_bloq.h"


// tda ContadorBloq

/*Crea un contador bloqueanteinicializado en uno.*/
ContadorBloq::ContadorBloq() : cuenta(1){}

/*Destruye un contador bloqueante.*/
ContadorBloq::~ContadorBloq(){}

/*Suma 1 al contador y devuelve el valor anterior (uint32_t).*/
uint32_t ContadorBloq::obtener_y_sumar_uno(){
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
    if (! in.good()){
        return;
    }
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
