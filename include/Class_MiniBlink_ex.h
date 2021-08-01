#pragma once
#include "help_ex.h"
#include "wke.h"
#pragma comment(lib, "Msimg32.lib")

void _miniblink_register();
LRESULT CALLBACK _miniblink_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void WKE_CALL_TYPE _miniblink_OnPaint(wkeWebView hWebView, LPVOID hObj,const HDC hDC, INT x, INT y, INT cx, INT cy);
LRESULT _miniblink_paint(HEXOBJ hObj, wkeWebView hWebView);
DWORD _miniblink_geteventflags(WPARAM wParam, BOOL fKeyEvent);
HCURSOR _miniblink_setcursor(HEXOBJ hObj, DWORD dwCursorType);
