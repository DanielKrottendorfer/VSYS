

#ifndef UTILS
#define UTILS

#include <string>
using namespace std;

void trimStirng(string &s);
int getThreeDigitHash(string s);
string findDirContentThatStartsWith(string path, string s);
string cutOffTillStr(string *str, string r);

#endif