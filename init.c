#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define NUM_PROCESS 2

int main(int argc, char *argv[])
{

  int proceso, status, procesos_exec = 0, parentpid = getpid();
  bool shutdown = false;
  printf("Proceso padre: %d\n", parentpid);

  while(shutdown == false)

  {

    if(procesos_exec < NUM_PROCESS)
    {
      procesos_exec ++;
      proceso = fork();
      if(proceso == 0)
      {
        printf("Creando proceso %d \n\r", getpid());
        execlp("xterm", "xterm", "-e", "./getty", NULL);
      }
    }
    else
    {
      wait(&status);
      procesos_exec --;
    }

  }
  return 0;
}
