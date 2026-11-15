
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "login.h"
#include "pila.h"
#include "verdadero_falso.h"
#include "multiple_choice.h"
#include "ranking.h"

///Constantes de Archivos usados
#define ARCHIVO_PARTIDAS "partidas.dat"

/// Prototipado
void partidaNueva(int idJugador, const char nombreJugador[]);
void mostrarJugadores(const char archivo[]);

int main(void) {

    printf("\n--- EL PROGRAMA INICIO CORRECTAMENTE ---\n");
    system("pause");

    const char archivo[] = "jugadores.dat";
    int opcionMenuLogin = -1;
    int opcionMenuJugador = -1;
    int opcHistorial;
    int logueado = 0;

    char usuario[50], clave[50];
    Jugador jugadorActual; // Variable para almacenar los datos del jugador logueado
    FILE *f;

    srand((unsigned int) time(NULL));
    inicializarBancoMC(ARCHIVO_MC);
    inicializarBancoVoF(ARCHIVO_VOF);

    do {
        printf("\n\t MENU PREGUNTADOS \t\n");
        printf("Elija una opcion: \n");
        printf("1_ Iniciar Sesion (Login)\n");
        printf("2_ Registrar Nuevo Jugador\n");
        printf("0_ Salir\n");
        scanf(" %d", &opcionMenuLogin);

        switch(opcionMenuLogin) {
            case 1:
                printf("\n\t LOGIN \t\n");
                printf("Usuario: ");
                fflush(stdin);
                fgets(usuario, 50, stdin);
                usuario[strcspn(usuario, "\n")] = 0;

                printf("Clave: ");
                fflush(stdin);
                fgets(clave, 50, stdin);
                clave[strcspn(clave, "\n")] = 0;

                logueado = validarLogin(archivo, usuario, clave);

                if(logueado == 1) {
                    // Rescatamos los datos completos del jugador que acaba de iniciar sesion
                    f = fopen(archivo, "rb");
                    if (f != NULL) {
                        Jugador aux;
                        while(fread(&aux, sizeof(Jugador), 1, f) == 1) {
                            if(strcmp(aux.nombreUsuario, usuario) == 0) {
                                jugadorActual = aux;
                                break;
                            }
                        }
                        fclose(f);
                    }

                    // Ciclo propio del Menu Jugador para no salir tras una partida
                    do {
                        printf("\nBienvenido de vuelta, %s!\n", jugadorActual.nombreJugador);
                        printf("Elija una opcion: \n");
                        printf("1_ Jugar Partida\n");
                        printf("2_ Ver Historiales y Rankings \n");
                        printf("3_ Ver Jugadores Registrados \n");
                        printf("0_ Cerrar Sesion (Volver al Login) \n");
                        scanf(" %d", &opcionMenuJugador);

                        switch(opcionMenuJugador) {
                            case 1:
                                // Se pasan el ID y el Nombre correspondientes al usuario logueado
                                partidaNueva(jugadorActual.ID, jugadorActual.nombreJugador);
                                break;

                            case 2:
                                // Submenu para visualizar historiales
                                do {
                                    printf("\n--- HISTORIALES ---\n");
                                    printf("1_ Ver Ranking de Mejores Partidas\n");
                                    printf("2_ Ver Ranking Historico (Podio)\n");
                                    printf("3_ Ver Banco de Preguntas\n");
                                    printf("0_ Volver al Menu Jugador\n");
                                    printf("Opcion: ");
                                    scanf("%d", &opcHistorial);

                                    switch(opcHistorial) {
                                        case 1:
                                            mostrarRankingPartidas(ARCHIVO_PARTIDAS);
                                            break;
                                        case 2:
                                            mostrarRankingHistorico(ARCHIVO_HISTORICO);
                                            break;
                                        case 3:
                                            listarBancoMC(ARCHIVO_MC);
                                            break;
                                        case 0:
                                            printf("Volviendo al Menu Jugador...\n");
                                            break;
                                        default:
                                            printf("Opcion invalida.\n");
                                            break;
                                    }
                                } while(opcHistorial != 0);
                                break;

                            case 3:
                                mostrarJugadores(archivo);
                                break;

                            case 0:
                                printf("Cerrando sesion... \n");
                                logueado = 0;
                                break;

                            default:
                                printf("\n\tOpcion invalida. \t\n");
                        }
                    } while(opcionMenuJugador != 0);
                }
                else {
                    printf("\nError: Usuario o clave incorrectos.\n");
                }
                break;

            case 2:
                printf("\nREGISTRO\n");
                Jugador nuevo = crearJugador(archivo);
                guardarJugadorEnArchivo(archivo, nuevo);
                break;

            case 0:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("\n\tOpcion invalida.\t\n");
        }
    } while(opcionMenuLogin != 0);

    return 0;
}

///Funcion para inicializar una partida
void partidaNueva(int idJugador, const char nombreJugador[]) {
    Categoria cats[5] = {
        {1, "Deportes"}, {2, "Historia"}, {3, "Geografia"}, {4, "Ciencia"}, {5, "Programacion"}
    };
    int indicesCats[5] = {0, 1, 2, 3, 4};
    int puntajeTotal = 0;
    int c;
    int puestoAntes = -1;
    int puestoNuevo = -1;

    mezclar_indices(indicesCats, 5); /// Elige de froma aleatoria el orden de las categorias

    printf("\n\t COMIENZA LA PARTIDA \t\n");

    for (c = 0; c < 3; c++) {
        Categoria catElegida = cats[indicesCats[c]];

        /// 0 = Multiple Choice, 1 = Verdadero / Falso
        int tipoJuegoAleatorio = rand() % 2;

        if (tipoJuegoAleatorio == 0) {
            PreguntaMC preguntasArchivo[5];
            int cargadas = cargarMCPorCategoria(ARCHIVO_MC, catElegida.idCategoria, preguntasArchivo, 5);

            if (cargadas > 0) {
                puntajeTotal += jugar_categoria_mc(catElegida, preguntasArchivo, 2, cargadas);
            }
        } else {
            PreguntaVoF preguntasArchivoVoF[5];
            int cargadas = cargarVoFPorCategoria(ARCHIVO_VOF, catElegida.idCategoria, preguntasArchivoVoF, 5);

            if (cargadas > 0) {
                puntajeTotal += jugar_categoria_vof(catElegida, preguntasArchivoVoF, 2, cargadas);
            }
        }
    }

    printf("\n\t FIN DEL JUEGO! PUNTAJE FINAL: %d \t\n", puntajeTotal);

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

///Funcion para leer archivo Jugadores
void mostrarJugadores(const char archivo[])
{
    FILE *archi = fopen(archivo, "rb");
    Jugador player;

    if(archi == NULL)
    {
        printf("\nNo hay usuarios registrados aun.\n");
        return;
    }

    printf("\n\tLISTADO DE JUGADORES EN SISTEMA\t\n");
    printf("-%5s | %-15s | %-20s | %-8s | %-10s\n", "ID", "Usuario", "Nombre Real", "Puntaje", "Partidas");

    while(fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        printf(" %-5d | %-15s | %-20s | %-8d | %-10d\n",
               player.ID,
               player.nombreUsuario,
               player.nombreJugador,
               player.puntaje,
               player.partidasJugadas);
    }

    fclose(archi);
}
=======
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
