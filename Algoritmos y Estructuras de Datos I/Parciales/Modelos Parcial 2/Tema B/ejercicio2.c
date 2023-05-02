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

int suma_multiplos(int a[], int tam, int k){

  int i = 0 , sum = 0;

  while (i < tam){

    if (a[i] % a[k] == 0){

      sum += a[i];

    }

    i ++;
  }

  return sum;
}

int main(void){

  int arreglo[N] , k , sumtotal;

  pedirArreglo(arreglo, N);

  printf("Elegir un indice k del arreglo:\n");
  scanf("%d", &k);

  assert (k >= 0 && k < N);

  sumtotal = suma_multiplos(arreglo , N , k);

  printf("La suma de los elementos multiplos de %d es:\n sumatotal = %d \n" , arreglo[k] , sumtotal);

  return 0;
}