#ifndef ARCHIVABLE_H
#define ARCHIVABLE_H

class Archivable {
    public:
    /*Constructor de la clase abstracta Archivable.*/
    Archivable(){}

    /*Destructor de la clase abstracta Archivable.*/
    virtual ~Archivable(){}

    /*
    PRE: Recibe un flujo de entrada (std::istream &).
    POST: No devuelve nada.
    */
    virtual void cargar(std::istream &in) = 0;

    /*
    PRE: Recibe un flujo de salida (std::ostream &).
    POST: No devuelve nada.
    */
    virtual void guardar(std::ostream &out) const = 0;
};

#endif // ARCHIVABLE_H
