#pragma once
#include "help_ex.h"

// 扩展选择框_背景颜色_按下或选中
#define ECBEP_CRBKGDOWNORCHECKED	1
// 扩展选择框_边框颜色_正常
#define ECBEP_CRBORDERNORMAL	2
// 扩展选择框_边框颜色_悬浮
#define ECBEP_CRBORDERHOVER	3
// 扩展选择框_边框颜色_按下或选中
#define ECBEP_CRBORDERDOWNORCHECKED	4

void _checkbuttonex_register();
LRESULT CALLBACK _checkbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _checkbuttonex_paint(HEXOBJ hObj);