#pragma once
#include "help_ex.h"

struct MosaicRect
{
	RECT rc;
	int		color;
	int		alpha;
};

void _Mosaic_Loading_register();
LRESULT CALLBACK _Mosaic_Loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _Mosaic_Loading_paint(HEXOBJ hObj);