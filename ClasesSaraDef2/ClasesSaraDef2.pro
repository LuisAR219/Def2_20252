TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        anuncio.cpp \
        artista.cpp \
        cancion.cpp \
        cargarcanciones.cpp \
        funcionalidades.cpp \
        main.cpp \
        personacredito.cpp \
        playlist.cpp \
        usuario.cpp

HEADERS += \
    album.h \
    anuncio.h \
    artista.h \
    cancion.h \
    cargarcancion.h \
    cargarcanciones.h \
    personacredito.h \
    playlist.h \
    usuario.h
