#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiple_choice.h"

void leerTextoMC(char texto[], int dimension) {
    fgets(texto, dimension, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

void mezclar_indices(int orden[], int cantidad) {
    int i, azar, aux;
    for (i = 0; i < cantidad; i++) {
        azar = rand() % cantidad;
        aux = orden[i];
        orden[i] = orden[azar];
        orden[azar] = aux;
    }
}
int jugar_categoria_mc(Categoria cat, PreguntaMC preguntasDeCat[], int cantAJugar, int preguntasCargadas) {
    int orden_preguntas[MC_POR_CATEGORIA];
    int i, p_idx, respuesta, puntaje = 0;

    if (cantAJugar > preguntasCargadas) {
        cantAJugar = preguntasCargadas;
    }

    for (i = 0; i < preguntasCargadas; i++) {
        orden_preguntas[i] = i;
    }

    mezclar_indices(orden_preguntas, preguntasCargadas);

    printf("\n---------------------------------------\n");
    printf("   CATEGORIA: %s\n", cat.nombre);
    printf("-----------------------------------------\n");

    for (i = 0; i < cantAJugar; i++) {
        p_idx = orden_preguntas[i];
        PreguntaMC p = preguntasDeCat[p_idx];

        printf("\nPregunta %d de %d: %s\n", i + 1, cantAJugar, p.enunciado);
        printf("  1) %s\n", p.opciones[0]);
        printf("  2) %s\n", p.opciones[1]);
        printf("  3) %s\n", p.opciones[2]);
        printf("  4) %s\n", p.opciones[3]);

        do {
            printf("Tu respuesta (1-4): ");
            scanf("%d", &respuesta);
        } while (respuesta < 1 || respuesta > 4);

        if (respuesta == p.respuestaCorrecta) {
            printf("-> Correcto! Sumas %d puntos.\n", p.puntosBase);
            puntaje += p.puntosBase;
        } else {
            printf("-> Incorrecto. La correcta era la %d.\n", p.respuestaCorrecta);
        }
    }

    printf("\nResultado de %s: Obtenidos %d puntos.\n", cat.nombre, puntaje);
    return puntaje;
}

long cantidadPreguntasMC(const char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    long bytes;
    if (archivo == NULL) return 0;
    fseek(archivo, 0, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);
    return bytes / (long) sizeof(PreguntaMC);
}

int guardarPreguntaMC(const char nombreArchivo[], PreguntaMC nueva) {
    FILE *archivo = fopen(nombreArchivo, "ab");
    size_t escritos;
    if (archivo == NULL) return 0;
    escritos = fwrite(&nueva, sizeof(PreguntaMC), 1, archivo);
    fclose(archivo);
    return escritos == 1;
}

int cargarMCPorCategoria(const char nombreArchivo[], int idCategoria, PreguntaMC destino[], int maxDim) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    PreguntaMC aux;
    int validos = 0;

    if (archivo == NULL) return 0;

    while (validos < maxDim && fread(&aux, sizeof(PreguntaMC), 1, archivo) == 1) {
        if (aux.activo == 1 && aux.categoria.idCategoria == idCategoria) {
            destino[validos] = aux;
            validos++;
        }
    }
    fclose(archivo);
    return validos;
}

void inicializarBancoMC(const char nombreArchivo[]) {
    if (cantidadPreguntasMC(nombreArchivo) >= TOTAL_PREGUNTAS_MC) return;

    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) return;

    PreguntaMC banco[25] = {
        {1, {1, "Deportes"}, "Cual es el deporte mas popular del mundo?", {"Tenis", "Futbol", "Basquet", "Rugby"}, 2, 100, 1},
        {2, {1, "Deportes"}, "Quien gano el mundial 2022?", {"Francia", "Brasil", "Argentina", "Croacia"}, 3, 100, 1},
        {3, {1, "Deportes"}, "Donde se crearon las olimpiadas?", {"China", "Estados Unidos", "Argentina", "Grecia"}, 4, 100, 1},
        {4, {1, "Deportes"}, "Cuantos campeonatos mundiales de F1 gano Juan Manuel Fangio?", {"1","4","8","5"}, 4, 100, 1},
        {5, {1, "Deportes"}, "En que anio gano Del Potro el grand slam?", {"2009", "2025", "2016", "2021"}, 1, 100, 1},
        {6, {2, "Historia"}, "Quien cruzo los Andes?", {"Belgrano", "San Martin", "Sarmiento", "Rosas"}, 2, 100, 1},
        {7, {2, "Historia"}, "En que anio fue la Revolucion de Mayo?", {"1810", "1816", "1820", "1853"}, 1, 100, 1},
        {8, {2, "Historia"}, "El asesinato de quien inicio la WWI (Primera guerra Mundial)?", {"Benedicto XV", "Jorge V", "Francisco Fernando de Austria", "Guillermo II"}, 3, 100, 1},
        {9, {2, "Historia"}, "Quien es Von Neumann?", {"Jugador de futbol", "Matematico", "Cantante", "Escritor de Fabulas"}, 2, 100, 1},
        {10, {2, "Historia"}, "En que anio cayo el Muro de Berlin?", {"2005", "1954", "1875", "1989"}, 4, 100, 1},
        {11, {3, "Geografia"}, "Cual es la capital de Argentina?", {"Cordoba", "Rosario", "Buenos Aires", "Mendoza"}, 3, 100, 1},
        {12, {3, "Geografia"}, "Donde esta el Aconcagua?", {"San Juan", "Mendoza", "Neuquen", "Chubut"}, 2, 100, 1},
        {13, {3, "Geografia"}, "Cual es el oceano mas grande del mundo?", {"Pacifico", "Atlantico", "Indico", "Antartico"}, 1, 100, 1},
        {14, {3, "Geografia"}, "Cual es el lugar mas aislado del mundo?", {"Hawaii", "Isla de Pascua", "Madagascar", "Punto Nemo"}, 4, 100, 1},
        {15, {3, "Geografia"}, "Cual es el continente con mayor cantidad de Poblacion?", {"Europa", "Asia", "America", "Africa"}, 2, 100, 1},
        {16, {4, "Ciencia"}, "Cual es la formula del agua?", {"HO", "H2O2", "H2O", "CO2"}, 3, 100, 1},
        {17, {4, "Ciencia"}, "Que planeta es el tercero desde el sol?", {"Marte", "Venus", "Tierra", "Jupiter"}, 3, 100, 1},
        {18, {4, "Ciencia"}, "Quien creo la bomba atomica?", {"Einstein", "Oppenheimer", "Marie Curie", "Nobel"}, 2, 100, 1},
        {19, {4, "Ciencia"}, "Quien creo el bypass coronario?", {"Freud", "Fleming", "Favaloro", "Pasteur"}, 3, 100, 1},
        {20, {4, "Ciencia"}, "Que laboratorio creo la droga 'Heroina'?", {"Bayer", "Pfizer", "Bago", "Elea"}, 1, 100, 1},
        {21, {5, "Programacion"}, "En C, que funcion libera memoria dinamica?", {"malloc", "free", "realloc", "calloc"}, 2, 100, 1},
        {22, {5, "Programacion"}, "Con que caracter termina un string en C?", {"\\n", "\\t", "\\0", "EOF"}, 3, 100, 1},
        {23, {5, "Programacion"}, "Que es un POO?", {"Problematicas Orientadas a Ondas", "Programacion orientada a objetos", "Paralelismos Orientados a Organizaciones", "Practicas Orientadas a Ordenadores"}, 2, 100, 1},
        {24, {5, "Programacion"}, "Que es el codigo Binario?", {"Lenguaje base de la computadora", "Protocolo de seguridad", "Sistema operativo de los 80", "Componente de la Computadora"}, 1, 100, 1},
        {25, {5, "Programacion"}, "A que se le llama comunmente 'cerebro de la computadora'?", {"Mother Board", "Memoria RAM", "Cpu", "Placa de video"}, 3, 100, 1}
    };

    fwrite(banco, sizeof(PreguntaMC), 25, archivo);
    fclose(archivo);
}

void listarBancoMC(const char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    PreguntaMC p;
    if (archivo == NULL) return;
    printf("\n--- BANCO MULTIPLE CHOICE ---\n");
    while (fread(&p, sizeof(PreguntaMC), 1, archivo) == 1) {
        if (p.activo == 1) {
            printf("ID: %d | Cat: %s | %s\n", p.idPregunta, p.categoria.nombre, p.enunciado);
        }
    }
    fclose(archivo);
}
