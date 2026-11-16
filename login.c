#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


///Funcion para calcular el ID segun la cantidad de jugadores dentro del archivo
int obtenerSiguienteID(const char archivo[])
{
    FILE *archi = fopen(archivo, "rb");

    if (archi == NULL)
    {
        return 1; ///Como el archivo no existe se le asigna el primer ID
    }

    fseek(archi, 0, SEEK_END);
    long bytesTotales = ftell(archi);
    fclose(archi);

    int cantidadRegistros = bytesTotales / sizeof(Jugador);

    return cantidadRegistros + 1;
}

int buscarJugadorEnArchivo(const char archivo[], char nombreBuscado[])
{
    FILE *archi = fopen(archivo, "rb");
    int hallado = 0;
    Jugador player;

    if(archi == NULL)
    {
        return 0;
    }

    while(!hallado && fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        if(strcmp(nombreBuscado, player.nombreUsuario) == 0)
        {
            hallado = 1;
        }
    }

    fclose(archi);
    return hallado;
}

Jugador crearJugador(const char archivo[])
{
    Jugador nuevoJugador;
    int existe = 1;

    while(existe == 1)
    {
        printf("\nIngrese su nombre de Usuario (Login): ");
        fflush(stdin);
        fgets(nuevoJugador.nombreUsuario, 50, stdin);

        nuevoJugador.nombreUsuario[strcspn(nuevoJugador.nombreUsuario, "\n")] = 0;

        existe = buscarJugadorEnArchivo(archivo, nuevoJugador.nombreUsuario);

        if(existe == 1)
        {
            printf("Error: El nombre de usuario ya esta en uso. Intente con otro.\n");
        }
    }

    printf("Ingrese su Nombre de Jugador: ");
    fflush(stdin);
    fgets(nuevoJugador.nombreJugador, 50, stdin);
    nuevoJugador.nombreJugador[strcspn(nuevoJugador.nombreJugador, "\n")] = 0;

    printf("Ingrese su clave: ");
    fflush(stdin);
    fgets(nuevoJugador.claveUsuario, 50, stdin);
    nuevoJugador.claveUsuario[strcspn(nuevoJugador.claveUsuario, "\n")] = 0;

    nuevoJugador.puntaje = 0;
    nuevoJugador.partidasJugadas = 0;
    nuevoJugador.ID = obtenerSiguienteID(archivo);

    return nuevoJugador;
}

void guardarJugadorEnArchivo(const char archivo[], Jugador nuevoJugador)
{
    FILE *archi = fopen(archivo, "ab");

    if (archi != NULL)
    {
        fwrite(&nuevoJugador, sizeof(Jugador), 1, archi);
        fclose(archi);
        printf("\nUsuario '%s' registrado exitosamente con el ID %d.\n", nuevoJugador.nombreUsuario, nuevoJugador.ID);
    }
    else
    {
        printf("\nError: No se pudo acceder al archivo %s para guardar.\n", archivo);
    }
}

///Funcion que suma el puntaje de una partida y una partida jugada al jugador indicado.
///Busca al jugador por ID dentro del archivo y sobreescribe su registro.
///Devuelve 1 si lo pudo actualizar y 0 si no lo encontro.
int actualizarEstadisticasJugador(const char archivo[], int idJugador, int puntajeObtenido)
{
    FILE *archi = fopen(archivo, "r+b");
    Jugador player;
    int actualizado = 0;

    if(archi == NULL)
    {
        return 0;
    }

    while(!actualizado && fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        if(player.ID == idJugador)
        {
            player.puntaje += puntajeObtenido;
            player.partidasJugadas += 1;

            fseek(archi, -(long) sizeof(Jugador), SEEK_CUR); ///Volvemos un registro atras para pisarlo
            fwrite(&player, sizeof(Jugador), 1, archi);
            actualizado = 1;
        }
    }

    fclose(archi);
    return actualizado;
}

int validarLogin(const char archivo[], char usuarioIngresado[], char claveIngresada[])
{
    FILE *archi = fopen(archivo, "rb");
    Jugador player;
    int loginExitoso = 0;

    if(archi == NULL)
    {
        return 0; ///Si el archivo no existe no se puede iniciar el login de nadie
    }

    while(fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        if(strcmp(usuarioIngresado, player.nombreUsuario) == 0)
        {
            if(strcmp(claveIngresada, player.claveUsuario) == 0)
            {
                loginExitoso = 1;
            }
            break;
        }
    }

    fclose(archi);
    return loginExitoso;
}
