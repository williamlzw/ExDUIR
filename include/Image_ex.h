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
	CHAR dispose_op;// 处理方式
	CHAR blend_op;// 混合模式
};
#pragma pack()

struct img_s
{
	INT dwFlags_;
	LPVOID pObj_;
	INT nMaxFrames_;
	INT nCurFrame_;
	LPVOID pWicDecoder_;
	EX_APNG_THUNK** lpFrames_;
	LPVOID lpHeader_;
	IWICBitmap* pPrev_;
	INT p_x_;
	INT p_y_;
	INT p_w_;
	INT p_h_;
};


HEXIMAGE _img_init(LPVOID pObj, INT curframe, INT frames, LPVOID pDecoder, INT* nError);
BOOL _img_createfrompngbits(LPVOID lpmem, HEXIMAGE* dstImg);
LPSTREAM _img_createfromstream_init(LPVOID lpData, INT dwLen, INT* nError);
LPVOID _img_getcontext(HEXIMAGE hImage);
void _apng_drawframe(img_s* pImage, INT nIndex);
INT _apng_thunk_getlength(LPVOID lpMem);
BOOL _apng_thunk_getnext(LPVOID lpMem, INT* nPos, INT dwThunkType, EX_APNG_THUNK** lpThunk, INT* dwThunkLen);
void _apng_int(HEXIMAGE hImage, LPVOID lpStream);
BOOL _apng_getframedelay(img_s* pImg, INT* lpDelay, INT nFrames);
HEXIMAGE _wic_create(INT width, INT height, GUID pFormat, INT* nError);
HEXIMAGE _wic_init_from_decoder(LPVOID pDecoder, INT* nError);
void _wic_savetobin(LPVOID pBitmap, LPVOID* lpBin, size_t* len, INT* nError);
void _wic_drawframe(img_s* pImg, LPVOID pFrame, INT* nError, D2D1_RECT_F* dest = NULL);
LPVOID _wic_convert(LPVOID pBitmap, BOOL bFreeOld, INT* nError);
LPVOID _wic_selectactiveframe(LPVOID pDecoder, INT nIndex, INT* nError, D2D1_RECT_F* dest);
EXARGB _wic_getpixel(LPVOID pBitmap, INT x, INT y, INT* nError);
BOOL _wic_getframedelay(LPVOID pDecoder, INT* lpDelay, INT nCount, INT* nError);