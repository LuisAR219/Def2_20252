#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "cancion.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#define MAX_BLOQUES 10
#define TAM_BLOQUE 1000

class Playlist {
private:
    Cancion** bloques[MAX_BLOQUES];
    int cancionesPorBloque[MAX_BLOQUES];
    Playlist* seguida;
    long iteraciones;

    long memoriaUsada;
    int idUsuario;

public:
    Playlist();
    void asignarIdUsuario(int id);
    int obtenerIdUsuario() const;

    void agregarCancion(Cancion* c);
    void seguirPlaylist(Playlist* otra);
    void mostrarPlaylist(bool desdeSeguido = false);

    Cancion* buscarCancionPorID(int id) const;

    void guardarEnArchivo(const string& carpetaDestino);
    void cargarDesdeArchivoPorId(int id, const string& carpetaOrigen);

    long obtenerIteracionesTotales() const { return iteraciones; }
    long obtenerMemoriaUsadaTotal() const { return memoriaUsada; }
};

#endif


