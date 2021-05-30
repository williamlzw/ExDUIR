#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define CVF_CLIPED 2147483648

struct canvas_s
{
	int dwFlags_;
	int width_;
	int height_;
	void* pContext_;
	void* pGdiInterop_;
	void* pBitmap_;
	wnd_s* pWnd_;
	int c_left_;
	int c_top_;
	int c_right_;
	int c_bottom_;
};

bool _md_create(void* pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, int width, int height, int* nError);

bool _md_destroy(void* pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits);

void _canvas_recreate(canvas_s* pCanvas, int width, int height, int* nError);

void _canvas_init(int* nError);

void _canvas_uninit();

bool _canvas_drawimagefrombkgimg_ex(HEXCANVAS hCanvas, HEXIMAGE hImage, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, RECT* lpRcSrc, RECTF* lpRCFDst);

bool _canvas_drawimagefrombkgimg(HEXCANVAS hCanvas, EX_BACKGROUNDIMAGEINFO* lpBkgImg);

bool _canvas_bitblt(HEXCANVAS hCanvas, HEXCANVAS sCanvas, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcLeft, int srcTop);

bool _canvas_alphablend(HEXCANVAS hCanvas, HEXCANVAS sCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha);

BOOL _canvas_calctextsize_ex(canvas_s* pCanvas, font_s* pFont, LPCWSTR lpwzText, size_t dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight, LPVOID* pLayout, INT* nError);

void _canvas_dx_drawtext_buffer(canvas_s* pCanvas, void* pLayout, EXARGB crText, float left, float top, int iGlowsize, int crShadow);

HEXCANVAS _canvas_createfrompwnd(wnd_s* pWnd, int width, int height, int dwFlags, int* nError);

HDC _canvas_getdc_ex(canvas_s* pCanvas, int* nError);

void _canvas_releasedc_ex(canvas_s* pCanvas, int* nError);

void* _cv_dx_bmp(canvas_s* pCanvas);

void* _cv_context(canvas_s* pCanvas);

void* _cv_dx_gdiinterop(canvas_s* pCanvas);
