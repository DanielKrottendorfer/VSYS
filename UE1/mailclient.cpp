#include "ClientSocket.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <string>
#include <iostream>
#define HEADERMAIL 82
#define RECEIVER 10
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
void maskpass(string *pwd)
{
    static struct termios oldt, newt;
    // saving the old settings of STDIN_FILENO and copy settings for resetting
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // setting the approriate bit in the termios struct
    newt.c_lflag &= ~(ECHO);
    // setting the new bits
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, *pwd);

    // resetting our old STDIN_FILENO
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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

  string ipAddress = cutOffTillStr(&input, ":");
  if (ipAddress.empty())
  {
    printf("Usage: %s ServerAdresse:Port\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int port = -1;
  port = stoi(input);

  if (port < 0)
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

  if (message.find("banned") != string::npos)
  {
    cs.closeCon();
    return EXIT_SUCCESS;
  }

  string uid;
  string pw;

  do
  {
    cout << "UID: ";
    getline(cin, uid);
    cout << "PW : ";
    maskpass(&pw);
    cout << endl;

    cs.sendMessage((uid + "\n" + pw));
    cs.recieveMessageWait(message);
    cout << message << endl;
  } while (message.find("Failure") != string::npos);

  if (message.find("Success") == string::npos)
    return EXIT_SUCCESS;

  uid = uid + "\n";

  char messageType[3] = {'w'};

  while (messageType[0] != 'Q' && messageType[0] != 'q')
  {
    printf("What would you like to do?\n[S]end\n[L]ist\n[R]ead\n[D]elete\n[Q]uit\n");
    fgets(messageType, sizeof(messageType), stdin);
    string buffer;
    if (messageType[0] == 'S' || messageType[0] == 's')
    {
      printf("Who receives it?\n");
      char empfaenger[RECEIVER];
      fgets(empfaenger, RECEIVER, stdin);
      printf("What is the header of the mail?\n");
      char betreff[HEADERMAIL];
      fgets(betreff, HEADERMAIL, stdin);
      printf("Write the Message.\n");

      string textMailString = "";

      do
      {
        char textMail[TEXTMAILSS];
        fgets(textMail, TEXTMAILSS, stdin);
        textMailString = textMailString + string(textMail);
      } while (textMailString.find("\n.\n") == string::npos);

      string empfaengerString(empfaenger);
      string betreffString(betreff);

      string messageType2(messageType);

      buffer = "0\n" + uid + empfaengerString + betreffString + textMailString;
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
      char index[RECEIVER];
      fgets(index, RECEIVER, stdin);
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
      char index[RECEIVER];
      fgets(index, RECEIVER, stdin);
      buffer = "3\n" + buffer + index;
      trimStirng(buffer);
      cs.sendMessage(buffer);
      cs.recieveMessageWait(message);
      printf("%s\n", message.c_str());
    }
  }
  cs.closeCon();
  return 0;
}
