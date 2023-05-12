#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

struct _list {

    list_elem elem;
    list next;
    
};

list empty(void){

    list l = NULL;

    return l;
}

list addl(list_elem e , list l){

    struct _list *p = NULL;

    p = malloc(sizeof(struct _list));

    p->elem = e;
    p->next = l;

    l = p;

    return l;
}


bool is_empty(list l){

    bool b;

    b = (l == NULL);

    return b;
}

list_elem head(list l){

    assert(!is_empty(l));

    list_elem e;

    e = l->elem;

    return e;
}

list tail(list l){

    assert(!is_empty(l));

    struct _list *p = NULL;

    p = l;

    l = l->next;

    free(p);

    return l;
}

list addr(list l , list_elem e){

    struct _list *p = NULL , *q = NULL;

    q = malloc(sizeof(struct _list));

    q->elem = e;
    q->next = NULL;

    if (!is_empty(l)) {

        p = l;

        while (p->next != NULL) {

            p = p->next;
        }

        p->next = q;

    } else {

        l = q;

    }

    return l;
}

unsigned int length(list l){

    struct _list *p = NULL;

    unsigned int n = 0u;

    p = l;

    while (p != NULL) {
        
        n ++;
        p = p->next;
    }

    return n;
}

void destroy(list l){

    struct _list *p = NULL;
    
    p = l;
    
    if (p != NULL) {

        while (p->next != NULL) {

            l = tail(l);

            p = l;
        }
        
        free(p);

        p =  NULL;
        l = NULL;
    }
}

list concat(list l , list l0){

    struct _list *p = NULL;

    if (!is_empty(l)) {

        p = l;

        while (p->next != NULL){

            p = p->next;
        }

        p->next = l0;

    } else {

        l = l0;

    }

    return l;
}

list_elem index(list l, unsigned int n){

    assert(length(l) >= n);
    
    struct _list *p = NULL;
    list_elem e;
    unsigned int count = 0u;

    p = l;

    while (count <= n) {

        e = p->elem;
        p = p->next;
        count ++;
    }
    
    return e;    
}

list take(list l, unsigned int n){

    struct _list *p = NULL;

    unsigned int count = 1u;

    p = l;

    if (n <= length(l)) {

        while (count <= n) {

            p = p->next;
        }

        destroy(p);
    }

    return l;
}

list drop(list l, unsigned int n){

    struct _list *p = NULL;

    unsigned int count = 0u;
    
    while (count < n && l != NULL) {

        p = l;
        l = p->next;
        free(p);
        count ++;
    }

    return l;
}

list copy_list(list l) {

    struct _list *p = NULL , *q = NULL;

    list l0 = NULL;

    q = malloc(sizeof(struct _list));

    l0 = q;
    p = l;

    for (size_t i = 0; i <= length(l); i++) {

        q->elem = p->elem;
        q = q->next;
        p = p->next;
        q = malloc(sizeof(struct _list));
    }
    
    free(q);
    q = NULL;

    return l0;
}
