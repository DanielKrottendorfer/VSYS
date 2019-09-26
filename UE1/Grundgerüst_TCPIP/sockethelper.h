#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef SOCKET_HELPER
#define SOCKET_HELPER

    int initServersocket(int* server_socket, struct sockaddr_in* address, int port);
    int sendMessage(char* buffer, char* mailpath);
    int listMessages(char* buffer, char* mailpath, int socket);
    int readMessage(char* buffer, char* mailpath, int socket);
    int deleteMessage(char* buffer, char* mailpath, int socket);
    int handleMessage(char* buffer,char* mailpath,int socket);

#endif