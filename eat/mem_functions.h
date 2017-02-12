#ifndef MEMFUNCT_H
#define MEMFUNCT_H

#include <windows.h>

void writetm(HANDLE, DWORD, DWORD, int);
DWORD reverse4b(DWORD);
DWORD reverse3b(DWORD);
DWORD reverse2b(DWORD);
DWORD readfm(HANDLE, DWORD, int);
DWORD point(HANDLE, DWORD, int);
BOOL SetPrivilege(HANDLE, LPCTSTR, BOOL);

#endif /* MEMFUNCT_H */
