#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H

#include <string>
#include "usuario.h"
#include "playlist.h"

using namespace std;

// Variables globales para métricas
extern long iteraciones_totales;
extern long memoria_total;

// Prototipos de funciones
void cargarDatosIniciales();
void mostrarMetricasFinales();
bool cargarUsuariosDesdeArchivo(Usuario**& usuarios, int& totalUsuarios);
void iniciarSesion(Usuario*& usuarioActual, Playlist*& playlistActual);
void menuPrincipal(Usuario* usuario, Playlist* playlist);
void menuReproduccionAleatoria(Usuario* usuario);
void menuReproducirAnteriores(Usuario* usuario);
void menuFavoritos(Usuario* usuario, Playlist* playlist);
void liberarUsuarios(Usuario** usuarios, int totalUsuarios);

#endif
