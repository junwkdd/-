#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <locale.h>
#include <wchar.h>

int main()
{
   setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

   wchar_t temp[10] = L"반가워";

   printf("%lc\n", temp[2]);
}