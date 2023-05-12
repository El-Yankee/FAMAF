#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

/* list as a pointer to structs to guarantee encapsulation */
typedef struct _list *list;
typedef float list_elem;

/* Constructors */
list empty(void);
/*
    Create a new empty list.
*/

list addl(list_elem e , list l);
/*
    Add the element e to the beginning of the list l.
*/


/* Destroyers */
void destroy(list l);
/*
   Frees memory for l.
*/


/* Operations */
bool is_empty(list l);
/*
   Returns True if l is empty.
*/

list_elem head(list l);
/*
   Returns the first element of the list l.
   Pre: not is_empty(l)
*/

list tail(list l);
/*
   Deletes the first elemento of the list l.
   Pre: not is_empty(l)
*/

list addr(list l , list_elem e);
/*
   Adds the element e at the end of the list l.
*/

unsigned int length(list l);
/*
   Returns the amount of elementes on the list l.
*/

list concat(list l , list l0);
/*
   Adds at the end of the list l all elements of the list l0 in the same order.
*/

list_elem index(list l, unsigned int n);
/*
   Returns the element in the position n of the list l.
   Pre: length(l) > n
*/

list take(list l, unsigned int n);
/*
   Leaves in the list l only the n first elements and eliminates the rest.
*/

list drop(list l, unsigned int n);
/*
   Erase the n first elements of the list l.
*/

list copy_list(list l);
/*
   Copy all the elements of the list l.
*/

#endif