#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//  Syscall de prueba para ver que ande
uint64
sys_numero(void)
{
  return 11;
}

// Definimos la estructura del semaforo
typedef struct semaphore_struct {

  // Valor
  int value;

  // Lock
  struct spinlock lock;

} semaphore;

// Arreglo de semaforos
semaphore sems_array[MAXSEMS];

/* 
  Syscall que inicializa todos los semaforos
  Se agrego a def.h y main.c para que se ejecute al bootear
*/
void init_sems(void)
{
  for (unsigned int i = 0; i < MAXSEMS; i++) {
    
    // Cargamos numero arbitrario
    sems_array[i].value = DEFSEM_VAL;
  }
}

/* 
  int sem_open(int sem, int value)
  Abre y/o inicializa el semáforo “sem” con un valor arbitrario “value”.
*/
uint64
sys_sem_open(void)
{
  // Creamos las variables
  int sem , value;

  // Leemos los argumentos
  argint(0 , &sem);
  argint(1 , &value);

  // Chequeamos que los argumentos sean validos y que el semaforo este libre
  if (sem < 0 || sem >= MAXSEMS || value < 0 || sems_array[sem].value != DEFSEM_VAL) {

    // Si no son validos devolvemos 0 = error
    return 0;
  }

  // Inicializamos el lock
  initlock(&sems_array[sem].lock , "sem_lock");

  // Habilitamos el lock, no se pueden realizar interrupciones
  // Critical Zone
  acquire(&sems_array[sem].lock);

  // Cargamos el value al semaforo
  sems_array[sem].value = value;

  // Deshabilitamos el lock
  release(&sems_array[sem].lock);
  // End Critical Zone

  return 1;
}

/* 
  int sem_close(int sem)
  Libera el semáforo “sem”.
*/
uint64
sys_sem_close(void)
{
  // Creamos la variable
  int sem;

  // Le asigno a sem el numero de semaforo a cerrar
  argint(0, &sem);

  // Chequeamos que el semaforo pertenzca al arreglo
  if (sem < 0 || sem >= MAXSEMS) {

    // Si no pertenece devolvemos 0 = error
    return 0;
  }

  // Antes de cerrar, despertamos el proceso por las dudas
  wakeup(&sems_array[sem]);

  // Critical Zone
  // Inhabilito las interrupciones
  acquire(&sems_array[sem].lock);
  
  // Pongo su valor default
  sems_array[sem].value = DEFSEM_VAL;

  // Habilito las interrupciones
  release(&sems_array[sem].lock);
  // End Critical Zone

  return 1;
}


/* 
  int sem_up(int sem)
  Incrementa el semáforo ”sem” desbloqueando los procesos cuando su valor es 0.
*/
uint64
sys_sem_up(void)
{
  // Creamos la variable
  int sem;
  
  // Le asigno a sem el numero de semaforo al que le quiero incrementar el valor
  argint(0, &sem);

  // Chequeamos que el semaforo pertenzca al arreglo
  if (sem < 0 || sem >= MAXSEMS) {

    // Si no pertenece devolvemos 0 = error
    return 0;
  }

  // Critical Zone
  // Inhabilito las interrupciones
  acquire(&sems_array[sem].lock);

  // Reviso si estaba durmiendo
  if(sems_array[sem].value == 0){

    // Lo "despertamos" 
    wakeup(&sems_array[sem]);
  }
  
  // Incremento el valor del semaforo
  sems_array[sem].value += 1;

  // Habilito las interrupciones
  release(&sems_array[sem].lock);
  // End Critical Zone
  
  return 1;
}


/* 
  int sem_down(int sem)
  Decrementa el semáforo ”sem” bloqueando los procesos cuando su valor es 0. El valor del semaforo nunca puede ser menor a 0
*/
uint64
sys_sem_down(void)
{
  // Creamos la variable
  int sem;

  // Le asigno a sem el numero de semaforo al que le quiero decrementar el valor
  argint(0,&sem);

  // Chequeamos que el semaforo pertenzca al arreglo
  if (sem < 0 || sem >= MAXSEMS) {

    // Si no pertenece devolvemos 0 = error
    return 0;
  }

  // Critical Zone
  // Inhabilito las interrupciones
  acquire(&sems_array[sem].lock);
  
  // Veo si tengo que poner a dormir al proceso
  while (sems_array[sem].value == 0) {
    /* 
      sleep toma 2 parametros. un proceso y un lock.
      Manda a dormir al proceso y libero el lock.
      Al despertar automaticamente vuelvo activar el lock
    */
    sleep(&sems_array[sem] , &sems_array[sem].lock); 
  }
  
  // Decremento el valor del semaforo 
  sems_array[sem].value -= 1;

  // Habilito las interrupciones
  release(&sems_array[sem].lock);
  // End Critical Zone
  
  return 1;
}