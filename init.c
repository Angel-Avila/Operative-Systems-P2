/*
  Autores:
    Angel Ávila, Giovanni Mazzolo
  Fecha:
    6/9/2017

  init.c source file

  Descripción:
    El proceso init se encarga de crear una cantidad predefinida de
    procesos getty. Si uno de esoos procesos se cierra, se vuelve a cear.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define NUM_PROCESS 6 // Numero de procesos

int main(int argc, char *argv[])
{
  // Declaración de variables
  int status, procesos_exec = 0;
  // Se imprime el PID del proceso INIT
  printf("Proceso padre: %d\n", getpid());

  for(;;) // Loop infinito
  {
    // Si la cantidad de procesos es menor a la predefinida, crea un nuevo proceso
    if(procesos_exec < NUM_PROCESS)
    {
      // La cantidad procesos creados aumenta
      procesos_exec ++;

      // Se crea un proceso, entra a la condicional si el proceso es hijo
      if(fork() == 0)
      {
        // Se imprime el PID del nuevo proceso creado
        printf("Creando proceso %d \n\r", getpid());
        // La imagen del proceso creado se reemplaza por un nuevo proceso xterm que ejecuta un nuevo proceso GETTY
        execlp("xterm", "xterm", "-e", "./getty", NULL);
      }
    }
    else
    {
      // Si ya se crearon la cantidad de procesos predefinidos, espera a que uno termine
      wait(&status);
      // Si un proceso termina, se decrementa la cantidad de procesos en ejecucción
      procesos_exec --;
    }

  }
  return 0;
}
