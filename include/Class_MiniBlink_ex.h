#pragma once
#include "help_ex.h"
#include "mb.h"

// mibiblink浏览框属性_浏览框句柄
#define MBBL_VIEW 0
// mibiblink浏览框属性_绘画函数地址
#define MBBL_ONPAINT 1

void _miniblink_register();
LRESULT CALLBACK _miniblink_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK _miniblink_onpaint(mbWebView hWebView, LPVOID hObj,const HDC hDC, INT x, INT y, INT cx, INT cy);
DWORD _miniblink_geteventflags(WPARAM wParam, BOOL fKeyEvent);
HCURSOR _miniblink_setcursor(HEXOBJ hObj, DWORD dwCursorType);
