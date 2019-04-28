#include "client.h"

/*
PRE: Recibe los nombres (std::string &) del host y
puerto al que se conectara. 
POST: Inicializa un cliente que pide crear y revocar
certificados.
*/
Cliente::Cliente(const char *host, const char *puerto) 
: skt(host, puerto){}

/*Destruye al cliente.*/
Cliente::~Cliente(){}

/*
PRE: Recibe el nombre (std::string &) del archivo donde estan 
las claves del cliente , el nombre (std::string &) del archivo 
donde estan las claves publicas del servidor de la autoridad 
certificante, y el nombre (std::string &) del archivo donde 
esta la informacion para crear el certificado.
POST: Devuelve true, si logro crear un certificado 
correctamente, false en caso contrario.
*/
bool Cliente::crear_certif(const char *infoCertif, const char *clvsClnt, 
const char *clvsSvr){
    Certificado certif;
    
    ifstream archInfo;
    archInfo.open(infoCertif);
    certif.cargar(archInfo);
    archInfo.close();

    ClaveRSA clavesClnt;
    ifstream archClavesCliente;
    archClavesCliente.open(clvsCliente);
    archClavesCliente >> clavesClnt;
    archClavesCliente.close();

    certif.setClave(clavesClnt);

    ClaveRSA clavesSvr;
    ifstream archClavesServidor;
    archClavesServidor.open(clvsSvr);
    archClavesServidor >> clavesSvr;
    archClavesServidor.close();
    try {
        Protocolo proto(this->skt);
        proto.enviar_bytes(0,1);
        certif.enviar_parametros(proto);
        uint8_t respuesta = 3;
        proto.recibir_un_byte(respuesta);
        if (respuesta == 1){
            std::cout << "Error: ya existe un certificado.\n";
            return;
        }   
        certif.recibir(proto);
        uint32_t huellaSvr;
        proto.recibir_cuatro_bytes(huellaSvr);
        std::cout << "Huella del servidor: " << std::to_string(huellaSvr) << "\n";
        uint32_t hashSvr = clavesClnt.encriptar_privado(huellaServidor);
        hashSvr = clavesSvr.encriptar_publico(hashSvr);
        std::cout << "Hash del servidor: " << std::to_string(hashSvr) << "\n";
        uint32_t hashCalculado = certif.hashear();
        std::cout << "Hash calculado: " << std::to_string(hashCalculado) << "\n";
        if (hashSvr != hashCalculado){
            proto.enviar_bytes(1,1);
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
        proto.enviar_bytes(0,1);
    } catch (OSError &e){
        std::string err = "Error al crear certificado.";
        throw OSError(err.data());
    }
    std::ofstream archCertif;
    std::string nombreArchCertif = certif.getSujeto() + ".cert";
    archCertif.open(nombreArchCertif);
    archCertif << certif;
    archCertif.close();
}

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
bool Cliente::revocar_certif(const char *nombreCertif, 
const char *nombreClavesClnt, const char *nombreClavesSvr){
    Certificado certif;
    
    ifstream archCertif;
    archCertif.open(nombreCertif);
    archCertif >> certif;
    archCertif.close();

    ClaveRSA clavesClnt;

    ifstream archClavesClnt;
    archClavesClnt.open(nombreClavesClnt);
    archClavesClnt >> clavesClnt;
    archClavesClnt.close();

    ClaveRSA clavesSvr;

    ifstream archClavesSvr;
    archClavesSvr.open(nombreClavesSvr);
    archClavesSvr >> clavesSvr;
    archClavesSvr.close();
    try {   
        Protocolo proto(this->skt);
        proto.enviar_bytes(1,1);
        certif.enviar(proto);
        uint32_t huellaCliente = certif.hashear();
        std::cout << "Hash calculado: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        huellaCliente = clavesClnt.encriptar_privado(huellaCliente);
        std::cout << "Hash encriptado con clave privada: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        huellaCliente = clavesSvr.encriptar_publico(huellaCliente);
        std::cout << "Huella enviada: ";
        std::cout << std::to_string(huellaCliente) << "\n";
        proto.enviar_bytes(huellaCliente, 4);
        uint8_t respuesta;
        proto.recibir_un_byte(respuesta);
        if (respuesta == 1){
            //No hay certificado registrado
            std::cout << "Error: usuario no registrado.\n";
            return;
        }
        if (respuesta == 2){
            //Los hashes no coinciden
            std::cout << "Error: los hashes no coinciden.\n";
            return;
        }
    } catch (OSError &e){
        std::string err = "Error al revocar certificado.";
        throw OSError(err.data());
    }
}

int main(int argc, const char* argv[]){
    if (argc != 7){
        std::cout << "Error: argumentos invalidos.\n";
    }
    try {
        const char *host = argv[1];
        const char *puerto = argv[2];
        Cliente cliente(host, puerto);
        
        const char *comando = argv[3];
        const char *nombreCertif = argv[4];
        const char *nombreClvClnt = argv[5];
        const char *nombreClvSvr = argv[6];

        if (comando == "new"){
            //./client localhost 2000 new certificate.req client.keys serverPublic.keys
            cliente.crear_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else if (comando == "revoke"){
            //./client localhost 2000 revoke “Federico Manuel Gomez Peter.cert” client.keys serverPublic.keys
            cliente.revocar_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
        } else {
            std::cout << "Error: argumentos invalidos.\n";   
        }
    } catch (OSError &e){
        return 1;
    }
    return 0;
}