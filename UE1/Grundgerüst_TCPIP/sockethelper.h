#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#ifndef SOCKET_HELPER
#define SOCKET_HELPER

    int initServersocket(int* server_socket, struct sockaddr_in* address, int port);
    int sendMessage     (string message, string mailpath);
    int listMessages    (string message, string mailpath, int socket);
    int readMessage     (string message, string mailpath, int socket);
    int deleteMessage   (string message, string mailpath, int socket);
    int handleMessage   (string message, string mailpath,int socket);

#endif