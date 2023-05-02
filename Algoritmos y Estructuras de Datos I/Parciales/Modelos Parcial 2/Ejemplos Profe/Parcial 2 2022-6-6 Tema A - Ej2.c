#include <stdio.h>


int suma_mayores(int a[], int tam, int k) {
    int res = 0;

    int i = 0;
    while (i < tam) {
        if (a[i] > a[k]) {
            res = res + a[i];
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

    int k;
    printf("Ingrese un valor para k: ");
    scanf("%d", &k);

    int res = suma_mayores(a, N, k);

    printf("res = %d\n", res);


    return 0;
}






