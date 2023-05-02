#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "array_helpers.h"
#include "sort_helpers.h"
#include "sort.h"

static unsigned int min_pos_from(int a[], unsigned int i, unsigned int length) {
    unsigned int min_pos = i;
    for (unsigned int j = i + 1; j < length; ++j) {
        if (!goes_before(a[min_pos],a[j])) {
            min_pos = j;
        }
    }
    return (min_pos);
}

void selection_sort(int a[], unsigned int length) {
    for (unsigned int i = 0u; i < length; ++i) {
        unsigned int min_pos = min_pos_from(a, i, length);
        swap(a, i, min_pos);
    }
}


static void insert(int a[], unsigned int i) {

    unsigned int j = i;

    /*  Aplicamos el algoritmo visto en el teorico. Con la diferencia que utilizamos el creiterio de
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
        insert(a, i);
    }
}


static unsigned int partition(int a[], unsigned int izq, unsigned int der) {

    unsigned int ppiv , i , j;

    /*  Hago el algoritmo tal cual al que vimos en el teorico.
        Con la diferencia que se ordena en base al criterio de goes_before().
    */

    ppiv = izq;
    i = izq + 1;
    j = der;

    while (i <= j) {

        if (goes_before(a[i] , a[ppiv])) {

            i++;

        } else if (goes_before(a[ppiv] , a[j] )) {
            
            j--;

        } else if (goes_before(a[ppiv] , a[i]) && goes_before(a[j] , a[ppiv])) {
            
            swap(a , i , j);
            i++;
            j--;

        }
    }

    swap(a , ppiv , j);
    ppiv = j;

    return ppiv;
}

static void quick_sort_rec(int a[], unsigned int izq, unsigned int der) {
    
    unsigned int ppiv = 0u;

    /*  Hago el mismo algoritmo visto en el teorico. Con un leve cambio debido al manejo de
        indices.
    */
    if (izq < der) {

        ppiv = partition(a , izq , der);

        /*  Si en este caso dejo el ppiv-1 como en el teorico, me va a fallar debido al uso del
            unsigned int, ya que si ppiv = 0 entonces deberia ir a -1, el cual no puede. 
            En mi caso uso la misma tecnica que use la primera vez que llame a quick_sort_rec.
        */
        quick_sort_rec(a , izq , (ppiv == 0u) ? 0u : ppiv - 1u);

        quick_sort_rec(a , ppiv + 1 , der);

    }
}

void quick_sort(int a[], unsigned int length) {
    quick_sort_rec(a, 0u, (length == 0u) ? 0u : length - 1u);
}
