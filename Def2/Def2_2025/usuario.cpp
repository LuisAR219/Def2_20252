#include "usuario.h"
#include "playlist.h"
#include <random>
#include <chrono>
#include <iostream>

using namespace std;

long iteraciones_totales = 0;
long memoria_total = 0;

Usuario::Usuario()
    : inicioHistorial(0),
    cantidadHistorial(0),
    iteracionesHistorial(0),
    memoriaHistorial(0),
    premium(false) {
    for (int i = 0; i < 5; ++i) ultimas5[i] = nullptr;
}


static long estimarMemoriaCancionPtrYObjeto() {
    return static_cast<long>(sizeof(Cancion*) + sizeof(Cancion));
}

void Usuario::reproducirAleatorioDesdeBloques(Playlist* lista) {
    if (lista == nullptr) {
        cout << "[reproducirAleatorio] Playlist nula.\n";
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    long iter_local = 0;
    long mem_local = 0;
    int reproducidas = 0;

    int bloquesValidos[MAX_BLOQUES];
    int nValid = 0;
    for (int b = 0; b < MAX_BLOQUES; ++b) {
        int cnt = lista->obtenerCancionesPorBloque(b);
        if (cnt > 0 && lista->obtenerBloque(b) != nullptr) {
            bloquesValidos[nValid++] = b;
            ++iter_local;
        }
    }

    if (nValid == 0) {
        cout << "[reproducirAleatorio] No hay canciones en la playlist.\n";
        return;
    }

    cout << "\n=== Reproducción aleatoria iniciada ===\n";

    while (reproducidas < K_REPRODUCCIONES_AUTOMATICAS) {
        std::uniform_int_distribution<int> distBloque(0, nValid - 1);
        int bloqueSel = bloquesValidos[distBloque(gen)];
        ++iter_local;

        int totalEnBloque = lista->obtenerCancionesPorBloque(bloqueSel);
        if (totalEnBloque <= 0) continue;

        std::uniform_int_distribution<int> distIndex(0, totalEnBloque - 1);
        int indice = distIndex(gen);
        ++iter_local;

        Cancion** bloquePtr = lista->obtenerBloque(bloqueSel);
        if (bloquePtr == nullptr) continue;
        Cancion* actual = bloquePtr[indice];
        if (actual == nullptr) continue;

        string calidad = (esPremium() ? "_320.ogg" : "_128.ogg");
        cout << "[Bloque " << bloqueSel << "] → Reproduciendo: "
             << actual->getNombre()
             << " | Ruta: " << actual->getRutaBase()
             << calidad << " (" << actual->getDuracion() << " min)" << endl;

        registrarEnHistorial5(actual);

        auto inicio = chrono::high_resolution_clock::now();
        while (true) {
            auto ahora = chrono::high_resolution_clock::now();
            auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
            if (segundos >= 3) break;
        }
        ++iter_local;
        ++reproducidas;
        mem_local += estimarMemoriaCancionPtrYObjeto();
    }

    iteraciones_totales += iter_local;
    memoria_total += mem_local;

    cout << "\n=== Reproducción aleatoria finalizada ===" << endl;
    cout << "Canciones reproducidas: " << reproducidas << endl;
    cout << "Iteraciones locales: " << iter_local << endl;
    cout << "Memoria estimada: " << mem_local << " bytes" << endl;
}



void Usuario::registrarEnHistorial5(Cancion* c) {
    if (c == nullptr) return;
    iteracionesHistorial++;
    memoriaHistorial += static_cast<long>(sizeof(Cancion*));

    if (cantidadHistorial < 5) {
        int pos = (inicioHistorial + cantidadHistorial) % 5;
        ultimas5[pos] = c;
        cantidadHistorial++;
    } else {
        ultimas5[inicioHistorial] = c;
        inicioHistorial = (inicioHistorial + 1) % 5;
    }
}

void Usuario::mostrarHistorial5() const {
    cout << "\n[Ultimas " << (cantidadHistorial < 5 ? cantidadHistorial : 5) << " canciones]" << endl;
    int idx = inicioHistorial;
    for (int i = 0; i < cantidadHistorial; ++i) {
        Cancion* c = ultimas5[idx];
        if (c != nullptr) {
            string calidad = (premium ? "_320.ogg" : "_128.ogg");
            cout << (i + 1) << ". " << c->getRutaBase() << calidad
                 << " (" << c->getDuracion() << " min)" << endl;
        }
        idx = (idx + 1) % 5;
    }
    cout << "Iteraciones historial: " << iteracionesHistorial
         << " | Memoria estimada historial: " << memoriaHistorial << " bytes" << endl;
}

void Usuario::reproducirEnOrden(Playlist* lista) {
    if (lista == nullptr) {
        cout << "[reproducirEnOrden] Playlist nula.\n";
        return;
    }

    long iter_local = 0;
    long mem_local = 0;
    int reproducidas = 0;

    cout << "\n=== Reproducción en orden iniciada ===\n";

    for (int b = 0; b < MAX_BLOQUES; ++b) {
        int totalEnBloque = lista->obtenerCancionesPorBloque(b);
        if (totalEnBloque <= 0) continue;

        Cancion** bloquePtr = lista->obtenerBloque(b);
        if (bloquePtr == nullptr) continue;

        cout << "\n[ Bloque " << b << " - " << totalEnBloque << " canciones ]\n";

        for (int i = 0; i < totalEnBloque; ++i) {
            Cancion* actual = bloquePtr[i];
            if (actual == nullptr) continue;

            string calidad = (esPremium() ? "_320.ogg" : "_128.ogg");
            cout << "→ Reproduciendo: " << actual->getNombre()
                 << " | Ruta: " << actual->getRutaBase()
                 << calidad << " (" << actual->getDuracion() << " min)" << endl;

            registrarEnHistorial5(actual);

            auto inicio = chrono::high_resolution_clock::now();
            while (true) {
                auto ahora = chrono::high_resolution_clock::now();
                auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
                if (segundos >= 3) break;
            }
            ++iter_local;

            ++reproducidas;
            mem_local += estimarMemoriaCancionPtrYObjeto();
        }
    }

    iteraciones_totales += iter_local;
    memoria_total += mem_local;

    cout << "\n=== Reproducción en orden finalizada ===" << endl;
    cout << "Canciones reproducidas: " << reproducidas << endl;
    cout << "Iteraciones locales: " << iter_local << endl;
    cout << "Memoria estimada: " << mem_local << " bytes" << endl;
}

