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

int anuncio::contadorAAA = 0;
int anuncio::contadorB = 0;
int anuncio::contadorC = 0;

anuncio::anuncio() : id(0), categoria(""), mensaje("") {
    totalMemoria += sizeof(anuncio);
}

anuncio::anuncio(int id, const string& categoria, const string& mensaje)
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
string anuncio::getCategoria() const { return categoria; }
string anuncio::getMensaje() const { return mensaje; }

void anuncio::setId(int id) { this->id = id; }

void anuncio::setCategoria(const string& categoria) {
    totalMemoria -= calcularMemoriaString(this->categoria);
    this->categoria = categoria;
    totalMemoria += calcularMemoriaString(this->categoria);
}

void anuncio::setMensaje(const string& mensaje) {
    totalMemoria -= calcularMemoriaString(this->mensaje);
    this->mensaje = mensaje;
    totalMemoria += calcularMemoriaString(this->mensaje);
}

size_t anuncio::calcularMemoriaString(const string& str) {
    return sizeof(string) + str.capacity() * sizeof(char);
}

size_t anuncio::calcularMemoriaAnuncio(const anuncio* anuncio) {
    if (!anuncio) return 0;
    return sizeof(anuncio) +
           calcularMemoriaString(anuncio->categoria) +
           calcularMemoriaString(anuncio->mensaje);
}

int anuncio::calcularPesoCategoria(const string& categoria) {
    if (categoria == "AAA") return 3;
    else if (categoria == "B") return 2;
    else return 1;
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
    if (!archivo.is_open()) return 0;

    int contador = 0;
    string linea;

    while (getline(archivo, linea)) {
        if (!linea.empty()) contador++;
    }

    archivo.close();
    return contador;
}

void anuncio::parsearLinea(const string& linea, int& id, string& categoria, string& mensaje) {
    size_t pos1 = 0;
    size_t pos2 = linea.find('|', pos1);
    if (pos2 != string::npos) {
        string idStr = linea.substr(pos1, pos2 - pos1);
        id = 0;
        for (char c : idStr) {
            if (c >= '0' && c <= '9') {
                id = id * 10 + (c - '0');
            }
        }

        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        if (pos2 != string::npos) {
            categoria = linea.substr(pos1, pos2 - pos1);
            pos1 = pos2 + 1;
            mensaje = linea.substr(pos1);
        }
    }
}

bool anuncio::cargarAnunciosDesdeTXT() {
    ifstream archivo("Anuncios.txt");
    if (!archivo.is_open()) return false;

    string linea;
    int contador = 0;

    contadorAAA = contadorB = contadorC = 0;

    int totalLineas = contarLineasArchivo();
    if (totalLineas == 0 || totalLineas > 50) {
        archivo.close();
        return false;
    }

    archivo.close();
    archivo.open("Anuncios.txt");

    while (getline(archivo, linea) && contador < 50) {
        if (linea.empty()) continue;

        int id;
        string categoria, mensaje;
        parsearLinea(linea, id, categoria, mensaje);

        anuncios[contador] = new anuncio(id, categoria, mensaje);
        contador++;

        if (categoria == "AAA") contadorAAA++;
        else if (categoria == "B") contadorB++;
        else if (categoria == "C") contadorC++;

        totalIteraciones += 3;
    }

    archivo.close();
    totalAnunciosCargados = contador;
    inicializarArregloUsados();
    anunciosCargados = true;
    return true;
}

anuncio* anuncio::obtenerAnuncioAleatorio() {
    if (!anunciosCargados || anunciosDisponibles == 0) return nullptr;

    int totalPesos = 0;
    for (int i = 0; i < totalAnunciosCargados; i++) {
        if (anuncios[i] != nullptr && !anunciosUsados[i])
            totalPesos += calcularPesoCategoria(anuncios[i]->getCategoria());
    }

    int numeroAleatorio = generarNumeroAleatorio(1, totalPesos);
    int acumulado = 0;

    for (int i = 0; i < totalAnunciosCargados; i++) {
        if (anuncios[i] != nullptr && !anunciosUsados[i]) {
            acumulado += calcularPesoCategoria(anuncios[i]->getCategoria());
            if (acumulado >= numeroAleatorio) {
                anunciosUsados[i] = true;
                anunciosDisponibles--;
                return anuncios[i];
            }
        }
    }

    return nullptr;
}

void anuncio::resetearAnuncios() {
    if (anunciosCargados) {
        inicializarArregloUsados();
        semillaAleatoria = (semillaAleatoria + 1) & 0x7fffffff;
    }
}

int anuncio::getAnunciosDisponibles() { return anunciosDisponibles; }
int anuncio::getTotalAnunciosCargados() { return totalAnunciosCargados; }

int anuncio::getContadorAAA() { return contadorAAA; }
int anuncio::getContadorB() { return contadorB; }
int anuncio::getContadorC() { return contadorC; }

void anuncio::mostrarMetricasFinales() {
    cout << "\n=== METRICAS DE RENDIMIENTO PARA ANUNCIOS ===" << endl;
    cout << "Total iteraciones acumuladas: " << totalIteraciones << endl;
    cout << "Total memoria acumulada: " << totalMemoria << " bytes" << endl;
    cout << "========================================" << endl;
}

int anuncio::getTotalIteraciones() { return totalIteraciones; }
size_t anuncio::getTotalMemoria() { return totalMemoria; }
