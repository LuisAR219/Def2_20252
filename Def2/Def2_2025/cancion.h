#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class Cancion {
private:
    int id;
    string nombre;
    float duracion;
    string rutaBase;
    long tamano;
    int relacionados[2];

public:
    Cancion(int id, const string& nombre, float duracion,
            const string& rutaBase, long tamano, int r1, int r2);

    void mostrarCancion(const string& membresia = "C") const;

    int getId() const;
    string getNombre() const;
    float getDuracion() const;
    string getRutaBase() const;
    long getTamano() const;
    int getRelacionado1() const;
    int getRelacionado2() const;
    bool operator==(const Cancion& otra) const;
};

#endif // CANCION_H


