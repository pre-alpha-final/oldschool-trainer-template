#ifndef EXY_WINDOW_H
#define EXY_WINDOW_H

#include <windows.h>

class exywindow
{
 public:
  //vairables
  WNDCLASS wc; 
  HINSTANCE hInstancecopy;
  HWND windowh;
  exywindow *next;
  int width;
  int height;
  int x;
  int y;
  HBITMAP bmp;
  char bmpname[100];
  byte showstatus;
  void (*windowmoved)();
  void (*lmbc)();
  bool created;
  bool dragmove;
  
  //functions
  exywindow(char *windowame, HINSTANCE hInstance);
  bool show();
  bool hide();
  void pos(int, int, int, int);
  void setbmp(char *);
  void deletefromtaskbar();
  void transparent(COLORREF color);
  void center();
  void under(HWND);
  void right(HWND);

  //events
  virtual void WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
  virtual void WmDestroy(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
  virtual void WmKeydown(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};

#endif
