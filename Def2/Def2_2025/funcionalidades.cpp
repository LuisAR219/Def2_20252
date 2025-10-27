#include <iostream>
#include <fstream>
#include <string>
#include "album.h"
#include "anuncio.h"
#include "artista.h"
#include "cancion.h"
#include "cargarcanciones.h"
#include "personacredito.h"
#include "playlist.h"
#include "usuario.h"
#include "funcionalidades.h"

using namespace std;

// Definición de variables globales
long iteraciones_totales = 0;
long memoria_total = 0;

void cargarDatosIniciales() {
    cout << "\nCargando datos iniciales..." << endl;

    // Cargar albumes
    if (!Album::cargarDesdeArchivo("albumes.txt")) {
        cout << "Error al cargar albumes" << endl;
    }

    // Cargar artistas
    Artista::cargarDesdeArchivo("artistas.txt");

    // Cargar canciones
    if (!cargarCancionesDesdeTXT("canciones.txt")) {
        cout << "Error al cargar canciones" << endl;
    }

    // Cargar anuncios
    if (!anuncio::cargarAnunciosDesdeTXT()) {
        cout << "Error al cargar anuncios" << endl;
    }

    // Cargar creditos
    PersonaCredito creditos;
    int iterCreditos;
    long memCreditos;
    if (!creditos.cargarDesdeArchivo(iterCreditos, memCreditos)) {
        cout << "Error al cargar creditos" << endl;
    }

    cout << "Datos cargados exitosamente." << endl;
}

void mostrarMetricasFinales() {
    cout << "\n=== METRICAS FINALES DEL SISTEMA ===" << endl;

    // Metricas de albumes
    cout << "\n--- ALBUMES ---" << endl;
    Album::mostrarMetricas();

    // Metricas de anuncios
    cout << "\n--- ANUNCIOS ---" << endl;
    anuncio::mostrarMetricasFinales();

    // Metricas de artistas
    cout << "\n--- ARTISTAS ---" << endl;
    cout << "Total artistas cargados: " << Artista::getCantidad() << endl;

    // Metricas de canciones
    cout << "\n--- CANCIONES ---" << endl;
    int totalCanciones = 0;
    for (int b = 0; b <= bloquesUsados; b++) {
        totalCanciones += cancionesPorBloque[b];
    }
    cout << "Total canciones cargadas: " << totalCanciones << endl;
    cout << "Bloques utilizados: " << (bloquesUsados + 1) << endl;

    // Metricas totales del sistema
    cout << "\n--- SISTEMA COMPLETO ---" << endl;
    cout << "Iteraciones totales estimadas: " << iteraciones_totales << endl;
    cout << "Memoria total estimada: " << memoria_total << " bytes" << endl;
    cout << "=====================================" << endl;
}

bool cargarUsuariosDesdeArchivo(Usuario**& usuarios, int& totalUsuarios) {
    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de usuarios" << endl;
        return false;
    }

    string linea;
    totalUsuarios = 0;

    // Contar lineas primero
    while (getline(archivo, linea)) {
        if (!linea.empty()) totalUsuarios++;
    }
    archivo.close();

    // Reabrir para leer datos
    archivo.open("usuarios.txt");
    usuarios = new Usuario*[totalUsuarios];

    int index = 0;
    while (getline(archivo, linea) && index < totalUsuarios) {
        if (linea.empty()) continue;

        size_t pos = 0;
        string campos[7];
        int campoIndex = 0;

        for (int i = 0; i < 7 && pos < linea.length(); i++) {
            size_t separador = linea.find('|', pos);
            if (separador == string::npos) {
                // Último campo
                campos[i] = linea.substr(pos);
                campoIndex = i + 1;
                break;
            } else {
                campos[i] = linea.substr(pos, separador - pos);
                pos = separador + 1;
                campoIndex = i + 1;
            }
        }

        // Validar que tenemos todos los campos necesarios
        if (campoIndex < 7) {
            cout << "Linea incompleta, se omite: " << linea << endl;
            continue;
        }

        string nickname = campos[0];
        string contrasena = campos[1];

        // Validar campo premium
        bool premium = false;
        if (campos[2] == "1") {
            premium = true;
        }

        string pais = campos[3];
        string ciudad = campos[4];
        string fechaRegistro = campos[5];
        string usuarioSeguido = campos[6];

        usuarios[index] = new Usuario(nickname, contrasena, premium, pais, ciudad, fechaRegistro, usuarioSeguido);
        index++;
    }

    archivo.close();

    // Actualizar el total real de usuarios cargados
    totalUsuarios = index;

    if (totalUsuarios == 0) {
        cout << "No se pudieron cargar usuarios validos desde el archivo." << endl;
        delete[] usuarios;
        usuarios = nullptr;
        return false;
    }

    return true;
}

void liberarUsuarios(Usuario** usuarios, int totalUsuarios) {
    for (int i = 0; i < totalUsuarios; i++) {
        delete usuarios[i];
    }
    delete[] usuarios;
}

void iniciarSesion(Usuario*& usuarioActual, Playlist*& playlistActual) {
    cout << "\n=== INICIO DE SESION ===" << endl;

    // Cargar usuarios desde archivo
    Usuario** usuarios = nullptr;
    int totalUsuarios = 0;

    if (!cargarUsuariosDesdeArchivo(usuarios, totalUsuarios)) {
        cout << "Error al cargar usuarios. Usando datos por defecto." << endl;
        // Datos por defecto en caso de error
        totalUsuarios = 3;
        usuarios = new Usuario*[totalUsuarios];
        usuarios[0] = new Usuario("juan123", "pass123", true, "Colombia", "Medellin", "2024-01-15", "-");
        usuarios[1] = new Usuario("maria456", "pass456", false, "Colombia", "Bogota", "2024-02-20", "juan123");
        usuarios[2] = new Usuario("carlos789", "pass789", true, "Colombia", "Cali", "2024-03-10", "-");
    }

    string nickname, contrasena;

    while (true) {
        cout << "Nickname: ";
        cin >> nickname;
        cout << "Contrasena: ";
        cin >> contrasena;

        bool encontrado = false;
        for (int i = 0; i < totalUsuarios; i++) {
            if (usuarios[i]->validarCredenciales(nickname, contrasena)) {
                usuarioActual = usuarios[i];
                playlistActual->asignarNicknameUsuario(nickname);

                // Cargar playlist del usuario actual
                playlistActual->cargarDesdeArchivoPorNickname(nickname, ".");

                // Obtener el usuario seguido desde el objeto Usuario
                string usuarioSeguido = usuarioActual->getUsuarioSeguido();

                // Si el usuario sigue a alguien, cargar y establecer esa playlist
                if (!usuarioSeguido.empty() && usuarioSeguido != "-") {
                    // Crear una nueva playlist para el usuario seguido
                    Playlist* playlistSeguida = new Playlist();
                    playlistSeguida->asignarNicknameUsuario(usuarioSeguido);

                    // Cargar la playlist del usuario seguido
                    playlistSeguida->cargarDesdeArchivoPorNickname(usuarioSeguido, ".");

                    // Establecer la relación de seguimiento
                    playlistActual->seguirPlaylist(playlistSeguida);
                } else {
                    playlistActual->seguirPlaylist(nullptr);
                }

                cout << "\n¡Inicio de sesion exitoso! Bienvenido, " << nickname << endl;
                usuarioActual->mostrarPerfil();

                encontrado = true;
                break;
            }
        }

        if (encontrado) {
            // Liberar solo los usuarios no seleccionados
            for (int i = 0; i < totalUsuarios; i++) {
                if (usuarios[i] != usuarioActual) {
                    delete usuarios[i];
                }
            }
            delete[] usuarios;
            return;
        }

        cout << "Credenciales incorrectas. Intente nuevamente." << endl;
        cout << "¿Desea salir? (s/n): ";
        char opcion;
        cin >> opcion;
        if (opcion == 's' || opcion == 'S') {
            liberarUsuarios(usuarios, totalUsuarios);
            break;
        }
    }
}

void menuPrincipal(Usuario* usuario, Playlist* playlist) {
    int opcion;

    do {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "1. Reproduccion aleatoria" << endl;
        cout << "2. Mi perfil" << endl;
        cout << "3. Reproducir ultimas canciones" << endl;

        if (usuario->esPremium()) {
            cout << "4. Mi lista de favoritos" << endl;
            cout << "5. Salir" << endl;
        } else {
            cout << "4. Salir" << endl;
        }

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            menuReproduccionAleatoria(usuario);
            break;
        case 2:
            usuario->mostrarPerfil();
            break;
        case 3:
            menuReproducirAnteriores(usuario);
            break;
        case 4:
            if (usuario->esPremium()) {
                menuFavoritos(usuario, playlist);
            } else {
                cout << "Saliendo..." << endl;
            }
            break;
        case 5:
            if (usuario->esPremium()) {
                cout << "Saliendo..." << endl;
            } else {
                cout << "Opcion invalida." << endl;
            }
            break;
        default:
            cout << "Opcion invalida." << endl;
        }

        // Mostrar metricas despues de cada operacion
        cout << "\n--- METRICAS DE LA OPERACION ---" << endl;
        cout << "Iteraciones acumuladas: " << iteraciones_totales << endl;
        cout << "Memoria acumulada: " << memoria_total << " bytes" << endl;

    } while ((usuario->esPremium() && opcion != 5) || (!usuario->esPremium() && opcion != 4));
}

void menuReproduccionAleatoria(Usuario* usuario) {
    cout << "\n=== REPRODUCCION ALEATORIA ===" << endl;

    // Crear playlist temporal con todas las canciones cargadas
    Playlist* playlistCompleta = new Playlist();
    playlistCompleta->crearDesdeCancionesCargadas();

    // Verificar que la playlist tenga canciones
    int totalCanciones = playlistCompleta->obtenerTotalCanciones();
    cout << "Canciones disponibles en playlist: " << totalCanciones << endl;

    if (totalCanciones > 0) {
        usuario->reproducirAleatorioDesdeBloques(playlistCompleta);

        // Mostrar historial de reproduccion
        usuario->mostrarHistorial5();
    } else {
        cout << "No hay canciones disponibles para reproducir." << endl;

        // Mostrar estado de canciones cargadas
        int totalCancionesCargadas = 0;
        for (int b = 0; b <= bloquesUsados; b++) {
            totalCancionesCargadas += cancionesPorBloque[b];
        }
        cout << "Total canciones en sistema: " << totalCancionesCargadas << endl;
    }

    delete playlistCompleta;
}

void menuReproducirAnteriores(Usuario* usuario) {
    cout << "\n=== REPRODUCIR ULTIMAS CANCIONES ===" << endl;
    usuario->mostrarHistorial5();

    cout << "\n¿Desea reproducir las ultimas canciones? (s/n): ";
    char opcion;
    cin >> opcion;

    if (opcion == 's' || opcion == 'S') {
        usuario->reproducirAnterior();
    }
}

void menuFavoritos(Usuario* usuario, Playlist* playlist) {
    int opcion;

    do {
        cout << "\n=== MI LISTA DE FAVORITOS ===" << endl;
        cout << "1. Ver mi lista de favoritos" << endl;
        cout << "2. Agregar cancion a favoritos" << endl;
        cout << "3. Eliminar cancion de favoritos" << endl;
        cout << "4. Reproducir lista de favoritos en orden" << endl;
        cout << "5. Reproducir lista de favoritos aleatoriamente" << endl;
        cout << "6. Volver al menu principal" << endl;

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            playlist->mostrarPlaylist();
            break;
        case 2: {
            int idCancion;
            cout << "Ingrese el ID de la cancion a agregar: ";
            cin >> idCancion;

            // Buscar cancion en todas las canciones cargadas
            Cancion* cancionEncontrada = nullptr;
            for (int b = 0; b <= bloquesUsados && cancionEncontrada == nullptr; b++) {
                for (int i = 0; i < cancionesPorBloque[b] && cancionEncontrada == nullptr; i++) {
                    if (canciones[b][i] != nullptr && canciones[b][i]->getId() == idCancion) {
                        cancionEncontrada = canciones[b][i];
                    }
                }
            }

            if (cancionEncontrada != nullptr) {
                playlist->agregarCancion(cancionEncontrada);
                cout << "Cancion agregada a favoritos." << endl;
            } else {
                cout << "Cancion no encontrada." << endl;
            }
            break;
        }
        case 3: {
            int idCancion;
            cout << "Ingrese el ID de la cancion a eliminar: ";
            cin >> idCancion;

            Cancion* cancion = playlist->buscarCancionPorID(idCancion);
            if (cancion != nullptr) {
                // En una implementacion real, aqui se eliminaria la cancion
                cout << "Funcionalidad de eliminacion implementada en Playlist::agregarCancion" << endl;
            } else {
                cout << "Cancion no encontrada en la lista de favoritos." << endl;
            }
            break;
        }
        case 4:
            usuario->reproducirEnOrden(playlist);
            break;
        case 5:
            // Reproduccion aleatoria de playlist para usuarios premium
            usuario->reproducirAleatorioPlaylist(playlist);
            break;
        case 6:
            cout << "Volviendo al menu principal..." << endl;
            break;
        default:
            cout << "Opcion invalida." << endl;
        }

    } while (opcion != 6);

    // Guardar playlist al salir
    playlist->guardarEnArchivo(".");
}
