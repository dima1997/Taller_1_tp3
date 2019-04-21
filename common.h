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
    POST: Inicializa un protocolo de comunicacion, donde
    los mensaje se enviar primero indicando su longitud en
    enteros sin signo de 4 bytes big endian, y luego se 
    envia el mensaje.
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
    bool enviar_cadena(const char *mensaje, uint32_t largo);

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
    bool enviar_bytes(uint32_t valor, size_t bytes);

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

#endif // COMMON_H
