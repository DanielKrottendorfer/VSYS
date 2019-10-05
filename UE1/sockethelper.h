
#include <netinet/in.h>

#ifndef SOCKET_HELPER
#define SOCKET_HELPER

    int initServersocket(int* server_socket, struct sockaddr_in* address, int port);
   
#endif