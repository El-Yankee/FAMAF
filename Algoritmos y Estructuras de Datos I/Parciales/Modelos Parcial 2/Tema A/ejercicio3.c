/**
 ** Para poder compilar los archivos usamos la sig linea en la consola
 * gcc -Wall -Wextra -std=c99 miarchivo.c -o miprograma
 * ./miprograma
 *
 * * Y para utilizar el debugger
 * gcc -Wall -Wextra -std=c99 -g miarchivo.c -o miprograma
 * gdb ./miprograma
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#define N 5

void pedirArreglo(int a[], int n_max){

  int x, i = 0;

  while (i < n_max){

    printf("Valor para la posicion %d:\n", i);
    scanf("%d", &x);

    a[i] = x;
    i++;
  }
}

// Estructura de paridad_t
typedef struct {

  int n_pares;
  int n_impares;

} paridad_t;

// Funcion que devuelve una tupla de paridad_t
paridad_t contar_paridad(int a[], int tam){

  int i = 0;

  paridad_t tupla;
  tupla.n_pares = 0;
  tupla.n_impares = 0;

  while (i < tam){

    if (a[i] % 2 == 0){

      tupla.n_pares ++; 

    } else {

      tupla.n_impares ++;

    }

    i ++;
  }

  return tupla;
}

int main(void){

  int arreglo[N] , pares , impares;

  pedirArreglo(arreglo, N);

  paridad_t tuplafinal = contar_paridad (arreglo , N);

  pares = tuplafinal.n_pares;
  impares = tuplafinal.n_impares;
  
  printf("La cantidad de valores pares e impares es:\n par = %d ; impares = %d \n" , pares , impares);

  return 0;
}