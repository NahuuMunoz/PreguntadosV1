#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "multiple_choice.h"
#include "ranking.h"

void partidaNueva(int idJugador, const char nombreJugador[]);

int main(void) {
    int idLogueado = 1;
    char nombreLogueado[40] = "JugadorDePrueba";
    int opcion;

    srand((unsigned int) time(NULL));

    inicializarBancoMC(ARCHIVO_MC);

    printf("=== Aca iria el login de tu amigo ===\n");
    printf("Simulamos que te logueaste como ID %d, Nombre: %s\n\n", idLogueado, nombreLogueado);

    do {
        printf("======= MENU PREGUNTADOS =======\n");
        printf("1. Jugar Partida (Multiple Choice)\n");
        printf("2. Ver Ranking de Mejores Partidas\n");
        printf("3. Ver Ranking Historico (Podio)\n");
        printf("4. Ver Banco de Preguntas\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                partidaNueva(idLogueado, nombreLogueado);
                break;
            case 2:
                mostrarRankingPartidas(ARCHIVO_PARTIDAS);
                break;
            case 3:
                mostrarRankingHistorico(ARCHIVO_HISTORICO);
                break;
            case 4:
                listarBancoMC(ARCHIVO_MC);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opcion != 0);

    return 0;
}

void partidaNueva(int idJugador, const char nombreJugador[]) {
    Categoria cats[5] = {
        {1, "Deportes"}, {2, "Historia"}, {3, "Geografia"}, {4, "Ciencia"}, {5, "Programacion"}
    };
    int indicesCats[5] = {0, 1, 2, 3, 4};
    int puntajeTotal = 0;
    int c;
    int puestoAntes = -1;
    int puestoNuevo = -1;

    mezclar_indices(indicesCats, 5);

    printf("\n>>> COMIENZA LA PARTIDA <<<\n");

    for (c = 0; c < 3; c++) {
        Categoria catElegida = cats[indicesCats[c]];
        PreguntaMC preguntasArchivo[5];

        int cargadas = cargarMCPorCategoria(ARCHIVO_MC, catElegida.idCategoria, preguntasArchivo, 5);

        if (cargadas > 0) {
            puntajeTotal += jugar_categoria_mc(catElegida, preguntasArchivo, 2, cargadas);
        }
    }

    printf("\n=== FIN DEL JUEGO! PUNTAJE FINAL: %d ===\n", puntajeTotal);

    Partida nuevaPartida;
    nuevaPartida.idPartida = (int)cantidadPartidasGuardadas(ARCHIVO_PARTIDAS) + 1;
    nuevaPartida.idJugador = idJugador;
    strcpy(nuevaPartida.nombreJugador, nombreJugador);
    nuevaPartida.modo = 1;
    nuevaPartida.puntajeObtenido = puntajeTotal;
    nuevaPartida.respuestasCorrectas = 0;
    nuevaPartida.rachaMaximaPartida = 0;
    nuevaPartida.activo = 1;

    guardarPartidaArchivo(ARCHIVO_PARTIDAS, nuevaPartida);
    registrarPuntajeYEscalar(ARCHIVO_HISTORICO, nuevaPartida, &puestoAntes, &puestoNuevo);
}
