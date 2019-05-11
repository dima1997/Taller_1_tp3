#include "common_error.h"

#include <errno.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>

/*
PRE: Recibe 1 o mas descripciones del error (const char *)
POST: Inicializa un OSerror
*/
OSError::OSError(const char* fmt, ...) noexcept {
    int _errno = errno;
    va_list args;
    va_start(args, fmt);
    int s = vsnprintf(this->msg_error, BUF_LEN, fmt, args);
    va_end(args);
    strncpy(msg_error+s, std::strerror(_errno), BUF_LEN-s);
    this->msg_error[BUF_LEN-1] = 0;
}

/*
Devuelve una descripcion (const char *) del error.
*/
const char* OSError::what() const noexcept{
    return this->msg_error;
}
