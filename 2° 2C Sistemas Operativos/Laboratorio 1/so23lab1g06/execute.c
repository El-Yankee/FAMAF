#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tests/syscall_mock.h" // Para realizar los tests

#include "execute.h"
#include "builtin.h"

static char **myargs (scommand comm) {  

    unsigned int length = scommand_length(comm);

    // Reservamos memoria para guardar el arreglo
    char** arg = calloc(length + 1 , sizeof(char*));
    
    // Guardamos el comando y sus argumentos en el arreglo
    for (size_t i = 0; i < length; i++) {

        arg[i] = strdup(scommand_front(comm));
        scommand_pop_front(comm);
    }

    // Execvp nos pide que el arreglo termine en NULL
    arg[length] = NULL;
    
    return (arg);
}

static void set_redir_in (scommand comm) {

    char* new_in = scommand_get_redir_in(comm);

    // Abrimos el archivo de entrada
    // Lo abro solo en modo lectura
    int set_r_in = open(new_in , O_RDONLY , S_IRUSR); 
    
    // Reviso que se abra correctamente
    if (set_r_in < 0){

        fprintf(stderr,"Open Error: File doesnt exist\n");
    }
    
    // Duplico el set_r_in, cierro el STDIN_FILENO, abro el set_r_in
    int check = dup2(set_r_in , STDIN_FILENO);

    // Reviso que dup2 no falle
    if (check < 0) {

        fprintf(stderr,"Error\n");
    }  

    // Cerramos el archivo
    int file_close = close(set_r_in);
    
    // Reviso que se cierre correctamente
    if (file_close < 0) {

        fprintf(stderr,"Close Error\n");
    }
}

static void set_redir_out (scommand comm) {

    char* new_out = scommand_get_redir_out(comm);

    // Abrimos el archivo de salida
    // Lo abre en modo escritura, y si no existe lo crea con permisos de usuario
    int set_r_out = open (new_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); 
    
    // Reviso que se abra correctamente
    if (set_r_out < 0){

        fprintf(stderr,"Open Error: File doesnt exist\n");
    }
    
    // Duplico el set_r_out, cierro el STDOUT_FILENO, abro el set_r_out
    int check = dup2(set_r_out , STDOUT_FILENO);

    // Reviso que dup2 no falle
    if (check < 0) {

        fprintf(stderr,"Error\n");
    }  

    // Cerramos el archivo
    int file_close = close(set_r_out);
    
    // Reviso que se cierre correctamente
    if (file_close < 0) {

        fprintf(stderr,"Close Error\n");
    }
}

static void execute_scommand (scommand comm) {

    // Obtenemos el comando y sus argumentos
    char **args = myargs(comm);

    // Veo si hay redireccion de entrada
    if (scommand_get_redir_in(comm) != NULL) {
    
        set_redir_in(comm);
    } 

    // Veo si hay redireccion de salida
    if (scommand_get_redir_out(comm) != NULL) {
    
        set_redir_out(comm);
    }
    
    // Ejecuto el comando con sus argumentos
    int check = execvp(args[0] , args);

    // Reviso que no falle la ejecucion
    if (check < 0) {
        
        fprintf(stderr,"Error: command unrecognized\n");
        exit(EXIT_FAILURE);
    }   
    
}

static void exec_simple_command (pipeline apipe) {

    scommand comm = pipeline_front(apipe);

    // Veo si es una ejecucion en background o no
    bool must_wait = pipeline_get_wait(apipe);

    // Me fijo si es un comando interno
    if (builtin_is_internal(comm)) {

        builtin_run(comm);

    } else {

        // Al no ser un comando interno, realizo el fork
        pid_t pid = fork();
            
        if (pid < 0) {

            // Si el fork falla    
            fprintf(stderr,"Error on fork\n");
        
        } else if (pid == 0) {    
            
            // Ejecuto el comando en el hijo
            execute_scommand(comm);
        
        } else {

            // Tiene que esperar el padre?
            if (must_wait){

                waitpid(pid, NULL, 0);
            }
        }
    }
}

static void execute_single_pipe(pipeline apipe) {

    scommand com1 = pipeline_front(apipe);

    // Veo si es una ejecucion en background o no
    bool must_wait = pipeline_get_wait(apipe);

    // Donde la posicion 0 sera el READ_END el extremo de lectura y la posicion 1 el WRTIE_END el extremo de escritura del pipe
    int fd[2];

    pipe(fd);

    int id1 = fork();
            
    if (id1 < 0) {

        // Si el fork falla    
        fprintf(stderr,"Error on fork pipe\n");
    
    } else if (id1 == 0) {    
       
        // Ahora hago que lo que se hubiera escrito en SRDOUT_FILENO (salida de consola) se escriba al extemo de escritura del pipe
        int check1 = dup2(fd[1],STDOUT_FILENO);

        // dup2() returnea el file descriptor nuevo, pero si returnea -1 establece la variable global 'errno'
        if (check1 == -1) {

            fprintf(stderr,"Error: file descriptor copy \n");

        } else {

            // Tengo que leer lo de la consola para el primer scommand, cierro el file descriptor de lectura.
            close(fd[0]);
            close(fd[1]);

            // Ejecuto el comando en el hijo 1
            execute_scommand(com1);
        }

    } else {

        pipeline_pop_front(apipe);
        scommand com2 = pipeline_front(apipe);
        int id2 = fork();

        if (id2 < 0) {

            // Si el fork falla    
            fprintf(stderr,"Error on fork pipe hijo\n");
    
        } else if (id2 == 0) {    

            int check2 = dup2(fd[0],STDIN_FILENO);

            if (check2 == -1) {

                fprintf(stderr,"Error: file descriptor copy \n");

            } else {

                close(fd[0]);
                close(fd[1]);

                // Ejecuto el comando en el hijo 2
                execute_scommand(com2);
            }

        } else {

            close(fd[0]);
            close(fd[1]);
        }

        // Tiene que esperar el padre?
        if (must_wait){

            waitpid(id1, NULL, 0);
            waitpid(id2, NULL, 0);
        }
    }
}

void execute_pipeline (pipeline apipe) {

    unsigned int length = pipeline_length(apipe);

    if (builtin_alone(apipe)) {

        builtin_run(pipeline_front(apipe));

    } else if (length == 1) {

        exec_simple_command(apipe);

    } else if (length == 2) {

        execute_single_pipe(apipe);

    } else if (length > 2) {

        fprintf(stderr,"Error: Only one pipe supported\n");
    }
}