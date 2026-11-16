#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "verdadero_falso.h"

long cantidadPreguntasVoF(const char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    long bytes;
    if (archivo == NULL) return 0;
    fseek(archivo, 0, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);
    return bytes / (long) sizeof(PreguntaVoF);
}

void inicializarBancoVoF(const char nombreArchivo[])
{
    if (cantidadPreguntasVoF(nombreArchivo) >= TOTAL_PREGUNTAS_VOF) return;

    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) return;

    PreguntaVoF banco[25] =
    {
        // Categoria 1: Deportes
        {1, {1, "Deportes"}, "El futbol se juega con 11 jugadores por equipo en la cancha.", "V", 100, 1},
        {2, {1, "Deportes"}, "Un partido de basquetbol dura 90 minutos de tiempo regular.", "F", 100, 1},
        {3, {1, "Deportes"}, "Michael Phelps es el deportista con mas medallas olimpicas de oro.", "V", 100, 1},
        {4, {1, "Deportes"}, "En el tenis, el puntaje siguiente a 30 es 45.", "F", 100, 1},
        {5, {1, "Deportes"}, "El maraton tiene una distancia oficial de 42.195 kilometros.", "V", 100, 1},

        // Categoria 2: Historia
        {6, {2, "Historia"}, "La Segunda Guerra Mundial finalizo en el anio 1945.", "V", 100, 1},
        {7, {2, "Historia"}, "Cristobal Colon llego a America en el anio 1492.", "V", 100, 1},
        {8, {2, "Historia"}, "El Imperio Romano cayo en el siglo XX.", "F", 100, 1},
        {9, {2, "Historia"}, "La Revolucion Francesa comenzo en 1789.", "V", 100, 1},
        {10, {2, "Historia"}, "Los faraones gobernaban en la antigua Grecia.", "F", 100, 1},

        // Categoria 3: Geografia
        {11, {3, "Geografia"}, "El monte Everest es la montana mas alta del mundo.", "V", 100, 1},
        {12, {3, "Geografia"}, "El oceano Atlantico es el oceano mas grande del planeta.", "F", 100, 1},
        {13, {3, "Geografia"}, "La capital de Australia es Sidney.", "F", 100, 1},
        {14, {3, "Geografia"}, "Brasil es el pais mas grande de America del Sur.", "V", 100, 1},
        {15, {3, "Geografia"}, "El rio Amazonas es el rio mas caudaloso del mundo.", "V", 100, 1},

        // Categoria 4: Ciencia
        {16, {4, "Ciencia"}, "El agua hierve a 100 grados Celsius a nivel del mar.", "V", 100, 1},
        {17, {4, "Ciencia"}, "La velocidad de la luz es mas lenta que la velocidad del sonido.", "F", 100, 1},
        {18, {4, "Ciencia"}, "El simbolo quimico del oro es Au.", "V", 100, 1},
        {19, {4, "Ciencia"}, "La Tierra es el cuarto planeta del sistema solar.", "F", 100, 1},
        {20, {4, "Ciencia"}, "El ADN contiene la informacion genetica de los seres vivos.", "V", 100, 1},

        // Categoria 5: Programacion
        {21, {5, "Programacion"}, "En C, un puntero almacena la direccion de memoria de otra variable.", "V", 100, 1},
        {22, {5, "Programacion"}, "La funcion fopen con modo 'rb' crea un archivo si este no existe.", "F", 100, 1},
        {23, {5, "Programacion"}, "El operador & se utiliza para obtener la direccion de memoria.", "V", 100, 1},
        {24, {5, "Programacion"}, "La funcion strcmp devuelve 1 cuando dos cadenas son exactamente iguales.", "F", 100, 1},
        {25, {5, "Programacion"}, "En lenguaje C, los arreglos comienzan en el indice 0.", "V", 100, 1}
    };

    fwrite(banco, sizeof(PreguntaVoF), 25, archivo);
    fclose(archivo);
}

int cargarVoFPorCategoria(const char nombreArchivo[], int idCategoria, PreguntaVoF destino[], int maxDim)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    PreguntaVoF aux;
    int validos = 0;

    if (archivo == NULL) return 0;

    while (validos < maxDim && fread(&aux, sizeof(PreguntaVoF), 1, archivo) == 1)
    {
        if (aux.activo == 1 && aux.categoria.idCategoria == idCategoria)
        {
            destino[validos] = aux;
            validos++;
        }
    }
    fclose(archivo);
    return validos;
}

int jugar_categoria_vof(Categoria cat, PreguntaVoF preguntasDeCat[], int cantAJugar, int preguntasCargadas)
{
    int orden_preguntas[VOF_POR_CATEGORIA];
    int i, p_idx, puntaje = 0;
    char respuesta[10];

    if (cantAJugar > preguntasCargadas)
    {
        cantAJugar = preguntasCargadas;
    }

    for (i = 0; i < preguntasCargadas; i++)
    {
        orden_preguntas[i] = i;
    }

    mezclar_indices(orden_preguntas, preguntasCargadas);

    printf("   CATEGORIA: %s (VERDADERO O FALSO)\n", cat.nombre);


    for (i = 0; i < cantAJugar; i++)
    {
        p_idx = orden_preguntas[i];
        PreguntaVoF p = preguntasDeCat[p_idx];

        printf("\nPregunta %d de %d: %s\n", i + 1, cantAJugar, p.enunciado);

        do
        {
            printf("Tu respuesta (Ingrese V o F): ");
            fflush(stdin);
            fgets(respuesta, 10, stdin);
            respuesta[strcspn(respuesta, "\n")] = 0;
        }
        while (strcmpi(respuesta, "V") != 0 && strcmpi(respuesta, "F") != 0);

        if (strcmpi(respuesta, p.respuesta) == 0)
        {
            printf("-> Correcto! Sumas %d puntos.\n", p.puntosBase);
            puntaje += p.puntosBase;
        }
        else
        {
            printf("-> Incorrecto! La respuesta era %s.\n", p.respuesta);
        }
    }

    printf("\nResultado de %s (V/F): Obtenidos %d puntos.\n", cat.nombre, puntaje);
    return puntaje;
}
