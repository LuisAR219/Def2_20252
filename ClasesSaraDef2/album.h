#ifndef ALBUM_H
#define ALBUM_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

// Límites
#define MAX_ALBUMS 1000
#define MAX_CANCIONES_POR_ALBUM 500

class Album {
private:
    // Metadatos
    int id;
    string titulo;
    int idArtista;
    string generosCSV;
    string fechaLanzamiento;
    string sello;
    string rutaPortada;
    int rating;

    // IDs de canciones (gestión manual de arreglo)
    int* idsCanciones;
    int capacidadIds;
    int cantidadIds;

    // Constructor privado usado por loader
    Album(int id,
          const string& titulo,
          int idArtista,
          const string& generosCSV,
          const string& fechaLanzamiento,
          const string& sello,
          const string& rutaPortada,
          int rating);

public:
    // Destructor
    ~Album();

    // Getters
    int getId() const;
    string getTitulo() const;
    int getIdArtista() const;
    string getGenerosCSV() const;
    string getFechaLanzamiento() const;
    string getSello() const;
    string getRutaPortada() const;
    int getRating() const;
    // Devuelve puntero al arreglo interno de ids (lectura)
    const int* getIdsCanciones(int &outCantidad) const;

    // Setters simples
    void setTitulo(const string& t);
    void setSello(const string& s);
    void setRutaPortada(const string& r);
    void setFechaLanzamiento(const string& f);
    void setRating(int r);

    // Gestión de ids de canciones
    bool agregarIdCancion(int idCancion);   // evita duplicados
    bool eliminarIdCancion(int idCancion);  // true si eliminó
    bool contieneCancion(int idCancion) const;
    int cantidadCanciones() const;

    // Serializar (no escribe a disco por sí mismo)
    string serializarLinea() const; // formato: id|titulo|idArtista|generosCSV|fecha|sello|ruta|rating

    // Mostrar
    void mostrarResumen() const;

    // -----------------------
    // Funciones estáticas para gestionar catálogo (I/O dentro de la clase)
    // -----------------------
    static bool cargarDesdeArchivo(const string& rutaArchivo);
    static bool guardarCatalogoEnArchivo(const string& rutaArchivo);

    // Acceso al catálogo
    static Album* obtenerPorId(int id);
    // obtenerPorArtista llena el array 'out' hasta 'maxOut' y retorna cuantos encontró
    static int obtenerPorArtista(int idArtista, Album** out, int maxOut);
    static int obtenerTodos(Album** out, int maxOut); // devuelve cantidad

    static void limpiarCatalogo();

private:
    // Catálogo estático (única clase responsable de memoria)
    static Album* catalogo[MAX_ALBUMS];
    static int catalogoCount;

    // Helpers de parsing y strings (implementados manualmente)
    static string trim(const string& s);
    static int findChar(const string& s, char c, int start);
    static string substring(const string& s, int start, int length);
    static bool isDigits(const string& s);

    // crear y registrar (interno)
    static Album* crearYRegistrar(int id,
                                  const string& titulo,
                                  int idArtista,
                                  const string& generosCSV,
                                  const string& fechaLanzamiento,
                                  const string& sello,
                                  const string& rutaPortada,
                                  int rating);
};

#endif // ALBUM_H

