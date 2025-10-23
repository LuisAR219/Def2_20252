#include "playlist.h"
#include "cancion.h"
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
    idUsuario = -1;
}

void Playlist::asignarIdUsuario(int id) {
    idUsuario = id;
}

int Playlist::obtenerIdUsuario() const {
    return idUsuario;
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
        cout << "Playlist a seguir no válida.\n";
        return;
    }
    seguida = otra;
    cout << "Ahora sigues la playlist del usuario " << otra->obtenerIdUsuario() << ".\n";
}

void Playlist::mostrarPlaylist(bool desdeSeguido) {
    iteraciones++;

    int* idsVistos[MAX_BLOQUES];
    int cantidadIDs[MAX_BLOQUES];

    for (int i = 0; i < MAX_BLOQUES; i++) {
        idsVistos[i] = new int[TAM_BLOQUE];
        cantidadIDs[i] = 0;
        for (int j = 0; j < TAM_BLOQUE; j++) idsVistos[i][j] = -1;
        memoriaUsada += sizeof(int) * TAM_BLOQUE;
    }

    auto existeID = [&](int id) -> bool {
        for (int b = 0; b < MAX_BLOQUES; b++) {
            for (int i = 0; i < cantidadIDs[b]; i++) {
                if (idsVistos[b][i] == id) return true;
            }
        }
        return false;
    };

    auto registrarID = [&](int id) {
        for (int b = 0; b < MAX_BLOQUES; b++) {
            if (cantidadIDs[b] < TAM_BLOQUE) {
                idsVistos[b][cantidadIDs[b]] = id;
                cantidadIDs[b]++;
                return;
            }
        }
    };

    cout << "\n======= Playlist del usuario " << idUsuario << " =======\n";

    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* c = bloques[b][i];
                if (c != nullptr && !existeID(c->getId())) {
                    cout << "[PROPIA] ";
                    c->mostrarCancion("C");
                    registrarID(c->getId());
                }
            }
        }
    }

    if (seguida != nullptr && !desdeSeguido) {
        cout << "\n--- Canciones de la playlist seguida ("
             << seguida->obtenerIdUsuario() << ") ---\n";

        for (int b = 0; b < MAX_BLOQUES; b++) {
            if (seguida->bloques[b] != nullptr) {
                for (int i = 0; i < seguida->cancionesPorBloque[b]; i++) {
                    Cancion* c = seguida->bloques[b][i];
                    if (c != nullptr && !existeID(c->getId())) {
                        cout << "[SEGUIDA] ";
                        c->mostrarCancion("C");
                        registrarID(c->getId());
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_BLOQUES; i++) {
        delete[] idsVistos[i];
    }
}

Cancion* Playlist::buscarCancionPorID(int id) const {
    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* actual = bloques[b][i];
                if (actual != nullptr && actual->getId() == id) {
                    return actual;
                }
            }
        }
    }
    return nullptr;
}

void Playlist::guardarEnArchivo(const string& carpetaDestino) {
    if (idUsuario == -1) {
        cout << "No se puede guardar: ID de usuario no asignado.\n";
        return;
    }

    string ruta = carpetaDestino + "\\" + to_string(idUsuario) + ".txt";
    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar: " << ruta << endl;
        return;
    }

    iteraciones++;

    int* idsGuardados[MAX_BLOQUES];
    int cantidadIDs[MAX_BLOQUES];

    for (int i = 0; i < MAX_BLOQUES; i++) {
        idsGuardados[i] = new int[TAM_BLOQUE];
        cantidadIDs[i] = 0;
        for (int j = 0; j < TAM_BLOQUE; j++) idsGuardados[i][j] = -1;
        memoriaUsada += sizeof(int) * TAM_BLOQUE;
    }

    auto existeID = [&](int id) -> bool {
        for (int b = 0; b < MAX_BLOQUES; b++) {
            for (int i = 0; i < cantidadIDs[b]; i++) {
                if (idsGuardados[b][i] == id)
                    return true;
            }
        }
        return false;
    };

    auto registrarID = [&](int id) {
        for (int b = 0; b < MAX_BLOQUES; b++) {
            if (cantidadIDs[b] < TAM_BLOQUE) {
                idsGuardados[b][cantidadIDs[b]] = id;
                cantidadIDs[b]++;
                return;
            }
        }
    };

    for (int b = 0; b < MAX_BLOQUES; b++) {
        if (bloques[b] != nullptr) {
            for (int i = 0; i < cancionesPorBloque[b]; i++) {
                Cancion* c = bloques[b][i];
                if (c != nullptr) {
                    int id = c->getId();
                    if (!existeID(id)) {
                        archivo << c->getId() << '|'
                                << c->getNombre() << '|'
                                << c->getDuracion() << '|'
                                << c->getRutaBase() << '|'
                                << c->getTamano() << '|'
                                << c->getRelacionado1() << ','
                                << c->getRelacionado2() << '\n';
                        registrarID(id);
                    }
                }
            }
        }
    }

    if (seguida != nullptr) {
        for (int b = 0; b < MAX_BLOQUES; b++) {
            if (seguida->bloques[b] != nullptr) {
                for (int i = 0; i < seguida->cancionesPorBloque[b]; i++) {
                    Cancion* c = seguida->bloques[b][i];
                    if (c != nullptr && !existeID(c->getId())) {
                        archivo << c->getId() << '|'
                                << c->getNombre() << '|'
                                << c->getDuracion() << '|'
                                << c->getRutaBase() << '|'
                                << c->getTamano() << '|'
                                << c->getRelacionado1() << ','
                                << c->getRelacionado2() << '\n';
                        registrarID(c->getId());
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_BLOQUES; i++) {
        delete[] idsGuardados[i];
    }

    archivo.close();
    cout << "Playlist del usuario " << idUsuario << " guardada correctamente en: " << ruta << endl;
}

void Playlist::cargarDesdeArchivoPorId(int id, const string& carpetaOrigen) {
    string ruta = carpetaOrigen + "\\" + to_string(id) + ".txt";
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << ruta << endl;
        return;
    }

    idUsuario = id;
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        iteraciones++;

        size_t pos = 0;
        int idCancion;
        string nombre, rutaBase;
        float duracion;
        long tamano;
        int r1, r2;

        pos = linea.find('|');
        idCancion = stoi(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find('|');
        nombre = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find('|');
        duracion = stof(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find('|');
        rutaBase = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        pos = linea.find('|');
        tamano = stol(linea.substr(0, pos));
        linea.erase(0, pos + 1);

        pos = linea.find(',');
        r1 = stoi(linea.substr(0, pos));
        r2 = stoi(linea.substr(pos + 1));

        Cancion* nueva = new Cancion(idCancion, nombre, duracion, rutaBase, tamano, r1, r2);
        memoriaUsada += sizeof(Cancion);
        agregarCancion(nueva);
    }

    archivo.close();
    cout << "Playlist del usuario " << idUsuario << " cargada correctamente desde: " << ruta << endl;
}
