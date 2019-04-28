#ifndef TIEMPO_H
#define TIEMPO_H
#include <string>

class Tiempo {
    public:
    //struct tm *tiempo;
    time_t tiempo;
    /*Inicializa el tiempo en el horario actual.*/
    Tiempo();

    /*Destruye el tiempo*/
    ~Tiempo();
    
    /*
    PRE: Recibe una cantidad (size_t) de dias a sumar.
    POST: Suma la cantidad recibida al tiempo actual
    */
    void sumar_dias(size_t dias);

    /*
    Devuelve una representacion (std::string) del tiempo actual:
    MM DD hh:mm:ss YYYY
    */
    std::string representar();
};

#endif // TIEMPO_H
