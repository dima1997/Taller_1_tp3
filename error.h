#ifndef ERROR_H
#define ERROR_H
#include <typeinfo>
#define BUF_LEN 256

class OSError : public std::exception{
    private:
    char msg_error[BUF_LEN];
    
    public:
    /*Crea un OSError*/
    explicit OSError(const char* fmt, ...) noexcept;
    
    /*
    Devuelve una cadena de caracteres (const char*) 
    describiendo el error
    */
    virtual const char *what() const noexcept;
    
    /*Destruye un OSError*/
    virtual ~OSError() noexcept {}   
};
#endif //ERROR_H
