#pragma once
#include "help_ex.h"

// 开关属性_背景颜色_普通
#define ESP_CRBKGNORMAL	1
// 开关属性_背景颜色_点燃
#define ESP_CRBKGHOVER	2
// 开关属性_背景颜色_按下或选中
#define ESP_CRBKGDOWNORCHECKED	3
// 开关属性_边框颜色_普通
#define ESP_CRBORDERNORMAL	4
// 开关属性_边框颜色_点燃
#define ESP_CRBORDERHOVER	5
// 开关属性_边框颜色_按下或选中
#define ESP_CRBORDERDOWNORCHECKED	6
// 开关属性_圆角度
#define ESP_RADIUS 7
// 开关属性_线宽
#define ESP_STROKEWIDTH 8


void _switch_register();
LRESULT CALLBACK _switch_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT	_switch_paint(HEXOBJ hObj);