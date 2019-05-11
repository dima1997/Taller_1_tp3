#ifndef ERROR_H
#define ERROR_H
#include <typeinfo>
#define BUF_LEN 256

class OSError : public std::exception{
private:
    char msg_error[BUF_LEN];
    
public:
    /*
    PRE: Recibe 1 o mas descripciones del error (const char *)
    POST: Inicializa un OSerror
    */
    explicit OSError(const char* fmt, ...) noexcept;
    
    /*
    Devuelve una descripcion (const char *) del error.
    */
    virtual const char *what() const noexcept;
    
    /*Destruye un OSError*/
    virtual ~OSError() noexcept {}   
};
#endif //ERROR_H
