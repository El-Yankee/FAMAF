#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {

    unsigned int size;

    priority_t max_prior;

    struct s_node **array;
    // array es un puntero a un arreglo de punteros

};

struct s_node {
    
    pstack_elem elem;

    struct s_node *next;

};

static struct s_node * create_node(pstack_elem e) {

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

static bool invrep(pstack s) {

    bool inv = (s != NULL);

    if (inv && s->size != 0u && s->array != NULL){

        struct s_node *p;

        unsigned int length = 0u;

        for (size_t i = 0; i <= s->max_prior; i++) {

            p = s->array[i];

            while (p != NULL) {

                p = p->next;

                length ++;
            }
        }

        inv = inv && length == s->size;
    }
    
    return inv;
}

pstack pstack_empty(priority_t max_priority) {

    pstack s = NULL;

    s = malloc(sizeof(struct s_pstack));

    s->size = 0u;
    s->max_prior = max_priority;
    s->array = calloc(max_priority + 1 , sizeof(struct s_node *));

    for (unsigned int i = 0u; i <= max_priority; i++){

        s->array[i] = NULL;
    }

    assert(invrep(s) && pstack_is_empty(s));

    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {

    assert(invrep(s) && priority <= s->max_prior);

    struct s_node *new_node = create_node(e);

    new_node->next = s->array[priority];
    s->array[priority] = new_node;

    s->size ++;

    assert(invrep(s) && !pstack_is_empty(s));

    return s;
}

bool pstack_is_empty(pstack s) {

    assert(invrep(s));
    
    bool empty = true;
    
    for (unsigned int i = 0u; i <= s->max_prior; i++) {

        empty = empty && (s->array[i] == NULL);
    }
    
    return empty;
}

pstack_elem pstack_top(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));

    pstack_elem e;

    unsigned int i = s->max_prior;

    while(s->array[i] == NULL){
        
        i --;
    }
    
    e = s->array[i]->elem;

    assert(invrep(s));

    return e;
}

priority_t pstack_top_priority(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));
    
    priority_t p = s->max_prior;

    while(s->array[p] == NULL){
        
        p --;
    }

    assert(invrep(s));

    return p;
}

size_t pstack_size(pstack s) {

    assert(invrep(s));

    return s->size;
}

pstack pstack_pop(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));

    priority_t i = pstack_top_priority(s);

    struct s_node *killme = s->array[i];

    s->array[i] = s->array[i]->next;

    killme = destroy_node(killme);

    s->size --;

    assert(invrep(s));

    return s;
}

pstack pstack_destroy(pstack s) {

    assert(invrep(s));

    while(!pstack_is_empty(s)){
        
        s = pstack_pop(s);
    }

    free(s->array);

    free(s);

    s = NULL;

    assert(s == NULL);

    return s;
}
