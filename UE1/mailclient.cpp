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

#include "utils.cpp"

void signal_handler(int sig)
{
  if (sig == SIGSEGV)
  {
    printf("Port is Missing, Usage: ServerAdresse:Port\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: %s ServerAdresse:Port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  (void)signal(SIGSEGV, signal_handler);

  string input(argv[1]);

  string ipAddress = cutOffTillStr(&input,":");
  if(ipAddress.empty())
  {
    printf("Usage: %s ServerAdresse:Port\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int port = -1;
  port = stoi(input);

  if(port<0)
  {
    printf("bad port \n");
    exit(EXIT_FAILURE);
  }

  //string ipAddress = strtok(argv[1], ":");
  //int port = atoi(strtok(NULL, ":"));
  string message;
  ClientSocket cs = ClientSocket::connectToSocket(ipAddress, port);
  cs.recieveMessageWait(message);
  printf("%s\n", message.c_str());

  string uid;
  string pw;

  do{
    cout << "UID: ";
    cin >> uid;
    cout << "PW : "; 
    cin >> pw;

    cs.sendMessage( (uid+"\n"+pw) );
    cs.recieveMessageWait(message);
    cout << message << endl;
  }while(message.find("Failure") != string::npos);

  if(message.find("Success") == string::npos)
    return EXIT_SUCCESS;

  uid = uid+"\n";

  char messageType[3] = {'w'};

  while (messageType[0] != 'Q' && messageType[0] != 'q')
  {
    printf("What do you want to do?\n[S]end\n[L]ist\n[R]ead\n[D]elete\n[Q]uit\n");
    fgets(messageType, sizeof(messageType), stdin);
    string buffer;
    if (messageType[0] == 'S' || messageType[0] == 's')
    {
      printf("To whom should this mail go to?\n");
      char empfaenger[USERNAMEHEADER];
      fgets(empfaenger, USERNAMEHEADER, stdin);
      printf("What is the header of the mail?\n");
      char betreff[GOODEIGHTIES];
      fgets(betreff, GOODEIGHTIES, stdin);
      printf("Write the big Message you got mate\n");

      string textMailString = "";

      do{
        char textMail[TEXTMAILSS];
        fgets(textMail, TEXTMAILSS, stdin);
        textMailString = textMailString + string(textMail);
      }while ( textMailString.find(".\n") != textMailString.length()-2 );

      string empfaengerString(empfaenger);
      string betreffString(betreff);

      string messageType2(messageType);

      buffer = "0\n" + uid + empfaengerString + betreffString + textMailString;
      //char buffer2[BUF] = "0if18b159\nhalt die Fresse\nichbringdiechum\0";
      //send(create_socket, buffer2, strlen (buffer2), 0);
      trimStirng(buffer);
      cs.sendMessage(buffer);
      printf("%s\n", buffer.c_str());
    }
    else if (messageType[0] == 'L' || messageType[0] == 'l')
    {
      buffer = "1\n" + uid;
      trimStirng(buffer);
      cs.sendMessage(buffer);
      cs.recieveMessageWait(message);
      printf("%s\n", message.c_str());
    }
    else if (messageType[0] == 'R' || messageType[0] == 'r')
    {
      buffer = uid;
      printf("Which mail would you like to read.\n");
      char index[USERNAMEHEADER];
      fgets(index, USERNAMEHEADER, stdin);
      buffer = "2\n" + buffer + index;
      trimStirng(buffer);
      cs.sendMessage(buffer);
      cs.recieveMessageWait(message);
      printf("%s\n\n", message.c_str());
    }
    else if (messageType[0] == 'D' || messageType[0] == 'd')
    {
      buffer = uid;
      printf("Which mail would you like to delete.\n");
      char index[USERNAMEHEADER];
      fgets(index, USERNAMEHEADER, stdin);
      buffer = "3\n" + buffer + index;
      trimStirng(buffer);
      cs.sendMessage(buffer);
      cs.recieveMessageWait(message);
      printf("%s\n", message.c_str());
    }
  }
  return 0;
}
