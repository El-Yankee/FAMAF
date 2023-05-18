/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_helpers.h"

/**
* @brief returns true when reach last line in prices file
* @return True when is the last line of the file, False otherwise
*/
static bool is_last_line(unsigned int store) {
  return store == STORES - 1u;
}

static void init_array(unsigned int a[STORES]) {

  for (unsigned int i = 0u; i < STORES; i = i + 1) {

    a[i] = STORES;
  }
}

static int array_value_count(unsigned int a[STORES], unsigned int value) {

  unsigned int count = 0u;

  for (unsigned int i = 0u; i < STORES; i = i + 1) {

    if (a[i] == value) {

      count = count + 1;
    }
  }

  return count;
}

void array_dump(PricesTable a) {
  for (unsigned int store = 0u; store < STORES; ++store) {
      Store sp = a[store][price];
      Store sd = a[store][discount];
      fprintf(stdout, "store %c %u: Potato: %u (%u), Cabbage: %u (%u), Carrot: %u (%u), Onion: %u (%u), Radish: %u (%u)",
                      sp.code, sd.index,
                      sp.potatoes, sd.potatoes,
                      sp.cabbages, sd.cabbages,
                      sp.carrots, sd.carrots,
                      sp.onions, sd.onions,
                      sp.radishes, sd.radishes
                    );
      if (!is_last_line(store))
      {
        fprintf(stdout, "\n");
      }
    }
}

unsigned int best_relative_price (PricesTable a) {

  unsigned int minimo = __INT_MAX__ , count = 0u;

  for (size_t i = 0; i < STORES; i++) {

    count = (a[i][price].potatoes * (100 - a[i][discount].potatoes)) +
            (a[i][price].cabbages * (100 - a[i][discount].cabbages)) +
            (a[i][price].carrots * (100 - a[i][discount].carrots)) +
            (a[i][price].onions * (100 - a[i][discount].onions)) +
            (a[i][price].radishes * (100 - a[i][discount].radishes));

    count = count / 100;

    if (count < minimo) {

      minimo = count;

    }

    count= 0u;

  }
  
  return minimo;
}

void array_from_file(PricesTable array, const char *filepath) {
  FILE *file = NULL;

  file = fopen(filepath, "r");
  if (file == NULL) {
    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  char code;
  unsigned int index;

  int i = 0;

  unsigned int a[STORES];
  init_array(a);

  while (i < STORES && !feof(file)) {

    int res = fscanf(file, "_%c_%u_" , &code , &index);

    if (res != 2) {
      fprintf(stderr, "Invalid file.\n");
      exit(EXIT_FAILURE);
    }

    if (index > STORES) {
      fprintf(stderr, "Wrong store number.\n");
      exit(EXIT_FAILURE);
    }

    Store store_prices = store_from_file(file , code , price , index);
    Store store_discounts = store_from_file(file , code , discount , index);

    a[i] = code;

    if (array_value_count(a, code) > 1) {

      fprintf(stderr, "More than one data for a same store.\n");
      exit(EXIT_FAILURE);
    }

    array[index-1][price]= store_prices;
    array[index-1][discount]= store_discounts;

    i++;
  }

  if (i < STORES){

    fprintf(stderr, "Not enough stores.\n");
    exit(EXIT_FAILURE);
  }
  fclose(file);
}
