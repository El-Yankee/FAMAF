
// En este archivo se encuntre el "que" hacen las funciones

// Se agrega esta libreria por el bool
#include <stdbool.h>

/**
 ** Prototipos funcion array_from_file:
 * Esta funcion lee un archivo y nos devuelve el tamaño del arreglo que se encuentra en el.
   Tambien asigna los valores encontrados dentro del archivo a nuestro arreglo creado originalmente. 
*/
unsigned int array_from_file(int *array, unsigned int max_size, const char *filepath);

/**
 ** Prototipos funcion array_dump:
 * Esta funcion toma el arreglo modificado por la funcion array_from_file y la imprime en pantalla.
*/
void array_dump(int *a, unsigned int length);

/**
 ** Prototipos funcion array_is_sorted:
 * Esta funcion nos dice si el arreglo esta ordenado de menor a mayor.
*/
bool array_is_sorted(int *a, unsigned int length);