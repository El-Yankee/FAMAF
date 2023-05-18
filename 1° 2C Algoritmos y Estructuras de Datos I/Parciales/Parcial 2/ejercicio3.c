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

// Estructura de paridad_t
typedef struct {

  int maximo_par;
  int maximo_impar;

} paridad_t;

// Funcion que devuelve una dupla de paridad_t
paridad_t maximo_paridad(int a[], int tam){

  int i = 0;

  paridad_t dupla = {INT_MIN , INT_MIN};

  while (i < tam){

    if (a[i] % 2 == 0){

      if (a[i] >= dupla.maximo_par){

        dupla.maximo_par = a[i];

      }

    } else {

      if (a[i] >= dupla.maximo_impar){

        dupla.maximo_impar = a[i];
      }
    }

    i ++;
  }

  return dupla;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N];

  pedirArreglo(arreglo, N);

  paridad_t duplafinal = maximo_paridad(arreglo , N);

  printf("El maximo elemento par e impar son:\n par = %d ; impar = %d \n" , duplafinal.maximo_par , duplafinal.maximo_impar);

  return 0;
}