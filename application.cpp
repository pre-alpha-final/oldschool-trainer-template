#include "main.h"
#include "application.h"
#include ".\eat\mem_functions.h"

void CALLBACK train(HWND, UINT, UINT, DWORD);

HWND hWnd;
HANDLE ghan, tghan;
DWORD lpdwProcessId;

void starttraining()
{
 SetTimer(window1->windowh,2,100,train);
}

void CALLBACK train(HWND, UINT, UINT, DWORD)
{
 hWnd=FindWindow(NULL, gamewindowname);
 GetWindowThreadProcessId(hWnd, &lpdwProcessId);
 OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                  &tghan);
 SetPrivilege(tghan, SE_DEBUG_NAME, TRUE);
 ghan=OpenProcess(PROCESS_ALL_ACCESS, false, lpdwProcessId);
 SetPrivilege(tghan, SE_DEBUG_NAME, FALSE);     
     
 if (GetKeyState(VK_Q) & 0x010000)
 {
  ShowMessage("Q key was pressed");
 } else
 if (GetKeyState(VK_F1) & 0x010000)
 {
  ShowMessage("F1 key was pressed");
 } else
 if (GetKeyState(VK_MENU) & 0x010000)
 {
  ShowMessage("ALT key was pressed");
 } else
 if (GetKeyState(VK_Z) & 0x010000)
 {
  //example warcraft 3 code
  //zero mine
  /* 
  writetm(ghan, 0x4002b5, 0x8b706c39, 4);
  writetm(ghan, 0x4002b9, 0xee53bb00, 4);
  writetm(ghan, 0x4002bd, 0x00000089, 4);
  writetm(ghan, 0x4002c1, 0x586c5be9, 4);
  writetm(ghan, 0x4002c5, 0x74eff56e, 4);
  writetm(ghan, 0x6f35f238, 0xe978100a, 4);
  writetm(ghan, 0x6f35f23c, 0x91, 1);
  */
 }
}
