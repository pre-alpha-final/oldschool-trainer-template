#include "exy_send.h"
#include "exy_find.h"
#include "exy_strutils.h"
#include "math.h"
#include "./zlib/zlib.h"

/*
info in header file
*/

int exysend(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl)
{
 char odbior[1024];
 string tmpsend;
 string tmprecv;

 if (*client == 0)
 {
  *client = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;

  if (!ip.empty())
  {
   struct in_addr addr;
   addr.s_addr = inet_addr(ip.c_str());
   memset(&sin, 0, sizeof sin);
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = addr.s_addr;
   sin.sin_port = htons(port);
  }else
  {
   struct hostent *hstnt;
   hstnt=gethostbyname(host.c_str());
   memset(&sin, 0, sizeof sin);
   sin.sin_family = AF_INET;
   if (hstnt != NULL)
    sin.sin_addr.s_addr = ((struct in_addr *)(hstnt->h_addr))->s_addr;
   else
   {
    closesocket(*client);
    return 1;
   }
   sin.sin_port = htons(port);
  }

  //deal with the windows' fucked up no timeout for connect
  if (!exyconnect(*client, &sin, timeout))
  {
   closesocket(*client);
   return 2;
  }

  //anti freeze on send, recv
  setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD));
  setsockopt(*client, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(DWORD));
 }
   
 sfl(&tmpsend, sendl);
 
 if (send(*client, tmpsend.c_str(), tmpsend.length(), 0)==SOCKET_ERROR)
 {
  closesocket(*client);
  return 3;
 }
 int i=1;
 tmprecv.clear();
 while (i > 0)
 {
  memset(odbior, 0, sizeof(odbior));
  i = recv(*client, odbior, sizeof(odbior)-1, 0);
  tmprecv += odbior;
 }

 if (tmprecv.length() < 10)
 {
  closesocket(*client);
  return 3;
 }

 lfs(recvl, &tmprecv);

// closesocket(*client);
 return 0;
}

 
int exysendb(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl)
{
 char odbior[1024];
 string tmpsend;

 if (*client == 0)
 {
  *client = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;

  if (!ip.empty())
  {
   struct in_addr addr;
   addr.s_addr = inet_addr(ip.c_str());
   memset(&sin, 0, sizeof sin);
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = addr.s_addr;
   sin.sin_port = htons(port);
  }else
  {
   struct hostent *hstnt;
   hstnt=gethostbyname(host.c_str());
   memset(&sin, 0, sizeof sin);
   sin.sin_family = AF_INET;
   if (hstnt != NULL)
    sin.sin_addr.s_addr = ((struct in_addr *)(hstnt->h_addr))->s_addr;
   else
   {
    closesocket(*client);
    return 1;
   }
   sin.sin_port = htons(port);
  }

  //deal with the windows' fucked up no timeout for connect
  if (!exyconnect(*client, &sin, timeout))
  {
   closesocket(*client);
   return 2;
  }
 }
 
 //anti freeze on send, recv
 setsockopt(*client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD));
 setsockopt(*client, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(DWORD));
  
 sfl(&tmpsend, sendl);
 
 if (send(*client, tmpsend.c_str(), tmpsend.length(), 0)==SOCKET_ERROR)
 {
  closesocket(*client);
  return 3;
 }
 int i=1;

 int max = 500000;
 byte *recvb = new byte[max];
 byte *recvb2 = new byte[max];
 memset(recvb, 0, max);
 int size = 0;

 while (i > 0)
 {
  memset(odbior, 0, sizeof(odbior));
  i = recv(*client, odbior, sizeof(odbior)-1, 0);
  if (i <= 0)
   break;
  memcpy((char*)&recvb[size], odbior, i);
  size += i;
 } 

 string tmprecv;
 tmprecv = (char*)recvb;

 if (tmprecv.length() < 10)
 {
  closesocket(*client);
  delete [] recvb;
  delete [] recvb2;
  return 3;
 }

//find data start
 int datafrom;
 exyfind(tmprecv, 0, 0x0d, 0x0a, 0x0d, 0x0a, 0, 0, 0, 0x0d, &datafrom);
 datafrom+=4;
 memset(recvb2, 0, max);
 memcpy(recvb2, &recvb[datafrom], size-datafrom);
 memset(recvb, 0, max);
 memcpy(recvb, recvb2, size-datafrom);
 size -= datafrom;
 datafrom=0;

//---------------------------
//data in recvb, size in size
//---------------------------
 
//transfer encoding chunked
 int tecsize = 0;
 int fronttrash; //2000 0x0d 0x0a
 int reartrash;  //0x0d 0x0a
 if (tmprecv.find("Transfer-encoding: chunked") != string::npos ||
     tmprecv.find("Transfer-Encoding: chunked") != string::npos)
 {
  int chunk = 1;
  char chunkc[10];
  memset(recvb, 0, max);
  while (chunk)
  {
   memset(chunkc, 0, 10);
   for (int i=0, k=0; i<9; i++)
   {
    if (recvb2[datafrom+i] == 0x0a)
     break;
    chunkc[k]=recvb2[datafrom+i];
    k++;
   }
   chunk = h2d(chunkc);
   if (!chunk)
    break;
   fronttrash = strlen(chunkc)-1+2;
   reartrash = 2;
   memcpy(&recvb[tecsize], &recvb2[datafrom+fronttrash], chunk);
   tecsize += chunk;
   datafrom+=fronttrash+chunk+reartrash;
  }
  size = tecsize;
 }
 
//---------------------------
//data in recvb, size in size
//---------------------------

//gzip encoding
 if (tmprecv.find("Content-encoding: gzip") != string::npos ||
     tmprecv.find("Content-Encoding: gzip") != string::npos)
 {
  FILE *locked = NULL;
  do
  {
   if (locked != NULL)
    fclose(locked);    
   locked = fopen("locked_exysend_gz", "r");
  }
  while (locked);
  FILE *comp;
  comp = fopen("tmp_exysend_gz", "wb");
  fwrite(recvb, size, 1, comp);
  fclose(comp);

  gzFile uncomp;
  uncomp = gzopen("tmp_exysend_gz", "rb");
  memset(recvb, 0, max);
  size = gzread(uncomp, recvb, max);
  gzclose(uncomp);
  
  DeleteFile("tmp_exysend_gz");
  DeleteFile("locked_exysend_gz");
 }
//---------------------------
//data in recvb, size in size
//---------------------------

 tmprecv = (char*)recvb;
 lfs(recvl, &tmprecv);
 
// closesocket(*client);
 delete [] recvb;
 delete [] recvb2;
 return 0;
}

int exysend(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl)
{
 SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
 struct sockaddr_in sin;
 char odbior[1024];
 string tmpsend;
 string tmprecv;

 if (!ip.empty())
 {
  struct in_addr addr;
  addr.s_addr = inet_addr(ip.c_str());
  memset(&sin, 0, sizeof sin);
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = addr.s_addr;
  sin.sin_port = htons(port);
 }else
 {
  struct hostent *hstnt;
  hstnt=gethostbyname(host.c_str());
  memset(&sin, 0, sizeof sin);
  sin.sin_family = AF_INET;
  if (hstnt != NULL)
   sin.sin_addr.s_addr = ((struct in_addr *)(hstnt->h_addr))->s_addr;
  else
  {
   closesocket(client);
   return 1;
  }
  sin.sin_port = htons(port);
 }

 //deal with the windows' fucked up no timeout for connect
 if (!exyconnect(client, &sin, timeout))
 {
  closesocket(client);
  return 2;
 }

 //anti freeze on send, recv
 setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD));
 setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(DWORD));
  
 sfl(&tmpsend, sendl);
 
 if (send(client, tmpsend.c_str(), tmpsend.length(), 0)==SOCKET_ERROR)
 {
  closesocket(client);
  return 3;
 }
 int i=1;
 tmprecv.clear();
 while (i > 0)
 {
  memset(odbior, 0, sizeof(odbior));
  i = recv(client, odbior, sizeof(odbior)-1, 0);
  tmprecv += odbior;
 }

 if (tmprecv.length() < 10)
 {
  closesocket(client);
  return 3;
 }

 lfs(recvl, &tmprecv);

// closesocket(client);
 return 0;
}

 
int exysendb(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl)
{
 SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
 struct sockaddr_in sin;
 char odbior[1024];
 string tmpsend;

 if (!ip.empty())
 {
  struct in_addr addr;
  addr.s_addr = inet_addr(ip.c_str());
  memset(&sin, 0, sizeof sin);
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = addr.s_addr;
  sin.sin_port = htons(port);
 }else
 {
  struct hostent *hstnt;
  hstnt=gethostbyname(host.c_str());
  memset(&sin, 0, sizeof sin);
  sin.sin_family = AF_INET;
  if (hstnt != NULL)
   sin.sin_addr.s_addr = ((struct in_addr *)(hstnt->h_addr))->s_addr;
  else
  {
   closesocket(client);
   return 1;
  }
  sin.sin_port = htons(port);
 }

 //deal with the windows' fucked up no timeout for connect
 if (!exyconnect(client, &sin, timeout))
 {
  closesocket(client);
  return 2;
 }

 //anti freeze on send, recv
 setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD));
 setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(DWORD));
  
 sfl(&tmpsend, sendl);
 
 if (send(client, tmpsend.c_str(), tmpsend.length(), 0)==SOCKET_ERROR)
 {
  closesocket(client);
  return 3;
 }
 int i=1;

 int max = 500000;
 byte *recvb = new byte[max];
 byte *recvb2 = new byte[max];
 memset(recvb, 0, max);
 int size = 0;

 while (i > 0)
 {
  memset(odbior, 0, sizeof(odbior));
  i = recv(client, odbior, sizeof(odbior)-1, 0);
  if (i <= 0)
   break;
  memcpy((char*)&recvb[size], odbior, i);
  size += i;
 } 

 string tmprecv;
 tmprecv = (char*)recvb;

 if (tmprecv.length() < 10)
 {
  closesocket(client);
  delete [] recvb;
  delete [] recvb2;
  return 3;
 }

//find data start
 int datafrom;
 exyfind(tmprecv, 0, 0x0d, 0x0a, 0x0d, 0x0a, 0, 0, 0, 0x0d, &datafrom);
 datafrom+=4;
 memset(recvb2, 0, max);
 memcpy(recvb2, &recvb[datafrom], size-datafrom);
 memset(recvb, 0, max);
 memcpy(recvb, recvb2, size-datafrom);
 size -= datafrom;
 datafrom=0;

//---------------------------
//data in recvb, size in size
//---------------------------
 
//transfer encoding chunked
 int tecsize = 0;
 int fronttrash; //2000 0x0d 0x0a
 int reartrash;  //0x0d 0x0a
 if (tmprecv.find("Transfer-encoding: chunked") != string::npos ||
     tmprecv.find("Transfer-Encoding: chunked") != string::npos)
 {
  int chunk = 1;
  char chunkc[10];
  memset(recvb, 0, max);
  while (chunk)
  {
   memset(chunkc, 0, 10);
   for (int i=0, k=0; i<9; i++)
   {
    if (recvb2[datafrom+i] == 0x0a)
     break;
    chunkc[k]=recvb2[datafrom+i];
    k++;
   }
   chunk = h2d(chunkc);
   if (!chunk)
    break;
   fronttrash = strlen(chunkc)-1+2;
   reartrash = 2;
   memcpy(&recvb[tecsize], &recvb2[datafrom+fronttrash], chunk);
   tecsize += chunk;
   datafrom+=fronttrash+chunk+reartrash;
  }
  size = tecsize;
 }
 
//---------------------------
//data in recvb, size in size
//---------------------------

//gzip encoding
 if (tmprecv.find("Content-encoding: gzip") != string::npos ||
     tmprecv.find("Content-Encoding: gzip") != string::npos)
 {
  FILE *locked = NULL;
  do
  {
   if (locked != NULL)
    fclose(locked);    
   locked = fopen("locked_exysend_gz", "r");
  }
  while (locked);
  FILE *comp;
  comp = fopen("tmp_exysend_gz", "wb");
  fwrite(recvb, size, 1, comp);
  fclose(comp);

  gzFile uncomp;
  uncomp = gzopen("tmp_exysend_gz", "rb");
  memset(recvb, 0, max);
  size = gzread(uncomp, recvb, max);
  gzclose(uncomp);
  
  DeleteFile("tmp_exysend_gz");
  DeleteFile("locked_exysend_gz");
 }
//---------------------------
//data in recvb, size in size
//---------------------------

 tmprecv = (char*)recvb;
 lfs(recvl, &tmprecv);
 
// closesocket(client);
 delete [] recvb;
 delete [] recvb2;
 return 0;
}

bool exyconnect(SOCKET client, sockaddr_in *sin, DWORD dwTimeout)
{
 HANDLE hThread;
 DWORD dwThreadID;
 exyconnectdata *ecd = new exyconnectdata();
 ecd->client = client;
 ecd->sin = sin;
 ecd->state = 0;

 hThread = CreateThread(NULL, 0, exyconnect_t, ecd, 0, &dwThreadID);

 // wait for connection
 if (WaitForSingleObject(hThread, dwTimeout) == WAIT_TIMEOUT)
 {
  TerminateThread(hThread, 1);
  delete ecd;
  return 0; //failed
 }
 if (ecd->state)
 {
  delete ecd;
  return 1; //ok
 }
 else
 {
  delete ecd;
  return 0; //failed
 }
}

DWORD WINAPI exyconnect_t(IN LPVOID vThreadParm)
{
 exyconnectdata *ecd;
 ecd = (exyconnectdata*)vThreadParm;
 if (!connect(ecd->client, (struct sockaddr *)ecd->sin, sizeof(struct sockaddr_in)))
  ecd->state = 1;
 return 0;
}

int h2d(char *hex)
{
 int dec = 0;
 char hexchar;
 for (int i=strlen(hex)-1-1, j=0; i>=0; i--, j++)
 {
  hexchar = hex[i];
  switch (hexchar)
  {
   case '0': dec+=0*(int)pow(16,j); break;
   case '1': dec+=1*(int)pow(16,j); break;
   case '2': dec+=2*(int)pow(16,j); break;
   case '3': dec+=3*(int)pow(16,j); break;
   case '4': dec+=4*(int)pow(16,j); break;
   case '5': dec+=5*(int)pow(16,j); break;
   case '6': dec+=6*(int)pow(16,j); break;
   case '7': dec+=7*(int)pow(16,j); break;
   case '8': dec+=8*(int)pow(16,j); break;
   case '9': dec+=9*(int)pow(16,j); break;
   case 'A': dec+=10*(int)pow(16,j); break;
   case 'B': dec+=11*(int)pow(16,j); break;
   case 'C': dec+=12*(int)pow(16,j); break;
   case 'D': dec+=13*(int)pow(16,j); break;
   case 'E': dec+=14*(int)pow(16,j); break;
   case 'F': dec+=15*(int)pow(16,j); break;
   case 'a': dec+=10*(int)pow(16,j); break;
   case 'b': dec+=11*(int)pow(16,j); break;
   case 'c': dec+=12*(int)pow(16,j); break;
   case 'd': dec+=13*(int)pow(16,j); break;
   case 'e': dec+=14*(int)pow(16,j); break;
   case 'f': dec+=15*(int)pow(16,j); break;
  }
 }
 return dec;
}

string gethostfromaddr(string s)
{
 string s2;
 if (s.find("://") != string::npos)
 {
  s2 = exyfind(s, 0, ':', '/', '/', 0, 3, 0, 0, '/', 0);
  if (s2.find("www.") != string::npos)
  {
   s = exyfind(s2, 0, 'w', 'w', 'w', '.', 4, 0, 0, '/', 0);
   s2 = s;
  }
 }
 else
 {
  if (s.find("/") != string::npos)
  {
   s.erase(s.find("/"), 999);
   s2 = s;
   if (s2.find("www.") != string::npos)
   {
    s = exyfind(s2, 0, 'w', 'w', 'w', '.', 4, 0, 0, '/', 0);
    s2 = s;
   }
  }
 }
 return s2;
}

string getrestfromaddr(string s)
{
 string s2;
 if (s.find("://") != string::npos)
 {
  s2 = exyfind(s, 9, '/', 0, 0, 0, 0, 0, 0, 0, 0);
 }
 else
 {
  if (s.find("/") != string::npos)
  {
   s.erase(0, s.find("/"));
   s2 = s;
  }
 }       
 return s2;
}
