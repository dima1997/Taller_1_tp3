#ifndef CLIENTE_H
#define CLIENTE_H
#include "common_socket.h"
class Cliente {
    public:
    Socket skt;

    /*
    PRE: Recibe los nombres (std::string &) del host y
    puerto al que se conectara. 
    POST: Inicializa un cliente que pide crear y revocar
    certificados.
    */
    Cliente(const char *host, const char *puerto);
    
    /*Destruye al cliente.*/
    ~Cliente();

    /*
    PRE: Recibe el nombre de un archivo que contiene informacion
    para crear un certificado: sujeto, fecha de inicio, fecha de 
    fin.
    POST: Devuelve un certificado (Certificado) con cargado
    con la informacion del archivo.
    Levanta OSError en caso de error.
    */
    Certificado cargar_info(const char *nombreInfoCertif);

    /*
    PRE: Recibe el nombre (const char *) de un archivo, 
    y un archivable (Archivable &).
    POST: Carga el archivable.
    Levanta OSError en caso de error.
    */
    void cargar_archivable(const char* nombre, Archivable &archivable);

    /*
    PRE: Recibe el nombre (const char *) de un archivo 
    que contenga un certificado.
    POST: Devuelve un certificado (Certificado)
    */
    Certificado cargar_certif(const char* nombreArchCertif);

    /*
    PRE: Recibe el nombre (const char *) de un archivo que 
    contiene un clave publica o completa (publica y privada).
    POST: Devuelve una clave (ClaveRSA) cargado con la 
    informacion del archivo.
    Levanta OSError en caso de error.
    */
    ClaveRSA cargar_claves(const char* nombreClaves); 
    /*
    PRE: Recibe un certificado (Certificado &).
    POST: Guardar el contenido del certificado en un
    archivo de nombre: "<sujeto del certificado>.cert"
    Levanta OSError en caso de error.
    */
    void guardar_certif(Certificado &certif);

    /*
    PRE: Recibe un certificado (Certificado &).
    POST: devuelve (uint32_t) e imprime el hash 
    del certicado, de la forma:
    "Hash calculado: <hash calculado>\n"
    */
    uint32_t calcular_imprimir_hash(Certificado &certif);

    /*
    PRE: Recibe un huella del servidor (uint32_t), las claves
    (ClaveRSA) del cliente y la publica del servidor.
    POST: Desencripta la huella recibida e imprime en el proceso:
    "Huella del servidor: <huella del servidor>"
    "Hash del servidor: <huella desencriptada>"
    Devuelve la huella desencritada.
    */
    uint32_t desencrip_imprimir_huella_svr(uint32_t huellaSvr,
    ClaveRSA &clvClnt, ClaveRSA clvSvr);

    /*
    PRE: Recibe un hash del cliente (uint32_t), las claves
    (ClaveRSA) del cliente y la publica del servidor.
    POST: Encripta el hash recibido y lo imprime en el proceso:
    "Hash encriptado con la clave privada: <hash encriptado con clave...>"
    "Huella enviada : <hash completamente encriptado>"
    */
    uint32_t encrip_imprimir_hash_clnt(uint32_t hashClnt,
    ClaveRSA &clvClnt, ClaveRSA &clvSvr);

    /*
    PRE: Recibe el nombre (const char *) del archivo donde esta 
    la informacion con la cual solicitar la creacion de un 
    certificado, del archivo con las claves del cliente, y el
    de la claves publicas del servidor.
    POST: Ejecuta el procedimiento para crear un certificado.
    Levanta OSError en caso de error.
    */
    void crear_certif(const char *nombreInfoCertif, 
    const char *nombreClavesClnt, const char *nombreClavesSvr);

    
    /*
    PRE: Recibe el nombre de 3 archivos (const char *): del archivo 
    que contiene el certificado a revocar, del archivo que contiene las
    claves del cliente, del archivo que contiene las claves publicas
    del servidor.
    POST: Ejecuta el procedimiento para revocar un certificado.
    Levanta OSError en caso de error.
    */
    void revocar_certif(const char *nombreCertif, 
    const char *nombreClavesClnt, const char *nombreClavesSvr);
};

#endif // CLIENTE_H
