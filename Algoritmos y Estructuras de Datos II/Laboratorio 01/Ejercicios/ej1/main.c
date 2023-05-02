/**
 ** Para poder compilar un archivo reader usamos la sig linea en la consola
 * gcc -Wall -Werror -Wextra -pedantic -std=c99 -o reader main.c
 * ./reader ../input/example-easy.in
 */

/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Maximum allowed length of the array */
#define MAX_SIZE 100000

void print_help(char *program_name) {
    /* Print the usage help of this program. */
    printf("Usage: %s <input file path>\n\n"
           "Loads an array given in a file in disk and prints it on the screen."
           "\n\n"
           "The input file must have the following format:\n"
           " * The first line must contain only a positive integer,"
           " which is the length of the array.\n"
           " * The second line must contain the members of the array"
           " separated by one or more spaces. Each member must be an integer."
           "\n\n"
           "In other words, the file format is:\n"
           "<amount of array elements>\n"
           "<array elem 1> <array elem 2> ... <array elem N>\n\n",
           program_name);
}

char *parse_filepath(int argc, char *argv[]) {
    /* Parse the filepath given by command line argument. */
    char *result = NULL;
    // Program takes exactly two arguments
    // (the program's name itself and the input-filepath)
    bool valid_args_count = (argc == 2);

    if (!valid_args_count) {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    result = argv[1];

    return result;
}

unsigned int array_from_file(int array[], unsigned int max_size, const char *filepath) {
    
    unsigned int true_length = 0u;
    int chequeo = 0;

    // Creo un puntero de archivo para luego pasarlo a las funciones fopen, fscanf y fclose
    FILE *archivo_de_entrada = NULL;

    // Le digo que abra el archivo que se encuentra en filepath en modo lectura
    archivo_de_entrada = fopen(filepath , "r");
    
    // Hago chequeo de que se haya abierto el archivo
    if (archivo_de_entrada == NULL) {

        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);

    }

    // Veo el tamaño del array en el archivo y se lo asigno a true_length
    chequeo = fscanf(archivo_de_entrada , "%u" , &true_length);

    // Verifico que haya leido un valor de tamaño
    if (chequeo!=1) {

        fprintf(stderr, "Invalid array.\n");
        exit(EXIT_FAILURE);

    }

    // Chequeo que el tamaño no sea mayor al tamaño maximo
    if (true_length > max_size) {

        fprintf(stderr, "Allowed size is %d.\n", max_size);
        exit(EXIT_FAILURE);

    }
    
    // Recorro el array agregando los datos del archivo
    for (size_t i = 0; i < true_length; i++) {

        /*  Escaneo todos los valores del archivo y se los asigno a array[i].
            Al mismo tiempo le voy asignando a chequeo el int que devuelve fscanf para ver que haya la misma cantidad de
            elementos que el tamaño.
            fscanf devuelve la cantidad de elementos que tomo, -1 si no pudo guardar nada y 0 si llega a EOF
        */
        chequeo = fscanf(archivo_de_entrada , "%d" , &array[i]);

        // Si fscanf me devuelve -1 signifia que no pudo leer ningun elemento, por ende hay menos elementos que el tamaño ingresado        
        if (chequeo!=1) {
            
            fprintf(stderr, "Invalid array.\n");
            exit(EXIT_FAILURE);

        }
        
    }    
    
    // Cierro el archivo de lectura
    fclose(archivo_de_entrada);

    // Devuelvo el tamaño verdadero del array
    return true_length;
}

// Imprimo el array en el formato que me piden
void array_dump(int a[], unsigned int length) {
    
    if (length <= 0) {

        printf("[]\n");

    } else {

        printf("[ ");

        for (size_t i = 0; i < length - 1; i++) {

            printf("%d, " , a[i]);

        }
        
        printf("%d]\n" , a[length-1]);

    }
}


int main(int argc, char *argv[]) {
    char *filepath = NULL;

    /* parse the filepath given in command line arguments */
    filepath = parse_filepath(argc, argv);
    
    /* create an array of MAX_SIZE elements */
    int array[MAX_SIZE];
    
    /* parse the file to fill the array and obtain the actual length */
    unsigned int length = array_from_file(array, MAX_SIZE, filepath);
    
    /*dumping the array*/
    array_dump(array, length);
    
    return EXIT_SUCCESS;
}
