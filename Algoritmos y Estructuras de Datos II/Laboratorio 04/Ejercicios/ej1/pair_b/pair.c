#include <stdio.h>
#include <stdlib.h>

#include "pair.h"

pair_t pair_new(int x, int y){

    pair_t new_pair = NULL;

    // Como le voy a cargar datos, debo de reservar la memoria necesaria para la estructura
    new_pair = malloc(sizeof(pair_t));

    new_pair->fst = x;
    new_pair->snd = y;

    return new_pair;
}

int pair_first(pair_t p){

    return p->fst;

}

int pair_second(pair_t p){

    return p->snd;

}

pair_t pair_swapped(pair_t p){

    pair_t q = NULL;

    // Como le voy a cargar datos, debo de reservar la memoria necesaria para la estructura
    q = malloc(sizeof(pair_t));

	q->fst = p->snd;
	q->snd = p->fst;
	
	return q;

}

pair_t pair_destroy(pair_t p){

    free(p);
    p = NULL;

    return p;

}

