/*
  Autores:
    Angel Ávila, Giovanni Mazzolo
  Fecha:
    6/9/2017

  getty.c source file
*/

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

  int status;

  for(;;) // Loop infinito
  {
    // Se abre el archivo que contiene los usuarios y se guarda en una variable apuntador a FILE
    FILE * user_passwrd_file = fopen("passwd.txt", "r");
    // Variable que define si el usuario y contraseña se ingresaron correctamente
    int user_password_is_equal = false;
    // Variables que contienen arreglos de caracteres
    char user_passwd_line[101], user[50], password[50], user_password[101];

    // Se pide que se ingrese el usuario y contraseña
    printf("Ingrese usuario: ");
    scanf("%s", user);
    printf("Ingrese contraseña: ");
    scanf("%s", password);

    // Se concatenan en un arreglo de caracteres el usuario y contraseña para comparar los guardados en el archivo .txt
    strcpy(user_password, user);
    strcat(user_password, ":");
    strcat(user_password, password);
    strcat(user_password, "\n");

    // Se inicializa la variable en falso, si la contraseña y usuario es incorrecto, se queda en falso
    user_password_is_equal = false;

    // Se reccorre el archivo .txt leyendo línea por línea y se guarda en una string
    // Si una línea es igual al usuario y contraseña del archivo, entonces se sale del ciclo de lectura de líneas
    while(fgets(user_passwd_line, sizeof(user_passwd_line), user_passwrd_file))
    {
      // Si el usuario y contraseña concuerdan, la función strcmp() regresa un 0
      if(strcmp(user_password, user_passwd_line) == 0)
      {
        user_password_is_equal = true;
      }
    }

    // Se cierra el archivo con las contraseñas
    fclose(user_passwrd_file);

    // Si la contraseña es ingresada correctamente
    if(user_password_is_equal == true)
    {
      // Se imprime un mensaje
      printf("Usuario y/o contraseña correcto.\n");

      // Se crea un proceso hijo, y se reemplaza la imagen por un nuevo proceso sh (shell)
      if(fork() == 0)
      {
        execlp("./sh", "sh.c", NULL);
      }
      // Espera a que el proceso hijo termine
      wait(&status);
      // Si el proceso hijo regresa un status mayor a 0, equivale al comando shutdown
      if(status > 0)
      {
        // Si el comando es de shutdown, entonces se mata el grupo de procesos que corresponde al padre, así matando a todos los otros procesos
        kill(getpgid(getppid())*(-1), SIGKILL);
      }
    }
    else // Si la contraseña es ingresada incorrectamente, se imprime un mensaje y se tiene que volver a ingresar usuario y contraseña
    {
      printf("Usuario y/o contraseña incorrecto.\n");
    }

  }
  return 0;
}
