#ifndef EXY_BUTTON_H
#define EXY_BUTTON_H

#include <windows.h>
#include <commctrl.h>

class exybutton
{
 public:
  //vairables
  WNDCLASS wc; 
  HWND buttonh;
  HINSTANCE hInstancecopy;
  exybutton *next;
  HBITMAP bmp, bmp2, bmp3;
  char bitmap1name[100];
  char bitmap2name[100];
  bool pressed;
  int width;
  int height;
  int x;
  int y;
  bool transp;
  HFONT font;
  char text[100];
  int tleft, tright, ttop;
  COLORREF textcolor;
  COLORREF transp_col;
  void (*lmbc)();
   
  //functions
  exybutton(char *buttonname, HINSTANCE hInstance, HWND parent);
  void show();
  void hide();
  void pos(int, int, int, int);
  void setbmp(char *, char *);
  void transparency(int);
  void settext(HFONT, char*, int, int ,int, COLORREF);
  bool TransparentBltU(HDC, int, int, int ,int ,HDC, int, int, int, int, UINT);
    
  //events
  virtual void WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};

#endif
