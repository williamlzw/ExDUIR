#pragma once

// 属性_折线图_数组指针
#define LINECHART_LONG_DATA 0
// 属性_折线图_背景线条颜色
#define LINECHART_LONG_BACKGROUNDCOLOR 1
// 属性_折线图_边缘前景色
#define LINECHART_LONG_FOREGROUNDCOLOR_FILL 2
// 属性_折线图_线条前景色
#define LINECHART_LONG_FOREGROUNDCOLOR_DRAW 3

void _linechart_register();
LRESULT CALLBACK _linechart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _linechart_paint(HEXOBJ hObj);
void _linechart_update_polygon(EX_POLYGON* ptr, int newY);
