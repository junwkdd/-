#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define MAXLENGTH 10
#define STRUCTSIZE 44
#define BUFFERSIZE 1024
#define TRUE 1
#define FALSE 0
#define PORT 3000

typedef struct inputWord    // 단어를 저장하기 위한 구조체
{
    wchar_t inputWord[MAXLENGTH];
    int isUsed;
} inputWordStruct;

int validWord(wchar_t *wordSend);
void timer();
void resetUsed(FILE *wordFp);
void inputWord(inputWordStruct *fileWriteWord);
int existWord(FILE *wordFp, wchar_t *comparedWord);
void writeWord(FILE *wordFp, inputWordStruct fileWriteWord);
int readWord(FILE *wordFp, wchar_t *comparedWord, inputWordStruct *fileReadWord);
int checkWordSame(wchar_t w1[], wchar_t w2[]);
FILE *openFile(FILE *wordFp, char *fileName);
int matchingPerson(int clientSocket);
int connectServer();