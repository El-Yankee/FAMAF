#include <stdio.h>


struct paridad_t {
    int n_pares;
    int n_impares;
};


struct paridad_t contar_paridad(int a[], int tam) {
    struct paridad_t res = {0, 0};

    int i = 0;
    while (i < tam) {

        if (a[i] % 2 == 0) {
            res.n_pares = res.n_pares + 1;
        }
        else {
            res.n_impares = res.n_impares + 1;
        }

        i = i+1;
    }

    return res;
}


void pedirArreglo(int a[], int n_max) {
    int j = 0;
    while (j < n_max) {
        printf("Ingrese el elemento %d del arreglo\n", j);
        scanf("%d", &a[j]);

        j = j+1;
    }
}


#define N 5

int main(void) {

    int a[N];

    pedirArreglo(a, N);

    struct paridad_t res = contar_paridad(a, N);

    printf("Cantidad de pares: %d\nCantidad de impares: %d\n", res.n_pares, res.n_impares);

    return 0;
}


