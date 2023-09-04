#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {

    // Creamos el scommand donde guardaremos el comando
    scommand command = scommand_new();

    arg_kind_t type;
    char *arg = parser_next_argument(p, &type);

    // Revisamos si esta vacio o que lo primero que aparece sea un comando. Si no lo es, entonces el orden es incorrecto.
    if (type != ARG_NORMAL || arg == NULL) {

        return NULL;
    }

    // Vamos agregando el comando, sus argumentos y sus redirecciones a command
    // Esta implementacion es algo flexible. El formato puede ser:
    // cmd -arg -arg ... > redir out < redir in
    // cmd -arg -arg ... < redir in > redir out 

    // Primero buscamos el comando y sus argumentos (si tiene)
    while (type == ARG_NORMAL && arg != NULL && !parser_at_eof(p)) {

        scommand_push_back(command, arg);
        arg = parser_next_argument(p, &type);
    }

    // Luego buscamos las redirecciones (si tiene)
    while (arg != NULL) {

        if (type == ARG_OUTPUT) {

            scommand_set_redir_out(command, arg);
            arg = parser_next_argument(p, &type);

        } else if (type == ARG_INPUT) {

            scommand_set_redir_in(command, arg);
            arg = parser_next_argument(p, &type);
            
        }
    }
  
    return command;
}  


pipeline parse_pipeline(Parser p) {

    // Creamos el pipeline donde guardaremos los scommands
    pipeline result = pipeline_new();

    scommand cmd = NULL;

    bool error = false, another_pipe=true;

    // Buscamos el primer scommand
    cmd = parse_scommand(p);

    error = (cmd==NULL); /* Comando inválido al empezar */

    while (another_pipe && !error) {
    
        // Guardamos el scommand en el pipeline
        pipeline_push_back(result , cmd);

        // Quitamos los espacios en blanco
        parser_skip_blanks(p);

        // Vemos si hay un pipe
        parser_op_pipe(p, &another_pipe);

        if (another_pipe) {

            // Si hay un pipe, buscamos el siguiente scommand
            cmd = parse_scommand(p);
        }
        
        // Vemos si el scommand cumple el formato
        error = (cmd==NULL);
    }

    /* Opcionalmente un OP_BACKGROUND al final */
    bool is_background;

    // Vemos si se encuentra el & al final
    parser_op_background(p, &is_background);
    
    if (is_background) {
        
        // Si se encuentra entonces indicamos que no se debe esperar
        pipeline_set_wait(result , false);
    }

    /* Tolerancia a espacios posteriores */
    parser_skip_blanks(p);

    /* Consumir todo lo que hay inclusive el \n */
    bool garbage;
    parser_garbage(p, &garbage);

    /* Si hubo error, hacemos cleanup */
    if (error) {

        result = pipeline_destroy(result);
        result = NULL;
    }

    return result; 
}
