
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
#include <mutex>
#include <chrono>
#include <iostream>
using namespace std;
//My Imports
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "messagehandler.hpp"
#include "filehelper.h"
#include "ldap.hpp"

#define LOGINTRYS 3

ServerSocket* s = nullptr;

vector<thread> threadList;
bool aliveFlagThread = true;
mutex dir_mutex;

vector<tuple<chrono::time_point<chrono::_V2::system_clock,chrono::nanoseconds>,string>> blackList;
chrono::seconds bantime = chrono::seconds(10);

bool checkIfListed(string ip)
{
  auto now = chrono::system_clock::now();

  for(unsigned int i=0 ; i < blackList.size() ; i++)
  {
    auto t = get<0>(blackList[i]);
    auto elapsed = now-t;
    if(elapsed>bantime){
      blackList.erase(blackList.begin()+i);
      i--;
    }
  }

  for(auto x:blackList)
  {
    string s = get<1>(x);
    if(ip.find(s)>=0)
      return true;
  }
  return false;
}

void signal_handler(int signal)
{
  aliveFlagThread = false;

  for(unsigned int x = 0; x < threadList.size() ; x++)
  {
    threadList.at(x).join();
  }
  delete(s);
  exit(EXIT_SUCCESS);
}

void socketThreadFunction(string dir, ClientSocket* c)
{
  string message;

  if(startLogin(c,LOGINTRYS))
  {
    do
    {
      int size = c->recieveMessage(message);
      if( size > 0)
      {
            dir_mutex.lock();
            printf("Message received: \n%s\n", message.c_str());
            handleMessage(message, dir, c);
            dir_mutex.unlock();
      }
      else if (size == 0)
      {
        printf("Client closed remote socket\n");
        break;
      }
      else
      {
        sleep(1);
        printf("Wait for Client %d ...\n",c->i);
      }
    }while (strncmp (message.c_str(), "quit", 4)  != 0 && aliveFlagThread);
  }else
  {
    auto a = chrono::system_clock::now();
    dir_mutex.lock();
    blackList.push_back(tuple<chrono::time_point<chrono::_V2::system_clock,chrono::nanoseconds>,string>(a,c->getIP()));
    printf("%s is now listed",c->getIP().c_str());
    dir_mutex.unlock();
  }
  
  
  c->closeCon();
  delete(c);
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

   s = new ServerSocket(port);

   printf("Server started with port %d\n",port);

   int i = 0;
   while (1)
   {
      ClientSocket* c = s->acceptClient();
      if(checkIfListed(c->getIP()))
      {
        c->sendMessage("you are banned");
        continue;
      }
      c->sendMessage("Welcome to the TWMailer Server, Please login:\n");
      c->i = i;
      printf("accepted \n");
      std::thread t(socketThreadFunction, dir, c);
      threadList.push_back(std::move(t));
      i++;
   }
   return 1;
}
