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

  int r , s , R , S , aux ;

  printf("Valor de r:\n");
  scanf("%d" , &r);

  printf("Valor de s:\n");
  scanf("%d" , &s);

  // Variables para verificar la pre y post condicion  
  R = r;
  S = s;

  assert (r == R && s == S && S > R);

  aux = s;

  s = 2*r - 2*s ;

  r = 2*r + aux;

  assert (s == 2*R - 2*S && r == 2*R + S);

  printf("Los valores finales son:\n s = %d ; r = %d \n" , s , r);

  return 0;
}