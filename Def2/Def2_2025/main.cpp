#include "cancion.h"
#include <iostream>

using namespace std;

int main() {
    // Crear un arreglo de canciones
    const int NUM_CANCIONES = 3;
    Cancion* canciones[NUM_CANCIONES];

    canciones[0] = new Cancion(1, "Canción 1", 3.5, "/music/song1", 1024, 2, 3);
    canciones[1] = new Cancion(2, "Canción 2", 4.2, "/music/song2", 2048, 1, 3);
    canciones[2] = new Cancion(3, "Canción 3", 2.8, "/music/song3", 512, 1, 2);

    // Buscar y mostrar canción por ID
    Cancion::mostrarCancionPorId(canciones, NUM_CANCIONES, 2, "premium");

    // Buscar canción y usar sus métodos
    Cancion* encontrada = Cancion::buscarPorId(canciones, NUM_CANCIONES, 1);
    if (encontrada != nullptr) {
        cout << "Encontrada: " << encontrada->getNombre() << endl;
        encontrada->reproducir();
        this_thread::sleep_for(chrono::seconds(2));
        encontrada->detener();
    }

    // Limpiar memoria
    for (int i = 0; i < NUM_CANCIONES; i++) {
        delete canciones[i];
    }

    return 0;
}
