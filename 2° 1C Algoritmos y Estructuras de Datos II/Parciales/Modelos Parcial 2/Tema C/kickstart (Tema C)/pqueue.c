#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

struct s_pqueue {

    unsigned int size;

    priority_t min_prior;

    struct s_node **array;
    // array es un puntero a un arreglo de punteros

};

struct s_node {

    pqueue_elem elem;

    struct s_node *next;

};

static struct s_node * create_node(pqueue_elem e) {

    struct s_node *new_node = malloc(sizeof(struct s_node));
    
    assert(new_node!=NULL);
    
    new_node->elem = e;    
    new_node->next = NULL;
    
    return new_node;
}

static struct s_node * destroy_node(struct s_node *node) {

    assert(node != NULL);
    
    node->next = NULL;

    free(node);

    node = NULL;

    assert(node == NULL);

    return node;
}

/*
Respuesta para invrep que tambien es valida:
En la implementacion, los nodos no acarrean su prioridad.
No hay relacion entre size y max_priority.
Asumo entonces que el invariante de representaciòn solo sera s != NULL.
*/

static bool invrep(pqueue q) {

    bool inv = (q != NULL);

    if (inv && q->size != 0u && q->array != NULL){

        struct s_node *p;

        unsigned int length = 0u;

        for (size_t i = 0; i <= q->min_prior; i++) {

            p = q->array[i];

            while (p != NULL) {

                p = p->next;

                length ++;
            }
        }

        inv = inv && length == q->size;
    }
    
    return inv;
}

pqueue pqueue_empty(priority_t min_priority) {

    pqueue q = NULL;

    q = malloc(sizeof(struct s_pqueue));

    q->size = 0u;
    q->min_prior = min_priority;
    q->array = calloc(min_priority + 1 , sizeof(struct s_node *));

    for (unsigned int i = 0u; i <= min_priority; i++){

        q->array[i] = NULL;
    }
    
    assert(invrep(q) && pqueue_is_empty(q));

    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, priority_t priority) {

    assert(invrep(q) && priority <= q->min_prior);

    struct s_node *new_node = create_node(e);
    
    if (q->array[priority] == NULL) {

        q->array[priority] = new_node;

    } else {

        struct s_node *p; 

        p = q->array[priority];
        
        while (p->next != NULL) {

            p = p->next;
        }

        p->next = new_node;
        
    }

    q->size +=1;

    assert(invrep(q) && !pqueue_is_empty(q));
    return q;
}

bool pqueue_is_empty(pqueue q) {

    assert(invrep(q));
    
    bool empty = true;

    for (unsigned int i = 0u; i <= q->min_prior; i++) {
        
        empty = empty && (q->array[i] == NULL);
    }
    
    return empty;
}

pqueue_elem pqueue_peek(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    pqueue_elem e;

    unsigned int i = 0;

    while(q->array[i] == NULL){
        
        i ++;
    }
    
    e = q->array[i]->elem;

    assert(invrep(q));

    return e;
}

priority_t pqueue_peek_priority(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    priority_t p = 0;

    while(q->array[p] == NULL){
        
        p ++;
    }

    assert(invrep(q));

    return p;
}

size_t pqueue_size(pqueue q) {

    assert(invrep(q));

    return q->size;
}

pqueue pqueue_copy(pqueue q){

    assert(invrep(q));
    
    pqueue copy = pqueue_empty(q->min_prior);

    for (size_t i = 0; i <= q->min_prior; i++) {

        struct s_node *clone = q->array[i];

        while (clone != NULL) {

            copy = pqueue_enqueue(copy , clone->elem , i);

            clone = clone->next;
        }
    }

    assert(invrep(q));

    return copy;

}

pqueue pqueue_dequeue(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    priority_t i = pqueue_peek_priority(q);

    struct s_node *killme = q->array[i];

    q->array[i] = q->array[i]->next;

    killme = destroy_node(killme);

    q->size --;

    assert(invrep(q));

    return q;
}

pqueue pqueue_destroy(pqueue q) {

    assert(invrep(q));

    while(!pqueue_is_empty(q)){
        
        q = pqueue_dequeue(q);
    }

    free(q->array);

    free(q);

    q = NULL;

    assert(q == NULL);

    return q;
}
