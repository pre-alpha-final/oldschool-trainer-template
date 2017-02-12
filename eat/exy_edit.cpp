#include "exy_edit.h"

/*------------------------------------------------------------------------------
Due to the extremely inconvenient........ bla bla bla
Goes by the same rules as exy_button (almost).
*/
//so it beginns
void exy_edit_blankfunction(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam){}
//and so it ends----------------------------------------------------------------

LRESULT CALLBACK exy_edit_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exyedit *_firstexyedit;
WNDPROC old_exy_edit_WindowProc;

exyedit::exyedit(char *editname, HINSTANCE hInstance, HWND parent, int hmenu, DWORD style)
{
 next = NULL;
 width = 50;
 height = 16;
 x = 0;
 y = 0;
 ebs = RGB(255,255,255);
 etxc = RGB(0,0,0);
 ebgc = RGB(255,255,255);
 this->hInstancecopy = hInstance;
 this->editchange = exy_edit_blankfunction;
 GetClassInfo(hInstance, "edit", &wc);
 wc.hInstance = hInstance;
 wc.lpszMenuName = NULL;
 wc.lpszClassName = editname;
 old_exy_edit_WindowProc = wc.lpfnWndProc;
 wc.lpfnWndProc = exy_edit_WindowProc;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 if (_firstexyedit == NULL)
  _firstexyedit = this;
 else
 {
  exyedit *current = _firstexyedit;
  while (current->next != NULL)
   current = current->next;
  current->next = this;    
 }

 this->edith = CreateWindow(editname, editname, style,
  200, 200, 200, 200, parent, (HMENU)hmenu, hInstance, NULL);
}

LRESULT CALLBACK exy_edit_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exyedit *current = _firstexyedit;
 while (current != NULL && current->edith != hwnd)
 {
  if (current->next == NULL) //this is cause some non edit messages are also
                             //being sent here
   return CallWindowProc(old_exy_edit_WindowProc, hwnd, Message, wParam, lParam);
  current = current->next;
 }
  
 switch(Message)
 {
  case WM_CTLCOLOREDIT:
   return current->CtlColorEdit(hwnd, Message, wParam, lParam);
  break;

  case WM_USER+1: //EN_CHANGE from window
   current->editchange(hwnd, Message, wParam, lParam);
  break;

  case WM_USER+2: //WM_CTLCOLORSTATIC from window
   return current->CtlColorEdit(hwnd, Message, wParam, lParam);
  break;
 } 

 return CallWindowProc(old_exy_edit_WindowProc, hwnd, Message, wParam, lParam);
}

LRESULT exyedit::CtlColorEdit(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 DeleteObject(this->hBrushStatic);
 this->hBrushStatic = CreateSolidBrush(this->ebs);
 this->hdc = (HDC)wParam;
 SetTextColor(this->hdc, this->etxc);
 SetBkColor(this->hdc, this->ebgc);
 SetBkMode(hdc, OPAQUE);
 return (LRESULT)this->hBrushStatic;
}

void exyedit::show()
{
 ShowWindow(this->edith, SW_SHOW);
}

void exyedit::hide()
{
 ShowWindow(this->edith, SW_HIDE);
}

void exyedit::pos(int x, int y, int width, int height)
{
 SetWindowPos(this->edith, NULL, x, y, width, height, SWP_NOOWNERZORDER);
 this->width = width;
 this->height = height;
 this->x = x;
 this->y = y;
}

void exyedit::seteditcolors(COLORREF a, COLORREF b, COLORREF c)
{
 this->ebs = a;
 this->etxc = b;
 this->ebgc = c;    
}

void exyedit::settext(const char *text)
{
 SetWindowText(this->edith, text);
}

void exyedit::gettext(char *text, int max)
{
 GetWindowText(this->edith, text, max);
}
