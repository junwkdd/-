#include "header/wordGame.h"

int server()
{
    int serverSocket, clientSocket[2], recvVal; 
    struct sockaddr_in serverAddr;
    int opt = 1; 
    int i = 0;
    int addrLen = sizeof(serverAddr); 
    char *word;
       
    // Creating socket file descriptor 
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons( 4000 ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(serverSocket, 2) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }

    while(1) {
        for(i=0; i<2; i++) {
            if ((clientSocket[i] = accept(serverSocket, (struct sockaddr *)&serverAddr, (socklen_t*)&addrLen))<0) { 
                perror("accept"); 
                exit(EXIT_FAILURE); 
            }
        }

        printf("매칭 성공\n");

        char matching[2] = {'1', '2'};

        for(i=0; i<2; i++) {
            send(clientSocket[i], &matching[i], 1, 0);
        }

        int i = 0;
        while(1) {
            word = malloc(1024);
            printf("recv 대기중...\n");
            printf("before recv: %s\n", word);
            recv(clientSocket[i++], word, 1024, 0);
            printf("after recv: %s\n", word);
            i %= 2;
            send(clientSocket[i] ,word ,1024 , 0 ); 
            free(word);
        }

        for(i=0; i<2; i++) {
            close(clientSocket[i]);
        }
        printf("yabal\n");
    }
}

int main()
{
    while(1) {
        server();
    }
}