#include "usuario.h"
#include "playlist.h"
#include "anuncio.h"
#include "album.h"
#include <random>
#include <chrono>
#include <iostream>

using namespace std;

const int REPRODUCCIONES_AUTOMATICAS = 10;


Usuario::Usuario()
    : inicioHistorial(0),
    cantidadHistorial(0),
    iteracionesHistorial(0),
    memoriaHistorial(0),
    premium(false),
    nickname(""),
    contrasena(""),
    pais(""),
    ciudad(""),
    fechaRegistro(""),
    usuarioSeguido("-") {
    for (int i = 0; i < 5; ++i)
        ultimas5[i] = nullptr;
}

Usuario::Usuario(const string& nick, const string& pass, bool esPremium,
                 const string& pais, const string& ciudad,
                 const string& fecha, const string& seguido)
    : inicioHistorial(0),
    cantidadHistorial(0),
    iteracionesHistorial(0),
    memoriaHistorial(0),
    nickname(nick),
    contrasena(pass),
    premium(esPremium),
    pais(pais),
    ciudad(ciudad),
    fechaRegistro(fecha),
    usuarioSeguido(seguido) {
    for (int i = 0; i < 5; ++i)
        ultimas5[i] = nullptr;

    // Si hay un usuario seguido, establecerlo en la playlist
    if (!usuarioSeguido.empty() && usuarioSeguido != "-") {
        // Esto necesitaría acceso a la playlist del usuario seguido
        // Depende de cómo tengas implementado el sistema de playlists
        cout << "DEBUG: Usuario " << nick << " sigue a " << seguido << endl;
    }
}

static long estimarMemoriaCancionPtrYObjeto() {
    return static_cast<long>(sizeof(Cancion*) + sizeof(Cancion));
}

bool Usuario::validarCredenciales(const string& nick, const string& pass) const {
    return (nick == nickname && pass == contrasena);
}

void Usuario::mostrarPerfil() const {
    cout << "\n---------- PERFIL DE USUARIO ----------" << endl;
    cout << "Nickname: " << nickname << endl;
    cout << "Pais: " << pais << endl;
    cout << "Ciudad: " << ciudad << endl;
    cout << "Fecha de registro: " << fechaRegistro << endl;
    cout << "Membresia: " << (premium ? "Premium" : "Estandar") << endl;
    if (usuarioSeguido != "-" && !usuarioSeguido.empty())
        cout << "Sigue al usuario: " << usuarioSeguido << endl;
    else
        cout << "No sigue a ningun usuario." << endl;
    cout << "----------------------------------------" << endl;
}

void Usuario::reproducirAnterior() {
    if (cantidadHistorial == 0) {
        cout << "No hay canciones en el historial." << endl;
        return;
    }

    cout << "\n=== REPRODUCIR CANCION ANTERIOR ===" << endl;

    for (int i = 0; i < cantidadHistorial && i < 5; i++) {
        Cancion* cancion = obtenerCancionAnterior(i);
        if (cancion != nullptr) {
            string calidad = (premium ? "_320.ogg" : "_128.ogg");
            cout << "Reproduciendo: " << cancion->getNombre()
                 << " | Artista: " << cancion->getArtista()  // CAMBIAR getAlbum() por getArtista()
                 << " | Ruta: " << cancion->getRutaBase()
                 << calidad << " (" << cancion->getDuracion() << " min)" << endl;

            // Simular reproduccion
            auto inicio = chrono::high_resolution_clock::now();
            while (true) {
                auto ahora = chrono::high_resolution_clock::now();
                auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
                if (segundos >= 3) break;
            }
        }
    }
}

// Modificar el método reproducirAleatorioDesdeBloques para incluir anuncios
void Usuario::reproducirAleatorioDesdeBloques(Playlist* lista) {
    if (lista == nullptr) {
        cout << "[reproducirAleatorio] Playlist nula." << endl;
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    long iter_local = 0;
    long mem_local = 0;
    int reproducidas = 0;
    int contadorParaAnuncio = 0;

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
        cout << "[reproducirAleatorio] No hay canciones en la playlist." << endl;
        return;
    }

    cout << "\n=== Reproduccion aleatoria iniciada ===" << endl;

    // CORRECCIÓN: Usar la constante correctamente
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
        cout << "[Bloque " << bloqueSel << "] Reproduciendo: "
             << actual->getNombre()
             << " | Artista: " << actual->getArtista()
             << " | Ruta: " << actual->getRutaBase()
             << calidad << " (" << actual->getDuracion() << " min)" << endl;

        registrarEnHistorial5(actual);

        // Simular reproduccion con temporizador
        auto inicio = chrono::high_resolution_clock::now();
        while (true) {
            auto ahora = chrono::high_resolution_clock::now();
            auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
            if (segundos >= 3) break;
        }
        ++iter_local;
        ++reproducidas;
        ++contadorParaAnuncio;
        mem_local += estimarMemoriaCancionPtrYObjeto();

        // Mostrar anuncio cada 2 canciones para usuarios no premium
        if (!premium && contadorParaAnuncio >= 2) {
            anuncio* anuncioActual = anuncio::obtenerAnuncioAleatorio();
            if (anuncioActual != nullptr) {
                cout << "\n--- ANUNCIO ---" << endl;
                cout << "Categoria: " << anuncioActual->getCategoria() << endl;
                cout << "Mensaje: " << anuncioActual->getMensaje() << endl;
                cout << "---------------" << endl;

                // Simular tiempo de anuncio
                auto inicioAnuncio = chrono::high_resolution_clock::now();
                while (true) {
                    auto ahoraAnuncio = chrono::high_resolution_clock::now();
                    auto segundosAnuncio = chrono::duration_cast<chrono::seconds>(ahoraAnuncio - inicioAnuncio).count();
                    if (segundosAnuncio >= 2) break;
                }
            }
            contadorParaAnuncio = 0;
        }
    }

    cout << "\n=== Reproduccion aleatoria finalizada ===" << endl;
    cout << "Canciones reproducidas: " << reproducidas << endl;
    cout << "Iteraciones locales: " << iter_local << endl;
    cout << "Memoria estimada: " << mem_local << " bytes" << endl;
}

void Usuario::reproducirAleatorioPlaylist(Playlist* playlist) {
    if (playlist == nullptr) {
        cout << "[reproducirAleatorioPlaylist] Playlist nula." << endl;
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    long iter_local = 0;
    long mem_local = 0;
    int reproducidas = 0;
    int contadorParaAnuncio = 0;

    // Obtener todas las canciones (propias + seguidas)
    vector<Cancion*> todasLasCanciones;

    // Agregar canciones propias
    for (int b = 0; b < MAX_BLOQUES; ++b) {
        int totalEnBloque = playlist->obtenerCancionesPorBloque(b);
        if (totalEnBloque <= 0) continue;

        Cancion** bloquePtr = playlist->obtenerBloque(b);
        if (bloquePtr == nullptr) continue;

        for (int i = 0; i < totalEnBloque; ++i) {
            if (bloquePtr[i] != nullptr) {
                todasLasCanciones.push_back(bloquePtr[i]);
            }
        }
        ++iter_local;
    }

    // Agregar canciones del usuario seguido si existe
    Playlist* playlistSeguida = playlist->obtenerPlaylistSeguida();
    if (playlistSeguida != nullptr) {
        for (int b = 0; b < MAX_BLOQUES; ++b) {
            int totalEnBloque = playlistSeguida->obtenerCancionesPorBloque(b);
            if (totalEnBloque <= 0) continue;

            Cancion** bloquePtr = playlistSeguida->obtenerBloque(b);
            if (bloquePtr == nullptr) continue;

            for (int i = 0; i < totalEnBloque; ++i) {
                if (bloquePtr[i] != nullptr) {
                    todasLasCanciones.push_back(bloquePtr[i]);
                }
            }
            ++iter_local;
        }
    }

    if (todasLasCanciones.empty()) {
        cout << "[reproducirAleatorioPlaylist] No hay canciones en la playlist." << endl;
        return;
    }

    cout << "\n=== Reproduccion aleatoria de playlist iniciada ===" << endl;
    cout << "Canciones disponibles: " << todasLasCanciones.size() << " (propias + seguidas)" << endl;

    std::uniform_int_distribution<int> dist(0, todasLasCanciones.size() - 1);

    while (reproducidas < K_REPRODUCCIONES_AUTOMATICAS && reproducidas < todasLasCanciones.size()) {
        Cancion* actual = todasLasCanciones[dist(gen)];
        if (actual == nullptr) continue;

        string calidad = (esPremium() ? "_320.ogg" : "_128.ogg");
        cout << "Reproduciendo: " << actual->getNombre()
             << " | Artista: " << actual->getArtista()
             << " | Ruta: " << actual->getRutaBase()
             << calidad << " (" << actual->getDuracion() << " min)" << endl;

        registrarEnHistorial5(actual);

        // Simular reproduccion con temporizador
        auto inicio = chrono::high_resolution_clock::now();
        while (true) {
            auto ahora = chrono::high_resolution_clock::now();
            auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
            if (segundos >= 3) break;
        }
        ++iter_local;
        ++reproducidas;
        ++contadorParaAnuncio;
        mem_local += estimarMemoriaCancionPtrYObjeto();

        // Mostrar anuncio cada 2 canciones para usuarios no premium
        if (!premium && contadorParaAnuncio >= 2) {
            anuncio* anuncioActual = anuncio::obtenerAnuncioAleatorio();
            if (anuncioActual != nullptr) {
                cout << "\n--- ANUNCIO ---" << endl;
                cout << "Categoria: " << anuncioActual->getCategoria() << endl;
                cout << "Mensaje: " << anuncioActual->getMensaje() << endl;
                cout << "---------------" << endl;

                // Simular tiempo de anuncio
                auto inicioAnuncio = chrono::high_resolution_clock::now();
                while (true) {
                    auto ahoraAnuncio = chrono::high_resolution_clock::now();
                    auto segundosAnuncio = chrono::duration_cast<chrono::seconds>(ahoraAnuncio - inicioAnuncio).count();
                    if (segundosAnuncio >= 2) break;
                }
            }
            contadorParaAnuncio = 0;
        }
    }

    cout << "\n=== Reproduccion aleatoria de playlist finalizada ===" << endl;
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

Cancion* Usuario::obtenerCancionAnterior(int posicion) {
    if (cantidadHistorial == 0 || posicion < 0 || posicion >= cantidadHistorial) {
        return nullptr;
    }

    int pos = (inicioHistorial + cantidadHistorial - 1 - posicion) % 5;
    if (pos < 0) pos += 5;

    return ultimas5[pos];
}

void Usuario::reproducirEnOrden(Playlist* lista) {
    if (lista == nullptr) {
        cout << "[reproducirEnOrden] Playlist nula." << endl;
        return;
    }

    long iter_local = 0;
    long mem_local = 0;
    int reproducidas = 0;
    int contadorParaAnuncio = 0;

    cout << "\n=== Reproduccion en orden iniciada ===" << endl;

    // Reproducir canciones propias primero
    for (int b = 0; b < MAX_BLOQUES; ++b) {
        int totalEnBloque = lista->obtenerCancionesPorBloque(b);
        if (totalEnBloque <= 0) continue;

        Cancion** bloquePtr = lista->obtenerBloque(b);
        if (bloquePtr == nullptr) continue;

        cout << "\n[ Bloque " << b << " - " << totalEnBloque << " canciones propias ]" << endl;

        for (int i = 0; i < totalEnBloque; ++i) {
            Cancion* actual = bloquePtr[i];
            if (actual == nullptr) continue;

            string calidad = (esPremium() ? "_320.ogg" : "_128.ogg");
            cout << " Reproduciendo: " << actual->getNombre()
                 << " | Artista: " << actual->getArtista()
                 << " | Ruta: " << actual->getRutaBase()
                 << calidad << " (" << actual->getDuracion() << " min)" << endl;

            registrarEnHistorial5(actual);

            // Simular reproduccion con temporizador
            auto inicio = chrono::high_resolution_clock::now();
            while (true) {
                auto ahora = chrono::high_resolution_clock::now();
                auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
                if (segundos >= 3) break;
            }
            ++iter_local;
            ++reproducidas;
            ++contadorParaAnuncio;
            mem_local += estimarMemoriaCancionPtrYObjeto();

            // Mostrar anuncio cada 2 canciones para usuarios no premium
            if (!premium && contadorParaAnuncio >= 2) {
                anuncio* anuncioActual = anuncio::obtenerAnuncioAleatorio();
                if (anuncioActual != nullptr) {
                    cout << "\n--- ANUNCIO ---" << endl;
                    cout << "Categoria: " << anuncioActual->getCategoria() << endl;
                    cout << "Mensaje: " << anuncioActual->getMensaje() << endl;
                    cout << "---------------" << endl;

                    // Simular tiempo de anuncio
                    auto inicioAnuncio = chrono::high_resolution_clock::now();
                    while (true) {
                        auto ahoraAnuncio = chrono::high_resolution_clock::now();
                        auto segundosAnuncio = chrono::duration_cast<chrono::seconds>(ahoraAnuncio - inicioAnuncio).count();
                        if (segundosAnuncio >= 2) break;
                    }
                }
                contadorParaAnuncio = 0;
            }
        }
    }

    // Reproducir canciones del usuario seguido si existe
    Playlist* playlistSeguida = lista->obtenerPlaylistSeguida();
    if (playlistSeguida != nullptr) {
        cout << "\n[ Canciones del usuario seguido ]" << endl;

        for (int b = 0; b < MAX_BLOQUES; ++b) {
            int totalEnBloque = playlistSeguida->obtenerCancionesPorBloque(b);
            if (totalEnBloque <= 0) continue;

            Cancion** bloquePtr = playlistSeguida->obtenerBloque(b);
            if (bloquePtr == nullptr) continue;

            for (int i = 0; i < totalEnBloque; ++i) {
                Cancion* actual = bloquePtr[i];
                if (actual == nullptr) continue;

                string calidad = (esPremium() ? "_320.ogg" : "_128.ogg");
                cout << " Reproduciendo [SEGUIDA]: " << actual->getNombre()
                     << " | Artista: " << actual->getArtista()
                     << " | Ruta: " << actual->getRutaBase()
                     << calidad << " (" << actual->getDuracion() << " min)" << endl;

                registrarEnHistorial5(actual);

                // Simular reproduccion con temporizador
                auto inicio = chrono::high_resolution_clock::now();
                while (true) {
                    auto ahora = chrono::high_resolution_clock::now();
                    auto segundos = chrono::duration_cast<chrono::seconds>(ahora - inicio).count();
                    if (segundos >= 3) break;
                }
                ++iter_local;
                ++reproducidas;
                ++contadorParaAnuncio;
                mem_local += estimarMemoriaCancionPtrYObjeto();

                // Mostrar anuncio cada 2 canciones para usuarios no premium
                if (!premium && contadorParaAnuncio >= 2) {
                    anuncio* anuncioActual = anuncio::obtenerAnuncioAleatorio();
                    if (anuncioActual != nullptr) {
                        cout << "\n--- ANUNCIO ---" << endl;
                        cout << "Categoria: " << anuncioActual->getCategoria() << endl;
                        cout << "Mensaje: " << anuncioActual->getMensaje() << endl;
                        cout << "---------------" << endl;

                        // Simular tiempo de anuncio
                        auto inicioAnuncio = chrono::high_resolution_clock::now();
                        while (true) {
                            auto ahoraAnuncio = chrono::high_resolution_clock::now();
                            auto segundosAnuncio = chrono::duration_cast<chrono::seconds>(ahoraAnuncio - inicioAnuncio).count();
                            if (segundosAnuncio >= 2) break;
                        }
                    }
                    contadorParaAnuncio = 0;
                }
            }
        }
    }

    cout << "\n=== Reproduccion en orden finalizada ===" << endl;
    cout << "Canciones reproducidas: " << reproducidas << endl;
    cout << "Iteraciones locales: " << iter_local << endl;
    cout << "Memoria estimada: " << mem_local << " bytes" << endl;
}


