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
	size_t hImage_;
	int x_;
	int y_;
	int dwRepeat_;
	void* lpGrid_;
	void* lpDelay_;
	int curFrame_;
	int maxFrame_;
	int dwAlpha_;
};

size_t _wic_create(int width, int height,GUID pFormat, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
size_t _wic_init_from_decoder(void* pDecoder, int* nError);
void _wic_savetobin(void* pBitmap, void* lpBin, size_t* len, int* nError);
void _wic_drawframe(void* pImg, void* pFrame, int* nError);
void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError);
void* _wic_getpixel(void* pBitmap, int x, int y, int* nError);
bool _wic_getframedelay(void* pDecoder, void* lpDelay, int nCount, int* nError);

bool _img_destroy(size_t hImg);
void* _img_getpixel(size_t hImg, int x, int y);
bool _img_lock(size_t hImg, void* lpRectL, int flags, void* lpLockedBitmapData);
bool _img_unlock(size_t hImg, void* lpLockedBitmapData);
bool _img_setpixel(size_t hImg, int x, int y, int color);
bool _img_getsize(size_t hImg, void* lpWidth, void* lpHeight);
int _img_width(size_t hImg);
int _img_height(size_t hImg);
size_t _img_create(int width, int height);
size_t _img_createfrompngbits(void* lpmem);
void* _img_createfromstream_init(void* lpData, int dwLen, int* nError);
size_t _img_createfromstream(void* lpStream);
size_t _img_createfrommemory(void* lpData, int dwLen);
size_t _img_createfromhicon(void* hIcon);
size_t _img_createfromfile(LPCWSTR lpwzFilename);
size_t _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError);
size_t _img_copyrect(size_t hImg, int x, int y, int width, int height);
size_t _img_copy(size_t hImg);
size_t _img_scale(size_t hImage, int dstWidth, int dstHeight);
size_t _img_savetomemory(size_t hImage, void* lpBuffer);
bool _img_getframedelay(size_t hImg, void* lpDelayAry, int nFrames);
int _img_getframecount(size_t hImage);
void* _img_getcontext(size_t hImage);
int _apng_thunk_getlength(void* lpMem);
bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, void** lpThunk, int* dwThunkLen);
void _apng_int(size_t hImage, void* lpStream);
bool _apng_getframedelay(void* pImg, void* lpDelay, int nFrames);
bool _img_selectactiveframe(size_t hImg, int nIndex);