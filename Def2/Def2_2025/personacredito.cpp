#include "personacredito.h"
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

PersonaCredito::PersonaCredito()
    : productores(nullptr), compositores(nullptr), musicos(nullptr),
    sizesProductores(nullptr), sizesCompositores(nullptr), sizesMusicos(nullptr),
    totalProductores(0), totalCompositores(0), totalMusicos(0) {

    productores = new Persona**[STATIC_SIZE];
    compositores = new Persona**[STATIC_SIZE];
    musicos = new Persona**[STATIC_SIZE];

    sizesProductores = new int[STATIC_SIZE];
    sizesCompositores = new int[STATIC_SIZE];
    sizesMusicos = new int[STATIC_SIZE];

    for (int i = 0; i < STATIC_SIZE; i++) {
        productores[i] = nullptr;
        compositores[i] = nullptr;
        musicos[i] = nullptr;
        sizesProductores[i] = 0;
        sizesCompositores[i] = 0;
        sizesMusicos[i] = 0;
    }
}

PersonaCredito::~PersonaCredito() {
    liberarArreglo(productores, sizesProductores);
    liberarArreglo(compositores, sizesCompositores);
    liberarArreglo(musicos, sizesMusicos);
}

bool PersonaCredito::cargarDesdeArchivo(int& iteraciones, long& memoria) {
    iteraciones = 0;
    const char* filename = "creditos.txt";
    ifstream archivo(filename);

    if (!archivo.is_open()) {
        memoria = calcularMemoria();
        return false;
    }

    char linea[256];
    int exitos = 0;

    while (archivo.getline(linea, sizeof(linea))) {
        iteraciones++;

        char* token = strtok(linea, "|");
        if (token == nullptr) continue;

        char* nombre = token;
        token = strtok(nullptr, "|");
        if (token == nullptr) continue;

        char* codigo = token;
        token = strtok(nullptr, "|");
        if (token == nullptr) continue;

        char* rol = token;

        if (strlen(codigo) != 10) continue;

        if (agregarPersona(nombre, codigo, rol, iteraciones, memoria)) {
            exitos++;
        }
    }

    archivo.close();
    memoria = calcularMemoria();

    cout << "Iteraciones totales (carga): " << iteraciones << endl;
    cout << "Memoria total usada: " << memoria << " bytes" << endl;

    return (exitos > 0);
}

bool PersonaCredito::agregarPersona(const char* nombre, const char* codigo, const char* rol,
                                    int& iteraciones, long& memoria) {
    iteraciones = 0;

    int iterExiste;
    long memExiste;
    if (personaExiste(codigo, iterExiste, memExiste)) {
        iteraciones += iterExiste;
        memoria = calcularMemoria();
        return false;
    }
    iteraciones += iterExiste;

    Persona nuevaPersona;
    nuevaPersona.nombre = copiarCadena(nombre);
    nuevaPersona.codigo = copiarCadena(codigo);
    nuevaPersona.rol = copiarCadena(rol);

    Persona*** arregloDestino = nullptr;
    int* sizesDestino = nullptr;
    int* totalDestino = nullptr;

    if (strcmp(rol, "Productor") == 0) {
        arregloDestino = productores;
        sizesDestino = sizesProductores;
        totalDestino = &totalProductores;
    } else if (strcmp(rol, "Compositor") == 0) {
        arregloDestino = compositores;
        sizesDestino = sizesCompositores;
        totalDestino = &totalCompositores;
    } else if (strcmp(rol, "Músico") == 0) {
        arregloDestino = musicos;
        sizesDestino = sizesMusicos;
        totalDestino = &totalMusicos;
    } else {
        liberarMemoriaPersona(nuevaPersona);
        memoria = calcularMemoria();
        return false;
    }

    int iterBloque;
    int bloqueIndex = encontrarBloqueDisponible(arregloDestino, sizesDestino, iterBloque);
    iteraciones += iterBloque;

    if (bloqueIndex == -1) {
        liberarMemoriaPersona(nuevaPersona);
        memoria = calcularMemoria();
        return false;
    }

    if (arregloDestino[bloqueIndex] == nullptr) {
        arregloDestino[bloqueIndex] = new Persona*[DYNAMIC_SIZE];
        for (int i = 0; i < DYNAMIC_SIZE; i++) {
            iteraciones++;
            arregloDestino[bloqueIndex][i] = nullptr;
        }
        sizesDestino[bloqueIndex] = 0;
    }

    arregloDestino[bloqueIndex][sizesDestino[bloqueIndex]] = new Persona;
    *arregloDestino[bloqueIndex][sizesDestino[bloqueIndex]] = nuevaPersona;
    sizesDestino[bloqueIndex]++;
    (*totalDestino)++;

    memoria = calcularMemoria();
    return true;
}

bool PersonaCredito::personaExiste(const char* codigo, int& iteraciones, long& memoria) const {
    iteraciones = 0;

    for (int i = 0; i < STATIC_SIZE; i++) {
        iteraciones++;
        if (productores[i] != nullptr) {
            for (int j = 0; j < sizesProductores[i]; j++) {
                iteraciones++;
                if (productores[i][j] != nullptr &&
                    strcmp(productores[i][j]->codigo, codigo) == 0) {
                    memoria = calcularMemoria();
                    return true;
                }
            }
        }
    }

    for (int i = 0; i < STATIC_SIZE; i++) {
        iteraciones++;
        if (compositores[i] != nullptr) {
            for (int j = 0; j < sizesCompositores[i]; j++) {
                iteraciones++;
                if (compositores[i][j] != nullptr &&
                    strcmp(compositores[i][j]->codigo, codigo) == 0) {
                    memoria = calcularMemoria();
                    return true;
                }
            }
        }
    }

    for (int i = 0; i < STATIC_SIZE; i++) {
        iteraciones++;
        if (musicos[i] != nullptr) {
            for (int j = 0; j < sizesMusicos[i]; j++) {
                iteraciones++;
                if (musicos[i][j] != nullptr &&
                    strcmp(musicos[i][j]->codigo, codigo) == 0) {
                    memoria = calcularMemoria();
                    return true;
                }
            }
        }
    }

    memoria = calcularMemoria();
    return false;
}

void PersonaCredito::liberarMemoriaPersona(Persona& persona) {
    if (persona.nombre != nullptr) delete[] persona.nombre;
    if (persona.codigo != nullptr) delete[] persona.codigo;
    if (persona.rol != nullptr) delete[] persona.rol;
}

void PersonaCredito::liberarArreglo(Persona***& arreglo, int*& sizes) {
    if (arreglo != nullptr) {
        for (int i = 0; i < STATIC_SIZE; i++) {
            if (arreglo[i] != nullptr) {
                for (int j = 0; j < DYNAMIC_SIZE; j++) {
                    if (arreglo[i][j] != nullptr) {
                        liberarMemoriaPersona(*arreglo[i][j]);
                        delete arreglo[i][j];
                    }
                }
                delete[] arreglo[i];
            }
        }
        delete[] arreglo;
        arreglo = nullptr;
    }

    if (sizes != nullptr) {
        delete[] sizes;
        sizes = nullptr;
    }
}

int PersonaCredito::encontrarBloqueDisponible(Persona*** arreglo, int* sizes, int& iteraciones) const {
    iteraciones = 0;
    for (int i = 0; i < STATIC_SIZE; i++) {
        iteraciones++;
        if (arreglo[i] == nullptr || sizes[i] < DYNAMIC_SIZE)
            return i;
    }
    return -1;
}

long PersonaCredito::calcularMemoria() const {
    long memoriaTotal = sizeof(PersonaCredito);
    memoriaTotal += sizeof(Persona***) * 3 + sizeof(int*) * 3 + sizeof(int) * 3;

    auto sumarMemoria = [&](Persona*** arr, int* sizes) {
        if (arr == nullptr) return;
        memoriaTotal += sizeof(Persona**) * STATIC_SIZE;
        for (int i = 0; i < STATIC_SIZE; i++) {
            if (arr[i] != nullptr) {
                memoriaTotal += sizeof(Persona*) * DYNAMIC_SIZE;
                for (int j = 0; j < sizes[i]; j++) {
                    if (arr[i][j] != nullptr) {
                        memoriaTotal += sizeof(Persona);
                        if (arr[i][j]->nombre) memoriaTotal += strlen(arr[i][j]->nombre) + 1;
                        if (arr[i][j]->codigo) memoriaTotal += strlen(arr[i][j]->codigo) + 1;
                        if (arr[i][j]->rol) memoriaTotal += strlen(arr[i][j]->rol) + 1;
                    }
                }
            }
        }
    };

    sumarMemoria(productores, sizesProductores);
    sumarMemoria(compositores, sizesCompositores);
    sumarMemoria(musicos, sizesMusicos);

    return memoriaTotal;
}

char* PersonaCredito::copiarCadena(const char* origen) {
    if (!origen) return nullptr;
    int longitud = strlen(origen);
    char* copia = new char[longitud + 1];
    strcpy(copia, origen);
    return copia;
}

