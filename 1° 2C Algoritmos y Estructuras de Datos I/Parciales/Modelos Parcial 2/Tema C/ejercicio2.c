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

int suma_divisibles(int a[], int tam, int d){

  int i = 0 , sum = 0;

  while (i < tam){

    if (a[i] % d == 0){

      sum += a[i];

    }

    i ++;
  }

  return sum;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N] , d , sumtotal;

  pedirArreglo(arreglo, N);

  printf("Ingrese un numero d:\n");
  scanf("%d" , &d);

  sumtotal = suma_divisibles(arreglo , N , d);

  printf("La suma de los elementos divisibles por %d es:\n sum=%d \n" , d , sumtotal);

  return 0;
}