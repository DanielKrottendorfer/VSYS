
// C Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
// C++ Imports
#include <string>
#include <csignal>
#include <thread>
#include <vector>
#include <tuple>
using namespace std;
//My Imports
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "messagehandler.hpp"

#include "filehelper.h"

vector<ClientSocket> socketList;
vector<thread> threadList;
bool terminateThreads = true;

void signal_handler(int signal)
{
  terminateThreads = false;

  for(int x = 0; x < threadList.size() ; x++)
  {
    threadList.at(x).join();
  }
  exit(EXIT_SUCCESS);
}

void socketThreadFunction(string dir, ClientSocket *c)
{
  string message;
  do
  {
    int size = c->recieveMessage(message);
    if( size > 0)
    {
        printf("Message received: \n%s\n", message.c_str());
        handleMessage(message, dir, c);
    }
    else if (size == 0)
    {
      printf("Client closed remote socket\n");
      break;
    }
    else
    {
      sleep(1);
      printf("sleep\n");
    }
  }while (strncmp (message.c_str(), "quit", 4)  != 0 && terminateThreads);
  c->closeCon();
}

int main(int argc, char **argv)
{
   /*
   CHECK IF GIVEN MAILDIRECTORY EXISTS
   */
    signal(SIGINT, signal_handler);
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

   printf("Server started with port %d\n",port);

   while (1)
   {
      ClientSocket c = s.acceptClient();
      printf("accepted \n");
      std::thread t(socketThreadFunction, dir, &c);
      socketList.push_back(std::move(c));
      threadList.push_back(std::move(t));
   }

   return 1;
}
