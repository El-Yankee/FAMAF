/**
 ** Para poder compilar los archivos usamos la sig linea en la consola
 * gcc -Wall -Werror -Wextra -pedantic -std=c99 main.c -o main
 * ./main
*/

#include <stdio.h>
#include <stdlib.h>

#include "data.h"

void print_data(data_t d) {
    
    printf("NOMBRE: %s\n"
           "EDAD  : %d años\n"
           "ALTURA: %d cm\n\n",
           d.name, d.age, d.height);
}

void set_name(name_t new_name, data_t *d) {

    // Como es un arreglo de chars, lo recorro y voy modificando char a char
    for (size_t i = 0; i < NAME_MAXSIZE; i++) {

        // d esta apuntando a una estructura, asique accedo al campo que quiero modificar con '->'
        d->name[i] = new_name[i];

    }
}

int main(void) {

    data_t messi = {"Leo Messi", 35, 169};
    print_data(messi);
    name_t messi_full_name = "Lionel Messi";

    // Creo el puntero a la estrcutura data_t    
    data_t *d = NULL;

    // Le digo que tome la direccion de la estructura a modificar
    d = &messi;

    set_name(messi_full_name , d);

    print_data(messi);

    return EXIT_SUCCESS;
}

