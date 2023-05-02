/**
 ** Para poder compilar los archivos usamos la sig linea en la consola
 * gcc -Wall -Wextra -pedantic -std=c99 miarchivo.c -o miprograma
 * ./miprograma
 *
 * * Y para utilizar el debugger
 * gcc -Wall -Wextra -std=c99 -g miarchivo.c -o miprograma
 * gdb ./miprograma
 *
 */

#include <stdlib.h>  /* exit() y EXIT_FAILURE */
#include <stdio.h>   /* printf(), scanf()     */
#include <stdbool.h> /* Tipo bool             */
#include <assert.h>  /* assert() */

// Elegir un valor para tam para jugar una partida de tateti de tamxtam
#define tam 5
#define CELL_MAX (tam * tam - 1)

void print_sep(int length) {  
  printf("\t ");
  for (int i = 0; i < length; i++)
    printf("................");
  printf("\n");
}

void print_board(char board[tam][tam]) {
  int cell = 0;

  print_sep(tam);
  for (int row = 0; row < tam; ++row) {
    for (int column = 0; column < tam; ++column) {
      printf("\t | %d: %c ", cell, board[row][column]);
      ++cell;
    }
    printf("\t | \n");
    print_sep(tam);
  }
}

char get_winner(char board[tam][tam]) {

  char winner = '-' ;

  // Variables para comparar las filas y columnas
  char comparo_fila , comparo_columna;

  // Variables para realizar el control de las diagonales
  int diag = 0 , diaginv = tam-1 , totaldiag = 0;
  char comparodiag = board[0][0] , comparodiaginv = board[0][tam-1];

  // Verifico el ganador horizontal
  for (size_t fila = 0; fila < tam; fila++) {

    comparo_fila = board[fila][0];
    int contador = 0;

    for (size_t columna = 0; columna < tam; columna++) {

      if (comparo_fila == board[fila][columna]) {

        comparo_fila = board[fila][columna];
        contador ++;

      } else {

        break;

      }
    }

    if ((contador == tam) && (comparo_fila != '-')) {

      winner = comparo_fila;
      return winner;

    }
  }

  //Verifico el ganador Vertical
  for (size_t columna = 0; columna < tam; columna++) {

    comparo_columna = board[0][columna];
    int contador = 0;

    for (size_t fila = 0; fila < tam; fila++) {

      if (comparo_columna == board[fila][columna]) {

        comparo_columna = board[fila][columna];
        contador ++;

      } else {

        break;

      }
    }

    if ((contador == tam) && (comparo_columna != '-')) {

      winner = comparo_columna;
      return winner;

    }
  }

  //Verifico la Diagonal de arriba abajo izq der
  for (size_t i = 0; i < tam; i++) {
    
    if (comparodiag == board[i][diag]) {

      comparodiag = board[i][diag];
      diag ++;
      totaldiag ++;

    } else {

      totaldiag = 0;

    }

    if (totaldiag == tam) {

      winner = comparodiag;
      return winner;

    }
  }
  
  //Verifico la Diagonal de arriba abajo der izq
  for (size_t i = 0; i < tam; i++) {
    
    if (comparodiaginv == board[i][diaginv]) {

      comparodiaginv = board[i][diaginv];
      diaginv --;
      totaldiag ++;

    } else {

      totaldiag = 0;

    }

    if (totaldiag == tam) {

      winner = comparodiaginv;
      return winner;

    }
  }

  return winner;
}

// Funcion para saber si hay celdas vacias
bool has_free_cell(char board[tam][tam]) {

  bool free_cell = false;

  for (size_t i = 0; i < tam; i++) {

    for (size_t j = 0; j < tam; j++) {

      if (board[i][j] == '-') {

        free_cell = true;

      }
    }
  }

  return free_cell;
}

// Funcion para crear la matriz de tamxtam con las celdas marcadas vacias con '-'
void inicio_matriz(char board[tam][tam]) {

  for (size_t i = 0; i < tam; i++) {

    for (size_t j = 0; j < tam; j++) {
      
      board[i][j] = '-';

    }
        
  }

}

int main(void) {

  printf("TicTacToe [InCoMpLeTo :'(]\n");

  char board[tam][tam];

  inicio_matriz(board);

  /* = {
      {'-', '-', '-'},
      {'-', '-', '-'},
      {'-', '-', '-'}};
  */

  char turn = 'X';
  char winner = '-';
  int cell = 0;
  while (winner == '-' && has_free_cell(board)) {
    print_board(board);
    printf("\nTurno %c - Elija posición (número del 0 al %d): ", turn,
           CELL_MAX);
    int scanf_result = scanf("%d", &cell);
    if (scanf_result <= 0) {
      printf("Error al leer un número desde teclado\n");
      exit(EXIT_FAILURE);
    }
    if (cell >= 0 && cell <= CELL_MAX) {
      int row = cell / tam;
      int colum = cell % tam;
      if (board[row][colum] == '-') {
        board[row][colum] = turn;
        turn = turn == 'X' ? 'O' : 'X';
        winner = get_winner(board);

      } else {
        printf("\nCelda ocupada!\n");
      }
    } else {
      printf("\nCelda inválida!\n");
    }
  }
  print_board(board);
  if (winner == '-') {
    printf("Empate!\n");
  } else {
    printf("Ganó %c\n", winner);
  }
  return 0;
}