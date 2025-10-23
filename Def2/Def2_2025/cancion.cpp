#include "cancion.h"
#include <iostream>

using namespace std;


Cancion::Cancion(int id, const string& nombre, float duracion,  const string& rutaBase, long tamano, int r1, int r2)
    : id(id), nombre(nombre), duracion(duracion), rutaBase(rutaBase), tamano(tamano) {
    relacionados[0] = r1;
    relacionados[1] = r2;
}

int Cancion::getId() const { return id; }
string Cancion::getNombre() const { return nombre; }
float Cancion::getDuracion() const { return duracion; }
string Cancion::getRutaBase() const { return rutaBase; }
long Cancion::getTamano() const { return tamano; }
int Cancion::getRelacionado1() const { return relacionados[0]; }
int Cancion::getRelacionado2() const { return relacionados[1]; }

void Cancion::mostrarCancion(const std::string& membresia) const {
    std::string calidad;

    if (membresia == "premium" || membresia == "PREMIUM")
        calidad = "_320.ogg";
    else
        calidad = "_128.ogg";

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Ruta al archivo de audio: " << rutaBase << calidad << std::endl;
    std::cout << "Duracion: " << duracion << " min" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

bool Cancion::operator==(const Cancion& otra) const {
    return (id == otra.id) ||
           (nombre == otra.nombre &&
            duracion == otra.duracion &&
            rutaBase == otra.rutaBase &&
            tamano == otra.tamano);
}
