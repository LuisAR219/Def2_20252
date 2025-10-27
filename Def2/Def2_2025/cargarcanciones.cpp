#include "cancion.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

static int totalIteraciones = 0;
static size_t totalMemoria = 0;

const int MAX_BLOQUES = 10;
const int TAM_BLOQUE = 1000;
int bloquesUsados = 0;
int cancionesPorBloque[10] = {0};
Cancion* canciones[10][1000] = {nullptr};

void parsearLineaCancion(const string& linea, Cancion*& nuevaCancion) {
    totalIteraciones++;
    if (linea.empty()) return;

    string idStr, nombre, artista, duracionStr, ruta, tamanoStr, relacionadosStr;
    int id = 0, rel1 = 0, rel2 = 0;
    float duracion = 0.0f;
    long tamano = 0;

    size_t pos1 = 0;
    size_t pos2 = linea.find('|');
    totalIteraciones += 2;

    if (pos2 != string::npos) {
        // Campo 1: ID
        idStr = linea.substr(pos1, pos2 - pos1);
        for (size_t i = 0; i < idStr.length(); i++) {
            if (idStr[i] >= '0' && idStr[i] <= '9') {
                id = id * 10 + (idStr[i] - '0');
                totalIteraciones++;
            }
        }

        // Campo 2: Nombre
        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        nombre = linea.substr(pos1, pos2 - pos1);

        // Campo 3: Artista (NUEVO CAMPO)
        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        artista = linea.substr(pos1, pos2 - pos1);

        // Campo 4: Duración
        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        duracionStr = linea.substr(pos1, pos2 - pos1);
        bool punto = false;
        float decimal = 0.1f;
        for (size_t i = 0; i < duracionStr.length(); i++) {
            char c = duracionStr[i];
            if (c == '.') { punto = true; continue; }
            if (c >= '0' && c <= '9') {
                if (!punto) duracion = duracion * 10 + (c - '0');
                else { duracion += (c - '0') * decimal; decimal /= 10.0f; }
            }
        }

        // Campo 5: Ruta
        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        ruta = linea.substr(pos1, pos2 - pos1);

        // Campo 6: Tamaño
        pos1 = pos2 + 1;
        pos2 = linea.find('|', pos1);
        tamanoStr = linea.substr(pos1, pos2 - pos1);
        for (size_t i = 0; i < tamanoStr.length(); i++) {
            if (tamanoStr[i] >= '0' && tamanoStr[i] <= '9') {
                tamano = tamano * 10 + (tamanoStr[i] - '0');
                totalIteraciones++;
            }
        }

        // Campo 7: Relacionados
        pos1 = pos2 + 1;
        relacionadosStr = linea.substr(pos1);
        size_t coma = relacionadosStr.find(',');
        if (coma != string::npos) {
            string r1Str = relacionadosStr.substr(0, coma);
            string r2Str = relacionadosStr.substr(coma + 1);
            for (size_t i = 0; i < r1Str.length(); i++)
                if (r1Str[i] >= '0' && r1Str[i] <= '9')
                    rel1 = rel1 * 10 + (r1Str[i] - '0');
            for (size_t i = 0; i < r2Str.length(); i++)
                if (r2Str[i] >= '0' && r2Str[i] <= '9')
                    rel2 = rel2 * 10 + (r2Str[i] - '0');
        }
    }

    totalMemoria += sizeof(Cancion) + sizeof(string) * 7;
    // Llamada CORREGIDA al constructor con artista
    nuevaCancion = new Cancion(id, nombre, artista, duracion, ruta, tamano, rel1, rel2);
}

bool cargarCancionesDesdeTXT(const string& nombreArchivoIgnorado) {
    string nombreArchivo = "canciones.txt";

    ifstream archivo(nombreArchivo.c_str());
    totalIteraciones++;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return false;
    }

    string linea;
    int totalCargadas = 0;

    bloquesUsados = 0;
    for (int i = 0; i < MAX_BLOQUES; i++) {
        cancionesPorBloque[i] = 0;
        for (int j = 0; j < TAM_BLOQUE; j++) {
            canciones[i][j] = nullptr;
        }
    }

    while (getline(archivo, linea)) {
        totalIteraciones++;
        if (linea.empty()) continue;

        Cancion* nueva = nullptr;
        parsearLineaCancion(linea, nueva);

        if (nueva) {
            if (cancionesPorBloque[bloquesUsados] >= TAM_BLOQUE) {
                bloquesUsados++;
                if (bloquesUsados >= MAX_BLOQUES) {
                    cout << "Limite maximo de bloques alcanzado.\n";
                    break;
                }
            }
            canciones[bloquesUsados][cancionesPorBloque[bloquesUsados]] = nueva;
            cancionesPorBloque[bloquesUsados]++;
            totalCargadas++;
        }
    }

    archivo.close();
    cout << "Se cargaron " << totalCargadas << " canciones en "
         << (bloquesUsados + 1) << " bloque(s).\n";
    cout << "Iteraciones totales: " << totalIteraciones
         << " | Memoria estimada: " << totalMemoria << " bytes\n";
    return true;
}

void mostrarCancionesCargadas(int limite = 10, const string& membresia = "C") {
    int mostradas = 0;
    for (int b = 0; b <= bloquesUsados && mostradas < limite; b++) {
        for (int i = 0; i < cancionesPorBloque[b] && mostradas < limite; i++) {
            if (canciones[b][i] != nullptr) {
                canciones[b][i]->mostrarCancion(membresia);
                mostradas++;
            }
        }
    }
}

// Función CORREGIDA para obtener canciones
Cancion** obtenerCanciones(int bloque) {
    if (bloque >= 0 && bloque <= bloquesUsados) {
        return canciones[bloque];
    }
    return nullptr;
}

int* obtenerCancionesPorBloque() {
    return cancionesPorBloque;
}

int obtenerBloquesUsados() {
    return bloquesUsados;
}

int obtenerTotalIteraciones() {
    return totalIteraciones;
}

size_t obtenerTotalMemoria() {
    return totalMemoria;
}
