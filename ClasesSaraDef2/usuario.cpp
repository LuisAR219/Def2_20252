#include "usuario.h"

// Helpers internos

static string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) ++start;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) --end;
    return s.substr(start, end - start);
}

//Constructores / Destructor

Usuario::Usuario()
    : nickname(""), contrasena(""), membresia(0), pais(""), ciudad(""),
    fecha_inscripcion("1970-01-01"), nickname_seguido("-"),
    calidad_audio_kbps(128), reproduccion_activa(false), indice_actual(0),
    repetir_cancion(false), historial_inicio(0), historial_count(0),
    contador_publicidad(0), ultimo_anuncio_mostrado(""),
    canciones_reproducidas_sesion(0)
{
    listas[0] = nullptr;
    listas[1] = nullptr;
    unsigned long long seed = chrono::steady_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed & 0xFFFFFFFFu));
    for (int i = 0; i < MAX_HISTORIAL; ++i) historial[i] = nullptr;
}

Usuario::Usuario(const string& nick, const string& contr, int membresia_tipo,
                 const string& pais_in, const string& ciudad_in,
                 const string& fecha_in, const string& seguido)
    : nickname(nick), contrasena(contr), membresia(membresia_tipo),
    pais(pais_in), ciudad(ciudad_in), fecha_inscripcion(fecha_in),
    nickname_seguido(seguido), reproduccion_activa(false), indice_actual(0),
    repetir_cancion(false), historial_inicio(0), historial_count(0),
    contador_publicidad(0), ultimo_anuncio_mostrado(""),
    canciones_reproducidas_sesion(0)
{
    listas[0] = nullptr;
    listas[1] = nullptr;
    establecerCalidadSegunMembresia();
    unsigned long long seed = chrono::steady_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed & 0xFFFFFFFFu));
    for (int i = 0; i < MAX_HISTORIAL; ++i) historial[i] = nullptr;
}

Usuario::~Usuario() {
    // No borramos listas[1] porque es referencia no propietaria.
    listas[0] = nullptr;
    listas[1] = nullptr;
}

// Utilitarios

bool Usuario::esPremium() const { return membresia == 1; }

void Usuario::establecerCalidadSegunMembresia() {
    calidad_audio_kbps = esPremium() ? 320 : 128;
}

void Usuario::mostrarPerfil() const {
    cout << "Usuario: " << nickname << " | Membresia: " << (esPremium() ? "Premium" : "Estandar")
    << " | Pais: " << pais << " | Ciudad: " << ciudad
    << " | Inscripcion: " << fecha_inscripcion << "\n";
    cout << "Sigue a: " << (nickname_seguido.empty() ? "-" : nickname_seguido) << "\n";
}

string Usuario::serializarLinea() const {
    // nickname|contrasena|membresia|pais|ciudad|fecha|nickname_seguido
    string res = nickname + "|" + contrasena + "|" + to_string(membresia) + "|" + pais + "|" + ciudad + "|" + fecha_inscripcion + "|" + (nickname_seguido.empty() ? "-" : nickname_seguido);
    return res;
}

bool Usuario::validarContrasena(const string& intento) const {
    return contrasena == intento;
}

// Reproducción

Cancion* Usuario::obtenerCancionDesdeFuenteAleatoria(Cancion** fuente, int total) {
    if (fuente == nullptr || total <= 0) return nullptr;
    uniform_int_distribution<int> dist(0, total - 1);
    int idx = dist(rng);
    return fuente[idx];
}

void Usuario::iniciarReproduccion() {
    if (!reproduccion_activa) {
        reproduccion_activa = true;
        canciones_reproducidas_sesion = 0;
    }
}

void Usuario::detenerReproduccion() {
    if (reproduccion_activa) reproduccion_activa = false;
}

void Usuario::reproducirAleatorio(Cancion** fuenteCanciones, int totalCanciones) {
    if (fuenteCanciones == nullptr || totalCanciones <= 0) {
        cout << "[reproducirAleatorio] Fuente vacía.\n";
        return;
    }
    iniciarReproduccion();
    int reproducidas = 0;
    while (reproduccion_activa && reproducidas < K_REPRODUCCIONES_AUTOMATICAS) {
        Cancion* actual = obtenerCancionDesdeFuenteAleatoria(fuenteCanciones, totalCanciones);
        if (actual == nullptr) break;
        // Mostrar ruta y portada (se asume campos públicos)
        cout << "Reproduciendo: " << (actual->ruta_archivo.empty() ? "[ruta_desconocida]" : actual->ruta_archivo)
             << " | Portada: " << (actual->ruta_portada.empty() ? "[portada_desconocida]" : actual->ruta_portada) << "\n";
        registrarCancionEnHistorial(actual);
        if (!esPremium()) verificarYMostrarPublicidad();
        // Espera 3 segundos (busy-wait con chrono, por restricción sin <thread>)
        auto inicio = chrono::steady_clock::now();
        while (chrono::steady_clock::now() - inicio < chrono::seconds(3)) { }
        ++reproducidas;
        ++canciones_reproducidas_sesion;
        if (esPremium() && repetir_cancion) {
            // repetir: no registramos duplicado (ya lo hicimos), volvemos a reproducir la misma hasta desactivar
            continue;
        }
        if (reproducidas >= K_REPRODUCCIONES_AUTOMATICAS) {
            cout << "[reproducirAleatorio] Límite K alcanzado (" << K_REPRODUCCIONES_AUTOMATICAS << ").\n";
            detenerReproduccion();
            break;
        }
    }
}

void Usuario::siguiente() {
    Playlist* activa = listas[0];
    if (listas[1] != nullptr && !nickname_seguido.empty()) {
        if (listas[1]->totalCanciones() > 0) activa = listas[1];
    }
    if (activa == nullptr) { cout << "[siguiente] No hay playlist activa.\n"; return; }
    int total = activa->totalCanciones();
    if (total == 0) { cout << "[siguiente] Playlist vacía.\n"; return; }
    if (indice_actual < total - 1) {
        ++indice_actual;
        Cancion* c = activa->obtenerPorIndice(indice_actual);
        if (c != nullptr) {
            cout << "Siguiente -> " << c->ruta_archivo << " | " << c->ruta_portada << "\n";
            registrarCancionEnHistorial(c);
            verificarYMostrarPublicidad();
        }
    } else {
        cout << "[siguiente] Ya estás al final de la playlist.\n";
    }
}

void Usuario::previa() {
    if (!esPremium()) { cout << "[previa] Operación no permitida para estándar.\n"; return; }
    Cancion* anterior = obtenerPreviaDesdeHistorial(1);
    if (anterior == nullptr) { cout << "[previa] No hay canción previa registrada.\n"; return; }
    cout << "Previa -> " << anterior->ruta_archivo << " | " << anterior->ruta_portada << "\n";
    registrarCancionEnHistorial(anterior);
    verificarYMostrarPublicidad();
}

void Usuario::establecerRepetir(bool activar) {
    if (!esPremium()) { cout << "[repetir] Solo premium.\n"; return; }
    repetir_cancion = activar;
    cout << "[repetir] Modo repetir " << (activar ? "activado" : "desactivado") << ".\n";
}

// Historial

void Usuario::registrarCancionEnHistorial(Cancion* c) {
    if (c == nullptr) return;
    if (historial_count > 0) {
        int last_idx = (historial_inicio + historial_count - 1) % MAX_HISTORIAL;
        if (historial[last_idx] == c) return; // evitar duplicado por repetir
    }
    if (historial_count < MAX_HISTORIAL) {
        int pos = (historial_inicio + historial_count) % MAX_HISTORIAL;
        historial[pos] = c;
        ++historial_count;
    } else {
        // sobrescribir el más antiguo
        historial[historial_inicio] = c;
        historial_inicio = (historial_inicio + 1) % MAX_HISTORIAL;
    }
}

Cancion* Usuario::obtenerPreviaDesdeHistorial(int pasos_atras) const {
    if (pasos_atras <= 0 || pasos_atras > MAX_PREVIAS_PREMIUM) return nullptr;
    if (historial_count == 0) return nullptr;
    if (pasos_atras > historial_count) return nullptr;
    int idx = (historial_inicio + historial_count - pasos_atras) % MAX_HISTORIAL;
    return historial[idx];
}

// Publicidad

void Usuario::verificarYMostrarPublicidad() {
    if (esPremium()) return;
    ++contador_publicidad;
    if (contador_publicidad % 2 == 0) {
        string anuncio = generarAnuncioAleatorio();
        int intentos = 0;
        while (anuncio == ultimo_anuncio_mostrado && intentos < 5) {
            anuncio = generarAnuncioAleatorio();
            ++intentos;
        }
        cout << "[ANUNCIO] " << anuncio << "\n";
        ultimo_anuncio_mostrado = anuncio;
    }
}

string Usuario::generarAnuncioAleatorio() {
    const char* ejemplos[] = {
        "Descubre la nueva coleccion - hasta 50% off!",
        "Prueba ahora Premium: 30 dias gratis y escucha sin anuncios.",
        "Oferta especial: cursos online con certificado.",
        "Concierto en tu ciudad este fin de semana. Compra ya tu entrada!",
        "Promo limitada: 2x1 en cafeterias participantes."
    };
    int n = sizeof(ejemplos) / sizeof(ejemplos[0]);
    uniform_int_distribution<int> dist(0, n - 1);
    int idx = dist(rng);
    string res = ejemplos[idx];
    if ((int)res.size() > 500) res = res.substr(0, 500);
    return res;
}

// Favoritos / seguir

void Usuario::menuFavoritos() {
    if (!esPremium()) { cout << "[menuFavoritos] Acceso denegado: solo premium.\n"; return; }
    cout << "=== Menu Favoritos (usuario: " << nickname << ") ===\n";
    cout << "1) Editar mis favoritos (agregar/quitar por id)\n";
    cout << "2) Seguir otra lista de favoritos (por nickname)\n";
    cout << "3) Ejecutar mis favoritos (orden original o aleatorio)\n";
    cout << "Nota: I/O de edición/selección se debe implementar en la capa UI.\n";
}

void Usuario::editarMisFavoritos() {
    if (!esPremium()) { cout << "[editarMisFavoritos] Acceso denegado.\n"; return; }
    if (!listas[0]) { cout << "[editarMisFavoritos] No existe playlist propia.\n"; return; }
    cout << "[editarMisFavoritos] Delegar edición a Playlist (implementar UI para ID input).\n";
}

bool Usuario::agregarFavoritoPorId(int idCancion) {
    if (!esPremium()) { cout << "[agregarFavoritoPorId] Solo premium.\n"; return false; }
    if (!listas[0]) { cout << "[agregarFavoritoPorId] Playlist propia no existe.\n"; return false; }
    // Dependemos de API Playlist (ej: insertarSiNoExiste(Cancion*) o similar)
    cout << "[agregarFavoritoPorId] Operación delegada a Playlist. Implementar método en Playlist.\n";
    return false;
}

bool Usuario::eliminarFavoritoPorId(int idCancion) {
    if (!esPremium()) { cout << "[eliminarFavoritoPorId] Solo premium.\n"; return false; }
    if (!listas[0]) { cout << "[eliminarFavoritoPorId] Playlist propia no existe.\n"; return false; }
    bool ok = listas[0]->eliminarPorId(idCancion);
    if (ok) cout << "[eliminarFavoritoPorId] Canción eliminada (si existía).\n";
    else cout << "[eliminarFavoritoPorId] No encontrada en favoritos.\n";
    return ok;
}

bool Usuario::seguirA(const string& nickSeguido, Usuario** todosUsuarios, int nUsuarios) {
    if (nickSeguido.empty() || nickSeguido == "-") { cout << "[seguirA] Nick inválido.\n"; return false; }
    Usuario* seguido = buscarUsuarioPorNickname(todosUsuarios, nUsuarios, nickSeguido);
    if (seguido == nullptr) { cout << "[seguirA] Usuario a seguir no encontrado: " << nickSeguido << "\n"; return false; }
    if (!seguido->esPremium()) { cout << "[seguirA] Solo se puede seguir a usuarios premium.\n"; return false; }
    // establecer referencia en vivo
    listas[1] = seguido->listas[0];
    nickname_seguido = nickSeguido;
    // opcional: fusionar canciones en la playlist propia
    if (listas[0] != nullptr && listas[1] != nullptr) {
        int añadidas = listas[0]->fusionarDesde(listas[1]);
        cout << "[seguirA] Se añadieron " << añadidas << " canciones desde " << nickSeguido << ".\n";
    } else {
        cout << "[seguirA] Referencia establecida (comprueba playlists asociadas).\n";
    }
    return true;
}

void Usuario::dejarDeSeguir() {
    listas[1] = nullptr;
    nickname_seguido = "-";
    cout << "[dejarDeSeguir] Dejas de seguir a alguien.\n";
}

void Usuario::ejecutarMisFavoritos(bool aleatorio) {
    if (!esPremium()) { cout << "[ejecutarMisFavoritos] Solo premium.\n"; return; }
    if (!listas[0]) { cout << "[ejecutarMisFavoritos] Playlist propia no existe.\n"; return; }
    int total = listas[0]->totalCanciones();
    if (total == 0) { cout << "[ejecutarMisFavoritos] Lista vacía.\n"; return; }
    iniciarReproduccion();
    int reproducidas = 0;
    if (aleatorio) {
        while (reproduccion_activa && reproducidas < K_REPRODUCCIONES_AUTOMATICAS && reproducidas < total) {
            uniform_int_distribution<int> dist(0, total - 1);
            int idx = dist(rng);
            Cancion* c = listas[0]->obtenerPorIndice(idx);
            if (!c) break;
            cout << "Favoritos (aleatorio) -> " << c->ruta_archivo << " | " << c->ruta_portada << "\n";
            registrarCancionEnHistorial(c);
            verificarYMostrarPublicidad();
            auto inicio = chrono::steady_clock::now();
            while (chrono::steady_clock::now() - inicio < chrono::seconds(3)) { }
            ++reproducidas;
        }
    } else {
        for (int i = 0; i < total && reproducidas < K_REPRODUCCIONES_AUTOMATICAS && reproducidas < total && reproduccion_activa; ++i) {
            Cancion* c = listas[0]->obtenerPorIndice(i);
            if (!c) continue;
            cout << "Favoritos (orden) -> " << c->ruta_archivo << " | " << c->ruta_portada << "\n";
            registrarCancionEnHistorial(c);
            verificarYMostrarPublicidad();
            auto inicio = chrono::steady_clock::now();
            while (chrono::steady_clock::now() - inicio < chrono::seconds(3)) { }
            ++reproducidas;
        }
    }
    detenerReproduccion();
}

int Usuario::fusionarFavoritosDesde(Playlist* otra) {
    if (!esPremium()) { cout << "[fusionarFavoritosDesde] Solo premium.\n"; return 0; }
    if (!listas[0] || !otra) return 0;
    return listas[0]->fusionarDesde(otra);
}

// Utilidades privadas

void Usuario::inicializarRng(unsigned int semilla) { rng.seed(semilla); }

void Usuario::reiniciarEstadoReproduccion() {
    reproduccion_activa = false;
    indice_actual = 0;
    repetir_cancion = false;
    historial_inicio = 0;
    historial_count = 0;
    contador_publicidad = 0;
    ultimo_anuncio_mostrado = "";
    canciones_reproducidas_sesion = 0;
}

bool Usuario::puedeRetroceder() const { return esPremium(); }

// Funciones globales de carga / búsqueda / autenticación

bool cargarUsuariosDesdeFichero(const string& ruta, Usuario*** outUsuarios, int& outCount) {
    ifstream in(ruta.c_str());
    if (!in.is_open()) {
        cout << "[cargarUsuariosDesdeFichero] No se pudo abrir: " << ruta << "\n";
        *outUsuarios = nullptr;
        outCount = 0;
        return false;
    }

    // Primera pasada: contar líneas válidas
    int cuenta = 0;
    string linea;
    while (getline(in, linea)) {
        string t = trim(linea);
        if (!t.empty()) ++cuenta;
    }
    if (cuenta == 0) {
        in.close();
        *outUsuarios = nullptr;
        outCount = 0;
        return true;
    }

    // Reservar array de punteros
    Usuario** arr = new Usuario*[cuenta];
    for (int i = 0; i < cuenta; ++i) arr[i] = nullptr;

    // Segunda pasada: parsear
    in.clear();
    in.seekg(0);
    int idx = 0;
    while (getline(in, linea)) {
        string t = trim(linea);
        if (t.empty()) continue;

        // Split en 7 campos (nickname|contrasena|membresia|pais|ciudad|fecha|nickname_seguido)
        string campos[7];
        int campo = 0;
        size_t prev = 0;
        for (int k = 0; k < 6; ++k) {
            size_t pos = t.find('|', prev);
            if (pos == string::npos) { // línea mal formada: abortar parseo de esta línea
                campo = k; break;
            }
            campos[k] = t.substr(prev, pos - prev);
            prev = pos + 1;
            campo = k + 1;
        }
        // resto = último campo
        if (prev < t.size()) campos[campo] = t.substr(prev);
        for (int i = 0; i <= campo; ++i) campos[i] = trim(campos[i]);

        // Asignar valores con defensas
        string nick = campos[0];
        string contr = (campo >= 1 ? campos[1] : string(""));
        int memb = 0;
        if (campo >= 2) {
            try { memb = (int)std::stol(campos[2]); } catch (...) { memb = 0; }
        }
        string pais = (campo >= 3 ? campos[3] : string(""));
        string ciudad = (campo >= 4 ? campos[4] : string(""));
        string fecha = (campo >= 5 ? campos[5] : string("1970-01-01"));
        string seguido = (campo >= 6 ? campos[6] : string("-"));
        if (seguido.empty()) seguido = "-";

        Usuario* u = new Usuario(nick, contr, memb, pais, ciudad, fecha, seguido);
        arr[idx++] = u;
    }
    in.close();

    *outUsuarios = arr;
    outCount = idx;
    return true;
}

Usuario* buscarUsuarioPorNickname(Usuario** usuarios, int nUsuarios, const string& nick) {
    if (!usuarios || nUsuarios <= 0) return nullptr;
    for (int i = 0; i < nUsuarios; ++i) {
        if (usuarios[i] && usuarios[i]->nickname == nick) return usuarios[i];
    }
    return nullptr;
}

int resolverSeguidos(Usuario** usuarios, int nUsuarios) {
    if (!usuarios || nUsuarios <= 0) return 0;
    int resueltas = 0;
    for (int i = 0; i < nUsuarios; ++i) {
        Usuario* u = usuarios[i];
        if (!u) continue;
        if (u->nickname_seguido.empty() || u->nickname_seguido == "-") {
            u->listas[1] = nullptr;
            continue;
        }
        Usuario* seguido = buscarUsuarioPorNickname(usuarios, nUsuarios, u->nickname_seguido);
        if (!seguido) { u->listas[1] = nullptr; continue; }
        if (!seguido->esPremium()) { u->listas[1] = nullptr; continue; }
        // asignar referencia en vivo
        u->listas[1] = seguido->listas[0];
        ++resueltas;
    }
    return resueltas;
}

Usuario* autenticarUsuario(Usuario** usuarios, int nUsuarios, const string& nick, const string& contrasena) {
    Usuario* u = buscarUsuarioPorNickname(usuarios, nUsuarios, nick);
    if (!u) return nullptr;
    if (u->validarContrasena(contrasena)) return u;
    return nullptr;
}

bool guardarUsuariosAFichero(const string& ruta, Usuario** usuarios, int nUsuarios) {
    ofstream out(ruta.c_str(), ios::trunc);
    if (!out.is_open()) {
        cout << "[guardarUsuariosAFichero] No se pudo abrir: " << ruta << "\n";
        return false;
    }
    for (int i = 0; i < nUsuarios; ++i) {
        if (!usuarios[i]) continue;
        out << usuarios[i]->serializarLinea() << "\n";
    }
    out.close();
    return true;
}
