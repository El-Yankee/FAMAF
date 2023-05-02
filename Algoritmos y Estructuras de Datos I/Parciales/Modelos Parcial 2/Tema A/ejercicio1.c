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

  int r , s , R , S , aux;

  printf("Elije el valor de r:\n");
  scanf("%d" , &r);

  printf("Elije el valor de s:\n");
  scanf("%d", &s);

  // Variables para verificar la pre y post condicion
  R = r;
  S = s;

  assert (r == R && s == S && S > R);

  aux = r;

  r = s - r;

  s = aux + s;

  assert(r == S - R && s == R + S);

  printf("Los valores finales son:\n r=%d ; s=%d \n" , r , s);

  return 0;
}