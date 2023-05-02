#ifndef _ARRAY_HELPERS_H_
#define _ARRAY_HELPERS_H_

#include "mybool.h"

unsigned int array_from_file(int array[], unsigned int max_size, const char *filepath); 

void array_dump(int a[], unsigned int length);

mybool array_is_sorted(int a[], unsigned int length);

#endif