#include "album.h"

Album* Album::lista[100] = {nullptr};
int Album::cantidad = 0;
long Album::totalIteraciones = 0;
size_t Album::totalMemoria = 0;

Album::Album()
    : nombre(""), id(0), totalGeneros(0),
    fecha(""), sello(""), rutaPortada(""), puntuacion(0) {
    for (int i = 0; i < 4; ++i) generos[i] = "";
    totalMemoria += sizeof(Album);
}

Album::Album(const string& nombre_, int id_, const string generos_[], int totalG,
             const string& fecha_, const string& sello_,
             const string& ruta_, int puntuacion_)
    : nombre(nombre_), id(id_), totalGeneros(totalG),
    fecha(fecha_), sello(sello_), rutaPortada(ruta_), puntuacion(puntuacion_) {
    for (int i = 0; i < totalG && i < 4; ++i) generos[i] = generos_[i];
    for (int i = totalG; i < 4; ++i) generos[i] = "";
    totalMemoria += sizeof(Album);
}

Album::~Album() {
    totalMemoria -= sizeof(Album);
}

string Album::getNombre() const { return nombre; }
int Album::getId() const { return id; }
string Album::getGenero(int i) const {
    if (i >= 0 && i < totalGeneros) return generos[i];
    return "";
}
int Album::getTotalGeneros() const { return totalGeneros; }
string Album::getFecha() const { return fecha; }
string Album::getSello() const { return sello; }
string Album::getRutaPortada() const { return rutaPortada; }
int Album::getPuntuacion() const { return puntuacion; }

void Album::mostrarAlbum() const {
    cout << "-----------------------------------------" << endl;
    cout << "Álbum: " << nombre << endl;
    cout << "ID: " << id << endl;
    cout << "Géneros: ";
    for (int i = 0; i < totalGeneros; ++i) {
        cout << generos[i];
        if (i < totalGeneros - 1) cout << ", ";
    }
    cout << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Sello: " << sello << endl;
    cout << "Puntuación: " << puntuacion << endl;
    cout << "Ruta de portada: " << rutaPortada << endl;
    cout << "-----------------------------------------" << endl;
}

bool Album::cargarDesdeArchivo(const string& ruta) {
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de álbumes: " << ruta << endl;
        return false;
    }

    string linea;
    int contador = 0;
    totalIteraciones++;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        totalIteraciones++;

        size_t pos = 0;
        string campos[7];
        for (int i = 0; i < 7; ++i) {
            size_t separador = linea.find('|', pos);
            if (separador == string::npos) {
                campos[i] = linea.substr(pos);
                break;
            } else {
                campos[i] = linea.substr(pos, separador - pos);
                pos = separador + 1;
            }
            totalIteraciones++;
        }

        string nombre = campos[0];
        int id = stoi(campos[1]);

        string generos_[4];
        int totalG = 0;
        size_t gpos = 0;
        while (totalG < 4 && gpos < campos[2].size()) {
            size_t coma = campos[2].find(',', gpos);
            if (coma == string::npos) coma = campos[2].size();
            generos_[totalG++] = campos[2].substr(gpos, coma - gpos);
            gpos = coma + 1;
            totalIteraciones++;
        }

        string fecha = campos[3];
        string sello = campos[4];
        string rutaPortada = campos[5];
        int puntuacion = stoi(campos[6]);

        if (id <= 0 || puntuacion < 0 || puntuacion > 10) {
            cout << "Línea inválida, se omite: " << linea << endl;
            continue;
        }

        if (contador >= 100) {
            cout << "Límite máximo de álbumes alcanzado (100)." << endl;
            break;
        }

        lista[contador] = new Album(nombre, id, generos_, totalG, fecha, sello, rutaPortada, puntuacion);
        contador++;
    }

    archivo.close();
    cantidad = contador;
    cout << "Se cargaron " << cantidad << " álbum(es) desde " << ruta << endl;
    return true;
}

void Album::imprimirTodos() {
    for (int i = 0; i < cantidad; ++i) {
        if (lista[i] != nullptr) lista[i]->mostrarAlbum();
        totalIteraciones++;
    }
}

int Album::getCantidad() { return cantidad; }

void Album::liberarMemoria() {
    for (int i = 0; i < cantidad; ++i) {
        if (lista[i]) {
            delete lista[i];
            lista[i] = nullptr;
            totalIteraciones++;
        }
    }
    cantidad = 0;
    cout << "Memoria liberada para álbumes.\n";
}

void Album::mostrarMetricas() {
    cout << "\n=== MÉTRICAS DE ÁLBUMES ===" << endl;
    cout << "Total iteraciones: " << totalIteraciones << endl;
    cout << "Memoria estimada: " << totalMemoria << " bytes" << endl;
    cout << "============================" << endl;
}

long Album::getTotalIteraciones() { return totalIteraciones; }
size_t Album::getTotalMemoria() { return totalMemoria; }
