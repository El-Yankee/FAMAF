/**
 ** Para poder compilar los archivos usamos la sig linea en la consola
 * gcc -Wall -Werror -Wextra -pedantic -std=c99 sizes.c -o sizes
 * ./sizes
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

int main(void) {

    data_t messi = {"Lionel Messi", 35, 169};
    print_data(messi);

    // %lu significa long unsigned int. Utilizo el operador sizeof() para saber la cantidad de memoria en bytes.

    printf("name-size  : %lu bytes\n"
           "age-size   : %lu bytes\n"
           "height-size: %lu bytes\n"
           "data_t-size: %lu bytes\n", sizeof(messi.name) , sizeof(messi.age) , sizeof(messi.height), sizeof(messi));

    return EXIT_SUCCESS;
}

/*
    Respuestas:

    * La suma de los miembros no siempre coincide con el total
    * El tamaño del campo name NO depende del nombre que contiene, depende del tamaño asignado al arreglo.

*/