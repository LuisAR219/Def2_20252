#ifndef ARTISTA_H
#define ARTISTA_H

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

using namespace std;

class Artista {
private:
    int id;
    string nombre;
    int edad;
    string pais;
    long seguidores;
    int posicionTendenciaGlobal; // campo 6: posición en tendencia global (1 = top, etc.)

    // almacenamiento interno (estático, dentro de la clase)
    static Artista* lista;
    static size_t capacidad;
    static size_t cantidad;

    // constructor auxiliar para inicializar desde datos
    void init(int id, const string& nombre, int edad,
              const string& pais, long seguidores, int posicion);

    // redimensiona el arreglo estático cuando hace falta
    static void asegurarCapacidad();

public:
    // constructores
    Artista();
    Artista(int id, const string& nombre, int edad,
            const string& pais, long seguidores, int posicion);

    // getters
    int getId() const;
    const string& getNombre() const;
    int getEdad() const;
    const string& getPais() const;
    long getSeguidores() const;
    int getPosicionTendenciaGlobal() const;

    // representación
    void print() const;
    string toLine() const; // formato id|nombre|edad|pais|seguidores|posicion

    // función que lee/parsea y almacena TODO dentro de la clase (tratamiento + almacenamiento)
    static void cargarDesdeArchivo(const string& ruta);

    // acceso estático a los datos almacenados
    static const Artista* getLista();
    static size_t getCantidad();

    // opciones utilitarias
    static void imprimirTodos();      // imprime todos los artistas
    static void liberarMemoria();     // liberar la memoria dinámica (si se desea)
};

#endif // ARTISTA_H

