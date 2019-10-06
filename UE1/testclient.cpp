#include "ClientSocket.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
using namespace std;

int main(){
    {
        string message;
        ClientSocket cs = ClientSocket::connectToSocket("127.0.0.1",12345);
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());
        cs.sendMessage("0\nif18b030\nMarius\nAre you gay?\nMeele ist ein verbuggtes Drecksgame");
        sleep(1);

        cs.sendMessage("1\nMarius");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());

        cs.sendMessage("2\nMarius\n292");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());
        
        cs.sendMessage("3\nMarius\n292");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());

    }
    return 0;
}