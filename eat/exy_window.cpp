#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>
#include "exy_window.h"

LRESULT CALLBACK exy_window_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exywindow *_firstexywindow;

/*------------------------------------------------------------------------------
Due to the extremely inconvenient........ bla bla bla
Funcion pointer -> class members and nonclass members solution:
1. funcion exy_window_blankfuncion is created outside
2. pointer to an outside function is created inside the class
   in this pointer is stored the address to current outside
   function
...couldn't be simpler
*/
//so it beginns
void exy_window_blankfunction(){}
//and so it ends----------------------------------------------------------------

exywindow::exywindow(char *windowname, HINSTANCE hInstance)
{
 created = false;
 windowmoved = exy_window_blankfunction;
 lmbc = exy_window_blankfunction;
 dragmove = 1;
 hInstancecopy = hInstance;
 next = NULL;
 width = 500;
 height = 500;
 x = 0;
 y = 0;
 showstatus = 0;
 wc.style=CS_HREDRAW | CS_VREDRAW;
 wc.lpfnWndProc=exy_window_WindowProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hInstance;
 wc.hIcon = LoadIcon(hInstance,"icon");
 wc.hCursor = LoadCursor(NULL, IDC_ARROW);
 wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
 wc.lpszMenuName = NULL;
 wc.lpszClassName = windowname;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 if (_firstexywindow == NULL)
  _firstexywindow = this;
 else
 {
  exywindow *current = _firstexywindow;
  while (current->next != NULL)
   current = current->next;
  current->next = this;
 }

 this->windowh = CreateWindow(windowname, windowname, WS_POPUPWINDOW,
  x, y, width, height, NULL, NULL, hInstance, NULL);

 POINT pts[4];  
 HRGN rgn;
 pts[0].x = 1; pts[0].y = 1;
 pts[1].x = this->width-1; pts[1].y = 1;
 pts[2].x = this->width-1; pts[2].y = this->height-1;
 pts[3].x = 1; pts[3].y = this->height-1;
 rgn = CreatePolygonRgn(&pts[0], 4, WINDING);
 SetWindowRgn(windowh, rgn, true); 
 DeleteObject(rgn);
 this->width -= 2;
 this->height -= 2;

 ZeroMemory(bmpname, 100);
 strcpy(bmpname, "x");
 bmp=LoadBitmap(hInstance, bmpname);
}

LRESULT CALLBACK exy_window_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exywindow *current = _firstexywindow;
 while (current != NULL && current->windowh != hwnd)
  current = current->next;

 switch(Message){
  case WM_LBUTTONDOWN:
   current->lmbc();
   if (current != NULL && current->dragmove)
    SendMessage(current->windowh, WM_NCLBUTTONDOWN, HTCAPTION, 0);
  break;
  
  case WM_CTLCOLORLISTBOX:
   return SendMessage((HWND)lParam, WM_CTLCOLORLISTBOX, wParam, lParam); //let the listbox handle it
  break;
  
  case WM_CTLCOLOREDIT:
   return SendMessage((HWND)lParam, WM_CTLCOLOREDIT, wParam, lParam); //let the edit handle it
  break;
  
  case WM_PAINT:
   current->WmPaint(hwnd, Message, wParam, lParam);
   current->created = true;
  break;
  
  case WM_MOVE:
   if (current != NULL && current->created)
    current->windowmoved();
  break;

  exywindow *tmpexywindow;
  case WM_NCPAINT:
   if (hwnd == _firstexywindow->windowh)
   {
    tmpexywindow = _firstexywindow->next;
    while (tmpexywindow != NULL)
    {
     if (tmpexywindow->showstatus)
      SetActiveWindow(tmpexywindow->windowh);
     tmpexywindow = tmpexywindow->next;
    }
   }
  break;

  case WM_DESTROY:
   current->WmDestroy(hwnd, Message, wParam, lParam);
  break;
  
  case WM_KEYDOWN:
   current->WmKeydown(hwnd, Message, wParam, lParam);
  break;
  
  case WM_CTLCOLORSTATIC:
   return SendMessage((HWND)lParam, WM_USER+2, wParam, lParam);
  break;     

  case WM_COMMAND:
   switch  HIWORD(wParam)
   {
    case CBN_SELCHANGE:
     SendMessage((HWND)lParam, WM_USER, wParam, lParam);
    break;     

    case EN_CHANGE:
     SendMessage((HWND)lParam, WM_USER+1, wParam, lParam);
    break;     
   }
  break;
  
  default: 
  return (DefWindowProc(hwnd,Message,wParam,lParam));
 } 
 return 0; 
}

bool exywindow::show()
{
 this->showstatus = 1;
 return ShowWindow(this->windowh, SW_SHOW);
}

bool exywindow::hide()
{
 this->showstatus = 0;
 return ShowWindow(this->windowh, SW_HIDE);
}

void exywindow::pos(int x, int y, int width, int height)
{
 //standard
 SetWindowPos(this->windowh, NULL, x, y, width+2, height+2, SWP_NOOWNERZORDER);
 this->width = width+2;
 this->height = height+2;
 this->x = x;
 this->y = y;

 //cut frame
 POINT pts[4];  
 HRGN rgn;
 pts[0].x = 1; pts[0].y = 1;
 pts[1].x = this->width-1; pts[1].y = 1;
 pts[2].x = this->width-1; pts[2].y = this->height-1;
 pts[3].x = 1; pts[3].y = this->height-1;
 rgn = CreatePolygonRgn(&pts[0], 4, WINDING);
 SetWindowRgn(windowh, rgn, true); 
 DeleteObject(rgn);
 this->width -= 2;
 this->height -= 2;
}

void exywindow::setbmp(char *name)
{
 this->bmp = LoadBitmap(hInstancecopy, name);
}

void exywindow::deletefromtaskbar()
{ 
 SetWindowLong(this->windowh, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
} 

void exywindow::transparent(COLORREF color)
{ 
 HWND hWnd=this->windowh; 
 SetWindowLong (hWnd , GWL_EXSTYLE , 
  GetWindowLong (hWnd , GWL_EXSTYLE ) | WS_EX_LAYERED ) ;
 typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);
 PSLWA pSetLayeredWindowAttributes;
 HMODULE hDLL = LoadLibrary ("user32");
 pSetLayeredWindowAttributes = 
 (PSLWA) GetProcAddress(hDLL,"SetLayeredWindowAttributes");
 if (pSetLayeredWindowAttributes != NULL)
 {
    pSetLayeredWindowAttributes (hWnd, 
        color, 128, LWA_COLORKEY);
 }
}

void exywindow::WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 PAINTSTRUCT ps;
 HDC kon,pom;
 kon=BeginPaint(hwnd,&ps);
 pom=CreateCompatibleDC(kon);
 SelectObject(pom, this->bmp);
 BitBlt(kon,0,0,this->width,this->height,pom,0,0,SRCCOPY);
 DeleteDC(pom);
 EndPaint(hwnd,&ps);
}

void exywindow::WmKeydown(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
}

void exywindow::WmDestroy(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 PostQuitMessage(0);
}

void exywindow::center()
{
 RECT screencenter;
 HWND desk;
 desk = GetDesktopWindow();
 GetWindowRect(desk, &screencenter);
 SetWindowPos(this->windowh, NULL, 
  screencenter.right/2 - this->width/2,
  screencenter.bottom/2 - this->height/2, 
  this->width, this->height, SWP_NOOWNERZORDER|SWP_NOSIZE);
 this->x = screencenter.right/2 - this->width/2;
 this->y = screencenter.bottom/2 - this->height/2;
}

void exywindow::under(HWND hwnd)
{
 RECT hwnd1pos;
 GetWindowRect(hwnd, &hwnd1pos);
 SetWindowPos(this->windowh, NULL, 
  hwnd1pos.left,
  hwnd1pos.bottom-2, 
  this->width, this->height, SWP_NOOWNERZORDER|SWP_NOSIZE);
 this->x = hwnd1pos.left;
 this->y = hwnd1pos.bottom-2;
}

void exywindow::right(HWND hwnd)
{
 RECT hwnd1pos;
 GetWindowRect(hwnd, &hwnd1pos);
 SetWindowPos(this->windowh, NULL, 
  hwnd1pos.right-2,
  hwnd1pos.top, 
  this->width, this->height, SWP_NOOWNERZORDER|SWP_NOSIZE);
 this->x = hwnd1pos.right-2;
 this->y = hwnd1pos.top;
}

