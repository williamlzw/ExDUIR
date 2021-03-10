#pragma once
#include "help_ex.h"
#include "DirectX_ex.h"

bool _rgn_destroy(void* hRgn);
void* _rgn_createfromroundrect(float left, float top, float right, float bottom, float radiusX, float radiusY);
void* _rgn_createfromrect(float left, float top, float right, float bottom);
void* _rgn_combine(void* hRgnSrc, void* hRgnDst, int nCombineMode, int dstOffsetX, int dstOffsetY);
bool _rgn_hittest(void* hRgn, float x, float y);
