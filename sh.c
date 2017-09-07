/*
Autores:
Angel Ávila, Giovanni Mazzolo
Fecha:
6/9/2017

sh.c source file
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

// Se declara una estructura que contiene las variables a guardar
struct dict {
  struct dict * next;
  char * command;
  char * value;
};

/** Funciones obtenidas de fuentes externas **********************************/
/** Fuente: www.stackoverflow.com **/
/** Función: Tabla hash con búsqueda */

static struct dict *command_list[1000];
char *copystr(char *);

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
struct dict *define(char *command, char *value) {
  struct dict *cl;
  unsigned val;
  if ((cl = search(command)) == NULL) {
    cl = (struct dict *) malloc(sizeof(*cl));
    if (cl == NULL || (cl->command = copystr(command)) == NULL)
      return NULL;
    val = hash(command);
    cl->next = command_list[val];
    command_list[val] = cl;
  }
  else{
    free((void *) cl->value);
    if ((cl->value = copystr(value)) == NULL)
      return NULL;}
  return cl;
}
char *copystr(char *s) {
  char *p;
  p = (char *) malloc(strlen(s)+1);
  if (p != NULL) strcpy(p, s);
  return p;
}
/** Fuentes externas **/
/*****************************************************************************/


int main(int argc, char *argv[])
{
  // String que contiene los comando que se recibirán
  char command[50];

  // Se inicializa la variable PATH en el directorio /bin
  define("PATH", "/bin");

  // Loop infinito
  for(;;)
  {

    // Se definen las variables y sus tamaños para recibir distintas entradas
    char *buffer;
    size_t bufsize = 32;

    buffer = (char *) malloc(bufsize * sizeof(char));

    // A la espera de un comando de entrada, se guardan los comandos
    printf("sh >");
    scanf("%[^ \t\n]", command);
    getline(&buffer, &bufsize, stdin);

    // Se guardan el resto de las instrucciones en un buffer
    int len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
    {
      buffer[len-1] = 0;
    }

    // Se inicializan variables que contienen argumentos de entrada
    char *args[50] = {0};
    char *token;

    // Se ejecuta la función de acuerdo al comando de entrada
    if(strcmp(command, "exit") == 0)
    {
      // El proceso se cierra y el proceso padre continúa con su ejecucción
      exit(0);
    }
    else if(strcmp(command, "shutdown") == 0)
    {
      // El proceso se cierra y manda señal de shutdown al padre
      exit(1);
    }
    else if(strcmp(command, "export") == 0)
    {

      // Declaraciones de variables
      char *variable;
      char *valor;
      // Se separan los distintos argumentos
      char *token = strtok(buffer, "=");

      // En caso de ser una entrada válida
      if(token != NULL)
      {
        // Si es una variable guardada, se busca en la tabla hash
        if(token[0] == '$')
        {
          token ++;
          variable = search(token)->value;
        }
        else // Si no hay que buscar un valor guardado se guarda el argunmento en una variable
        {
          variable = token;
          if(variable[0] == ' ')
          {
            variable ++;
          }
        }

        // Se lee el próximo argumento de entrada
        token = strtok(NULL, " ");

        // Cerciora si es un valor válido
        if(token != NULL)
        {

          // Si es un valor que debe buscarse, se busca en la tabla hash
          if(token[0] == '$')
          {
            token ++;
            valor = search(token)->value;
          }
          else // Si no es un valor guardado, se guarda el segundo argumento en una variable
          {
            valor = token;
          }

          // Si la variable y el valor son valores válidos, se guarda
          if(variable != NULL && valor != NULL)
          {
            define(variable, valor);
          }
        }
        else // Si son entradas no válidas, se imprime salto de línea
        {
          printf("\n\r");
        }
      }
      else // Si son entradas no válidas, se imprime salto de línea
      {
        printf("\n\r");
      }
    }
    else if(strcmp(command, "echo") == 0)
    {

      // Declaración de variables
      char *token = strtok(buffer, " ");
      char *variable;

      // Si es una entrada válida se ejecuta
      if(token != NULL)
      {

        // Si se desea buscar una variable conocida se busca en la tabla hash
        if(token[0] == '$')
        {
          token ++;
          // Si la variable existe, se guarda el valor
          if(search(token) != NULL)
          {
            variable = search(token)->value;
          }

          // Si es un valor distinto de nulo, se imprime, sinó se imprime salto de línea
          if(variable != NULL)
          {
            printf("%s\n", variable);
          }
          else
          {
            printf("\n\r");
          }
        }
        // Si no se tiene que buscar un valor, se imnprime el argumento de entrada
        else if(token != NULL)
        {
          printf("%s\n", token);
        }
        else // Si es una entrada no permitida, se imprime saldo de línea
        {
          printf("\n\r");
        }
      }
      else // Si es una entrada no permitida, se imprime saldo de línea
      {
        printf("\n\r");
      }

    }

    // Si no se desea hacer uso de comandos predefinidos, se accede la ajacucción de comandos de terminal
    else
    {
      // Declaración e inicialización de variables
      char *com_ptr = command;
      char *real_ptr = command;

      // Si se intenta ejecutar un comando previamente guardado, se busca
      if(com_ptr[0] == '$') {
        com_ptr++;
        real_ptr = search(com_ptr)->value;
      }

      // Se busca el valor de la variable PATH y se carga en una variable
      char *path = search("PATH")->value;

      // Se inicializan nuevas variables
      int newsize = strlen(real_ptr) + strlen(path) + 2;
      char *pathCommand = (char *)malloc(newsize);

      // Se verifica que se se quiere ejecutar un nuevo proceso
      if(real_ptr[0] != '.' && real_ptr[0] != '/') {
        // Se concatenan los comandos para ejeccción de un nuevo proceso
        strcpy(pathCommand, path);
        strcat(pathCommand, "/");
        strcat(pathCommand, real_ptr);
      }
      // Si no se desea ejecutar un proceso nuevo, se guarda el comando para ejecutarlo
      else
      {
        strcpy(pathCommand, real_ptr);
      }

      // Se leen los primeros argumentos de entrada
      token = strtok(buffer, " ");

      // inicialización de variables
      int background = 0;
      int i = 1;

      // Se guarda en el primer argumento el comando
      args[0] = pathCommand;

      while(token != NULL)
      {
        // Si se quiere ejecutar una variable guardada, se busca y se guarda
        if(token[0] == '$')
        {
          token ++;
          args[i] = search(token)->value;
        }
        // Si es un comando estándar, se guarda el argumento
        else
        {
          args[i] = token;
        }
        //  Si se desea ejecutar en modo no bloquante, se levanta una bandera
        if(token[0] == '&' && i == 1)
        {
          background = 1;
        }
        // Se continúando leyendo los comandos de entrada y guardándose
        token = strtok(NULL, " ");
        i++;
      }

      // Se hace nulo la última posición del arreglo de apuntadores que contiene los argumentos
      args[i+1] = NULL;

      // Se crea un proceso hijo para ejecutar el comando recivido de entrada
      int status, pid = fork();
      // Si es un proceso hijo, se reemplaza la imagen del hijo por la del proceso que se quiere ejecutar
      if (pid == 0)
      {
        // Si es un proceso no válido, se termina la ejecución del proceso
        if(execvp(pathCommand, args) == -1)
        {
          exit(EXIT_FAILURE);
        }
      }
      //  Si se está ejecutando un proceso en modo bloqueante, espera a ese proceso
      else
      {
        if(!background)
        wait(&status);
      }
    }
  }

  return 0;
}
