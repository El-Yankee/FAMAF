/*
  @file store.h
  @brief Defines store prices and discounts information
*/

#ifndef _STORE_H
#define _STORE_H
#define EXPECTED_PRICES_FILE_FORMAT " %u %u %u %u %u"

typedef enum { price, discount } type_t;

#include <stdio.h>

/** @brief Type used to represent store data.*/
typedef struct _store
{
  char code; // store code
  type_t type;
  unsigned int index; // store number
  unsigned int potatoes;
  unsigned int cabbages;
  unsigned int carrots;
  unsigned int onions;
  unsigned int radishes;
} Store;

/**
 * @brief reads store data from file line
 * @details
 * Store file line must contain:
 * <unsigned int> <unsigned int> <unsigned int> <unsigned int> <unsigned int>
 *
 * the returned structure is filled with the data read from the file and also
 * from the suplied parameters
 *
 *
 * @param[in] file Opened file stream
 * @param[in] code The store code
 * @param[in] type The type of data
 * @param[in] index The store number
 *
 * @return Store structure which contains read information from file
 */
Store store_from_file(FILE* file, char code, type_t type, unsigned int index);

#endif //_STORE_H
