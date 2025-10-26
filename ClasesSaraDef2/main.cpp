#include <iostream>
#include <string>
#include <chrono>
#include "usuario.h"
#include "playlist.h"
#include "cancion.h"
#include "album.h"

using namespace std;

int main() {
    // ─────────────────────────────────────────────
    // PRUEBA 1: PLAYLIST + USUARIO
    // ─────────────────────────────────────────────
    Playlist miLista;

    // Crear canciones de prueba
    Cancion* c1 = new Cancion(1, "Bohemian Rhapsody", 6.0, "Rock/Bohemian_Rhapsody", 10240, 2, 3);
    Cancion* c2 = new Cancion(2, "Don't Stop Me Now", 4.0, "Rock/Dont_Stop_Me_Now", 9500, 1, 4);
    Cancion* c3 = new Cancion(3, "Thriller", 5.0, "Pop/Thriller", 11000, 2, 5);
    Cancion* c4 = new Cancion(4, "Billie Jean", 4.0, "Pop/Billie_Jean", 9800, 3, 1);
    Cancion* c5 = new Cancion(5, "Master of Puppets", 8.0, "Metal/Master_of_Puppets", 15000, 2, 6);
    Cancion* c6 = new Cancion(6, "Enter Sandman", 5.0, "Metal/Enter_Sandman", 12000, 1, 5);

    // Agregar canciones a la playlist
    miLista.agregarCancion(c1);
    miLista.agregarCancion(c2);
    miLista.agregarCancion(c3);
    miLista.agregarCancion(c4);
    miLista.agregarCancion(c5);
    miLista.agregarCancion(c6);

    // Crear usuario
    Usuario user;
    user.setPremium(false);  // Cambia a true para modo premium

    cout << "\n========== MODO ALEATORIO ==========\n";
    user.reproducirAleatorioDesdeBloques(&miLista);

    cout << "\n========== MODO ORDENADO ==========\n";
    user.reproducirEnOrden(&miLista);

    cout << "\n========== HISTORIAL ==========\n";
    user.mostrarHistorial5();

    // ─────────────────────────────────────────────
    // PRUEBA 2: ÁLBUMES
    // ─────────────────────────────────────────────
    cout << "\n========== CARGA DE ÁLBUMES ==========\n";

    // El archivo se debe llamar, por ejemplo, "albumes.txt" en la misma carpeta del ejecutable
    if (Album::cargarDesdeArchivo("albumes.txt")) {
        cout << "\n--- Lista de álbumes cargados ---\n";
        Album::imprimirTodos();
        Album::mostrarMetricas();
    } else {
        cout << "No se pudo cargar el archivo de álbumes.\n";
    }

    // ─────────────────────────────────────────────
    // PRUEBA 3: MINI SIMULACIÓN DE USUARIO DESDE TEXTO
    // ─────────────────────────────────────────────
    cout << "\n========== SIMULACIÓN DE USUARIO DESDE TEXTO ==========\n";

    string lineaEjemplo = "PremiumUser1|300010203|1|Colombia|Ciudad P1|2025-01-01|-";
    cout << "Ejemplo de línea del archivo usuarios.txt:\n" << lineaEjemplo << endl;

    // Parseo manual simple (solo demostrativo)
    size_t pos = 0;
    string campos[7];
    for (int i = 0; i < 7; ++i) {
        size_t sep = lineaEjemplo.find('|', pos);
        if (sep == string::npos) {
            campos[i] = lineaEjemplo.substr(pos);
            break;
        } else {
            campos[i] = lineaEjemplo.substr(pos, sep - pos);
            pos = sep + 1;
        }
    }

    cout << "\nNickname: " << campos[0]
         << "\nContraseña: " << campos[1]
         << "\nMembresía: " << (campos[2] == "1" ? "Premium" : "Estándar")
         << "\nPaís: " << campos[3]
         << "\nCiudad: " << campos[4]
         << "\nFecha: " << campos[5]
         << "\nSigue a: " << campos[6] << endl;

    // ─────────────────────────────────────────────
    // LIBERAR MEMORIA
    // ─────────────────────────────────────────────
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
    delete c6;

    Album::liberarMemoria();

    cout << "\n=== FIN DE LAS PRUEBAS ===\n";
    return 0;
}
