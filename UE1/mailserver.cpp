
// C Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
// C++ Imports
#include <string>
using namespace std;
//My Imports
#include "ServerSocket.hpp"
#include "threadServer.hpp"
#include "ClientSocket.hpp"
#include "messagehandler.hpp"

#include "filehelper.h"

void * socketThread(void *arg)
{
  threadServer shrekZwei = *((threadServer *)arg);
  ClientSocket c = shrekZwei.shiti;
  string dir = shrekZwei.shata;
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

int main(int argc, char **argv)
{
   /*
   CHECK IF GIVEN MAILDIRECTORY EXISTS
   */
   if( argc < 3 ){
      printf("Usage: %s Mailpoolpath Port\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   if(doesDirectoryExist(argv[1]) != 0){
      exit(EXIT_FAILURE);
   }
   int port = atoi(argv[2]);

  string dir(argv[1]);

  ServerSocket s(port);
  printf("Server started with port %d\n", port);

  ServerSocket *shiet;
  shiet = &s;
  pthread_t tid[60];
  int limiter = 0;

   while (1)
   {
     ClientSocket c = s.acceptClient();
     printf("accepted \n");

     threadServer shrek { c, dir};
     sleep(2);
     if(pthread_create(&tid[limiter], NULL, socketThread, &shrek) != 0)
     {
       printf("shit ain't good fam\n");
     }
     else
     {
       printf("sweet\n");
     }
     pthread_join(tid[limiter], NULL);

     if(limiter >= 50)
     {
       limiter = 0;
       while(limiter < 50)
       {
         pthread_join(tid[limiter++],NULL);
       }
       limiter = 0;
     }
   }

   return 1;
}
