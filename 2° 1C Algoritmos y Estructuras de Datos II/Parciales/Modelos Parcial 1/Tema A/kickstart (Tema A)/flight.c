/*
  @file layover.c
  @brief Implements flight structure and methods
*/
#include <stdlib.h>
#include "flight.h"

static const int AMOUNT_OF_FLIGHT_VARS = 3 ;

Flight flight_from_file(FILE* file, char code) {

  Flight flight;
  flight.code = code;

  if (file != NULL) {

    int res = fscanf(file , EXPECTED_FLIGHT_FILE_FORMAT , &flight.type , &flight.hour , &flight.passengers_amount);

    // Hago control de los casos que pueden dar error
    if (res != AMOUNT_OF_FLIGHT_VARS) {

      fprintf(stderr, "Invalid file. Error 2\n");
      exit(EXIT_FAILURE);
    }

    if (flight.type != 0 && flight.type != 1) {

      fprintf(stderr, "Invalid file. Error 3\n");
      exit(EXIT_FAILURE);
    }

    if (flight.hour < 1 || flight.hour > 24) {

      fprintf(stderr, "Invalid file. Error 5\n");
      exit(EXIT_FAILURE);
    }

  }

  // En el caso de leer los datos de despegue, luego debo leer el salto de linea para leer el siguiente vuelo
  if (flight.type == 1) {

    fscanf(file , "\n");

  }

  return flight;
}
