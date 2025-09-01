#pragma once

// 扩展单选框_背景颜色_按下或选中
#define RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED 1
// 扩展单选框_边框颜色_正常
#define RADIOBUTTONEX_PROP_CRBORDERNORMAL 2
// 扩展单选框_边框颜色_悬浮
#define RADIOBUTTONEX_PROP_CRBORDERHOVER 3
// 扩展单选框_边框颜色_按下或选中
#define RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED 4
// 扩展单选框_属性_状态 0正常 1点燃 2选中
#define RADIOBUTTONEX_LONG_STATE 0

void             _radiobuttonex_register();
LRESULT CALLBACK _radiobuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                     LPARAM lParam);
void             _radiobuttonex_paint(HEXOBJ hObj);
