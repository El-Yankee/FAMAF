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

// Estructura persona_t
typedef struct{

  int dni;
  float altura;

} persona_t;

// Estructura stats_t
typedef struct {

  float altura_media;
  float altura_minima;
  float altura_maxima;

} stats_t;

void pedirArreglo(persona_t a[], int n_max){

  int i = 0 , dni;
  float altura;

  persona_t persona;
  persona.dni = 0;
  persona.altura = 0;

  while (i < n_max){

    printf("Datos de la persona numero %d:\n" , i+1);

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
stats_t calcular_estadisticas(persona_t a[], int tam){

  int i = 0 ;
  float sum = 0;  

  stats_t datos;
  datos.altura_media = 0;
  datos.altura_minima = INT_MAX;
  datos.altura_maxima = INT_MIN;

  while (i < tam){

    if (a[i].altura <= datos.altura_minima){

      datos.altura_minima = a[i].altura;

    }

    if (a[i].altura >= datos.altura_maxima){

      datos.altura_maxima = a[i].altura;
      
    }

    sum += a[i].altura;

    i ++;
  }

  datos.altura_media = sum / tam;

  return datos;
}

int main(void){

  persona_t arreglo[N];
  float min , prom , max;

  pedirArreglo(arreglo, N); 

  stats_t triofinal = calcular_estadisticas (arreglo , N);

  prom = triofinal.altura_media;
  min = triofinal.altura_minima;
  max = triofinal.altura_maxima;

  printf("La altura promedio, minima y maxima son:\n  prom = %f ; min = %f ; max = %f \n", prom, min, max);

  return 0;
}