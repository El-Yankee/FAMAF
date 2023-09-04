#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "command.h"
#include "strextra.h"

//----------- Scommand ----------- 

/* Definimos la estructura del scommand
 * Es una tupla con 3 elementos del tipo
 * ( [char*] , char* , char* )
 */
struct scommand_s {

    // La lista de los argumentos
    GSList* arg;
    // Redirector de entrada
    char* redir_in;
    // Redirector de salida
    char* redir_out;
};

scommand scommand_new(void){

    // Reservamos memoria para la estructura
    scommand result = malloc(sizeof(struct scommand_s));

    result->arg = NULL;
    result->redir_in = NULL;
    result->redir_out = NULL;

    assert(result != NULL && scommand_is_empty (result) &&
            scommand_get_redir_in (result) == NULL &&
            scommand_get_redir_out (result) == NULL);

    return result;
}

scommand scommand_destroy(scommand self){

    assert(self != NULL);

    // Liberamos memoria usada por la lista y los redirectores
    g_slist_free_full(self->arg, free);
    free(self->redir_in);
    free(self->redir_out);

    self->arg = NULL;
    self->redir_in = NULL;
    self->redir_out = NULL;

    free(self);
    self = NULL;

    assert(self == NULL);

    return self;
}

void scommand_push_back(scommand self, char * argument){

    assert(self!=NULL && argument!=NULL);

    // Agregamos argument al final de la lista
    self->arg = g_slist_append(self->arg , argument);

    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){

    assert(self!=NULL && !scommand_is_empty(self));

    /* Al usar GSList tenemos 2 funciones que podemos unir para resolver lo que se pide.
    Una funcion para obtener el primer elemento.
    Y la otra funcion para eliminar de la lista el elemento que le demos.
    */

    gpointer first = g_slist_nth_data(self->arg, 0u);

    self->arg = g_slist_remove(self->arg, first);

    // Liberamos la memoria utilizada para guardar el primer elemento
    free(first);
}

void scommand_set_redir_in(scommand self, char * filename){

    assert(self!=NULL);

    /*
     * Al ejecutar el make memtest vimos que no contemplamos 
     * el caso en que se modifica un redirector ya seteado
    */

    if (self->redir_in == NULL) {

        self->redir_in = filename;

    } else {

        // Liberamos el anterior y seteamos el nuevo
        free(self->redir_in);        
        self->redir_in = filename;
    }

}

void scommand_set_redir_out(scommand self, char * filename){

    assert(self!=NULL);

    /*
     * Al ejecutar el make memtest vimos que no contemplamos 
     * el caso en que se modifica un redirector ya seteado
    */

    if (self->redir_out == NULL) {

        self->redir_out = filename;

    } else {

        // Liberamos el anterior y seteamos el nuevo
        free(self->redir_out);        
        self->redir_out = filename;
    }
}

bool scommand_is_empty(const scommand self){

    assert(self != NULL);

    return (self->arg == NULL);
}

unsigned int scommand_length(const scommand self){

    assert(self!=NULL);

    // Obtenemos la cantidad de elementos en la lista
    unsigned int size = g_slist_length(self->arg);

    assert((size==0) == scommand_is_empty(self));

    return size;
}

char * scommand_front(const scommand self){

    assert(self!=NULL && !scommand_is_empty(self));

    // Tomamos el primer elemento de la lista
    // 1° elemento = posicion 0 de la lista
    // guint = unsigned int
    char * result = g_slist_nth_data(self->arg , 0u);

    assert(result!=NULL);

    return result;
}

char * scommand_get_redir_in(const scommand self){

    assert(self!=NULL);

    return self->redir_in;
}

char * scommand_get_redir_out(const scommand self){

    assert(self!=NULL);

    return self->redir_out;
}

char * scommand_to_string(const scommand self){

    assert(self!=NULL);

    // Debo recorrer todo el scommand y convertir todo a un string

    // Creo el string vacio
    char * result = strdup("");

    unsigned int length = scommand_length(self);

    // Recorro la lista de argumentos
    for (size_t i = 0; i < length; i++) {

        result = strmerge_with_free(result , g_slist_nth_data(self->arg , i));
        // Ponemos un espacio entre argumentos, excepeto al ultimo argumento
        if (i < (length - 1)) {

            result = strmerge_with_free(result , " ");
        }
       
    }

    // Agrego el redirector de salida, si es que hay
    if (self->redir_out != NULL) {

        result = strmerge_with_free(result , " > ");
        result = strmerge_with_free(result , self->redir_out);
    }

    // Hago lo mismo con el redirector de entrada
    if (self->redir_in != NULL) {

        result = strmerge_with_free(result , " < ");
        result = strmerge_with_free(result , self->redir_in);
    }

    assert(scommand_is_empty(self) ||
            scommand_get_redir_in(self)==NULL || 
            scommand_get_redir_out(self)==NULL ||
            strlen(result)>0);

    return result;
}


//----------- Pipeline -----------  

/* Definimos la estructura del pipeline
 * Es una tupla con elementos del tipo
 * ( [scommand] , bool )
 */
struct pipeline_s {

    // La lista de los scommands
    GSList* cmds;
    // Debe esperar o no?
    bool wait;
};

pipeline pipeline_new(void){

    // Reservamos memoria para la estructura
    pipeline result = malloc(sizeof(struct pipeline_s));

    result->cmds = NULL;
    result->wait = TRUE;

    assert(result != NULL &&
            pipeline_is_empty(result) &&
            pipeline_get_wait(result));

    return result;
}

pipeline pipeline_destroy(pipeline self){

    assert(self != NULL);

    while (!pipeline_is_empty(self)) {
        
        pipeline_pop_front(self);
    }

    free(self->cmds);
    self->cmds = NULL;

    free(self);
    self = NULL;

    assert(self == NULL);

    return self;
}

void pipeline_push_back(pipeline self, scommand sc){

    assert(self!=NULL && sc!=NULL);

    self->cmds = g_slist_append(self->cmds, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self){

    assert(self!=NULL && !pipeline_is_empty(self));
    
    gpointer first = g_slist_nth_data(self->cmds, 0u);

    self->cmds = g_slist_remove(self->cmds, first);

    // Destruimos el scommands eliminado del pipeline
    scommand_destroy(first);
}

void pipeline_set_wait(pipeline self, const bool w){

    assert(self!=NULL);

    self->wait = w;
}

bool pipeline_is_empty(const pipeline self){

    assert(self!=NULL);

    return(self->cmds == NULL);
}

unsigned int pipeline_length(const pipeline self){

    assert(self!=NULL);

    unsigned int size = g_slist_length(self->cmds);

    assert((size==0) == pipeline_is_empty(self));

    return size;
}

scommand pipeline_front(const pipeline self){

    assert(self!=NULL && !pipeline_is_empty(self));

    scommand result= g_slist_nth_data(self->cmds, 0u);

    assert(result!=NULL);

    return result;
}

bool pipeline_get_wait(const pipeline self){

    assert(self!=NULL);
    
    return self->wait;
}

char * pipeline_to_string(const pipeline self){

    assert(self!=NULL);

    // Debo recorrer todo el pipeline y convertir todo a un string

    // Creo el string vacio
    char * result = strdup("");

    unsigned int length = pipeline_length(self);

    // Recorro la lista de scommands
    for (size_t i = 0; i < length; i++) {

        scommand sc = g_slist_nth_data(self->cmds, i);

        char *scommand_string = scommand_to_string(sc);

        result = strmerge_with_free(result , scommand_string);

        // Ponemos el pipe entre scommands, excepeto al ultimo scommand
        if (i < (length - 1)) {

            result = strmerge_with_free(result , " | ");
        }

        free(scommand_string);
    }

    if (!self->wait){

        result = strmerge_with_free(result , " &");
    }

    assert(pipeline_is_empty(self) ||
            pipeline_get_wait(self) ||
            strlen(result)>0);

    return result;
}