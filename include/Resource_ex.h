#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void** retPtr, size_t* retLen);
HEXRES _res_unpack(void* lpData, size_t dwDataLen, UCHAR byteHeader);