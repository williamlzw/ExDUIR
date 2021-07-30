#pragma once
#include "help_ex.h"

struct MosaicRect
{
	RECT rc;
	INT color;
	INT alpha;
};

void _mosaic_loading_register();
LRESULT CALLBACK _mosaic_loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _mosaic_loading_paint(HEXOBJ hObj);