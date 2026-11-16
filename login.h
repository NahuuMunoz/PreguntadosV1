#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

#define ARCHIVO_JUGADORES "jugadores.dat"

typedef struct
{
    int ID;
    char nombreJugador[50];
    char nombreUsuario[50];
    char claveUsuario[50];
    int puntaje;
    int partidasJugadas;
} Jugador;

int obtenerSiguienteID(const char archivo[]);
int buscarJugadorEnArchivo(const char archivo[], char nombreBuscado[]);
Jugador crearJugador(const char archivo[]);
void guardarJugadorEnArchivo(const char archivo[], Jugador nuevoJugador);
int validarLogin(const char archivo[], char usuarioIngresado[], char claveIngresada[]);
int actualizarEstadisticasJugador(const char archivo[], int idJugador, int puntajeObtenido);

#endif // LOGIN_H_INCLUDED
