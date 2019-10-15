#include "ClientSocket.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <iostream>
#define GOODEIGHTIES 82
#define USERNAMEHEADER 10
#define TEXTMAILSS 927
using namespace std;

void trimStirng(string& s)
{
    s.erase(s.find_last_not_of(" \n\r\t")+1);
}

void signal_handler(int sig) {
	if(sig == SIGSEGV)
	{
		prinft("Port is Missing, Usage: ServerAdresse:Port\n");
		exit(EXIT_FAILURE);
	}
	
}


int main (int argc, char **argv) {
        if( argc < 2 )
        {
          printf("Usage: %s ServerAdresse:Port\n", argv[0]);
          exit(EXIT_FAILURE);
        }
		
		(void) signal(SIGSEGV, signal_handler);
		
        string ipAddress = strtok (argv[1], ":");
        int port = atoi(strtok (NULL, ":"));
        string message;
        ClientSocket cs = ClientSocket::connectToSocket(ipAddress,port);
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());
        char messageType[3] = {'w'};

        while (messageType[0] != 'Q' && messageType[0] != 'q')
        {
           printf ("What do you want to do?\n[S]end\n[L]ist\n[R]ead\n[D]elete\n[Q]uit\n");
           fgets(messageType, sizeof(messageType), stdin);
           string buffer;
           if(messageType[0] == 'S' || messageType[0] == 's')
           {
             printf ("Your Username?\n");
             char senderr[USERNAMEHEADER];
             fgets (senderr, USERNAMEHEADER, stdin);
             printf ("To whom should this mail go to?\n");
             char empfaenger[USERNAMEHEADER];
             fgets (empfaenger, USERNAMEHEADER, stdin);
             printf ("What is the header of the mail?\n");
             char betreff[GOODEIGHTIES];
             fgets (betreff, GOODEIGHTIES, stdin);
             printf ("Write the big Message you got mate\n");
             char textMail[TEXTMAILSS];
             fgets (textMail, TEXTMAILSS, stdin);

             string senderString(senderr);
             string empfaengerString(empfaenger);
             string betreffString(betreff);
             string textMailString(textMail);

             string messageType2(messageType);

             buffer = "0\n" + senderString + empfaengerString + betreffString + textMailString;
             //char buffer2[BUF] = "0if18b159\nhalt die Fresse\nichbringdiechum\0";
             //send(create_socket, buffer2, strlen (buffer2), 0);
             trimStirng(buffer);
             cs.sendMessage(buffer);
             printf("%s\n",buffer.c_str());
         }
         if(messageType[0] == 'L' || messageType[0] == 'l')
         {
           printf("What's your username?\n");
           char bufferChar[USERNAMEHEADER];
           fgets (bufferChar, USERNAMEHEADER, stdin);
           buffer = bufferChar;
           buffer = "1\n" + buffer;
           trimStirng(buffer);
           cs.sendMessage(buffer);
           cs.recieveMessage(message);
           printf("%s\n",message.c_str());
         }
         if(messageType[0] == 'R' || messageType[0] == 'r')
         {
           printf("What's your username?\n");
           char bufferChar[USERNAMEHEADER];
           fgets (bufferChar, USERNAMEHEADER, stdin);
           buffer = bufferChar;
           printf("Which mail would you like to read.\n");
           char index[USERNAMEHEADER];
           fgets (index, USERNAMEHEADER, stdin);
           buffer = "2\n" + buffer + index;
           trimStirng(buffer);
           cs.sendMessage(buffer);
           cs.recieveMessage(message);
           printf("%s\n\n",message.c_str());
         }
         if(messageType[0] == 'D' || messageType[0] == 'd')
         {
           printf("What's your username?\n");
           char bufferChar[USERNAMEHEADER];
           fgets (bufferChar, USERNAMEHEADER, stdin);
           buffer = bufferChar;
           printf("Which mail would you like to delete.\n");
           char index[USERNAMEHEADER];
           fgets (index, USERNAMEHEADER, stdin);
           buffer = "3\n" + buffer + index;
           trimStirng(buffer);
           cs.sendMessage(buffer);
           cs.recieveMessage(message);
           printf("%s\n",message.c_str());
         }
      }
      return 0;
      /*
        cs.sendMessage("0\nif18b030\nMarius\nAre you gay?\nMeele ist ein verbuggtes Drecksgame");
        sleep(1);

        cs.sendMessage("1\nMarius\n");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());

        cs.sendMessage("2\nMarius\n292");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());

        cs.sendMessage("3\nMarius\n292");
        cs.recieveMessage(message);
        printf("%s\n",message.c_str());
        */

}
