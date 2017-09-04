#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{

  bool shutdown = false;
  char command[8], temp_string[50];

  while(shutdown != true)
  {

    printf("sh > ");
    scanf("%s", command);

    if(strcmp(command, "export") == 0)
    {
      printf("in export\n\r");
      scanf("%s", temp_string);
    }
    else if(strcmp(command, "echo") == 0)
    {
      printf("in echo\n\r");
      scanf("%s", temp_string);
    }
    else if(strcmp(command, "exit") == 0)
    {
      printf("in exit\n\r");
      kill(0, SIGKILL);
    }
    else if(strcmp(command, "shutdown") == 0)
    {
      // shutdown = true;
      printf("in shutdown\n\r");
      kill(getpid(), SIGKILL);
    }
    else
    {
      printf("in else\n\r");
    }

  }
  return 0;
}
