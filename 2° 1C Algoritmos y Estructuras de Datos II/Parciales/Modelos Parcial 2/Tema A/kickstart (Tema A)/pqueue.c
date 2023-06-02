#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pqueue.h"

struct s_pqueue {

    unsigned int size;

    struct s_node *first;
};

struct s_node {

    pqueue_elem elem;

    unsigned int prio;

    struct s_node *next;
};

static struct s_node *create_node(pqueue_elem e, unsigned int priority) {

    struct s_node *new_node = malloc(sizeof(struct s_node));
    
    assert(new_node!=NULL);
    
    new_node->elem = e;
    new_node->prio = priority;
    new_node->next = NULL;
    
    return new_node;
}

static struct s_node *destroy_node(struct s_node *node) {

    assert(node != NULL);
    
    node->next = NULL;

    free(node);

    node = NULL;

    assert(node == NULL);

    return node;
}

static bool invrep(pqueue q) {

    bool inv = (q != NULL);

    if (inv && q->size != 0u && q->first != NULL){

        struct s_node *p = q->first;

        unsigned int length = 1u;

        while (p->next != NULL) {

            inv = inv && p->prio <= p->next->prio;
            p = p->next;
            length = length + 1u;
        }

        inv = inv && length == q->size;
    }
    
    return inv;
}

pqueue pqueue_empty(void) {

    pqueue q = NULL;

    q = malloc(sizeof(struct s_pqueue));

    q->size = 0u;
    q->first = NULL;

    assert(invrep(q) && pqueue_is_empty(q));

    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, unsigned int priority) {

    assert(invrep(q));

    struct s_node *new_node = create_node(e, priority);
    
    if (q == NULL) {

        q->first = new_node;
        q->size = 1;

    } else {

        struct s_node *p; 
        struct s_node *s;

        p = q->first;
        s = NULL;

        while (p != NULL && priority >= p->prio) {

            s = p;

            p = p->next;
        }

        new_node->next = p;

        if (s != NULL) {
            
            s->next = new_node;

        } else {

            q->first = new_node;
        }

        q->size +=1;
        
    }

    assert(invrep(q) && !pqueue_is_empty(q));
    
    return q;
}

bool pqueue_is_empty(pqueue q) {

    return q->first == NULL;
    
}

pqueue_elem pqueue_peek(pqueue q) {
    
    assert(invrep(q) && !pqueue_is_empty(q));

    pqueue_elem e;

    e = q->first->elem;

    assert(invrep(q));

    return e;
}

unsigned int pqueue_peek_priority(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    unsigned int pri;

    pri = q->first->prio;

    assert(invrep(q));
    
    return pri;
}

unsigned int pqueue_size(pqueue q) {

    assert(invrep(q));

    unsigned int size=0;

    size = q->size;
   
    return size;
}

pqueue pqueue_dequeue(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    struct s_node *killme = q->first;

    q->first = q->first->next;

    killme = destroy_node(killme);

    q->size --;

    assert(invrep(q));

    return q;
}

pqueue pqueue_destroy(pqueue q) {

    assert(invrep(q));
    
    struct s_node *node = q->first;

    while (node != NULL) {

        struct s_node *killme = node;

        node = node->next;

        killme = destroy_node(killme);
    }

    free(q);

    q = NULL;

    assert(q == NULL);

    return q;
}
