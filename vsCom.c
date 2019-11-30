#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#define MAXLENGTH 20
#define TRUE 1
#define FALSE 0

FILE *openFile(FILE *fp, char *fileName);
int checkWordSame(wchar_t *w1, wchar_t *w2);
wchar_t *readWord(FILE *fp, wchar_t *comparedWord);

typedef struct inputWord    // 단어를 저장하기 위한 구조체
{
    wchar_t *inputWord;
    int isUsed;
} inputWordStruct;

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *fp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;    // 파일에 넣을 단어를 저장하는 구조체
    inputWordStruct fileReadWord;     // 파일에서 꺼내온 단어를 저장하는 구조체
    fileWriteWord.inputWord = malloc(sizeof(char) * MAXLENGTH);  // 사용자로부터 입력받는 단어를 저장한 변수

    fp = openFile(fp, "words");
    
    scanf("%ls", fileWriteWord.inputWord);

    fwrite(&fileWriteWord, sizeof(fileWriteWord), 1, fp);      
    
    fseek(fp, 0, SEEK_SET);
    fread(&fileReadWord, sizeof(fileReadWord), 1, fp);
    printf("dd:%ls\n", fileReadWord.inputWord);
    printf("%ld\n", ftell(fp));

    //checkWordSame();

    free(fileWriteWord.inputWord);
    fclose(fp);
}

wchar_t *readWord(FILE *fp, wchar_t *comparedWord)      // 단어장으로 부터 단어 읽기
{
    srand(time(NULL));

    inputWordStruct temp;     // 단어 비교를 위한 임시로 단어를 담아놓을 변수
    
    fseek(fp, 0, SEEK_END);
    long int wordCount = ftell(fp) / 9;      // 단어의 총 개수

    int random = (rand() % wordCount + 1);      // 0 ~ 단어의 개수 만큼 범위의 난수 생성

    fseek(fp, 9 * random, SEEK_SET);
    fread(&temp, sizeof(temp), 1, fp); 

    while(temp.isUsed != FALSE) {       // 이미 사용된 단어가 아닐 때 까지 반복
        fread(&temp, sizeof(temp), 1, fp);
        if(ftell(fp) == (wordCount * 9) - 1) {        // 단어장의 끝에 도달
            fseek(fp, 0, SEEK_SET);
        }
    }

    return temp.inputWord;
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