#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <fstream>
#include "common_certificado.h"

class Archivo {
    /*
    Sobrecarga del operador >> en la clase Archivo.
    PRE: Recibe una instancia de un archivo (Archivo &),
    y una instancia de un objeto archivable (Archivable &).
    POST: Carga el archivo recibido en el objeto archivable.
    Levanta OSError en caso de error.
    */
    friend Archivo& operator>>(Archivo &arch, Archivable &archivable);

    /*
    Sobrecarga del operador << en la clase Archivo.
    PRE: Recibe una instancia de un archivo (Archivo &),
    y una instancia de un objeto archivable (Archivable &).
    POST: Guarda el objeto archivable en el archivo recibido.
    Levanta OSError en caso de error.
    */
    friend Archivo& operator<<(Archivo &arch, const Archivable &archivable);
    
    private:
    char modoApertura;

    public:
    std::fstream archivo;
    /*
    PRE: Recibe el nombre del archivo a abrir 
    y un modo de apertura:
    r: modo lectura,
    w: modo escritura
    POST: Inicializa un archivo.
    Levanta OSError en caso de error.
    */
    Archivo(const char *nombre, char modoApertura);

    /*Destruye un archivo*/
    virtual ~Archivo();

    /*Cierra el archivo si esta abierto*/
    void cerrar();
};

class ArchivoCertif : public Archivo{
    public:
    /*
    PRE: Recibe el nombre del archivo que contenga 
    informacion para crear un certicado: sujeto, fecha
    de inicio y fecha de fin.
    POST: Inicializa un archivo que carga certificados.
    Levanta OSError en caso de error.
    */
    explicit ArchivoCertif(const char *nombre);

    /*Destruye un archivo que carga certificados*/
    virtual ~ArchivoCertif();

    /*
    PRE: Recibe una referencia a un certificado (Certificado &).
    POST: Carga el certificado.
    */
    void cargar_info(Certificado &certif);
};


#endif // ARCHIVO_H
