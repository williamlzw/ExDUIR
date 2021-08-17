#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

void _object_init();
BOOL Flag_Query(INT dwFlag);
void Flag_Add(INT dwFlag);
void Flag_Del(INT dwFlag);
void IME_Control(HWND hWnd, wnd_s* pWnd, BOOL bEnable);
LPVOID Ex_MemAlloc(size_t dwSize, INT dwFlags = LMEM_ZEROINIT);
LPVOID Ex_MemReAlloc(LPVOID hMem, size_t dwSizes);
BOOL Ex_MemFree(LPVOID hMem);
LPCWSTR GetErrorMessage(DWORD error);