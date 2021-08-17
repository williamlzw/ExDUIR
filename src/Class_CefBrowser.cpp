#include "Class_CefBrowser.h"

static INT M_CefInitialize = 0;

Ck_InitializeEx_ Ck_InitializeEx;
Ck_Browser_Create_ Ck_Browser_Create;
Ck_Browser_Close_ Ck_Browser_Close;
Ck_Browser_LoadUrl_ Ck_Browser_LoadUrl;
Ck_Browser_SendMouse_ Ck_Browser_SendMouse;
Ck_Browser_SendKey_ Ck_Browser_SendKey;
Ck_Browser_Focus_ Ck_Browser_Focus;
Ck_Browser_Move_ Ck_Browser_Move;

BOOL Ex_ObjCefBrowserInitialize(LPCWSTR libPath, BOOL singleProcess, LPCWSTR cachePath, LPCWSTR userAgent, INT debuggingPort, CefPROC lpBeforeCommandLine)
{
	int ret = 1;
	if (M_CefInitialize == 0) 
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

		HMODULE Moudule = LoadLibraryW(L"Ex_libcef.dll");
		if (Moudule == 0) {
			return 0;
		}
		
		M_CefInitialize = 1;
		Ck_InitializeEx = (Ck_InitializeEx_)GetProcAddress(Moudule, "Ck_InitializeEx");
		Ck_Browser_Create = (Ck_Browser_Create_)GetProcAddress(Moudule, "Ck_Browser_Create");
		Ck_Browser_Close = (Ck_Browser_Close_)GetProcAddress(Moudule, "Ck_Browser_Close");
		Ck_Browser_LoadUrl = (Ck_Browser_LoadUrl_)GetProcAddress(Moudule, "Ck_Browser_LoadUrl");

		Ck_Browser_SendMouse = (Ck_Browser_SendMouse_)GetProcAddress(Moudule, "Ck_Browser_SendMouse");
		Ck_Browser_SendKey = (Ck_Browser_SendKey_)GetProcAddress(Moudule, "Ck_Browser_SendKey");
		Ck_Browser_Focus = (Ck_Browser_Focus_)GetProcAddress(Moudule, "Ck_Browser_Focus");
		Ck_Browser_Move = (Ck_Browser_Move_)GetProcAddress(Moudule, "Ck_Browser_Move");

		if (singleProcess) {
			ret = Ck_InitializeEx(1, 0, cachePath, userAgent, debuggingPort, lpBeforeCommandLine);
		}
		else {
			std::wstring subprocess_path;
			if (libPath != 0) {
				subprocess_path = std::wstring(libPath) + L"\\CefSubprocess.exe";
			}
			else {
				subprocess_path =  L"CefSubprocess.exe";
			}
			ret = Ck_InitializeEx(0, subprocess_path.c_str(), cachePath, userAgent, debuggingPort, lpBeforeCommandLine);
		}
		return ret == 0;
	}
	return FALSE;
}

void _cefbrowser_setcursor(HEXOBJ hObj, DWORD dwCursorType) {
	if (dwCursorType < 0 || dwCursorType > 44) {
		return;
	}
	std::vector<INT> arrCurs = { 32512, 32515, 32649, 32513, 32514, 32651, 32644, 32645, 32643, 32642, 32645, 32642, 32643, 32644, 32645, 32644, 32643, 32642, 32644, 32645, 32640, 32644, 32644, 32643, 32642, 32645, 32642, 32643, 32644, 32646, 32513, 32512, 32512, 32512, 32650, 32648, 32512, -1, 32648, 32515, 32646, 32512, 32512, 32512 };

	LPCWSTR CursorName = NULL;
	int i = 0;
	for (i = 0; i < arrCurs.size(); i++) {
		if (i == dwCursorType) {
			CursorName = MAKEINTRESOURCE(arrCurs[i]);
			break;
		}
	}
	HCURSOR hCursor = LoadCursorW(0, CursorName);
	Ex_ObjSetLong(hObj, EOL_CURSOR, (LONG_PTR)hCursor);
}

BOOL CALLBACK _cefbrowser_callback(int uMsg, HANDLE handler, int hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, LONG_PTR attach5) {
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
		if (uMsg == 1) {
			Ex_ObjSetLong(hObj, CEFL_INIT, 1);
			Ex_ObjDispatchNotify(hObj, CEFN_CREATE, 0, (LPARAM)handler);

			LPCWSTR nUrl = (LPCWSTR)Ex_ObjGetLong(hObj, CEFL_URL);
			if (nUrl != 0) {
				Ck_Browser_LoadUrl(handler, nUrl);
				Ex_MemFree((LPVOID)nUrl);
				Ex_ObjSetLong(hObj, CEFL_URL, 0);
			}
		}
		else if (uMsg == 2) {
			//绘制
			Ex_ObjSendMessage(hObj, WM_PAINT, attach1, MAKELONG(attach2, attach3));
		}
		else if (uMsg == 3) {
			_cefbrowser_setcursor(hObj, attach1);
		}
		else if (uMsg == 4) {
			Ex_ObjPostMessage(hObj, WM_NEXTDLGCTL, attach1, 99);
		}
		else if (uMsg == 5) {
			int model = attach1;
			int TypeFlags = attach2;
			LPCWSTR LinkUrl = L"";
			LPCWSTR SourceUrl = L"";
			LPCWSTR SelectionText = L"";
			if (attach3 != 0) {
				LinkUrl = (LPCWSTR)attach3;
			}
			if (attach4 != 0) {
				SourceUrl = (LPCWSTR)attach4;
			}
			if (attach5 != 0) {
				SelectionText = (LPCWSTR)attach5;
			}

			return 0;
		}
		else if (uMsg == 6) {
			int command_id = attach1;

		}
	}
	return 0;
}

void _cefbrowser_register() {
	Ex_ObjRegister(L"CefBrowser", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, -1, 4 * sizeof(SIZE_T), 0, 0, _cefbrowser_proc);
}

LRESULT CALLBACK _cefbrowser_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
		HANDLE hWebView = (HANDLE)Ex_ObjGetLong(hObj, CEFL_VIEW);
		INT hInit = Ex_ObjGetLong(hObj, CEFL_INIT);
		if (uMsg == WM_CREATE) {
			Ex_ObjDisableTranslateSpaceAndEnterToClick(hObj, TRUE);
			RECT Rect = { pObj->left_, pObj->top_, pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_ };

			hWebView = Ck_Browser_Create(hWnd, hObj, &Rect, Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND), L"", _cefbrowser_callback);
			if (hWebView) {
				Ex_ObjSetLong(hObj, CEFL_VIEW, (LONG_PTR)hWebView);
			}
		}
		else if (uMsg == WM_DESTROY) {
			Ex_ObjSetLong(hObj, CEFL_INIT, 0);
			Ex_ObjSetLong(hObj, CEFL_VIEW, 0);
			LPCWSTR nUrl = (LPCWSTR)Ex_ObjGetLong(hObj, CEFL_URL);
			if (nUrl != 0) {
				Ex_MemFree((LPVOID)nUrl);
				Ex_ObjSetLong(hObj, CEFL_URL, 0);
			}
			if (hWebView != 0) {
				Ck_Browser_Close(hWebView);
			}
		}
		else if (uMsg == WM_PAINT) {
			if (wParam != 0) {
				EX_PAINTSTRUCT ps{};
				if (Ex_ObjBeginPaint(hObj, &ps)) {
					HEXIMAGE hImg = 0;
					INT nWidth = LOWORD(lParam);
					INT nHeight = HIWORD(lParam);
					_img_createfrompngbits2(nWidth, nHeight, (BYTE*)wParam, &hImg);
					if (hImg != 0) {
						_canvas_drawimage(ps.hCanvas, hImg, 0, 0, 255);
						_img_destroy(hImg);
					}
					Ex_ObjEndPaint(hObj, &ps);
				}
			}
			else {
				return 1;
			}
		}
		else if (uMsg == WM_MOVE) {
			if (hWebView != 0)
			{
				Ck_Browser_Move(hWebView, LOWORD(lParam), HIWORD(lParam), EOP_DEFAULT, EOP_DEFAULT);
			}
		}
		else if (uMsg == WM_SIZE) {
			if (hWebView != 0)
			{
				Ck_Browser_Move(hWebView, EOP_DEFAULT, EOP_DEFAULT, LOWORD(lParam), HIWORD(lParam));
			}
		}
		else if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_MBUTTONDBLCLK) {
			if (uMsg == WM_MOUSEMOVE) {
				Ex_ObjSetLong(hObj, CEFL_lParam, lParam);
			}
			if (hInit == 1) Ck_Browser_SendMouse(hWebView, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_MOUSEWHEEL) {
			if (hInit == 1) Ck_Browser_SendMouse(hWebView, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) {
			if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) {
			if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR) {
			if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_SETFOCUS) {
			if (hInit == 1) Ck_Browser_Focus(hWebView, TRUE);
		}
		else if (uMsg == WM_KILLFOCUS) {
			if (hInit == 1) Ck_Browser_Focus(hWebView, FALSE);
		}
		else if (uMsg == CEFM_GETWEBVIEW) {
			return (LRESULT)hWebView;
		}
		else if (uMsg == WM_NEXTDLGCTL && lParam == 99) {
			Ex_ObjSetIMEState(hObj, wParam == 0);
		}
		else if (uMsg == CEFM_LOADURL) {
			if (hInit != 1) {
				LPVOID nUrl = (LPVOID)Ex_ObjGetLong(hObj, CEFL_URL);
				if (nUrl != 0) {
					Ex_MemFree(nUrl);
				}
				Ex_ObjSetLong(hObj, CEFL_URL, (LONG_PTR)StrDupW((LPCWSTR)lParam));
			}
			else {
				Ck_Browser_LoadUrl(hWebView, (LPCWSTR)lParam);
			}
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}