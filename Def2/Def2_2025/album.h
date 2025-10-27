#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Album {
private:
    string nombre;
    int id;
    string generos[4];
    int totalGeneros;
    string fecha;
    string sello;
    string rutaPortada;
    int puntuacion;

    static Album* lista[100];
    static int cantidad;

    static long totalIteraciones;
    static size_t totalMemoria;

public:
    Album();
    Album(const string& nombre, int id, const string generos_[], int totalG,
          const string& fecha, const string& sello,
          const string& ruta, int puntuacion);
    ~Album();

    string getNombre() const;
    int getId() const;
    string getGenero(int i) const;
    int getTotalGeneros() const;
    string getFecha() const;
    string getSello() const;
    string getRutaPortada() const;
    int getPuntuacion() const;

    void mostrarAlbum() const;

    static bool cargarDesdeArchivo(const string& ruta);
    static void imprimirTodos();
    static int getCantidad();
    static void liberarMemoria();

    static void mostrarMetricas();
    static long getTotalIteraciones();
    static size_t getTotalMemoria();
};

#endif // ALBUM_H
