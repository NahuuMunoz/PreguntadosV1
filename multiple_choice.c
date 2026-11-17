#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiple_choice.h"

void leerTextoMC(char texto[], int dimension)
{
    fgets(texto, dimension, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

/// Lee un numero entero por teclado y descarta el resto de la linea (incluido el Enter).
/// Devuelve -1 si el usuario escribio algo que no es un numero.
/// Se usa en lugar de scanf + fflush(stdin), porque fflush(stdin) no limpia el
/// teclado en todos los compiladores y el Enter sobrante rompia los fgets siguientes.
int leerEntero(void)
{
    int valor;
    int c;

    if (scanf("%d", &valor) != 1)
    {
        valor = -1;
    }

    do
    {
        c = getchar();
    }
    while (c != '\n' && c != EOF);

    if (c == EOF && valor == -1)
    {
        exit(0); /// Se cerro la entrada: no hay nada mas para leer
    }

    return valor;
}

void mezclar_indices(int orden[], int cantidad)
{
    int i, azar, aux;
    for (i = 0; i < cantidad; i++)
    {
        azar = rand() % cantidad;
        aux = orden[i];
        orden[i] = orden[azar];
        orden[azar] = aux;
    }
}

int jugar_categoria_mc(Categoria cat, PreguntaMC preguntasDeCat[], int cantAJugar, int preguntasCargadas, int *racha, int multiplicadorRacha)
{
    int orden_preguntas[MC_POR_CATEGORIA];
    int i, p_idx, respuesta, puntaje = 0;

    if (cantAJugar > preguntasCargadas)
    {
        cantAJugar = preguntasCargadas;
    }

    for (i = 0; i < preguntasCargadas; i++)
    {
        orden_preguntas[i] = i;
    }

    mezclar_indices(orden_preguntas, preguntasCargadas);

    printf("\n---------------------------------------\n");
    printf("   CATEGORIA: %s\n", cat.nombre);
    printf("-----------------------------------------\n");

    for (i = 0; i < cantAJugar; i++)
    {
        p_idx = orden_preguntas[i];
        PreguntaMC p = preguntasDeCat[p_idx];

        printf("\nPregunta %d de %d: %s\n", i + 1, cantAJugar, p.enunciado);
        printf("  1) %s\n", p.opciones[0]);
        printf("  2) %s\n", p.opciones[1]);
        printf("  3) %s\n", p.opciones[2]);
        printf("  4) %s\n", p.opciones[3]);

        do
        {
            printf("Tu respuesta (1-4): ");
            scanf("%d", &respuesta);
        }
        while (respuesta < 1 || respuesta > 4);

        if (respuesta == p.respuestaCorrecta)
        {
            (*racha)++;
            int puntosASumar = p.puntosBase;

            if (multiplicadorRacha > 1 && (*racha) >= 5)
            {
                printf("\n>>> RACHA x%d ACTIVADA! <<<\n", multiplicadorRacha);
                puntosASumar *= multiplicadorRacha;
            }

            printf("-> Correcto! Sumas %d puntos.\n", puntosASumar);
            puntaje += puntosASumar;
        }
        else
        {
            *racha = 0;
            printf("-> Incorrecto. La correcta era la %d.\n", p.respuestaCorrecta);
            if (multiplicadorRacha > 1)
            {
                printf(">>> Has perdido tu racha! <<<\n");
            }
        }
    }

    printf("\nResultado de %s: Obtenidos %d puntos.\n", cat.nombre, puntaje);
    return puntaje;
}

long cantidadPreguntasMC(const char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    long bytes;
    if (archivo == NULL) return 0;
    fseek(archivo, 0, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);
    return bytes / (long) sizeof(PreguntaMC);
}

int guardarPreguntaMC(const char nombreArchivo[], PreguntaMC nueva)
{
    FILE *archivo = fopen(nombreArchivo, "ab");
    size_t escritos;
    if (archivo == NULL) return 0;
    escritos = fwrite(&nueva, sizeof(PreguntaMC), 1, archivo);
    fclose(archivo);
    return escritos == 1;
}

int cargarMCPorCategoria(const char nombreArchivo[], int idCategoria, PreguntaMC destino[], int maxDim)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    PreguntaMC aux;
    int validos = 0;

    if (archivo == NULL) return 0;

    while (validos < maxDim && fread(&aux, sizeof(PreguntaMC), 1, archivo) == 1)
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

void inicializarBancoMC(const char nombreArchivo[])
{
    if (cantidadPreguntasMC(nombreArchivo) >= TOTAL_PREGUNTAS_MC) return;

    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) return;

    PreguntaMC banco[25] =
    {
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

void inicializarBancoMCHardcore(const char nombreArchivo[])
{
    if (cantidadPreguntasMC(nombreArchivo) >= TOTAL_PREGUNTAS_MC) return;

    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) return;

    PreguntaMC banco[25] =
    {
        {1, {1, "Deportes"}, "Cuantas derrotas oficiales tuvo Mike Tyson en su carrera?", {"0", "2", "6", "4"}, 3, 300, 1},
        {2, {1, "Deportes"}, "Que seleccion perdio 3 finales del mundo antes de ganar su primera?", {"Holanda", "Inglaterra", "Ninguna", "Francia"}, 3, 300, 1},
        {3, {1, "Deportes"}, "Cual fue el primer futbolista en ganar una copa mundial tanto como jugador como entrenador?", {"Maradona", "Zagallo", "Pele", "Beckenbauer"}, 2, 300, 1},
        {4, {1, "Deportes"}, "Quien es el mejor ajedrecista del mundo actualmente?", {"Hikaru Nakamura", "Magnus Carlsen", "Bobby Fischer", "Levy Rozman"}, 2, 300, 1},
        {5, {1, "Deportes"}, "Que jugador marco el gol mas rapido en la historia de los mundiales?", {"Hakan sukur", "Lionel Messi", "Roberto Baggio", "Thomas Muller"}, 2, 300, 1},
        {6, {2, "Historia"}, "Quien fue el ultimo faraon o faraona en gobernar el Antiguo Egipto antes de ser anexado por Roma?", {"Cleopatra", "Tutankamon", "Narmer", "Ramses"}, 1, 300, 1},
        {7, {2, "Historia"}, "En que anio se firmo la Carta Magna en Inglaterra?", {"1215", "1438", "1730", "1270"}, 1, 300, 1},
        {8, {2, "Historia"}, "En que guerra mundial se inventaron los molotovs?", {"Primera", "Segunda", "Ninguna", "Tercera"}, 3, 300, 1},
        {9, {2, "Historia"}, "La invasion de Alemania a que pais marca el comienzo de la segunda guerra mundial?", {"Francia", "Austria", "Italia", "Polonia"}, 4, 300, 1},
        {10, {2, "Historia"}, "Quien fundo el fascismo?", {"Hitler", "Stalin", "Mussolini", "Videla"}, 3, 300, 1},
        {11, {3, "Geografia"}, "Cual es el pais mas poblado actualmente?", {"China", "India", "Russia", "EEUU"}, 2, 300, 1},
        {12, {3, "Geografia"}, "Que pais esta completamente rodeado por territorio de sudafrica?", {"Lesotho", "Ethiopia", "Kenya", "Esuatini"}, 1, 300, 1},
        {13, {3, "Geografia"}, "Como se llama el punto mas alejado de la superficie terrestre?", {"Nemo", "Doris", "Mariana", "Bermudas"}, 1, 300, 1},
        {14, {3, "Geografia"}, "Cual es el pais sin salida al mar mas grande?", {"Iran", "Mongolia", "Kazajistan", "Bolivia"}, 3, 300, 1},
        {15, {3, "Geografia"}, "En qué país se encuentra el lago Baikal, el más profundo del mundo?", {"Mongolia", "Russia", "Ucrania", "Bielorussia"}, 2, 300, 1},
        {16, {4, "Ciencia"}, "En que medio viaja más rapido el sonido?", {"Acero", "Agua", "Aire", "Madera"}, 1, 300, 1},
        {17, {4, "Ciencia"}, "Como se denominan los animales que son más activos durante el amanecer y el anochecer?", {"Noctambulos", "Acechadores", "Diurnos", "Crepusculares"}, 4, 300, 1},
        {18, {4, "Ciencia"}, "Cual fue la temperatura (en celsius) mas baja registrada? ", {"-45", "-39", "-52", "-89"}, 4, 300, 1},
        {19, {4, "Ciencia"}, "Que nombre recibe una reaccion quimica que libera energia termica(calor)?", {"Endotermica", "Exotermica", "Hypertermica", "Endocalida"}, 2, 300, 1},
        {20, {4, "Ciencia"}, "Cuantos elementos hay en la tabla periodica?", {"120", "89", "218", "118"}, 4, 300, 1},
        {21, {5, "Programacion"}, "Cual fue el primer lenguaje de programacion de la historia?", {"Fortran", "Plankalkul", "C", "Cobol"}, 1, 300, 1},
        {22, {5, "Programacion"}, "En javascript, que devuelve la expresion typeof null?", {"NULL", "undefined", "object", "boolean"}, 3, 300, 1},
        {23, {5, "Programacion"}, "Que estructura de datos funciona bajo el principio LIFO?", {"Cola", "Arbol binario", "Pila", "Tabla Hash"}, 3, 300, 1},
        {24, {5, "Programacion"}, "Quien es considerada la primera programadora en computadora?", {"Ada Lovelace", "Alan Turing", "Margaret Hamilton", "Grace Hopper"}, 1, 300, 1},
        {25, {5, "Programacion"}, "Cual de los siguientes lenguajes de programacion es estatico?", {"Python", "Javascript", "Ruby", "Java"}, 4, 300, 1},
    };

    fwrite(banco, sizeof(PreguntaMC), 25, archivo);
    fclose(archivo);
}

void listarBancoMC(const char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    PreguntaMC p;
    if (archivo == NULL) return;
    printf("\n--- BANCO MULTIPLE CHOICE ---\n");
    while (fread(&p, sizeof(PreguntaMC), 1, archivo) == 1)
    {
        if (p.activo == 1)
        {
            printf("ID: %d | Cat: %s | %s\n", p.idPregunta, p.categoria.nombre, p.enunciado);
        }
    }
    fclose(archivo);
}
