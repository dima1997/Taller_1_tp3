#include "client_error.h"

/*
PRE: Recibe un varias descripciones de error.
POST: Inicializa un error de cliente.
*/
ClienteError::ClienteError(const char* msgError) noexcept : OSError(msgError) {}

/*Destruye un error de cliente*/
ClienteError::~ClienteError() {}
