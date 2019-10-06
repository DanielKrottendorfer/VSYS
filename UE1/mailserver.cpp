
// C Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
// C++ Imports
#include <string>
using namespace std;
//My Imports
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "messagehandler.hpp"

#include "filehelper.h"

int main(int argc, char **argv)
{
   /*
   CHECK IF GIVEN MAILDIRECTORY EXISTS
   */
   if( argc < 2 ){
      printf("Usage: %s Mailpoolpath\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   if(doesDirectoryExist(argv[1]) != 0){
      exit(EXIT_FAILURE);
   }

   string dir(argv[1]);

   ServerSocket s(12345);

   while (1)
   {
      ClientSocket c = s.acceptClient();
      printf("accepted \n");

      string message;
      do
      {
         int size = c.recieveMessage(message);
         if( size > 0)
         {
            printf("Message received: \n%s\n", message.c_str());
            handleMessage(message, dir, &c);
         }
         else if (size == 0)
         {
            printf("Client closed remote socket\n");
            break;
         }
         else
         {
            perror("Client disconnected");
            break;
         }
      }while (strncmp (message.c_str(), "quit", 4)  != 0);
      
   }

   return 1;
}