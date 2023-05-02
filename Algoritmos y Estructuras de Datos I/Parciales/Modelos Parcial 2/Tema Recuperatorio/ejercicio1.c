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

  int x, y, z;

  printf("Elije el valor de x:\n");
  scanf("%d", &x);

  printf("Elije el valor de y:\n");
  scanf("%d", &y);

  printf("Elije el valor de z:\n");
  scanf("%d", &z);

  // Variables para verificar la pre y post condicion
  int X = x, Y = y, Z = z;

  assert(x == X && y == Y && z == Z && Y > X && X > 0);

  x = Y;
  y = Z;
  z = (X / Y) + (X % Y);

  assert(x == Y && y == Z && z == X);

  printf("Los valores finales son:\n x=%d ; y=%d ; z=%d \n", x, y, z);

  return 0;
}