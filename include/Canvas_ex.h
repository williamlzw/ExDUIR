#pragma once
#include "help_ex.h"
#include "nanosvgrast.h"


struct canvas_s
{
	INT dwFlags_;
	INT width_;
	INT height_;
	ID2D1DeviceContext *pContext_;
	ID2D1GdiInteropRenderTarget *pGdiInterop_;
	ID2D1Bitmap *pBitmap_;
	wnd_s *pWnd_;
	INT c_left_;
	INT c_top_;
	INT c_right_;
	INT c_bottom_;
};

BOOL _md_create(LPVOID pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, INT width, INT height, INT *nError);
BOOL _md_destroy(LPVOID pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits);
BOOL _canvas_drawtextwitheffect(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hrText, LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, HEXBRUSH hrShadom, LPARAM lParam);
void _canvas_recreate(canvas_s *pCanvas, INT width, INT height, INT *nError);
void _canvas_init(INT *nError);
void _canvas_uninit();
BOOL _canvas_drawimagefrombkgimg_ex(HEXCANVAS hCanvas, HEXIMAGE hImage, INT x, INT y, INT dwRepeat, RECT *lpGrid, INT dwFlags, INT dwAlpha, RECT *lpRcSrc, EX_RECTF *lpRCFDst);
BOOL _canvas_drawimagefrombkgimg(HEXCANVAS hCanvas, EX_BACKGROUNDIMAGEINFO *lpBkgImg);
BOOL _canvas_bitblt(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight, INT dstBottom, INT srcLeft, INT srcTop);
BOOL _canvas_alphablend(HEXCANVAS hCanvas, HEXCANVAS sCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT alpha);
BOOL _canvas_calctextsize_ex(canvas_s *pCanvas, font_s *pFont, LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT *lpWidth, FLOAT *lpHeight, IDWriteTextLayout** pLayout, INT *nError);
BOOL _canvas_drawtextex2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBursh, LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, EXARGB crShadom, LPARAM lParam);
HEXCANVAS _canvas_createfrompwnd(wnd_s *pWnd, INT width, INT height, INT dwFlags, INT *nError);
HDC _canvas_getdc_ex(canvas_s *pCanvas, INT *nError);
void _canvas_releasedc_ex(canvas_s *pCanvas, INT *nError);