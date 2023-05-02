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

// Estructura producto_t
typedef struct{

  int codigo;
  int cantidad;

} producto_t;

// Estructura stats_t
typedef struct{

  float cantidad_media;
  int cantidad_minima;
  int cantidad_maxima;

} stats_t;

void pedirArreglo(producto_t a[], int n_max){

  int i = 0 , codigo , cantidad ;
 
  producto_t producto;
  producto.codigo = 0;
  producto.cantidad = 0;

  while (i < n_max){

    printf("Datos del producto numero %d:\n", i + 1);

    printf("Ingrese el codigo:\n");
    scanf("%d", &codigo);

    producto.codigo = codigo;

    printf("Ingrese la cantidad:\n");
    scanf("%d", &cantidad);

    producto.cantidad = cantidad;

    a[i] = producto;

    i++;
  }
}

stats_t calcular_estadisticas(producto_t a[], int tam){

  int i = 0 ;
  float sum = 0;

  stats_t datos;
  datos.cantidad_media = 0;
  datos.cantidad_minima = INT_MAX;
  datos.cantidad_maxima = INT_MIN;

  while (i < tam){

    if (a[i].cantidad <= datos.cantidad_minima){

      datos.cantidad_minima = a[i].cantidad;

    }

    if (a[i].cantidad >= datos.cantidad_maxima){

      datos.cantidad_maxima = a[i].cantidad;
      
    }

    sum += a[i].cantidad ;

    i ++;
  }

  datos.cantidad_media = sum / tam ;

  return datos;
}

int main(void){

  producto_t arreglo[N];
  float cant_media;
  int cant_min , cant_max;

  pedirArreglo(arreglo, N);

  stats_t triofinal = calcular_estadisticas(arreglo, N);

  cant_media = triofinal.cantidad_media;
  cant_min = triofinal.cantidad_minima;
  cant_max = triofinal.cantidad_maxima;

  printf("La cantidad promedio, minima y maxima son:\n  prom = %f ; min = %d ; max = %d \n", cant_media, cant_min, cant_max);

  return 0;
}