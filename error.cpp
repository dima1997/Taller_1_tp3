#include <errno.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "error.h"

/*Construye un OSerror*/
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
Devuelve una descripcion del error.
*/
const char* OSError::what()const noexcept{
    return this->msg_error;
}