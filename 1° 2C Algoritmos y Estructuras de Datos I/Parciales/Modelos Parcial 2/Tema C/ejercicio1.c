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

int main(void){

  int i , j , k ;

  printf("Elije el valor de i:\n");
  scanf("%d", &i);

  printf("Elije el valor de j:\n");
  scanf("%d", &j);

  printf("Elije el valor de k:\n");
  scanf("%d", &k);

  // Variables para verificar la pre y post condicion
  int I = i , J = j , K = k;

  assert (i == I && j == J && k == K && k > 0 && i > j);

  i = J + I;
  j = J + K;
  k = K + I;

  assert(i == J + I && j == J + K && k == K + I);

  printf("Los valores finales son:\n i=%d ; j=%d ; k=%d \n", i, j , k);

  return 0;
}
