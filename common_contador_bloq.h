#ifndef CONTADOR_BLOQ_H
#define CONTADOR_BLOQ_H
#include <string>
#include <mutex>
#include <iostream>

class ContadorBloq {
    public:
    uint32_t cuenta;
    mutable std::mutex centinela;
    /*Crea un contador bloqueante inicializado en uno.*/
    ContadorBloq();

    /*
    PRE: Recibe un flujo de entrada (istream &) cuyo puntero
    apunte a una linea que contenga el numero de cuenta inicial 
    del contador.
    POST: Inicializa un contador a partir de dicha informacion.
    El puntero del flujo queda en la siguiente linea a la recibida.
    Si el puntero del flujo esta en el final del mismo, entonces el 
    contador se iniciara en 1, por defecto
    */
    explicit ContadorBloq(std::istream &in);

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
};

/*Sobrecarga del operador << de ostream para clase ContadorBloq*/
std::ostream& operator<<(std::ostream &out, const ContadorBloq &contador);

#endif // CONTADOR_BLOQ_H
