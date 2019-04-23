#include <iostream>
#include <ctime>
#include <cstdint>
#include <arpa/inet.h>
#include "client.h"
#include "common.h"
#define LARGO_TIEMPO_MAXIMO 20
#define DIAS_SUMAR_INICIO_CERTIFICADO 30

/*Inicializa el tiempo en el horario actual.*/
Tiempo::Tiempo(){
    time(&this->tiempo);
}

/*Destruye el tiempo*/
Tiempo::~Tiempo(){}

/*Devuelve true, si el año del tiempo es bisiesto.*/
bool Tiempo::es_bisiesto(){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    size_t anio = tiempoInfo->tm_year;
    return ( (anio % 4 == 0) && ( (! (anio % 100 == 0)) || (anio % 400 == 0) ) );
}
/*Devuelve la cantidad (size_t) de dias del mes del tiempo actual.*/
size_t Tiempo::cuantos_dias_mes(){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    size_t mes = tiempoInfo->tm_mon; 
    if (mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12){
        return 31;
    }
    if (mes==4 || mes==6 || mes==9 || mes==11){
        return 30;
    }
    // Es febrero: 2
    if (this->es_bisiesto()){
        return 29;
    }
    return 28;
}

/*
PRE: Recibe una cantidad (size_t) de dias a sumar.
POST: Suma la cantidad recibida al tiempo actual
*/
void Tiempo::sumar_dias(size_t dias){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    size_t diasMesMaximo = this->cuantos_dias_mes();
    size_t diasActuales = tiempoInfo->tm_mday;
    size_t sumaDias = diasActuales + dias;
    if (sumaDias > diasMesMaximo){
        tiempoInfo->tm_mday = sumaDias - diasMesMaximo;
        tiempoInfo->tm_mon += 1;
    } else {
        tiempoInfo->tm_mday = sumaDias;
    }
    if (tiempoInfo->tm_mon > 11){ //0 a 11 meses (12)
        tiempoInfo->tm_mon = 0;
        tiempoInfo->tm_year += 1;
    }
    this->tiempo = mktime(tiempoInfo);
}

/*
PRE: Recibe una referencia a una string (string &).
POST: Modifica el contenido de la string, 
reemplazandolo por una representacion del tiempo actual:
MM DD hh:mm:ss YYYY
*/
void Tiempo::representar(str &representacion){
    struct tm *tiempoInfo;
    tiempoInfo = gmtime(&this->tiempo);
    char tiempoBuff[LARGO_TIEMPO_MAXIMO]; //+1
    strftime(tiempoBuff, sizeof(tiempoBuff), "%b %d %T %Y", tiempoInfo);
    //representacion.erase(0, std::string::npos);
    representacion = tiempoBuff; 
}

/*
PRE: Recibe los nombres (std::string &) del host y
puerto al que se conectara. 
POST: Inicializa un cliente que pide crear y revocar
certificados.
*/
Cliente::Cliente(str &host, str &puerto){
    this->skt = new Socket();
    this->skt->conectar(host.data(), puerto.data());
}

/*Destruye al cliente.*/
Cliente::~Cliente(){
    delete this->skt;
}

/*
PRE: Recibe el nombre (string &) de un archivo que contengo 
informacion del certificado a crear : subject, fecha de inicio
fecha de finalizacion; y una referencia a un vector de strings 
(std::vector<std::string>).
POST: Carga el la informacion del archivo en el vector de 
recibido.
Devuelve true si logro lo anterior, false en caso contrario.
*/
bool Cliente::cargar_info(str &nombreArch, strVec &info){
    std::string contenido = 0;
    bool todoOK = cargarArchivo(nombreArch, contenido);
    if (! todoOK){
        return false;
    }
    std::string separador = "\n";
    split(contenido, separador, info);
    if (info.size() < 1){
        //Archivo invalido
        return false;
    }
    if (info.size() < 2){
        //Suponemos que solo puede pasar esto, y no que puede estar la 
        //fecha de inicio, y no la de final
        Tiempo tiempoActual(tiempoActual);
        std::string fechaTemp = 0;
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
        tiempoActual.sumar_dias(30);
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
    }
    return true;

}

/*
PRE: Recibe un certificado (Certificado &) con la 
informacion necesario para solicitar su creacion a
una autoridad certificante.
POST: Envia los parametros necesarios, en el orden 
correcto, al servidor para crear un certificado.
*/
bool Cliente::solicitar_creacion(Certificado &certif){
    Protocolo proto(*(this->skt));
    //Enviamos el comando de creacion
    bool todoOK = true;
    todoOK = proto.enviar_bytes(0,1);
    if (!todoOK) {
        return false;
    }
    return certif.enviar_parametros(proto);
}

/*
PRE: Recibe las claves del cliente y el servidor (ClaveRSA &).
POST: Devuelve true si logro recibir y procesar las respuesta del servidor
a la creacion de un certificado (comprueba hashes y guarda certificado); 
devuelve false en caso de algun error ajeno los esperados.
*/
bool Cliente::recibir_certif(ClaveRSA &clvClnt, ClaveRSA &clvSvr){
    bool todoOK = true;

    //¿el esta sujeto vigente?
    Protocolo proto(*(this->skt));
    uint8_t vigencia = 0;
    todoOK = proto.recibir_un_byte(vigencia);
    if (! todoOK){
        return false;
    }
    if (vigencia == 0){
        //El subject ya esta vigente
        std::cout << "Error: ya existe un certificado.\n";
        return true; 
    }
    Certificado certif;
    todoOK = certif.recibir(proto);
    if (! todoOK){
        return false;
    }
    uint16_t hashCalculado = certif.hashear();

    uint32_t huellaServidor = 0;
    todoOK = proto.recibir_cuatro_bytes(huellaServidor);
    if (! todoOK){
        return false;
    }
    uint32_t hashServidor = clvClnt.encriptar_privado(huellaServidor);
    hashServidor = clvSvr.encriptar_publico(hashServidor);

    //Imprimimos lo que haga la huella y los hashes
    std::cout << "Huella del servidor: " << huellaServidor << "\n";
    std::cout << "Hash del servidor: " << hashServidor << "\n";
    std::cout << "Hash calculado: " << hashCalculado << "\n";

    //Comprobamos que el hash recibido sea el calculado
    if (hashServidor != hashCalculado){
        std::cout << "Error: los hashes no coinciden.\n";
        todoOK = proto.enviar_bytes(1,1);
        return todoOK;
    }

    //todo esto en metodo aparte
    //-------------------------------------------------------

    todoOK = certif.guardar();
    if (! todoOK){
        return false;
    }
    todoOK = proto.enviar_bytes(0,1);
    return todoOK;
}

/*
PRE: Recibe el nombre (std::string &) del archivo donde estan 
las claves del cliente , el nombre (std::string &) del archivo 
donde estan las claves publicas del servidor de la autoridad 
certificante, y el nombre (std::string &) del archivo donde 
esta la informacion para crear el certificado.
POST: Devuelve true, si logro crear un certificado 
correctamente, false en caso contrario.
*/
bool Cliente::crear_certif(str &ifCert, str &clvsClnt, str &pblSvr){
    bool todoOK = true;
    strVec infoVec(0);
    todoOK = this->cargar_info(ifCert, infoVec);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveCliente(0,0,0);
    todoOK = claveCliente.cargar_claves(clvsClnt);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveServidor(0,0,0);
    todoOK = claveServidor.cargar_claves(pblSvr);
    if (! todoOK){
        return false;
    }
    Certificado certif(claveCliente, infoVec);

    todoOK = this->solicitar_creacion(certif);
    if (! todoOK){
        return false;
    }
    todoOK = this->recibir_certif(claveCliente, claveServidor);
    return todoOK;
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
bool Cliente::revocar_certif(str &nmbrCrtf, str &nmbrClvClnt, str &nmbrClvSvr){
    bool todoOK = true;
    Certificado certif;
    todoOK = certif.cargar(nmbrCrtf); 
    if (! todoOK){
        return false;
    }
    ClaveRSA claveCliente(0,0,0);
    todoOK = claveCliente.cargar_claves(nmbrClvClnt);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveServidor(0,0,0);
    todoOK = claveServidor.cargar_claves(nmbrClvSvr);
    if (! todoOK){
        return false;
    }
    Protocolo proto(*(this->skt)); 
    // Quiero revocar
    todoOK = proto.enviar_bytes(1,1);
    if (! todoOK){
        return false;
    }
    //Envio certificado
    todoOK = certif.enviar(proto);
    if (! todoOK){
        return false;
    }
    //Calculo huella
    uint32_t hashCertif = certif.hashear();
    uint32_t hashEncripPriv = claveCliente.encriptar_publico(hashCertif);
    uint32_t huella = claveServidor.encriptar_privado(hashEncripPriv); 
    todoOK = proto.enviar_bytes(huella, 4);
    if (! todoOK){
        return false;
    }
    std::cout << "Hash calculado: " << hashCertif << "\n";
    std::cout << "Hash encriptado con la clave privada: ";
    std::cout << hashEncripPriv << "\n";
    std::cout << "Huella enviada: " << huella << "\n";
    uint8_t retorno;
    todoOK = proto.recibir_un_byte(retorno);
    if (! todoOK){
        return false;
    }
    if (retorno == 1){
        std::cout << "Error: usuario no registrado.\n";
        return true;
    } else if (retorno == 2){
        std::cout << "Error: los hashes no coinciden.\n";
        return true;
    } else if (retorno == 0){
        return true;
    }
    return false;
}

int main(int argc, const char* argv[]){
    if (argc != 7){
        std::cout << "Error: argumentos invalidos.\n";
    }
    str host = argv[1];
    str puerto = argv[2];
    Cliente cliente(host, puerto);
    
    str comando = argv[3];
    str nombreCertif = argv[4];
    str nombreClvClnt = argv[5];
    str nombreClvSvr = argv[6];

    if (comando == "new"){
        //./client localhost 2000 new certificate.req client.keys serverPublic.keys
        cliente.crear_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
    } else if (comando == "revoke"){
        //./client localhost 2000 revoke “Federico Manuel Gomez Peter.cert” client.keys serverPublic.keys
        cliente.revocar_certif(nombreCertif, nombreClvClnt, nombreClvSvr);
    } else {
        std::cout << "Error: argumentos invalidos.\n";   
    }
    return 0;
}