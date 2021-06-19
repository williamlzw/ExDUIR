#pragma once
#include "help_ex.h"

//扩展按钮_热点图标
#define EBEL_IMG_NORMAL	0
//扩展按钮_热点图标
#define EBEL_IMG_HOVER	1
//扩展按钮_按下图标
#define EBEL_IMG_DOWNORCHECKED	2
//扩展按钮_按下图标
#define EBEL_RECT	3
//扩展按钮_图标
#define EBEL_ICON	4

void _buttonex_register();
LRESULT CALLBACK _buttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _buttonex_paint(HEXOBJ hObj);