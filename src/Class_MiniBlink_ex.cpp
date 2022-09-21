#include "Class_MiniBlink_ex.h"

static INT M_MiniblinkInitialize = 0;

BOOL Ex_ObjMiniblinkBrowserInitialize(LPCWSTR libPath, LPCWSTR dllName)
{
	if (M_MiniblinkInitialize == 0)
	{
		if (libPath != 0)
		{
			DWORD nSize = GetEnvironmentVariableW(L"Path", 0, 0);
			if (nSize > 0)
			{
				wchar_t* buffer = new wchar_t[nSize];
				nSize = GetEnvironmentVariableW(L"Path", buffer, nSize);
				if (nSize > 0)
				{
					std::wstring path(buffer, nSize);
					path = path + L";" + libPath;
					SetEnvironmentVariableW(L"Path", path.c_str());
				}
				delete[] buffer;
			}
		}
		if (!dllName)
		{
			dllName = L"miniblink_4975_x64.dll";
		}
		mbSetMbDllPath(dllName);
		mbInit(0);
		M_MiniblinkInitialize = 1;
		return TRUE;
	}
	return FALSE;
}

void _miniblink_register()
{
	Ex_ObjRegister(L"mbBrowser", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, -1, 2 * sizeof(SIZE_T), 0, 0, _miniblink_proc);
}

void CALLBACK _miniblink_onpaint(mbWebView hWebView, LPVOID hObj,const HDC hDC, INT x, INT y, INT cx, INT cy)
{
	cx = cx + x;
	cy = cy + y;
	RECT rc = { x,y,cx,cy };
	Ex_ObjInvalidateRect((HEXOBJ)hObj, &rc);
}

void CALLBACK _miniblink_onpaintbit(mbWebView hWebView, LPVOID hObj, const void* buffer, const mbRect* rect, INT cx, INT cy)
{
	EX_PAINTSTRUCT ps{ 0 };
	Ex_ObjBeginPaint((HEXOBJ)hObj, &ps);
	if (buffer)
	{
		HEXIMAGE hImg;
		if (_img_create(ps.uWidth, ps.uHeight, &hImg))
		{
			EX_BITMAPDATA bd;
			_img_lock(hImg, 0, 3, 925707, &bd);
			RtlMoveMemory(bd.scan0, buffer, ps.uWidth * ps.uHeight * 4);
			_img_unlock(hImg, &bd);
			_canvas_drawimage(ps.hCanvas, hImg, 0, 0, 255);
			_img_destroy(hImg);
		}
	}
	Ex_ObjEndPaint((HEXOBJ)hObj, &ps);
	RECT rc;
	rc.left = rect->x;
	rc.right = rect->x + rect->w;
	rc.top = rect->y;
	rc.bottom = rect->y + rect->h;
	Ex_ObjInvalidateRect((HEXOBJ)hObj, &rc);
}


DWORD _miniblink_geteventflags(WPARAM wParam, BOOL fKeyEvent)
{
	DWORD dwFlags = 0;
	if (fKeyEvent)
	{
		if ((HIWORD(wParam) & KF_REPEAT) == KF_REPEAT)
		{
			dwFlags = dwFlags | MB_REPEAT;
		}
		if ((HIWORD(wParam) & KF_EXTENDED) == KF_EXTENDED)
		{
			dwFlags = dwFlags | MB_REPEAT;
		}
	}
	else {
		if ((wParam & MK_LBUTTON) == MK_LBUTTON)
		{
			dwFlags = dwFlags | MB_LBUTTON;
		}
		if ((wParam & MK_MBUTTON) == MK_MBUTTON)
		{
			dwFlags = dwFlags | MB_MBUTTON;
		}
		if ((wParam & MK_RBUTTON) == MK_RBUTTON)
		{
			dwFlags = dwFlags | MB_RBUTTON;
		}
		if ((wParam & MK_CONTROL) == MK_CONTROL)
		{
			dwFlags = dwFlags | MB_CONTROL;
		}
		if ((wParam & MK_SHIFT) == MK_SHIFT)
		{
			dwFlags = dwFlags | MB_SHIFT;
		}
	}
	return dwFlags;
}

HCURSOR _miniblink_setcursor(HEXOBJ hObj, DWORD dwCursorType)
{
	std::vector<INT> arrCurs = { 0, 32512, 1, 32515, 2, 32649, 3, 32513, 4, 32514, 5, 32651, 6, 32644, 7, 32645, 8, 32643, 9, 32642, 10, 32645, 11, 32642, 12, 32643, 13, 32644, 14, 32645, 15, 32644, 16, 32643, 17, 32642, 18, 32644, 19, 32645, 20, 32640, 21, 32644, 22, 32644, 23, 32643, 24, 32642, 25, 32645, 26, 32642, 27, 32643, 28, 32644, 29, 32646, 30, 32513, 31, 32512, 32, 32512, 33, 32512, 34, 32650, 35, 32648, 36, 32512, 37, -1, 38, 32648, 39, 32515, 40, 32646, 41, 32512, 42, 32512, 43, 32512 };
	LPCWSTR CursorName = NULL;
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
	mbWebView hWebView = (mbWebView)Ex_ObjGetLong(hObj, MBBL_VIEW);
	BOOL fResult = FALSE;
	LRESULT nResult = 0;
	if (uMsg == WM_CREATE)
	{
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hObj, TRUE);
		hWebView = mbCreateWebView();
		if (hWebView)
		{
			mbSetHandle(hWebView, hWnd);
			mbSetAutoDrawToHwnd(hWebView, FALSE);
			Ex_ObjSetLong(hObj, MBBL_VIEW, (LONG_PTR)hWebView);
			Ex_ObjSetLong(hObj, MBBL_ONPAINT, (LONG_PTR)_miniblink_onpaintbit);
			mbOnPaintBitUpdated(hWebView, _miniblink_onpaintbit, (LPVOID)hObj);
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		mbDestroyWebView(hWebView);
	}
	else if (uMsg == WM_SIZE)
	{
		mbResize(hWebView, LOWORD(lParam), HIWORD(lParam));
	}
	//else if (uMsg == WM_PAINT)
	//{
	//	return _miniblink_paint(hObj, hWebView);
	//}
	else if (uMsg == WM_MOVE)
	{
		mbSetHandleOffset(hWebView, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_SETFOCUS)
	{
		Ex_ObjSetIMEState(hObj, TRUE);
		mbSetFocus(hWebView);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		Ex_ObjSetIMEState(hObj, FALSE);
		mbKillFocus(hWebView);
	}
	else if (uMsg == WM_CONTEXTMENU)
	{
		fResult = mbFireContextMenuEvent(hWebView, LOWORD(lParam), HIWORD(lParam), 0);
	}
	else if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_MBUTTONDBLCLK)
	{
		fResult = mbFireMouseEvent(hWebView, uMsg, LOWORD(lParam), HIWORD(lParam), _miniblink_geteventflags(wParam, FALSE));
	}
	else if (uMsg == MB_MSG_MOUSEWHEEL)
	{
		fResult = mbFireMouseWheelEvent(hWebView, LOWORD(lParam), HIWORD(lParam), HIWORD(wParam), _miniblink_geteventflags(LOWORD(wParam), FALSE));
	}
	else if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
	{
		fResult = mbFireKeyDownEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSKEYDOWN);
	}
	else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
	{
		fResult = mbFireKeyUpEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSKEYUP);
	}
	else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR)
	{
		fResult = mbFireKeyPressEvent(hWebView, wParam, _miniblink_geteventflags(lParam, TRUE), uMsg == WM_SYSCHAR);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		INT nType = mbGetCursorInfoType(hWebView);
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
		auto ustr = w2u((LPCWSTR)lParam);
		mbLoadURL(hWebView, ustr.c_str());
		fResult = TRUE;
	}
	else if (uMsg == MBBM_JS)
	{
		if (lParam)
		{
			auto url = std::wstring((LPCWSTR)lParam);
			auto js = L"play(\"" + url + L"\")";
			auto jsStr = w2u(js);
			mbRunJs(hWebView, mbWebFrameGetMainFrame(hWebView), jsStr.c_str(), TRUE, (mbRunJsCallback)wParam, 0, 0);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}