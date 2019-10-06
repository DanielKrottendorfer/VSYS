/* myserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <string>

#include "filehelper.h"

std::string cutOffTillChar(std::string *str, const char r)
{
    /*
    looks for the first occurance of a character,
    cuts the string off at the index
    e.g :

        string s = "Are you gay?"
        string z = cutOffTillChar(&s,' ');
        
        s == "you gay?"
        z == "Are"

    Note the ' ' in between got cut
    */
    std::size_t pos = str->find(r);
    std::string substr = str->substr(0, pos);
    *str = str->substr(pos + 1, str->length());
    return substr;
}


using namespace std;

int sendMessage(string message, string mailpath)
{

    string sender = cutOffTillChar(&message, '\n');
    string reciever = cutOffTillChar(&message, '\n');
    string subject = cutOffTillChar(&message, '\n');

    if (doesDirectoryExist((mailpath + reciever).c_str()) != 0)
    {
        mkdir((mailpath + reciever).c_str(), 0700);
    }

    reciever.append(1, '/');

    FILE *file = fopen((mailpath + reciever + subject).c_str(), "w");

    if (file == nullptr)
    {
        printf("ERROR ON FOPEN\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", ("Von " + sender + ",\n\n" + message).c_str());
    fclose(file);

    return EXIT_SUCCESS;
}

int listMessages(string message, string mailpath, ClientSocket socket)
{

    string user = message;
    string answer;

    if (doesDirectoryExist((mailpath + user).c_str()) != 0)
    {
        socket.sendMessage("0");
    }
    else
    {

        DIR *d;
        struct dirent *dir;
        d = opendir((mailpath + user).c_str());

        int i = 0;

        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.')
                {
                    i++;
                    answer += to_string(i) + " ";
                    answer += dir->d_name;
                    answer += '\n';
                }
            }
            closedir(d);
        }
        socket.sendMessage(answer);
    }
    return EXIT_SUCCESS;
}
int readMessage(string message, string mailpath, ClientSocket socket)
{

    string user = cutOffTillChar(&message, '\n');
    string answer;
    string sn = cutOffTillChar(&message, '\n');
    int subjectNumber = -1;

    subjectNumber = stoi(sn);

    if (doesDirectoryExist((mailpath + user).c_str()) != 0)
    {
        answer = "User does not exist! \n";
        printf("!!doesDirectoryExist\n");
    }
    else
    {

        DIR *d;
        struct dirent *dir;
        string dirName;
        d = opendir((mailpath + user).c_str());

        int i = 0;

        if (d)
        {
            while ((dir = readdir(d)) != NULL && subjectNumber > i)
            {
                if (dir->d_name[0] != '.')
                {
                    i++;
                    if (i == subjectNumber)
                        break;
                }
            }
            dirName = dir->d_name;
            closedir(d);
        }

        user.append(1, '/');
        printf("app %s\n", (mailpath + user + dirName).c_str());
        FILE *fp = fopen((mailpath + user + dirName).c_str(), "r"); // read mode

        if (fp == NULL)
        {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }

        char ch;
        while ((ch = fgetc(fp)) != EOF)
        {
            answer += ch;
        }
        fclose(fp);
    }

    socket.sendMessage(answer);

    return EXIT_SUCCESS;
}
int deleteMessage(string message, string mailpath, ClientSocket socket)
{
    string user = cutOffTillChar(&message,'\n');
    string subject = message;

    if(remove( (mailpath+'/'+user+'/'+subject).c_str() ) != 0)
        socket.sendMessage("Something went wrong !");
    else
        socket.sendMessage("ok");
    

    return EXIT_SUCCESS;
}

int handleMessage(string message, string mailpath, ClientSocket socket)
{

    int requestType = cutOffTillChar(&message, '\n')[0] - '0';

    /*
    0 => SEND
    1 => LIST
    2 => READ
    3 => DEL
    4 => QUIT
    */

    switch (requestType)
    {
    case 0:
        sendMessage(message, mailpath);
        break;
    case 1:
        listMessages(message, mailpath, socket);
        break;
    case 2:
        readMessage(message, mailpath, socket);
        break;
    case 3:
        deleteMessage(message, mailpath, socket);
        break;
    }

    return EXIT_SUCCESS;
}
