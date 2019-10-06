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


std::hash<std::string> hash_fn;

int absolut(int a){
    if(a<0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

int getThreeDigitHash(string s)
{
    int hash =  absolut(hash_fn(s));
    hash%=1000;
    return hash;
}

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

int saveMessage(string message, string mailpath)
{
    string sender = cutOffTillChar(&message,'\n');
    string reciever = cutOffTillChar(&message,'\n');
    string topic = cutOffTillChar(&message,'\n');

    if( doesDirectoryExist( (mailpath+"/"+reciever).c_str() ) != 0 )
    {
        mkdir((mailpath+"/"+reciever).c_str(), 0700);
    }

    int hash = getThreeDigitHash(topic);

    FILE * fp;
    fp = fopen ( (mailpath+"/"+reciever+"/"+ to_string(hash) + " " + topic).c_str() , "w+");

    fprintf(fp, "%s" , ("Von "+sender+"\n\n").c_str());
    fprintf(fp, "%s" , message.c_str());

    fclose(fp);

    return EXIT_SUCCESS;
}

int listMessages(string message, string mailpath, ClientSocket* socket)
{
    string user = message;
    string answer;

    if ( doesDirectoryExist((mailpath+"/"+user).c_str()) != 0 )
    {
        socket->sendMessage("0");
        return EXIT_FAILURE;
    }
    else
    {
        DIR *d;
        struct dirent *dir;
        d = opendir((mailpath+"/"+user).c_str());
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
            socket->sendMessage(to_string(i)+"\n"+answer);
        }
        return EXIT_SUCCESS;
    }
}

int readMessage(string message, string mailpath, ClientSocket* socket)
{

    string user = cutOffTillChar(&message,'\n');
    string id = cutOffTillChar(&message,'\n');

    if ( doesDirectoryExist((mailpath+"/"+user).c_str()) != 0 )
    {
        socket->sendMessage("ERR");
        return EXIT_FAILURE;
    }
    else
    {
        DIR *d;
        struct dirent *dir;
        d = opendir((mailpath+"/"+user).c_str());
        if (d != NULL)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.')
                {
                    string temp(dir->d_name);
                    if(temp.find(id) == 0){

                        FILE* f = fopen( (mailpath+"/"+user+"/"+temp).c_str(), "r" );
                        if (f==NULL) {fputs ("File error",stderr); exit (1);}

                        fseek (f , 0 , SEEK_END);
                        long lSize = ftell (f);
                        rewind(f);

                        char* buffer = (char*) malloc (sizeof(char)*lSize);
                        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

                        int result = fread (buffer,1,lSize,f);
                        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

                        socket->sendMessage(string(buffer));

                        fclose(f);
                        closedir(d);
                        return EXIT_SUCCESS;

                    }
                }
            }
            socket->sendMessage("ERR");
            closedir(d);
        }
        return EXIT_FAILURE;
    }
}
int deleteMessage(string message, string mailpath, ClientSocket* socket)
{
    string user = cutOffTillChar(&message,'\n');
    string id = cutOffTillChar(&message,'\n');

    if ( doesDirectoryExist((mailpath+"/"+user).c_str()) != 0 )
    {
        socket->sendMessage("ERR");
        return EXIT_FAILURE;
    }
    else
    {
        DIR *d;
        struct dirent *dir;
        d = opendir((mailpath+"/"+user).c_str());
        if (d != NULL)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] != '.')
                {
                    string temp(dir->d_name);
                    if(temp.find(id) == 0){
                        string tmep(dir->d_name);
                        if( remove( (mailpath+'/'+user+'/'+temp).c_str() ) != 0 )
                        {
                            socket->sendMessage("Something went wrong");
                        }else {
                            socket->sendMessage("Ok");
                        }
                        closedir(d);
                        return EXIT_SUCCESS;
                    }
                }
            }
            socket->sendMessage("ERR");
            closedir(d);
        }
        return EXIT_FAILURE;
    }
}

int handleMessage(string message, string mailpath, ClientSocket* socket)
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
        saveMessage(message, mailpath);
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
