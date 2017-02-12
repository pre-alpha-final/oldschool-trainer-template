#ifndef EXY_RBUTTON_H
#define EXY_RBUTTON_H

#include <windows.h>

class exyrbutton
{
 public:
  //vairables
  WNDCLASS wc; 
  HWND rbuttonh;
  HINSTANCE hInstancecopy;
  exyrbutton *next;
  HBITMAP bmp, bmp2, bmp3;
  bool on;
  int width;
  int height;
  int x;
  int y;
  bool transp;
  HFONT font;
  char text[100];
  int tleft, tright, ttop;
  COLORREF textcolor;
  int transp_col;
  void (*lmbc_on)();  
  void (*lmbc_off)();  
   
  //functions
  exyrbutton(char *buttonname, HINSTANCE hInstance, HWND parent);
  void show();
  void hide();
  void pos(int, int, int, int);
  void setbmp(char *, char *);
  void transparency(int);
  void settext(HFONT, char*, int, int ,int);
  bool TransparentBltU(HDC, int, int, int ,int ,HDC, int, int, int, int, UINT);
  
  //events
  void WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
  void LeftMouseButtonClick(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};

#endif
