
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
vector<tuple<thread,bool>> threadboolList;

void signal_handler(int signal)
{
  for(int x = 0; x < threadboolList.size() ; x++)
  {
    bool * b = &(get<1>(threadboolList[x]));
    *b = false;
  }

  for(int x = 0; x < threadboolList.size() ; x++)
  {
    (get<0>(threadboolList[x])).join();
  }
  exit(EXIT_SUCCESS);
}

void socketThreadFunction(string dir, ClientSocket *c, bool *running)
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
  }while (strncmp (message.c_str(), "quit", 4)  != 0 && *running);
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
      bool b = true;
      ClientSocket c = s.acceptClient();
      printf("accepted \n");
      std::thread t(socketThreadFunction, dir, &c, &b);
      socketList.push_back(std::move(c));
      threadboolList.push_back(tuple<thread,bool>(std::move(t), std::move(b)));
   }

   return 1;
}
