#pragma once
#include "help_ex.h"

int _fmt_getatom(void* lpValue, void** lpValueOffset);
int _fmt_intary_ex(void* lpValue, void** lpAry, int nMax, bool fPercentFlags);
int _fmt_intary(void* lpValue, void* lpAry, int nMaxCount, bool fZero, void* lpdwPercentFlags);
bool _fmt_color(void* lpValue, void* lpColor);
