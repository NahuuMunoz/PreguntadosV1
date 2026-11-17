#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "login.h"
#include "pila.h"
#include "verdadero_falso.h"
#include "multiple_choice.h"
#include "ranking.h"
#include <conio.h>
#include <windows.h>


/// Prototipado
void partidaNueva(int idJugador, const char nombreJugador[]);
void mostrarJugadores(const char archivo[]);

int main(void)
{

    printf("\n--- EL PROGRAMA INICIO CORRECTAMENTE ---\n");
    system("pause");

    const char archivo[] = ARCHIVO_JUGADORES;
    int opcionMenuLogin = -1;
    int opcionMenuJugador = -1;
    int opcHistorial;
    int logueado = 0;

    char usuario[50], clave[50];
    Jugador jugadorActual; // Variable para almacenar los datos del jugador logueado
    FILE *f;

    srand((unsigned int) time(NULL));
    inicializarBancoMC(ARCHIVO_MC);
    inicializarBancoMCHardcore(ARCHIVO_MC_HARDCORE);
    inicializarBancoVoF(ARCHIVO_VOF);

    do
    {
        printf("\n\t MENU PREGUNTADOS \t\n");
        printf("Elija una opcion: \n");
        printf("1_ Iniciar Sesion (Login)\n");
        printf("2_ Registrar Nuevo Jugador\n");
        printf("0_ Salir\n");
        opcionMenuLogin = leerEntero();

        switch(opcionMenuLogin)
        {
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

            if(logueado == 1)
            {
                // Rescatamos los datos completos del jugador que acaba de iniciar sesion
                f = fopen(archivo, "rb");
                if (f != NULL)
                {
                    Jugador aux;
                    while(fread(&aux, sizeof(Jugador), 1, f) == 1)
                    {
                        if(strcmp(aux.nombreUsuario, usuario) == 0)
                        {
                            jugadorActual = aux;
                            break;
                        }
                    }
                    fclose(f);
                }

                // Ciclo propio del Menu Jugador para no salir tras una partida
                do
                {
                    printf("\nBienvenido de vuelta, %s!\n", jugadorActual.nombreJugador);
                    printf("Elija una opcion: \n");
                    printf("1_ Jugar Partida\n");
                    printf("2_ Ver Historiales y Rankings \n");
                    printf("3_ Ver Jugadores Registrados \n");
                    printf("0_ Cerrar Sesion (Volver al Login) \n");
                    opcionMenuJugador = leerEntero();

                    switch(opcionMenuJugador)
                    {
                    case 1:
                        // Se pasan el ID y el Nombre correspondientes al usuario logueado
                        partidaNueva(jugadorActual.ID, jugadorActual.nombreJugador);
                        break;

                    case 2:
                        // Submenu para visualizar historiales
                        do
                        {
                            printf("\n--- HISTORIALES ---\n");
                            printf("1_ Ver Ranking de Mejores Partidas\n");
                            printf("2_ Ver Ranking Historico (Podio)\n");
                            printf("3_ Ver Banco de Preguntas\n");
                            printf("0_ Volver al Menu Jugador\n");
                            printf("Opcion: ");
                            opcHistorial = leerEntero();

                            switch(opcHistorial)
                            {
                            case 1:
                                mostrarRankingPartidas(ARCHIVO_PARTIDAS);
                                break;
                            case 2:
                                mostrarRankingHistorico(ARCHIVO_HISTORICO);
                                break;
                            case 3:
                                listarBancoMC(ARCHIVO_MC);
                                listarBancoMC(ARCHIVO_MC_HARDCORE);
                                break;
                            case 0:
                                printf("Volviendo al Menu Jugador...\n");
                                break;
                            default:
                                printf("Opcion invalida.\n");
                                break;
                            }
                        }
                        while(opcHistorial != 0);
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
                }
                while(opcionMenuJugador != 0);
            }
            else
            {
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
    }
    while(opcionMenuLogin != 0);

    return 0;
}

///Funcion para inicializar una partida
void partidaNueva(int idJugador, const char nombreJugador[])
{
    Categoria cats[5] =
    {
        {1, "Deportes"}, {2, "Historia"}, {3, "Geografia"}, {4, "Ciencia"}, {5, "Programacion"}
    };
    int indicesCats[5] = {0, 1, 2, 3, 4};
    int puntajeTotal = 0;
    int c;
    int puestoAntes = -1;
    int puestoNuevo = -1;
    int modo = 0;
    int cantCategorias;
    int preguntasPorCategoria;
    int segundosPorPregunta = 0; // 0 = sin limite de tiempo por defecto para facil y dificil

    /// --- VARIABLES DE RACHA Y POTENCIADOR ---
    int rachaActual = 0;
    int rachaMaxima = 0;
    int multiplicadorRacha = 1;

    ///Variables de archivo hardcore
    const char *archivoMC = ARCHIVO_MC;
    char *nombreModo = "Facil";
    /// Eleccion del modo de juego
    do
    {
        printf("\n--- ELEGI EL MODO DE JUEGO ---\n");
        printf("1_ Facil    (3 categorias al azar, 3 preguntas de cada una)\n");
        printf("2_ Dificil  (5 categorias al azar, 5 preguntas de cada una, Multiplicador x2)\n");
        printf("3_ Hardcore (5 categorias al azar, 5 preguntas de cada una, contrarreloj, Multiplicador x3)\n");
        printf("Modo: ");
        modo = leerEntero();
    }
    while (modo != 1 && modo != 2 && modo != 3);

    if (modo == 1)
    {
        cantCategorias = 3;
        preguntasPorCategoria = 3;
        multiplicadorRacha = 1;
        nombreModo = "Facil";
    }
    else if(modo == 2)
    {
        cantCategorias = 5;
        preguntasPorCategoria = 5;
        multiplicadorRacha = 2;
        nombreModo = "Dificil";
    }
    else
    {
        cantCategorias = 5;
        preguntasPorCategoria = 5;
        multiplicadorRacha = 3;
        archivoMC = ARCHIVO_MC_HARDCORE;
        nombreModo = "Hardcore";
        segundosPorPregunta = 10; //Limite de respuesta para el modo HARDCORE
    }

    Pila mazoCategorias;
    inicpila(&mazoCategorias);

    mezclar_indices(indicesCats, 5);

    // Apilamos la cantidad de categorias que se van a jugar en este modo
    for (int i = 0; i < cantCategorias; i++) {
        apilar(&mazoCategorias, indicesCats[i]);
    }

    printf("\n\t COMIENZA LA PARTIDA (Modo %s) \t\n", nombreModo);

    // 4. En lugar de un for, juegas MIENTRAS la pila no este vacia
    while (!pilavacia(&mazoCategorias)) {

        // Sacas la categoria del tope de la pila y la desapilas
        int idCatActual = desapilar(&mazoCategorias);
        Categoria catElegida = cats[idCatActual];

        int tipoJuegoAleatorio = (modo == 3) ? 0 : rand() % 2;

        if (tipoJuegoAleatorio == 0) {
            PreguntaMC preguntasArchivo[5];
            int cargadas = cargarMCPorCategoria(archivoMC, catElegida.idCategoria, preguntasArchivo, 5);

            if (cargadas > 0) {
                puntajeTotal += jugar_categoria_mc(catElegida, preguntasArchivo, preguntasPorCategoria, cargadas, &rachaActual, multiplicadorRacha, segundosPorPregunta);
            }
        } else {
            PreguntaVoF preguntasArchivoVoF[5];
            int cargadas = cargarVoFPorCategoria(ARCHIVO_VOF, catElegida.idCategoria, preguntasArchivoVoF, 5);

            if (cargadas > 0) {
                puntajeTotal += jugar_categoria_vof(catElegida, preguntasArchivoVoF, preguntasPorCategoria, cargadas, &rachaActual, multiplicadorRacha);
            }
        }

        if (rachaActual > rachaMaxima) {
            rachaMaxima = rachaActual;
        }
    }

    printf("\n\t FIN DEL JUEGO! PUNTAJE FINAL: %d \t\n", puntajeTotal);

    Partida nuevaPartida;
    nuevaPartida.idPartida = (int)cantidadPartidasGuardadas(ARCHIVO_PARTIDAS) + 1;
    nuevaPartida.idJugador = idJugador;
    strcpy(nuevaPartida.nombreJugador, nombreJugador);
    nuevaPartida.modo = modo;
    nuevaPartida.puntajeObtenido = puntajeTotal;
    nuevaPartida.respuestasCorrectas = 0;

    // Guardamos la racha definitiva en la partida
    nuevaPartida.rachaMaximaPartida = rachaMaxima;
    nuevaPartida.activo = 1;

    guardarPartidaArchivo(ARCHIVO_PARTIDAS, nuevaPartida);
    registrarPuntajeYEscalar(ARCHIVO_HISTORICO, nuevaPartida, &puestoAntes, &puestoNuevo);

    /// Suma el puntaje y la partida al jugador, para que se vea en "Jugadores Registrados"
    actualizarEstadisticasJugador(ARCHIVO_JUGADORES, idJugador, puntajeTotal);
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
