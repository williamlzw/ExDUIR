#pragma once
#include "help_ex.h"
#include <Gdiplus.h>
#include <gdiplusbrush.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void* retPtr, size_t retLen);