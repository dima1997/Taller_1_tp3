#ifndef COMMON_H
#define COMMON_H
#include <cstdint>
#include <string>

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


class Socket{
    public:
    int skt;

    /*
    Inicializa el socket dejandolo en un estado
    invalido hasta tanto no llamar a otros metodos del 
    socket.
    */
    Socket();

    /*Destruye un socket.*/
    ~Socket();
    /*
    Metodo para cliente.
    PRE: Recibe los nombres (const char*) del host y 
    puerto al que se desea conectar el socket.
    */
    bool conectar(const char *host, const char *puerto);  

    /*
    Metodo para Servidor.
    PRE: Recibe un socket (skt_t *) ya creado, y el
    nombre (char *) de un puerto al cual enlazarlo.
    POST: Configura al socket para que funcion de 
    forma PASIVA, es decir, se lo enlaza al puerto de
    nombre recibido. 
    Devuelve true, si logro lo anterior, false en caso
    contrario.
    */
    bool enlazar(const char *puerto);

    /*
    Metodo para Servidor.
    PRE: Recibe la cantidad de sockets entrantes a escuchar.
    El socket ya esta enlazado a algun puerto.
    POST: Pone a escuchar la cantidad de recibida de sockets 
    entrantes.
    Devuelve true, si logro lo anterior, false en caso contrario.
    */
    bool escuchar(size_t cuantosEscuchar);

    /*
    Metodo para Servidor.
    PRE: Recibe un socket (Socket &) para ser configurado como 
    ACTIVO. 
    POST: Devuelve true si logro aceptar una nueva comunicacion y
    configurar al socket ACTIVO para la misma, o false en caso 
    contrario. 
    */
    bool aceptar(Socket &sktActivo); 

    /*
    PRE: Recibe el modo en que se desea cerrar al socket:
    SHUT_RD, SHUT_WR, SHUT_RDWR
    POST: Cierra el socket. 
    */
    void cerrar_canal(int modoCierre);
    
    /*
    PRE: Recibe un socket ya conectado (skt_t*), un 
    mensaje a enviar, y su longitud.
    POST: Devuelve true si logro enviar todo el mensaje, 
    false en caso contrario, dado algun error.
    */
    bool enviar_todo(const void *msj, size_t largo);

    /*
    PRE: Recibe un buffer (string &) donde guardar los 
    bytes recibidos y el largo a llenar (int) del mismo.
    POST: Devuelve la cantidad de bytes recibidos.
    Si esta cantidad es negativa, entonces significa 
    que hubo algun error de socket. Si la cantidad es 
    igual a cero, significa que ya no queda nada mas 
    que recibir.
    Nota: el ultimo caracter del buffer se completa con 
    \0, por lo que llena a lo sumo, largo -1 del buffer.
    */
    int socket_recibir_algo(void *buffer, size_t largo);
};

class Protocolo {
    public: 
    Socket &skt;
    /*
    PRE: Recibe una referencia al socket de comunicacion.
    POST: Inicializa un protocolo de comunicacion.
    */
    Protocolo(Socket &skt);

    /*Destruye el protocolo*/
    ~Protocolo();

    /*
    PRE: Recibe el largo/cantidad de bytes (uint32_t) 
    de un mensaje a enviar. 
    y el mensaje a enviar (const void *).
    POST: Envia el mensaje, primero enviando la longitud
    del mismo, seguido del mensaje.
    Devuelve true si logro enviar el mensaje, false en caso 
    contrario.
    */
    bool Protocolo::enviar_mensaje(const char *mensaje, uint32_t largo);

    /*
    PRE: Recibe una referencia a una string (std::string &).
    POST: Devuelve el largo del mensaje recibido, si logro
    correctamente recibir y guardar el mensaje recibido en
    la referencia recibida.
    Si ocurrio algun error devuelve 0;  
    */
    uint32_t Protocolo::recibir_mensaje(std::string &mensaje);

    /*
    PRE: Recibe un valor (uint32_t) y una cantidad de bytes 
    a enviar:
    1: un byte
    2: dos bytes
    4: cuatro bytes
    POST: lo bytes (recibidos) menos significativos del valor
    recibido. 
    Si bytes es 2 o 4, se envia en formato big endian.
    Devuelve true si logro enviar los bytes del valor,
    false en caso contrario.
    */
    bool Protocolo::enviar_bytes(uint32_t valor, size_t bytes);

    /*
    PRE: Recibe una referencia a un entero sin signo de 1 bytes
    (uint8_t &).
    POST: Devuelve true si logro recibir correctamente 1 bytes
    y guardalo en la referencia recibida; false en caso 
    contrario.
    */
    bool Protocolo::recibir_un_byte(uint8_t &valor);

    /*
    PRE: Recibe una referencia a un entero sin signo de 2 bytes
    (uint16_t &).
    POST: Devuelve true si logro recibir correctamente 2 bytes
    en big endian, y escribirlos en el endianess local en la
    referencia recibida; false en caso contrario.
    */
    bool Protocolo::recibir_dos_bytes(uint16_t &valor);

    /*
    PRE: Recibe una referencia a un entero sin signo de 4 bytes
    (uint32_t &).
    POST: Devuelve true si logro recibir correctametne 4 bytes
    en big endian, y escribirlos en el endianess local en la 
    referencia recibida; false en caso contrario.
    */
    bool recibir_cuatro_bytes(uint32_t &valor);
};


struct ClaveRSA{
    public:
    uint8_t expPublico;
    uint8_t expPrivado;
    uint16_t modulo;

    /*
    PRE: REcibe un exponente publico: entero sin 
    signo de 1 byte (uint8_t) entero sin signo de 
    2 bytes, un exponente privado: entero sin 
    signo de 1 bytes (uint8_t) y de un modulo: 
    entero sin signo de 2 bytes (uint16_t). 
    POST: Crea una clave rsa.
    */
    ClaveRSA(uint8_t expPublico, uint8_t expPrivado, uint16_t modulo);

    /*Destruye la clave rsa*/
    ~ClaveRSA();

    /*
    PRE: Recibe el nombre de un archivo que contiene las claves
    publicas y/o privadas de una entidad, de la forma:
    <exponente-publico> <exponente-privado> <modulo>
    o
    <exponente-publico> <modulo>
    POST: Actualiza los valores de la claves rsa, con los 
    encontrados en el archivo.
    */
    cargar_claves(std::string &nombreArchivo);
};

class Certificado {
    public:
    uint32_t numeroSerie;
    std::string asunto;
    std::string sujeto;
    std::string inicio;
    std::string fin;
    uint8_t exp;
    uint16_t mod;

    Certificado();
    Certificado(ClaveRSA &clvClnt, std::vector<std::string> &info);
    ~Certificado();
    bool recibir(Socket &skt);
    bool enviar(Socket &skt);
    uint32_t hashear();
    bool guardar();
};

#endif // COMMON_H
