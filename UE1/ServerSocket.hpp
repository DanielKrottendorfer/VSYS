
#include <netinet/in.h>
#include "ClientSocket.hpp"

class ServerSocket
{
private:
    int server_socket;
    struct sockaddr_in address;
public:
    ClientSocket acceptClient();
    ServerSocket(int port);
    ~ServerSocket();
};

