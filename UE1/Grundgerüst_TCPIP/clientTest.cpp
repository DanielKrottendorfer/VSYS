/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#define BUF 1024
#define PORT 6543

int main (int argc, char **argv) {
  int create_socket;
  char buffer[BUF];
  struct sockaddr_in address;
  int size;


  if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
     perror("Socket error");
     return EXIT_FAILURE;
  }
  
  memset(&address,0,sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons (PORT);
  inet_aton ("127.0.0.1", &address.sin_addr);

  if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
  {
     printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
     size=recv(create_socket,buffer,BUF-1, 0);
     if (size>0)
     {
        buffer[size]= '\0';
        printf("%s",buffer);
     }
  }
  else
  {
     perror("Connect error - no server available");
     return EXIT_FAILURE;
  }

   
   printf ("Send message: \n");
   char buffer1[BUF] =   "0\nif18b030\nMarius\nAre you gay?\nMeele ist ein verbuggtes Drecksgame \0";
   send(create_socket, buffer1, strlen (buffer1), 0);
   sleep(1);

   printf ("List message: \n");
   char buffer2[BUF] =   "1\nMarius\0";
   send(create_socket, buffer2, strlen (buffer2), 0);

   size=recv(create_socket,buffer,BUF-1, 0);
   if (size>0)
   {
      buffer[size]= '\0';
      printf("%s\n",buffer);
   }
   sleep(1);
   
   printf ("Read message: \n");
   char buffer3[BUF] =   "2\nMarius\n0 \0";
   send(create_socket, buffer3, strlen (buffer3), 0);

   size=recv(create_socket,buffer,BUF-1, 0);
   if (size>0)
   {
      buffer[size]= '\0';
      printf("%s\n",buffer);
   }
   sleep(1);
   
   printf ("Read message: \n");
   char buffer4[BUF] =   "3\nMarius\nAre you gay?\0";
   send(create_socket, buffer4, strlen (buffer4), 0);

   size=recv(create_socket,buffer,BUF-1, 0);
   if (size>0)
   {
      buffer[size]= '\0';
      printf("%s\n",buffer);
   }



   
   close (create_socket);
   return EXIT_SUCCESS;
}
