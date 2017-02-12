#ifndef EXY_STRUTILS_H
#define EXY_STRUTILS_H

#include <string>
#include <list>
#include <cstdio>

using namespace std;

void sfl(string*, list<string>*);
void lfs(list<string>*, string*);
string exyload(char*, list<string>*);
void exysave(char*, string);
void exyerase(string*, char);
int strinstr(string, string);

#endif
