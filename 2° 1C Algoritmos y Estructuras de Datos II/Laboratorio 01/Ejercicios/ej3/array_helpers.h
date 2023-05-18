
// En este archivo se encuntre el "que" hacen las funciones

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