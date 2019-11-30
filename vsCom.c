#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#define MAXLENGTH 10
#define STRUCTSIZE 44
#define TRUE 1
#define FALSE 0

typedef struct inputWord    // 단어를 저장하기 위한 구조체
{
    wchar_t inputWord[MAXLENGTH];
    int isUsed;
} inputWordStruct;

FILE *openFile(FILE *fp, char *fileName);
int checkWordSame(wchar_t *w1, wchar_t *w2);
wchar_t readWord(FILE *fp, wchar_t *comparedWord, inputWordStruct fileReadWord);

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *fp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;
    inputWordStruct fileReadWord;

    fp = openFile(fp, "words.dic");

    // fseek(fp, 0, SEEK_END);
    // printf("%ld\n", ftell(fp));

    readWord(fp, L" ", fileReadWord);

    printf("%ls\n", fileReadWord.inputWord);
    printf("%d\n", fileReadWord.isUsed);

    //scanf("%ls", fileWriteWord.inputWord);

    // fileWriteWord.isUsed = FALSE;
    // fwrite(&fileWriteWord, sizeof(fileWriteWord), 1, fp);

    fclose(fp);
}

wchar_t readWord(FILE *fp, wchar_t *comparedWord, inputWordStruct fileReadWord)      // 단어장으로 부터 단어 읽기
{
    setlocale(LC_ALL, "");

    srand(time(NULL));
    
    fseek(fp, 0, SEEK_END);
    long int wordCount = ftell(fp) / STRUCTSIZE;      // 단어의 총 개수
    __ssize_t ret;

    int random = ((rand() % wordCount) + 1);      // 1 ~ 단어의 개수 만큼 범위의 난수 생성

    fseek(fp, STRUCTSIZE * (random-1), SEEK_SET);   // 랜덤 수 - 1 이 해당 번호의 수

    do {       // 이미 사용된 단어가 아닐 때 까지 반복
        ret = fread(&fileReadWord, sizeof(fileReadWord), 1, fp);

        if(ret != 1) {   // fread() 오류 발생 시
            printf("fread() error!\n");
            exit(-1);
        }

        if(ftell(fp) == (wordCount * STRUCTSIZE)) {        // 단어장의 끝에 도달
            fseek(fp, 0, SEEK_SET);
        }

        if(checkWordSame(comparedWord, fileReadWord.inputWord)) {
            if(fileReadWord.isUsed != FALSE) {
                break;
            }
        }

    } while(fileReadWord.isUsed != FALSE);
}

int checkWordSame(wchar_t w1[], wchar_t w2[])
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
    fp = fopen(fileName, "a+");

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