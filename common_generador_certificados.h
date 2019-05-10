#ifndef GENERADOR_CERTIFICADO_H
#define GENERADOR_CERTIFICADO_H

#include "common_claves.h"
#include "common_protocolo.h"
#include "common_mapa_bloq.h"
#include "common_contador_bloq.h"

class GeneradorCertificados {
private:
    std::string sujeto;
    std::string fechaInicio;
    std::string fechaFin;
    ClaveRSA clavesCliente;
public:

    /*
    Crea un generador de certificados por defecto, 
    con sujeto, fecha de inicio y de fin nulos
    */
    GeneradorCertificados();

    /*
    PRE: Recibe un archivo de entrada (std::istream &) que 
    contenga informacion para generar un certificado de la 
    forma:
    <sujeto>
    <fecha de inicio del certificado>
    <fecha de finalizacion del certificado>
    o
    <sujeto>
    Y las claves publicas (ClaveRSA &) del cliente que desea 
    generar el certificado.
    POST: Crea un generador de certificados. 
    Si no se especificado fecha de inicio y finalizacion del 
    certificado se tomara la fecha actual como inicio, la 30
    dias despues la de finalizacion. 
    */
    GeneradorCertificados(const char *nombreArchivoInfo, ClaveRSA &clavesCliente);
    
    /*Destruye un generador de certificados.*/
    ~GeneradorCertificados();

    // No permite copías implicitas

    GeneradorCertificados(const GeneradorCertificados &otroGenerador) = delete;
    GeneradorCertificados& operator=(const GeneradorCertificados &otroGenerador) = delete;


    /*
    PRE: Recibe una doble referencia a otro generador de 
    certificados (GeneradorCertificados &&).
    POST: Construye un nuevo generador de certificados por 
    movimiento semantico de los atributos del recibido.
    El generador de certificados recibidido queda en estado nulo. 
    */
    GeneradorCertificados(GeneradorCertificados &&otroGenerador);

    /*
    PRE: Recibe una doble referencia a otro generador de certificados 
    (GeneradorCertificados &&).
    POST: Mueve semanticamente todos los atributos del generador
    recibido al actual.
    El generador recibido queda en estado nulo.
    Devuelve una referencia al actual generador 
    (GeneradorCertificados &). 
    */
    GeneradorCertificados& operator=(GeneradorCertificados &&otroGenerador);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para enviar informacion a una autoridad certificante.
    POST: Envia los parametros que la autoridad certicicadora 
    necesita para generar un certificado
    Levanta OSError en caso de error. 
    */
    void enviar_parametros(Protocolo &proto);

    /*
    PRE: Recibe un protocolo (Protocolo &) creado 
    para enviar informacion a una autoridad certificante.
    POST: Recibe los parametros que la autoridad certifiadora 
    necesita para generar un certificado
    Levanta OSError en caso de error. 
    */
    void recibir_parametros(Protocolo &proto);

    /*
    PRE: Recibe un mapa bloqueante (MapaBloq &) de sujetos asociados claves RSA.
    POST: Devuelve true si agrego un nuevo (que no existia antes) sujeto y clave 
    al mapa; false en caso contrario (y no se agrego nada). 
    */
    bool agregar_sujeto_clave_mapa(MapaBloq &sujetosClaves);

    /*
    PRE: Recibe un contador (ContadorBloq &) de numeros de serie, 
    y un asunto (std::string &) para el certificado.
    POST: Genera  y devuelve un certificado (Certificado).
    */
    Certificado generar(ContadorBloq &contador, std::string &asunto);

    /*
    PRE: Recibe un mapa bloqueante de sujetos y claves.
    POST: Elimina del mapa al sujeto del ceritificado si es que esta.
    */
    void borrar_sujeto_mapa(MapaBloq &sujetosClaves);
};

#endif //GENERADOR_CERTIFICADO_H
