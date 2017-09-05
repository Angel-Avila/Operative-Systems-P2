#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

struct dict {
    struct dict * next;
    char * command;
    char * value;
};

static struct dict *command_list[1000];

unsigned hash(char *c) {
    unsigned val;

    for (val = 0; *c != '\0'; c++)
      val = *c + 31 * val;

    return val % 1000;
}

struct dict *search(char *c) {
    struct dict *cl;

    for (cl = command_list[hash(c)]; cl != NULL; cl = cl->next)
        if (strcmp(c, cl->command) == 0)
          return cl;

    return NULL;
}

char *copystr(char *);

struct dict *define(char *command, char *value)
{
    struct dict *cl;
    unsigned val;

    if ((cl = search(command)) == NULL) {
        cl = (struct dict *) malloc(sizeof(*cl));
        if (cl == NULL || (cl->command = copystr(command)) == NULL)
          return NULL;
        val = hash(command);
        cl->next = command_list[val];
        command_list[val] = cl;
    } else
        free((void *) cl->value);

    if ((cl->value = copystr(value)) == NULL)
       return NULL;

    return cl;
}

char *copystr(char *s) {
    char *p;
    p = (char *) malloc(strlen(s)+1);

    if (p != NULL)
       strcpy(p, s);

    return p;
}

int main(int argc, char *argv[])
{

  bool shutdown = false;
  char command[25], sentence[100];

  while(shutdown != true)
  {

    printf("sh > ");
    scanf("%s%*c", command);

    //nt index = strstr(sentence, " ") - sentence;
    //strncpy(command, sentence, index);

    //printf("\nSentence> %s\nCommand> %s\n", sentence, command);

    if(strcmp(command, "export") == 0)
    {
      printf("in export\n\r");
    }
    else if(strcmp(command, "echo") == 0)
    {
      printf("in echo\n\r");

      define("Hola", "perrita");
      char *variable = "Hola";
      char *valor = search(variable)->value;
      printf("%s\n", valor);

    }
    else if(strcmp(command, "exit") == 0)
    {
      printf("in exit\n\r");
      exit(0);
    }
    else if(strcmp(command, "shutdown") == 0)
    {
      printf("in shutdown\n\r");
      exit(1);
    }
    else
    {
      printf("in else\n\r");

      char *arguments[] = {"sh", "-c", command, [INSERTE], [ARGUMENTOS], [AQUI], NULL};

      int status, pid = fork();

      if (pid == 0) {
        execvp("/bin/sh", arguments);
      }

      wait(&status);
    }

  }
  return 0;
}
