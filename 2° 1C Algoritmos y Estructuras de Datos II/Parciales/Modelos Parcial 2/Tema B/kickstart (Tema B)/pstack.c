#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {

    unsigned int size;

    struct s_node *first;
};

struct s_node {

    pstack_elem elem;

    priority_t prio;

    struct s_node *next;

};


static struct s_node * create_node(pstack_elem e, priority_t priority) {

    struct s_node *new_node = malloc(sizeof(struct s_node));
    
    assert(new_node!=NULL);

    new_node->elem = e;
    new_node->prio = priority;
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


static bool invrep(pstack s) {

    bool inv = (s != NULL);

    if (inv && s->size != 0u && s->first != NULL){

        struct s_node *p = s->first;

        unsigned int length = 1u;

        while (p->next != NULL) {

            inv = inv && p->prio >= p->next->prio;
            p = p->next;
            length = length + 1u;
        }

        inv = inv && length == s->size;
    }
    
    return inv;
}

pstack pstack_empty(void) {

    pstack s = NULL;

    s = malloc(sizeof(struct s_pstack));

    s->size = 0u;
    s->first = NULL;

    assert(invrep(s) && pstack_is_empty(s));

    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {

    assert(invrep(s));

    struct s_node *new_node = create_node(e, priority);

    if (s == NULL) {

        s->size = 1u;
        s->first = new_node;

    } else {

        struct s_node *p;
        struct s_node *q;

        p = s->first;
        q = NULL;

        while (p != NULL && p->prio > priority) {

            q = p;
            p = p->next;
        }

        new_node->next = p;

        if (q != NULL) {

            q->next = new_node;

        } else {

            s->first = new_node;

        }

        s->size ++;

    }
    

    assert(invrep(s) && !pstack_is_empty(s));

    return s;
}

bool pstack_is_empty(pstack s) {
    
    return s->first == NULL;

}

pstack_elem pstack_top(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));

    pstack_elem e;

    e = s->first->elem;

    assert(invrep(s));

    return e;
}

priority_t pstack_top_priority(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));

    priority_t p;

    p = s->first->prio;

    assert(invrep(s));
    
    return p;
}

unsigned int pstack_size(pstack s) {

    assert(invrep(s));

    unsigned int size=0;
    
    size = s->size;

    return size;
}

pstack pstack_pop(pstack s) {

    assert(invrep(s) && !pstack_is_empty(s));

    struct s_node *killme = s->first;

    s->first = s->first->next;

    killme = destroy_node(killme);

    s->size --;

    assert(invrep(s));

    return s;
}

pstack pstack_destroy(pstack s) {

    assert(invrep(s));

    struct s_node *node = s->first;

    while (node != NULL) {

        struct s_node *killme = node;

        node = node->next;

        killme = destroy_node(killme);
    }

    free(s);

    s = NULL;

    assert(s == NULL);
    
    return s;
}

