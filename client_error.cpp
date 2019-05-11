#include "client_error.h"

#include <string>
/*
PRE: Recibe un varias descripciones de error.
POST: Inicializa un error de cliente.
*/
ClienteError::ClienteError(std::string &msgError) noexcept 
: msgError(msgError) {}

/*Destruye un error de cliente*/
ClienteError::~ClienteError() noexcept {}

/*
Devuelve una descripcion del error.
*/
const char *ClienteError::what() const noexcept {
    return this->msgError.data();
}