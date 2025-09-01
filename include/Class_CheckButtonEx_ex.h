#pragma once

// 扩展选择框_背景颜色_按下或选中
#define CHECKBOXEX_PROP_CRBKGDOWNORCHECKED 1
// 扩展选择框_边框颜色_正常
#define CHECKBOXEX_PROP_CRBORDERNORMAL 2
// 扩展选择框_边框颜色_悬浮
#define CHECKBOXEX_PROP_CRBORDERHOVER 3
// 扩展选择框_边框颜色_按下或选中
#define CHECKBOXEX_PROP_CRBORDERDOWNORCHECKED 4

// 扩展选择框_属性_状态 0正常 1点燃 2选中
#define CHECKBOXEX_LONG_STATE 0

void             _checkbuttonex_register();
LRESULT CALLBACK _checkbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                     LPARAM lParam);
void             _checkbuttonex_paint(HEXOBJ hObj);
