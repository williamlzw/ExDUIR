#pragma once
#include "help_ex.h"

#define PNG_HEADER 1196314761
#define PNG_IHDR 1380206665
#define PNG_PLTE 1163152464
#define PNG_tRNS 1397641844
#define PNG_acTL 1280598881
#define PNG_fcTL 1280598886
#define PNG_IDAT 1413563465
#define PNG_fdAT 1413571686

#define IMGF_APNG 1

struct lockedbitmapdata_s
{
	int width_;
	int height_;
	int stride_;
	int pixelformat_;
	void* scan0_;
	int dwlen_;
	void* pLock_;
};

struct img_s
{
	int dwFlags_;
	void* pObj_;
	int nMaxFrames_;
	int nCurFrame_;
	void* pWicDecoder_;
	void* lpFrames_;
	void* lpHeader_;
	void* pPrev_;
	void* p_x_;
	void* p_y_;
	void* p_w_;
	void* p_h_;
};

struct bkgimg_s
{
	int dwFlags_;
	ExHandle hImage_;
	int x_;
	int y_;
	int dwRepeat_;
	void* lpGrid_;
	void* lpDelay_;
	int curFrame_;
	int maxFrame_;
	int dwAlpha_;
};

ExHandle _wic_create(int width, int height, GUID pFormat, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
ExHandle _wic_init_from_decoder(void* pDecoder, int* nError);
void _wic_savetobin(void* pBitmap, void* lpBin, size_t* len, int* nError);
void _wic_drawframe(void* pImg, void* pFrame, int* nError);
void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
void* _wic_getpixel(void* pBitmap, int x, int y, int* nError);
bool _wic_getframedelay(void* pDecoder, void* lpDelay, int nCount, int* nError);

bool _img_destroy(ExHandle hImg);
void* _img_getpixel(ExHandle hImg, int x, int y);
bool _img_lock(ExHandle hImg, void* lpRectL, int flags, void* lpLockedBitmapData);
bool _img_unlock(ExHandle hImg, void* lpLockedBitmapData);
bool _img_setpixel(ExHandle hImg, int x, int y, int color);
bool _img_getsize(ExHandle hImg, void* lpWidth, void* lpHeight);
int _img_width(ExHandle hImg);
int _img_height(ExHandle hImg);
ExHandle _img_create(int width, int height);
ExHandle _img_createfrompngbits(void* lpmem);
void* _img_createfromstream_init(void* lpData, int dwLen, int* nError);
ExHandle _img_createfromstream(void* lpStream);
ExHandle _img_createfrommemory(void* lpData, int dwLen);
ExHandle _img_createfromhicon(void* hIcon);
ExHandle _img_createfromfile(LPCWSTR lpwzFilename);
ExHandle _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError);
ExHandle _img_copyrect(ExHandle hImg, int x, int y, int width, int height);
ExHandle _img_copy(ExHandle hImg);
ExHandle _img_scale(ExHandle hImage, int dstWidth, int dstHeight);
size_t _img_savetomemory(ExHandle hImage, void* lpBuffer);
bool _img_getframedelay(ExHandle hImg, void* lpDelayAry, int nFrames);
int _img_getframecount(ExHandle hImage);
void* _img_getcontext(ExHandle hImage);
int _apng_thunk_getlength(void* lpMem);
bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, void** lpThunk, int* dwThunkLen);
void _apng_int(ExHandle hImage, void* lpStream);
bool _apng_getframedelay(void* pImg, void* lpDelay, int nFrames);
bool _img_selectactiveframe(ExHandle hImg, int nIndex);