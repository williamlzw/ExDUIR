#pragma once

// 开关属性_背景颜色_普通
#define SWITCH_PROP_CRBKGNORMAL 1
// 开关属性_背景颜色_悬浮
#define SWITCH_PROP_CRBKGHOVER 2
// 开关属性_背景颜色_按下或选中
#define SWITCH_PROP_CRBKGDOWNORCHECKED 3
// 开关属性_边框颜色_普通
#define SWITCH_PROP_CRBORDERNORMAL 4
// 开关属性_边框颜色_悬浮
#define SWITCH_PROP_CRBORDERHOVER 5
// 开关属性_边框颜色_按下或选中
#define SWITCH_PROP_CRBORDERDOWNORCHECKED 6
// 开关属性_圆角度
#define SWITCH_PROP_RADIUS 7
// 开关属性_线宽
#define SWITCH_PROP_STROKEWIDTH 8

void             _switch_register();
LRESULT CALLBACK _switch_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void             _switch_paint(HEXOBJ hObj);
