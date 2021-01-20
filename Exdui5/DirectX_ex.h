#pragma once
#include <wincodec.h>
//#include <Gdiplus.h>
//#include <gdiplusbrush.h>
//using namespace Gdiplus;
//#pragma comment(lib, "Gdiplus.lib")
#include <d3d11.h>
#include <d2d1_1.h>
#include <d2d1.h>

#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "windowscodecs.lib")


#include "help_ex.h"
#include "Global_ex.h"

bool _dx_init(int* nError);
void _dx_uninit();
void* _dx_get_gdiInterop(void* prt);
void* _dx_createbitmap(void* pDeviceContext, int width, int height, bool fGDI, int* nError);
void _dx_settarget(void* pDeviceContext, void* pBitmap);
void _dx_getsize(void* pBitmap, float* width, float* height);
void* _dx_gettarget(void* pDeviceContext);
void _dx_begindraw(void* pDeviceContext);
int _dx_enddraw(void* pDeviceContext);
void _dx_flush(void* pDeviceContext);
void _dx_clear(void* pDeviceContext, int Color);
bool _dx_createeffect(void* pDeviceContext, IID peffectId, void** peffect, int* nError);
void _dx_blur(void* pDeviceContext, void* pBitmap, float fDeviation, void* lprc, int* nError);
void _dx_rotate_hue(void* pContext, void* pBitmap, float fAngle, int* nError);
void _dx_cliprect(void* pDeviceContext, float left, float top, float right, float bottom);
void _dx_resetclip(void* pDeviceContext);
void _dx_drawbitmaprect(void* pDeviceContext, void* pBitmap, float dstLeft, float dstTop, float dstRight, float dstBottom, int dwAlpha);
void _dx_drawbitmaprectrect(void* pDeviceContext, void* pBitmap, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int dwAlpha);
void _dx_drawimage(void* pDeviceContext, void* pimage, float left, float top, int mode);
void _dx_drawimage_ex(void* pDeviceContext, void* pimage, float dstLeft, float dstTop, float srcLeft, float srcTop, float srcRight, float srcBottom, int mode);
void _dx_bmp_copyfrom(void** pDestBitmap, void* pSrcBitmap, int dX, int dY, int srcLeft, int srcTop, int srcRight, int srcBottom);

void ARGB2ColorF(int argb, D2D1_COLOR_F *color);