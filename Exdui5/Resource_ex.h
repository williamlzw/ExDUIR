#pragma once
#include "help_ex.h"

#include <Gdiplus.h>
using namespace Gdiplus;
//#include <gdiplusbrush.h>


#pragma comment(lib, "Gdiplus.lib")

//#include <gdiplusflat.h>



void _bin_uncompress(void* lpData, size_t dwSize, void* lpKey, size_t dwLen, void** retPtr, size_t* retLen);
void* _res_unpack(void* lpData, size_t dwDataLen, UCHAR byteHeader);
void* Ex_ResLoadFromMemory(void* lpData, size_t dwDataLen);
void* Ex_ResLoadFromFile(void* lptszFile);
void Ex_ResFree(void* hRes);
bool Ex_ResGetFileFromAtom(void* hRes, int atomPath, void** lpFile, size_t* dwFileLen);
bool Ex_ResGetFile(void* hRes, void* lpwzPath, void** lpFile, size_t* dwFileLen);
