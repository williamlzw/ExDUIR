#pragma once
#include "ExDUIR_Func.h"

void test_webview2(HWND hWnd);
LRESULT CALLBACK OnWebViewWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult);
