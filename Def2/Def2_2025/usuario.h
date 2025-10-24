#ifndef USUARIO_H
#define USUARIO_H

#include "cancion.h"
#include "playlist.h"
#include <string>
#include <iostream>
using namespace std;

// ─────────────────────────────────────────────
// Protegemos los defines para evitar conflictos
// ─────────────────────────────────────────────
#ifndef MAX_BLOQUES
#define MAX_BLOQUES 10
#endif

#ifndef TAM_BLOQUE
#define TAM_BLOQUE 1000
#endif

#ifndef K_REPRODUCCIONES_AUTOMATICAS
#define K_REPRODUCCIONES_AUTOMATICAS 10
#endif
// ─────────────────────────────────────────────

class Usuario {
private:
    Cancion* ultimas5[5];
    int inicioHistorial;
    int cantidadHistorial;

    long iteracionesHistorial;
    long memoriaHistorial;

    bool premium;

public:
    Usuario();

    void reproducirAleatorioDesdeBloques(Playlist* lista);
    void registrarEnHistorial5(Cancion* c);
    void mostrarHistorial5() const;

    void setPremium(bool p) { premium = p; }
    bool esPremium() const { return premium; }

    void reproducirEnOrden(Playlist* lista);
};

#endif // USUARIO_H

