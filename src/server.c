#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <wchar.h>
#include <netinet/in.h> 
#include <string.h> 
#include <locale.h>

#define PORT 3000 

int main(int argc, char const *argv[]) 
{
    setlocale(LC_ALL, "");  // 한글 사용을 위한 지역 설정

    int serverSocket;
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    // wchar_t buffer[1024] = {0}; 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 

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
    address.sin_port = htons( PORT ); 

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    if (listen(serverSocket, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    if ((new_socket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    
    valread = read( new_socket , buffer, 1024); 
    printf("buffer: %s\n",buffer ); 
    // send(new_socket , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 
    return 0;
}