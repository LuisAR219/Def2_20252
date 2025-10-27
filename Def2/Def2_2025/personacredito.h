#ifndef PERSONACREDITO_H
#define PERSONACREDITO_H

#include <fstream>

class PersonaCredito {
private:
    static const int STATIC_SIZE = 5;
    static const int DYNAMIC_SIZE = 10;

    struct Persona {
        char* nombre;
        char* codigo;
        char* rol;
    };

    Persona*** productores;
    Persona*** compositores;
    Persona*** musicos;

    int* sizesProductores;
    int* sizesCompositores;
    int* sizesMusicos;

    int totalProductores;
    int totalCompositores;
    int totalMusicos;

public:
    PersonaCredito();
    ~PersonaCredito();

    bool cargarDesdeArchivo(int& iteraciones, long& memoria);
    void mostrarCreditos(int& iteraciones, long& memoria) const;
    int getTotalProductores() const;
    int getTotalCompositores() const;
    int getTotalMusicos() const;
    const char* getNombreProductor(int index) const;
    const char* getNombreCompositor(int index) const;
    const char* getNombreMusico(int index) const;
    const char* getCodigoProductor(int index) const;
    const char* getCodigoCompositor(int index) const;
    const char* getCodigoMusico(int index) const;
    long calcularMemoria() const;
    bool agregarPersona(const char* nombre, const char* codigo, const char* rol, int& iteraciones, long& memoria);

private:
    bool personaExiste(const char* codigo, int& iteraciones, long& memoria) const;
    void liberarMemoriaPersona(Persona& persona);
    void liberarArreglo(Persona***& arreglo, int*& sizes);
    char* copiarCadena(const char* origen);
    int encontrarBloqueDisponible(Persona*** arreglo, int* sizes, int& iteraciones) const;
};

#endif
