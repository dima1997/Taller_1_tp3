#include "common_contador_bloq.h"

#include <iostream>

/*Crea un contador bloqueanteinicializado en uno.*/
ContadorBloq::ContadorBloq() : cuenta(1){}

/*
PRE: Recibe un flujo de entrada (istream &) cuyo puntero
apunte a una linea que contenga el numero de cuenta inicial 
del contador.
POST: Inicializa un contador a partir de dicha informacion.
El puntero del flujo queda en la siguiente linea a la recibida.
Si el puntero del flujo esta en el final del mismo, entonces el 
contador se iniciara en 1, por defecto
*/
ContadorBloq::ContadorBloq(std::istream &in){
    uint32_t valorDefecto = 1;
    if (! in.good()){
        this->cuenta = valorDefecto;
        return;
    }
    std::string linea;
    std::getline(in, linea);
    if (! in.good()){
        this->cuenta = valorDefecto;
        return;
    }
    std::lock_guard<std::mutex> lock(this->centinela);
    this->cuenta = (uint32_t) stoi(linea);
}

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

/*Sobrecarga del operador << de ostream para clase ContadorBloq*/
std::ostream& operator<<(std::ostream &out, const ContadorBloq &contador){
    contador.guardar(out);
    return out;
}
