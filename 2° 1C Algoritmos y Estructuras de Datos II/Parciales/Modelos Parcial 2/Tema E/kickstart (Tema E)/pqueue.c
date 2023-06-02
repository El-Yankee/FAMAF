#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pqueue.h"

struct s_pqueue {
    
    unsigned int size;

    struct s_node *front;
};

struct s_node {

    pqueue_elem elem;

    float average_grade;

    unsigned int approved_courses;

    float priority;

    struct s_node *next;
};


static float calculate_priority(float average_grade, unsigned int approved_courses) {

    float priority;

    priority = 0.5 * (average_grade/MAX_GRADE) + 0.5 * (approved_courses/TOTAL_COURSES);

    return priority;
}

static struct s_node *create_node(pqueue_elem e, float average_grade, unsigned int approved_courses) {

    struct s_node *new_node = malloc(sizeof(struct s_node));

    float priority = calculate_priority(average_grade, approved_courses);

    assert(new_node != NULL);
   
    new_node->elem = e;
    new_node->average_grade = average_grade;
    new_node->approved_courses = approved_courses;
    new_node->priority = priority;
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

    if (inv && q->size != 0u && q->front != NULL){

        struct s_node *p = q->front;

        unsigned int length = 1u;

        while (p->next != NULL) {

            inv = inv && p->priority >= p->next->priority;
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
    q->front = NULL;
    
    assert(invrep(q) && pqueue_is_empty(q));

    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, float average_grade, unsigned int approved_courses) {

    assert(invrep(q));

    struct s_node *new_node = create_node(e, average_grade, approved_courses);

    if (q == NULL){

        q->front = new_node;
        q->size = 1;

    } else {

        struct s_node *p;
        struct s_node *s;

        p = q->front;
        s = NULL;

        while (p != NULL && new_node->priority < p->priority) {

            s = p;

            p = p->next;
        }

        new_node->next = p;

        if (s != NULL){

            s->next = new_node;

        } else {

            q->front = new_node;

        }

        q->size ++;
    }

    assert(invrep(q) && !pqueue_is_empty(q));

    return q;
}

bool pqueue_is_empty(pqueue q) {

    return q->front == NULL;
}


pqueue_elem pqueue_peek(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    pqueue_elem e;

    e = q->front->elem;

    assert(invrep(q));

    return e;
}

float pqueue_peek_average_grade(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    float g;

    g = q->front->average_grade;

    assert(invrep(q));

    return g;
}

unsigned int pqueue_peek_approved_courses(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    unsigned int c;

    c = q->front->approved_courses;

    assert(invrep(q));

    return c;
}

float pqueue_peek_priority(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    float p;

    p = q->front->priority;

    assert(invrep(q));

    return p;
}

unsigned int pqueue_size(pqueue q) {

    assert(invrep(q));

    unsigned int size = 0;

    size = q->size;
   
    return size;
}

pqueue pqueue_copy(pqueue q) {

    assert(invrep(q));
    
    pqueue copy = pqueue_empty();

    struct s_node *clone = q->front;

    while (clone != NULL) {

        copy = pqueue_enqueue(copy, clone->elem, clone->average_grade, clone->approved_courses);

        clone = clone->next;
    }

    assert(invrep(q));

    return copy;
}

pqueue pqueue_dequeue(pqueue q) {

    assert(invrep(q) && !pqueue_is_empty(q));

    struct s_node *killme = q->front;

    q->front = q->front->next;

    killme = destroy_node(killme);

    q->size --;

    assert(invrep(q));

    return q;
}

pqueue pqueue_destroy(pqueue q) {

    assert(invrep(q));
    
    struct s_node *node = q->front;

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
