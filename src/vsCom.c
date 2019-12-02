#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

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
wchar_t readWord(FILE *fp, wchar_t *comparedWord, inputWordStruct *fileReadWord);
void writeWord(FILE *fp, inputWordStruct fileWriteWord);
int existWord(FILE *fp, wchar_t *comparedWord);
void inputWord(inputWordStruct *fileWriteWord);
void resetUsed(FILE *fp);
void timer();

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *fp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;
    inputWordStruct fileReadWord;

    fp = openFile(fp, "../data/words.dic");

    // inputWord(&fileWriteWord);
    // fileWriteWord.isUsed = FALSE;
    // writeWord(fp, fileWriteWord);

    rewind(fp);
    while(!feof(fp)) {
        printf("feof: %d\n", feof(fp));
        fread(&fileReadWord, sizeof(fileReadWord), 1, fp);
        printf("inputword: %ls\n", fileReadWord.inputWord);
        printf("isUsed: %d\n", fileReadWord.isUsed);
        printf("ftell: %ld\n", ftell(fp));
    }

    while(1) {
        // printf("readWord() start\n");
        if(readWord(fp, fileWriteWord.inputWord, &fileReadWord) == 0) {     // Com의 패배
            printf("Com 패배\n");
            // printf("existWord() start\n");

            if(existWord(fp, fileWriteWord.inputWord) == 1) {      // 단어장에 없던 단어
                // printf("writeWord() start\n");
                writeWord(fp, fileWriteWord);
                return 0;
            } else {
                return 0;
            }
        }

        printf("%ls\n", fileReadWord.inputWord);

        // printf("inputWord() start\n");
        inputWord(&fileWriteWord);

        //timer(5)      // 5초 제한시간

        // printf("checkWordSame() start\n");
        if(!checkWordSame(fileReadWord.inputWord, fileWriteWord.inputWord)) {
            printf("com: %ls, you: %ls\n", fileReadWord.inputWord, fileWriteWord.inputWord);
            printf("끝말이 일치하지 않음\n");
            resetUsed(fp);
            return 0;
        }
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

void resetUsed(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long int wordCount = ftell(fp) / STRUCTSIZE;
    long int curPos;
    inputWordStruct temp;

    rewind(fp);

    for(int i=0; i<wordCount; i++) {
        fseek(fp, i * STRUCTSIZE, SEEK_SET);
        // printf("i*structsize: %d\n", i*STRUCTSIZE);

        fread(&temp, sizeof(temp), 1, fp);
        // printf("%d\n", temp.isUsed);

        if(temp.isUsed != FALSE) {
            curPos = ftell(fp);
            // printf("curpos: %ld\n", curPos);

            temp.isUsed = FALSE;
            fseek(fp, curPos - STRUCTSIZE, SEEK_SET);
            fwrite(&temp, sizeof(temp), 1, fp);
        }
    }
}

void inputWord(inputWordStruct *fileWriteWord)
{
    scanf("%ls", fileWriteWord->inputWord);

    if(wcslen(fileWriteWord->inputWord) <= 1) {     // 입력받은 글자가 한 글자 이하인 경우
        printf("입력이 잘못되었다.");
        exit(-1);
    }
}

int existWord(FILE *fp, wchar_t *comparedWord)
{
    fseek(fp, 0, SEEK_END);
    long int wordCount = ftell(fp) / STRUCTSIZE;
    inputWordStruct temp;
    int existFlag = FALSE;

    rewind(fp);
    for(int i=0; i<wordCount; i++) {
        fread(&temp, sizeof(temp), 1, fp);
        printf("temp: %ls, compared: %ls\n", temp.inputWord, comparedWord);
        if(wcscmp(temp.inputWord, comparedWord) == 0) {
            existFlag = TRUE;
        }
    }
    if(existFlag == TRUE) {
        printf("단어장에 이미 존재하는 단어\n");
        return 0;       // 이미 단어가 존재
    } else {
        printf("단어를 추가합니다.\n");
        return 1;       // 단어장에 없던 단어
    }
}

void writeWord(FILE *fp, inputWordStruct fileWriteWord)
{
    fseek(fp, 0, SEEK_END);

    fileWriteWord.isUsed = FALSE;
    ssize_t ret = fwrite(&fileWriteWord, sizeof(fileWriteWord), 1, fp);

    if(ret != 1) {
        printf("fwrite() error!\n");
        exit(-1);
    }
}

int readWord(FILE *fp, wchar_t *comparedWord, inputWordStruct *fileReadWord)      // 단어장으로 부터 단어 읽기
{
    srand(time(NULL));

    ssize_t ret;
    
    fseek(fp, 0, SEEK_END);
    long int wordCount = ftell(fp) / STRUCTSIZE;      // 단어의 총 개수

    // printf("wordCount: %ld\n", wordCount);

    int random = ((rand() % wordCount) + 1);      // 1 ~ 단어의 개수 만큼 범위의 난수 생성

    // printf("random: %d\n", random);

    fseek(fp, STRUCTSIZE * (random-1), SEEK_SET);   // 랜덤 수 - 1 이 해당 번호의 수
    long int firstPos = ftell(fp);      // 최초의 포지션

    // printf("firstPos: %ld\n", firstPos);

    do {
        ret = fread(fileReadWord, sizeof(*fileReadWord), 1, fp);

        if(ret != 1) {   // fread() 오류 발생 시
            printf("fread() error!\n");
            exit(-1);
        }

        if(ftell(fp) == (wordCount * STRUCTSIZE)) {        // 단어장의 끝에 도달
            fseek(fp, 0, SEEK_SET);
        }

        if(ftell(fp) == firstPos && wcscmp(comparedWord, L"") != 0) {     // 단어장을 한바퀴 다 돌았을 때(Com 패배)
            return 0;
        }

        // printf("compare: %d\n", wcscmp(comparedWord, L""));
        // printf("check: %d\n", checkWordSame(comparedWord, fileReadWord->inputWord));

        if(checkWordSame(comparedWord, fileReadWord->inputWord) || wcscmp(comparedWord, L"") == 0) {      // 사용자 입력단어와 일치하다면
            // printf("inputword: %ls\n", fileReadWord->inputWord);
            printf("isUsed: %d\n", fileReadWord->isUsed);
            if(fileReadWord->isUsed == FALSE) {     // 사용되지 않은 단어일 경우
                fileReadWord->isUsed = TRUE;
                fseek(fp, -STRUCTSIZE, SEEK_CUR);
                printf("ftell(): %ld\n", ftell(fp));
                fread(fileReadWord, sizeof(fileReadWord), 1, fp);
                return 1;
            }
        }
    } while(1);      // 이미 사용된 단어가 아닐 때 까지 반복
}

int checkWordSame(wchar_t w1[], wchar_t w2[])
{
    setlocale(LC_ALL, "");
    wchar_t last = w1[wcslen(w1) - 1];
    // printf("%lc\n", last);
    wchar_t first = w2[0];
    // printf("%lc\n", first);

    if(last == first) {
        return 1;       // 일치하면
    } else {
        return 0;       // 일치하지 않으면
    }
}

FILE *openFile(FILE *fp, char *fileName)
{
    fp = fopen(fileName, "r+");

    if(fp == NULL) {
        printf("file open error!\n");
        exit(1);
    } 

    return fp;
}

int main()
{
    vsCom();
    return 0;
}