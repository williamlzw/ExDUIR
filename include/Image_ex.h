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

#define IMGF_APNG 1

#pragma pack(1)
struct EX_APNG_THUNK {
	DWORD dwLen;
	DWORD Type;
	UINT sequence_number;// 序列
	UINT width;// 宽度
	UINT height;// 高度
	UINT x_offset;// 水平偏移
	UINT y_offset;// 垂直偏移
	USHORT delay_num;// 为这一帧显示时间的以秒为单位的分子
	USHORT delay_den;// 为这一帧显示时间以秒为单位的分母
	char dispose_op;// 处理方式
	char blend_op;// 混合模式
};
#pragma pack()

struct img_s
{
	int dwFlags_;
	void* pObj_;
	int nMaxFrames_;
	int nCurFrame_;
	void* pWicDecoder_;
	EX_APNG_THUNK** lpFrames_;
	void* lpHeader_;
	void* pPrev_;
	int p_x_;
	int p_y_;
	int p_w_;
	int p_h_;
};


HEXIMAGE _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError);
bool _img_createfrompngbits(void* lpmem, HEXIMAGE* dstImg);
void* _img_createfromstream_init(void* lpData, int dwLen, int* nError);
void* _img_getcontext(HEXIMAGE hImage);
void _apng_drawframe(img_s* pImage, int nIndex);
int _apng_thunk_getlength(void* lpMem);
bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, EX_APNG_THUNK** lpThunk, int* dwThunkLen);
void _apng_int(HEXIMAGE hImage, void* lpStream);
bool _apng_getframedelay(img_s* pImg, int* lpDelay, int nFrames);
HEXIMAGE _wic_create(int width, int height, GUID pFormat, int* nError);
HEXIMAGE _wic_init_from_decoder(void* pDecoder, int* nError);
void _wic_savetobin(void* pBitmap, void** lpBin, size_t* len, int* nError);
void _wic_drawframe(img_s* pImg, void* pFrame, int* nError, D2D1_RECT_F* dest = NULL);
void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError);
void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError, D2D1_RECT_F* dest);
EXARGB _wic_getpixel(void* pBitmap, int x, int y, int* nError);
bool _wic_getframedelay(void* pDecoder, int* lpDelay, int nCount, int* nError);