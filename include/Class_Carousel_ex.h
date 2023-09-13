#pragma once
#include "help_ex.h"

#define CAROUSEL_LONG_HIMAGELIST 0
#define CAROUSEL_LONG_WIDTH 1
#define CAROUSEL_LONG_HEIGHT 2
#define CAROUSEL_LONG_INDEX 3

void _carousel_register();
LRESULT CALLBACK _carousel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _carousel_paint(HEXOBJ hObj);
void _carousel_drawcircle(HEXCANVAS canvas, INT x, INT y, INT radius, EXARGB color);