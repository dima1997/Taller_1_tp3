#include "client_comando_crear.h"

/*
PRE: Recibe un protocolo de comunicacion (Protocolo &), las claves (ClaveRSA &) privadas del cliente
las claves publicas del servidor, y un generador de certificados (GeneradorCertificados &).
POST: Inicializa un comando creador de certificados del cliente.
*/
ClienteComandoCrear::ClienteComandoCrear(Protocolo &proto, ClaveRSA &privCliente, 
ClaveRSA &pubSvr, GeneradorCertificados &gen) : proto(proto), 
privCliente(privCliente), pubServidor(pubSvr), genCertif(gen) {}

/*
Ejecuta el comando crear del cliente.
Levanta ClienteError en caso de un error esperado 
en el comportamiento del cliente.
Levanta OSError en caso de otro error. 
*/
void ClienteComandoCrear::ejecutar(){
    try {
        this->proto.enviar_bytes(0,1);
        this->genCertif.enviar_parametros(this->proto);
        uint8_t respuesta = this->proto.recibir_un_byte();
        if (respuesta == 1){
            std::string err = "Error: ya existe un certificado.";
            throw ClienteError(err);
        }   
        Certificado certif;
        certif.recibir(proto);
        uint32_t huellaSvr = this->proto.recibir_cuatro_bytes();
        uint32_t hashSvr = this->clavesCliente.encriptar_privado(huellaSvr);
        hashSvr = this->clavesServidor.encriptar_publico(hashSvr);
        uint32_t hashCalculado = certif.hashear();

        this->imprimir_huellas_hashes(huellaSvr, hashSvr, hashCalculado);

        if (hashSvr != hashCalculado){
            proto.enviar_bytes(1,1);
            std::string err = "Error: los hashes no coinciden.";
            throw ClienteError(err);
        }
        proto.enviar_bytes(0,1);
        certif.guardar();
    } catch (OSError &error){
        std::string err = "Error en comando crear certificado.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*
PRE: Recibe 3 enteros sin signo de 4 bytes (uint32_t):
la huella del servidor, el hash del servidor, y el hash 
calculado.
POST: Imprime los valor recibidos de la forma:
"Huella del servidor: " <huellaSvr>
"Hash del servidor: " <hashSvr>
"Hash calculado " <hashCalculado>
*/
void ClienteComandoCrear::imprimir_huellas_hashes(uint32_t huellaSvr, 
uint32_t hashSvr, uint32_t hashCalculado) {
    std::cout << "Huella del servidor: ";
    std::cout << std::dec << huellaSvr << "\n";
    std::cout << "Hash del servidor: ";
    std::cout << std::dec << hashSvr << "\n";
    std::cout << "Hash calculado: ";
    std::cout << std::dec << hashCalculado << "\n";
}


/*Destruye el comando crear del cliente*/
ClienteComandoCrear::~ClienteComandoCrear(){}