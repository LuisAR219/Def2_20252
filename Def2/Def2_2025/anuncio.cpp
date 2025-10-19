#include "anuncio.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

anuncio* anuncio::anuncios[50] = {nullptr};
bool anuncio::anunciosUsados[50] = {false};
int anuncio::anunciosDisponibles = 0;
bool anuncio::anunciosCargados = false;
int anuncio::totalAnunciosCargados = 0;
int anuncio::semillaAleatoria = 12345;

int anuncio::totalIteraciones = 0;
size_t anuncio::totalMemoria = 0;

anuncio::anuncio() : id(0), categoria(""), mensaje("") {
    totalMemoria += sizeof(anuncio);
}

anuncio::anuncio(int id, const std::string& categoria, const std::string& mensaje)
    : id(id), categoria(categoria), mensaje(mensaje) {
    totalMemoria += sizeof(anuncio);
    totalMemoria += calcularMemoriaString(categoria);
    totalMemoria += calcularMemoriaString(mensaje);
}

anuncio::~anuncio() {
    totalMemoria -= sizeof(anuncio);
    totalMemoria -= calcularMemoriaString(categoria);
    totalMemoria -= calcularMemoriaString(mensaje);
}

int anuncio::getId() const {
    return id;
}

std::string anuncio::getCategoria() const {
    return categoria;
}

std::string anuncio::getMensaje() const {
    return mensaje;
}

void anuncio::setId(int id) {
    this->id = id;
}

void anuncio::setCategoria(const std::string& categoria) {
    totalMemoria -= calcularMemoriaString(this->categoria);
    this->categoria = categoria;
    totalMemoria += calcularMemoriaString(this->categoria);
}

void anuncio::setMensaje(const std::string& mensaje) {
    totalMemoria -= calcularMemoriaString(this->mensaje);
    this->mensaje = mensaje;
    totalMemoria += calcularMemoriaString(this->mensaje);
}


