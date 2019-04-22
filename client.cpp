#include <iostream>
#include <ctime>
#include <cstdint>
#include <arpa/inet.h>
#include "client.h"
#define LARGO_TIEMPO_MAXIMO 20
#define DIAS_SUMAR_INICIO_CERTIFICADO 30

typedef std::vector<std::string> strVec;
typedef std::string str;

/*Inicializa el tiempo en el horario actual.*/
Tiempo::Tiempo(){
    time_t tiempo;
    *(this->tiempo) = localtime(tiempo); 
}

/*Destruye el tiempo*/
Tiempo::~Tiempo(){}

/*Devuelve true, si el año del tiempo es bisiesto.*/
bool Tiempo::es_bisiesto(){
    size_t anio = this->tiempo-tm_year;
    return ( (anio % 4 == 0) && ( (! (anio % 100 == 0)) || (anio % 400 == 0) ) );
}
/*Devuelve la cantidad (size_t) de dias del mes del tiempo actual.*/
size_t Tiempo::cuantos_dias_mes(){
    size_t mes = this->tiempo.tm_month; 
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
    size_t diasMesMaximo = this->cuantos_dias_mes();
    size_t diasActuales = this->tiempo.tm_days;
    size_t sumaDias = diasActuales + dias;
    if (sumaDias > diasMesMaximo){
        this->tiempo.tm_days = sumaDias - diasMesMaximo;
        this->tiempo.tm_month += 1;
    } else {
        this->tiempo.tm_days = sumaDias;
    }
    if (this->tiempo.tm_month > 11){ //0 a 11 meses (12)
        this->tiempo.tm_month = 0;
        this->tiempo.tm_year += 1;
    }
}

/*
PRE: Recibe una referencia a una string (string &).
POST: Modifica el contenido de la string, 
reemplazandolo por una representacion del tiempo actual:
MM DD hh:mm:ss YYYY
*/
void Tiempo::representar(std::string &representacion){
    char tiempoBuff[LARGO_TIEMPO_MAXIMO]; //+1
    strftime(tiempoBuff, sizeof(tiempoActual), "%b %d %T %Y", this->tiempo);
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
bool Cliente::cargar_info(std::string &arch, strVec &info){
    std::string contenido = 0;
    bool todoOK = cargarArchivo(nombreArch, contenido);
    if (! todoOK){
        return false;
    }
    split(contenido, "\n", info);
    if (info.size() < 1){
        //Archivo invalido
        return false;
    }
    if (info.size() < 2){
        //Suponemos que solo puede pasar esto, y no que puede estar la 
        //fecha de inicio, y no la de final
        Tiempo tiempoActual(tiempoActual);
        string fechaTemp = 0;
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
        tiempoActual.sumar_dias(30);
        tiempoActual.representar(fechaTemp);
        info.push_back(fechaTemp);
    }
    return true;

}

/*
PRE: Recibe un vector de strings con la informacion
para crear el certificado (subject, fecha de inicio, 
fecha de finalizacion) (stringVector &); y una clave
rsa (ClaveRSA &).
POST: Envia los parametros necesarios, en el orden 
correcto, al servidor para crear un certificado.
*/
bool Cliente::solicitar_creacion(Certificado &certif){
    Protocolo proto(this->skt);
    //Enviamos el comando de creacion
    todoOK = proto.enviar_bytes(0,1);
    if (!todoOK) {
        return false;
    }
    return certif.enviar_parametros(this->skt);
}

/*
PRE: Recibe las claves del cliente y el servidor (ClaveRSA &), y el subject
(std::string &) del certificado usado para crearlo.
POST: Devuelve true si logro recibir y procesar las respuesta del servidor
a la creacion de un certificado (comprueba hashes y guarda certificado); 
devuelve false en caso de algun error ajeno los esperados.
*/
bool Cliente::recibir_certificado(ClaveRSA &clvClt, ClaveRSA &clvSvr){
    bool todoOK = true;

    //¿el esta sujeto vigente?
    Protocolo proto(this->skt);
    uint8_t vigencia = 0;
    todoOK = proto.recibir_un_byte(vigencia);
    if (! todoOK){
        return false;
    }
    if (vigencia == 0){
        //El subject ya esta vigente 
        std::cout << ​ "Error: ya existe un certificado.\n";
        return true; 
    }
    Certificado certif();
    todoOK = certif.recibir(this->skt);
    if (! todoOK){
        return false;
    }

    uint32_t huellaServidor = 0;
    todoOK = proto.recibir_cuatro_bytes(huellaServidor);
    if (! todoOK){
        return false;
    }

    uint16_t hashCalculado = certif.hashear();

    Encriptador encrip;
    
    //Desencriptamos primero con la privada del cliente.
    uint8_t expPrivCliente = clvClnt->expPrivado;
    uint16_t modCliente = clvClnt->modulo;
    uint32_t hashServidor = encrip.encriptar(huellaServidor, expPrivCliente, modCliente);
    
    //Luego desencriptamos con la publica del servidor.
    uint8_t expPubSvr = clvSvr->expPublico;
    uint16_t modSvr = clvSvr->modulo;
    hashServidor = encrip.encriptar(hashServidor, expPubSvr, modSvr);

    this->procesar_huellas(huellaServidor, hashServidor, hashCalculado);

    //Imprimimos lo que haga la huella y los hashes

    std::cout << "Huella del servidor: " << huellaServidor << "\n";
    std::cout << "Hash del servidor: " << hashServidor << "\n";
    std::cout << "Hash calculado: " << hashCalculado << "\n";

    //Comprobamos que el hash recibido sea el calculado

    if (hashServidor != hashCalculado){
        std::cout << "Error: los hashes no coinciden.\n"
        todoOK = proto.enviar_bytes(1,1);
        return todoOK;
    }

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
bool Cliente::crear_certificado(str &ifCert, str &clvsClnt, str &pblSvr){
    bool todoOK = true;
    stringVector infoVec(0);
    todoOK = this->cargar_info(infoCertif, infoVec);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveCliente(0,0,0);
    todoOK = claveCliente.cargar_claves(clvsClnt);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveServidor(0,0,0);
    todoOK = claveServidor.cargar_claves(publSvr);
    if (! todoOK){
        return false;
    }
    Certificado certif(clvsClnt, infoVec);

    todoOK = this->solicitar_creacion(certif);
    if (! todoOk){
        return false;
    }
    todoOK = this->recibir_certificado(claveCliente, claveServidor);
    return todoOK;
}

bool Cliente::revocar(str &nombreCertif, str &nombreClvClnt, str &nombreClvSvr){
    bool todoOK = true;
    Certificado certif(nombreCertif);
    todoOK = this->cargar_info(infoCertif, infoVec);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveCliente(0,0,0);
    todoOK = claveCliente.cargar_claves(clvsClnt);
    if (! todoOK){
        return false;
    }
    ClaveRSA claveServidor(0,0,0);
    todoOK = claveServidor.cargar_claves(publSvr);
    if (! todoOK){
        return false;
    }
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
        cliente.crear_certificado(nombreCertif, nombreClvClnt, nombreClvSvr);
    } else if (comando == "revoke"){
        //./client localhost 2000 revoke “Federico Manuel Gomez Peter.cert” client.keys serverPublic.keys
        cliente.revocar_certificado(nombreCertif, nombreClvClnt, nombreClvSvr);
    } else {
        std::cout << "Error: argumentos invalidos.\n";   
    }
    return 0;
}