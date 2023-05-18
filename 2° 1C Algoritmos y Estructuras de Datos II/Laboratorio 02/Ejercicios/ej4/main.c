/**
 ** Para poder compilar este ejercicio usamos la sig linea en la consola
 * gcc -Wall -Werror -Wextra -pedantic -std=c99 -c array_helpers.c sort.c main.c
 * gcc -Wall -Werror -Wextra -pedantic -std=c99 -no-pie array_helpers.o sort.o sort_helpers.o main.o -o sorter
 * ./sorter ../input/example-unsorted.in
 * 
 **  La opción -no-pie tiene que ver con que se están “linkeando” los objetos array_helpers.o, sort.o y
 **  main.o compilados en nuestra computadora con el objeto precompilado sort_helpers.o, cuya compilación fue
 **  realizada en una computadora distinta
 *
 * * El goes_before(x , y) nos dice si X <= Y pero con valor absoluto
*/

/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Then, this project's includes, alphabetically ordered */
#include "array_helpers.h"
#include "sort_helpers.h"
#include "sort.h"

/* Maximum allowed length of the array */
static const unsigned int MAX_SIZE = 100000u;

void print_help(char *program_name) {
    /* Print the usage help of this program. */
    printf("Usage: %s <input file path>\n\n"
           "Sort an array given in a file in disk.\n"
           "\n"
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

    if (argc < 2) {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    result = argv[1];

    return result;
}

int main(int argc, char *argv[]) {

    char *filepath = NULL;

    /* parse the filepath given in command line arguments */
    filepath = parse_filepath(argc, argv);

    /* create an array of MAX_SIZE elements */
    int array[MAX_SIZE];

    /* parse the file to fill the array and obtain the actual length */
    unsigned int length = array_from_file(array, MAX_SIZE, filepath);

    /*

     ------------------- Selection Sort ------------------- 

    */

    /* create a copy of the array */
    int copy_selection_sort[MAX_SIZE];
    array_copy(copy_selection_sort, array, length);

    /* reset counters and set time */
    reset_comparisons_counter();
    reset_swaps_counter();
    set_current_time();

    /* do the actual sorting */
    selection_sort(copy_selection_sort, length);

    /* show statistics for selection_sort */
    printf("statistics for selection_sort\n");
    printf("time elapsed = %g,    comparisons: %10u,    swaps: %10u\n", calculate_elapsed_time(), comparisons_number(), swaps_number());
    printf("-----------------------------\n");

    /*
    
     ------------------- Insertion Sort ------------------- 

    */
    
    /* create a copy of the array */
    int copy_insertion_sort[MAX_SIZE];
    array_copy(copy_insertion_sort, array, length);

    /* reset counters and set time */
    reset_comparisons_counter();
    reset_swaps_counter();
    set_current_time();

    /* do the actual sorting */
    insertion_sort(copy_insertion_sort, length);

    /* show statistics for insertion_sort */
    printf("statistics for insertion_sort\n");
    printf("time elapsed = %g,    comparisons: %10u,    swaps: %10u\n", calculate_elapsed_time(), comparisons_number(), swaps_number());
    printf("-----------------------------\n");

    /*
    
     ------------------- Quick Sort ------------------- 

    */

    /* create a copy of the array */
    int copy_quick_sort[MAX_SIZE];
    array_copy(copy_quick_sort, array, length);

    /* reset counters and set time */
    reset_comparisons_counter();
    reset_swaps_counter();
    set_current_time();

    /* do the actual sorting */
    quick_sort(copy_quick_sort, length);

    /* show statistics for quick_sort */
    printf("statistics for quick_sort\n");
    printf("time elapsed = %g,    comparisons: %10u,    swaps: %10u\n", calculate_elapsed_time(), comparisons_number(), swaps_number());
    printf("-----------------------------\n");

    return EXIT_SUCCESS;
}
