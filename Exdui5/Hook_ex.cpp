#include "Hook_ex.h"

LRESULT CALLBACK _hook_proc(int code, size_t wParam, size_t lParam)
{
	if (code == 3) //HCBT_CREATEWND
	{
		return _hook_oncreate(code, (HWND)wParam, lParam);
	}
	return CallNextHookEx((HHOOK)g_Li.hHookMsgBox, code, wParam, lParam);
}

LRESULT _hook_oncreate(int code, HWND hWnd, size_t lParam)
{
	auto lpcs = ((CBT_CREATEWND*)(void*)lParam)->lpcs;
	auto atomClass = (int)(lpcs->lpszClass);
	auto hParent = lpcs->hwndParent;

	if (atomClass == 32770)
	{

		auto hExDui = Ex_DUIFromWindow(hParent);
		void* pWnd = nullptr;
		int nError = 0;
		if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
		{
			void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
			((wnd_s*)pWnd)->lpMsgParams_ = 0;
			if (pMsg != 0)
			{
				SetClassLongPtrW(hWnd, -12, (LONG)g_Li.hCursor);
				int style = EWS_TITLE | EWS_BUTTON_CLOSE | EWS_ESCEXIT | EWS_MOVEABLE | EWS_MESSAGEBOX;
				if (__query(pMsg, offsetof(mbp_s, dwFlags_), EMBF_WINDOWICON))
				{
					style = style | EWS_HASICON;
				}
				Ex_DUIBindWindowEx(hWnd, ((wnd_s*)pWnd)->hTheme_, style, (size_t)pMsg, ((mbp_s*)pMsg)->lpfnNotifyCallback_);
			}
		}
	}
	else if (atomClass == 32768)
	{
		Thunkwindow(hWnd, &_menu_proc, 0, 0);
	}
	return CallNextHookEx((HHOOK)g_Li.hHookMsgBox, code, (WPARAM)hWnd, lParam);
}

size_t _menu_proc(void* pData, int uMsg, size_t wParam, size_t lParam)
{
#if defined(_M_IX86)
	HWND hWnd = (HWND)__get_int(pData, 13);
	LONG pOld = (LONG)__get_int(pData, 17);
	void* pWnd = (void*)__get_int(pData, 21);
#elif defined(_M_AMD64)
	HWND hWnd = (HWND)__get(pData, 22);
	LONG64 pOld = (LONG64)__get(pData, 30);
	void* pWnd = (void*)__get(pData, 38);
#endif
	if (uMsg == WM_DESTROY)
	{
		SetWindowLongPtrW(hWnd, -4, pOld);
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	else if (uMsg == 482)//MN_SIZEWINDOW
	{
		_menu_init(hWnd);
	}
	return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
}

void _menu_init(HWND hWnd)
{
	void* hMenu = (void*)SendMessageW(hWnd, 481, 0, 0);
	if (hMenu != 0)
	{
		size_t hExDui;
		HashTable_Get(g_Li.hTableLayout, (size_t)hMenu, &hExDui);
		void* pWnd = nullptr;
		int nError = 0;
		if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
		{
			HashTable_Remove(g_Li.hTableLayout, (size_t)hMenu);
			void* lpMenuParams = ((wnd_s*)pWnd)->lpMenuParams_;
			SetWindowLongPtrW(hWnd, -20, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
			SetClassLongPtrW(hWnd, -26, 1 | 2 | 8);
			int dwStyle = EWS_MENU | EWS_NOINHERITBKG | EWS_ESCEXIT | EWS_FULLSCREEN;
			void* pfnCallback = nullptr;
			if (!IsBadReadPtr(lpMenuParams, sizeof(menu_s)))
			{
				if (__query(lpMenuParams, offsetof(menu_s, dwFlags_), EMNF_NOSHADOW))
				{
					dwStyle = dwStyle | EWS_NOSHADOW;
				}
				pfnCallback = ((menu_s*)lpMenuParams)->pfnCallback_;
			}
			size_t hExDui = Ex_DUIBindWindowEx(hWnd, ((wnd_s*)pWnd)->hTheme_, dwStyle, (size_t)pWnd, pfnCallback);
			if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
			{
				((wnd_s*)pWnd)->lpMenuParams_ = lpMenuParams;
				((wnd_s*)pWnd)->hMenuPopup_ = hMenu;
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_INTED);
			}
		}
	}
}

void _msgbox_drawinfo(void* pWnd, size_t cvBkg)
{
	int w, h;
	_canvas_getsize(cvBkg, &w, &h);
	void* hTheme = ((wnd_s*)pWnd)->hTheme_;
	Ex_ThemeDrawControl(hTheme, cvBkg, 0, 0, w, h, ATOM_MESSAGEBOX, ATOM_RECT, 255);
	void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
	if (pMsg != 0)
	{
		int wType = ((mbp_s*)pMsg)->uType_ & 240;
		int l = Ex_Scale(15);
		int t = Ex_Scale(15);
		void* pCaption = nullptr;
		int nError = 0;
		if (_handle_validate(((wnd_s*)pWnd)->objCaption_, HT_OBJECT, &pCaption, &nError))
		{
			t = t + ((obj_s*)pCaption)->bottom_;
		}

		if (wType != 0)
		{
			std::wstring value;
			value.push_back(wType);
			void* pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, Ex_Atom(value.data()));
			int r, b;
			if (pValue != 0)
			{
				r = l + Ex_Scale(__get_int(pValue, 8) - __get_int(pValue, 0));
				b = t + Ex_Scale(__get_int(pValue, 12) - __get_int(pValue, 4));
				Ex_ThemeDrawControlEx(hTheme, cvBkg, l, t, r, b, ATOM_MESSAGEBOX, Ex_Atom(value.data()), 0, 0, 0, 0, 255);
			}
			l = r + Ex_Scale(15);
		}
		void* pText = ((mbp_s*)pMsg)->lpText_;
		if (pText != 0)
		{
			void* hFont = _font_create();
			if (hFont != 0)
			{
				_canvas_drawtext(cvBkg, hFont, -16777216, (LPCWSTR)pText, -1, DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL, l, t, w - Ex_Scale(15), h);
				_font_destroy(hFont);
			}
		}
	}
}

void _msgbox_initdialog(HWND hWnd, void* pWnd, size_t wParam, size_t lParam)
{
	void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
	if (pMsg == 0) return;
	auto lpwzCheckbox = ((mbp_s*)pMsg)->lpCheckBox_;
	auto lpCheckboxChecked = ((mbp_s*)pMsg)->lpCheckBoxChecked_;
	auto uType = ((mbp_s*)pMsg)->uType_;
	auto pfnCallback = ((mbp_s*)pMsg)->lpfnNotifyCallback_;
	auto hTheme = ((wnd_s*)pWnd)->hTheme_;
	auto hWndChild = GetWindow(hWnd, GW_CHILD);
	RECT rcText;
	std::vector<std::wstring> aryText;
	std::vector<size_t> aryID;
	size_t iDef = 0;
	while (hWndChild != 0)
	{
		auto i = GetWindowLongPtrW(hWndChild, -12);
		if (i == 65535)//内容
		{
			GetWindowRect(hWndChild, &rcText);
		}
		else if (i == 20)//图标
		{

		}
		else {
			aryText.push_back(窗口_取标题(hWndChild));
			aryID.push_back(i);
			if (窗口_查询风格(hWndChild, BS_DEFPUSHBUTTON, false))  iDef = i;
		}
		DestroyWindow(hWndChild);
		hWndChild = GetWindow(hWnd, GW_CHILD);
	}
	auto n = aryID.size();
	int maxWidth = Ex_Scale(10) * 2 + Ex_Scale(80) * n + Ex_Scale(5) * n;
	size_t hCanvas = ((wnd_s*)pWnd)->canvas_bkg_;
	void* hFont = _font_create();
	int width = rcText.right - rcText.left;
	int w, h;
	int widthCheckbox;
	if (hFont != 0 && hCanvas != 0)
	{
		if (lpwzCheckbox != 0)
		{

			if (_canvas_begindraw(hCanvas))
			{

				_canvas_calctextsize(hCanvas, hFont, (LPCWSTR)lpwzCheckbox, -1, DT_LEFT | DT_SINGLELINE, 0, 0, 0, &widthCheckbox, 0);
				_canvas_enddraw(hCanvas);
			}
			widthCheckbox = widthCheckbox + 16 + 8;
			maxWidth = maxWidth + Ex_Scale(widthCheckbox);
		}

		if (_canvas_begindraw(hCanvas))
		{
			_canvas_calctextsize(hCanvas, hFont, (LPCWSTR)((mbp_s*)pMsg)->lpText_, -1, DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL, 0, width, rcText.bottom - rcText.top, &w, &h);
			_canvas_enddraw(hCanvas);
		}
		_font_destroy(hFont);
		if (w > width) width = w;
	}
	if (!Flag_Query(EXGF_DPI_ENABLE))
	{
		width = width + g_Li.DpiX_Real;
	}
	width = width + Ex_Scale(20) * 2;
	std::wstring bin = L"16";
	auto pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, Ex_Atom(bin.c_str()));
	if ((uType & 240) != 0)
	{
		width = width + Ex_Scale(__get_int(pValue, 8) - __get_int(pValue, 0)) + Ex_Scale(15);
	}
	if (width > maxWidth) maxWidth = width;
	int height = Ex_Scale(__get_int(pValue, 12) - __get_int(pValue, 4));
	int maxHeight = h;
	if (maxHeight < height) maxHeight = height;
	pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, ATOM_BACKGROUND_GRID);
	maxHeight = Ex_Scale(__get_int(pValue, 4)) + Ex_Scale(__get_int(pValue, 12)) + maxHeight + Ex_Scale(15) * 2;
	if (maxHeight < Ex_Scale(140)) maxHeight = Ex_Scale(140);
	if (maxWidth < Ex_Scale(220)) maxWidth = Ex_Scale(220);
	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	int left = rcWindow.right - rcWindow.left;
	int top = rcWindow.bottom - rcWindow.top;
	MoveWindow(hWnd, rcWindow.left - (maxWidth - left) / 2, rcWindow.top - (maxHeight - top) / 2, maxWidth, maxHeight, 0);
	if (pfnCallback != 0)
	{
		_wnd_dispatch_notify(hWnd, pWnd, ((wnd_s*)pWnd)->hexdui_, -1, NM_INTDLG, 0, 0, 0);
	}
	width = Ex_Scale(80);
	height = Ex_Scale(24);
	GetWindowRect(hWnd, &rcWindow);
	maxWidth = rcWindow.right - rcWindow.left;
	maxHeight = rcWindow.bottom - rcWindow.top;
	top = maxHeight / g_Li.DpiY - __get_int(pValue, 12) + (__get_int(pValue, 12) - 24) / 2;
	left = maxWidth / g_Li.DpiX - 85;
	ExHandle hObj = 0;
	void* pObj = nullptr;
	int nError = 0;
	for (int i = aryID.size(); i > 1; i--)
	{
		hObj = _obj_create_init(hWnd, pWnd, ATOM_BUTTON, 0, &pObj, &nError);
		if (hObj != 0)
		{
			_obj_create_proc(&nError, true, hTheme, pObj, -1, ATOM_BUTTON, aryText.data(), -1, left, top, 80, 24, 0, ~aryID[i], 0, aryID[i], -1);
			__add(pObj, offsetof(obj_s, dwFlags_), eof_bMsgBoxControl);
			_obj_create_done(hWnd, pWnd, hObj, pObj);
			if (aryID[i] == iDef) _obj_setfocus(hWnd, pWnd, hObj, pObj, true);
		}
		left = left - 65;
	}
	if (lpwzCheckbox != 0)
	{
		hObj = _obj_create_init(hWnd, pWnd, ATOM_CHECKBUTTON, 0, &pObj, &nError);
		if (hObj != 0)
		{
			_obj_create_proc(&nError, true, hTheme, pObj, -1, ATOM_CHECKBUTTON, lpwzCheckbox, -1, 4, top, widthCheckbox, 24, 0, (size_t)lpCheckboxChecked, 0, (size_t)lpCheckboxChecked, -1);
			_obj_create_done(hWnd, pWnd, hObj, pObj);
			if (__get_int(lpCheckboxChecked, 0) != 0)
			{
				_obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, 1, 0);
			}
		}
	}

	int wType = uType & 7;
	if (wType == 2 || wType == 4)
	{
		Ex_ObjEnable(Ex_ObjGetFromID(((wnd_s*)pWnd)->objCaption_, EWS_BUTTON_CLOSE), false);
	}
	if (__query(pMsg, offsetof(mbp_s, dwFlags_), EMBF_CENTEWINDOW))
	{
		Ex_WndCenterFrom(hWnd, (HWND)GetWindowLongPtrW(hWnd, -8), false);
	}
	else {
		Ex_WndCenterFrom(hWnd, 0, false);//检查是否同个屏幕
	}
	__add(pWnd, offsetof(mbp_s, dwFlags_), EWF_INTED);
	ShowWindow(hWnd, 1);
	InvalidateRect(hWnd, 0, false);
	UpdateWindow(hWnd);
}