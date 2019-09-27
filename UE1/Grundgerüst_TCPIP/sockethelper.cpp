/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
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

#include <string>
#include <sstream>
#include <iostream>

#include "filehelper.h"

using namespace std;

int sendMessage(char* buffer, string mailpath){

    string reciever;    
    string betreff;    
    string message;

    char* temp = &buffer[1];
    istringstream f(temp);

    getline(f, reciever);
    getline(f, betreff);
    getline(f, message);


    if( doesDirectoryExist( (mailpath + reciever ).c_str() ) != 0 )
    {
        mkdir((mailpath + reciever).c_str(),0700);
    }

    reciever.append(1,'/');

    FILE* file = fopen((mailpath + reciever + betreff ).c_str(),"w");

    if(file == nullptr){
        printf("ERROR ON FOPEN\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file,"%s",message.c_str());   
    fclose(file);

    return EXIT_SUCCESS;

}

int listMessages(char* buffer, string mailpath, int socket){

    return EXIT_SUCCESS;
}
int readMessage(char* buffer, string mailpath, int socket){

    return EXIT_SUCCESS;
}
int deleteMessage(char* buffer, string mailpath, int socket){

    return EXIT_SUCCESS;
}
#include <unistd.h>
int handleMessage(char* buffer, string mailpath, int socket){

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

