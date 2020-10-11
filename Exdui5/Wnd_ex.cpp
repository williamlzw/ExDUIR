#include "Wnd_ex.h"

LRESULT Wnd_DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor)
{
	WNDCLASSEXW WndClass = {};
	WndClass.cbSize = sizeof(WNDCLASSEXW);
	WndClass.style = 1 | 2 | 8;
	WndClass.lpfnWndProc = (WNDPROC)Wnd_DefWindowProcW;
	WndClass.hInstance = g_Li.hInstance;
	WndClass.hCursor = (hCursor == NULL ? g_Li.hCursor : hCursor);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WndClass.hIcon = (hIcon == NULL ? g_Li.hIcon : hIcon);
	WndClass.hIconSm = (hIconsm == NULL ? g_Li.hIconsm : hIconsm);
	WndClass.lpszClassName = lpwzClassName;
	return RegisterClassExW(&WndClass);
}

size_t Ex_DUIFromWindow(HWND hWnd)
{
	return SendMessageW(hWnd, g_Li.dwMessage, EMV_HEXDUI, 合并整数(EMT_DUI, 0));
}

bool _wnd_getfromhandle(size_t handle, HWND* hWnd, void** pWnd, void** pObj, bool* isObject, int* nError)
{
	*isObject = false;
	//hWnd, pWnd ,pObj= 0;
	if (IsWindow((HWND)handle)) handle = Ex_DUIFromWindow((HWND)handle);
	if (_handle_validate(handle, HT_DUI, pWnd, nError))
	{
		*hWnd = ((wnd_s*)pWnd)->hWnd_;
		pObj = pWnd;
	}
	else if (_handle_validate(handle, HT_OBJECT, pObj, nError))
	{
		*pWnd = ((obj_s*)pObj)->pwnd_;
		*hWnd= ((wnd_s*)pWnd)->hWnd_;
		*isObject = true;
	}
	else {
		return false;
	}
	return true;
}

int _wnd_dispatch_notify(HWND hWnd, void* pWnd, size_t hObj, int nID,int nCode, size_t wParam, size_t lParam,void* pObj)
{
	auto pfnMsgProc = ((wnd_s*)pWnd)->pfnMsgProc_;
	int ret = 1;
	if (pfnMsgProc != 0)
	{
		((MsgPROC)pfnMsgProc)(hWnd, ((wnd_s*)pWnd)->hexdui_, WM_NOTIFY, nID, &hObj, &ret);
	}
	return ret;
}

void _wnd_redraw_bkg(HWND hWnd, void* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate)
{
	if (bRedrawBkg)
	{
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRedrawBackground);
	}
	InvalidateRect(hWnd, (RECT*)lpRect, 0);
	if (bUpdate)
	{
		UpdateWindow(hWnd);
	}
}

bool 窗口_查询风格(HWND hWnd, int dwStyle, bool bExStyle)
{
	return (GetWindowLongW(hWnd, bExStyle ? -20 : -16) & dwStyle) != 0;
}

size_t 窗口_取图标句柄(HWND hWnd, bool 大图标)
{
	size_t ret = SendMessageW(hWnd, 127, (大图标 ? 1 : 0), 0);
	if (ret == 0)
	{
		ret = GetClassLongW(hWnd, 大图标 ? -14 : -34);
	}
	return ret;
}