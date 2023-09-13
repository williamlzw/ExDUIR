#pragma once
#include "help_ex.h"

// 扩展编辑框_背景颜色_正常
#define EDITEX_PROP_CRBKGNORMAL 1
// 扩展编辑框_边框颜色_正常
#define EDITEX_PROP_CRBORDERNORMAL 2
// 扩展编辑框_边框颜色_悬浮
#define EDITEX_PROP_CRBORDERHOVER 3
// 扩展编辑框_边框颜色_按下或选中
#define EDITEX_PROP_CRBORDERDOWNORCHECKED 4
// 扩展编辑框_图标颜色_正常
#define EDITEX_PROP_CRICONNORMAL 5
// 扩展编辑框_图标颜色_按下或焦点
#define EDITEX_PROP_CRICONDOWNORFOCUS 6
// 扩展编辑框_圆角度
#define EDITEX_PROP_RADIUS 7
// 扩展编辑框_线宽
#define EDITEX_PROP_STORKEWIDTH 8
// 扩展编辑框_图标位置
#define EDITEX_PROP_ICONPOSITION 9

void _editex_register();
LRESULT CALLBACK _editex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);