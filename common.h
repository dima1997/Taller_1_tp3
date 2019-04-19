class Hash {
    public:
    /*Crea un Hash*/
    Hash();

    /*Destruye un Hash*/
    ~Hash();

    /*
    PRE: Recibe una cadena de caracteres (std::string &),
    y su largo (size_t).
    POST: Devuelve la suma (uint16_t) de todos los caracteres 
    de la cadena recibida, expresados como enteros sin signo 
    de 2 bytes.
    */
    uint16_t hashear(std::string &cadena, size_t largo);  
};

class Encriptador {
    public:
    /*Creo un encriptador*/
    Encriptador();

    /*Destruye un encriptador*/
    ~Encriptador();

    /*
    PRE: Recibe un huella: entero sin signo de 4 bytes 
    (uint32_t); un exponente: entero sin signo de 1 bytes 
    (uint8_t); y un modulo: entero sin signo de 2 bytes
    (uint16_t).
    POST: Devuelve la encriptacion de la huella recibida:
    entero sin signo de 4 bytes (uint32_t), en base al 
    exponente y modulo recibidos. 
    */
    uint32_t encriptar(uint32_t huella, uint8_t exp, uint16_t mod); 
};