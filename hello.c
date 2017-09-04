#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  char t;
  printf("Hola mundo desde %d\n\r", getpid());
  scanf("%s\n", t);
  return 0;
}
