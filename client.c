#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>  
#define PORT 4444 
#define BUF_SIZE 2000 
//add new
#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201
int sockfd = 0;
int close(int fd);
volatile sig_atomic_t flag = 0;
char nickname[LENGTH_NAME] = {};
void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}
void recv_msg_handler() {
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {
            printf("\r%s\n", receiveMessage);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else { 
            // -1 
        }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, LENGTH_MSG, stdin) != NULL) {
            str_trim_lf(message, LENGTH_MSG);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        send(sockfd, message, LENGTH_MSG, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }
}

//add new
in_addr_t inet_addr(const char *cp);  

int main(int argc, char**argv) {  
    struct sockaddr_in addr, cl_addr;  
    int ret;  
    char buffer[BUF_SIZE];  
    struct hostent * server;
    char * serverAddr;

    if (argc < 2) 
    {
        printf("usage: client < ip address >\n");
        exit(1);  
    }

    serverAddr = argv[1]; 
    // Naming
    printf("Please enter your name: ");
    if (fgets(nickname, LENGTH_NAME, stdin) != NULL) {
        str_trim_lf(nickname, LENGTH_NAME);
    }
    
    //add new
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd < 0) 
    {  
        printf("Error creating socket!\n");  
        exit(1);  
    }  
    printf("Socket created...\n");   

    memset(&addr, 0, sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(serverAddr);
    addr.sin_port = PORT;     

    ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
    if (ret < 0) 
    {  
        printf("Error connecting to the server!\n");  
        exit(1);  
    }  
    printf("Connected to the server...\n");  
    //add new
    // Names
    //getsockname(sockfd, (struct sockaddr*) &cl_addr, (socklen_t*) &c_addrlen);
   // getpeername(sockfd, (struct sockaddr*) &addr, (socklen_t*) &s_addrlen);
    //printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
   // printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    send(sockfd, nickname, LENGTH_NAME, 0);

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }

    close(sockfd);
    //end add new
    //memset(buffer, 0, BUF_SIZE);
    //printf("Me: ");

    //while (fgets(buffer, BUF_SIZE, stdin) != NULL) 
    //{
      //  ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));  
     //   if (ret < 0) 
     //   {  
      //      printf("Error sending data!\n\t-%s", buffer);  
      //  }
      //  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);  
      //  if (ret < 0) 
      //  {  
       //     printf("Error receiving data!\n");    
       // } 
      //  else 
      //  {
      //      printf("Server: ");
      //      fputs(buffer, stdout);
            //printf("\n");
      //  }  
   // }
    
    return 0;    
}  