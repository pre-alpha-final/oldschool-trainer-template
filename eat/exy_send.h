/*
Module for sending and reciving data via winsock.

Remarks:
1. If ip is "", then host is used

2. To use this first init winsocks:
 int err;
 WORD ver;
 WSADATA wsaData;
 ver = MAKEWORD(2,0);
 err = WSAStartup(ver, &wsaData);
 if (err != 0)
 {
  ShowMessage("WSA error");
  WSACleanup();
 }
 if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
 {
  ShowMessage("Wrong winsocket version");
  WSACleanup();
 }

3. Return values
0 ok
1 couldn't resolve host
2 connection error
3 sending error
4 nothing recieved


4. Function remarks:
- int exysend(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
   This one is as basic as it gets. Send some plain text data and returns the
   whole answer from the server. Doesn't support transfer encoding
   nor compression.
   
- int exysendb(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
   This one is more advanced returns only the main data (header is cut out)
   after previously unzipping and "unchunking" it.

- int exysend(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
  int exysendb(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
   These two work the same way as the above accept that if the socket passed to
   them is 0, new socket is opened and after completion the socket isn't closed.
   So use these if you need a whole conversation in one connection. Just don't
   forget to close the socket afterwards.
*/

#ifndef EXY_SEND_H
#define EXY_SEND_H

#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <string>
#include <list>
#include <cstdio>

using namespace std;

int exysend(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
int exysendb(SOCKET *client, string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
int exysend(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
int exysendb(string ip, string host, int port, DWORD timeout,
            list<string> *sendl, list<string> *recvl);
bool exyconnect(SOCKET connect, sockaddr_in *sin, DWORD dwTimeout);            
DWORD WINAPI exyconnect_t(IN LPVOID vThreadParm);
int h2d(char*);
string gethostfromaddr(string);
string getrestfromaddr(string);


class exyconnectdata
{
 public:
  SOCKET client;
  sockaddr_in *sin;
  int state;
};

#endif /* EXY_SEND_H */

