#include "artista.h"

using namespace std;

// Inicialización de miembros estáticos
Artista* Artista::lista = nullptr;
size_t Artista::capacidad = 0;
size_t Artista::cantidad = 0;

// Métodos privados
void Artista::init(int id_, const string& nombre_, int edad_,
                   const string& pais_, long seguidores_, int posicion_) {
    id = id_;
    nombre = nombre_;
    edad = edad_;
    pais = pais_;
    seguidores = seguidores_;
    posicionTendenciaGlobal = posicion_;
}

void Artista::asegurarCapacidad() {
    if (lista == nullptr) {
        capacidad = 8; // capacidad inicial
        lista = new Artista[capacidad];
        cantidad = 0;
        return;
    }
    if (cantidad < capacidad) return;

    // duplicar capacidad
    size_t nuevaCap = capacidad * 2;
    Artista* nuevo = new Artista[nuevaCap];
    for (size_t i = 0; i < cantidad; ++i) {
        nuevo[i] = lista[i]; // usa el operador de asignación por defecto
    }
    delete[] lista;
    lista = nuevo;
    capacidad = nuevaCap;
}

// Constructores
Artista::Artista()
    : id(0), nombre(""), edad(0), pais(""), seguidores(0), posicionTendenciaGlobal(0)
{}

Artista::Artista(int id_, const string& nombre_, int edad_,
                 const string& pais_, long seguidores_, int posicion) {
    init(id_, nombre_, edad_, pais_, seguidores_, posicion);
}

// ---------- Getters ----------
int Artista::getId() const { return id; }
const string& Artista::getNombre() const { return nombre; }
int Artista::getEdad() const { return edad; }
const string& Artista::getPais() const { return pais; }
long Artista::getSeguidores() const { return seguidores; }
int Artista::getPosicionTendenciaGlobal() const { return posicionTendenciaGlobal; }

// Representación
void Artista::print() const {
    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Edad: " << edad
         << " | País: " << pais
         << " | Seguidores: " << seguidores
         << " | PosicionTendenciaGlobal: " << posicionTendenciaGlobal
         << '\n';
}

string Artista::toLine() const {
    return to_string(id) + "|" + nombre + "|" + to_string(edad) + "|" +
           pais + "|" + to_string(seguidores) + "|" + to_string(posicionTendenciaGlobal);
}

// Carga
void Artista::cargarDesdeArchivo(const string& ruta) {
    ifstream in(ruta.c_str());
    if (!in.is_open()) {
        cerr << "Error: no se pudo abrir el archivo: " << ruta << '\n';
        return;
    }

    // aseguramos la estructura de almacenamiento inicial
    if (lista == nullptr) {
        asegurarCapacidad();
    }

    string linea;
    size_t nroLinea = 0;
    while (getline(in, linea)) {
        ++nroLinea;
        // Saltar líneas vacías
        if (linea.empty()) continue;

        // Parseo manual: extraer hasta 6 campos separados por '|'
        string campos[6];
        int campoIndex = 0;
        size_t start = 0;

        for (size_t i = 0; i <= linea.size() && campoIndex < 6; ++i) {
            if (i == linea.size() || linea[i] == '|') {
                campos[campoIndex] = linea.substr(start, i - start);
                ++campoIndex;
                start = i + 1;
            }
        }

        if (campoIndex != 6) {
            cerr << "Linea " << nroLinea << ": formato inválido (se esperaban 6 campos), se omite.\n";
            continue;
        }

        // Conversión y validación de campos numéricos
        int idVal = 0;
        int edadVal = 0;
        long seguidoresVal = 0;
        int posicionVal = 0;

        try {
            idVal = stoi(campos[0]);
            edadVal = stoi(campos[2]);
            seguidoresVal = stol(campos[4]);
            posicionVal = stoi(campos[5]);
        } catch (const exception& e) {
            cerr << "Linea " << nroLinea << ": error en conversión numérica (" << e.what()
                << "), se omite.\n";
            continue;
        }

        // Validaciones simples
        if (idVal <= 0) {
            cerr << "Linea " << nroLinea << ": id inválido (" << idVal << "), se omite.\n";
            continue;
        }
        if (edadVal < 0 || edadVal > 120) {
            cerr << "Linea " << nroLinea << ": edad fuera de rango (" << edadVal << "), se omite.\n";
            continue;
        }
        if (seguidoresVal < 0) {
            cerr << "Linea " << nroLinea << ": seguidores negativos (" << seguidoresVal << "), se omite.\n";
            continue;
        }
        if (posicionVal < 0) {
            cerr << "Linea " << nroLinea << ": posicion en tendencia negativa (" << posicionVal << "), se omite.\n";
            continue;
        }

        // Verificar duplicados (búsqueda lineal)
        bool duplicado = false;
        for (size_t i = 0; i < cantidad; ++i) {
            if (lista[i].getId() == idVal) {
                duplicado = true;
                break;
            }
        }
        if (duplicado) {
            cerr << "Linea " << nroLinea << ": id " << idVal << " ya existe, se ignora la línea duplicada.\n";
            continue;
        }

        // Si todo OK, asegurar capacidad y añadir
        asegurarCapacidad();
        lista[cantidad] = Artista(idVal, campos[1], edadVal, campos[3], seguidoresVal, posicionVal);
        ++cantidad;
    }

    in.close();
}

// Accesos y utilidades
const Artista* Artista::getLista() {
    return lista;
}

size_t Artista::getCantidad() {
    return cantidad;
}

void Artista::imprimirTodos() {
    for (size_t i = 0; i < cantidad; ++i) {
        lista[i].print();
    }
}

void Artista::liberarMemoria() {
    if (lista) {
        delete[] lista;
        lista = nullptr;
        capacidad = 0;
        cantidad = 0;
    }
}

