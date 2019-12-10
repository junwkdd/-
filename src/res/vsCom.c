#include "../header/vsGame.h"
#include "../header/wordGame.h"

int vsCom()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    FILE *wordFp;   // 파일 포인터 선언
    inputWordStruct fileWriteWord;
    inputWordStruct fileReadWord;

    int curPos;

    wordFp = openFile(wordFp, "../../data/words.dic");

    resetUsed(wordFp);

    wcscpy(fileWriteWord.inputWord, L"");

    while(1) {
        curPos = readWord(wordFp, fileWriteWord.inputWord, &fileReadWord);
        if(curPos == 0) {     // Com의 패배
            printf("Com 패배\n");
            if(existWord(wordFp, fileWriteWord.inputWord) == 1) {      // 단어장에 없던 단어
                writeWord(wordFp, fileWriteWord);
            }
            return 0;
        }

        printf("com: %ls\n", fileReadWord.inputWord);

        printf("you: ");
        inputWord(&fileWriteWord);

        if(checkWordSame(fileReadWord.inputWord, fileWriteWord.inputWord) == 1) {       // 사용자 입력단어와 일치하면
            printf("단어 적합성 확인중...\n");
            if(validWord(fileWriteWord.inputWord) == 0) {            // 올바른 단어가 아니라면
                return 0;
            }
            printf("올바른 단어\n\n");
        } else {
            printf("끝말이 일치하지 않습니다.\n");
            printf("com: %ls, you: %ls\n\n", fileReadWord.inputWord, fileWriteWord.inputWord);
            return 0;
        }
        //timer(5)      // 5초 제한시간

    }

    fclose(wordFp);
} 