/*
  @file store.c
  @brief Implements store structure and methods
*/
#include <stdlib.h>
#include "store.h"

static const int AMOUNT_OF_STORE_VARS = 5;

Store store_from_file(FILE* file, char code, type_t type, unsigned int index)
{
    Store store;
    store.code = code;
    store.type = type;
    store.index = index;
    
    int res = fscanf(file , EXPECTED_PRICES_FILE_FORMAT , &store.potatoes , 
                                                          &store.cabbages,
                                                          &store.carrots,
                                                          &store.onions,
                                                          &store.radishes);

    if (res != AMOUNT_OF_STORE_VARS) {

      fprintf(stderr, "Prices format error.\n");
      exit(EXIT_FAILURE);
    }

    if (type == price){

      if( store.potatoes == 0 || 
          store.cabbages == 0 ||
          store.carrots == 0  ||
          store.onions == 0   || 
          store.radishes == 0 ) {

        fprintf(stderr, "There is a price equal to 0.\n");
        exit(EXIT_FAILURE);
      }
    }

    if (type == discount){

      if( store.potatoes == 0 || store.potatoes > 99 || 
          store.cabbages == 0 || store.cabbages > 99 ||
          store.carrots == 0  || store.carrots > 99 ||
          store.onions == 0   || store.onions > 99 ||
          store.radishes == 0 || store.radishes > 99 ) {

        fprintf(stderr, "There is a discount in wrong format.\n");
        exit(EXIT_FAILURE);
      }
    }

    if(type == discount){

      fscanf(file , "\n");

    }

    return store;
}
