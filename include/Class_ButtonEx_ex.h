#pragma once
#include "help_ex.h"

//扩展按钮_热点图标
#define BEL_IMG_NORMAL	0
//扩展按钮_热点图标
#define BEL_IMG_HOVER	1
//扩展按钮_按下图标
#define BEL_IMG_DOWNORCHECKED	2
//扩展按钮_按下图标
#define BEL_RECT	3
//扩展按钮_图标
#define BEL_ICON	4

void _ButtonEx_register();
LRESULT CALLBACK _ButtonEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
int _ButtonEx_paint(HEXOBJ hObj);