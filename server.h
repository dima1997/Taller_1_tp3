#include <map>

class ContadorBloq {
public:
    /*
    PRE: Recibe un valor inicial para el contador
    POST: Inicializa un contador bloqueante.
    */
    ContadorBloq(size_t inicio);

    /*Destruye el contador bloqueante*/
    ~ContadorBloq();

    /*
    Obtiene el valor actual del contador 
    y suma uno al contador
    */
    size_t obtenerYSumarUno();
};

class MapaBloq {
    public:
    /*Crea un mapa bloqueante ordenado alfabeticamente*/
    MapaBloq();

    /*Destruye un mapa bloqueante*/
    ~MapaBloq();

    /*
    PRE: Recibe un vector de informacion
    donde cada elemento es un string de la forma
    <sujeto>; <clave publica>
    POST: el mapa con dichos valores.  
    */
    cargar(std::vector<std::string> &info);
    
    /*
    Guardar el mapa bloqueante en forma de:
    <sujeto>; <clave publica>, al final del 
    archivo recibido.
    */
    guardar(std::ostream &out);

    /*
    PRE: Recibe un clave (std::string), y una clave publica
    (ClaveRSA &).
    POST: guarda la clave publica bajo la clave 
    recibida.
    */
    bool agregar(std::string &clave, ClaveRSA &clave);

    /*
    PRE: Recibe un clave (std::string), y una clave publica
    (ClaveRSA &).
    POST: guarda la clave publica bajo la clave 
    recibida.
    */
    bool obtener(std::string &clave, ClaveRSA &clave){
        
    }

    /*
    PRE: Recibe una clave (std:string &).
    POST: Devuelve true si la clave y su valor asociado 
    fueron borrados del mapa, false en caso de no 
    pertenecer a el.
    */
    bool borrar(std::string &clave);
};

class Thread {
    private:
    std::thread thread;

    public:
    /*Inicializa un Thread*/
    Thread() {}
    
    /*Inicia un nuevo hilo de ejecucion*/
    void start() {
        thread = std::thread(&Thread::run, this);
    }
    
    /*Espera a que el hilo en ejecucion termine*/
    void join() {
        thread.join();
    }
        
    /*Ejecuta la funcion del Thread*/
    virtual void run() = 0;
        
    /*Destruye el Thread*/
    virtual ~Thread() {}
        
    /*Impide que se copien hilos por parametro*/
    Thread(const Thread&) = delete;

    /*Evita que se copien hilos por asignacion*/
    Thread& operator=(const Thread&) = delete;

    /*
    Mueve semanticamente los recursos de un Thread a otro,
    por parametro.
    */
    Thread(Thread&& other) {
        this->thread = std::move(other.thread);
    }
        
    /*
    Mueve semanticamente los recursos de un Thread a otro, 
    por asignacion.
    */
    Thread& operator=(Thread&& other) {
        this->thread = std::move(other.thread);
        return *this;
    }
};

//clase HiloFin

class HiloFin: public Thread {
    private:
    bool &seguirEjecutando;
    public: 
    /*
    PRE: Recibe una referencia a un booleano que indica si algo externo
    debe seguir ejecutandose.
    POST: Inicializa un hilo de fin.
    */
    explicit HiloFin(bool &seguirEjecutando);
    
    /*Destruye el hilo de fin.*/
    ~HiloBF();

    /*
    Lee de la entrada estandard un caracter hasta recibir una letra 'q'.
    Cuando eso pasa modifica el valor del booleano con que se inicializo
    a false.
    */
    virtual void run() override;
};