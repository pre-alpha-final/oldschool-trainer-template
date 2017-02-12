#ifndef EXY_EDIT_H
#define EXY_EDIT_H

#include <windows.h>

class exyedit
{
 public:
  //vairables
  WNDCLASS wc; 
  HWND edith;
  HINSTANCE hInstancecopy;
  exyedit *next;
  COLORREF ebs, etxc, ebgc;
  HBRUSH hBrushStatic;
  HDC hdc;
  int width;
  int height;
  int x;
  int y;
  void (*editchange)(HWND,UINT,WPARAM,LPARAM);
  
  //functions
  exyedit(char *editname, HINSTANCE hInstance, HWND parent, int hmenu, DWORD style);
  void show();
  void seteditcolors(COLORREF, COLORREF, COLORREF);
  void hide();
  void pos(int, int, int, int);
  void settext(const char *);
  void gettext(char *, int);

  //events
  virtual LRESULT CtlColorEdit(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};
extern exyedit *_firstexyedit;

#endif
