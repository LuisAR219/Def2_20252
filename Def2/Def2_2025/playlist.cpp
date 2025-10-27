#include "playlist.h"
#include "cancion.h"
#include <filesystem>
using namespace std;

extern bool cargarCancionesDesdeTXT(const string& nombreArchivo);
extern Cancion* canciones[10][1000];
extern int cancionesPorBloque[10];
extern int bloquesUsados;

Playlist::Playlist() {
    for (int i = 0; i < MAX_BLOQUES; i++) {
        bloques[i] = nullptr;
        cancionesPorBloque[i] = 0;
    }
    seguida = nullptr;
    iteraciones = 0;
    memoriaUsada = sizeof(Playlist);
    nicknameUsuario = "";
}

void Playlist::asignarNicknameUsuario(const string& nickname) {
    nicknameUsuario = nickname;
}

string Playlist::obtenerNicknameUsuario() const {
    return nicknameUsuario;
}

void Playlist::crearDesdeCancionesCargadas() {
    // Limpiar la playlist actual primero
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                if (bloques[b][i] != nullptr) {
                    delete bloques[b][i];
                    bloques[b][i] = nullptr;
                }
            }
            delete[] bloques[b];
            bloques[b] = nullptr;
            cancionesPorBloque[b] = 0;
        }
    }

    // CORRECCIÓN: Usar las variables globales de canciones del sistema
    // Cargar todas las canciones disponibles desde las canciones globales
    for (int b = 0; b <= ::bloquesUsados; b++) {
        for (int i = 0; i < ::cancionesPorBloque[b]; i++) {
            if (::canciones[b][i] != nullptr) {
                // Crear una nueva canción con los mismos datos
                Cancion* original = ::canciones[b][i];
                Cancion* nueva = new Cancion(
                    original->getId(),
                    original->getNombre(),
                    original->getArtista(),
                    original->getDuracion(),
                    original->getRutaBase(),
                    original->getTamano(),
                    original->getRelacionado1(),
                    original->getRelacionado2()
                    );
                agregarCancion(nueva);
            }
        }
    }

    cout << "Playlist creada con " << obtenerTotalCanciones() << " canciones cargadas.\n";
}

Cancion** Playlist::obtenerBloque(int bloque) const {
    if (bloque >= 0 && bloque < MAX_BLOQUES) {
        return bloques[bloque];
    }
    return nullptr;
}

int Playlist::obtenerCancionesPorBloque(int bloque) const {
    if (bloque >= 0 && bloque < MAX_BLOQUES) {
        return cancionesPorBloque[bloque];
    }
    return 0;
}

// También necesitamos agregar el método obtenerTotalCanciones() si no existe
int Playlist::obtenerTotalCanciones() const {
    int total = 0;
    for (int b = 0; b < MAX_BLOQUES; b++) {
        total += cancionesPorBloque[b];
    }
    return total;
}

void Playlist::agregarCancion(Cancion* c) {
    if (c == nullptr) return;

    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                if (bloques[b][i] != nullptr && *(bloques[b][i]) == *c) {
                    cout << "La canción ya existe en la playlist.\n";
                    return;
                }
            }
        }
    }

    iteraciones++;
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] == nullptr) {
            bloques[b] = new Cancion*[TAM_BLOQUE];
            memoriaUsada += sizeof(Cancion*) * TAM_BLOQUE;
            for (int j = 0; j < TAM_BLOQUE; j++) bloques[b][j] = nullptr;
        }

        if (cancionesPorBloque[b] < TAM_BLOQUE) {
            bloques[b][cancionesPorBloque[b]] = c;
            cancionesPorBloque[b]++;
            return;
        }
    }

    cout << "Playlist llena. No se pudo agregar la canción.\n";
}

void Playlist::seguirPlaylist(Playlist* otra) {
    if (otra == nullptr) {
        if (seguida != nullptr) {
            cout << "Has dejado de seguir la playlist de " << seguida->obtenerNicknameUsuario() << ".\n";
            seguida = nullptr;
        } else {
            cout << "No estabas siguiendo ninguna playlist.\n";
        }
        return;
    }
    seguida = otra;
    cout << "Ahora sigues la playlist de " << otra->obtenerNicknameUsuario() << ".\n";
}


void Playlist::mostrarPlaylist(bool desdeSeguido) {
    iteraciones++;

    // Si es una llamada desde un usuario seguido, mostrar solo sus canciones
    if (desdeSeguido) {
        cout << "\n--- Canciones de " << nicknameUsuario << " ---\n";
        for (int b = 0; b < MAX_BLOQUES; b++) {
            if (bloques[b] != nullptr) {
                for (int i = 0; i < cancionesPorBloque[b]; i++) {
                    Cancion* c = bloques[b][i];
                    if (c != nullptr) {
                        cout << "[SEGUIDA] ";
                        c->mostrarCancion("C");
                    }
                }
            }
        }
        return;
    }

    // Mostrar playlist normal (usuario actual + seguido)
    cout << "\n======= Playlist de " << nicknameUsuario << " =======\n";

    // Mostrar canciones propias
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* c = bloques[b][i];
                if (c != nullptr) {
                    cout << "[PROPIA] ";
                    c->mostrarCancion("C");
                }
            }
        }
    }

    // Mostrar canciones del usuario seguido
    if (seguida != nullptr) {
        cout << "\n--- Canciones seguidas de " << seguida->obtenerNicknameUsuario() << " ---\n";
        seguida->mostrarPlaylist(true); // true indica que es una llamada desde seguido
    }
}

Cancion* Playlist::buscarCancionPorID(int id) const {
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* actual = bloques[b][i];
                if (actual != nullptr && actual->getId() == id) return actual;
            }
        }
    }
    return nullptr;
}

void Playlist::guardarEnArchivo(const string& carpetaDestino) {
    if (nicknameUsuario.empty()) {
        cout << "No se puede guardar: nickname de usuario no asignado.\n";
        return;
    }

    string ruta = carpetaDestino + "\\" + nicknameUsuario + ".txt";
    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar: " << ruta << endl;
        return;
    }

    iteraciones++;

    // Guardar canciones propias
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* c = bloques[b][i];
                if (c != nullptr) {
                    archivo << c->getId() << '|'
                            << c->getNombre() << '|'
                            << c->getArtista() << '|'
                            << c->getDuracion() << '|'
                            << c->getRutaBase() << '|'
                            << c->getTamano() << '|'
                            << c->getRelacionado1() << ','
                            << c->getRelacionado2() << '\n';
                }
            }
        }
    }

    archivo.close();
    cout << "Playlist de " << nicknameUsuario << " guardada correctamente en: " << ruta << endl;
}

void Playlist::cargarDesdeArchivoPorNickname(const string& nickname, const string& carpetaOrigen) {
    string ruta = carpetaOrigen + "\\" + nickname + ".txt";
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << ruta << endl;
        return;
    }

    nicknameUsuario = nickname;
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        iteraciones++;

        // Verificar si es una línea de usuario seguido
        if (linea.find("SEGUIDO|") == 0) {
            string usuarioSeguido = linea.substr(8); // Extraer después de "SEGUIDO|"
            // Aquí deberías cargar la playlist del usuario seguido
            // Por ahora solo guardamos la referencia
            cout << "DEBUG: Usuario seguido encontrado: " << usuarioSeguido << endl;

            // Cargar la playlist del usuario seguido
            Playlist* playlistSeguida = new Playlist();
            playlistSeguida->cargarDesdeArchivoPorNickname(usuarioSeguido, carpetaOrigen);
            seguirPlaylist(playlistSeguida);
            continue;
        }

        // Procesar línea de canción normal
        size_t pos = 0;
        int idCancion;
        string nombre, artista, rutaBase;
        float duracion;
        long tamano;
        int r1, r2;

        // Campo 1: ID
        pos = linea.find('|');
        idCancion = stoi(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        // Campo 2: Nombre
        pos = linea.find('|');
        nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        // Campo 3: Artista
        pos = linea.find('|');
        artista = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        // Campo 4: Duración
        pos = linea.find('|');
        duracion = stof(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        // Campo 5: Ruta
        pos = linea.find('|');
        rutaBase = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        // Campo 6: Tamaño
        pos = linea.find('|');
        tamano = stol(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        // Campo 7: Relacionados
        pos = linea.find(',');
        r1 = stoi(linea.substr(0, pos));
        r2 = stoi(linea.substr(pos + 1));

        Cancion* nueva = new Cancion(idCancion, nombre, artista, duracion, rutaBase, tamano, r1, r2);
        memoriaUsada += sizeof(Cancion);
        agregarCancion(nueva);
    }

    archivo.close();
    cout << "Playlist de " << nicknameUsuario << " cargada correctamente desde: " << ruta << endl;
}



