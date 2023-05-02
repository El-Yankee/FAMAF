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

bool todos_multiplos(int a[], int tam, int k){

  int i = 0;
  
  while (i < tam){

    if (a[i] % a[k] != 0){

      return false;

    }

    i ++;
  }

  return true;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N], k;
  bool res;

  pedirArreglo(arreglo, N);

  printf("Elegir un indice k del arreglo:\n");
  scanf("%d", &k);

  assert(k >= 0 && k < N && arreglo[k] != 0);

  res = todos_multiplos(arreglo , N , k);

  if (res == true){

    printf("Son todos multiplos de %d \n" , arreglo[k]);

  } else {

    printf("No son todos multiplos de %d \n", arreglo[k]);
  }  

  return 0;
}