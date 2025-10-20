#ifndef CANCION_H
#define CANCION_H

#include <string>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

class Cancion {
private:
    int id;
    string nombre;
    float duracion;
    string rutaBase;
    long tamano;
    int relacionados[2];

    // Variables para control de reproducción
    atomic<bool> reproduciendo;
    atomic<bool> pausado;
    thread hiloReproduccion;

    void simularReproduccion();

public:
    Cancion(int id, const string& nombre, float duracion,
            const string& rutaBase, long tamano, int r1, int r2);
    ~Cancion();

    // Métodos de reproducción
    void reproducir();
    void pausar();
    void detener();
    bool estaReproduciendo() const;
    bool estaPausado() const;

    void mostrarCancion(const string& membresia = "C") const;

    // Métodos de búsqueda estáticos
    static Cancion* buscarPorId(Cancion** canciones, int cantidad, int id);
    static const Cancion* buscarPorId(const Cancion** canciones, int cantidad, int id);
    static void mostrarCancionPorId(Cancion** canciones, int cantidad, int id, const string& membresia = "C");

    int getId() const;
    string getNombre() const;
    float getDuracion() const;
    string getRutaBase() const;
    long getTamano() const;
    int getRelacionado1() const;
    int getRelacionado2() const;
};

#endif // CANCION_H


