/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "array_helpers.h"

static const int EXPECTED_DIM_VALUE = 2;

static const char* CITY_NAMES[CITIES] = {"Cordoba", "Rosario", "Posadas", "Tilcara", "Bariloche"};
static const char* SEASON_NAMES[SEASONS] = {"low", "high"};

static void init_array(unsigned int a[CITIES * SEASONS]) {

    for(unsigned int i = 0u; i < CITIES * SEASONS; i = i + 1) {

        a[i] = CITIES;   // cualquier dato que no pertencezca a [0,4]
    }
}

static int array_value_count(unsigned int a[CITIES * SEASONS], unsigned int value) {

    unsigned int count = 0u;

    for (unsigned int i = 0u; i < CITIES * SEASONS; i = i + 1) {

    	if (a[i] == value) {

            count = count + 1;
        }
    }

    return count;
}

void array_dump(BakeryProductTable a) {

    for (unsigned int city = 0u; city < CITIES; ++city) {

        for (unsigned int season = 0u; season < SEASONS; ++season) {

            fprintf(stdout, "%s in %s season: flour (%u,%u) Yeast (%u,%u) Butter (%u,%u) Sales value %u",
                    CITY_NAMES[city], SEASON_NAMES[season], a[city][season].flour_cant,
                    a[city][season].flour_price, a[city][season].yeast_cant,
                    a[city][season].yeast_price, a[city][season].butter_cant,
                    a[city][season].butter_price, a[city][season].sale_value);
            fprintf(stdout, "\n");
        }
    }
}

unsigned int best_profit(BakeryProductTable a) {

    unsigned int max_profit = 0u, product_cost = 0u , market_value = 0u , profit = 0u ;

    for (size_t i = 0; i < CITIES; i++) {

        for (size_t j = 0; j < SEASONS; j++) {

            product_cost = (a[i][j].butter_cant * a[i][j].butter_price) +
                           (a[i][j].yeast_cant * a[i][j].yeast_price) +
                           (a[i][j].flour_cant * a[i][j].flour_price) ;

            market_value = a[i][j].sale_value;

            profit = market_value - product_cost;
        
            if (profit > max_profit) {

                max_profit = profit;

            }

            profit = 0u;

        }
    }

    return max_profit;
}

void array_from_file(BakeryProductTable array, const char* filepath) {

    FILE* file = NULL;

    file = fopen(filepath, "r");

    if (file == NULL) {

        fprintf(stderr, "File does not exist.\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    unsigned int a[CITIES * SEASONS];
    init_array(a);

    while (i < SEASONS * CITIES && !feof(file)) {

        unsigned int city , season;

        int res = fscanf(file , "##%u??%u" , &city , &season);

        if (res != EXPECTED_DIM_VALUE) {

            fprintf(stderr, "Invalid file.\n");
            exit(EXIT_FAILURE);
        }

        if (city >= CITIES || season >= SEASONS ) {

            fprintf(stderr, "Invalid city or season code.\n");
            exit(EXIT_FAILURE);
        }

        BakeryProduct product = bakery_product_from_file(file);

        a[i] = code;

        if(array_value_count(a, code) > EXPECTED_DIM_VALUE) {	

            fprintf(stderr, "More than 2 data for a city.\n");
            exit(EXIT_FAILURE);
        }

        array[city][season] = product;

        ++i;
    }

    if (i < SEASONS * CITIES){

        fprintf(stderr, "Missing info for cities.\n");
        exit(EXIT_FAILURE);

    }

    fclose(file);
}
