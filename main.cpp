#include <windows.h>
#include "main.h"
#include "application.h"

#define music_on uFMOD_PlaySong((char*)"music",0,XM_RESOURCE);
#define music_off uFMOD_PlaySong((char*)"",0,XM_RESOURCE);

void CALLBACK what_to_do(HWND, UINT, UINT, DWORD);
void closeappbuttonf();
void closeinfof();
void minimizeappbuttonf();
void showinfof();
void followme();

//windows, buttons and shit
char *programpath;
exywindow *window1, *window2;
exybutton *closeappbutton, *closeinfo, *minimizeappbutton, *showinfo,
          *scrollup, *scrolldown;
exylistbox *nfobox;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
 MSG Message;
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
 DeleteFile("tmp_exysend_gz"); 
 DeleteFile("lock_exysend_gz");
 char pp[MAX_PATH];
 GetCurrentDirectory(MAX_PATH, pp);
 programpath = pp;

 music_on;


//controls being created here
//window 1 - main window
 window1 = new exywindow("trainerwindow", hInstance);
 window1->pos(0, 0, window1_width, window1_height);
 window1->setbmp("back");
 window1->transparent(RGB(255,0,0));
 window1->center();
 window1->windowmoved = followme;
  
//window2 - info window
 window2 = new exywindow("window2", hInstance);
 window2->pos(0, 0, window2_width, window2_height);
 window2->setbmp("back2");
 window2->deletefromtaskbar();
 window2->transparent(RGB(255,0,0));
 window2->dragmove = 0;
 followme();

//window 1 stuff
 closeappbutton = new exybutton("closeappbutton", hInstance, window1->windowh);
 closeappbutton->pos(482, 13, 28, 28);
 closeappbutton->setbmp("close1", "close2");
 closeappbutton->lmbc = closeappbuttonf;
 closeappbutton->show();

 minimizeappbutton = new exybutton("minimizeappbutton", hInstance, window1->windowh);
 minimizeappbutton->pos(482, 48, 28, 28);
 minimizeappbutton->setbmp("min1", "min2");
 minimizeappbutton->lmbc = minimizeappbuttonf;
 minimizeappbutton->show();

 showinfo = new exybutton("showinfo", hInstance, window1->windowh);
 showinfo->pos(482, 83, 28, 77);
 showinfo->setbmp("info1", "info2");
 showinfo->lmbc = showinfof;
 showinfo->show();

//window 2 stuff
 closeinfo = new exybutton("closeinfo", hInstance, window2->windowh);
 closeinfo->pos(499, 9, 28, 28);
 closeinfo->setbmp("close1", "close2");
 closeinfo->transparency(RGB(255, 0, 0));
 closeinfo->lmbc = closeinfof;
 closeinfo->show();
 
 nfobox = new exylistbox("nfobox", hInstance, window2->windowh, 100);
 nfobox->pos(15, 15, 480, 252);
 nfobox->setnfo(900); //for resource: >>900 RCData "app.nfo"<<
 nfobox->setlbcolors(RGB(255, 255, 255), RGB(0, 0, 0), RGB(255, 255, 255));
 nfobox->show();

 scrollup = new exybutton("scrollup", hInstance, window2->windowh);
 scrollup->pos(499, 65, 21, 80);
 scrollup->setbmp("up1", "up2");
 scrollup->transparency(RGB(255, 0, 0));
 scrollup->show();

 scrolldown = new exybutton("scrolldown", hInstance, window2->windowh);
 scrolldown->pos(499, 150, 21, 80);
 scrolldown->setbmp("down1", "down2");
 scrolldown->transparency(RGB(255, 0, 0));
 scrolldown->show();

 window1->show();
//finished creating controls

 SetTimer(window1->windowh,1,100,what_to_do);
 starttraining();

 while(GetMessage(&Message, NULL, 0, 0))
 {
  TranslateMessage(&Message);
  DispatchMessage(&Message);
 }
 return Message.wParam;
}

//program behaviour here
void CALLBACK what_to_do(HWND hwnd, UINT komu, UINT nr, DWORD czas)
{
 if (GetAsyncKeyState(VK_ESCAPE) & 1) //escape pressed
 {
  if ((GetActiveWindow()==window1->windowh || GetActiveWindow()==window2->windowh)
      && !window2->hide()) PostQuitMessage(0);
 }
 
 if (scrollup->pressed)   //for scrolling the listbox up
  nfobox->nfoup();
 if (scrolldown->pressed) //for scrolling the listbox down
  nfobox->nfodown();
}

void ShowMessage(const char *mes)
{
 MessageBox(NULL, mes, "Message", 0);
}

void ShowMessage(int mes)
{
 char tmp[10];
 memset(tmp, 0, 10);
 itoa(mes, tmp, 10);
 MessageBox(NULL, tmp, "Message", 0);    
}

void closeappbuttonf()
{
 PostQuitMessage(0);
}

void closeinfof()
{
 window2->hide();
}

void minimizeappbuttonf()
{
 ShowWindow(window1->windowh, SW_MINIMIZE);
}

void showinfof()
{
 window2->show();
}

void followme()
{
 RECT r;
 GetWindowRect(window1->windowh, &r);
 SetWindowPos(window2->windowh, NULL, r.left + window2_x_offset,
              r.top + window2_y_offset, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE);
}
