#pragma once
#include "help_ex.h"

// 超级列表属性_表头字体
#define ESLVL_FONTHEADER 0
// 超级列表属性_表项字体
#define ESLVL_FONTITEM 1
// 超级列表属性_表头高度
#define ESLVL_HEADERHEIGHT 2
// 超级列表属性_表头背景色
#define ESLVL_HEADERBACKGROUNDCOLOR 3
// 超级列表属性_表项高度
#define ESLVL_ITEMHEIGHT 4
// 超级列表属性_内部列表框
#define ESLVL_LISTVIEW 5

void _superlistview_register();
LRESULT CALLBACK _superlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);