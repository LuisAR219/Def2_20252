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
};

#endif // ANUNCIO_H
