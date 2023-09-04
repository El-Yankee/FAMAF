#ifndef _STREXTRA_H_
#define _STREXTRA_H_


/*
 * Concatena las cadenas en s1 y s2 devolviendo nueva memoria (debe ser
 * liberada por el llamador con free())
 *
 * USAGE:
 *
 * merge = strmerge(s1, s2);
 *
 * REQUIRES:
 *     s1 != NULL &&  s2 != NULL
 *
 * ENSURES:
 *     merge != NULL && strlen(merge) == strlen(s1) + strlen(s2)
 *
 */
char * strmerge(char *s1, char *s2);

/*
 * Al realizar el make memtest encontramos varios memory leaks ocasionado por strmerge
 * Por eso decidimos crear esta funcion extra que nos libera la memoria extra
 */
char * strmerge_with_free(char *s1, char *s2);

#endif
