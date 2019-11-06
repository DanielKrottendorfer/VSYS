
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <string>

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "sockethelper.h"


ServerSocket::ServerSocket(int port)
{
    int server_socket;
    struct sockaddr_in address;
    if(initServersocket(&server_socket,&address,port) != 0){
        printf("Failed to initialice Serversocket\n");
        exit(EXIT_FAILURE);
    }
    this->server_socket = server_socket;
    this->address = address;
}

ClientSocket* ServerSocket::acceptClient()
{
    socklen_t addrlen;
    addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddress;
    int client_socket;

    printf("Waiting for connections...\n");
    client_socket = accept ( this->server_socket, (struct sockaddr *) &clientaddress, &addrlen );
    if (client_socket > 0)
    {
        printf ("Client connected from %s:%d...\n", inet_ntoa (clientaddress.sin_addr),ntohs(clientaddress.sin_port));
        std::string message = "Welcome to the TWMailer Server, Please login:\n";
        send(client_socket, message.c_str(), message.length(),0);
    }
    return new ClientSocket(clientaddress, client_socket);
}

ServerSocket::~ServerSocket()
{
    printf("closing server socket\n");
    close(this->server_socket);
}
