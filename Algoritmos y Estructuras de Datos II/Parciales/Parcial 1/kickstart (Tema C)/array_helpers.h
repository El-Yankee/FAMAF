/*
  @file array_helpers.h
  @brief defines array helpers methods. These methods operates over multidimensional array of prices
*/
#ifndef _ARRAY_HELPERS_H
#define _ARRAY_HELPERS_H

#include <stdbool.h>
#include "store.h"

#define STORES 26
#define TYPE 2

typedef Store PricesTable [STORES][TYPE];

/**
 * @brief Write the content of the array 'a' into stdout.
 * @param[in] a array to dump in stdout
 */
void array_dump(PricesTable a);

/**
 * @brief calculates best relative price for the store table.
 * @param[in] a array with data
 */
unsigned int best_relative_price(PricesTable a);


/**
 * @brief reads an array of prices information from file
 * @details
 *
 *  Each element is read from the file located at 'filepath'.
 *  The file must exist in disk and must have its contents in a sequence of
 *  lines, each with the following format:
 *
 *  _<store_code>_<store_num>_ <p> <cb> <ca> <o> <r> <p> <cb> <ca> <o> <r>
 *
 *   Those elements are copied into the multidimensional array 'a'.
 *   The dimensions of the array are given by the macros noted above.
 *
 * @param a array which will contain read file
 * @param filepath file with prices data
 */
void array_from_file(PricesTable a, const char *filepath);

#endif
