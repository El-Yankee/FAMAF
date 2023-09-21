#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
    
// CUANDO CREEN UN NUEVO PROGRAMA AGREGUENLO EN EL MAKEFILE EN $U
    
int main(int argc, char *argv[])
{
  printf("Hello World\n");
  printf("The syscall numero returns %d\n", numero());
  exit(0);
}