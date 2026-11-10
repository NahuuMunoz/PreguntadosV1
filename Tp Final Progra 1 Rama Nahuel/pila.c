#include <stdio.h>
#include "pila.h"

/**
 * @brief Inicializa una Pila para ser utilizada.
 * @param p la referencia de una Pila.
 */
void inicpila(Pila *p)
{
    p->postope = 0;
}

/**
 * @brief Agrega un dato a la Pila.
 * @param p la referencia de de una Pila.
 * @param dato el dato a agregar.
 */
void apilar(Pila *p, int dato)
{
    p->valores[p->postope] = dato;
    p->postope++;
}

/**
 * @brief Quita un valor de la Pila y lo retorna. Es responsabilidad del programador verificar que la Pila no esté vacía para evitar errores de segmentación. Para más información buscar "segfault".
 * @param p la referencia de una pila.
 * @return El valor removido.
 */
int desapilar(Pila *p)
{
    int retval = p->valores[p->postope - 1];
    p->postope--;
    return retval;
}

/**
 * @brief Retorna el valor en la cima de la Pila sin altertarla. Es responsabilidad del programador verificar que la Pila no esté vacía para evitar errores de segmentación. Para más información buscar "segfault".
 * @param p la referencia de una Pila.
 * @return el último valor en ingresar a la pila.
 */
int tope(Pila *p)
{
    return p->valores[p->postope - 1];
}

/**
 * @brief Retorna si la Pila está vacía.
 * @param p la referencia de una Pila.
 * @return 1 si la Pila tiene al menos un valor, 0 en caso contrario.
 */
int pilavacia(Pila *p)
{
    return p->postope == 0;
}

/**
 * @brief Permite ingresar un valor por teclado a la Pila. Imprime mensajes.
 * @param p la referencia de una Pila.
 */
void leer(Pila *p)
{
    if (p->postope >= 50)
    {
        printf("Error: la pila esta llena");
        return;
    }
    int aux;
    printf("Ingrese un valor entero: ");
    scanf("%d", &aux);
    apilar(p, aux);
}

/**
 * @brief Imprime por pantalla en un formato predeterminado la información guardada en una Pila.
 * @param p la referencia de una Pila.
 */
void mostrar(Pila *p)
{
    printf("\nBase .............................................. Tope\n\n");
    for (int i = 0; i < p->postope; i++)
        printf("| %d ", p->valores[i]);
    printf("|\n\nBase .............................................. Tope\n\n");
}
