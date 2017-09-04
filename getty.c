#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

// struct user {
//   char[50] name, pass;
//   user * next;
// }

int main(int argc, char *argv[])
{
  bool incorrecto = true;

  while(1) {

    FILE * user_passwrd_file = fopen("passwd.txt", "r");
    int user_password_is_equal = true, status;
    char user_passwd_line[101], user[50], password[50], user_password[101];

    printf("Ingrese usuario: ");
    scanf("%s", user);
    printf("Ingrese contraseña: ");
    scanf("%s", password);

    strcpy(user_password, user);
    strcat(user_password, ":");
    strcat(user_password, password);
    strcat(user_password, "\n");

    while(fgets(user_passwd_line, sizeof(user_passwd_line), user_passwrd_file) && user_password_is_equal != 0)
    {
      user_password_is_equal = strcmp(user_password, user_passwd_line);
    }

    fclose(user_passwrd_file);

    if(user_password_is_equal == false)
    {
      // incorrecto = false;
      printf("Usuario y/o contraseña correcto.\n");
      execlp("./sh", "sh.c", NULL);
      wait(&status);
      kill(getpid(), SIGKILL);
    }
    else
    {
      printf("Usuario y/o contraseña incorrecto.\n");
    }

  }
  return 0;
}
