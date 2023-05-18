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

void pedirArreglo(int a[], int n_max){

  int x, i = 0;

  while (i < n_max)  {

    printf("Valor para la posicion %d:\n", i);
    scanf("%d", &x);

    a[i] = x;
    i++;
  }
}

int indice_maximo_par(int a[], int tam){

  int i = 0 , indice = -1;

  while (i < tam){

    if (a[i] % 2 == 0){

      indice = i;

    }

    i ++;
  }

  return indice;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N], max_indice;

  pedirArreglo(arreglo, N);

  max_indice = indice_maximo_par(arreglo , N);

  printf("El indice mas grande que tiene un elemneto par es: \n indice = %d \n" , max_indice);

  return 0;
}