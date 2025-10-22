TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        anuncio.cpp \
        cancion.cpp \
        cargarcanciones.cpp \
        main.cpp \
        personacredito.cpp \
        playlist.cpp

HEADERS += \
    anuncio.h \
    cancion.h \
    personacredito.h \
    playlist.h
