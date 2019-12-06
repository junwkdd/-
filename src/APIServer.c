#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <wchar.h>
#include <unistd.h> 
#include <string.h> 
#include <locale.h>
#include <netinet/in.h> 
#include <netdb.h>

#define TRUE 1
#define FALSE 0
#define SERVERPORT 3000
#define CLIENTPORT 80
#define URL "stdict.korean.go.kr"
#define BUFFERSIZE 1024
#define KEY "499B702221DEC1E4174FF462866A64DD"

char client(char *word);

int main() 
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    printf("서버 준비완료\n");

    int serverSocket, newSocket, recvVal; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrLen = sizeof(address); 
    char buffer[1024] = {0}; 

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)  { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( SERVERPORT ); 

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    if (listen(serverSocket, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    while(1) {
        char buffer[1024] = {0}; 

        printf("클라이언트 대기중...\n");
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrLen))<0) { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        
        printf("클라이언트 소켓 전달받음...\n");
        recvVal = recv(newSocket, buffer, 1024, 0);

        char vaildWord = client(buffer);

        if(vaildWord == '1') {
            printf("%s: 올바른 단어\n\n", buffer);
        } else if(vaildWord == '2') {
            printf("%s: 명사가 아닌 단어\n\n", buffer);
        } else {
            printf("%s: 존재하지 않는 단어\n\n", buffer);
        }

        send(newSocket, &vaildWord , 1 , 0 ); 
    }
}


char client(char *word)
{
    int clientSocket = 0, recvVal; 
    struct sockaddr_in serverAddr; 
    struct hostent *host;
    wchar_t pos;
    char buffer[BUFFERSIZE] = {0};
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
    } 

    host = gethostbyname(URL);

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(CLIENTPORT); 
    serverAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
   
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) { 
        printf("\nConnection Failed \n"); 
    } 

    char header[BUFFERSIZE] = "GET /api/search.do"; 

    strcat(header, "?key=");
    strcat(header, KEY);

    strcat(header, "&q=");
    strcat(header, word);

    strcat(header, " HTTP/1.1\r\n");

    strcat(header, "Host: stdict.korean.go.kr\r\n\r\n");

    send(clientSocket , header , strlen(header) , 0 );      // 서버에 request 요청

    recv(clientSocket, buffer, 1024, 0);        // 서버 응답을 버퍼에 수신

    char *totalPtr = strstr(buffer, "<total>");
    int total = totalPtr[7] - '0';

    if(total > 0) {
        char *posPtr = strstr(buffer, "<pos>");
        mbstowcs(&pos, &posPtr[5], 3);      // 3바이트 한글을 wchar 한바이트에 저장

        if(pos == L'명') {
            return '1';         // 올바른 단어
        } else {
            return '2';         // 명사가 아닌 단어
        }
    } else {
        return '3';             // 존재하지 않는 단어
    }
}