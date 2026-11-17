#ifndef VERDADERO_FALSO_H_INCLUDED
#define VERDADERO_FALSO_H_INCLUDED

#include "multiple_choice.h" // Se lo incluye para reutilizar la estructura categoria, hecho por Pedro

#define ARCHIVO_VOF "preguntas_vof.dat"
#define VOF_POR_CATEGORIA 5
#define TOTAL_PREGUNTAS_VOF 25

typedef struct {
    int idPregunta;
    Categoria categoria;
    char enunciado[100];
    char respuesta[100];
    int puntosBase;
    int activo;
} PreguntaVoF;

long cantidadPreguntasVoF(const char nombreArchivo[]);
void inicializarBancoVoF(const char nombreArchivo[]);
int cargarVoFPorCategoria(const char nombreArchivo[], int idCategoria, PreguntaVoF destino[], int maxDim);

// Funciones de juego
int jugar_categoria_vof(Categoria cat, PreguntaVoF preguntasDeCat[], int cantAJugar, int preguntasCargadas, int *racha, int multiplicadorRacha);

#endif // VERDADERO_FALSO_H_INCLUDED
