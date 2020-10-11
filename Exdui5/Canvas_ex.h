#pragma once
#include "help_ex.h"
#include "Font_ex.h"
#include "Brush_ex.h"
#include "Image_ex.h"
#include "DirectX_ex.h"
#include "Wnd_ex.h"
#include "Path_ex.h"
#include "Global_ex.h"


#define CVF_CLIPED 2147483648




struct canvas_s
{
	int dwFlags_;
	int width_;
	int height_;
	void* pContext_;
	void* pBitmap_;
	void* pWnd_;
	int c_left_;
	int c_top_;
	int c_right_;
	int c_bottom_;
};

bool _canvas_destroy(size_t hCanvas);
void _canvas_recreate(void* pCanvas, int width, int height, int* nError);
bool _canvas_resize(size_t hCanvas, int width, int height);
void _canvas_init(int* nError);
void* _canvas_getcontext(size_t hCanvas, int nType);
bool _canvas_begindraw(size_t hCanvas);
bool _canvas_enddraw(size_t hCanvas);
bool _canvas_clear(size_t hCanvas, int Color);
bool _canvas_drawline(size_t hCanvas, void* hBrush, float X1, float Y1, float X2, float Y2, float strokeWidth, void* strokeStyle);
bool _canvas_drawrect(size_t hCanvas, void* hBrush, float left, float top, float right, float bottom, float strokeWidth, void* strokeStyle);
bool _canvas_fillrect(size_t hCanvas, void* hBrush, float left, float top, float right, float bottom);
bool _canvas_drawroundedrect(size_t hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY, float strokeWidth, void* strokeStyle);
bool _canvas_fillroundedrect(size_t hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY);
bool _canvas_drawpath(size_t hCanvas, size_t hPath, void* hBrush, float strokeWidth, void* strokeStyle);
bool _canvas_fillpath(size_t hCanvas, size_t hPath, void* hBrush);
bool _canvas_drawellipse(size_t hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY, float strokeWidth, void* strokeStyle);
bool _canvas_fillellipse(size_t hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY);
bool _canvas_fillregion(size_t hCanvas, void* hRgn, void* hBrush);
bool _canvas_drawimagerectrect(size_t hCanvas, size_t hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha);
bool _canvas_drawimagerect(size_t hCanvas, size_t hImage, float Left, float Top, float Right, float Bottom, int alpha);
bool _canvas_drawimage(size_t hCanvas, size_t hImage, float Left, float Top, int alpha);
bool _canvas_drawimagefromgrid(size_t hCanvas, size_t hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, float gridPaddingLeft, float gridPaddingTop, float gridPaddingRight,
	float gridPaddingBottom, int dwFlags, int dwAlpha);
bool _canvas_drawimagefrombkgimg_ex(size_t hCanvas, size_t hImage, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, void* lpRcSrc, void* lpRCFDst);
bool _canvas_drawimagefrombkgimg(size_t hCanvas, void* lpBkgImg);
bool _canvas_cliprect(size_t hCanvas, int left, int top, int right, int bottom);
bool _canvas_resetclip(size_t hCanvas);
bool _canvas_flush(size_t hCanvas);
bool _canvas_bitblt(size_t hCanvas, size_t sCanvas, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcLeft, int srcTop);
bool _canvas_alphablend(size_t hCanvas, size_t sCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha);
bool _canvas_getsize(size_t hCanvas, int* width, int* height);
bool _canvas_calctextsize_ex(void* pCanvas, void* pFont, LPCWSTR lpwzText, int dwLen, int dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, void* lpWidth, void* lpHeight, void** pLayout, int* nError);
bool _canvas_calctextsize(size_t hCanvas, void* hFont, LPCWSTR lpwzText, int dwLen, int dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, void* lpWidth, void* lpHeight);
void _canvas_dx_drawtext_buffer(void* pCanvas, void* pLayout, int crText, float left, float top, int iGlowsize, int crShadow);
bool _canvas_drawtextex(size_t hCanvas, void* hFont, int crText, LPCWSTR lpwzText, int dwLen, int dwDTFormat, float left, float top, float right, float bottom, int iGlowsize, int crShadom, LPARAM lParam, void* prclayout);
bool _canvas_drawtext(size_t hCanvas, void* hFont, int crText, LPCWSTR lpwzText, int dwLen, int dwDTFormat, float left, float top, float right, float bottom);
bool _canvas_blur(size_t hCanvas, float fDeviation, void* lprc);
bool _canvas_rotate_hue(size_t hCanvas, float fAngle);
size_t _canvas_createfrompwnd(void* pWnd, int width, int height, int dwFlags, int* nError);

void* _cv_dx_bmp(void* pCanvas);
void* _cv_context(void* pCanvas);
