#include "Class_MiniBlink_ex.h"

void _miniblink_register()
{
	wkeInitialize();
	Ex_ObjRegister(L"mbBrowser", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, -1, 2 * sizeof(SIZE_T), 0, 0, _miniblink_proc);
}

void WKE_CALL_TYPE _miniblink_OnPaint(wkeWebView hWebView, LPVOID hObj,const HDC hDC, INT x, INT y, INT cx, INT cy)
{
	cx = cx + x;
	cy = cy + y;
	RECT rc = { x,y,cx,cy };
	Ex_ObjInvalidateRect((HEXOBJ)hObj, &rc);
}

LRESULT _miniblink_paint(HEXOBJ hObj, wkeWebView hWebView)
{
	EX_PAINTSTRUCT ps{};
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		INT nWidth = ps.rcPaint.right - ps.rcPaint.left;
		INT nHeight = ps.rcPaint.bottom - ps.rcPaint.top;
		HDC hDCWebView = wkeGetViewDC(hWebView);
		HDC hDCObj = _canvas_getdc(ps.hCanvas);
		BLENDFUNCTION blendfunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(hDCObj, ps.rcPaint.left, ps.rcPaint.top, nWidth, nHeight, hDCWebView, ps.rcPaint.left, ps.rcPaint.top, nWidth, nHeight, blendfunction);
		_canvas_releasedc(ps.hCanvas);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}

DWORD _miniblink_geteventflags(WPARAM wParam, BOOL fKeyEvent)
{
	DWORD dwFlags = 0;
	if (fKeyEvent)
	{
		if ((HIWORD(wParam) & KF_REPEAT) == KF_REPEAT)
		{
			dwFlags = dwFlags | WKE_REPEAT;
		}
		if ((HIWORD(wParam) & KF_EXTENDED) == KF_EXTENDED)
		{
			dwFlags = dwFlags | WKE_REPEAT;
		}
	}
	else {
		if ((wParam & MK_LBUTTON) == MK_LBUTTON)
		{
			dwFlags = dwFlags | WKE_LBUTTON;
		}
		if ((wParam & MK_MBUTTON) == MK_MBUTTON)
		{
			dwFlags = dwFlags | WKE_MBUTTON;
		}
		if ((wParam & MK_RBUTTON) == MK_RBUTTON)
		{
			dwFlags = dwFlags | WKE_RBUTTON;
		}
		if ((wParam & MK_CONTROL) == MK_CONTROL)
		{
			dwFlags = dwFlags | WKE_CONTROL;
		}
		if ((wParam & MK_SHIFT) == MK_SHIFT)
		{
			dwFlags = dwFlags | WKE_SHIFT;
		}
	}
	return dwFlags;
}

HCURSOR _miniblink_setcursor(HEXOBJ hObj, DWORD dwCursorType)
{
	std::vector<INT> arrCurs = { 0, 32512, 1, 32515, 2, 32649, 3, 32513, 4, 32514, 5, 32651, 6, 32644, 7, 32645, 8, 32643, 9, 32642, 10, 32645, 11, 32642, 12, 32643, 13, 32644, 14, 32645, 15, 32644, 16, 32643, 17, 32642, 18, 32644, 19, 32645, 20, 32640, 21, 32644, 22, 32644, 23, 32643, 24, 32642, 25, 32645, 26, 32642, 27, 32643, 28, 32644, 29, 32646, 30, 32513, 31, 32512, 32, 32512, 33, 32512, 34, 32650, 35, 32648, 36, 32512, 37, -1, 38, 32648, 39, 32515, 40, 32646, 41, 32512, 42, 32512, 43, 32512 };
	LPCWSTR CursorName;
	for (int i = 0; i < arrCurs.size() / 2; i++)
	{
		if (dwCursorType = arrCurs[i * 2])
		{
			CursorName = MAKEINTRESOURCE(arrCurs[i * 2 + 1]);
			break;
		}
	}
	HCURSOR hCursor = 0;
	if ((INT)CursorName == -1)
	{
		CursorName = 0;
	}
	if (CursorName != 0)
	{
		hCursor = LoadCursorW(0, CursorName);
		SetCursor(hCursor);
	}
	return hCursor;
}

LRESULT CALLBACK _miniblink_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	wkeWebView hWebView = (wkeWebView)Ex_ObjGetLong(hObj, MBBL_VIEW);
	BOOL fResult = FALSE;
	LRESULT nResult = 0;
	if (uMsg == WM_CREATE)
	{
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hObj, TRUE);
		hWebView = wkeCreateWebView();
		if (hWebView)
		{
			Ex_ObjSetLong(hObj, MBBL_VIEW, (LONG_PTR)hWebView);
			Ex_ObjSetLong(hObj, MBBL_ONPAINT, (LONG_PTR)_miniblink_OnPaint);
			wkeOnPaintUpdated(hWebView, _miniblink_OnPaint, (LPVOID)hObj);
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		wkeDestroyWebView(hWebView);
	}
	else if (uMsg == WM_SIZE)
	{
		wkeResize(hWebView, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_PAINT)
	{
		return _miniblink_paint(hObj, hWebView);
	}
	else if (uMsg == WM_MOVE)
	{
		wkeSetHandleOffset(hWebView, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_SETFOCUS)
	{
		Ex_ObjSetIMEState(hObj, TRUE);
		wkeSetFocus(hWebView);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		Ex_ObjSetIMEState(hObj, FALSE);
		wkeKillFocus(hWebView);
	}
	else if (uMsg == WM_CONTEXTMENU)
	{
		fResult = wkeFireContextMenuEvent(hWebView, LOWORD(lParam), HIWORD(lParam), 0);
	}
	else if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_MBUTTONDBLCLK)
	{
		fResult = wkeFireMouseEvent(hWebView, uMsg, LOWORD(lParam), HIWORD(lParam), _miniblink_geteventflags(wParam, FALSE));
	}
	else if (uMsg == WKE_MSG_MOUSEWHEEL)
	{
		fResult = wkeFireMouseWheelEvent(hWebView, LOWORD(lParam), HIWORD(lParam), HIWORD(wParam), _miniblink_geteventflags(LOWORD(wParam), FALSE));
	}
	else if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
	{
		fResult = wkeFireKeyDownEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSKEYDOWN);
	}
	else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
	{
		fResult = wkeFireKeyUpEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSKEYUP);
	}
	else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR)
	{
		fResult = wkeFireKeyPressEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSCHAR);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		INT nType = wkeGetCursorInfoType(hWebView);
		nResult =(LRESULT) _miniblink_setcursor(hObj, nType);
		fResult = TRUE;
	}
	else if (uMsg == MBBM_GETWEBVIEW)
	{
		fResult = TRUE;
		nResult = (LRESULT)hWebView;
	}
	else if (uMsg == MBBM_LOAD)
	{
		if (wParam == MBBL_TYPE_URL)
		{
			wkeLoadURLW(hWebView, (LPCWSTR)lParam);
		}
		else if (wParam == MBBL_TYPE_FILE)
		{
			wkeLoadFileW(hWebView, (LPCWSTR)lParam);
		}
		else if (wParam == MBBL_TYPE_HTML)
		{
			wkeLoadHTMLW(hWebView, (LPCWSTR)lParam);
		}
		fResult = TRUE;
	}
	if (!fResult || nResult == 0)
	{
		nResult = Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
	}
	return nResult;
}