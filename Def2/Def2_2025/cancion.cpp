#include "cancion.h"
#include <iostream>
#include <chrono>

using namespace std;

Cancion::Cancion(int id, const string& nombre, float duracion,
                 const string& rutaBase, long tamano, int r1, int r2)
    : id(id), nombre(nombre), duracion(duracion), rutaBase(rutaBase),
    tamano(tamano), reproduciendo(false), pausado(false) {
    relacionados[0] = r1;
    relacionados[1] = r2;
}

Cancion::~Cancion() {
    detener();
}

void Cancion::simularReproduccion() {
    float tiempoTranscurrido = 0.0f;
    const float intervaloCambio = 3.0f; // Cambio automático cada 3 segundos

    while (reproduciendo) {
        if (!pausado) {
            // Simular avance del tiempo de reproducción
            this_thread::sleep_for(chrono::seconds(1));
            tiempoTranscurrido += 1.0f;

            // Verificar si es tiempo de cambiar de canción (cada 3 segundos)
            if (tiempoTranscurrido >= intervaloCambio) {
                cout << "--- Cambio automático después de 3 segundos ---" << endl;
                break;
            }

            // Verificar si la canción ha terminado
            if (tiempoTranscurrido >= duracion * 60) {
                cout << "--- Canción finalizada ---" << endl;
                break;
            }
        } else {
            // Si está pausado, esperar brevemente
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    reproduciendo = false;
    pausado = false;
}

void Cancion::reproducir() {
    // Detener reproducción actual si hay una
    detener();

    reproduciendo = true;
    pausado = false;

    cout << "▶ Reproduciendo: " << nombre << endl;
    mostrarCancion();

    // Iniciar hilo de reproducción
    hiloReproduccion = thread(&Cancion::simularReproduccion, this);
}

void Cancion::pausar() {
    if (reproduciendo && !pausado) {
        pausado = true;
        cout << "⏸ Pausado: " << nombre << endl;
    } else if (reproduciendo && pausado) {
        pausado = false;
        cout << "▶ Reanudado: " << nombre << endl;
    }
}

void Cancion::detener() {
    if (reproduciendo) {
        reproduciendo = false;
        pausado = false;

        if (hiloReproduccion.joinable()) {
            hiloReproduccion.join();
        }

        cout << "⏹ Detenido: " << nombre << endl;
    }
}

bool Cancion::estaReproduciendo() const {
    return reproduciendo && !pausado;
}

bool Cancion::estaPausado() const {
    return reproduciendo && pausado;
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
    std::cout << "Duración: " << duracion << " min" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

// Métodos de búsqueda implementados
Cancion* Cancion::buscarPorId(Cancion** canciones, int cantidad, int id) {
    for (int i = 0; i < cantidad; i++) {
        if (canciones[i] != nullptr && canciones[i]->getId() == id) {
            return canciones[i];
        }
    }
    return nullptr;
}

const Cancion* Cancion::buscarPorId(const Cancion** canciones, int cantidad, int id) {
    for (int i = 0; i < cantidad; i++) {
        if (canciones[i] != nullptr && canciones[i]->getId() == id) {
            return canciones[i];
        }
    }
    return nullptr;
}

void Cancion::mostrarCancionPorId(Cancion** canciones, int cantidad, int id, const string& membresia) {
    Cancion* cancion = buscarPorId(canciones, cantidad, id);
    if (cancion != nullptr) {
        cout << "Canción encontrada:" << endl;
        cancion->mostrarCancion(membresia);
    } else {
        cout << "No se encontró ninguna canción con ID: " << id << endl;
    }
}



