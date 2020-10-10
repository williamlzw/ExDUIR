#pragma once
#include "help_ex.h"
#include <Gdiplus.h>
#include <gdiplusbrush.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void* retPtr, size_t retLen);
void* _res_unpack(void* lpData, size_t dwDataLen, char byteHeader);
void* Ex_ResLoadFromMemory(void* lpData, size_t dwDataLen);
