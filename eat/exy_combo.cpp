#include "exy_combo.h"
#include <windows.h>

/*------------------------------------------------------------------------------
Due to the extremely inconvenient........ bla bla bla
Goes by the same rules as exy_button (almost).
*/
//so it beginns
void exy_combo_blankfunction(){}
//and so it ends----------------------------------------------------------------

LRESULT CALLBACK exy_combo_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exycombo *_firstexycombo;
WNDPROC old_exy_combo_WindowProc;

exycombo::exycombo(char *comboname, HINSTANCE hInstance, HWND parent, int hmenu, DWORD style)
{
 next = NULL;
 width = 50;
 height = 16;
 x = 0;
 y = 0;
 this->bmp = NULL;
 cbbs = RGB(255,255,255);
 cbtxc = RGB(0,0,0);
 cbbgc = RGB(255,255,255);
 this->hInstancecopy = hInstance;
 selchange = exy_combo_blankfunction;
 GetClassInfo(hInstance, "combobox", &wc);
 wc.hInstance = hInstance;
 wc.lpszMenuName = NULL;
 wc.lpszClassName = comboname;
 old_exy_combo_WindowProc = wc.lpfnWndProc;
 wc.lpfnWndProc = exy_combo_WindowProc;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 if (_firstexycombo == NULL)
  _firstexycombo = this;
 else
 {
  exycombo *current = _firstexycombo;
  while (current->next != NULL)
   current = current->next;
  current->next = this;    
 }

 comboh = CreateWindow(comboname, comboname, style,
  200, 200, 200, 200, parent, (HMENU)hmenu, hInstance, NULL);
}

LRESULT CALLBACK exy_combo_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exycombo *current = _firstexycombo;
 while (current != NULL && current->comboh != hwnd)
  current = current->next;

 switch(Message){
  case WM_CTLCOLOREDIT:
   return current->CtlColorEdit(hwnd, Message, wParam, lParam);
  break;

  case WM_CTLCOLORLISTBOX:
   return current->CtlColorListbox(hwnd, Message, wParam, lParam);
  break;

  case WM_USER: //CBN_SELCHANGE from window
   current->selchange();
   current->bmp = current->bmp2;
   InvalidateRect(current->comboh, NULL, true);
  break;
  
  case WM_LBUTTONDOWN:
   current->bmp = current->bmp3;
   InvalidateRect(current->comboh, NULL, true);
  break;

  case WM_LBUTTONUP:
   current->bmp = current->bmp2;
   InvalidateRect(current->comboh, NULL, true);
  break;

  case WM_ERASEBKGND:
   return 0;
  break;
  
  WPARAM wp;
  case WM_ACTIVATE:
   wp = LOWORD(WA_ACTIVE)|HIWORD(0);
   SendMessage((HWND)lParam, WM_ACTIVATE, wp, (LPARAM)hwnd);
  break;
  
  case WM_PAINT:
   current->WmPaint(hwnd, Message, wParam, lParam);
  break;

  case WM_NCPAINT:
   HWND old = GetActiveWindow();
   wp = LOWORD(WA_ACTIVE)|HIWORD(0);
   SendMessage(hwnd, WM_ACTIVATE, wp, (LPARAM)old);
   SetFocus(old);
  break;
 } 

 return CallWindowProc(old_exy_combo_WindowProc, hwnd, Message, wParam, lParam);
}

LRESULT exycombo::CtlColorEdit(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 DeleteObject(this->hBrushStatic);
 this->hBrushStatic = CreateSolidBrush(this->cbbs);
 this->hdc = (HDC)wParam;
 SetTextColor(this->hdc, this->cbtxc);
 SetBkColor(this->hdc, this->cbbgc);
 return (LRESULT)this->hBrushStatic;
}

LRESULT exycombo::CtlColorListbox(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 DeleteObject(this->hBrushStatic);
 this->hBrushStatic = CreateSolidBrush(this->cbbgc);
 this->hdc = (HDC)wParam;
 SetTextColor(this->hdc, this->cbtxc);
 SetBkColor(this->hdc, this->cbbgc);
 return (LRESULT)this->hBrushStatic;
}

void exycombo::setcombocolors(COLORREF a, COLORREF b, COLORREF c)
{
 this->cbbs = a;
 this->cbtxc = b;
 this->cbbgc = c;    
}

void exycombo::setstarttext(int i)
{
 SendMessage(this->comboh, CB_SETCURSEL, (WPARAM)i, 0);
}

void exycombo::show()
{
 ShowWindow(this->comboh, SW_SHOW);
}

void exycombo::hide()
{
 ShowWindow(this->comboh, SW_HIDE);
}

void exycombo::pos(int x, int y, int width, int height)
{
 SetWindowPos(this->comboh, NULL, x, y, width, height, SWP_NOOWNERZORDER);
 this->width = width;
 this->height = height;
 this->x = x;
 this->y = y;
}

void exycombo::setbmp(char *name2, char *name3, int color)
{
 this->bmp2 = LoadBitmap(hInstancecopy, name2);
 this->bmp3 = LoadBitmap(hInstancecopy, name3);
 this->bmp = bmp2;
 this->transp_col = color;
}

void exycombo::addline(char *text)
{
 SendMessage(this->comboh, CB_ADDSTRING, 0, (LPARAM)text);
}

void exycombo::delline(int index)
{
 SendMessage(this->comboh, CB_DELETESTRING, (WPARAM)index, 0);
}

void exycombo::getline(char *text, int max)
{
 GetWindowText(this->comboh, text, max);
}

void exycombo::WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 if (this->bmp == NULL)
  return;
 PAINTSTRUCT ps;
 HDC kon,pom;
 kon=BeginPaint(hwnd,&ps);
 pom=CreateCompatibleDC(kon);
 SelectObject(pom, this->bmp);
 TransparentBltU(kon,0,0,this->width,24,pom,0,0,this->width,24,this->transp_col);
 DeleteDC(pom);
 EndPaint(hwnd,&ps);
}

bool exycombo::TransparentBltU(
     HDC dcDest,         // handle to Dest DC
     int nXOriginDest,   // x-coord of destination upper-left corner
     int nYOriginDest,   // y-coord of destination upper-left corner
     int nWidthDest,     // width of destination rectangle
     int nHeightDest,    // height of destination rectangle
     HDC dcSrc,          // handle to source DC
     int nXOriginSrc,    // x-coord of source upper-left corner
     int nYOriginSrc,    // y-coord of source upper-left corner
     int nWidthSrc,      // width of source rectangle
     int nHeightSrc,     // height of source rectangle
     UINT crTransparent  // color to make transparent
  )
{
     if (nWidthDest < 1) return false;
     if (nWidthSrc < 1) return false;
     if (nHeightDest < 1) return false;
     if (nHeightSrc < 1) return false;

     HDC dc         = CreateCompatibleDC(NULL);
     HBITMAP bitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1,
                                    GetDeviceCaps(dc, BITSPIXEL), NULL);

     if (bitmap == NULL)
         return false;

     HBITMAP oldBitmap = (HBITMAP)SelectObject(dc, bitmap);

     if (!BitBlt(dc, 0, 0, nWidthSrc, nHeightSrc, dcSrc,
                     nXOriginSrc, nYOriginSrc, SRCCOPY))
         return false;

     HDC maskDC = CreateCompatibleDC(NULL);
     HBITMAP maskBitmap = CreateBitmap(nWidthSrc,
            nHeightSrc, 1, 1, NULL);

     if (maskBitmap == NULL)
         return false;

     HBITMAP oldMask =  (HBITMAP)SelectObject(maskDC,
            maskBitmap);

     SetBkColor(maskDC, RGB(0,0,0));
     SetTextColor(maskDC, RGB(255,255,255));
     if (!BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,NULL,
                     0,0,BLACKNESS))
         return false;

     SetBkColor(dc, crTransparent);
     BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,dc,0,0,SRCINVERT);

     SetBkColor(dc, RGB(0,0,0));
     SetTextColor(dc, RGB(255,255,255));
     BitBlt(dc, 0,0,nWidthSrc,nHeightSrc,maskDC,0,0,SRCAND);

     HDC newMaskDC = CreateCompatibleDC(NULL);
     HBITMAP newMask;
     newMask = CreateBitmap(nWidthDest, nHeightDest, 1,
                GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);

     if (newMask == NULL)
     {
         SelectObject(dc, oldBitmap);
         DeleteDC(dc);
         SelectObject(maskDC, oldMask);
         DeleteDC(maskDC);
          DeleteDC(newMaskDC);
         return false;
     }

     SetStretchBltMode(newMaskDC, COLORONCOLOR);
     HBITMAP oldNewMask = (HBITMAP) SelectObject(newMaskDC,
            newMask);
     StretchBlt(newMaskDC, 0, 0, nWidthDest, nHeightDest,
            maskDC, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY);

     SelectObject(maskDC, oldMask);
     DeleteDC(maskDC);

     HDC newImageDC = CreateCompatibleDC(NULL);
     HBITMAP newImage = CreateBitmap(nWidthDest, nHeightDest,
                1, GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);

     if (newImage == NULL)
     {
         SelectObject(dc, oldBitmap);
         DeleteDC(dc);
         DeleteDC(newMaskDC);
         return false;
     }

     HBITMAP oldNewImage = (HBITMAP)SelectObject(newImageDC,
                newImage);
     StretchBlt(newImageDC, 0, 0, nWidthDest, nHeightDest,
                dc, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY);

     SelectObject(dc, oldBitmap);
     DeleteDC(dc);

     BitBlt( dcDest, nXOriginDest, nYOriginDest, nWidthDest,
                nHeightDest, newMaskDC, 0, 0, SRCAND);

     BitBlt( dcDest, nXOriginDest, nYOriginDest, nWidthDest,
            nHeightDest, newImageDC, 0, 0, SRCPAINT);

     newImage = (HBITMAP)SelectObject(newImageDC, oldNewImage);
     DeleteObject(newImage) ;
     DeleteDC(newImageDC);
     newMask  = (HBITMAP)SelectObject(newMaskDC, oldNewMask);
     DeleteObject(newMask) ;
     DeleteDC(newMaskDC);
     DeleteDC(dc) ;
     DeleteObject(bitmap) ;

     return true;
}
