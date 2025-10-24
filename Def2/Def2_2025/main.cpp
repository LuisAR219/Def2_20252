#include <iostream>
#include <string>
#include <chrono>
#include "usuario.h"
#include "playlist.h"
#include "cancion.h"

using namespace std;

int main() {
    Playlist miLista;

    // Crear canciones usando el constructor real
    Cancion* c1 = new Cancion(1, "Bohemian Rhapsody", 6.0, "Rock/Bohemian_Rhapsody", 10240, 2, 3);
    Cancion* c2 = new Cancion(2, "Don't Stop Me Now", 4.0, "Rock/Dont_Stop_Me_Now", 9500, 1, 4);
    Cancion* c3 = new Cancion(3, "Thriller", 5.0, "Pop/Thriller", 11000, 2, 5);
    Cancion* c4 = new Cancion(4, "Billie Jean", 4.0, "Pop/Billie_Jean", 9800, 3, 1);
    Cancion* c5 = new Cancion(5, "Master of Puppets", 8.0, "Metal/Master_of_Puppets", 15000, 2, 6);
    Cancion* c6 = new Cancion(6, "Enter Sandman", 5.0, "Metal/Enter_Sandman", 12000, 1, 5);

    // Añadir canciones a la playlist usando el método oficial
    miLista.agregarCancion(c1);
    miLista.agregarCancion(c2);
    miLista.agregarCancion(c3);
    miLista.agregarCancion(c4);
    miLista.agregarCancion(c5);
    miLista.agregarCancion(c6);

    Usuario user;
    user.setPremium(false);  // Cambia a true para modo premium

    cout << "\n========== MODO ALEATORIO ==========\n";
    user.reproducirAleatorioDesdeBloques(&miLista);

    cout << "\n========== MODO ORDENADO ==========\n";
    user.reproducirEnOrden(&miLista);

    cout << "\n========== HISTORIAL ==========\n";
    user.mostrarHistorial5();

    // Liberar memoria
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
    delete c6;

    return 0;
}
