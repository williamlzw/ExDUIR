#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

LPVOID Ex_MemAlloc(size_t dwSize, INT dwFlags = LMEM_ZEROINIT);
LPVOID Ex_MemReAlloc(LPVOID hMem, size_t dwSizes);
BOOL Ex_MemFree(LPVOID hMem);
