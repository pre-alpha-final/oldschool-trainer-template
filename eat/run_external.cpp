#include "run_external.h"

HANDLE readdaemonconsolethread;
DWORD WINAPI readdaemonconsolethreadf(LPVOID);

class data
{
 public:
  HANDLE h;
  char *text;
  int size;    
};


void runconsole(char* program, char* args, HANDLE* read)
{
  SECURITY_ATTRIBUTES secattr; 
  ZeroMemory(&secattr,sizeof(secattr));
  secattr.nLength = sizeof(secattr);
  secattr.bInheritHandle = TRUE;

  HANDLE wPipe;

  //Create pipes to write and read data

  CreatePipe(read,&wPipe,&secattr,0);
  //

  STARTUPINFO sInfo; 
  ZeroMemory(&sInfo,sizeof(sInfo));
  PROCESS_INFORMATION pInfo; 
  ZeroMemory(&pInfo,sizeof(pInfo));
  sInfo.cb=sizeof(sInfo);
  sInfo.dwFlags=STARTF_USESTDHANDLES;
  sInfo.hStdInput=NULL; 
  sInfo.hStdOutput=wPipe; 
  sInfo.hStdError=wPipe;

  //Create the process here.
  char tmp[1000];
  sprintf(tmp, "\"%s\" %s", program, args);
  CreateProcess(NULL, tmp, 0, 0, TRUE,
                NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW,0,0,&sInfo,&pInfo);
  CloseHandle(wPipe);
}

void readconsole(HANDLE h, char *text)
{
 //now read the output pipe here.
 char buf[101];
 DWORD dw; 
 BOOL r;
 int i=0;
 do
 {
  memset(buf, 0, 101);
  r=ReadFile(h, buf, 100, &dw, 0);
  strncpy(text+i, buf, dw);
  i += dw;
 }
 while(r);
}

void readdaemonconsole(HANDLE h, char *text, int size)
{
 DWORD dw;
 data *d = new data();
 d->h = h;
 d->text = text;
 d->size = size;
 CloseHandle(readdaemonconsolethread);
 readdaemonconsolethread = CreateThread(NULL, 0, readdaemonconsolethreadf,
                                        (void*)d, 0, &dw);
}

DWORD WINAPI readdaemonconsolethreadf(LPVOID d2)
{
 //now read the output pipe here.
 data *d = (data*)d2;
 char buf[101];
 DWORD dw; 
 BOOL r;
 int i=0;
 int size = d->size;
 char *text = d->text;
 HANDLE h = d->h;
 do
 {
  memset(buf, 0, 101);
  r=ReadFile(h, buf, 100, &dw, 0);

  if (i+dw+1 > size)
  {
   if (size == size/2*2) //even
   {
    strncpy(text, text+size/2, size/2);
    memset(text+size/2, 0, size/2);
    i = size/2;
   }
   else //odd
   {
    strncpy(text, text+size/2, size/2+1);
    memset(text+size/2, 0, size/2+1);
    i = size/2+1;
   }
   while (text[i] == 0x00)
    i--;
  }

  strncpy(text+i, buf, dw);
  i += dw;
 }
 while(r);
}
