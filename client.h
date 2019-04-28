#ifndef CLIENTE_H
#define CLIENTE_H
class Cliente {
    public:
    Socket *skt;
    /*
    PRE: Recibe los nombres (std::string &) del host y
    puerto al que se conectara. 
    POST: Inicializa un cliente que pide crear y revocar
    certificados.
    */
    Cliente(str &host, str &puerto);
    
    /*Destruye al cliente.*/
    ~Cliente();
    
    /*
    PRE: Recibe el nombre (string &) de un archivo que contengo 
    informacion del certificado a crear : subject, fecha de inicio
    fecha de finalizacion; y una referencia a un vector de strings 
    (std::vector<std::string>).
    POST: Carga el la informacion del archivo en el vector de 
    recibido.
    Devuelve true si logro lo anterior, false en caso contrario.
    */
    bool cargar_info(str &nombreArch, strVec &info);

    /*
    PRE: Recibe el nombre (std::string &) del archivo donde estan 
    las claves del cliente , el nombre (std::string &) del archivo 
    donde estan las claves publicas del servidor de la autoridad 
    certificante, y el nombre (std::string &) del archivo donde 
    esta la informacion para crear el certificado.
    POST: Devuelve true, si logro crear un certificado 
    correctamente, false en caso contrario.
    */
    bool crear_certif(str &ifCert, str &clvsClnt, str &pblSvr);

    /*
    PRE: Recibe un certificado (Certificado &) con la 
    informacion necesario para solicitar su creacion a
    una autoridad certificante.
    POST: Envia los parametros necesarios, en el orden 
    correcto, al servidor para crear un certificado.
    */
    bool solicitar_creacion(Certificado &certif);
    
    /*
    PRE: Recibe las claves del cliente y el servidor (ClaveRSA &).
    POST: Devuelve true si logro recibir y procesar las respuesta del servidor
    a la creacion de un certificado (comprueba hashes y guarda certificado); 
    devuelve false en caso de algun error ajeno los esperados.
    */
    bool recibir_certif(ClaveRSA &clvClnt, ClaveRSA &clvSvr);
    
    /*
    PRE: Recibe el nombre de 3 archivos (std::string): del archivo 
    que contiene el certificado a revocar, del archivo que contiene las
    claves del cliente, del archivo que contiene las claves publicas
    del servidor.
    POST Devuelve true si lograr solicitar  al revocacion de su certificado,
    y obtener una respuesta del servidor esperada, ya se que se logro revocar,
    como que si no lo logro dado huellas que no coinciden o si el certificado
    no esta vigente; o false en caso de algun error excepcional.
    */
    bool revocar_certif(str &nmbrCertif, str &nmbrClvClnt, str &nmbrClvSvr);
};

#endif // CLIENTE_H
