
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
        string ip;
    public:

        int i=0;
        static ClientSocket connectToSocket(string ip, int port);
        void closeCon();
        ClientSocket(struct sockaddr_in ca, int cs, string ip);
        int sendMessage(string message);
        int recieveMessage(string& buffer);
        int recieveMessageWait(string& buffer);
        string getIP();
        ~ClientSocket();
};

#endif 