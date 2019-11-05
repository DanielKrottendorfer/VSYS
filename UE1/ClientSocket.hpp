
#include <netinet/in.h>

#include <string>
using namespace std;

#ifndef CLIENT_SOCKET
#define CLIENT_SOCKET

#define BUF 1030

class ClientSocket
{
    private:
        struct sockaddr_in client_address;
        int client_socket;
        char buffer[BUF];
    public:
    
        static ClientSocket connectToSocket(string ip, int port);
        void closeCon();
        ClientSocket(struct sockaddr_in ca, int cs);
        int sendMessage(string message);
        int recieveMessage(string& buffer);
        ~ClientSocket();
};

#endif 