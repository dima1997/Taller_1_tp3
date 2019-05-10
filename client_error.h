#ifndef CLIENTE_ERROR_H
#define CLIENTE_ERROR_H

#include "common_error.h"

class ClienteError : public OSError{
public:
    /*
    PRE: Recibe un varias descripciones de error.
    POST: Inicializa un error de cliente.
    */
    explicit ClienteError(const char* msgError) noexcept;

    /*Destruye un error de cliente*/
    virtual ~ClienteError() noexcept;
};

#endif // CLIENTE_ERROR_H
