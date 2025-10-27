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

#include <cctype>

bool isFloat(const string& str) {
    if (str.empty()) return false;

    bool hasDecimal = false;
    bool hasDigits = false;
    bool hasExponent = false;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];

        if (isdigit(c)) {
            hasDigits = true;
        } else if (c == '.' && !hasDecimal && !hasExponent) {
            hasDecimal = true;
        } else if ((c == 'e' || c == 'E') && !hasExponent && hasDigits) {
            hasExponent = true;
            hasDigits = false;
        } else if (c == '-' && i == 0) {
            // Signo negativo al inicio
        } else if (c == '-' && hasExponent && !hasDigits) {
            // Signo negativo después del exponente
        } else if (c == '+' && i == 0) {
            // Signo positivo al inicio
        } else if (c == '+' && hasExponent && !hasDigits) {
            // Signo positivo después del exponente
        } else {
            return false;
        }
    }

    return hasDigits;
}

// Función auxiliar min (si no existe)
template<typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
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
    int lineasCargadas = 0;
    int lineasConError = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        iteraciones++;

        string campos[7];
        int campoIndex = 0;
        size_t start = 0;
        size_t end = 0;

        for (int i = 0; i < 7 && start < linea.length(); i++) {
            end = linea.find('|', start);
            if (end == string::npos) {
                campos[i] = linea.substr(start);
                campoIndex = i + 1;
                break;
            }
            campos[i] = linea.substr(start, end - start);
            start = end + 1;
            campoIndex = i + 1;
        }

        if (campoIndex < 7) {
            lineasConError++;
            continue;
        }

        try {
            int idCancion = stoi(campos[0]);
            string nombre = campos[1];

            string artista;
            float duracion;
            string rutaBase;
            long tamano;
            int r1, r2;

            if (!isFloat(campos[2]) && isFloat(campos[3])) {
                artista = campos[2];
                duracion = stof(campos[3]);
                rutaBase = campos[4];
                tamano = stol(campos[5]);
            }
            else if (isFloat(campos[2]) && !isFloat(campos[3])) {
                duracion = stof(campos[2]);
                artista = campos[3];
                rutaBase = campos[4];
                tamano = stol(campos[5]);
            }
            else {
                artista = campos[2];
                duracion = stof(campos[3]);
                rutaBase = campos[4];
                tamano = stol(campos[5]);
            }

            size_t comma = campos[6].find(',');
            if (comma != string::npos) {
                r1 = stoi(campos[6].substr(0, comma));
                r2 = stoi(campos[6].substr(comma + 1));
            } else {
                r1 = r2 = 0;
            }

            if (duracion > 1000 || duracion < 0.1) {
                duracion = 3.5f;
            }

            if (isFloat(artista)) {
                artista = "Artista Desconocido";
            }

            Cancion* nueva = new Cancion(idCancion, nombre, artista, duracion, rutaBase, tamano, r1, r2);
            memoriaUsada += sizeof(Cancion);
            agregarCancion(nueva);
            lineasCargadas++;

        } catch (const exception& e) {
            lineasConError++;
            continue;
        }
    }

    archivo.close();
    cout << "Playlist de " << nicknameUsuario << " cargada: " << lineasCargadas << " canciones" << endl;
}



