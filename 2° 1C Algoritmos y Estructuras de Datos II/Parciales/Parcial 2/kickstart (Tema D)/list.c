#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "list.h"

struct _node_t {

  unsigned int key;

  list_value value;
  
  struct _node_t *next;
};

typedef struct _node_t *node_t;

struct _list_t {
  
  unsigned int size;

  node_t elems;
};

static bool invrep(list_t list) {

  bool inv = (list != NULL);

  if (inv && list->size != 0u && list->elems != NULL){

    node_t p = list->elems;

    unsigned int length = 1u;

    while (p->next != NULL) {

        inv = inv && p->key < p->next->key;
        p = p->next;
        length = length + 1u;
    }

    inv = inv && length == list->size;
  }
    
  return inv;
}

static struct _node_t * create_node(unsigned int key, list_value value) {

  node_t new_node = malloc(sizeof(struct _node_t));
    
  assert(new_node!=NULL);
  
  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;
  
  return new_node;
}

static struct _node_t *destroy_node(struct _node_t *node) {

  assert(node != NULL);
    
  node->next = NULL;

  free(node);

  node = NULL;

  assert(node == NULL);

  return node;
  
}

list_t list_empty(void) {

  list_t list = NULL;

  list = malloc(sizeof(struct _list_t));

  list->size = 0u;
  list->elems = NULL;

  assert(invrep(list) && list_length(list) == 0);

  return list;
}

list_t list_add(list_t list, unsigned int key, list_value value) {

  assert(invrep(list));
    
  if (list == NULL) {

    node_t new_node = create_node(key,value);

    list->elems = new_node;
    list->size = 1;

  } else {

    node_t p; 
    node_t s;

    p = list->elems;
    s = NULL;

    while (p != NULL && key > p->key) {

      s = p;

      p = p->next;
    }

    if (p != NULL && key == p->key) {

      p->value = value;

    } else {

      node_t new_node = create_node(key,value);

      new_node->next = p;

      if (s != NULL) {
          
        s->next = new_node;

      } else {

        list->elems = new_node;
      }

      list->size +=1;
      
    }
  }

  assert(invrep(list));

  return list;
}

unsigned int list_length(list_t list) {

  assert(invrep(list));

  return list->size;
}

bool list_is_empty(list_t list) {

  assert(invrep(list));

  return list->size == 0 && list->elems == NULL;
}

bool list_exists(list_t list, unsigned int key) {

  assert(invrep(list));
  node_t cursor = list->elems;
  while (cursor != NULL && cursor->key < key) {
    cursor = cursor->next;
  }
  return cursor != NULL && cursor->key == key;
}

char list_search(list_t list, unsigned int key) {

  assert(invrep(list));

  char value;

  node_t p = NULL;

  while (p != NULL && p->key != key) {

    p = p->next;
  }

  if (p != NULL) {

    value = p->value;

  }
 
  assert((p != NULL) == list_exists(list, key));

  return value;
}

list_t list_remove(list_t list, unsigned int key) {

  assert(invrep(list));

  if (list_exists(list , key)){

    node_t p;
    node_t s;

    p = list->elems;
    s = NULL;

    while (p->next != NULL && p->next->key != key) {

      p = p->next;
    }

    s = p->next;

    p->next = p->next->next;

    s = destroy_node(s);

    s = NULL;

    list->size --;
  }

  assert(invrep(list) && !list_exists(list, key));

  return list;

}

list_t list_remove_all(list_t list) {

  assert(invrep(list));

  node_t node = list->elems;

  while (node != NULL) {

    node_t killme = node;

    node = node->next;

    killme = destroy_node(killme);

    list->size --;
  }

  list->elems = NULL;

  assert(invrep(list) && list_length(list) == 0);

  return list;

}


list_value* list_to_array(list_t list) {

  assert(invrep(list));

  unsigned int length = list_length(list);

  node_t node = list->elems;

  list_value *array = calloc(length , sizeof(list_value));

  for (size_t i = 0; i < length; i++) {

    array[i] = node->value;

    node = node->next;
  }
  
  assert(invrep(list) && !list_is_empty(list));

  return array;
}

list_t list_destroy(list_t list) {

  assert(invrep(list));
    
  node_t node = list->elems;

  while (node != NULL) {

    node_t killme = node;

    node = node->next;

    killme = destroy_node(killme);
  }

  free(list);

  list = NULL;

  assert(list == NULL);

  return list;
}
