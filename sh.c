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

  define("PATH", "/bin");

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

        char *token = strtok(buffer, "=");
        char *variable;
        char *valor;

        if(token[0] == '$'){
          token ++;
          variable = search(token)->value;
        } else {
          variable = token;
          if(variable[0] == ' ')
            variable ++;
        }

        token = strtok(NULL, " ");

        if(token[0] == '$'){
          token ++;
          valor = search(token)->value;
        } else {
          valor = token;
        }

        define(variable, valor);
      }
      else if(strcmp(command, "echo") == 0)
      {
        printf("in echo\n\r");

        char *token = strtok(buffer, " ");
        char *variable;

        if(token[0] == '$'){
          printf("hay algo\n\r");
          token ++;
          variable = search(token)->value;
          printf("%s\n", variable);
        }

      }
      else
      {
        printf("in else\n\r");

        char *com_ptr = command;
        char *real_ptr = command;

        if(com_ptr[0] == '$') {
          com_ptr++;
          real_ptr = search(com_ptr)->value;
        }

        char *path = search("PATH")->value;

        int newsize = strlen(real_ptr) + strlen(path) + 2;

        char *pathCommand = (char *)malloc(newsize);

        if(real_ptr[0] != '.' && real_ptr[0] != '/') {
          strcpy(pathCommand, path);
          strcat(pathCommand, "/");
          strcat(pathCommand, real_ptr);
        } else {
          strcpy(pathCommand, real_ptr);
        }

        token = strtok(buffer, " ");
        int i = 1;

        args[0] = pathCommand;

        while(token != NULL) {

          if(token[0] == '$'){
            printf("hay algo\n\r");
            token ++;
            args[i] = search(token)->value;
          } else {
            args[i] = token;
          }

          token = strtok(NULL, " ");
          i++;
        }
        args[i+1] = NULL;

        int status, pid = fork();

        if (pid == 0) {
          execvp(pathCommand, args);
        }

        wait(&status);
      }
    }

  return 0;
}
