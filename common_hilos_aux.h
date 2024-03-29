#ifndef HILOS_AUX_H
#define HILOS_AUX_H
#include <string>
#include <mutex>

class ContadorBloq : public Archivable {
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
