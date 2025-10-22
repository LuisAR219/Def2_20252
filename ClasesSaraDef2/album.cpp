#include "album.h"

using namespace std;

// Inicialización del catálogo estático
Album* Album::catalogo[MAX_ALBUMS] = { nullptr };
int Album::catalogoCount = 0;

// Constructor / Destructor
Album::Album(int id,
             const string& titulo,
             int idArtista,
             const string& generosCSV,
             const string& fechaLanzamiento,
             const string& sello,
             const string& rutaPortada,
             int rating)
    : id(id),
    titulo(titulo),
    idArtista(idArtista),
    generosCSV(generosCSV),
    fechaLanzamiento(fechaLanzamiento),
    sello(sello),
    rutaPortada(rutaPortada),
    rating(rating),
    idsCanciones(nullptr),
    capacidadIds(0),
    cantidadIds(0)
{
    // inicializar arreglo de ids con capacidad inicial pequeña
    capacidadIds = 8;
    idsCanciones = new int[capacidadIds];
}

Album::~Album() {
    if (idsCanciones) {
        delete[] idsCanciones;
        idsCanciones = nullptr;
    }
}

// Getters / Setters
int Album::getId() const { return id; }
string Album::getTitulo() const { return titulo; }
int Album::getIdArtista() const { return idArtista; }
string Album::getGenerosCSV() const { return generosCSV; }
string Album::getFechaLanzamiento() const { return fechaLanzamiento; }
string Album::getSello() const { return sello; }
string Album::getRutaPortada() const { return rutaPortada; }
int Album::getRating() const { return rating; }
const int* Album::getIdsCanciones(int &outCantidad) const {
    outCantidad = cantidadIds;
    return idsCanciones;
}

void Album::setTitulo(const string& t) { titulo = t; }
void Album::setSello(const string& s) { sello = s; }
void Album::setRutaPortada(const string& r) { rutaPortada = r; }
void Album::setFechaLanzamiento(const string& f) { fechaLanzamiento = f; }
void Album::setRating(int r) { rating = r; }

// Gestión de ids de canciones
bool Album::contieneCancion(int idCancion) const {
    if (idCancion <= 0) return false;
    for (int i = 0; i < cantidadIds; ++i) {
        if (idsCanciones[i] == idCancion) return true;
    }
    return false;
}

bool Album::agregarIdCancion(int idCancion) {
    if (idCancion <= 0) return false;
    if (contieneCancion(idCancion)) return false;
    // redimensionar si es necesario
    if (cantidadIds >= capacidadIds) {
        int nuevaCap = capacidadIds * 2;
        if (nuevaCap <= 0) nuevaCap = 8;
        if (nuevaCap > MAX_CANCIONES_POR_ALBUM) nuevaCap = MAX_CANCIONES_POR_ALBUM;
        int* nuevo = new int[nuevaCap];
        for (int i = 0; i < cantidadIds; ++i) nuevo[i] = idsCanciones[i];
        delete[] idsCanciones;
        idsCanciones = nuevo;
        capacidadIds = nuevaCap;
    }
    if (cantidadIds < capacidadIds) {
        idsCanciones[cantidadIds++] = idCancion;
        return true;
    }
    return false;
}

bool Album::eliminarIdCancion(int idCancion) {
    for (int i = 0; i < cantidadIds; ++i) {
        if (idsCanciones[i] == idCancion) {
            // mover elementos hacia atrás
            for (int j = i; j + 1 < cantidadIds; ++j) idsCanciones[j] = idsCanciones[j+1];
            --cantidadIds;
            return true;
        }
    }
    return false;
}

int Album::cantidadCanciones() const {
    return cantidadIds;
}

// Serializar / Mostrar
string Album::serializarLinea() const {
    // Formato: id|titulo|idArtista|generosCSV|fecha|sello|rutaPortada|rating
    string out = "";
    out += to_string(id);
    out += "|";
    out += titulo;
    out += "|";
    out += to_string(idArtista);
    out += "|";
    out += generosCSV;
    out += "|";
    out += fechaLanzamiento;
    out += "|";
    out += sello;
    out += "|";
    out += rutaPortada;
    out += "|";
    out += to_string(rating);
    return out;
}

void Album::mostrarResumen() const {
    cout << "------- Album " << id << " -------\n";
    cout << "Titulo     : " << titulo << "\n";
    cout << "Artista ID : " << idArtista << "\n";
    cout << "Generos    : " << generosCSV << "\n";
    cout << "Fecha Lanz.: " << fechaLanzamiento << "\n";
    cout << "Sello      : " << sello << "\n";
    cout << "Portada    : " << rutaPortada << "\n";
    cout << "Rating     : " << rating << " (ids guardados: " << cantidadIds << ")\n";
    cout << "-----------------------------\n";
}

// Helpers de strings (simples)
string Album::trim(const string& s) {
    int a = 0;
    int b = (int)s.size() - 1;
    while (a <= b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n')) ++a;
    while (b >= a && (s[b] == ' ' || s[b] == '\t' || s[b] == '\r' || s[b] == '\n')) --b;
    if (b < a) return string("");
    return substring(s, a, b - a + 1);
}

int Album::findChar(const string& s, char c, int start) {
    int n = (int)s.size();
    for (int i = start; i < n; ++i) if (s[i] == c) return i;
    return -1;
}

string Album::substring(const string& s, int start, int length) {
    if (length <= 0) return string("");
    if (start < 0) start = 0;
    int n = (int)s.size();
    if (start >= n) return string("");
    int maxlen = n - start;
    if (length > maxlen) length = maxlen;
    return s.substr(start, length);
}

bool Album::isDigits(const string& s) {
    if (s.empty()) return false;
    for (int i = 0; i < (int)s.size(); ++i) {
        char c = s[i];
        if (c < '0' || c > '9') return false;
    }
    return true;
}

// Registro / Creación interna
Album* Album::crearYRegistrar(int id,
                              const string& titulo,
                              int idArtista,
                              const string& generosCSV,
                              const string& fechaLanzamiento,
                              const string& sello,
                              const string& rutaPortada,
                              int rating) {
    if (id <= 0) return nullptr;
    // comprobar duplicado simple
    for (int i = 0; i < catalogoCount; ++i) {
        if (catalogo[i] != nullptr && catalogo[i]->getId() == id) {
            // advertencia y no duplicar
            cerr << "Advertencia: album con id " << id << " ya existe. Ignorando.\n";
            return nullptr;
        }
    }
    if (catalogoCount >= MAX_ALBUMS) {
        cerr << "Catalogo lleno, no se puede agregar album id " << id << "\n";
        return nullptr;
    }
    Album* a = new Album(id, titulo, idArtista, generosCSV, fechaLanzamiento, sello, rutaPortada, rating);
    catalogo[catalogoCount++] = a;
    return a;
}

// Cargar / Guardar catalogo (I/O)
bool Album::cargarDesdeArchivo(const string& rutaArchivo) {
    ifstream in(rutaArchivo.c_str());
    if (!in.is_open()) {
        cerr << "No se pudo abrir archivo: " << rutaArchivo << "\n";
        return false;
    }

    string linea;
    int lineNum = 0;
    int cargados = 0;
    while (getline(in, linea)) {
        ++lineNum;
        linea = trim(linea);
        if (linea.empty()) continue;

        // parsear en 8 campos separados por '|'
        int n = (int)linea.size();
        string campos[8];
        bool fallo = false;
        int start = 0;
        for (int idx = 0; idx < 7; ++idx) {
            int p = findChar(linea, '|', start);
            if (p == -1) { fallo = true; break; }
            campos[idx] = trim(substring(linea, start, p - start));
            start = p + 1;
        }
        if (!fallo) {
            campos[7] = trim(substring(linea, start, n - start));
        } else {
            cerr << "Linea " << lineNum << " invalida (campos insuficientes): " << linea << "\n";
            continue;
        }

        // Parsear campos
        string idStr = campos[0];
        string titulo = campos[1];
        string idArtStr = campos[2];
        string generos = campos[3];
        string fecha = campos[4];
        string sello = campos[5];
        string rutaPort = campos[6];
        string ratingStr = campos[7];

        bool ok = true;
        int albumId = 0;
        int idArtista = 0;
        int ratingVal = 0;

        if (isDigits(idStr)) {
            try { albumId = stoi(idStr); }
            catch (...) { ok = false; }
        } else ok = false;

        if (isDigits(idArtStr)) {
            try { idArtista = stoi(idArtStr); }
            catch (...) { ok = false; }
        } else ok = false;

        if (isDigits(ratingStr)) {
            try { ratingVal = stoi(ratingStr); }
            catch (...) { ratingVal = 0; }
        } else {
            // si rating no es numérico, lo dejamos en 0
            ratingVal = 0;
        }

        if (!ok) {
            cerr << "Linea " << lineNum << " contiene id no numerico. Se omite.\n";
            continue;
        }

        Album* creado = crearYRegistrar(albumId, titulo, idArtista, generos, fecha, sello, rutaPort, ratingVal);
        if (creado) ++cargados;
    }

    in.close();
    cout << "Carga finalizada. Albums cargados: " << cargados << ". Total en catalogo: " << catalogoCount << "\n";
    return (cargados > 0);
}

bool Album::guardarCatalogoEnArchivo(const string& rutaArchivo) {
    ofstream out(rutaArchivo.c_str(), ios::trunc);
    if (!out.is_open()) {
        cerr << "No se pudo abrir para escribir: " << rutaArchivo << "\n";
        return false;
    }

    for (int i = 0; i < catalogoCount; ++i) {
        Album* a = catalogo[i];
        if (a != nullptr) {
            out << a->serializarLinea() << "\n";
        }
    }
    out.close();
    return true;
}

// Acceso al catalogo
Album* Album::obtenerPorId(int id) {
    for (int i = 0; i < catalogoCount; ++i) {
        if (catalogo[i] != nullptr && catalogo[i]->getId() == id) return catalogo[i];
    }
    return nullptr;
}

int Album::obtenerPorArtista(int idArtista, Album** out, int maxOut) {
    int found = 0;
    for (int i = 0; i < catalogoCount && found < maxOut; ++i) {
        Album* a = catalogo[i];
        if (a != nullptr && a->getIdArtista() == idArtista) {
            out[found++] = a;
        }
    }
    return found;
}

int Album::obtenerTodos(Album** out, int maxOut) {
    int copied = 0;
    for (int i = 0; i < catalogoCount && copied < maxOut; ++i) {
        out[copied++] = catalogo[i];
    }
    return copied;
}

void Album::limpiarCatalogo() {
    for (int i = 0; i < catalogoCount; ++i) {
        if (catalogo[i] != nullptr) {
            delete catalogo[i];
            catalogo[i] = nullptr;
        }
    }
    catalogoCount = 0;
}
