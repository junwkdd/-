#include "../header/vsGame.h"
#include "../header/wordGame.h"

int vsPerson()
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    int clientSocket;
    char word[1024] = {0};
    char recvVal;
    int lose = 0, win = 0;

    inputWordStruct myWord;
    inputWordStruct yourWord;

    if((clientSocket = connectServer()) == -1) {
        return -1;
    }

    recvVal = matchingPerson(clientSocket);

    while(1) {        
        if((recvVal % 2) == 1) {
            printf("당신의 차례: ");
            inputWord(&myWord);
            
            if(checkWordSame(yourWord.inputWord, myWord.inputWord) == 1 || recvVal == 1) {       // 사용자 입력단어와 일치하면
                printf("단어 적합성 확인중...\n");
                if(validWord(myWord.inputWord) == 0) {            // 올바른 단어가 아니라면
                    lose = 1;
                } else {
                    printf("올바른 단어\n\n");
                }
            } else {
                printf("끝말이 일치하지 않습니다.\n");
                printf("상대방: %ls, 당신: %ls\n\n", yourWord.inputWord, myWord.inputWord);
                lose = 1;
            }

            strcpy(word, " ");
            wcstombs(word, myWord.inputWord, 1024);

            send(clientSocket , word, strlen(word) , 0); 

            if(lose == 1) {
                printf("당신의 패배...\n");
                return 0;
            }

            recvVal++;
        } else {
            printf("상대방의 차례: ");

            strcpy(word, " ");
            recv( clientSocket , word, 1024, 0); 

            printf("%s\n",word);

            mbstowcs(yourWord.inputWord, word, 1024);

            if(checkWordSame(myWord.inputWord, yourWord.inputWord) == 1 || recvVal == 2) {       // 사용자 입력단어와 일치하면
                printf("단어 적합성 확인중...\n");
                if(validWord(yourWord.inputWord) == 0) {            // 올바른 단어가 아니라면
                    win = 1;
                } else {
                    printf("올바른 단어\n\n");
                }
            } else {
                printf("끝말이 일치하지 않습니다.\n");
                printf("당신: %ls, 상대방: %ls\n\n", myWord.inputWord, yourWord.inputWord);
                win = 1;
            }

            if(win == 1) {
                printf("당신의 승리!!!\n");
                return 0;
            }

            recvVal++;
        }
    }
} 


int matchingPerson(int clientSocket)
{
    char recvVal;

    printf("상대방 매칭중...\n");

    recv(clientSocket, &recvVal, 1024, 0);

    if(recvVal != '0') {
        printf("매칭 성공!\n");
        sleep(2);
        if(recvVal == '1') {
            printf("첫번째 차례입니다.\n");
        } else {
            printf("두번째 차례입니다.\n");
        }
        system("clear");
        return (recvVal - '0');
    }
}

int connectServer()
{
    int clientSocket = 0; 
    struct sockaddr_in serverAddr; 

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("소켓 생성 에러\n"); 
        return -1; 
    } 
   
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(4000); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0)  
    { 
        printf("잘못된 서버 주소\n"); 
        return -1; 
    } 
   
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) { 
        printf("서버가 열려있지 않습니다.\n"); 
        return -1; 
    }

    printf("\n서버 접속 성공\n");

    return clientSocket;
}