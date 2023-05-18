/**
 ** Para poder compilar los archivos usamos la sig linea en la consola
 * gcc -Wall -Wextra -pedantic -std=c99 miarchivo.c -o miprograma
 * ./miprograma
 *
 * * Y para utilizar el debugger
 * gcc -Wall -Wextra -std=c99 -g miarchivo.c -o miprograma
 * gdb ./miprograma
 *
 */

/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Maximum allowed length of the array */
#define MAX_SIZE 100000


unsigned int array_from_stdin(int array[], unsigned int max_size) {
    
    unsigned int true_length = 0u;
    int chequeo = 0;

    // Creo un puntero de archivo
    FILE *datos_de_entrada = NULL;

    // Le asigno al puntero stdin que tiene un formato File*
    datos_de_entrada = stdin;

    // Le asigno a true_length el valor ingresado en consola
    chequeo = fscanf(datos_de_entrada , "%u" , &true_length);

    // Verifico que haya leido un valor de tamaño
    if (chequeo!=1) {

        fprintf(stderr, "Invalid array.\n");
        exit(EXIT_FAILURE);

    }

    // Chequeo que el tamaño no sea mayor al tamaño maximo
    if (true_length > max_size) {

        fprintf(stderr, "Allowed size is %d.\n", max_size);
        exit(EXIT_FAILURE);

    }
    
    // Recorro el array original agregando los datos ingresado en consola separados por SPACE
    for (size_t i = 0; i < true_length; i++) {

        /*  Escaneo todos los valores de la terminal y se los asigno a array[i].
            Al mismo tiempo le voy asignando a chequeo el int que devuelve fscanf para ver que haya la misma cantidad de
            elementos que el tamaño.
            fscanf devuelve la cantidad de elementos que tomo, -1 si no pudo guardar nada y 0 si llega a EOF
        */
        chequeo = fscanf(datos_de_entrada , "%d" , &array[i]);

        // Si fscanf me devuelve -1 signifia que no pudo leer ningun elemento, por ende hay menos elementos que el tamaño ingresado        
        if (chequeo!=1) {
            
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);

        }
        
    }    
        
    // Devuelvo el tamaño verdadero del array
    return true_length;
}

// Imprimo el array en el formato que me piden
void array_dump(int a[], unsigned int length) {
    
    if (length <= 0) {

        printf("[]\n");

    } else {

        printf("[ ");

        for (size_t i = 0; i < length - 1; i++) {

            printf("%d, " , a[i]);

        }
        
        printf("%d]\n" , a[length-1]);

    }
}


int main(void) {
            
    /* create an array of MAX_SIZE elements */
    int array[MAX_SIZE];
    
    /* parse the file to fill the array and obtain the actual length */
    unsigned int length = array_from_stdin(array, MAX_SIZE);
    
    /*dumping the array*/
    array_dump(array, length);
    
    return EXIT_SUCCESS;
}
