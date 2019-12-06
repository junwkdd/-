#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <locale.h>
#include <wchar.h>

int main()
{
   char temp[10] = "개나리";

   printf("%s\n", temp);

   strcpy(temp, " ");

   printf("%s\n", temp);
}