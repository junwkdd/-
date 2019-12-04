#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 

#define MAXLENGTH 10
#define STRUCTSIZE 44
#define TRUE 1
#define FALSE 0
#define PORT 3000

typedef struct inputWord    // 단어를 저장하기 위한 구조체
{
    wchar_t inputWord[MAXLENGTH];
    int isUsed;
} inputWordStruct;

FILE *openFile(FILE *wordFp, char *fileName);
int checkWordSame(wchar_t *w1, wchar_t *w2);
wchar_t readWord(FILE *wordFp, wchar_t *comparedWord, inputWordStruct *fileReadWord);
void writeWord(FILE *wordFp, inputWordStruct fileWriteWord);
int existWord(FILE *wordFp, wchar_t *comparedWord);
void inputWord(inputWordStruct *fileWriteWord);
void resetUsed(FILE *wordFp);
void wordSend(wchar_t *word);
void timer();

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *wordFp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;
    inputWordStruct fileReadWord;

    int curPos;

    wordFp = openFile(wordFp, "../data/words.dic");

    resetUsed(wordFp);

    rewind(wordFp);
    while(!feof(wordFp)) {
        fread(&fileReadWord, sizeof(fileReadWord), 1, wordFp);
    }

    while(1) {
        curPos = readWord(wordFp, fileWriteWord.inputWord, &fileReadWord);
        if(curPos == 0) {     // Com의 패배
            printf("Com 패배\n");

            if(existWord(wordFp, fileWriteWord.inputWord) == 1) {      // 단어장에 없던 단어
                writeWord(wordFp, fileWriteWord);
                return 0;
            } else {
                return 0;
            }
        }

        printf("com: %ls\n", fileReadWord.inputWord);

        printf("you: ");
        inputWord(&fileWriteWord);
        wordSend(fileWriteWord.inputWord);

        //timer(5)      // 5초 제한시간

        if(!checkWordSame(fileReadWord.inputWord, fileWriteWord.inputWord)) {       // 사용자 입력단와 비교
            printf("com: %ls, you: %ls\n", fileReadWord.inputWord, fileWriteWord.inputWord);
            printf("끝말이 일치하지 않음\n");
            return 0;
        }
    }

    fclose(wordFp);
} 

void wordSend(wchar_t *wordSend)
{
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    char *word = malloc(sizeof(10));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        exit(-1);
    }

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, "192.168.10.131", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        exit(-1);
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  { 
        printf("\nConnection Failed \n"); 
        exit(-1);
    } 

    wcstombs (word, wordSend, 10);

    send(sock , word , strlen(word) , 0 ); 
    printf("client: word sent: %s\n", word);
}

void timer()
{
    for(int i=5; i>=0; i--) {
        printf("%d\n", i);
        sleep(1);
    }
}

void resetUsed(FILE *wordFp)            // 사용된 단어 초기화
{
    fseek(wordFp, 0, SEEK_END);
    long int wordCount = ftell(wordFp) / STRUCTSIZE;
    long int curPos;
    inputWordStruct temp;

    rewind(wordFp);

    for(int i=0; i<wordCount; i++) {
        fseek(wordFp, i * STRUCTSIZE, SEEK_SET);

        fread(&temp, sizeof(temp), 1, wordFp);

        if(temp.isUsed != FALSE) {
            curPos = ftell(wordFp);

            temp.isUsed = FALSE;
            fseek(wordFp, curPos - STRUCTSIZE, SEEK_SET);
            fwrite(&temp, sizeof(temp), 1, wordFp);
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

int existWord(FILE *wordFp, wchar_t *comparedWord)
{
    fseek(wordFp, 0, SEEK_END);
    long int wordCount = ftell(wordFp) / STRUCTSIZE;
    inputWordStruct temp;
    int existFlag = FALSE;

    rewind(wordFp);
    for(int i=0; i<wordCount; i++) {
        fread(&temp, sizeof(temp), 1, wordFp);
        if(wcscmp(temp.inputWord, comparedWord) == 0) {         // 같은 단어가 있다면
            existFlag = TRUE;
            break;
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

void writeWord(FILE *wordFp, inputWordStruct fileWriteWord)
{
    // FILE *dicFp = fopen("../data/dictionary.dic", "r");       // 읽기전용으로 사전파일 열기 
    // int appropriateWord = FALSE;
    // wchar_t temp[10];

    // while(!feof(dicFp)) {
    //     fgetws(temp, sizeof(temp), dicFp);
    //     if(wcscmp(temp, fileWriteWord.inputWord) == 0) {
    //         printf("%ls\n", temp);
    //         appropriateWord = TRUE;
    //         break;
    //     }
    // }

    // if(appropriateWord == TRUE) {
    //     fseek(wordFp, 0, SEEK_END);

    fileWriteWord.isUsed = FALSE;
    ssize_t ret = fwrite(&fileWriteWord, sizeof(fileWriteWord), 1, wordFp);

    if(ret != 1) {
        printf("fwrite() error!\n");
        exit(-1);
    }
    // } else {
    //     printf("적절하지 않은 단어입니다.\n");
    //     printf("그럼에도 불구하고 추가하기\n");
    // }

    // fclose(dicFp);
}

int readWord(FILE *wordFp, wchar_t *comparedWord, inputWordStruct *fileReadWord)      // 단어장으로 부터 단어 읽기
{
    srand(time(NULL));

    ssize_t ret;
    
    fseek(wordFp, 0, SEEK_END);
    long int wordCount = ftell(wordFp) / STRUCTSIZE;      // 단어의 총 개수

    if(wordCount == 0) {
        inputWord(fileReadWord);
        writeWord(wordFp, *fileReadWord);
        exit(-1);
    }

    int random = ((rand() % wordCount) + 1);      // 1 ~ 단어의 개수 만큼 범위의 난수 생성

    fseek(wordFp, STRUCTSIZE * (random-1), SEEK_SET);   // 랜덤 수 - 1 이 해당 번호의 수

    long int firstPos = ftell(wordFp);      // 최초의 포지션

    do {
        ret = fread(fileReadWord, sizeof(*fileReadWord), 1, wordFp);

        if(ret != 1) {   // fread() 오류 발생 시
            printf("fread() error!\n");
            exit(-1);
        }

        int curPos = ftell(wordFp);

        if(ftell(wordFp) == (wordCount * STRUCTSIZE)) {        // 단어장의 끝에 도달
            fseek(wordFp, 0, SEEK_SET);
        }

        if(checkWordSame(comparedWord, fileReadWord->inputWord) || wcscmp(comparedWord, L"") == 0) {      // 사용자 입력단어와 일치하다면
            if(fileReadWord->isUsed == FALSE) {     // 사용되지 않은 단어일 경우
                fileReadWord->isUsed = TRUE;

                fseek(wordFp, curPos-STRUCTSIZE, SEEK_SET);
                fwrite(fileReadWord, sizeof(*fileReadWord), 1, wordFp);

                return curPos;
            }
        }

        if(ftell(wordFp) == firstPos && wcscmp(comparedWord, L"") != 0) {     // 단어장을 한바퀴 다 돌았을 때(Com 패배)
            return 0;
        }

    } while(1);      // 이미 사용된 단어가 아닐 때 까지 반복
}

int checkWordSame(wchar_t w1[], wchar_t w2[])
{
    setlocale(LC_ALL, "");
    wchar_t last = w1[wcslen(w1) - 1];
    wchar_t first = w2[0];

    if(last == first) {
        return 1;       // 일치하면
    } else {
        return 0;       // 일치하지 않으면
    }
}

FILE *openFile(FILE *wordFp, char *fileName)
{
    wordFp = fopen(fileName, "r+");

    if(wordFp == NULL) {
        printf("file open error!\n");
        exit(1);
    } 

    return wordFp;
}

int main()
{
    system("clear");

    vsCom();

    sleep(3);

    system("clear");

    return 0;
}