#include "client_error.h"

/*
PRE: Recibe un varias descripciones de error.
POST: Inicializa un error de cliente.
*/
ClienteError::ClienteError(const char* fmt, ...) : OSError(fmt, ...) {}

/*Destruye un error de cliente*/
ClienteError::~ClienteError() {}
