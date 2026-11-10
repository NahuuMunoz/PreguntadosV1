
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

long cantidadPartidasGuardadas(const char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    long bytes;

    if (archivo == NULL){
        return 0;
    }

    fseek(archivo, 0, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);

    if (bytes < 0){
        return 0;
    }

    return bytes / (long) sizeof(Partida);
}

int guardarPartidaArchivo(const char nombreArchivo[], Partida nueva){
    FILE *archivo = fopen(nombreArchivo, "ab");
    size_t escritos;

    if (archivo == NULL){
        return 0;
    }

    escritos = fwrite(&nueva, sizeof(Partida), 1, archivo);
    fclose(archivo);

    return escritos == 1;
}

void ordenarPartidasSeleccion(Partida partidas[], int validos){
    int i;
    int j;
    int posMayor;
    Partida aux;

    for (i = 0; i < validos - 1; i++){
        posMayor = i;
        for (j = i + 1; j < validos; j++){
            if (partidas[j].puntajeObtenido > partidas[posMayor].puntajeObtenido){
                posMayor = j;
            }
        }

        aux = partidas[i];
        partidas[i] = partidas[posMayor];
        partidas[posMayor] = aux;
    }
}

void mostrarRankingPartidas(const char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Partida *partidas = NULL;
    Partida *temporal = NULL;
    Partida reg;
    int capacidad = 5;
    int validos = 0;
    int i;

    if (archivo == NULL){
        printf("\nAun no se han registrado partidas.\n");
        return;
    }

    partidas = malloc((size_t) capacidad * sizeof(Partida));
    if (partidas == NULL){
        fclose(archivo);
        return;
    }

    while (fread(&reg, sizeof(Partida), 1, archivo) == 1){
        if (reg.activo == 1){
            if (validos == capacidad){
                int nuevaCapacidad = capacidad + 5;
                temporal = realloc(partidas, (size_t) nuevaCapacidad * sizeof(Partida));
                if (temporal != NULL){
                    partidas = temporal;
                    capacidad = nuevaCapacidad;
                }
            }

            if (validos < capacidad){
                partidas[validos] = reg;
                validos++;
            }
        }
    }

    fclose(archivo);

    if (validos == 0){
        printf("\nNo hay partidas activas para mostrar\n");
        free(partidas);
        partidas = NULL;
        return;
    }

    ordenarPartidasSeleccion(partidas, validos);

    printf("\n----- RANKING DE PARTIDAS INDIVIDUALES -----\n");
    for (i = 0; i < validos; i++){
        printf("  Puesto #%2d | Partida #%2d | %-15s (ID %d) | Modo: %-7s | %4d pts | Aciertos: %2d | Racha: %d\n",
               i + 1,
               partidas[i].idPartida,
               partidas[i].nombreJugador,
               partidas[i].idJugador,
               (partidas[i].modo == 1) ? "Facil" : "Dificil",
               partidas[i].puntajeObtenido,
               partidas[i].respuestasCorrectas,
               partidas[i].rachaMaximaPartida);
    }
    free(partidas);
    partidas = NULL;
}

void mostrarPartidasDeUnJugador(const char nombreArchivo[], int idJugador){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Partida p;
    int contador = 0;

    if (archivo == NULL){
        printf("\nAun no hay partidas guardadas\n");
        return;
    }

    printf("\n--- HISTORIAL DE PARTIDAS DEL JUGADOR (ID %d) ---\n", idJugador);
    while (fread(&p, sizeof(Partida), 1, archivo) == 1){
        if (p.activo == 1 && p.idJugador == idJugador){
            printf("  Partida #%2d | Modo: %-7s | Puntaje: %4d pts | Aciertos: %2d | Racha Max: %d\n",
                   p.idPartida,
                   (p.modo == 1) ? "Facil" : "Dificil",
                   p.puntajeObtenido,
                   p.respuestasCorrectas,
                   p.rachaMaximaPartida);
            contador++;
        }
    }

    if (contador == 0){
        printf("  Este jugador aun no registra partidas\n");
    }

    fclose(archivo);
}

long cantidadJugadoresHistorico(const char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    long bytes;

    if (archivo == NULL){
        return 0;
    }

    fseek(archivo, 0, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);

    if (bytes < 0){
        return 0;
    }

    return bytes / (long) sizeof(RankingHistorico);
}

void ordenarHistoricoSeleccion(RankingHistorico lista[], int validos){
    int i;
    int j;
    int posMayor;
    RankingHistorico aux;

    for (i = 0; i < validos - 1; i++){
        posMayor = i;
        for (j = i + 1; j < validos; j++){
            if (lista[j].puntajeHistorico > lista[posMayor].puntajeHistorico){
                posMayor = j;
            }
        }

        aux = lista[i];
        lista[i] = lista[posMayor];
        lista[posMayor] = aux;
    }
}

int obtenerPuestoEnRanking(const char nombreArchivo[], int idJugador){
    long cantidad = cantidadJugadoresHistorico(nombreArchivo);
    RankingHistorico *vector = NULL;
    FILE *archivo;
    int validos = 0;
    int i;
    int puesto = -1;

    if (cantidad <= 0){
        return -1;
    }

    vector = malloc((size_t) cantidad * sizeof(RankingHistorico));
    if (vector == NULL){
        return -1;
    }

    archivo = fopen(nombreArchivo, "rb");
    if (archivo != NULL){
        while (validos < cantidad && fread(&vector[validos], sizeof(RankingHistorico), 1, archivo) == 1){
            if (vector[validos].activo == 1){
                validos++;
            }
        }
        fclose(archivo);
    }

    ordenarHistoricoSeleccion(vector, validos);

    for (i = 0; i < validos; i++){
        if (vector[i].idJugador == idJugador){
            puesto = i + 1;
        }
    }

    free(vector);
    vector = NULL;

    return puesto;
}

int registrarPuntajeYEscalar(const char nombreArchivo[], Partida partidaJugada, int *puestoAnterior, int *puestoNuevo){
    FILE *archivo;
    RankingHistorico reg;
    int hallado = 0;

    *puestoAnterior = obtenerPuestoEnRanking(nombreArchivo, partidaJugada.idJugador);

    archivo = fopen(nombreArchivo, "r+b");
    if (archivo != NULL){
        while (!hallado && fread(&reg, sizeof(RankingHistorico), 1, archivo) == 1){
            if (reg.activo == 1 && reg.idJugador == partidaJugada.idJugador){
                reg.puntajeHistorico += partidaJugada.puntajeObtenido;
                reg.partidasJugadas += 1;

                if (partidaJugada.rachaMaximaPartida > reg.mejorRachaHistorica){
                    reg.mejorRachaHistorica = partidaJugada.rachaMaximaPartida;
                }

                fseek(archivo, -(long) sizeof(RankingHistorico), SEEK_CUR);
                fwrite(&reg, sizeof(RankingHistorico), 1, archivo);
                hallado = 1;
            }
        }
        fclose(archivo);
    }

    if (!hallado){
        RankingHistorico nuevo;
        nuevo.idJugador = partidaJugada.idJugador;
        strcpy(nuevo.nombreJugador, partidaJugada.nombreJugador);
        nuevo.puntajeHistorico = partidaJugada.puntajeObtenido;
        nuevo.partidasJugadas = 1;
        nuevo.mejorRachaHistorica = partidaJugada.rachaMaximaPartida;
        nuevo.activo = 1;

        archivo = fopen(nombreArchivo, "ab");
        if (archivo == NULL){
            return 0;
        }
        fwrite(&nuevo, sizeof(RankingHistorico), 1, archivo);
        fclose(archivo);
    }

    *puestoNuevo = obtenerPuestoEnRanking(nombreArchivo, partidaJugada.idJugador);
    return 1;
}

void mostrarTablaHistoricaRecursiva(const RankingHistorico lista[], int validos, int indice){
    if (indice == validos){
        return;
    }

    printf("  Puesto #%2d | %-16s (ID %2d) | Total: %5d pts | Partidas: %2d | Mejor Racha: %d\n",
           indice + 1,
           lista[indice].nombreJugador,
           lista[indice].idJugador,
           lista[indice].puntajeHistorico,
           lista[indice].partidasJugadas,
           lista[indice].mejorRachaHistorica);

    mostrarTablaHistoricaRecursiva(lista, validos, indice + 1);
}

void mostrarRankingHistorico(const char nombreArchivo[])
{
    long cantidad = cantidadJugadoresHistorico(nombreArchivo);
    RankingHistorico *lista = NULL;
    FILE *archivo;
    int validos = 0;

    if (cantidad <= 0){
        printf("\nAun no hay jugadores en el Ranking Historico.\n");
        return;
    }

    lista = malloc((size_t) cantidad * sizeof(RankingHistorico));
    if (lista == NULL){
        printf("No fue posible reservar memoria para el ranking.\n");
        return;
    }

    archivo = fopen(nombreArchivo, "rb");
    if (archivo != NULL){
        while (validos < cantidad && fread(&lista[validos], sizeof(RankingHistorico), 1, archivo) == 1){
            if (lista[validos].activo == 1){
                validos++;
            }
        }
        fclose(archivo);
    }

    ordenarHistoricoSeleccion(lista, validos);

    printf("\n----- RANKING HISTORICO DE JUGADORES -----\n");
    mostrarTablaHistoricaRecursiva(lista, validos, 0);

    free(lista);
    lista = NULL;
}
