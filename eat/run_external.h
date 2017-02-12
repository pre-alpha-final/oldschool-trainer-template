#ifndef RUNEXTERNAL_H
#define RUNEXTERNAL_H

#include <stdio.h>
#include <windows.h>

void runconsole(char* program, char* args, HANDLE* read);             
void readconsole(HANDLE h, char *text);
void readdaemonconsole(HANDLE h, char *text, int size);

#endif /* RUNEXTERNAL_H */
