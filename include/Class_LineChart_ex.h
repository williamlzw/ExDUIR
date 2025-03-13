#pragma once

// 属性_折线图_数组指针
#define LINECHART_LONG_DATA 0


void _linechart_register();
LRESULT CALLBACK _linechart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _linechart_paint(HEXOBJ hObj);
void _linechart_update_polygon(EX_POLYGON* ptr, int newY);