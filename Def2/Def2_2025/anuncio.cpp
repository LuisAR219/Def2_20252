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

int anuncio::totalAAA = 0;
int anuncio::totalB = 0;
int anuncio::totalC = 0;

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

int anuncio::getId() const { return id; }
std::string anuncio::getCategoria() const { return categoria; }
std::string anuncio::getMensaje() const { return mensaje; }

void anuncio::setId(int id) { this->id = id; }

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

size_t anuncio::calcularMemoriaString(const std::string& str) {
    return sizeof(string) + str.capacity() * sizeof(char);
}

size_t anuncio::calcularMemoriaAnuncio(const anuncio* anuncio) {
    if (!anuncio) return 0;
    return sizeof(anuncio) +
           calcularMemoriaString(anuncio->categoria) +
           calcularMemoriaString(anuncio->mensaje);
}

void anuncio::inicializarArregloUsados() {
    totalIteraciones++;
    for (int i = 0; i < 50; i++) {
        totalIteraciones++;
        anunciosUsados[i] = false;
    }
    anunciosDisponibles = totalAnunciosCargados;
}

int anuncio::generarNumeroAleatorio(int min, int max) {
    totalIteraciones += 2;
    semillaAleatoria = (semillaAleatoria * 1103515245 + 12345) & 0x7fffffff;
    return min + (semillaAleatoria % (max - min + 1));
}

int anuncio::contarLineasArchivo() {
    ifstream archivo("Anuncios.txt");
    totalMemoria += sizeof(ifstream);
    totalIteraciones++;

    if (!archivo.is_open()) {
        totalMemoria -= sizeof(ifstream);
        return 0;
    }

    int contador = 0;
    string linea;
    totalMemoria += sizeof(int) + sizeof(string);

    while (getline(archivo, linea)) {
        totalIteraciones++;
        if (!linea.empty()) {
            contador++;
            totalIteraciones++;
        }
    }

    archivo.close();
    totalMemoria -= sizeof(ifstream);
    return contador;
}






