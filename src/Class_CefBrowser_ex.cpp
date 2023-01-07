#include "Class_CefBrowser_ex.h"

static INT M_CefInitialize = 0;

EX_DECLEAR_API(Ck_Init);
EX_DECLEAR_API(Ck_Browser_Init);
EX_DECLEAR_API(Ck_WCharToChar);
EX_DECLEAR_API(Ck_CharToWChar);
EX_DECLEAR_API(Ck_RunMessageLoop);
EX_DECLEAR_API(Ck_Shutdown);
EX_DECLEAR_API(Ck_QuitMessageLoop);
EX_DECLEAR_API(Ck_Browser_Create);
EX_DECLEAR_API(Ck_Browser_Close);
EX_DECLEAR_API(Ck_Browser_LoadUrl);
EX_DECLEAR_API(Ck_Browser_SendMouse);
EX_DECLEAR_API(Ck_Browser_SendKey);
EX_DECLEAR_API(Ck_Browser_Focus);
EX_DECLEAR_API(Ck_Browser_Move);
EX_DECLEAR_API(Ck_Browser_GoBack);
EX_DECLEAR_API(Ck_Browser_GoForward);
EX_DECLEAR_API(Ck_Browser_IsBack);
EX_DECLEAR_API(Ck_Browser_IsForward);
EX_DECLEAR_API(Ck_Browser_IsLoading);
EX_DECLEAR_API(Ck_Browser_ReloadIgnoreCache);
EX_DECLEAR_API(Ck_Browser_StopLoad);
EX_DECLEAR_API(Ck_Browser_Release);
EX_DECLEAR_API(Ck_Browser_ExecJs);
EX_DECLEAR_API(Ck_Browser_Refresh);
EX_DECLEAR_API(Ck_Browser_GetUrl);
EX_DECLEAR_API(Ck_Browser_GetName);
EX_DECLEAR_API(Ck_Browser_GetFrameCount);
EX_DECLEAR_API(Ck_Browser_GetMframe);
EX_DECLEAR_API(Ck_Browser_GetFframe);
EX_DECLEAR_API(Ck_Browser_GetWebcode);
EX_DECLEAR_API(Ck_Browser_GetWebText);
EX_DECLEAR_API(Ck_Browser_GetIdentifier);
EX_DECLEAR_API(Ck_Browser_GetFrame);
EX_DECLEAR_API(Ck_Browser_StartDownload);
EX_DECLEAR_API(Ck_Browser_IsSame);

EX_DECLEAR_API(Ck_Frame_LoadUrl);
EX_DECLEAR_API(Ck_Frame_GetUrl);
EX_DECLEAR_API(Ck_Frame_ExecJs);
EX_DECLEAR_API(Ck_Frame_GetName);
EX_DECLEAR_API(Ck_Frame_GetWebcode);
EX_DECLEAR_API(Ck_Frame_GetWebText);
EX_DECLEAR_API(Ck_Frame_GetParent);
EX_DECLEAR_API(Ck_Frame_IsFocused);
EX_DECLEAR_API(Ck_Frame_IsMain);
EX_DECLEAR_API(Ck_Frame_ViewSource);

EX_DECLEAR_API(Ck_Browser_EnumAllCookies);
EX_DECLEAR_API(Ck_Browser_SetCookie);
EX_DECLEAR_API(Ck_Browser_DeleteCookies);
EX_DECLEAR_API(Ck_Browser_FlushStore);


EX_DECLEAR_API(Ck_V8CreateString);
EX_DECLEAR_API(Ck_V8CreateInt);
EX_DECLEAR_API(Ck_V8CreateFunction);
EX_DECLEAR_API(Ck_V8CreateObject);
EX_DECLEAR_API(Ck_V8GetStringValue);
EX_DECLEAR_API(Ck_V8SetValue);
EX_DECLEAR_API(Ck_V8IsFunction);
EX_DECLEAR_API(Ck_V8IsString);
EX_DECLEAR_API(Ck_V8IsInt);
EX_DECLEAR_API(Ck_V8GetIntValue);
EX_DECLEAR_API(Ck_V8ExecuteFunction_Int);
EX_DECLEAR_API(Ck_V8ExecuteFunction_String);
EX_DECLEAR_API(Ck_V8SetValueEx);
EX_DECLEAR_API(Ck_V8SetValueindex);
EX_DECLEAR_API(Ck_V8IsObject);
EX_DECLEAR_API(Ck_V8IsNull);

EX_DECLEAR_API(Ck_V8CGetGlobal);
EX_DECLEAR_API(Ck_V8CEnter);
EX_DECLEAR_API(Ck_V8CExit);
EX_DECLEAR_API(Ck_V8CEval);
EX_DECLEAR_API(Ck_V8CIsSame);

EX_DECLEAR_API(Ck_RegisterExtension);
EX_DECLEAR_API(Ck_RegisterExtensionEx);
EX_DECLEAR_API(Ck_CommandLine_AppendSwitch);
EX_DECLEAR_API(Ck_CommandLine_AppendSwitchWithValue);
EX_DECLEAR_API(Ck_CommandLine_GetSwitchValue);
EX_DECLEAR_API(Ck_CommandLine_GetProgram);
EX_DECLEAR_API(Ck_CommandLine_HasSwitch);
EX_DECLEAR_API(Ck_CommandLine_HasSwitches);

HMODULE _cefbrowser_init_(LPCWSTR wzDllName)
{
	HMODULE hModule;
	hModule = ::LoadLibraryW(wzDllName);
	if (hModule)
	{
		EX_GET_API(Ck_Init);
		EX_GET_API(Ck_Browser_Init);
		EX_GET_API(Ck_WCharToChar);
		EX_GET_API(Ck_CharToWChar);
		EX_GET_API(Ck_RunMessageLoop);
		EX_GET_API(Ck_Shutdown);
		EX_GET_API(Ck_QuitMessageLoop);
		EX_GET_API(Ck_Browser_Create);
		EX_GET_API(Ck_Browser_Close);
		EX_GET_API(Ck_Browser_LoadUrl);
		EX_GET_API(Ck_Browser_SendMouse);
		EX_GET_API(Ck_Browser_SendKey);
		EX_GET_API(Ck_Browser_Focus);
		EX_GET_API(Ck_Browser_Move);
		EX_GET_API(Ck_Browser_GoBack);
		EX_GET_API(Ck_Browser_GoForward);
		EX_GET_API(Ck_Browser_IsBack);
		EX_GET_API(Ck_Browser_IsForward);
		EX_GET_API(Ck_Browser_IsLoading);
		EX_GET_API(Ck_Browser_ReloadIgnoreCache);
		EX_GET_API(Ck_Browser_StopLoad);
		EX_GET_API(Ck_Browser_Release);
		EX_GET_API(Ck_Browser_ExecJs);
		EX_GET_API(Ck_Browser_Refresh);
		EX_GET_API(Ck_Browser_GetUrl);
		EX_GET_API(Ck_Browser_GetName);
		EX_GET_API(Ck_Browser_GetFrameCount);
		EX_GET_API(Ck_Browser_GetMframe);
		EX_GET_API(Ck_Browser_GetFframe);
		EX_GET_API(Ck_Browser_GetWebcode);
		EX_GET_API(Ck_Browser_GetWebText);
		EX_GET_API(Ck_Browser_GetIdentifier);
		EX_GET_API(Ck_Browser_GetFrame);
		EX_GET_API(Ck_Browser_StartDownload);
		EX_GET_API(Ck_Browser_IsSame);

		EX_GET_API(Ck_Frame_LoadUrl);
		EX_GET_API(Ck_Frame_GetUrl);
		EX_GET_API(Ck_Frame_ExecJs);
		EX_GET_API(Ck_Frame_GetName);
		EX_GET_API(Ck_Frame_GetWebcode);
		EX_GET_API(Ck_Frame_GetWebText);
		EX_GET_API(Ck_Frame_GetParent);
		EX_GET_API(Ck_Frame_IsFocused);
		EX_GET_API(Ck_Frame_IsMain);
		EX_GET_API(Ck_Frame_ViewSource);

		EX_GET_API(Ck_Browser_EnumAllCookies);
		EX_GET_API(Ck_Browser_SetCookie);
		EX_GET_API(Ck_Browser_DeleteCookies);
		EX_GET_API(Ck_Browser_FlushStore);


		EX_GET_API(Ck_V8CreateString);
		EX_GET_API(Ck_V8CreateInt);
		EX_GET_API(Ck_V8CreateFunction);
		EX_GET_API(Ck_V8CreateObject);
		EX_GET_API(Ck_V8GetStringValue);
		EX_GET_API(Ck_V8SetValue);
		EX_GET_API(Ck_V8IsFunction);
		EX_GET_API(Ck_V8IsString);
		EX_GET_API(Ck_V8IsInt);
		EX_GET_API(Ck_V8GetIntValue);
		EX_GET_API(Ck_V8ExecuteFunction_Int);
		EX_GET_API(Ck_V8ExecuteFunction_String);
		EX_GET_API(Ck_V8SetValueEx);
		EX_GET_API(Ck_V8SetValueindex);
		EX_GET_API(Ck_V8IsObject);
		EX_GET_API(Ck_V8IsNull);

		EX_GET_API(Ck_V8CGetGlobal);
		EX_GET_API(Ck_V8CEnter);
		EX_GET_API(Ck_V8CExit);
		EX_GET_API(Ck_V8CEval);
		EX_GET_API(Ck_V8CIsSame);

		EX_GET_API(Ck_RegisterExtension);
		EX_GET_API(Ck_RegisterExtensionEx);
		EX_GET_API(Ck_CommandLine_AppendSwitch);
		EX_GET_API(Ck_CommandLine_AppendSwitchWithValue);
		EX_GET_API(Ck_CommandLine_GetSwitchValue);
		EX_GET_API(Ck_CommandLine_GetProgram);
		EX_GET_API(Ck_CommandLine_HasSwitch);
		EX_GET_API(Ck_CommandLine_HasSwitches);
	}
	return hModule;
}

BOOL Ex_ObjCefBrowserInitialize(HMODULE hModule, LPCWSTR libPath, LPCWSTR dllName, LPCWSTR cachePath, LPCWSTR userAgent, INT debuggingPort, void* lpBeforeCommandLine)
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
		if (!dllName)
		{
			dllName = L"FTBrowser.dll";
		}
		_cefbrowser_init_(dllName);


		ret = Ck_Browser_Init(hModule, 1, NULL, cachePath, userAgent, debuggingPort, lpBeforeCommandLine, NULL);
		BOOL is_ok = ret == -1;
		if (is_ok)
		{
			M_CefInitialize = 1;
		}
		return is_ok;
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

BOOL CALLBACK _cefbrowser_callback(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam) 
{
	if (uMsg == type_BCreated) 
	{
		Ex_ObjSetLong(hObj, CEFL_INIT, 1);
		Ex_ObjDispatchNotify(hObj, CEFN_CREATE, 0, (LPARAM)handler);

		LPCWSTR nUrl = (LPCWSTR)Ex_ObjGetLong(hObj, CEFL_URL);
		if (nUrl != 0) {
			Ck_Browser_LoadUrl((HWEBVIEW)handler, nUrl);
			Ex_MemFree((LPVOID)nUrl);
			Ex_ObjSetLong(hObj, CEFL_URL, 0);
		}
	}
	else if (uMsg == type_browserdraw) 
	{
		//绘制
		Ex_ObjSendMessage(hObj, WM_PAINT, attach1, MAKELONG(attach2, attach3));
	}
	else if (uMsg == type_Setcursor) 
	{
		_cefbrowser_setcursor(hObj, attach1);
	}
	else if (uMsg == type_KeyboardRequested) 
	{
		Ex_ObjPostMessage(hObj, WM_NEXTDLGCTL, attach1, 99);
	}
	else if (uMsg == type_MenuCreated) 
	{
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
		return 0;
	}
	else if (uMsg == type_MenuCommand) 
	{
		int command_id = attach1;
	}
	else if (uMsg == type_LoadEnd)
	{
		Ex_ObjDispatchNotify(hObj, CEFN_LOADEND, 0, (LPARAM)handler);
	}
	else if (uMsg == type_LoadStart)
	{
		Ex_ObjDispatchNotify(hObj, CEFN_LOADSTART, 0, (LPARAM)handler);
	}
	return 0;
}

void _cefbrowser_register() {
	Ex_ObjRegister(L"CefBrowser", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, -1, 4 * sizeof(size_t), 0, 0, _cefbrowser_proc);
}

LRESULT CALLBACK _cefbrowser_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	HANDLE hWebView = (HANDLE)Ex_ObjGetLong(hObj, CEFL_VIEW);
	INT hInit = Ex_ObjGetLong(hObj, CEFL_INIT);
	if (uMsg == WM_CREATE) 
	{
		Ex_ObjDisableTranslateSpaceAndEnterToClick(hObj, TRUE);
		RECT Rect;
		Ex_ObjGetRect(hObj, &Rect);
		hWebView = Ck_Browser_Create(hWnd, hObj, &Rect, Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND), L"", _cefbrowser_callback, NULL);
		if (hWebView) {
			Ex_ObjSetLong(hObj, CEFL_VIEW, (LONG_PTR)hWebView);
		}
	}
	else if (uMsg == WM_DESTROY) 
	{
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
	else if (uMsg == WM_PAINT) 
	{
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
	else if (uMsg == WM_MOVE) 
	{
		if (hWebView != 0)
		{
			Ck_Browser_Move(hWebView, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), EOP_DEFAULT, EOP_DEFAULT);
		}
	}
	else if (uMsg == WM_SIZE) 
	{
		if (hWebView != 0)
		{
			Ck_Browser_Move(hWebView, EOP_DEFAULT, EOP_DEFAULT, LOWORD(lParam), HIWORD(lParam));
		}
	}
	else if (uMsg >= WM_MOUSEMOVE && uMsg <= WM_MBUTTONDBLCLK) 
	{
		if (uMsg == WM_MOUSEMOVE) {
			Ex_ObjSetLong(hObj, CEFL_lParam, lParam);
		}
		if (hInit == 1) Ck_Browser_SendMouse(hWebView, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_MOUSEWHEEL) 
	{
		if (hInit == 1) Ck_Browser_SendMouse(hWebView, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) 
	{
		if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) 
	{
		if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CHAR || uMsg == WM_SYSCHAR) 
	{
		if (hInit == 1) Ck_Browser_SendKey(hWebView, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SETFOCUS) 
	{
		if (hInit == 1) Ck_Browser_Focus(hWebView, TRUE);
	}
	else if (uMsg == WM_KILLFOCUS) 
	{
		if (hInit == 1) Ck_Browser_Focus(hWebView, FALSE);
	}
	else if (uMsg == CEFM_GETWEBVIEW) 
	{
		return (LRESULT)hWebView;
	}
	else if (uMsg == WM_NEXTDLGCTL && lParam == 99) 
	{
		Ex_ObjSetIMEState(hObj, wParam == 0);
	}
	else if (uMsg == CEFM_LOADURL) 
	{
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
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}