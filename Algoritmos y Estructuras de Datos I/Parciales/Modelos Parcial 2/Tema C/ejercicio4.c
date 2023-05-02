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

// Estructura persona_t
typedef struct{

  int dni;
  float altura;

} persona_t;

// Estructura paridad_t
typedef struct{

  float altura_media;
  float altura_minima;
  float altura_maxima;

} paridad_t;

void pedirArreglo(persona_t a[], int n_max){

  int i = 0, dni;
  float altura;

  persona_t persona = {0 , 0};  

  while (i < n_max){

    printf("Datos de la persona numero %d:\n", i + 1);

    printf("Ingrese el dni:\n");
    scanf("%d", &dni);

    persona.dni = dni;

    printf("Ingrese la altura:\n");
    scanf("%f", &altura);

    persona.altura = altura;

    a[i] = persona;

    i++;
  }
}

// Funcion que devuelve trio de struct
paridad_t calcular_estadisticas(persona_t a[], int tam){

  int i = 0;
  float sum = 0;

  paridad_t datos = {0 , INT_MAX , INT_MIN};  

  while (i < tam){

    if (a[i].altura <= datos.altura_minima){

      datos.altura_minima = a[i].altura;
    }

    if (a[i].altura >= datos.altura_maxima){

      datos.altura_maxima = a[i].altura;
    }

    sum += a[i].altura;

    i++;
  }

  datos.altura_media = sum / tam;

  return datos;
}

// Tamaño constante del arreglo
#define N 5

int main(void){

  persona_t arreglo[N];
  float min, prom, max;

  pedirArreglo(arreglo, N);

  paridad_t triofinal = calcular_estadisticas(arreglo, N);

  prom = triofinal.altura_media;
  min = triofinal.altura_minima;
  max = triofinal.altura_maxima;

  printf("La altura promedio, minima y maxima son:\n  prom = %f ; min = %f ; max = %f \n", prom, min, max);

  return 0;
}