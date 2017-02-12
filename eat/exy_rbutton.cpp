#include "exy_rbutton.h"

LRESULT CALLBACK exy_rbutton_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
exyrbutton *_firstexyrbutton;

/*------------------------------------------------------------------------------
Due to the extremely inconvenient........ bla bla bla
Goes by the same rules as exy_button.
*/
//so it beginns
void exy_rbutton_blankfunction(){}
//and so it ends----------------------------------------------------------------


exyrbutton::exyrbutton(char *rbuttonname, HINSTANCE hInstance, HWND parent)
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
 textcolor = RGB(255,0,255);
 on = false;
 transp = false;
 this->hInstancecopy = hInstance;
 lmbc_on = exy_rbutton_blankfunction;
 lmbc_off = exy_rbutton_blankfunction;
 wc.style=CS_HREDRAW | CS_VREDRAW;
 wc.lpfnWndProc=exy_rbutton_WindowProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hInstance;
 wc.hIcon = LoadIcon(hInstance,"icon");
 wc.hCursor = LoadCursor(NULL, IDC_ARROW);
 wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
 wc.lpszMenuName = NULL;
 wc.lpszClassName = rbuttonname;
 RegisterClass(&wc);
 HWND desk = GetDesktopWindow();

 if (_firstexyrbutton == NULL)
  _firstexyrbutton = this;
 else
 {
  exyrbutton *current = _firstexyrbutton;
  while (current->next != NULL)
   current = current->next;
  current->next = this;    
 }

 rbuttonh = CreateWindow(rbuttonname, rbuttonname, WS_CHILD,
  x, y, width, height, parent, NULL, hInstance, NULL);
}

LRESULT CALLBACK exy_rbutton_WindowProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 exyrbutton *current = _firstexyrbutton;
 while (current != NULL && current->rbuttonh != hwnd)
  current = current->next;

 switch(Message){
  case WM_LBUTTONDOWN:
   current->LeftMouseButtonClick(hwnd, Message, wParam, lParam);
  break;
  
  case WM_ERASEBKGND:
   return 0;
  break;
  
  case WM_PAINT:
   current->WmPaint(hwnd, Message, wParam, lParam);
  break;
  
  default: 
  return (DefWindowProc(hwnd,Message,wParam,lParam));
 } 
 return 0; 
}

void exyrbutton::show()
{
 ShowWindow(this->rbuttonh, SW_SHOW);
}

void exyrbutton::hide()
{
 ShowWindow(this->rbuttonh, SW_HIDE);
}

void exyrbutton::pos(int x, int y, int width, int height)
{
 SetWindowPos(this->rbuttonh, NULL, x, y, width, height, SWP_NOOWNERZORDER);
 this->width = width;
 this->height = height;
 this->x = x;
 this->y = y;
}

void exyrbutton::setbmp(char *name2, char *name3)
{
 this->bmp2 = LoadBitmap(hInstancecopy, name2);
 this->bmp3 = LoadBitmap(hInstancecopy, name3);
 this->bmp = bmp2;
}

void exyrbutton::LeftMouseButtonClick(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
 if (this->on)
 {
  this->on = false;
  this->lmbc_off();
  this->bmp = this->bmp2;
  InvalidateRect(this->rbuttonh, NULL, true);
 } else
 {
  this->on = true;
  this->lmbc_on();
  this->bmp = this->bmp3;
  InvalidateRect(this->rbuttonh, NULL, true);
 }
}

void exyrbutton::WmPaint(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
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
  DrawText(pom, this->text, strlen(this->text), &rc,
           DT_CENTER|DT_NOCLIP|DT_NOPREFIX);
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

void exyrbutton::settext(HFONT font, char* text, int tleft, int tright ,int ttop)
{
 this->font = font;
 strcpy(this->text, text);
 this->tleft = tleft;
 this->tright = tright;
 this->ttop = ttop;
}

void exyrbutton::transparency(int c)
{
 this->transp_col = c;
 this->transp = true;   
}

bool exyrbutton::TransparentBltU(
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
