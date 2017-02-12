/*
Searching for texts in strings

Remarks:
If you don't need all abcd chars 0 the rest
If you don't want to pass any characters 0 pass1 and pass2

*/

#ifndef EXY_FIND_H
#define EXY_FIND_H

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

string exyfind(string text, int start, char a, char b, char c, char d,
               int offset, char pass1, char pass2, char stop, int* index);

#endif /* EXY_FIND_H */

