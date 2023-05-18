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

// Estructura de s_suma_t
typedef struct{

  int suma_pares;
  int suma_impares;
  int suma_total;

} s_suma_t;

// Funcion que devuelve un trio de s_suma_t
s_suma_t sumas(int a[], int tam){

  int i = 0;

  s_suma_t trio = {0 , 0 , 0};

  while (i < tam){

    if (a[i] % 2 == 0){

      trio.suma_pares += a[i];

    } else {

      trio.suma_impares += a[i];

    }

    trio.suma_total += a[i];

    i ++;
  }
  
  return trio;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  int arreglo[N];

  pedirArreglo(arreglo, N);

  s_suma_t res = sumas(arreglo , N);
  
  printf("La suma de pares , de impares y la sum total es:\n sum_par = %d ; sum_impares = %d ; sum_total = %d\n", res.suma_pares , res.suma_impares, res.suma_total);

  return 0;
}