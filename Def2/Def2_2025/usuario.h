#ifndef USUARIO_H
#define USUARIO_H

#include "cancion.h"
#include "playlist.h"
#include <string>
#include <iostream>
using namespace std;

#ifndef MAX_BLOQUES
#define MAX_BLOQUES 10
#endif

#ifndef TAM_BLOQUE
#define TAM_BLOQUE 1000
#endif

#ifndef K_REPRODUCCIONES_AUTOMATICAS
#define K_REPRODUCCIONES_AUTOMATICAS 10
#endif

class Usuario {
private:
    // Datos personales
    string nickname;
    string contrasena;
    bool premium;
    string pais;
    string ciudad;
    string fechaRegistro;
    string usuarioSeguido;

    Cancion* ultimas5[5];
    int inicioHistorial;
    int cantidadHistorial;

    long iteracionesHistorial;
    long memoriaHistorial;

public:
    Usuario();
    Usuario(const string& nick, const string& pass, bool esPremium,
            const string& pais, const string& ciudad,
            const string& fecha, const string& seguido);
    string getNickname() const { return nickname; }
    string getContrasena() const { return contrasena; }
    bool esPremium() const { return premium; }
    string getPais() const { return pais; }
    string getCiudad() const { return ciudad; }
    string getFechaRegistro() const { return fechaRegistro; }
    string getUsuarioSeguido() const { return usuarioSeguido; }
    void setPremium(bool p) { premium = p; }

    void reproducirAleatorioDesdeBloques(Playlist* lista);
    void reproducirAleatorioPlaylist(Playlist* playlist);
    void reproducirEnOrden(Playlist* lista);
    void registrarEnHistorial5(Cancion* c);
    void mostrarHistorial5() const;
    Cancion* obtenerCancionAnterior(int posicion = 0);
    void reproducirAnterior();

    bool validarCredenciales(const string& nick, const string& pass) const;

    void mostrarPerfil() const;
};

#endif // USUARIO_H
