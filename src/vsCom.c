#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

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
void writeWord(FILE *fp, inputWordStruct fileWriteWord);
void timer();

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *fp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;
    inputWordStruct fileReadWord;

    fp = openFile(fp, "words.dic");

    while(1) {
        if(!readWord(fp, L" ", fileReadWord)) {
            if(existWord == 1) {
                writeWord(fp, fileWriteWord);
            }
        }
        printf("%ls\n", fileReadWord.inputWord);

        scanf("%ls", fileWriteWord.inputWord);
        if(!checkWordSame(fileReadWord.inputWord, fileWriteWord.inputWord)) {
            printf("끝말이 일치하지 않음");
            return 0;
        }

        //timer();
    }

    fclose(fp);
}

void timer()
{
    for(int i=5; i>=0; i--) {
        printf("%d\n", i);
        sleep(1);
    }
}

int existWord(FILE *fp, wchar_t *comparedWord)
{
    fseek(fp, 0, SEEK_SET);
    long int wordCount = ftell(fp) / STRUCTSIZE;
    inputWordStruct temp;

    for(int i=0; i<wordCount; i++) {
        fread(&temp, sizeof(temp), 1, fp);
        if(strcmp(temp.inputWord, comparedWord) == 0) {
            return 0;       // 이미 단어가 존재
        } else {
            return 1;       // 단어장에 없던 단어
        }
    }
}

void writeWord(FILE *fp, inputWordStruct fileWriteWord)
{
    ssize_t ret = fwrite(&fileWriteWord, sizeof(fileWriteWord), 1, fp);
    
    if(ret != 1) {
        printf("fwrite() error!\n");
        exit(-1);
    }
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
    long int firstPos = ftell(fp);

    do {       // 이미 사용된 단어가 아닐 때 까지 반복
        ret = fread(&fileReadWord, sizeof(fileReadWord), 1, fp);

        if(ret != 1) {   // fread() 오류 발생 시
            printf("fread() error!\n");
            exit(-1);
        }

        if(ftell(fp) == (wordCount * STRUCTSIZE)) {        // 단어장의 끝에 도달
            fseek(fp, 0, SEEK_SET);
        }

        if(ftell(fp) == firstPos) {     // 단어장을 한바퀴 다 돌았을 때
            pritnf("Com 패배\n");
            return 0;
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