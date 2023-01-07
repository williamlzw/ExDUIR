#pragma once
#include "help_ex.h"
// Cef 浏览框属性_浏览框句柄
#define CEFL_VIEW 0
// Cef 浏览框属性_是否已创建
#define CEFL_INIT 1
// Cef 浏览框属性_初始URL
#define CEFL_URL 2
// Cef 浏览框属性_移动坐标
#define CEFL_lParam 3

void _cefbrowser_register();
BOOL CALLBACK _cefbrowser_callback(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam);
LRESULT CALLBACK _cefbrowser_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);