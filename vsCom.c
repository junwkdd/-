#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#define MAXLENGTH 20

FILE *openFile(FILE *fp, char *fileName);
int checkWordSame(wchar_t *w1, wchar_t *w2);

struct inputWordStruct
{
    wchar_t *fileInputWord;
};


int vsCom()
{
    setlocale(LC_ALL, "");

    FILE *fp;
    struct inputWordStruct w;
    wchar_t *inputWord = malloc(sizeof(char) * MAXLENGTH);

    fp = openFile(fp, "words");
    
    scanf("%ls", inputWord);

    //checkWordSame();



    free(inputWord);
    fclose(fp);
}

wchar_t *readWord(FILE *fp, wchar_t comparedWord)
{
    
}

int checkWordSame(wchar_t *w1, wchar_t *w2)
{
    setlocale(LC_ALL, "");
    wchar_t last = w1[wcslen(w1) - 1];
    wchar_t first = w2[0];

    if(last == first) {
        return 1;
    } else {
        return 0;
    }
}

FILE *openFile(FILE *fp, char *fileName)
{
    fp = fopen("words.txt", "a+");

    if(fp == NULL) {
        printf("file open error!\n");
        exit(1);
    } 

    return fp;
}

int main()
{
    vsCom();
}