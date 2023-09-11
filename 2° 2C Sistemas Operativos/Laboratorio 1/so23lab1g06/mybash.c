#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

#include "ANSI-color-codes.h" // Colores texto

static void show_prompt(void) {

    char host[256];
    gethostname(host, 256);

    char dir[2556];
    getcwd(dir, 2556);

    printf(BHWHT "mybash> " BHRED "%s:"  BHMAG "%s" reset "$ ", host, dir);
    
    fflush(stdout);
}

int main(int argc, char *argv[]) {

    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);

    while (!quit) {

        show_prompt();

        pipe = parse_pipeline(input);

        if (pipe != NULL){

            execute_pipeline(pipe);

            pipe = pipeline_destroy(pipe);
        }

        quit = parser_at_eof(input);
    }

    input = parser_destroy(input);
    
    input = NULL;

    return EXIT_SUCCESS;
}

