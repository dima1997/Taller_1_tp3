#ifndef CLIENTE_ERROR_H
#define CLIENTE_ERROR_H

#include "common_error.h"

#include <string>

class ClienteError : public std::exception{
private:
    std::string msgError;

public:
    /*
    PRE: Recibe un mensaje de error (std::string &)
    POST: Inicializa un error de cliente.
    */
    explicit ClienteError(std::string &msgError) noexcept;


    /*Destruye un error de cliente*/
    virtual ~ClienteError() noexcept;

    /*
    Devuelve una descripcion del error.
    */
    const char* what() const noexcept;
};

#endif // CLIENTE_ERROR_H
