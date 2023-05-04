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
* @return True when is the last entry of the flight table, False otherwise
*/
static bool is_last_line(unsigned int hour, unsigned int type) {
  return  hour == HOURS - 1u && type == TYPE - 1u;
}

void array_dump(LayoverTable a) {
  for (unsigned int hour = 0u; hour < HOURS; ++hour) {
    for (unsigned int type = 0u; type < TYPE; ++type) {
      Flight f = a[hour][type];
      fprintf(stdout, "%c: %s at %u:00 with %u passengers", f.code, f.type == 0 ? "arrives" : "departs", f.hour - 1, f.passengers_amount);
      if (!is_last_line(hour, type))
      {
        fprintf(stdout, "\n");
      }
    }
  }
}


unsigned int passengers_amount_in_airport (LayoverTable a, unsigned int h) {

  unsigned amount_pas = 0u;

  for (size_t i = 0u; i <= h; i++) {

    amount_pas += a[i][0].passengers_amount;
    
    if (i != h) {

      amount_pas -= a[i][1].passengers_amount;

    }
  }

  return amount_pas;
}


void array_from_file(LayoverTable array, const char *filepath) {

  FILE *file = NULL;

  file = fopen(filepath, "r");

  if (file == NULL) {
    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  // Variables para guardar el codigo del vuelo y para ir pasando por todas las horas del dia
  char code;
  int i=0;

  // Como el arreglo se ordena por horas y tipo, uso i para ir pasando las horas
  while (i < 24) {

    // El codigo del vuelo esta entre '_', asique uso este formato para solamente leer la letra
    int res = fscanf(file , "_%c_" , &code);
    
    if (res != 1) {
      fprintf(stderr, "Invalid file. Error 1\n");
      exit(EXIT_FAILURE);
    }

    // Ahora leo los datos del vuelo y los guardo en su estructura dependiendo de su tipo
    Flight flight_arrival = flight_from_file(file , code);
    Flight flight_departure = flight_from_file(file , code);

    if (flight_arrival.hour > flight_departure.hour){

      fprintf(stderr, "Invalid file. Error 4\n");
      exit(EXIT_FAILURE);
    }


    // Una vez guardado los datos, los cargo al arreglo dependiendo de su hora y tipo
    array[flight_arrival.hour - 1][flight_arrival.type] = flight_arrival;
    array[flight_departure.hour - 1][flight_departure.type] = flight_departure;

    i++;
  }

  fclose(file);
}
