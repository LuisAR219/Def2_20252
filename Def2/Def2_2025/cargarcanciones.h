#ifndef CARGAR_CANCIONES_H
#define CARGAR_CANCIONES_H

#include "cancion.h"
#include <string>
using namespace std;

extern const int MAX_BLOQUES;
extern const int TAM_BLOQUE;
extern int bloquesUsados;
extern int cancionesPorBloque[10];
extern Cancion* canciones[10][1000];

bool cargarCancionesDesdeTXT(const string& nombreArchivo);
void mostrarCancionesCargadas(int limite = 10, const string& membresia = "C");

#endif // CARGAR_CANCIONES_H
