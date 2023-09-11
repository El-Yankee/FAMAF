#include <assert.h>
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

    // Recorremos todo el comando tomando los datos necesarios
    // arg es el dato y type el tipo del dato
    while (arg != NULL) {

        if (type == ARG_NORMAL) {

            // Si es de tipo normal lo agregamos
            scommand_push_back(command , arg);

        } else if (type == ARG_INPUT) {

            // Si es redireccion de entrada, la seteamos
            scommand_set_redir_in(command , arg);

        } else if (type == ARG_OUTPUT) {

            // Si es redireccion de salida, la seteamos
            scommand_set_redir_out(command , arg);
        }

        // Busco el siguiente dato
        arg = parser_next_argument(p, &type);
    }

    // Cuando el arg esta vacio, vemos cual fue el ultimo tipo que encontro
    // Se puede dar el caso de tener redirecciones vacias, lo cual estan mal
    // Tambien puede pasar que nunca se agrego nada ya que estaba vacio
    if(type == ARG_INPUT || type == ARG_OUTPUT || scommand_is_empty(command)) {
        
        command = scommand_destroy(command);

        command = NULL;
    }
  
    return command;
}  


pipeline parse_pipeline(Parser p) {

    assert(p != NULL && !parser_at_eof(p)); 

    // Creamos el pipeline donde guardaremos los scommands
    pipeline result = pipeline_new();

    scommand cmd = NULL;

    bool error = false, another_pipe = true;

    // Recorremos mientra no tengamos errores y haya pipes
    while (another_pipe && !error) {
        
        // Buscamos el primer scommand
        cmd = parse_scommand(p);

        // Vemos si hubo algun error
        error = (cmd == NULL); 

        // Si el comando que obtuvimos esta bien, lo guardamos en el pipe
        if (!error) {

            // Guardamos el scommand en el pipeline
            pipeline_push_back(result , cmd);

            // Vemos si hay un pipe
            parser_op_pipe(p, &another_pipe);
        }
    }
        
    // Si todavia no llegue al final, busco si hay un &
    if (!parser_at_eof(p)) {

        bool is_background;

        parser_op_background(p, &is_background);

        // Indico si debe esperar o no
        pipeline_set_wait(result, !is_background); 

        // Una vez pasado el &, borramos todo lo que sobre
        parser_skip_blanks(p); 
        parser_garbage(p, &error);
    }

    // Si tuvimos un error o el pipe esta vacio, borramos todo
    if (error || pipeline_is_empty(result)) {

        result = pipeline_destroy(result);
        result = NULL;
    }

    return result; 
}