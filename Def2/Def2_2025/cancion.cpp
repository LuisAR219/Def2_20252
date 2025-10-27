#include "cancion.h"
#include <iostream>

using namespace std;

Cancion::Cancion(int id, const string& nombre, const string& artista, float duracion,
                 const string& rutaBase, long tamano, int r1, int r2)
    : id(id), nombre(nombre), artista(artista), duracion(duracion),
    rutaBase(rutaBase), tamano(tamano) {
    relacionados[0] = r1;
    relacionados[1] = r2;
}

string Cancion::getArtista() const {
    return artista;
}

int Cancion::getId() const { return id; }
string Cancion::getNombre() const { return nombre; }
float Cancion::getDuracion() const { return duracion; }
string Cancion::getRutaBase() const { return rutaBase; }
long Cancion::getTamano() const { return tamano; }
int Cancion::getRelacionado1() const { return relacionados[0]; }
int Cancion::getRelacionado2() const { return relacionados[1]; }

void Cancion::mostrarCancion(const std::string& tipoCalidad) const {
    std::string calidad;

    if (tipoCalidad == "C" || tipoCalidad == "premium" || tipoCalidad == "PREMIUM" || tipoCalidad == "320")
        calidad = "_320.ogg";
    else
        calidad = "_128.ogg";

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Artista: " << artista << std::endl;
    std::cout << "Duración: " << duracion << " min" << std::endl;
    std::cout << "Ruta: " << rutaBase << calidad << std::endl;
    std::cout << "Tamaño: " << tamano << " bytes" << std::endl;
    std::cout << "Relacionados: " << relacionados[0] << ", " << relacionados[1] << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

bool Cancion::operator==(const Cancion& otra) const {
    return (id == otra.id) ||
           (nombre == otra.nombre &&
            artista == otra.artista &&
            duracion == otra.duracion &&
            rutaBase == otra.rutaBase &&
            tamano == otra.tamano);
}
