
#include "ClientSocket.hpp"
#include "ServerSocket.hpp"

#include <stdio.h>
#include <stdlib.h>

int main(){

    ServerSocket s(12345);

    while (1)
    {

        ClientSocket c = s.acceptClient();

        int size = 0;

        do{
            string message;
            size = c.recieveMessage(message);
            c.sendMessage(message);
        }while (size>0);
        
    }

    return 0;
}