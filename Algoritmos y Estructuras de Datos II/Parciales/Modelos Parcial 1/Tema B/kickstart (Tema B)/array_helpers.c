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
* @brief returns true when reach last entry in flight table
* @return True when is the last entre of the flight table, False otherwise
*/
static bool is_last_line(unsigned int hour, unsigned int type) {
  return  hour == HOURS - 1u && type == TYPE - 1u;
}

void array_dump(DeliveryTable a) {
  for (unsigned int type = 0u; type < TYPE; ++type) {
    for (unsigned int hour = 0u; hour < HOURS; ++hour) {
      Flight f = a[type][hour];
      fprintf(stdout, "%c: flight with %u %s arrived at %u:00", f.code, f.items_amount, f.type == 0 ? "boxes" : "letters", f.hour - 1);
      if (!is_last_line(hour, type))
      {
        fprintf(stdout, "\n");
      }
    }
  }
}

unsigned int extra_space_fee_cost (DeliveryTable a) {

  unsigned int fee = 0u , amount_boxes = 0u , amount_letters = 0u;

  for (size_t i = 0; i <= FEE_CLOSE_HOUR; i++) {

    amount_boxes += a[boxes][i].items_amount;
    amount_letters += a[letters][i].items_amount;

  }

  if (amount_boxes > MAX_ALLOWED_BOXES) {

    fee += (amount_boxes - MAX_ALLOWED_BOXES) * BOX_PENALTY;

  }

  if (amount_letters > MAX_ALLOWED_LETTERS) {

    fee += (amount_letters - MAX_ALLOWED_LETTERS) * LETTER_PENALTY;

  }

  
  return fee;
}


void array_from_file(DeliveryTable array, const char *filepath) {

  FILE *file = NULL;

  file = fopen(filepath, "r");

  if (file == NULL) {

    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  char code;
  int i = 0;

  while (i < HOURS && !feof(file)) {

    int res = fscanf(file , "_%c_" , &code);

    if (res != 1) {
      fprintf(stderr, "Invalid file. Error 1\n");
      exit(EXIT_FAILURE);
    }
    
    Flight flight_boxes = flight_from_file(file , code , boxes);
    Flight flight_letters = flight_from_file(file , code , letters);

    array[boxes][flight_boxes.hour - 1] = flight_boxes;
    array[letters][flight_letters.hour - 1] = flight_letters;

    i ++;
  }
}
