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

// Estructura producto_t
typedef struct {

  int precio;
  int peso_en_kilos;

} producto_t;

// Estructura total_t
typedef struct {

  int precio_total;
  int peso_total;

} total_t;

void pedirArreglo(producto_t a[], int n_max){

  int i = 0 , precio , peso;

  producto_t dupla = {0 , 0};

  while (i < n_max) {

    printf("Datos del prodcuto numero %d:\n", i);

    printf("Ingrese el precio:\n");
    scanf("%d", &precio);

    dupla.precio = precio;

    printf("Ingrese el peso en kilos:\n");
    scanf("%d", &peso);

    dupla.peso_en_kilos = peso;

    a[i] = dupla;

    i++;
  }
}

// Funcion que devuelve una dupla de total_t
total_t calcular_montos(producto_t a[], int tam){

  int i = 0;

  total_t monto = {0 , 0};
 
  while (i < tam){

    monto.precio_total += a[i].precio;

    monto.peso_total += a[i].peso_en_kilos;

    i ++;
  }

  return monto;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  producto_t arreglo[N];
  
  pedirArreglo(arreglo, N);

  total_t dupla_final = calcular_montos(arreglo , N);

  printf("El precio total y la cantidad de kilos es:\n precio = %d ; kilos = %d \n" , dupla_final.precio_total , dupla_final.peso_total);
  
  return 0;
}