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
	EXHANDLE hImage_;
	int x_;
	int y_;
	int dwRepeat_;
	void* lpGrid_;
	void* lpDelay_;
	int curFrame_;
	int maxFrame_;
	int dwAlpha_;
};

EXHANDLE _wic_create(int width, int height, GUID pFormat, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
EXHANDLE _wic_init_from_decoder(void* pDecoder, int* nError);
void _wic_savetobin(void* pBitmap, void* lpBin, size_t* len, int* nError);
void _wic_drawframe(void* pImg, void* pFrame, int* nError);
void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
void* _wic_getpixel(void* pBitmap, int x, int y, int* nError);
bool _wic_getframedelay(void* pDecoder, void* lpDelay, int nCount, int* nError);

bool _img_destroy(EXHANDLE hImg);
void* _img_getpixel(EXHANDLE hImg, int x, int y);
bool _img_lock(EXHANDLE hImg, void* lpRectL, int flags, void* lpLockedBitmapData);
bool _img_unlock(EXHANDLE hImg, void* lpLockedBitmapData);
bool _img_setpixel(EXHANDLE hImg, int x, int y, int color);
bool _img_getsize(EXHANDLE hImg, void* lpWidth, void* lpHeight);
int _img_width(EXHANDLE hImg);
int _img_height(EXHANDLE hImg);
EXHANDLE _img_create(int width, int height);
EXHANDLE _img_createfrompngbits(void* lpmem);
void* _img_createfromstream_init(void* lpData, int dwLen, int* nError);
EXHANDLE _img_createfromstream(void* lpStream);
EXHANDLE _img_createfrommemory(void* lpData, int dwLen);
EXHANDLE _img_createfromhicon(void* hIcon);
EXHANDLE _img_createfromfile(LPCWSTR lpwzFilename);
EXHANDLE _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError);
EXHANDLE _img_copyrect(EXHANDLE hImg, int x, int y, int width, int height);
EXHANDLE _img_copy(EXHANDLE hImg);
EXHANDLE _img_scale(EXHANDLE hImage, int dstWidth, int dstHeight);
size_t _img_savetomemory(EXHANDLE hImage, void* lpBuffer);
bool _img_getframedelay(EXHANDLE hImg, void* lpDelayAry, int nFrames);
int _img_getframecount(EXHANDLE hImage);
void* _img_getcontext(EXHANDLE hImage);
int _apng_thunk_getlength(void* lpMem);
bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, void** lpThunk, int* dwThunkLen);
void _apng_int(EXHANDLE hImage, void* lpStream);
bool _apng_getframedelay(void* pImg, void* lpDelay, int nFrames);
bool _img_selectactiveframe(EXHANDLE hImg, int nIndex);