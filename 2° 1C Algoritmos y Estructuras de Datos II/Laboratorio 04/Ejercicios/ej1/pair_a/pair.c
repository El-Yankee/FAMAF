#include <stdio.h>
#include <stdlib.h>

#include "pair.h"

pair_t pair_new(int x, int y){

    pair_t new_pair;

    new_pair.fst = x;
    new_pair.snd = y;

    return new_pair;
}

int pair_first(pair_t p){

    return p.fst;

}

int pair_second(pair_t p){

    return p.snd;

}

pair_t pair_swapped(pair_t p){

    pair_t q;
    
	q.fst = p.snd;
	q.snd = p.fst;
	
	return q;

}

pair_t pair_destroy(pair_t p){

    return p;

}

