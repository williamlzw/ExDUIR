#include "test_webview2.h"

HEXOBJ hObj_webview2;

LRESULT CALLBACK OnWebViewWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_SIZE) {
	
		if (hObj_webview2 != 0)
		{
			Ex_ObjMove(hObj_webview2, 1, 0, (LOWORD(lParam) - 2),
				(HIWORD(lParam) ), FALSE);
		}
	}
	return 0;
}

void test_webview2(HWND hWnd)
{
	HWND hWnd_webview2 = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试webview2浏览器", 0, 0, 1100, 800, 0, 0);
	HEXDUI hExDui_webview2 = Ex_DUIBindWindowEx(hWnd_webview2, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
		WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_SIZEABLE |
		WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON,
		0, OnWebViewWndMsgProc);

	Ex_DUISetLong(hExDui_webview2, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    hObj_webview2 = Ex_ObjCreate(L"WebView", 0, -1, 1, 0, 1100 - 2, 800 , hExDui_webview2);
	Ex_DUIShowWindow(hExDui_webview2, SW_SHOWNORMAL);
}