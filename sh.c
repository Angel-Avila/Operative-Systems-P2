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
  char command[25];

  while(shutdown != true)
  {

    char *buffer;
    size_t bufsize = 32;

    buffer = (char *) malloc(bufsize * sizeof(char));

    printf("sh >");
    scanf("%[^ \t\n]", command);

    //printf("Command> %s\n", command);

    getline(&buffer, &bufsize, stdin);

    int len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
      buffer[len-1] = 0;

    printf("Command>%s\n", command);
    printf("Buffer>%s\n", buffer);

    char *args[20] = {0};

    char *token;

    if(strcmp(command, "exit") == 0)
    {
      printf("in exit\n\r");
      exit(0);
    }
    else if(strcmp(command, "shutdown") == 0)
    {
      printf("in shutdown\n\r");
      exit(1);
    }
    else if(strcmp(command, "export") == 0)
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
      else
      {
        printf("in else\n\r");

        token = strtok(buffer, " ");
        int i = 1;

        args[0] = command;

        while(token != NULL) {
          args[i] = token;

          if(args[i][0] == '$'){
            printf("hay algo\n\r");
            args[i] ++;
          }

          printf("Token %d>%s\n", i, token);
          token = strtok(NULL, " ");
          i++;
        }
        args[i+1] = NULL;

        int status, pid = fork();

        if (pid == 0) {
          execvp(command, args);
        }

        wait(&status);
      }

    }



  return 0;
}
