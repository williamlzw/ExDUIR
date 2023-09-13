#pragma once
#include "help_ex.h"

// 扩展单选框_背景颜色_按下或选中
#define RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED 1
// 扩展单选框_边框颜色_正常
#define RADIOBUTTONEX_PROP_CRBORDERNORMAL 2
// 扩展单选框_边框颜色_悬浮
#define RADIOBUTTONEX_PROP_CRBORDERHOVER 3
// 扩展单选框_边框颜色_按下或选中
#define RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED 4

void _radiobuttonex_register();
LRESULT CALLBACK _radiobuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _radiobuttonex_paint(HEXOBJ hObj);