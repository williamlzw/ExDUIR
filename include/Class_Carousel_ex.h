#pragma once
#include "help_ex.h"

#define ECL_HIMAGELIST 0
#define ECL_WIDTH 1
#define ECL_HEIGHT 2
#define ECL_INDEX 3

void _carousel_register();
LRESULT CALLBACK _carousel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _carousel_paint(HEXOBJ hObj);
void _carousel_drawcircle(HEXCANVAS canvas, INT x, INT y, INT radius, EXARGB color);