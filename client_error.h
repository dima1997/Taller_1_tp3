#ifndef CLIENTE_ERROR_H
#define CLIENTE_ERROR_H

#include "common_error.h"

class ClientError : public OSError{
public:
    /*
    PRE: Recibe un varias descripciones de error.
    POST: Inicializa un error de cliente.
    */
    explicit ClientError(const char* fmt, ...) noexcept;

    /*Destruye un error de cliente*/
    virtual ~ClientError() noexcept;
};

#endif // CLIENTE_ERROR_H
