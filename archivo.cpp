Archivo::Archivo(char modoApertura) : modoApertura(modoApertura) {
    this->archivo.open(modoApertura);
    if (! this->archivo.is_open()){
        std::string err = "Error al abrir";
        throw OSError(__FILE__,__LINE__,err.data());
    }
}
Archivo::~Archivo(){}
Archivo& operator>>(Archivo &arch, Archivable &archivable){
    if (this->modoApertura != 'r'){
        std::string err;
        err = "Error: operadoracion no valida en este modo lectura";
        throw OSError(err.data());
    }
    archivable.cargar(arch.archivo);
    return arch;
}

Archivo& operator<<(Archivo &arch, Archivable &){
    //cosas
}