#include <fstream>
#include <iostream>
#include <string>
//#include "client_generador_certificados.h"
#include "common_certificado.h"
#include "common_error.h"
#include "common_archivable.h"
#include "common_archivo.h"

/*
PRE: Recibe el nombre del archivo a abrir 
y un modo de apertura:
r: modo lectura,
w: modo escritura
POST: Inicializa un archivo.
Levanta OSError en caso de error.
*/
Archivo::Archivo(const char *nombre, char modoApertura) 
: modoApertura(modoApertura) {
    if (modoApertura != 'r' && modoApertura != 'w'){
        std::string err = "Error: modo de apertura invalido.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    if (modoApertura == 'r'){
        this->archivo.open(nombre, std::ios::in);
    } else {
        this->archivo.open(nombre, std::ios::out);
    }
    if (! this->archivo.is_open()){
        std::string err = "Error al abrir archivo.";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}

/*Destruye un archivo*/
Archivo::~Archivo(){
    this->cerrar();
}

/*Cierra el archivo si esta abierto*/
void Archivo::cerrar(){
    if (this->archivo.is_open()){
        this->archivo.close();
    }
}

/*
Devuelve el flujo de entrada (istream &) 
de archivo abierto.
*/
std::istream& Archivo::getFlujoEntrada(){
    return this->archivo;
}

/*
Sobrecarga del operador >> en la clase Archivo.
PRE: Recibe una instancia de un archivo (Archivo &),
y una instancia de un objeto archivable (Archivable &).
POST: Carga el archivo recibido en el objeto archivable .
*/
Archivo& operator>>(Archivo &arch, Archivable &archivable){
    if (arch.modoApertura != 'r'){
        std::string err;
        err = "Error: operadoracion solo valida en modo lectura";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    archivable.cargar(arch.archivo);
    return arch;
}

/*
Sobrecarga del operador << en la clase Archivo.
PRE: Recibe una instancia de un archivo (Archivo &),
y una instancia de un objeto archivable (Archivable &).
POST: Guarda el objeto archivable en el archivo recibido.
*/
Archivo& operator<<(Archivo &arch, const Archivable &archivable){
    if (arch.modoApertura != 'w'){
        std::string err;
        err = "Error: operacion solo valida en modo escritura";
        throw OSError(__FILE__,__LINE__,err.data());
    }
    archivable.guardar(arch.archivo);
    return arch;
}

/*
PRE: Recibe el nombre del archivo que contenga 
informacion para crear un certicado: sujeto, fecha
de inicio y fecha de fin.
POST: Inicializa un archivo que carga certificados.
Levanta OSError en caso de error.
*/
ArchivoCertif::ArchivoCertif(const char *nombre)
: Archivo(nombre, 'r') {}

/*Destruye un archivo que carga certificados*/
ArchivoCertif::~ArchivoCertif(){}

/*
PRE: Recibe una referencia a un certificado.
POST: Carga el certificado.
*/
void ArchivoCertif::cargar_info(Certificado &certif){
    certif.cargar_info(this->archivo);
}

/*
PRE: Recibe el nombre del archivo con la informacion 
para crear la instancia de clase correspondiente.
POST: Inicializa un archivo factory.
*/
/*
ArchivoFactory::ArchivoFactory(std::string &nombreArchivo)
: Archivo(nombreArchivo) {}
*/
/*Destruye un archivo factory*/
//ArchivoFactory::~ArchivoFactory() {}

/*
Pre: Recibe la claves publicas (ClaveRSA &) de un cliente 
para generar un certificado.
POST: Devuelve un generador de certificados (GeneradorCertificados) 
ya incialziado. 
*/
/*
GeneradorCertificados ArchivoFactory::crear_generador_certificados(
ClaveRSA &clavesCliente){
    return std::move(GeneradorCertificados(this->archivo, clavesCliente));
}
*/