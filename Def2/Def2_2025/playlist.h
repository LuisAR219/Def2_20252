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
    string nicknameUsuario;

public:
    Playlist();

    void asignarNicknameUsuario(const string& nickname);
    string obtenerNicknameUsuario() const;
    int obtenerTotalCanciones() const;
    Cancion** obtenerBloque(int bloque) const;
    int obtenerCancionesPorBloque(int bloque) const;
    bool tienePlaylistSeguida() const { return seguida != nullptr; }
    Playlist* obtenerPlaylistSeguida() const { return seguida; }

    void agregarCancion(Cancion* c);
    void seguirPlaylist(Playlist* otra);
    void mostrarPlaylist(bool desdeSeguido = false);

    Cancion* buscarCancionPorID(int id) const;

    void guardarEnArchivo(const string& carpetaDestino);
    void cargarDesdeArchivoPorNickname(const string& nickname, const string& carpetaOrigen);
    void crearDesdeCancionesCargadas();

    long obtenerIteracionesTotales() const { return iteraciones; }
    long obtenerMemoriaUsadaTotal() const { return memoriaUsada; }
};

#endif




