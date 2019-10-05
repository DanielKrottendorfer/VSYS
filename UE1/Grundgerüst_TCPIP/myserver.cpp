/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUF 1024
#define PORT 6543

#include <string>
using namespace std;

#include "filehelper.h"
#include "sockethelper.h"

int main (int argc, char **argv) {


   if( argc < 2 ){
      printf("Usage: %s Mailpoolpath\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   string dir(argv[1]);
   if(doesDirectoryExist(dir.c_str()) != 0){
      exit(EXIT_FAILURE);
   }
   dir.append(1,'/');

   int server_socket;
   struct sockaddr_in address;
   if(initServersocket(&server_socket,&address,PORT) != 0){
      printf("Failed to initialice Serversocket\n");
      exit(EXIT_FAILURE);
   }


   socklen_t addrlen;
   addrlen = sizeof (struct sockaddr_in);
   struct sockaddr_in cliaddress;
   int client_socket;

   int message_size;
   char buffer[BUF];

   while (1) {
      printf("Waiting for connections...\n");
      client_socket = accept ( server_socket, (struct sockaddr *) &cliaddress, &addrlen );
      if (client_socket > 0)
      {
         printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
         strcpy(buffer,"Welcome to myserver, Please enter your command:\n");
         send(client_socket, buffer, strlen(buffer),0);
      }

      do {
         message_size = recv (client_socket, buffer, BUF-1, 0);
         if( message_size > 0)
         {
            buffer[message_size] = '\0';
            string message(buffer);
            handleMessage(message, dir, client_socket);
         }
         else if (message_size == 0)
         {
            printf("Client closed remote socket\n");
            break;
         }
         else
         {
            perror("recv error");
            return EXIT_FAILURE;
         }
      } while (strncmp (buffer, "quit", 4)  != 0);
      close (client_socket);
   }
   close (server_socket);

   return EXIT_SUCCESS;
}
