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

// Estructura de cantidad_t
typedef struct {

  int n_multiplos_dos;
  int n_multiplos_tres;

} cantidad_t;

// Funcion que devuelve una tupla de cantidad_t
cantidad_t contar_multiplos(int a[], int tam){

  int i = 0;

  cantidad_t dupla;
  dupla.n_multiplos_dos = 0;
  dupla.n_multiplos_tres = 0;

  while (i < tam){

    if (a[i] % 2 == 0){

      dupla.n_multiplos_dos ++;

    }

    if (a[i] % 3 == 0){

      dupla.n_multiplos_tres ++;
    }

    i ++;

  }

  return dupla;
}

int main(void){

  int arreglo[N], multi_dos, multi_tres;

  pedirArreglo(arreglo, N);

  cantidad_t tuplafinal = contar_multiplos(arreglo, N);

  multi_dos = tuplafinal.n_multiplos_dos ;
  multi_tres = tuplafinal.n_multiplos_tres;

  printf("La cantidad de multiplos de 2 y 3 son:\n multi_dos = %d ; multi_tres = %d \n", multi_dos, multi_tres);

  return 0;
}