#ifndef RANKING_H
#define RANKING_H

#define ARCHIVO_PARTIDAS "partidas.dat"
#define ARCHIVO_HISTORICO "ranking_historico.dat"
#define TAM_JUGADOR 30

typedef struct
{
    int idPartida;
    int idJugador;
    char nombreJugador[TAM_JUGADOR];
    int modo;
    int puntajeObtenido;
    int respuestasCorrectas;
    int rachaMaximaPartida;
    int activo;
} Partida;

typedef struct
{
    int idJugador;
    char nombreJugador[TAM_JUGADOR];
    int puntajeHistorico;
    int partidasJugadas;
    int mejorRachaHistorica;
    int activo;
} RankingHistorico;

// Funciones de Partidas
long cantidadPartidasGuardadas(const char nombreArchivo[]);
int guardarPartidaArchivo(const char nombreArchivo[], Partida nueva);
void ordenarPartidasSeleccion(Partida partidas[], int validos);
void mostrarRankingPartidas(const char nombreArchivo[]);
void mostrarPartidasDeUnJugador(const char nombreArchivo[], int idJugador);

// Funciones de Ranking Historico y Escalado de Posiciones
long cantidadJugadoresHistorico(const char nombreArchivo[]);
void ordenarHistoricoSeleccion(RankingHistorico lista[], int validos);
int obtenerPuestoEnRanking(const char nombreArchivo[], int idJugador);
int registrarPuntajeYEscalar(const char nombreArchivo[], Partida partidaJugada, int *puestoAnterior, int *puestoNuevo);
void mostrarTablaHistoricaRecursiva(const RankingHistorico lista[], int validos, int indice);
void mostrarRankingHistorico(const char nombreArchivo[]);

#endif
