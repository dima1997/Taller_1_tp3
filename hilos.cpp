/*
Crea un mapa bloqueante ordenado alfabeticamente, 
cuyas claves son los sujetos de certificados creados 
y sus valores asociados son ClaveRSA publicas de dichos
sujetos
*/
MapaBloq::MapaBloq(){}

/*Destruye un mapa bloqueante*/
MapaBloq::~MapaBloq(){}

/*
PRE: Recibe un vector de informacion
donde cada elemento es un string de la forma
<sujeto>; <clave publica>
POST: el mapa con dichos valores.  
*/
//void cargar(std::vector<std::string> &info);
    
/*
Guardar el mapa bloqueante en forma de:
<sujeto>; <clave publica>, al final del 
archivo recibido.
*/
//void guardar(std::ostream &out);

/*
PRE: Recibe un clave (std::string), y una clave publica
(ClaveRSA &).
POST: Devuelve true si la tupla clave,valor recibida no 
estaba y se agrego al mapa; o false si ya habia un elemento
con dicha clave, y por ende no se agrego.
*/
bool agregarSiNoEsta(std::string &clave, &ClaveRSA valor){
    std::lock_guard<std::mutex>(this->centinela);
    if (this->mapa.find(clave) == this->mapa.end()){
        this->mapa[clave] = valor;
        return true;
    }
    return false;
}

/*
PRE: Recibe una clave (std::string) y un copia (ClaveRSA &)
POST: Devuelve true si la clave esta en el mapa y se si se 
copio a la referencia recibida, el valor asociado; o false
en caso de que no este.
*/
bool obtenerSiEsta(std::string &clave, ClaveRSA &copia){
    std::lock_guard<std::mutex>(this->centinela);
    if (this->mapa.find(clave) == this->mapa.end()){
        return false;
    }
    copia = this->mapa[clave]; //Aqui hago una copia
    return false; 
}

/*
PRE: Recibe una clave (std:string &).
POST: Borra la clave y su valor asociado si 
es que existen en el mapa.
*/
void borrar(std::string &clave){
    this->erase(clave);
}