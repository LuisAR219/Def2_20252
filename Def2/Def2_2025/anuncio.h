#ifndef ANUNCIO_H
#define ANUNCIO_H

#include <string>

class anuncio {
private:
    int id;
    std::string categoria;
    std::string mensaje;

    static anuncio* anuncios[50];
    static bool anunciosUsados[50];
    static int anunciosDisponibles;
    static bool anunciosCargados;
    static int totalAnunciosCargados;
    static int semillaAleatoria;

    static int totalIteraciones;
    static size_t totalMemoria;

    static int contadorAAA;
    static int contadorB;
    static int contadorC;

public:
    anuncio();
    anuncio(int id, const std::string& categoria, const std::string& mensaje);
    ~anuncio();

    int getId() const;
    std::string getCategoria() const;
    std::string getMensaje() const;

    void setId(int id);
    void setCategoria(const std::string& categoria);
    void setMensaje(const std::string& mensaje);

    static bool cargarAnunciosDesdeTXT();
    static anuncio* obtenerAnuncioAleatorio();
    static void resetearAnuncios();
    static int getAnunciosDisponibles();
    static int getTotalAnunciosCargados();

    static int getContadorAAA();
    static int getContadorB();
    static int getContadorC();

    static void mostrarMetricasFinales();
    static int getTotalIteraciones();
    static size_t getTotalMemoria();

private:
    static void inicializarArregloUsados();
    static int generarNumeroAleatorio(int min, int max);
    static int contarLineasArchivo();
    static void parsearLinea(const std::string& linea, int& id, std::string& categoria, std::string& mensaje);

    static int calcularPesoCategoria(const std::string& categoria);
    static size_t calcularMemoriaString(const std::string& str);
    static size_t calcularMemoriaAnuncio(const anuncio* anuncio);
};

#endif // ANUNCIO_H

