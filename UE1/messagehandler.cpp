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

#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>

using namespace std;

#include "messagehandler.hpp"

#include "filehelper.h"

#include "utils.cpp"

int saveMessage(string message, string mailpath)
{
    string sender = cutOffTillStr(&message, "\n");
    string reciever = cutOffTillStr(&message, "\n");
    string topic = cutOffTillStr(&message, "\n");

    string path = (mailpath + "/" + reciever);
    if (doesDirectoryExist(path.c_str()) != 0)
    {
        mkdir(path.c_str(), 0700);
        printf("make directory %s\n", path.c_str());
    }

    int hash = getThreeDigitHash(topic);
    while (!findDirContentThatStartsWith(path, to_string(hash)).empty())
    {
        hash++;
    }

    FILE *fp;
    fp = fopen((mailpath + "/" + reciever + "/" + to_string(hash) + " " + topic).c_str(), "w+");
    if (fp==NULL) {fputs ("File error",stderr); exit (1);}

    fprintf(fp, "%s", ("Von " + sender + "\n\n").c_str());
    fprintf(fp, "%s", message.c_str());

    fclose(fp);

    return EXIT_SUCCESS;
}

int listMessages(string message, string mailpath, ClientSocket *socket)
{
    string user = message;
    string answer;

    if (doesDirectoryExist((mailpath + "/" + user).c_str()) != 0)
    {
        socket->sendMessage("0");
        return EXIT_FAILURE;
    }
    else
    {
        DIR *d;
        struct dirent *dir;
        d = opendir((mailpath + "/" + user).c_str());
        if (d != NULL)
        {
            int i = 0;
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.')
                {
                    i++;
                    answer += dir->d_name;
                    answer += '\n';
                }
            }
            closedir(d);
            socket->sendMessage(to_string(i) + "\n" + answer);
        }
        return EXIT_SUCCESS;
    }
}

int readMessage(string message, string mailpath, ClientSocket *socket)
{

    string user = cutOffTillStr(&message, "\n");
    string id = cutOffTillStr(&message, "\n");

    string path = (mailpath + "/" + user);

    string temp = findDirContentThatStartsWith(path, id);

    if (temp.empty())
    {
        socket->sendMessage("ERR");
    }
    else
    {
        //test
	FILE *pFile;
        long lSize;
        char *buffer;
        size_t result;

        pFile = fopen((mailpath + "/" + user + "/" + temp).c_str(), "r");
        if (pFile == NULL)
        {
            fputs("File error", stderr);
            exit(1);
        }

        // obtain file size:
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);

        // allocate memory to contain the whole file:
        buffer = (char *)malloc(sizeof(char) * lSize);
        if (buffer == NULL)
        {
            fputs("Memory error", stderr);
            exit(2);
        }

        // copy the file into the buffer:
        result = fread(buffer, 1, lSize, pFile);
        if (result != lSize)
        {
            fputs("Reading error", stderr);
            exit(3);
        }

        /* the whole file is now loaded in the memory buffer. */

        // terminate
        fclose(pFile);
        socket->sendMessage(string(buffer));
        free(buffer);
    }
    return EXIT_SUCCESS;
}

int deleteMessage(string answer, string mailpath, ClientSocket *socket)
{
    string user = cutOffTillStr(&answer, "\n");
    string id = cutOffTillStr(&answer, "\n");

    string path = (mailpath + "/" + user);

    if (doesDirectoryExist(path.c_str()) != 0)
    {
        socket->sendMessage("ERR");
        return EXIT_FAILURE;
    }
    else
    {
        string temp = findDirContentThatStartsWith(path, id);
        if (temp.empty())
        {
            socket->sendMessage("ERR");
        }
        else
        {
            if (remove((mailpath + '/' + user + '/' + temp).c_str()) != 0)
            {
                socket->sendMessage("Something went wrong");
            }
            else
            {
                socket->sendMessage("Ok");
            }
        }
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}

int handleMessage(string answer, string mailpath, ClientSocket *socket)
{

    int requestType = cutOffTillStr(&answer, "\n")[0] - '0';

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
        return saveMessage(answer, mailpath);
        break;
    case 1:
        return listMessages(answer, mailpath, socket);
        break;
    case 2:
        return readMessage(answer, mailpath, socket);
        break;
    case 3:
        return deleteMessage(answer, mailpath, socket);
        break;
    }

    return EXIT_FAILURE;
}
