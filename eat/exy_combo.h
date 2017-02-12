#ifndef EXY_COMBO_H
#define EXY_COMBO_H

#include <windows.h>

class exycombo
{
 public:
  //vairables
  WNDCLASS wc; 
  HWND comboh;
  HINSTANCE hInstancecopy;
  exycombo *next;
  COLORREF cbbs, cbtxc, cbbgc;
  HBRUSH hBrushStatic;
  HBITMAP bmp, bmp2, bmp3;
  HDC hdc;
  int width;
  int height;
  int x;
  int y;
  int transp_col;
  void (*selchange)();
  
  //functions
  exycombo(char *comboname, HINSTANCE hInstance, HWND parent, int hmenu, DWORD style);
  void show();
  void hide();
  void pos(int, int, int, int);
  void addline(char *);
  void delline(int);
  void getline(char *, int);
  void setcombocolors(COLORREF a, COLORREF b, COLORREF c);
  void setstarttext(int);
  void setbmp(char *, char *, int);
  bool TransparentBltU(HDC, int, int, int ,int ,HDC, int, int, int, int, UINT);

  //events
  virtual LRESULT CtlColorEdit(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
  virtual LRESULT CtlColorListbox(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
  void WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};

#endif
