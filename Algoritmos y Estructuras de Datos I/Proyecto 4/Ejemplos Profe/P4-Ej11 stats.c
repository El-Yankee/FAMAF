#include <assert.h>

struct datos_t {
    float maximo;
    float minimo;
    float promedio;
};


struct datos_t stats(float a[], int tam) {
    assert(tam >= 1);

    struct datos_t res;
    res.maximo = a[0];
    res.minimo = a[0];

//    float suma = a[0];

    res.promedio = a[0] / tam;

    int i = 1;
    while (i < tam) {
        if (a[i] > res.maximo) {
            res.maximo = a[i];
        }

        if (a[i] < res.minimo) {
            res.minimo = a[i];
        }

//        suma = suma + a[i];

        res.promedio = res.promedio + a[i] / tam;

        i = i + 1;
    }

//    res.promedio = suma / tam;

    return res;
}


