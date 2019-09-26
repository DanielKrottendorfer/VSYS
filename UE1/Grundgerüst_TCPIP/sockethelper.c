/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int initServersocket(int* server_socket, struct sockaddr_in* address, int port){

   *server_socket = socket (AF_INET, SOCK_STREAM, 0);
   memset(address,0,sizeof(*address));
   address->sin_family = AF_INET;
   address->sin_addr.s_addr = INADDR_ANY;
   address->sin_port = htons (port);

   if (bind ( *server_socket, (struct sockaddr *) address, sizeof (*address)) != 0) {
      perror("bind error");
      return EXIT_FAILURE;
   }
   if(listen (*server_socket, 5) != 0 ){
      perror("listen error");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

int sendMessage(char* buffer, char* mailpath){

    char* temp = &buffer[1];

    char reciever[9] = "        \n";

    for(int i=0 ; temp[i] != '\n' ; i++){
        reciever[i] = temp[i];
        printf("abc3 %s\n",reciever);
        sleep(1);
    }

    printf("%s",reciever);

    return EXIT_SUCCESS;
}

int listMessages(char* buffer, char* mailpath, int socket){

    return EXIT_SUCCESS;
}
int readMessage(char* buffer, char* mailpath, int socket){

    return EXIT_SUCCESS;
}
int deleteMessage(char* buffer, char* mailpath, int socket){

    return EXIT_SUCCESS;
}
#include <unistd.h>
int handleMessage(char* buffer, char* mailpath, int socket){

    int requestType = *buffer -48;

    /*
    0 => SEND
    1 => LIST
    2 => READ
    3 => DEL
    4 => QUIT
    */
    
    switch(requestType){
        case 0: sendMessage(buffer, mailpath);break;
        case 1: listMessages(buffer, mailpath, socket);break;
        case 2: readMessage(buffer, mailpath, socket);break;
        case 3: deleteMessage(buffer, mailpath, socket);break;
        case 4: exit(EXIT_SUCCESS);break;
    }

    return EXIT_SUCCESS;
}

