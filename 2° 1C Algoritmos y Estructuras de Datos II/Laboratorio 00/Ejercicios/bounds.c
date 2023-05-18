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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ARRAY_SIZE 4

// Estructura de bound_data
typedef struct {

    bool is_upperbound;
    bool is_lowerbound;
    bool exists;
    unsigned int where;

} bound_data;

// Funcion que nos dan en el enunciado, devuelve una struct bound_data
bound_data check_bound(int value, int arr[], unsigned int length) {

    bound_data res = {true , true , false, 0};
    
    unsigned int i = 0;

    while (i < length) {

        if (arr[i] < value ) {

            res.is_upperbound = true && res.is_upperbound;
            res.is_lowerbound = false && res.is_lowerbound;            

        } else if (arr[i] > value) {

            res.is_lowerbound = true && res.is_lowerbound;
            res.is_upperbound = false && res.is_upperbound; 
            
        }    

        if (arr[i] == value) {

            res.is_lowerbound = true && res.is_lowerbound;
            res.is_upperbound = true && res.is_upperbound;

            res.exists = true;
            res.where = i;

        }  
        
        i ++;

    }

    return res;
}

int main(void) {

    int arreglo[ARRAY_SIZE];
    int value , i = 0;

    while (i < ARRAY_SIZE) {

    printf ("Ingrese un valor: \n");
    scanf ("%d", &arreglo[i]);

    i++;

   }

    printf ("Ingrese cota: \n");
    scanf ("%d",&value);

    bound_data result = check_bound(value, arreglo, ARRAY_SIZE);

    if (result.exists && result.is_upperbound){

        printf("Es maximo y se encuentre en la posicion %d \n" , result.where);

    } else if (result.exists && result.is_lowerbound) {

        printf("Es minimo y se encuentre en la posicion %d \n", result.where);
        
    }
    
    if (!result.exists && result.is_upperbound) { 

        printf("Es cota superior\n");

    } 

    if (!result.exists && result.is_lowerbound) {

        printf("Es cota inferior\n");

    }    

    return EXIT_SUCCESS;
}

