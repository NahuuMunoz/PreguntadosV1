#ifndef MULTIPLE_CHOICE_H
#define MULTIPLE_CHOICE_H

#define ARCHIVO_MC "preguntas_mc.dat"
#define CANT_CATEGORIAS 5
#define MC_POR_CATEGORIA 5
#define TOTAL_PREGUNTAS_MC 25

typedef struct {
    int idCategoria;
    char nombre[40];
} Categoria;

typedef struct {
    int idPregunta;
    Categoria categoria;
    char enunciado[140];
    char opciones[4][60];
    int respuestaCorrecta;
    int puntosBase;
    int activo;
} PreguntaMC;

// Utilidades
void leerTextoMC(char texto[], int dimension);
void mezclar_indices(int orden[], int cantidad);

// Persistencia y ABM
long cantidadPreguntasMC(const char nombreArchivo[]);
void inicializarBancoMC(const char nombreArchivo[]);
int guardarPreguntaMC(const char nombreArchivo[], PreguntaMC nueva);
void listarBancoMC(const char nombreArchivo[]);
int cargarMCPorCategoria(const char nombreArchivo[], int idCategoria, PreguntaMC destino[], int maxDim);

// Funciones de juego
int jugar_categoria_mc(Categoria cat, PreguntaMC preguntasDeCat[], int cantAJugar, int preguntasCargadas);
#endif
