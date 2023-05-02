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

void pedirArreglo(int a[], int n_max){

  int x, i = 0;

  while (i < n_max){

    printf("Valor para la posicion %d:\n", i);
    scanf("%d", &x);

    a[i] = x;
    i++;
  }
}

// Estructura de s_paridad_t
typedef struct {

  int max_par;
  int min_par;
  int sum_par;

} s_paridad_t;

// Funcion que devuelve una trio de s_paridad_t
s_paridad_t stats_paridad(int a[], int tam){

  int i = 0;

  s_paridad_t trio = {INT_MIN , INT_MAX , 0};

  while (i < tam){

    if (a[i] % 2 == 0){

      if (a[i] >= trio.max_par){

        trio.max_par = a[i];

      }

      if (a[i] <= trio.min_par){

        trio.min_par = a[i];
      }

      trio.sum_par += a[i];
    }

    i ++;
  }

  return trio;

}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N];

  pedirArreglo(arreglo, N);

  s_paridad_t res = stats_paridad(arreglo , N);

  printf("El maximo , minimo y suma de los pares es: \n max=%d ; min=%d ; sum=%d \n" , res.max_par , res.min_par , res.sum_par);

  return 0;
}