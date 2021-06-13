#pragma once
#include "help_ex.h"

struct ARGB_s
{
	INT r;
	INT g;
	INT b;
	INT a;
};

EXATOM _fmt_getatom(LPVOID lpValue, LPVOID* lpValueOffset);
INT _fmt_intary_ex(LPVOID lpValue, LPVOID* lpAry, INT nMax, BOOL fPercentFlags);
INT _fmt_intary(LPVOID lpValue, LPVOID lpAry, INT nMaxCount, BOOL fZero, LPVOID lpdwPercentFlags);
BOOL _fmt_color(LPVOID lpValue, LPVOID lpColor);
LPVOID _fmt_int(LPVOID lpValue, LPVOID lpdwPercentFlags);
BOOL _fmt_getvalue(LPVOID* lpValue, INT atomDest);
BOOL _fmt_bin(HEXRES hRes, LPVOID lpValue, LPVOID* lpBin, size_t* lpLen, BOOL* lpFreeBuffer);
