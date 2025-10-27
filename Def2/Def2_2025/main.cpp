#include <iostream>
#include <string>
#include "album.h"
#include "anuncio.h"
#include "artista.h"
#include "cancion.h"
#include "cargarcanciones.h"
#include "personacredito.h"
#include "playlist.h"
#include "usuario.h"
#include "funcionalidades.h"

using namespace std;


int main() {
    cout << "=== BIENVENIDO A UdeATunes ===" << endl;

    // Cargar datos iniciales
    cargarDatosIniciales();

    Usuario* usuarioActual = nullptr;
    Playlist* playlistActual = new Playlist();

    // Sistema de inicio de sesion
    iniciarSesion(usuarioActual, playlistActual);

    if (usuarioActual != nullptr) {
        menuPrincipal(usuarioActual, playlistActual);
    }

    // Liberar memoria y mostrar metricas finales
    delete playlistActual;
    Album::liberarMemoria();
    Artista::liberarMemoria();
    anuncio::resetearAnuncios();

    mostrarMetricasFinales();

    cout << "\nGracias por usar UdeATunes. ¡Hasta pronto!" << endl;
    return 0;
}
