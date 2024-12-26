#pragma once

BOOL                         _dx_init(INT* nError);
void                         _dx_uninit();
ID2D1GdiInteropRenderTarget* _dx_get_gdiInterop(ID2D1DeviceContext* prt);
ID2D1Bitmap* _dx_createbitmap(ID2D1DeviceContext* pDeviceContext, INT width, INT height,
                              INT* nError);
void         _dx_settarget(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap);
void         _dx_getsize(ID2D1Bitmap* pBitmap, FLOAT* width, FLOAT* height);
ID2D1Bitmap* _dx_gettarget(ID2D1DeviceContext* pDeviceContext);
void         _dx_begindraw(ID2D1DeviceContext* pDeviceContext);
INT          _dx_enddraw(ID2D1DeviceContext* pDeviceContext);
void         _dx_flush(ID2D1DeviceContext* pDeviceContext);
void         _dx_clear(ID2D1DeviceContext* pDeviceContext, EXARGB Color);
BOOL _dx_createeffect(ID2D1DeviceContext* pDeviceContext, IID peffectId, ID2D1Effect** peffect,
                      INT* nError);
void _dx_blur(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT fDeviation,
              RECT* lprc, INT* nError);
void _dx_rotate_hue(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT fAngle,
                    INT* nError);
void _dx_cliprect(ID2D1DeviceContext* pDeviceContext, FLOAT left, FLOAT top, FLOAT right,
                  FLOAT bottom);
void _dx_resetclip(ID2D1DeviceContext* pDeviceContext);
void _dx_drawbitmaprect(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT dstLeft,
                        FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, INT dwAlpha);
void _dx_drawbitmaprectrect(ID2D1DeviceContext* pDeviceContext, ID2D1Bitmap* pBitmap, FLOAT dstLeft,
                            FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft,
                            FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, INT dwAlpha);
void _dx_drawimage(ID2D1DeviceContext* pDeviceContext, img_s* pImage, FLOAT left, FLOAT top,
                   INT mode);
void _dx_drawimage_ex(ID2D1DeviceContext* pDeviceContext, img_s* pImage, FLOAT dstLeft,
                      FLOAT dstTop, FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom,
                      INT mode);
void _dx_bmp_copyfrom(ID2D1Bitmap** pDestBitmap, ID2D1Bitmap* pSrcBitmap, INT dX, INT dY,
                      INT srcLeft, INT srcTop, INT srcRight, INT srcBottom);
void ARGB2ColorF(EXARGB argb, D2D1_COLOR_F* color);
void ColorF2ARGB(D2D1_COLOR_F color, EXARGB* argb);
