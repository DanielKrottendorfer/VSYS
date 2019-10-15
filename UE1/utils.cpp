
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>

using namespace std;

void trimStirng(string& s)
{
    s.erase(s.find_last_not_of(" \n\r\t")+1);
}

std::hash<std::string> hash_fn;

int absolut(int a)
{
    if (a < 0)
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
    int hash = absolut(hash_fn(s));
    hash %= 1000;
    return hash;
}

string findDirContentThatStartsWith(string path, string s)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path.c_str());
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] != '.')
            {
                string temp(dir->d_name);
                if (temp.find(s) == 0)
                {
                    closedir(d);
                    return temp;
                }
            }
        }
    }
    closedir(d);
    return "";
}

std::string cutOffTillStr(std::string *str, string r)
{
    /*
    looks for the first occurance of a character,
    cuts the string off at the index
    e.g :

        string s = "How are you?"
        string z = cutOffTillStr(&s," ");
        
        s == "are you?"
        z == "How"

        Note the ' ' in between got cut
    */
    size_t pos = str->find(r);
    if(pos == string::npos)
        return "";
    string substr = str->substr(0, pos);
    *str = str->substr(pos + 1, str->length());
    return substr;
}