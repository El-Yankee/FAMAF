
// En este archivo se encuntre el "como" hacen las funciones

// Incluimos los prototipos de las funciones encontrados en el .h
#include "array_helpers.h"

// Incluimos el resto de librerias necesarias
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


unsigned int array_from_file(int array[], unsigned int max_size, const char *filepath) {
    
    unsigned int true_length = 0u;
    int chequeo = 0;

    // Creo un puntero de archivo para luego pasarlo a las funciones fopen, fscanf y fclose
    FILE *archivo_de_entrada = NULL;

    // Le digo que abra el archivo que se encuentra en filepath en modo lectura
    archivo_de_entrada = fopen(filepath , "r");
    
    // Hago chequeo de que se haya abierto el archivo
    if (archivo_de_entrada == NULL) {

        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);

    }

    // Veo el tamaño del array en el archivo y se lo asigno a true_length
    chequeo = fscanf(archivo_de_entrada , "%u" , &true_length);

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
    
    // Recorro el array agregando los datos del archivo
    for (size_t i = 0; i < true_length; i++) {

        /*  Escaneo todos los valores del archivo y se los asigno a array[i].
            Al mismo tiempo le voy asignando a chequeo el int que devuelve fscanf para ver que haya la misma cantidad de
            elementos que el tamaño.
            fscanf devuelve la cantidad de elementos que tomo, -1 si no pudo guardar nada y 0 si llega a EOF
        */
        chequeo = fscanf(archivo_de_entrada , "%d" , &array[i]);

        // Si fscanf me devuelve -1 signifia que no pudo leer ningun elemento, por ende hay menos elementos que el tamaño ingresado        
        if (chequeo!=1) {
            
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);

        }
        
    }    
    
    // Cierro el archivo de lectura
    fclose(archivo_de_entrada);

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

// Veo si el arreglo esta ordenado
bool array_is_sorted(int a[], unsigned int length) {

    bool ordenado = true;

    for (size_t i = 0; i < length-1; i++) {

        if (a[i] <= a[i+1]) {

            ordenado = true && ordenado;

        } else {

            ordenado = false;

        }

    }
    
    return ordenado;
}