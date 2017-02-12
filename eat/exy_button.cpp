#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>
#include "exy_button.h"

LRESULT CALLBACK exy_button_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exybutton *_firstexybutton;

/*------------------------------------------------------------------------------
Due to the extremely inconvenient........ bla bla bla
Funcion pointer -> class members and nonclass members solution:
1. funcion exy_button_blankfuncion is created outside
2. lmbc pointer to an outside function is created inside the class
   in this pointer is stored the address to current outside
   left_mouse_button_pressed function (exy_button_blankfuction at start)
...couldn't be simpler
*/
//so it beginns
void exy_button_blankfunction(){}
//and so it ends----------------------------------------------------------------

exybutton::exybutton(char *buttonname, HINSTANCE hInstance, HWND parent)
{
 next = NULL;
 width = 100;
 height = 100;
 x = 0;
 y = 0;
 tleft=0;
 tright=0;
 ttop=0;
 memset(text,0,100);
 font = CreateFont(8,5,0,0,FW_NORMAL,0,0,0,OEM_CHARSET,
  OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS ,DEFAULT_QUALITY,
  FIXED_PITCH || FF_DONTCARE,"Terminal");
 textcolor = RGB(0,0,0);
 transp = false;
 pressed = false;
 lmbc = exy_button_blankfunction;
 this->hInstancecopy = hInstance;
 this->bmp = NULL;
 wc.style=CS_HREDRAW | CS_VREDRAW;
 wc.lpfnWndProc=exy_button_WindowProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hInstance;
 wc.hIcon = LoadIcon(hInstance,"icon");
 wc.hCursor = LoadCursor(NULL, IDC_ARROW);
 wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
 wc.lpszMenuName = NULL;
 wc.lpszClassName = buttonname;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 if (_firstexybutton == NULL)
  _firstexybutton = this;
 else
 {
  exybutton *current = _firstexybutton;
  while (current->next != NULL)
   current = current->next;
  current->next = this;    
 }

 buttonh = CreateWindow(buttonname, buttonname, WS_CHILD,
  x, y, width, height, parent, NULL, hInstance, NULL);
}

LRESULT CALLBACK exy_button_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exybutton *current = _firstexybutton;
 while (current != NULL && current->buttonh != hwnd)
  current = current->next;

 switch(Message){
  case WM_LBUTTONDOWN:
   current->bmp = current->bmp3;
   InvalidateRect(current->buttonh, NULL, true);
   current->pressed = true;
   SetCapture(current->buttonh);
  break;
  
  case WM_ERASEBKGND:
   return 0;
  break;

  case WM_LBUTTONUP:
   current->bmp = current->bmp2;
   InvalidateRect(current->buttonh, NULL, true);
   current->lmbc();
   current->pressed = false;
   ReleaseCapture();
  break;
  
  case WM_PAINT:
   current->WmPaint(hwnd, Message, wParam, lParam);

  break;
  
  default: 
  return (DefWindowProc(hwnd,Message,wParam,lParam));
 } 
 return 0; 
}

void exybutton::show()
{
 ShowWindow(this->buttonh, SW_SHOW);
}

void exybutton::hide()
{
 ShowWindow(this->buttonh, SW_HIDE);
}

void exybutton::pos(int x, int y, int width, int height)
{
 SetWindowPos(this->buttonh, NULL, x, y, width, height, SWP_NOOWNERZORDER);
 this->width = width;
 this->height = height;
 this->x = x;
 this->y = y;
}

void exybutton::transparency(int c)
{
 this->transp_col = c;
 this->transp = true;   
}

void exybutton::setbmp(char *name2, char *name3)
{
 this->bmp2 = LoadBitmap(hInstancecopy, name2);
 this->bmp3 = LoadBitmap(hInstancecopy, name3);
 this->bmp = bmp2;
 strcpy(this->bitmap1name, name2);
 strcpy(this->bitmap2name, name3);
}

void exybutton::settext(HFONT font, char* text, int tleft,  int ttop, int tright, COLORREF col)
{
 this->setbmp(bitmap1name, bitmap2name);     
 this->font = font;
 this->textcolor = col;
 strcpy(this->text, text);
 this->tleft = tleft;
 this->tright = tright;
 this->ttop = ttop;
 InvalidateRect(this->buttonh, NULL, true);
}

void exybutton::WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 PAINTSTRUCT ps;
 HDC kon,pom;
 kon=BeginPaint(hwnd,&ps);
 pom=CreateCompatibleDC(kon);
 if (this->bmp != NULL)
  SelectObject(pom, this->bmp);
 if (this->text[0] != 0x00)
 {
  RECT rc;
  rc.left=this->tleft; rc.top=this->ttop; rc.right=this->tright; rc.bottom=100;
  SetBkMode(pom, TRANSPARENT);
  SetTextColor(pom, this->textcolor);
  SelectObject(pom, this->font);
  DrawTextEx(pom, this->text, strlen(this->text), &rc,
           DT_CENTER|DT_NOPREFIX, NULL);
 }
 if (this->transp)
 {
  TransparentBltU(kon,0,0,this->width,this->height,pom,0,0,this->width,this->height,this->transp_col);
 } else
 {
  BitBlt(kon,0,0,this->width,this->height,pom,0,0,SRCCOPY);
 }
 DeleteDC(pom);
 EndPaint(hwnd,&ps);
}

bool exybutton::TransparentBltU(
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
