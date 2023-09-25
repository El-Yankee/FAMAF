#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"


static int select_free_sem(void){

  int n = 0;

  int occupy = sem_open(n,1);

  while (occupy == 0 && n < MAXSEMS) {

    n ++;

    occupy = sem_open(n,1);

  }

  sem_close(n);
  
  return n;
}

int main(int argc, char *argv[])
{
  // Revisamos que la cantidad de argumentos sea valido
  if (argc <= 1) {

    printf("ERROR: Not enough arguments\n");
    exit(1);
  }
  
  // Tomamos la cantidad de rounds de ping pong
  int rounds = atoi(argv[1]);

  // Vemos que la cantidad de rounds sea valido
  if (rounds <= 0) {

    printf("ERROR: Round number must be positive\n");
    exit(1);
  }
  
  // Creamos variable de checkeo de errores
  int error = 1;

  /* 
    Inicializo los semaforos, uno para el hijo y otro para el padre
    El del hijo = "ping" empieza activo y el del padre = "pong" dormido    
  */

  // Buscamos semaforo libre para el hijo
  int son_sem = select_free_sem();

  // Creo el semaforo del hijo 
  error = sem_open(son_sem, 1);  
  // Reviso que se haya creado bien el semaforo
  if (error == 0 ) {

    printf("ERROR: Failed open son semaphore\n");
    exit(1);
  }

  // Buscamos semaforo libre para el padre
  int father_sem = select_free_sem();

  // Creo el semaforo del padre 
  error = sem_open(father_sem, 0);
  // Reviso que se haya creado bien el semaforo
  if (error == 0 ) {

    printf("ERROR: Failed open father semaphore\n");
    exit(1);
  }
  
  // Realizamos el fork
  int pid = fork();

  if (pid < 0) {

    // El fork fallo   
    printf("ERROR: Fork Failed\n");
  
  } else if (pid == 0) {

    /* 
      Estoy en el hijo    
      Debo imprimir ping por la cantidad de rounds
      Pero como debo intercalar con pong, debo modificar los semaforos
    */
    for (unsigned int i = 0; i < rounds; i++) {

      // Modifico el semaforo de ping
      error = sem_down(son_sem);
      // Reviso que se haya modificado bien
      if (error == 0 ) {

        printf("ERROR: Failed son sem_down\n");
        exit(1);
      }

      // Escribo ping
      printf("ping\n");

      // Modifico el semaforo de pong
      error = sem_up(father_sem);
      // Reviso que se haya modificado bien
      if (error == 0 ) {

        printf("ERROR: Failed father sem_up\n");
        exit(1);
      }
    }

  } else {

    /* 
      Estoy en el padre    
      Debo imprimir pong por la cantidad de rounds
      Pero como debo intercalar con ping, debo modificar los semaforos
    */
    for (unsigned int i = 0; i < rounds; i++) {

      // Modifico el semaforo de pong
      error = sem_down(father_sem);
      // Reviso que se haya modificado bien
      if (error == 0 ) {

        printf("ERROR: Failed father sem_down\n");
        exit(1);
      }

      // Escribo pong
      printf("    pong\n");

      // Modifico el semaforo de ping
      error = sem_up(son_sem);
      // Reviso que se haya modificado bien
      if (error == 0 ) {

        printf("ERROR: Failed son sem_up\n");
        exit(1);
      }
    }

    // Una vez terminados los rounds, cierro los semaforos
    error = sem_close(son_sem);
    if (error == 0 ) {

      printf("ERROR: Failed son sem_close\n");
      exit(1);
    }

    error = sem_close(father_sem);
    if (error == 0 ) {

      printf("ERROR: Failed father sem_close\n");
      exit(1);
    }
  }

  exit(0);
}