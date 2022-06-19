#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define PNG_HEADER 1196314761
#define PNG_IHDR 1380206665
#define PNG_PLTE 1163152464
#define PNG_tRNS 1397641844
#define PNG_acTL 1280598881
#define PNG_fcTL 1280598886
#define PNG_IDAT 1413563465
#define PNG_fdAT 1413571686

struct img_s
{
    INT dwFlags_;
    IWICBitmapSource* pBitmapSource_;
    INT nMaxFrames_;
    INT nCurFrame_;
    IWICBitmapDecoder *pWicDecoder_;
    LPVOID lpHeader_;
    IWICBitmap *pPrev_;
    INT p_x_;
    INT p_y_;
    INT p_w_;
    INT p_h_;
};

HEXIMAGE _img_init(IWICBitmapSource* pObj, INT curframe, INT frames, IWICBitmapDecoder *pDecoder, INT *nError);
BOOL _img_createfrompngbits(LPVOID lpmem, HEXIMAGE *dstImg);
BOOL _img_createfrompngbits2(INT nWidth, INT nHeight, BYTE* pbBuffer, HEXIMAGE* dstImg);
LPSTREAM _img_createfromstream_init(LPVOID lpData, INT dwLen, INT *nError);
LPVOID _img_getcontext(HEXIMAGE hImage);
HEXIMAGE _wic_create(INT width, INT height, GUID pFormat, INT *nError);
HEXIMAGE _wic_init_from_decoder(IWICBitmapDecoder *pDecoder, INT *nError);
void _wic_savetobin(IWICBitmapSource *pBitmapSource, LPVOID *lpBin, size_t *len, INT *nError);
void _wic_drawframe(img_s *pImg, IWICBitmapSource *pFrame, INT *nError, D2D1_RECT_F *dest = NULL);
IWICBitmap *_wic_convert(IWICBitmapSource *pBitmapSource, BOOL bFreeOld, INT *nError);
HEXIMAGE _wic_convert_to_png(IWICBitmapSource* pBitmapSource, INT* nError);
IWICBitmap *_wic_selectactiveframe(IWICBitmapDecoder *pDecoder, INT nIndex, INT *nError, D2D1_RECT_F *dest);
EXARGB _wic_getpixel(IWICBitmapSource *pBitmap, INT x, INT y, INT *nError);
BOOL _wic_getframedelay(IWICBitmapDecoder *pDecoder, INT *lpDelay, INT nCount, INT *nError);