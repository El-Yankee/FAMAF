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
#include <assert.h>

int pedirEntero(void){

  int x;

  printf("Ingresar un numero entero:\n");
  scanf("%d", &x);

  return x;
}

void imprimeEntero(int x){

  printf("El entero es %d\n", x);
  
}

int main(void){

  int x, y, z;

  x = pedirEntero();
  y = pedirEntero();
  z = pedirEntero();

  // Variables para verificar la pre y post condicion
  int X = x, Y = y, Z = z;

  assert(x == X && y == Y && z == Z && Y != 0 && z > 0);

  x = Y + Z;
  y = Z % Y;
  z = X / Y;

  assert(x == Y + Z && y == Z % Y && z == X / Y);

  imprimeEntero(x);
  imprimeEntero(y);
  imprimeEntero(z);

  return 0;
}
