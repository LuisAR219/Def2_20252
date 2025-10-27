#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class Cancion {
private:
    int id;
    string nombre;
    string artista;  // NUEVO CAMPO
    float duracion;
    string rutaBase;
    long tamano;
    int relacionados[2];

public:
    // Constructor actualizado con artista
    Cancion(int id, const string& nombre, const string& artista, float duracion,
            const string& rutaBase, long tamano, int r1, int r2);

    int getId() const;
    string getNombre() const;
    string getArtista() const;  // NUEVO MÉTODO
    float getDuracion() const;
    string getRutaBase() const;
    long getTamano() const;
    int getRelacionado1() const;
    int getRelacionado2() const;

    void mostrarCancion(const std::string& membresia) const;
    bool operator==(const Cancion& otra) const;
};

#endif // CANCION_H


