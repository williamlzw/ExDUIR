#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

void _bin_uncompress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen, LPVOID* retPtr, size_t* retLen);
HEXRES _res_unpack(LPVOID lpData, size_t dwDataLen, UCHAR byteHeader);