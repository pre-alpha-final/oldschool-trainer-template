#include "mem_functions.h"

DWORD reverse4b(DWORD val)
{
  asm
  (
   "mov eax, [ebp+8];"
   "bswap eax;"
  );
}

DWORD reverse3b(DWORD val)
{
  asm
  (
   "mov eax, [ebp+8];"
   "bswap eax;"
   "ror eax, 8;"
  );
}

DWORD reverse2b(DWORD val)
{
  asm
  (
   "mov eax, [ebp+8];"
   "bswap eax;"
   "ror eax, 16;"
  );
}

void writetm(HANDLE ghan, DWORD address, DWORD value, int nob)
{
  if (nob == 2)value = reverse2b(value);
  if (nob == 3)value = reverse3b(value);
  if (nob == 4)value = reverse4b(value);
  WriteProcessMemory(
    ghan,
    (void*)address,
    (void *)&value,
    nob,
    NULL
   );
}

DWORD readfm(HANDLE ghan, DWORD address, int nob)
{
 DWORD tmpValue=0;
 ReadProcessMemory(
  ghan,
  (void*)address,
  (void *)&tmpValue,
  nob,
  NULL
 );
 return tmpValue;
}

DWORD point(HANDLE ghan, DWORD a, int o)
{
 a = readfm(ghan, a, 4);
 a += o;
 return a;
}

BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    TOKEN_PRIVILEGES tpPrevious;
    DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

    if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;

    //
    // first pass.  get current privilege setting
    //
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            &tpPrevious,
            &cbPrevious
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    //
    // second pass.  set privilege based on previous setting

    //
    tpPrevious.PrivilegeCount       = 1;
    tpPrevious.Privileges[0].Luid   = luid;

    if(bEnablePrivilege) {
        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    }
    else {
        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
            tpPrevious.Privileges[0].Attributes);
    }

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tpPrevious,
            cbPrevious,
            NULL,

            NULL
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    return TRUE;
}
