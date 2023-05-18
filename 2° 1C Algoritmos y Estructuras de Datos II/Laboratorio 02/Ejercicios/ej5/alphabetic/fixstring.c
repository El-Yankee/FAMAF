#include <stdbool.h>
#include <assert.h>

#include "fixstring.h"


unsigned int fstring_length(fixstring s) {
    
    unsigned int i = 0u;

    // Recorremos toda la palabra buscando el \0 que nos indica el final
    while (s[i] != '\0') {

        i++;

    }

    // Contamos uno mas porque se debe considerar el lugar que ocupa el \0
    i++;

    return i;
}

bool fstring_eq(fixstring s1, fixstring s2) {
    
    bool equal = true;
    unsigned int i = 0u;

    // Lo mas rapido es verificar si tienen la misma cantidad de letras, sino ya sabemos que no son iguales
    equal = fstring_length(s1) == fstring_length(s2);

    // Ahora que se que tienen la misma cantidad de letras las comparo una a una
    while (equal && (s1[i] != '\0' && s2[i] != '\0')) {

        equal = equal && (s1[i] == s2[i]);
        i++;

    }

    return equal;
}

bool fstring_less_eq(fixstring s1, fixstring s2) {
    
    bool less_equal;
    unsigned int i = 0u;

    // Primero verifico si son la misma palabra
    less_equal = fstring_eq(s1 , s2);

    // Si no los son, entonces veo que las letras no sean las mismas y avanzo hasta encontrar unas que no lo son
    while (!less_equal && (s1[i] == s2[i])) {

        i++;

    }

    // Una vez encontradas letras diferentes y si s2 no es mas corta que s1, entonces chequeo cual es menor
    if (!less_equal && s2[i] != '\0') {

        less_equal = (s1[i] <= s2[i]);

    }

    return less_equal;
}

void fstring_set(fixstring s1, const fixstring s2) {

    int i=0;

    while (i<FIXSTRING_MAX && s2[i]!='\0') {

        s1[i] = s2[i];
        i++;
    }

    s1[i] = '\0';
}

void fstring_swap(fixstring s1,  fixstring s2) {

    fixstring aux;

    // Es igual al swap de los elementos de un arreglo. Usamos la ayuda de un aux

    fstring_set(aux , s1);

    fstring_set(s1 , s2);
    fstring_set(s2 , aux);

}


