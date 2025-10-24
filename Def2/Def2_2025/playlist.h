#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "cancion.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;



class Playlist {
private:
    static const int MAX_BLOQUES = 10;
    static const int TAM_BLOQUE = 1000;
    static const int K_REPRODUCCIONES_AUTOMATICAS = 10;
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
    void crearDesdeCancionesCargadas();

    long obtenerIteracionesTotales() const { return iteraciones; }
    long obtenerMemoriaUsadaTotal() const { return memoriaUsada; }

    Cancion** obtenerBloque(int idx) const {
        if (idx < 0 || idx >= MAX_BLOQUES) return nullptr;
        return bloques[idx];
    }

    int obtenerCancionesPorBloque(int idx) const {
        if (idx < 0 || idx >= MAX_BLOQUES) return 0;
        return cancionesPorBloque[idx];
    }
};

#endif



