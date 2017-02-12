#ifndef EXY_LISTBOX_H
#define EXY_LISTBOX_H

#include <windows.h>

class exylistbox
{
 public:
  //vairables
  WNDCLASS wc; 
  HWND listboxh;
  HINSTANCE hInstancecopy;
  exylistbox *next;
  COLORREF lbbs, lbtxc, lbbgc;
  HBRUSH hBrushStatic;
  HDC hdc;
  HFONT hFont;
  int width;
  int height;
  int x;
  int y;
  int nfoshow, max_nfo;
  
  //functions
  exylistbox(char *listboxname, HINSTANCE hInstance, HWND parent, int hmenu);
  void show();
  void hide();
  void pos(int, int, int, int);
  void settext(char*);
  void setnfo(int);
  void nfoup();
  void nfodown();
  void setlbcolors(COLORREF a, COLORREF b, COLORREF c);
  void add(const char*);
  void clear();

  //events
  virtual LRESULT CtlColorListbox(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
};

#endif
