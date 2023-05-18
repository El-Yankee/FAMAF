#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "array_helpers.h"
#include "sort_helpers.h"
#include "sort.h"


static void insert(int a[], unsigned int i) {

    unsigned int j = i;

    /*  Aplicamos el algoritmo visto en el teorico. Con la diferencia que utilizamos el criterio de
        goes_before para comparar. Como en el teorico se usa otro criterio para los valores de los indices del arreglo,
        aca debemos de restarle 1 a los valores de los indices para que concuerde con nuestro lenguaje.
    */

    while (j > 0 && goes_before(a[j] , a[j-1]) ) {

        swap(a , j , j-1);

        j--;

    }
}

void insertion_sort(int a[], unsigned int length) {

    for (unsigned int i = 1u; i < length; ++i) {

        assert(array_is_sorted(a , i));

        insert(a, i);

    }
}
