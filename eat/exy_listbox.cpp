#include "exy_listbox.h"

LRESULT CALLBACK exy_listbox_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exylistbox *_firstexylistbox;
WNDPROC old_exy_listbox_WindowProc;

exylistbox::exylistbox(char *listboxname, HINSTANCE hInstance, HWND parent, int hmenu)
{
 next = NULL;
 width = 100;
 height = 100;
 x = 0;
 y = 0;
 nfoshow = 0;
 max_nfo = 0;
 lbbs = RGB(255,255,255);
 lbtxc = RGB(0,0,0);
 lbbgc = RGB(255,255,255);
 this->hInstancecopy = hInstance;
 GetClassInfo(hInstance, "listbox", &wc);
 wc.hInstance = hInstance;
 wc.lpszMenuName = NULL;
 wc.lpszClassName = listboxname;
 old_exy_listbox_WindowProc = wc.lpfnWndProc;
 wc.lpfnWndProc = exy_listbox_WindowProc;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 this->hFont = CreateFont(10,0,0,0,0,0,0,0,0,0,0,0,0,"Lucida Console");
// this->hFont = CreateFont(8,5,0,0,FW_NORMAL,0,0,0,OEM_CHARSET,
//  OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS ,DEFAULT_QUALITY,
//  FIXED_PITCH || FF_DONTCARE,"Terminal");
 
 if (_firstexylistbox == NULL)
  _firstexylistbox = this;
 else
 {
  exylistbox *current = _firstexylistbox;
  while (current->next != NULL)
   current = current->next;
  current->next = this;    
 }

 listboxh = CreateWindow(listboxname, listboxname, WS_CHILD|LBS_NOSEL,
  200, 200, 200, 200, parent, (HMENU)hmenu, hInstance, NULL);
}

LRESULT CALLBACK exy_listbox_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exylistbox *current = _firstexylistbox;
 while (current != NULL && current->listboxh != hwnd)
  current = current->next;

 switch(Message){
  case WM_CTLCOLORLISTBOX:
   return current->CtlColorListbox(hwnd, Message, wParam, lParam);
  break;
 } 
 return CallWindowProc(old_exy_listbox_WindowProc, hwnd, Message, wParam, lParam);
}

LRESULT exylistbox::CtlColorListbox(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{        
 DeleteObject(hBrushStatic);
 this->hBrushStatic = CreateSolidBrush(this->lbbs);
 this->hdc = (HDC)wParam;
 SetTextColor(this->hdc, this->lbtxc);
 SetBkColor(this->hdc, this->lbbgc);
 SelectObject(this->hdc, this->hFont);
 return (LRESULT)this->hBrushStatic;
}

void exylistbox::setlbcolors(COLORREF a, COLORREF b, COLORREF c)
{
 this->lbbs = a;
 this->lbtxc = b;
 this->lbbgc = c;    
}

void exylistbox::show()
{
 ShowWindow(this->listboxh, SW_SHOW);
}

void exylistbox::hide()
{
 ShowWindow(this->listboxh, SW_HIDE);
}

void exylistbox::clear()
{
 int lines;
 lines = SendMessage(this->listboxh, LB_GETCOUNT, 0, 0);
 for (int i=0; i<lines; i++)
 {
  SendMessage(this->listboxh, LB_DELETESTRING, WPARAM(0), 0);
 }
 this->max_nfo = 0;
}

void exylistbox::pos(int x, int y, int width, int height)
{
 SetWindowPos(this->listboxh, NULL, x, y, width, height, SWP_NOOWNERZORDER);
 this->width = width;
 this->height = height;
 this->x = x;
 this->y = y;
}

void exylistbox::add(const char* text)
{
 SendMessage(this->listboxh, LB_ADDSTRING, 0, (LPARAM)text);
 SendMessage(this->listboxh, LB_SETITEMHEIGHT, 0, MAKELPARAM(8, 0));
 this->max_nfo++;
}

void exylistbox::settext(char *text)
{
 int size = strlen(text);
 char line[250];
 ZeroMemory(line, 250);
 char* now;
 int k = 0;
 this->max_nfo = 0;
 for (int i=0; i<size; i++)
 {
  now = text + i;
  
  if (*now == 0x0d)
   continue;
  if (*now == 0x0a)
  {
   SendMessage(this->listboxh, LB_ADDSTRING, 0, (LPARAM)line);
   SendMessage(this->listboxh, LB_SETITEMHEIGHT, 0, MAKELPARAM(8, 0));
   k = 0;
   ZeroMemory(line, 250);
   this->max_nfo++;
   continue;
  }
  line[k] = *now;
  k++;
 }
 if (strlen(line)>0)
 {
  SendMessage(this->listboxh, LB_ADDSTRING, 0, (LPARAM)line);
  SendMessage(this->listboxh, LB_SETITEMHEIGHT, 0, MAKELPARAM(8, 0));
 }
}

void exylistbox::setnfo(int res)
{
 this->max_nfo = 0;     
 HRSRC hrsrc = FindResource(this->hInstancecopy,(LPCTSTR)res,RT_RCDATA);
 HGLOBAL hrsrcglob = LoadResource(this->hInstancecopy, hrsrc);
 int size = SizeofResource(this->hInstancecopy, hrsrc);
 char line[250];
 ZeroMemory(line, 250);
 char* now;
 int k = 0;
 for (int i=0; i<size; i++)
 {
  now = (char*)hrsrcglob + i;
  
  if (*now == 0x0d)
   continue;
  if (*now == 0x0a)
  {
   SendMessage(this->listboxh, LB_ADDSTRING, 0, (LPARAM)line);
   SendMessage(this->listboxh, LB_SETITEMHEIGHT, 0, MAKELPARAM(8, 0));
   k = 0;
   ZeroMemory(line, 250);
   this->max_nfo++;
   continue;
  }
  line[k] = *now;
  k++;
 }
 if (strlen(line)>0)
 {
  SendMessage(this->listboxh, LB_ADDSTRING, 0, (LPARAM)line);
  SendMessage(this->listboxh, LB_SETITEMHEIGHT, 0, MAKELPARAM(8, 0));
 }
}

void exylistbox::nfoup()
{
 if (this->nfoshow>2) this->nfoshow-=2; else this->nfoshow = 0;
 SendMessage(this->listboxh, LB_SETTOPINDEX, this->nfoshow, 0);
}

void exylistbox::nfodown()
{
 if (this->nfoshow<this->max_nfo) this->nfoshow+=2;
 if (this->nfoshow>this->max_nfo-this->height/8+1)
  this->nfoshow = this->max_nfo-this->height/8+1;
 SendMessage(this->listboxh, LB_SETTOPINDEX, this->nfoshow, 0);
}
