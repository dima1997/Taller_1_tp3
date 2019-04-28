#ifndef SOCKET_H
#define SOCKET_H
#include <cstring>

class Socket{
    public:
    int skt;

    private:
    /*
    PRE: Recibe un codigo de socket (int).
    POST: Inicializa un socket con dicho 
    codigo.
    */
    explicit Socket(int skt);
        
    public:
    /*Inicializa un Socket por default en estado invalido*/
    Socket();

    /*Destruye un socket.*/
    ~Socket();

    //move semantics

    /*
    PRE: Recibe una doble referencia a otro socket (Socket &&).
    POST: Crea un nuevo socket por movimiento semantico.
    El socket recibido que en estado nulo.
    */
    Socket(Socket&& otroSkt);

    /*
    PRE: Recibe una doble referencia a otro socket (Socket &&).
    POST: Mueve semanticamente los atributos del socket recido
    al actual, y devuelve una referencia a este ultimo.
    El socket recibido que en estado nulo.
    */
    Socket& operator=(Socket&& otroSkt);

    /*
    Constructor para cliente.
    PRE: Recibe los nombres (const char*) del host y 
    puerto al que se desea conectar el socket.
    POST: Inicializa un socket conectado al host y puerto 
    recibidos.
    Levanta OSError en caso de error de conexion.
    */
    Socket(const char *host, const char *puerto); 

    /*
    Constructor para Servidor.
    PRE: Recibe el nombre (char *) de un puerto al 
    cual enlazar el socket.
    POST: Inicializa un socket para que funcione de 
    forma PASIVA, es decir, se lo enlaza al puerto de
    nombre recibido. 
    Levanta OSError en caso de error de enlazamiento.
    */
    explicit Socket(const char *puerto);

    /*
    Metodo para Servidor.
    PRE: Recibe la cantidad de sockets entrantes a escuchar.
    El socket ya esta enlazado a algun puerto.
    POST: Pone a escuchar la cantidad de recibida de sockets 
    entrantes.
    Levanta OSError en caso de error.
    */
    void escuchar(size_t cuantosEscuchar);

    /*
    Metodo para Servidor.
    Devuelve un nuevo socket (Socket) obtenido de aceptar
    una conexion. 
    Levanta OSError en caso de error de aceptacion.
    */
    Socket aceptar();

    /*
    PRE: Recibe el modo en que se desea cerrar al socket:
    SHUT_RD, SHUT_WR, SHUT_RDWR
    POST: Cierra el socket. 
    */
    void cerrar_canal(int modoCierre);
    
    /*
    PRE: Recibe un mensaje (const char*) a enviar, y su 
    longitud (size_t).
    POST: Envia el mensaje, y devuelve la cantidad de 
    bytes enviados. Si esta cantidad es menor al largo 
    recibido es porque se cerro el canal de escritura.
    Levanta OSError en caso de error.
    */
    int enviar_todo(const char *msj, size_t largo);

    /*
    PRE: Recibe un buffer (char *) donde guardar los 
    bytes recibidos y el largo a llenar maximo (size_t) 
    del mismo.
    POST: Devuelve la cantidad de bytes recibidos.
    Si esta cantidad es menor al largo del buffer es 
    poque se cerro el canal de lectura.
    Levanta OSError en caso de error.
    */
    int recibir_algo(char *buffer, size_t largo);
};

#endif //SOCKET_H
