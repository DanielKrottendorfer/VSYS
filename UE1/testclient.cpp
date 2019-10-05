#include "ClientSocket.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    {
        ClientSocket cs = ClientSocket::connectToSocket("127.0.0.1",12345);
        cs.sendMessage("hello\n");
    }
    return 0;
}