#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>         // Para poder comparar strings

#include <unistd.h>         // Para tener chdir
#include "tests/syscall_mock.h" // Para realizar los tests

#include "builtin.h"

#define builtin_amount 3

bool builtin_is_internal(scommand cmd) {

    assert(cmd != NULL);

    char *comm = NULL;
    bool res = false;

    // Si se añade un comando, aumentar el nro de builtin_amount
    const char *list_buildin[] = {"cd", "help","exit"};             

    if (!scommand_is_empty(cmd)) {

        comm = scommand_front(cmd);

        // Comparamos el comando ingresado con los buildin
        for (size_t i = 0 ; i < builtin_amount && !res ; i++) {

            res = res || (!strcmp(list_buildin[i] , comm));            
        }
    }

    return res;
}

bool builtin_alone(pipeline p) {

    assert(p != NULL);

    bool res = pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p));

    return res;
}

//Funcion encargada de ejecutar cd
static void run_cd(scommand cmd){

    // Elimino el comando y me quedo solo con el path
    scommand_pop_front(cmd);                        

    char *path = scommand_to_string(cmd);               
    // Estoy asumiendo que chdir tiene la robustez de que hacer si el path esta mal
    
    if (path == NULL) {

        char *home = getenv("HOME");
        chdir(home);

    } else {

        chdir(path);
    }
        
}

//Funcion encargada de ejecutar help
static void run_help(void) {

    printf("Usted esta usando el shell «Demetrios Padawans»\n"
            "Los autores del mismo son: Afonso Santiago, Angeli Mateo, DiForte Federico y Usaj Santiago\n"
            "Este shell tiene los siguientes comandos internos:\n"
            "\t-cd: se usa para moverse entre directorios\n"
            "\tcd <path>\n"
            "\tdonde <path> es el directorio al que quiere ir\n"
            "\t-help: le muestra este mensaje (lo acaba de usar)\n"
            "\t-exit: termina con el proceso del shell\n");
}

//Funcion encargada de ejecutar exit
static void run_exit(void){
    
    printf("Goodbye!\n");
    exit(EXIT_SUCCESS);
}

void builtin_run(scommand cmd){

    assert(builtin_is_internal(cmd));

    char *comm = scommand_front(cmd);    

    if (!strcmp(comm, "cd")){

        run_cd(cmd);

    } else if (!strcmp(comm, "help")) {

        run_help();

    } else if (!strcmp(comm, "exit")) {

        run_exit();
        
    }
}