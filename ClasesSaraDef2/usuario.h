#ifndef USUARIO_H
#define USUARIO_H

#pragma once

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;


class Cancion;
class Playlist;

class Usuario {
public:
    // Constantes del sistema
    static const int LIMITE_FAVORITOS = 10000;
    static const int K_REPRODUCCIONES_AUTOMATICAS = 5;
    static const int M_FAVORITOS_RETROCESO = 6;
    static const int MAX_HISTORIAL = 10;
    static const int MAX_PREVIAS_PREMIUM = 4;

    // Datos del archivo de texto
    string nickname;
    string contrasena;        // contraseña en texto plano según fichero
    int membresia;            // 1 = premium, 0 = estandar
    string pais;
    string ciudad;
    string fecha_inscripcion; // "YYYY-MM-DD"
    string nickname_seguido;  // '-' o "" => no sigue a nadie

    // Playlists y comportamiento
    int calidad_audio_kbps;   // 128 o 320
    Playlist* listas[2];      // [0] propia (owner), [1] playlist seguida (referencia o nullptr)

    // Estado de reproducción por usuario (sesión)
    bool reproduccion_activa;
    int indice_actual;
    bool repetir_cancion;
    Cancion* historial[MAX_HISTORIAL];
    int historial_inicio;
    int historial_count;

    // Publicidad (para estándar)
    int contador_publicidad;
    string ultimo_anuncio_mostrado;

    int canciones_reproducidas_sesion;

    // RNG local
    mt19937 rng;

    // Constructores / destructor
    Usuario();
    Usuario(const string& nick, const string& contr, int membresia_tipo,
            const string& pais_in, const string& ciudad_in,
            const string& fecha_in, const string& seguido = "-");
    ~Usuario();

    // Utilitarios
    bool esPremium() const;
    void establecerCalidadSegunMembresia();
    void mostrarPerfil() const;
    string serializarLinea() const; // para guardar en fichero

    // Autenticación
    bool validarContrasena(const string& intento) const;

    // Reproducción
    void reproducirAleatorio(Cancion** fuenteCanciones, int totalCanciones);
    void iniciarReproduccion();
    void detenerReproduccion();
    void siguiente();
    void previa();
    void establecerRepetir(bool activar);

    // Historial
    void registrarCancionEnHistorial(Cancion* c);
    Cancion* obtenerPreviaDesdeHistorial(int pasos_atras) const;

    // Publicidad
    void verificarYMostrarPublicidad();
    string generarAnuncioAleatorio();

    // Favoritos y seguir (operan sobre listas[0])
    void menuFavoritos();
    void editarMisFavoritos();
    bool agregarFavoritoPorId(int idCancion);
    bool eliminarFavoritoPorId(int idCancion);

    bool seguirA(const string& nicknameSeguido, Usuario** todosUsuarios, int nUsuarios); // resuelve usando el array dado
    void dejarDeSeguir();

    void ejecutarMisFavoritos(bool aleatorio);
    int fusionarFavoritosDesde(Playlist* otra);

    // RNG util
    void inicializarRng(unsigned int semilla);

private:
    // helpers privados
    void reiniciarEstadoReproduccion();
    Cancion* obtenerCancionDesdeFuenteAleatoria(Cancion** fuente, int total);
    bool puedeRetroceder() const;

    // no permitir copias triviales
    Usuario(const Usuario& otro) = delete;
    Usuario& operator=(const Usuario& otro) = delete;
};


// Funciones de carga / búsqueda / autenticación

// Carga usuarios desde archivo. Devuelve true si carga correctamente.
// outUsuarios será un array dinámico de Usuario* (allocated with new[] of pointers) y outCount la cantidad.
// El caller es responsable de liberar cada Usuario* y luego delete[] del array.
bool cargarUsuariosDesdeFichero(const string& ruta, Usuario*** outUsuarios, int& outCount);

// Resuelve 'nickname_seguido' para cada usuario: asigna listas[1] = seguido->listas[0] si existe y es premium.
// Retorna cantidad de relaciones resueltas.
int resolverSeguidos(Usuario** usuarios, int nUsuarios);

// Búsqueda lineal por nickname (devuelve pointer o nullptr).
Usuario* buscarUsuarioPorNickname(Usuario** usuarios, int nUsuarios, const string& nick);

// Autenticar usuario por nickname y contraseña (devuelve Usuario* o nullptr)
Usuario* autenticarUsuario(Usuario** usuarios, int nUsuarios, const string& nick, const string& contrasena);

// Guardar usuarios de vuelta al fichero (reescribe fichero completo). Retorna true si ok.
bool guardarUsuariosAFichero(const string& ruta, Usuario** usuarios, int nUsuarios);


#endif // USUARIO_H
