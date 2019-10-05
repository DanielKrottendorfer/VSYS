#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int initServersocket(int *server_socket, struct sockaddr_in *address, int port)
{
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(address, 0, sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);

    if (bind(*server_socket, (struct sockaddr *)address, sizeof(*address)) != 0)
    {
        perror("bind error");
        return EXIT_FAILURE;
    }
    if (listen(*server_socket, 5) != 0)
    {
        perror("listen error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}