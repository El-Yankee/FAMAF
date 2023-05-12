#include <stdlib.h>  /* EXIT_SUCCESS... */
#include <stdio.h>   /* printf()...     */
#include "pair.h"    /* TAD Par         */

/*
    Ahora como el main solo tiene acceso al .h, no se como esta implementado el TAD.
    Debo utilizar las funciones dadas en el spec (.h)
*/
static void show_pair(pair_t p) {
    printf("(%d, %d)\n", pair_first(p), pair_second(p));
}

int main(void) {
    pair_t p, q;
    // Nuevo par p
    p = pair_new(3, 4);
    // Se muestra el par por pantalla
    printf("p = ");
    show_pair(p);
    // Nuevo para q con elementos de p intercambiados
    q = pair_swapped(p);
    // Se muestra q
    printf("q = ");
    show_pair(q);
    // Se destruyen p y q
    p = pair_destroy(p);
    q = pair_destroy(q);
    return EXIT_SUCCESS;
}

/*
    Ahora si se logra el encapsulamiento ya que solo podemos utilizar las funciones del spec(.h) para trabajar con el TAD
    En la funcion show_pair ya no podemos acceder directo a la estructura porque no sabemos como esta implementada.
    Usamos las funciones del spec(.h) que nos devuelven el primer y segundo elemento
*/