#include "Wnd_ex.h"

LRESULT CALLBACK Wnd_DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor)
{
	 WNDCLASSEXW WndClass;
	WndClass.cbSize = sizeof(WNDCLASSEXW);
	WndClass.style = CS_VREDRAW | CS_HREDRAW ;//| CS_DBLCLKS
	WndClass.lpfnWndProc = Wnd_DefWindowProcW;
	WndClass.hInstance = g_Li.hInstance;
	WndClass.hCursor = (hCursor == NULL ? g_Li.hCursor : hCursor);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //NULL_BRUSH
	WndClass.hIcon = (hIcon == NULL ? g_Li.hIcon : hIcon);
	WndClass.hIconSm = (hIconsm == NULL ? g_Li.hIconsm : hIconsm);// (hIconsm == NULL ? g_Li.hIconsm : hIconsm);
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
	if (IsWindow((HWND)handle)) handle = Ex_DUIFromWindow((HWND)handle);
	if (_handle_validate(handle, HT_DUI, pWnd, nError))
	{
		*hWnd = ((wnd_s*)pWnd)->hWnd_;
		pObj = pWnd;
	}
	else if (_handle_validate(handle, HT_OBJECT, pObj, nError))
	{
		*pWnd = ((obj_s*)pObj)->pwnd_;
		*hWnd = ((wnd_s*)pWnd)->hWnd_;
		*isObject = true;
	}
	else {
		return false;
	}
	return true;
}

int _wnd_dispatch_notify(HWND hWnd, void* pWnd, size_t hObj, int nID, int nCode, size_t wParam, size_t lParam, void* pObj)
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
	return (GetWindowLongPtrW(hWnd, bExStyle ? -20 : -16) & dwStyle) != 0;
}

bool 窗口_删除风格(HWND hWnd, int dwStyle, bool bExStyle)
{
	int nIndex = bExStyle ? -20 : -16;
	auto ret = GetWindowLongPtrW(hWnd, nIndex);
	if ((ret & dwStyle) != 0)
	{
		SetWindowLongPtr(hWnd, nIndex, (ret & ~dwStyle));
		return true;
	}
	return false;
}

bool 窗口_添加风格(HWND hWnd, int dwStyle, bool bExStyle)
{
	int nIndex = bExStyle ? -20 : -16;
	auto ret = GetWindowLongPtrW(hWnd, nIndex);
	if ((ret & dwStyle) == 0)
	{
		SetWindowLongPtr(hWnd, nIndex, (ret & dwStyle));
		return true;
	}
	return false;
}

size_t 窗口_默认回调(HWND hWnd, int uMsg, size_t wParam, size_t lParam)
{
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
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

void* Thunkwindow(HWND hWnd, void* pfnProc, void* dwData, int* nError)
{
#if defined(_M_IX86)
	void* lpData = VirtualAlloc(0, 25, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#elif defined(_M_AMD64)
	void* lpData = VirtualAlloc(0, 46, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#endif
	if (lpData != 0)
	{
		std::vector<UCHAR> data;
		std::vector<UCHAR> value;
#if defined(_M_IX86)
		data = { 199, 68, 36, 4 };
		value = 到字节数组((ULONG)lpData);
		data.insert(data.end(), value.begin(), value.end());
		data.push_back((UCHAR)233);
		value = 到字节数组((ULONG)((ULONG)pfnProc - (ULONG)lpData - 13));
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG)hWnd);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组(GetWindowLongPtrW(hWnd, -4));
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG)dwData);
		data.insert(data.end(), value.begin(), value.end());
		RtlMoveMemory(lpData, data.data(), 25);
		FlushInstructionCache(GetCurrentProcess(), lpData, 25);
#elif defined(_M_AMD64)
		value = 到字节数组((USHORT)0xb948);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG64)lpData);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((USHORT)0xb848);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG64)pfnProc);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((USHORT)0xe0ff);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG64)hWnd);
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG64)GetWindowLongPtrW(hWnd, -4));
		data.insert(data.end(), value.begin(), value.end());
		value = 到字节数组((ULONG64)dwData);
		data.insert(data.end(), value.begin(), value.end());
		FlushInstructionCache(GetCurrentProcess(), lpData, 46);//22+24
#endif
		SetWindowLongPtr(hWnd, -4, (LONG_PTR)lpData);
	}
	else {
		*nError = GetLastError();
	}
	return lpData;
}

bool 窗口_取屏幕矩形(HWND hWnd, RECT* rcMonitor, RECT* rcDesk)
{
	bool ret = false;
	auto hMonitor = MonitorFromWindow(hWnd, 2);
	if (hMonitor != 0)
	{
		MONITORINFO info = {};
		if (GetMonitorInfoW(hMonitor, &info))
		{
			*rcMonitor = info.rcMonitor;
			*rcDesk = info.rcWork;
			ret = true;
		}
	}
	if (!ret)
	{
		Ex_SetLastError(GetLastError());
	}
	return ret;
}

std::wstring 窗口_取标题(HWND hWnd)//OK
{
	auto len = SendMessageW(hWnd, WM_GETTEXTLENGTH, 0, 0);
	len = (len + 1) * 2;
	std::wstring ret;
	ret.resize(len);
	SendMessageW(hWnd, WM_GETTEXT, len, (LPARAM)(ret.data()));
	return ret;
}

int Wnd_ClassToAtom(HWND hWnd)//OK
{
	std::wstring ret;
	ret.resize(64);
	GetClassNameW(hWnd, (LPWSTR)ret.data(), 64);
	return Ex_Atom(ret.data());
}

int 窗口_取功能键() //OK
{
	int ret = 0;
	if ((GetAsyncKeyState(VK_CONTROL) & 32768) != 0)
	{
		ret = ret | 1;
	}
	if ((GetAsyncKeyState(VK_SHIFT) & 32768) != 0)
	{
		ret = ret | 2;
	}
	if ((GetAsyncKeyState(VK_MENU) & 32768) != 0)
	{
		ret = ret | 4;
	}
	return ret;
}

void IME_Control(HWND hWnd, void* pWnd, bool bEnable)
{
	ImmAssociateContext(hWnd, (bEnable ? ((wnd_s*)pWnd)->hImc_ : 0));
}

void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, bool bFullScreen)
{
	RECT rcWindow, rcParent;
	GetWindowRect(hWnd, &rcWindow);
	if (hWndFrom == 0)
	{
		if (bFullScreen)
		{
			窗口_取屏幕矩形(hWnd, &rcParent);
		}
		else {
			窗口_取屏幕矩形(hWnd, {}, &rcParent);
		}
	}
	else {
		GetWindowRect(hWndFrom, &rcParent);
	}
	SetWindowPos(hWnd, 0, rcParent.left + (rcParent.right - rcParent.left - (rcWindow.right - rcWindow.left)) / 2, rcParent.top + (rcParent.bottom - rcParent.top - (rcWindow.bottom - rcWindow.top)) / 2, 0, 0, SWP_NOSIZE);
}

size_t Ex_WndMsgLoop()
{
	MSG ret = {};
	while (GetMessageW(&ret, 0, 0, 0))
	{
		TranslateMessage(&ret);
		DispatchMessageW(&ret);
	}
	return ret.wParam;
}

HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, int x, int y, int width, int height, int dwStyle, int dwStyleEx)
{
	if (dwStyle == 0) dwStyle = WS_OVERLAPPEDWINDOW;
	if (dwStyleEx == 0) dwStyleEx = 768;
	dwStyleEx = dwStyleEx | WS_EX_LAYERED;
	if (lpwzClassName == 0) lpwzClassName = (LPCWSTR)g_Li.atomClassName;
	HINSTANCE hInst = g_Li.hInstance;
	if (IsWindow(hWndParent)) hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, -6);
	HWND hWnd = CreateWindowExW(dwStyleEx, lpwzClassName, lpwzWindowName, dwStyle, x, y, width, height, hWndParent, NULL, hInst, NULL);
	if (hWnd != 0)
	{
		SendMessageW(hWnd, 128, 0, (LPARAM)g_Li.hIconsm);
		SendMessageW(hWnd, 128, 1, (LPARAM)g_Li.hIcon);
	}
	return hWnd;
}

void _wnd_recalcclient(void* pWnd, HWND hWnd, int width, int height)
{
	((wnd_s*)pWnd)->width_ = width;
	((wnd_s*)pWnd)->height_ = height;
	if (_canvas_resize(((wnd_s*)pWnd)->canvas_display_, width, height))
	{
		if (_canvas_resize(((wnd_s*)pWnd)->canvas_bkg_, width, height))//客户区被改变,必定触发背景被改变事件
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRedrawBackground);
		}
	}
	_rgn_destroy(((wnd_s*)pWnd)->hrgn_client_);
	_rgn_destroy(((wnd_s*)pWnd)->hrgn_sizebox_);
	int nSizebox = 0;
	if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_SIZEABLE))
	{
		nSizebox = 5;
	}
	auto rectround = ((wnd_s*)pWnd)->radius_;
	void* hRgnClient = _rgn_createfromroundrect(nSizebox, nSizebox, width - nSizebox, height - nSizebox, rectround, rectround);
	((wnd_s*)pWnd)->hrgn_client_ = hRgnClient;
	if (nSizebox != 0)
	{
		void* hRgnNC = _rgn_createfromrect(0, 0, width, height);
		void* hRgnSizebox = _rgn_combine(hRgnNC, hRgnClient, RGN_COMBINE_EXCLUDE, 0, 0);
		_rgn_destroy(hRgnNC);
		((wnd_s*)pWnd)->hrgn_sizebox_ = hRgnSizebox;
	}
	if (rectround != 0)//gdi object
	{
		auto hRgn = CreateRoundRectRgn(0, 0, width, height, rectround, rectround);
		SetWindowRgn(hWnd, hRgn, 1);
		DeleteObject(hRgn);
	}
}

bool _wnd_wm_stylechanging(void* pWnd, HWND hWnd, size_t wParam, size_t lParam)
{
	bool ret = false;
	int styleNew = __get_int((void*)lParam, 4);
	int styleDui = ((wnd_s*)pWnd)->dwStyle_;
	if (wParam == GWL_EXSTYLE)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLayered);
		}
		else {
			if (__query((void*)lParam, 4, WS_EX_LAYERED))
			{
				__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLayered);
			}
			else {
				if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLayered))
				{
					__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLayered);
				}
			}
		}
		InvalidateRect(hWnd, 0, 0);
	}
	else {
		styleNew = (styleNew & ~(WS_DLGFRAME | WS_BORDER));
		if ((styleDui & EWS_BUTTON_MIN) == 0 || (styleDui & EWS_FULLSCREEN) != 0)
		{
			styleNew = (styleNew & ~WS_MINIMIZEBOX);
		}
		else {
			styleNew = styleNew | WS_MINIMIZEBOX;
		}

		if ((styleDui & EWS_BUTTON_MAX) == 0 || (styleDui & EWS_FULLSCREEN) != 0)
		{
			styleNew = (styleNew & ~WS_MAXIMIZEBOX);
		}
		else {
			styleNew = styleNew | WS_MAXIMIZEBOX;
		}

		if ((styleDui & EWS_FULLSCREEN) != 0)
		{
			styleNew = (styleNew & ~WS_SYSMENU);
		}
		else {
			styleNew = styleNew | WS_SYSMENU;
		}

		if ((styleDui & EWS_SIZEABLE) != 0)
		{
			styleNew = styleNew | WS_THICKFRAME;
		}
		else {
			styleNew = styleNew & ~WS_THICKFRAME;
		}

		if (styleNew != __get_int((void*)lParam, 4))
		{
			__set_int((void*)lParam, 4, styleNew);
			ret = true;
		}
	}
	return ret;
}

void _wnd_loadtheme(void* pWnd, HWND hWnd, void* hTheme)
{
	int atom = __query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU) ? ATOM_MENU : ATOM_WINDOW;
	void* pPADDING_SHADOW = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_PADDING_SHADOW);
	
	//system("pause");
	if (pPADDING_SHADOW != 0)
	{
		((wnd_s*)pWnd)->padding_shadow_ = pPADDING_SHADOW;
		((wnd_s*)pWnd)->minWidth_ = Ex_Scale(__get_int(pPADDING_SHADOW, 0) + __get_int(pPADDING_SHADOW, 8));
		((wnd_s*)pWnd)->minHeight_ = Ex_Scale(__get_int(pPADDING_SHADOW, 4) + __get_int(pPADDING_SHADOW, 8));
	}
	void* pSIZE = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);
	
	if (pSIZE != 0)
	{
		((wnd_s*)pWnd)->size_caption_ = pSIZE;
	}
	void* pMARGIN = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_MARGIN);
	
	if (pMARGIN != 0)
	{
		((wnd_s*)pWnd)->margin_caption_ = pMARGIN;
	}
	void* pRADIUS = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_RADIUS);
	
	if (pRADIUS != 0)
	{
		((wnd_s*)pWnd)->radius_ = __get_int(pRADIUS, 0);
	}
	std::cout << "_wnd_loadtheme->pRADIUS:" << pRADIUS << ",__get_int(pRADIUS, 0):" << __get(pRADIUS, 0) << std::endl;
	void* pBACKGROUND_COLOR = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_BACKGROUND_COLOR);
	
	if (pBACKGROUND_COLOR != 0)
	{
		((wnd_s*)pWnd)->crBkg_ = __get_int(pBACKGROUND_COLOR, 0);
	}
	//菜单
	void* pPADDING_CLIENT = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_CLIENT);
	if (pPADDING_CLIENT != 0)
	{
		((wnd_s*)pWnd)->padding_client_ = pPADDING_CLIENT;
	}
	void* pPADDING_TEXT = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_TEXT);
	if (pPADDING_TEXT != 0)
	{
		((wnd_s*)pWnd)->padding_text_ = pPADDING_TEXT;
	}
	void* pPADDING_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_SEPARATOR);
	if (pPADDING_SEPARATOR != 0)
	{
		((wnd_s*)pWnd)->padding_separator_ = pPADDING_SEPARATOR;
	}
	void* pSIZE_ITEM = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_ITEM);
	WORD szItem;
	if (pSIZE_ITEM != 0)
	{
		szItem = 合并短整数(__get_int(pSIZE_ITEM, 0), __get_int(pSIZE_ITEM, 4));
	}
	void* pSIZE_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_SEPARATOR);
	WORD szSeparator;
	if (pSIZE_SEPARATOR != 0)
	{
		szSeparator = 合并短整数(__get_int(pSIZE_SEPARATOR, 0), __get_int(pSIZE_SEPARATOR, 4));
	}
	
	((wnd_s*)pWnd)->szItemSeparator_ = 合并整数(szItem, szSeparator);
	void* lpFontFamily = nullptr;
	int dwFontSize = -1;
	int dwFontStyle = -1;
	void* pFONT_SIZE = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_SIZE);
	if (pFONT_SIZE != 0)
	{
		dwFontSize = __get_int(pFONT_SIZE, 0);
	}
	void* pFONT_STYLE = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_STYLE);
	if (pFONT_STYLE != 0)
	{
		dwFontStyle = __get_int(pFONT_STYLE, 0);
	}
	void* pFONT_FAMILY = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_FAMILY);
	if (pFONT_FAMILY != 0)
	{
		lpFontFamily = pFONT_FAMILY;
	}
	
	void* pfont = _font_createfromfamily((LPWSTR)lpFontFamily, dwFontSize, dwFontStyle);
	
	if (pfont != 0)
	{
		_font_destroy(((wnd_s*)pWnd)->hFont_Menu_);
		((wnd_s*)pWnd)->hFont_Menu_ = pfont;
	}
	_wnd_recalcclient(pWnd, hWnd, ((wnd_s*)pWnd)->width_, ((wnd_s*)pWnd)->height_);
}

void _wnd_calc_captionrect(void* pWnd, RECT* rcCaption)
{
	void* lpValue = ((wnd_s*)pWnd)->size_caption_;
	if (lpValue != 0)
	{
		(*rcCaption).right = __get_int(lpValue, 0) / 100 * ((wnd_s*)pWnd)->width_;//保证百分比
		(*rcCaption).bottom = Ex_Scale(__get_int(lpValue, 4));//绝对高度
	}
	lpValue = ((wnd_s*)pWnd)->margin_caption_;
	if (lpValue != 0)
	{
		(*rcCaption).left = Ex_Scale(__get_int(lpValue, 0));
		(*rcCaption).top = Ex_Scale(__get_int(lpValue, 4));
		(*rcCaption).right = (*rcCaption).right - Ex_Scale(__get_int(lpValue, 8));
	}
	(*rcCaption).bottom = (*rcCaption).bottom + (*rcCaption).top;
}


void _wnd_backgroundimage_timer_inherit(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	KillTimer(hWnd, idEvent);
	void* pWnd = (void*)(idEvent - TIMER_BKG_INHERIT);
	if (!__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_NOINHERITBKG))
	{
		void* ppWnd = nullptr;
		int nError = 0;
		if (_handle_validate(((wnd_s*)pWnd)->hExDuiParent_, HT_DUI, &ppWnd, &nError))
		{
			void* lpBI = ((wnd_s*)ppWnd)->lpBackgroundImage_;
			if (lpBI != 0)
			{
				void* pDelay = ((bkgimg_s*)lpBI)->lpDelay_;
				if (pDelay != 0)
				{
					_wnd_redraw_bkg(hWnd, pWnd, 0, true, false);
					UpdateWindow(hWnd);
					SetTimer(hWnd, idEvent, __get_int(pDelay, ((bkgimg_s*)lpBI)->curFrame_ * 4) * 10, (TIMERPROC)&_wnd_backgroundimage_timer_inherit);
				}
			}
		}
	}
}

int _wnd_dispatch_msg(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t lParam)
{
	auto nType = 取低位(lParam);
	int ret = 0;
	if (nType == EMT_OBJECT)//组件消息
	{
		ret = _wnd_dispatch_msg_obj(hWnd, (void*)wParam, 0, 0, 0, 0);
	}
	else if (nType == EMT_DUI)//窗体消息
	{
		if (wParam == EMV_HEXDUI)
		{
			return ((wnd_s*)pWnd)->hexdui_;
		}
		else if (wParam == EMV_PFN_PACK)
		{
			///////////return (size_t)(&_res_pack);
		}
	}
	else if (nType >= WM_MOUSEFIRST && nType <= WM_MOUSELAST || nType >= 1024 && nType <= 1027)
	{
		_wnd_dispatch_notify(hWnd, pWnd, ((wnd_s*)pWnd)->hexdui_, -1, NM_TRAYICON, wParam, lParam, 0);
	}
	else if (nType == EMT_LAYOUT_UPDATE)
	{
		_layout_update(wParam);
	}
	else if (nType == EMT_EASING)
	{
		_wnd_dispatch_notify(hWnd, pWnd, ((wnd_s*)pWnd)->hexdui_, -1, NM_EASING, __get((void*)wParam, 0), wParam, 0);
	}
	return ret;
}

int _wnd_dispatch_msg_obj(HWND hWnd, void* lpData, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	int ret;
	if (MemPool_AddressIsUsed(lpData))
	{
		RtlMoveMemory(&pObj, lpData, 20);
		if (pObj != 0)
		{
			size_t hObj = ((obj_s*)pObj)->hObj_;
			ret = _obj_msgproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		MemPool_Free(g_Li.hMemPoolMsg, lpData);
	}
	return ret;
}

size_t _wnd_proc(void* pData, int uMsg, size_t wParam, size_t lParam)
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
	void* pfnMsgProc = ((wnd_s*)pWnd)->pfnMsgProc_;
	if (pfnMsgProc != 0)
	{
		int ret=0;
		if (((MsgPROC)pfnMsgProc)(hWnd, ((wnd_s*)pWnd)->hexdui_, uMsg, wParam, (void*)lParam, &ret) != 0)
		{
			return ret;
		}
	}
	
	if (uMsg == g_Li.dwMessage)
	{
		return _wnd_dispatch_msg(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_NCHITTEST)
	{
		return _wnd_wm_nchittest(pWnd, hWnd, lParam);
	}
	else if (uMsg == WM_SETCURSOR)
	{
		if (_wnd_wm_setcursor(hWnd, pWnd, lParam))
		{
			return 1;
		}
	}
	else if (uMsg == WM_DESTROY)
	{
		SetWindowLongPtrW(hWnd, -4, pOld);
		if (_wnd_destroy(hWnd, pWnd) != 0)
		{
			PostQuitMessage(0);
		}
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (uMsg == WM_SIZE)
	{
		_wnd_wm_size(pWnd, hWnd, wParam, 取低位(lParam), 取高位(lParam));
	}
	else if (uMsg == WM_WINDOWPOSCHANGING)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
		{
			if (!__query((void*)lParam, 24, SWP_NOSIZE))
			{
				void* padding_client = ((wnd_s*)pWnd)->padding_client_;
				if (padding_client != 0)
				{
					__addn((void*)lParam, 20, Ex_Scale(__get_int(padding_client, 4) + __get_int(padding_client, 12)));
				}
			}

			if (!__query((void*)lParam, 24, SWP_NOMOVE))//被移动了
			{
				if (GetWindow(hWnd, GW_OWNER) != 0)//子菜单
				{
					__del(((wnd_s*)pWnd)->pMenuHostWnd_, offsetof(wnd_s, dwFlags_), EWF_bMenuRepostion);
					_wnd_menu_setpos(hWnd, pWnd, lParam);
				}
				((wnd_s*)pWnd)->left_ = __get_int((void*)lParam, 8);
				((wnd_s*)pWnd)->top_ = __get_int((void*)lParam, 12);
				_wnd_menu_init(hWnd, pWnd);
				return 0;
			}
		}
	}
	else if (uMsg == WM_WINDOWPOSCHANGED)
	{
		if (!__query((void*)lParam, 24, SWP_NOMOVE))
		{
			__set_int(pWnd, offsetof(wnd_s, left_), __get_int((void*)lParam, 8));
			__set_int(pWnd, offsetof(wnd_s, top_), __get_int((void*)lParam, 12));
			_wnd_paint_shadow(pWnd, false, false);
		}
	}
	else if (uMsg == WM_NCCALCSIZE)
	{
		return 0;
	}
	else if (uMsg == WM_NCACTIVATE)
	{
		_wnd_wm_leavecheck(hWnd, pWnd, ((wnd_s*)pWnd)->objHittest_, -1, (void*)-1, true);
		if (wParam == 0)
		{
			if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_ACTIVE))
			{
				size_t focus = ((wnd_s*)pWnd)->objFocus_;
				__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_ACTIVE);
				((wnd_s*)pWnd)->objFocusPrev_ = focus;
				Ex_ObjKillFocus(focus);
			}
		}
		else {
			if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_ACTIVE))
			{
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_ACTIVE);
				size_t focus = ((wnd_s*)pWnd)->objFocusPrev_;
				((wnd_s*)pWnd)->objFocusPrev_ = 0;
				Ex_ObjSetFocus(focus);
			}
		}
		_wnd_paint_shadow(pWnd, false, true);
		if (wParam == 0)
		{
			if (lParam == (size_t)(((wnd_s*)pWnd)->hWndShadow_))
			{
				if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
				{
					return 0;
				}
			}
		}
		return 1;//防止出现边框/系统按钮
	}
	else if (uMsg == WM_NCPAINT)
	{
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
	
		if (_wnd_wm_paint(pWnd, hWnd))
		{
			return 0;
		}
	}
	else if (uMsg == WM_SHOWWINDOW)
	{
		if (wParam == 0)
		{
			size_t objHittest = ((wnd_s*)pWnd)->objHittest_;
			if (objHittest != 0)
			{
				Ex_ObjDispatchMessage(objHittest, WM_MOUSELEAVE, 0, 0);
				((wnd_s*)pWnd)->objHittest_ = 0;
				UpdateWindow(hWnd);
			}
		}
		HWND hWndShadow = ((wnd_s*)pWnd)->hWndShadow_;
		if (!窗口_查询风格(hWnd, WS_EX_TOPMOST, true))
		{
			SetWindowPos(hWndShadow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
		}
		
		ShowWindow(hWndShadow, wParam == 0 ? 0 : SW_SHOWNOACTIVATE);
		InvalidateRect(hWnd, 0, false);
	}
	else if (uMsg == WM_GETMINMAXINFO)
	{
		if (_wnd_wm_getminmaxinfo(pWnd, hWnd, lParam))
		{
			return 0;
		}
	}
	else if (uMsg == WM_STYLECHANGING)
	{
		if (_wnd_wm_stylechanging(pWnd, hWnd, wParam, lParam))
		{
			return 0;
		}
	}
	else if (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
	{
		_wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
	{
		_wnd_wm_keyboard(pWnd, hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CAPTURECHANGED)
	{
		_wnd_wm_captionchange(hWnd, pWnd);
	}
	else if (uMsg == WM_CONTEXTMENU)//被动接受消息
	{
		_wnd_wm_menucontext(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		_wnd_wm_mousewheel(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SYSCOMMAND || uMsg == WM_COMMAND)
	{
		_wnd_wm_command(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_ENTERSIZEMOVE)
	{
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bSizeMoving);
	}
	else if (uMsg == WM_EXITSIZEMOVE)
	{
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bSizeMoving);
	}
	else if (uMsg == WM_MEASUREITEM)
	{
		if (_wnd_wm_measureitem_host(pWnd, wParam, lParam))
		{
			return 1;
		}
	}
	else if (uMsg == WM_INITDIALOG)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{
			_msgbox_initdialog(hWnd, pWnd, wParam, lParam);
			auto ret = CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
			SetFocus(hWnd);
			return ret;
		}
	}
	else if (uMsg == WM_GETDLGCODE)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{
			return (DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTALLKEYS | DLGC_WANTMESSAGE | DLGC_HASSETSEL | DLGC_WANTCHARS);
		}
	}
	else if (uMsg == WM_INITMENUPOPUP)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
		{
			return 0;
		}
		_wnd_wm_initmenupopup(hWnd, pWnd, (void*)wParam);
	}
	else if (uMsg == WM_EXITMENULOOP)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
		{
			return 0;
		}
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuInited);
		size_t objHittest = ((wnd_s*)pWnd)->objHittest_;
		_wnd_wm_nchittest(pWnd, hWnd, -1);
		_wnd_wm_leavecheck(hWnd, pWnd, objHittest, -1, (void*)-1, true);
	}
	else if (uMsg == WM_IME_COMPOSITION)
	{
		_wnd_wm_ime_composition(hWnd, pWnd);
	}
	else if (uMsg == WM_SETTEXT)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_TITLE))
		{
			Ex_ObjSendMessage(_obj_getobjfromidorname(pWnd, EWS_TITLE), uMsg, wParam, lParam);
		}
	}
	else if (uMsg == 485)// mn_selectitem
	{
		if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
		{
			GetCursorPos((LPPOINT)&wParam);
			_wnd_wm_mouse(pWnd, hWnd, WM_MOUSEMOVE, 1, 合并整数(wParam - ((wnd_s*)pWnd)->left_, lParam - ((wnd_s*)pWnd)->top_));
			return 0;
		}
		else {
			_wnd_menu_mouse(hWnd, pWnd, WM_MOUSEMOVE, 0, &wParam);
			if (wParam == -1)
			{
				return 0;
			}
			else {
				void* pMenuHostWnd = ((wnd_s*)pWnd)->pMenuHostWnd_;
				if (pMenuHostWnd != 0)
				{
					((wnd_s*)pMenuHostWnd)->pMenuPrevWnd_ = pWnd;
				}
			}
		}
	}
	else if (uMsg == 486)
	{
		void* pMenuHostWnd = ((wnd_s*)pWnd)->pMenuHostWnd_;
		if (pMenuHostWnd != 0)
		{
			void* pMenuTrackWnd = ((wnd_s*)pMenuHostWnd)->pMenuTrackWnd_;
			if (pMenuTrackWnd != 0)
			{
				if (__query(pMenuTrackWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
				{
					_wnd_obj_untrack(((wnd_s*)pMenuTrackWnd)->hWnd_, pMenuTrackWnd, true);
					return CallWindowProcW((WNDPROC)pOld, hWnd, 495, -1, 0);
				}
			}
		}
	}
	else if (uMsg == 493)//MN_BUTTONDOWN
	{
		if (!_wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONDOWN, 1, &wParam))
		{
			return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, -1, lParam);
		}
		if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
		{
			return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, -1, lParam);
		}
	}
	else if (uMsg == 495)//MN_BUTTONUP
	{
		_wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONUP, 0, &wParam);
		if (((wnd_s*)pWnd)->objTrackPrev_ != ((wnd_s*)pWnd)->objHittest_)
		{
			return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, -1, lParam);
		}
	}
	else if (uMsg == 497)//MN_BUTTONUP
	{
		return 0;
	}
	return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
}

int _wnd_create(size_t hExDui, void* pWnd, HWND hWnd, int dwStyle, void* hTheme, size_t lParam, void* lpfnMsgProc)
{
	ShowWindow(hWnd, 0);
	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);
	SIZE size;
	size.cx = rcWindow.right - rcWindow.left;
	size.cy = rcWindow.bottom - rcWindow.top;
	if ((dwStyle & EWS_MESSAGEBOX) == 0)
	{
		int offsetX = 0;
		int offsetY = 0;
		if (Flag_Query(EXGF_DPI_ENABLE) )
		{
			size.cx = Ex_Scale(size.cy);
			size.cy = Ex_Scale(size.cy);
			offsetX = (size.cx - (rcWindow.right - rcWindow.left)) / 2;
			offsetY = (size.cy - (rcWindow.bottom - rcWindow.top)) / 2;
		}
		MoveWindow(hWnd, rcWindow.left - offsetX, rcWindow.top - offsetY, size.cx, size.cy, false);
	}
	HWND hWndParent;
	if ((dwStyle & EWS_MESSAGEBOX) != 0)
	{
		
		_wnd_getfromhandle(((mbp_s*)lParam)->handle_, &hWndParent);
	}
	else {
		hWndParent = (HWND)GetWindowLongPtrW(hWnd, -8);
	}
	if ((dwStyle & EWS_CENTERWINDOW) != 0)
	{
		
		Ex_WndCenterFrom(hWnd, hWndParent, (dwStyle & EWS_FULLSCREEN) != 0);
	}

	GetWindowRect(hWnd, &rcWindow);
	size.cx = rcWindow.right - rcWindow.left;
	size.cy = rcWindow.bottom - rcWindow.top;
	int dwFlags;
	if (窗口_查询风格(hWnd, WS_EX_LAYERED, true) || (dwStyle & EWS_MESSAGEBOX) != 0)
	{
		dwFlags = EWF_bLayered;
	}
	((wnd_s*)pWnd)->hExDuiParent_ = Ex_DUIFromWindow(hWndParent);
	
	if (hTheme == 0) hTheme = g_Li.hThemeDefault;
	
	if ((dwStyle & EWS_BLUR) != 0) ((wnd_s*)pWnd)->fBlur_ = 15;
	((wnd_s*)pWnd)->dwStyle_ = dwStyle;
	((wnd_s*)pWnd)->hImc_ = ImmGetContext(hWnd);
	((wnd_s*)pWnd)->hWnd_ = hWnd;
	((wnd_s*)pWnd)->hexdui_ = hExDui;
	((wnd_s*)pWnd)->hTheme_ = hTheme;
	((wnd_s*)pWnd)->width_ = size.cx;
	((wnd_s*)pWnd)->height_ = size.cy;
	((wnd_s*)pWnd)->alpha_ = 255;
	((wnd_s*)pWnd)->dwFlags_ = dwFlags | EWF_bLeaveSent;
	((wnd_s*)pWnd)->pfnMsgProc_ = lpfnMsgProc;
	((wnd_s*)pWnd)->hTableObjects_ = HashTable_Create(取最近质数(256), NULL);


	if ((dwStyle & EWS_MESSAGEBOX) != 0)
	{
		((wnd_s*)pWnd)->lpMsgParams_ = (void*)lParam;
	}
	else {
		((wnd_s*)pWnd)->lParam_ = lParam;
	}

	//layer
	((wnd_s*)pWnd)->pptDst_ = (void*)((size_t)pWnd + offsetof(wnd_s, pptDst_x_));
	((wnd_s*)pWnd)->psize_ = (void*)((size_t)pWnd + offsetof(wnd_s, width_));
	((wnd_s*)pWnd)->pptSrc_ = (void*)((size_t)pWnd + offsetof(wnd_s, pptSrc_x_));
	((wnd_s*)pWnd)->pblend_ = (void*)((size_t)pWnd + offsetof(wnd_s, pblend_bytes_));
	((wnd_s*)pWnd)->prcDirty_ = (void*)((size_t)pWnd + offsetof(wnd_s, prcDirty_left_));
	((wnd_s*)pWnd)->cbsize_ = 40;
	((wnd_s*)pWnd)->dwFlags_ = 2;
	((wnd_s*)pWnd)->pblend_bytes_ = 33488896;
	((wnd_s*)pWnd)->pptDst_x_ = rcWindow.left;
	((wnd_s*)pWnd)->pptDst_y_ = rcWindow.top;
	((wnd_s*)pWnd)->width_ = size.cx;
	((wnd_s*)pWnd)->height_ = size.cy;
	((wnd_s*)pWnd)->prcDirty_right_ = size.cx;
	((wnd_s*)pWnd)->prcDirty_bottom_ = size.cy;

	//sysshadow
	auto tmp1 = GetClassLongPtrW(hWnd, -26);
	if ((tmp1 & CS_DROPSHADOW) != 0)
	{
		SetClassLongPtrW(hWnd, -26, (tmp1 & ~CS_DROPSHADOW));
	}
	int tmp2 = WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST;
	
	auto tmp3 = CreateWindowExW(tmp2, (LPCWSTR)g_Li.atomSysShadow, 0, WS_POPUP, 0, 0, 0, 0, hWnd, 0, 0, 0);
	
	((wnd_s*)pWnd)->hWndShadow_ = tmp3;
	int nError = 0;
	
	Thunkwindow(tmp3, &_wnd_shadow_proc, (void*)(size_t)1, &nError);
	
	//tips
	HWND hWndTips = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST, L"tooltips_class32", 0, WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
	SendMessageW(hWndTips, 1048, 0, 2048);//TTM_SETMAXTIPWIDTH 支持多行
	((wnd_s*)pWnd)->hWndTips_ = hWndTips;
	
	void* tmp4 = _struct_createfromaddr(pWnd, offsetof(wnd_s, ti_auto_), sizeof(ti_s), &nError);
	
	((ti_s*)tmp4)->cbSize_ = sizeof(ti_s);
	((ti_s*)tmp4)->uFlags_ = 16 | 0 | 4096;//TTF_SUBCLASS ,TTF_TRANSPARENT ,TTF_PARSELINKS
	((ti_s*)tmp4)->hWnd_ = hWnd;
	((ti_s*)tmp4)->uId_ = tmp4;
	SendMessageW(hWndTips, 1074, 0, (LPARAM)tmp4);//TTM_ADDTOOLW
	
	void* tmp5 = _struct_createfromaddr(pWnd, offsetof(wnd_s, ti_track_), sizeof(ti_s), &nError);
	
	((ti_s*)tmp5)->cbSize_ = sizeof(ti_s);
	((ti_s*)tmp5)->uFlags_ = 32 | 128 | 0 | 4096;// TTF_TRACK ,TTF_ABSOLUTE ,TTF_TRANSPARENT,TTF_PARSELINKS
	((ti_s*)tmp5)->hWnd_ = hWnd;
	((ti_s*)tmp5)->uId_ = tmp5;
	SendMessageW(hWndTips, 1074, 0, (LPARAM)tmp5);//TTM_ADDTOOLW
	
	Thunkwindow(hWndTips, &_wnd_tooltips_proc, pWnd, &nError);
	
	//初始化渲染,以及背景
	_wnd_dx_init(pWnd);
	int w1, h1;
	w1 = rcWindow.right - rcWindow.left;
	h1 = rcWindow.bottom - rcWindow.top;
	
	((wnd_s*)pWnd)->canvas_display_ = _canvas_createfrompwnd(pWnd, w1, h1, CVF_GDI_COMPATIBLE, &nError);
	
	
	((wnd_s*)pWnd)->canvas_bkg_ = _canvas_createfrompwnd(pWnd, w1, h1, 0, &nError);


	if (nError == 0)
	{
		//加载主题，并确定客户区
		
		_wnd_loadtheme(pWnd, hWnd, hTheme);
	
		if ((dwStyle & EWS_MENU) == 0)
		{
			_wnd_sysbutton_create(hWnd, pWnd, dwStyle);
			
		}
		
		窗口_添加风格(hWnd, WS_THICKFRAME, false);//强制触发样式被修改事件
	
		Thunkwindow(hWnd, &_wnd_proc, pWnd, &nError);
		
		窗口_删除风格(hWnd, WS_THICKFRAME, false);
		IME_Control(hWnd, pWnd, false);
	}
	
	return nError;
}

size_t _wnd_tooltips_proc(void* pData, int uMsg, size_t wParam, size_t lParam)
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
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		lParam = CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
		SendMessageW(hWnd, 1041, 0, (size_t)pWnd + offsetof(wnd_s, ti_track_));
		return lParam;
	}
	return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
}

size_t _wnd_shadow_proc(void* pData, int uMsg, size_t wParam, size_t lParam)
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
	if (uMsg == WM_NCACTIVATE)
	{
		size_t hWndParent = GetWindowLongPtrW(hWnd, -8);
		if (!(wParam == 0 && lParam == hWndParent))
		{
#if defined(_M_IX86)
			if (__get_int(pData, 21) == wParam && lParam == 0)
			{
				SendMessageW((HWND)hWndParent, uMsg, 1, lParam);
				SetFocus((HWND)hWndParent);
				__set_int(pData, 21, 1);
				return 0;
			}
			SendMessageW((HWND)hWndParent, uMsg, wParam, lParam);
			__set_int(pData, 21, wParam);
#elif defined(_M_AMD64)
			if (__get(pData, 38) == wParam && lParam == 0)
			{
				SendMessageW((HWND)hWndParent, uMsg, 1, lParam);
				SetFocus((HWND)hWndParent);
				__set(pData, 38, 1);
				return 0;
			}
			SendMessageW((HWND)hWndParent, uMsg, wParam, lParam);
			__set(pData, 38, wParam);
#endif
	}
	}
	else if (uMsg == WM_DESTROY)
	{
		SetWindowLongPtrW(hWnd, -4, pOld);
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
}

void _wnd_dx_unint(void* pWnd)
{
	void* pDeviceContext = ((wnd_s*)pWnd)->context_;

	if (pDeviceContext != 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->Release();
	}
	void* pgdiinterop = ((wnd_s*)pWnd)->gdiinterop_;
	if (pgdiinterop != 0)
	{
		((ID2D1GdiInteropRenderTarget*)pgdiinterop)->Release();
	}
}

void _wnd_dx_init(void* pWnd)
{
	_wnd_dx_unint(pWnd);
	void* pDeviceContext = nullptr;
	if (((ID2D1Device*)g_Ri.pD2DDevice)->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext**)&pDeviceContext) == 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
		((wnd_s*)pWnd)->context_ = pDeviceContext;
		((wnd_s*)pWnd)->gdiinterop_ = _dx_get_gdiInterop(pDeviceContext);
	}
}

size_t _wnd_wm_nchittest_obj(HWND hWnd, void* pWnd, size_t objLast, int x, int y, int* hitCode, void** pObjMouse)
{
	size_t objPrev = objLast;
	void* pObj = nullptr;
	int nError = 0;
	while (_handle_validate(objPrev, HT_OBJECT, &pObj, &nError))
	{
		if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
		{
			RECT rt = { ((obj_s*)pObj)->left_,((obj_s*)pObj)->top_,((obj_s*)pObj)->right_,((obj_s*)pObj)->bottom_ };
			POINT po = { x,y };
			if (PtInRect(&rt, po))
			{
				int ox = x - ((obj_s*)pObj)->left_;
				int oy = y - ((obj_s*)pObj)->top_;
				auto lParam = 合并整数(ox, oy);
				if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED))//检测是否被禁止，被禁止组件还需检测是否可穿透
				{
					*hitCode = _obj_baseproc(hWnd, objPrev, pObj, WM_NCHITTEST, 0, lParam);
					if (*hitCode == HTTRANSPARENT)//在穿透区域,则继续寻找
					{
						objPrev = ((obj_s*)pObj)->objPrev_;
						continue;
					}
					else {
						*hitCode = HTCLIENT;//不可穿透则直接停止寻找
						*pObjMouse = pObj;
						return objPrev;
					}
				}
				else {
					objLast = ((obj_s*)pObj)->objChildLast_;
					if (objLast != 0)
					{
						auto ret = _wnd_wm_nchittest_obj(hWnd, pWnd, objLast, ox, oy, hitCode, pObjMouse);
						if (ret != 0 && *hitCode != HTTRANSPARENT)//找到目标子组件
						{
							return ret;
						}
					}
				}
				*hitCode = _obj_baseproc(hWnd, objPrev, pObj, WM_NCHITTEST, 0, lParam);
				if (*hitCode == HTTRANSPARENT)// 如果可穿透，则继续寻找
				{
					objPrev = ((obj_s*)pObj)->objPrev_;
					continue;
				}
				else {
					((wnd_s*)pWnd)->dwHitObjPos_Abs_ = lParam;
					*pObjMouse = pObj;
					return objPrev;
				}
			}
		}
		objPrev = ((obj_s*)pObj)->objPrev_;
	}
	return 0;
}

int _wnd_wm_nchittest_sizebox(int width, int height, int x, int y)
{
	int ret = 0;
	int nOffset = 5 * 2;
	if (y < nOffset)//鼠标在顶部
	{
		if (x < nOffset)//鼠标在左边
		{
			ret = HTTOPLEFT;
		}
		else if (x > width - nOffset) //鼠标在右边
		{

			ret = HTTOPRIGHT;
		}
		else {
			ret = HTTOP;
		}
	}
	else {
		if (y > height - nOffset)//鼠标在底部
		{
			if (x < nOffset)//鼠标在左边
			{
				ret = HTBOTTOMLEFT;
			}
			else if (x > width - nOffset)//鼠标在右边
			{
				ret = HTBOTTOMRIGHT;
			}
			else {
				ret = HTBOTTOM;
			}
		}
		else {
			if (x < nOffset)//鼠标在左边
			{
				if (y < nOffset)//鼠标在上面
				{
					ret = HTTOPLEFT;
				}
				else {
					if (y > height - nOffset)//鼠标在下面
					{
						ret = HTBOTTOMLEFT;
					}
					else {
						ret = HTLEFT;
					}
				}
			}
			else {
				//鼠标在右边
				if (y < nOffset)//鼠标在上面
				{
					ret = HTTOPRIGHT;
				}
				else {
					if (y > height - nOffset)//鼠标在下面
					{
						ret = HTBOTTOMRIGHT;
					}
					else {
						ret = HTRIGHT;
					}
				}
			}
		}
	}
	return ret;
}

int _wnd_wm_nchittest(void* pWnd, HWND hWnd, size_t lParam)
{
	int dwHitCode = HTCLIENT;
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject)) return dwHitCode;
	if (!__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
	{
		if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuInited))
		{
			return dwHitCode;
		}
	}

	POINT pt;
	size_t objMouse;
	if (lParam == -1)
	{
		GetCursorPos(&pt);
	}
	else {
		pt.x = 取低位(lParam);
		pt.y = 取高位(lParam);
	}
	ScreenToClient(hWnd, &pt);
	if (_rgn_hittest(((wnd_s*)pWnd)->hrgn_client_, pt.x, pt.y) || ((wnd_s*)pWnd)->dwWinState_ == 2)
	{
		void* pobjMouse = nullptr;
		objMouse = _wnd_wm_nchittest_obj(hWnd, pWnd, ((wnd_s*)pWnd)->objChildLast_, pt.x, pt.y, &dwHitCode, &pobjMouse);
		if (objMouse == 0)
		{
			pobjMouse = 0;
			if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MOVEABLE))//允许随意移动
			{
				dwHitCode = HTCAPTION;
			}
		}
		else {
			((wnd_s*)pWnd)->dwHitCode_ = dwHitCode;
			if (objMouse == ((wnd_s*)pWnd)->objCaption_)
			{
				dwHitCode = HTCAPTION;
			}
			else {
				if (__query(pobjMouse, offsetof(obj_s, dwStyle_), EOS_DISABLED))
				{
					if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MOVEABLE))//允许随意移动
					{
						dwHitCode = HTCAPTION;
					}
					objMouse = 0;
				}
			}
		}
	}
	else {
		//在尺寸边框内
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_SIZEABLE))//允许调整尺寸
		{
			dwHitCode = _wnd_wm_nchittest_sizebox(((wnd_s*)pWnd)->width_, ((wnd_s*)pWnd)->height_, pt.x, pt.y);
		}
		else {
			dwHitCode = HTCAPTION;
		}
	}

	auto objHittest = ((wnd_s*)pWnd)->objHittest_;
	if (objHittest != objMouse)
	{
		((wnd_s*)pWnd)->objHittestPrev_ = objHittest;
		dwHitCode = HTCLIENT;
	}
	return dwHitCode;
}

void _wnd_sysbutton_create(HWND hWnd, void* pWnd, int dwStyle)
{
	void* hTheme = ((wnd_s*)pWnd)->hTheme_;
	RECT rcCaption = { 0 };
	_wnd_calc_captionrect(pWnd, &rcCaption);
	void* pObjCaption = nullptr;
	int nError = 0;
	size_t objCaption = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjCaption, &nError);
	
	int nMinHeight = 0;
	if (objCaption != 0)
	{
		
		void* lpTitle = nullptr;
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{

			lpTitle = ((mbp_s*)((wnd_s*)pWnd)->lpMsgParams_)->lpCaption_;

		}
		else {
			
			auto title = 窗口_取标题(hWnd);
			lpTitle = (void*)title.data();
		}
		std::cout << "_wnd_sysbutton_create->_obj_create_proc" << std::endl;
		_obj_create_proc(&nError, false, hTheme, pObjCaption, (dwStyle & EWS_NOCAPTIONTOPMOST) != 0 ? 0 : -1, ATOM_SYSBUTTON, lpTitle,
			EOS_VISIBLE | EWS_TITLE, rcCaption.left, rcCaption.top, rcCaption.right - rcCaption.left, rcCaption.bottom - rcCaption.top, 0, EWS_TITLE, 0, 0, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		
		void* lpPT = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_PADDING_TEXT);
		if (lpPT != 0)
		{
			RtlMoveMemory((void*)((size_t)pObjCaption + offsetof(obj_s, t_left_)), lpPT, 16);
			if (g_Li.DpiX > 1)
			{
				((obj_s*)pObjCaption)->t_left_ = ((obj_s*)pObjCaption)->t_left_ * g_Li.DpiX;
				((obj_s*)pObjCaption)->t_top_ = ((obj_s*)pObjCaption)->t_top_ * g_Li.DpiX;
				((obj_s*)pObjCaption)->t_right_ = ((obj_s*)pObjCaption)->t_right_ * g_Li.DpiX;
				((obj_s*)pObjCaption)->t_bottom_ = ((obj_s*)pObjCaption)->t_bottom_ * g_Li.DpiX;
			}
		}
		void* lpCr = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_COLOR_NORMAL);
		if (lpCr != 0)
		{
			((obj_s*)pObjCaption)->crNormal_ = __get_int(lpCr, 0);
		}
		void* pFamily = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_FAMILY);
		void* pSize = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_SIZE);
		void* pStyle = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_STYLE);
		
		int Size, Style;
		if (pFamily == 0) pFamily = (void*)-1;
		if (pSize == 0)
		{
			Size = -1;
		}
		else {
			Size = __get_int(pSize, 0);
		}
		if (pStyle == 0)
		{
			Style = -1;
		}
		else {
			Style = __get_int(pStyle, 0);
		}
		if ((size_t)pFamily != -1 || Size != -1 || Style != -1)
		{
			void* hfont = ((obj_s*)pObjCaption)->hFont_;
			((obj_s*)pObjCaption)->hFont_ = _font_createfromfamily((LPWSTR)pFamily, Size, Style);
			if (hfont != 0) _font_destroy(hfont);
		}
		_obj_create_done(hWnd, pWnd, objCaption, pObjCaption);
		((wnd_s*)pWnd)->objCaption_ = objCaption;
		nMinHeight = rcCaption.bottom + rcCaption.top;
	}
	
	std::vector<int> aryBtn = { EWS_BUTTON_CLOSE ,EWS_BUTTON_MAX ,EWS_BUTTON_MIN ,EWS_BUTTON_MENU ,EWS_BUTTON_SKIN ,EWS_BUTTON_SETTING ,EWS_BUTTON_HELP };
	std::vector<int> aryAtom = { ATOM_SYSBUTTON_CLOSE ,ATOM_SYSBUTTON_MAX ,ATOM_SYSBUTTON_MIN ,ATOM_SYSBUTTON_MENU ,ATOM_SYSBUTTON_SKN ,ATOM_SYSBUTTON_SETTING ,ATOM_SYSBUTTON_HELP };
	int left = rcCaption.right - rcCaption.left;
	int nMinWidth = 0;

	for (int i = 0; i < 7; i++)
	{
		if ((dwStyle & aryBtn[i]) != 0)
		{
			
			void* lpValuea = Ex_ThemeGetValuePtr(hTheme, aryAtom[i], ATOM_NORMAL);
			
			if (lpValuea != 0)
			{
				
				RECT rcObject;
				RtlMoveMemory(&rcObject, lpValuea, 16);
				rcObject.right = Ex_Scale(rcObject.right - rcObject.left);
				rcObject.bottom = Ex_Scale(rcObject.bottom - rcObject.top);
				left = left - rcObject.right;
				void* pObjTmp = nullptr;
				int nError = 0;
				size_t objTmp = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjTmp, &nError);
				if (objTmp != 0)
				{
					_obj_create_proc(&nError, false, hTheme, pObjTmp, -1, ATOM_SYSBUTTON, 0, EOS_VISIBLE | aryBtn[i], left, 0, rcObject.right, rcObject.bottom, objCaption, aryBtn[i], 0, 0, 0);
					_obj_create_done(hWnd, pWnd, objTmp, pObjTmp);
					nMinWidth = nMinWidth + rcObject.right;
				}
			}
		}
	}
	__addn(pObjCaption, offsetof(obj_s, t_right_), nMinWidth);
	nMinWidth = nMinWidth + rcCaption.left + ((wnd_s*)pWnd)->width_ - rcCaption.right;
	if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_HASICON))
	{
		nMinWidth = nMinWidth + Ex_Scale(16);
	}
	if (((wnd_s*)pWnd)->minWidth_ < nMinWidth)
	{
		((wnd_s*)pWnd)->minWidth_ = nMinWidth;
	}
	if (((wnd_s*)pWnd)->minHeight_ < nMinHeight)
	{
		((wnd_s*)pWnd)->minHeight_ = nMinHeight;
	}
}

void _wnd_render_obj(HWND hWnd, void* pWnd, void* pContext, size_t cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	size_t objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush)
{
	size_t objNext = objChildFirst;
	int nError = 0;
	if (objNext != 0)
	{
		do
		{
			void* pObj = nullptr;
			if (_handle_validate(objNext, HT_OBJECT, &pObj, &nError))
			{
				if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VISIBLE))
				{
					RECT rcObj;
					rcObj.left = ((obj_s*)pObj)->left_;
					rcObj.top = ((obj_s*)pObj)->top_;
					rcObj.right = ((obj_s*)pObj)->right_;
					rcObj.bottom = ((obj_s*)pObj)->bottom_;
					
					OffsetRect(&rcObj, offsetX, offsetY);
					RECT rcClip;
					if (IntersectRect(&rcClip, &rcPaint, &rcObj))
					{
						auto fPage = !__query(pObj, offsetof(obj_s, dwFlags_), eof_bPage);
						if (fPage)
						{
							if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bCanRedraw))
							{
								if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw))
								{
									__del(pObj, offsetof(obj_s, dwFlags_), eof_bNeedRedraw);
									if (!IsRectEmpty((RECT*)((size_t)pObj + offsetof(obj_s, d_left_))))
									{
										_obj_baseproc(hWnd, objNext, pObj, WM_PAINT, 0, 0);
										if (fDX)
										{
											_dx_settarget(pContext, pBitmapDisplay);
										}
									}
								}
							}
						}
						int alpha = ((obj_s*)pObj)->dwAlpha_;
						alpha = alpha * pAlpha / 255;
						if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_DISABLED))
						{
							alpha = alpha * ((obj_s*)pObj)->dwAlphaDisable_ / 255;
						}
						if (fPage)//非页面
						{
							if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bPath))
							{
								void* hPathBrush = _brush_createfromcanvas(((obj_s*)pObj)->canvas_obj_);
								if (hPathBrush != 0)
								{
									void* matrix = _matrix_create();
									_matrix_translate(matrix, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_top_, 0);
									_brush_settransform(hPathBrush, matrix);
									_canvas_fillpath(cvDisplay, ((obj_s*)pObj)->hPath_Window_, hPathBrush);
									_brush_destroy(hPathBrush);
									_matrix_destroy(matrix);
								}
							}
							else {
								if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_COMPOSITED))
								{
									_canvas_bitblt(cvDisplay, ((obj_s*)pObj)->canvas_obj_, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom, rcClip.left - rcObj.left, rcClip.top - rcObj.top);
								}
								else {
									_canvas_alphablend(cvDisplay, ((obj_s*)pObj)->canvas_obj_, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom, rcClip.left - rcObj.left, rcClip.top - rcObj.top, rcClip.right - rcObj.left, rcClip.bottom - rcObj.top, alpha);
								}
							}
						}
						if (hBorderBrush != 0)
						{
							_canvas_drawrect(cvDisplay, hBorderBrush, ((obj_s*)pObj)->w_left_, ((obj_s*)pObj)->w_top_, ((obj_s*)pObj)->w_right_, ((obj_s*)pObj)->w_bottom_, 1, 0);
						}

						size_t objChild = ((obj_s*)pObj)->objChildFirst_;
						if (objChild != 0)
						{
							_wnd_render_obj(hWnd, pWnd, pContext, cvDisplay, pBitmapDisplay, rcClip, objChild, rcObj.left, rcObj.top, alpha, fDX, hBorderBrush);
						}
					}
				}
			}
			else {
				break;
			}
			objNext = ((obj_s*)pObj)->objNext_;
		} while (objNext != 0);
	}
}

bool _wnd_wm_setcursor(HWND hWnd, void* pWnd, size_t lParam)
{
	auto uHitCode = 取低位(lParam);
	auto hCursor = ((wnd_s*)pWnd)->hCursor_;
	if (uHitCode == HTCLIENT)
	{
		auto objHittest = ((wnd_s*)pWnd)->objHittest_;
		void* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objHittest, HT_OBJECT, &pObj, &nError))
		{
			if (_obj_baseproc(hWnd, objHittest, pObj, WM_SETCURSOR, ((wnd_s*)pWnd)->dwHitObjPos_Abs_, 合并整数(((wnd_s*)pWnd)->dwHitCode_, 取高位(lParam))) != 0)
			{
				return true;
			}
		}
	}
	else if (uHitCode == HTCAPTION)
	{
		hCursor = g_Li.hCursor;
	}
	else if (uHitCode == HTTOPLEFT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENWSE);
	}
	else if (uHitCode == HTTOPRIGHT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENESW);
	}
	else if (uHitCode == HTTOP)
	{
		hCursor = LoadCursorW(0, IDC_SIZENS);
	}
	else if (uHitCode == HTBOTTOMLEFT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENESW);
	}
	else if (uHitCode == HTBOTTOMRIGHT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENWSE);
	}
	else if (uHitCode == HTBOTTOM)
	{
		hCursor = LoadCursorW(0, IDC_SIZENS);
	}
	else if (uHitCode == HTTOPLEFT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENWSE);
	}
	else if (uHitCode == HTBOTTOMLEFT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENESW);
	}
	else if (uHitCode == HTLEFT)
	{
		hCursor = LoadCursorW(0, IDC_SIZEWE);
	}
	else if (uHitCode == HTTOPRIGHT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENESW);
	}
	else if (uHitCode == HTBOTTOMRIGHT)
	{
		hCursor = LoadCursorW(0, IDC_SIZENWSE);
	}
	else if (uHitCode == HTRIGHT)
	{
		hCursor = LoadCursorW(0, IDC_SIZEWE);
	}

	if (hCursor != 0)
	{
		SetCursor((HCURSOR)hCursor);
		return true;
	}
	return false;
}

void _wnd_timer_mousetrack(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	KillTimer(hWnd, idEvent);
	void* pWnd = (void*)(idEvent - TIMER_MOUSETRACK);
	if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
	{
		POINT pt;
		GetCursorPos(&pt);
		if (WindowFromPoint(pt) != hWnd)
		{
			if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeaveSent))
			{
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeaveSent);
				size_t hObj = ((wnd_s*)pWnd)->objHittestPrev_;
				void* pObj = nullptr;
				int nError = 0;
				if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
				{
					_obj_baseproc(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0);//发送离开
					((wnd_s*)pWnd)->objHittestPrev_ = ((wnd_s*)pWnd)->objHittest_;
					((wnd_s*)pWnd)->objHittest_ = 0;
				}
			}
		}
	}
}

void _wnd_wm_leavecheck(HWND hWnd, void* pWnd, size_t objCheck, size_t objHittest, void* pObjHittest, bool fTrack)
{
	int nError = 0;
	if (objHittest = -1)
	{
		objHittest = ((wnd_s*)pWnd)->objHittest_;
		_handle_validate(objHittest, HT_OBJECT, &pObjHittest, &nError);
	}
	if (objCheck != objHittest)
	{
		void* pObjCheck = nullptr;
		if (_handle_validate(objCheck, HT_OBJECT, &pObjCheck, &nError))
		{
			if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeaveSent))
			{
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeaveSent);
				_obj_baseproc(hWnd, objCheck, pObjCheck, WM_MOUSELEAVE, 0, 0);
			}
		}
		if (objHittest != 0)
		{
			__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeaveSent);
			((wnd_s*)pWnd)->objHittestPrev_ = objHittest;
			_obj_baseproc(hWnd, objHittest, pObjHittest, WM_MOUSEHOVER, 0, 0);
		}
	}

	if (objHittest != 0 && fTrack)
	{
		objHittest = (size_t)pWnd + TIMER_MOUSETRACK;
		KillTimer(hWnd, objHittest);
		SetTimer(hWnd, objHittest, 50, (TIMERPROC)&_wnd_timer_mousetrack);
	}
}

int _wnd_destroy(HWND hWnd, void* pWnd)
{
	__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bDestroyWindow);
	DestroyWindow(((wnd_s*)pWnd)->hWndTips_);
	DestroyWindow(((wnd_s*)pWnd)->hWndShadow_);
	size_t hObj = ((wnd_s*)pWnd)->objChildFirst_;
	void* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		_obj_notify_brothers(hWnd, hObj, pObj, WM_DESTROY, 0, 0, false, false);
	}
	ReleaseCapture();

	//对话框
	if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
	{
		void* pMsg = ((wnd_s*)pWnd)->lpMsgParams_;
		if (pMsg != 0)
		{
			if (((mbp_s*)pMsg)->lpCheckBox_ != 0)
			{
				size_t dwTmp = 0;
				if (HashTable_Get(((wnd_s*)pWnd)->hTableObjects_, (size_t)(((mbp_s*)pMsg)->lpCheckBoxChecked_), &dwTmp))
				{
					if (dwTmp != 0)
					{
						__set(((mbp_s*)pMsg)->lpCheckBoxChecked_, 0, Ex_ObjDispatchMessage(dwTmp, BM_GETCHECK, 0, 0));
					}
				}
			}
		}
	}

	//清理托盘图标
	void* nID = ((wnd_s*)pWnd)->nID_;
	if (nID != 0)
	{
		Shell_NotifyIconW(NIM_DELETE, (PNOTIFYICONDATAW)nID);
		释放内存(nID);
	}

	HashTable_Destroy(((wnd_s*)pWnd)->hTableObjects_);

	//bkgimginfo
	KillTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT);
	_obj_backgroundimage_clear(hWnd, pWnd);
	_font_destroy(((wnd_s*)pWnd)->hFont_Menu_);
	bool bMainWnd = __query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MAINWINDOW);
	_struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_auto_));
	_struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_track_));
	_rgn_destroy(((wnd_s*)pWnd)->hrgn_client_);
	_rgn_destroy(((wnd_s*)pWnd)->hrgn_sizebox_);
	_canvas_destroy(((wnd_s*)pWnd)->canvas_bkg_);
	_canvas_destroy(((wnd_s*)pWnd)->canvas_display_);
	size_t hLayout = ((wnd_s*)pWnd)->hLayout_;
	if (hLayout != 0)
	{
		_layout_destory(hLayout);
	}

	_wnd_dx_unint(pWnd);
	size_t hExDui = ((wnd_s*)pWnd)->hexdui_;
	_handle_destroy(hExDui, &nError);
	释放内存(pWnd);
	Ex_SetLastError(nError);
	return (int)bMainWnd;
}

void _wnd_paint_bkg(HWND hWnd, void* pWnd)
{
	size_t cvBkg = ((wnd_s*)pWnd)->canvas_bkg_;
	void* p = pWnd;
	if (_canvas_begindraw(cvBkg))
	{
		if (!__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_NOINHERITBKG))
		{
			void* pParent = nullptr;
			int nError = 0;
			if (_handle_validate(((wnd_s*)pWnd)->hExDuiParent_, HT_DUI, &pParent, &nError))
			{
				p = pParent;
			}
			if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bInheritBkgStarted))
			{
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bInheritBkgStarted);
				SetTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT, 100, (TIMERPROC)&_wnd_backgroundimage_timer_inherit);
			}
		}
		//绘制底色
		int crBkg = ((wnd_s*)p)->crBkg_;
		_canvas_clear(cvBkg, crBkg);

		//绘制背景
		void* lpBackgroundImage = ((wnd_s*)p)->lpBackgroundImage_;
		if (lpBackgroundImage != 0)
		{
			_canvas_drawimagefrombkgimg(cvBkg, lpBackgroundImage);
		}

		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_BLUR))
		{
			float fBlur = ((wnd_s*)pWnd)->fBlur_;
			_canvas_blur(cvBkg, fBlur, 0);
		}

		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{
			_msgbox_drawinfo(pWnd, cvBkg);
		}

		_canvas_enddraw(cvBkg);
		_obj_compostied_all(((wnd_s*)pWnd)->objChildFirst_);
	}
}

void _wnd_render_dc(HWND hWnd, void* pWnd, void* hDC, size_t cvDisplay, RECT rcPaint, bool fLayer)
{
	void* mDC = _canvas_getdc(cvDisplay);
	if (mDC != 0)
	{
		if (fLayer)
		{
			((wnd_s*)pWnd)->prcDirty_left_ = rcPaint.left;
			((wnd_s*)pWnd)->prcDirty_top_ = rcPaint.top;
			((wnd_s*)pWnd)->prcDirty_right_ = rcPaint.right;
			((wnd_s*)pWnd)->prcDirty_bottom_ = rcPaint.bottom;
			
			((wnd_s*)pWnd)->hdcSrc_ = mDC;
			if (g_Li.pfnUpdateLayeredWindowIndirect == 0)
			{
				BLENDFUNCTION pb_;
				pb_.BlendOp = 0;
				pb_.BlendFlags = 0;
				pb_.SourceConstantAlpha = 0xFF;
				pb_.AlphaFormat = 1;
				UpdateLayeredWindow(hWnd, 0, 0, (SIZE*)(((wnd_s*)pWnd)->psize_), (HDC)(((wnd_s*)pWnd)->hdcSrc_), (POINT*)(((wnd_s*)pWnd)->pptSrc_), 0, &pb_, ULW_ALPHA);
			}
			else {
				((UpdateLayeredWindowIndirectPROC)g_Li.pfnUpdateLayeredWindowIndirect)(hWnd, (UPDATELAYEREDWINDOWINFO*)((size_t)pWnd + offsetof(wnd_s, ulwi_)));
			}
		}
		else {
			BitBlt((HDC)hDC, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, (HDC)mDC, rcPaint.left, rcPaint.top, SRCCOPY);
		}
		_canvas_releasedc(cvDisplay);
	}
}

void _wnd_render(HWND hWnd, void* pWnd, void* hDC, RECT rcPaint, bool fLayer, bool fDX)
{
	__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendering);
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRedrawBackground))
	{
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRedrawBackground);
		_wnd_paint_bkg(hWnd, pWnd);
	}
	size_t cvDisplay = ((wnd_s*)pWnd)->canvas_display_;
	if (_canvas_begindraw(cvDisplay))
	{
		void* pContext = nullptr;
		void* pBitmapDisplay = nullptr;
		if (fDX)
		{
			pContext = ((wnd_s*)pWnd)->context_;
			pBitmapDisplay = _canvas_getcontext(cvDisplay, CVC_DX_D2DBITMAP);
			_dx_bmp_copyfrom(pBitmapDisplay, _canvas_getcontext(((wnd_s*)pWnd)->canvas_bkg_, CVC_DX_D2DBITMAP), rcPaint.left, rcPaint.top, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
			_dx_settarget(pContext, pBitmapDisplay);
		}
		else {
			_canvas_bitblt(cvDisplay, ((wnd_s*)pWnd)->canvas_bkg_, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, rcPaint.left, rcPaint.top);
		}

		void* hBrush = nullptr;
		if (Flag_Query(EXGF_OBJECT_SHOWRECTBORDER))
		{
			hBrush = _brush_create(-65536);
		}
		_wnd_render_obj(hWnd, pWnd, pContext, cvDisplay, pBitmapDisplay, rcPaint, ((wnd_s*)pWnd)->objChildFirst_, 0, 0, 255, fDX, hBrush);
		_brush_destroy(hBrush);
		_wnd_render_dc(hWnd, pWnd, hDC, cvDisplay, rcPaint, fLayer);
		_canvas_enddraw(cvDisplay);
	}
	__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendering);
}

void _wnd_wm_size(void* pWnd, HWND hWnd, size_t wParam, int width, int height)
{
	__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendered);
	__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_SIZED | EWF_bRedrawBackground);
	if (width != ((wnd_s*)pWnd)->width_ || height != ((wnd_s*)pWnd)->height_)
	{
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
		{
			窗口_添加风格(hWnd, WS_EX_LAYERED, true);
		}

		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
		{
			width = width - GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
			height = height - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
		}
		_wnd_recalcclient(pWnd, hWnd, width, height);
		_layout_update(((wnd_s*)pWnd)->hLayout_);
		RECT rcCaption;
		_wnd_calc_captionrect(pWnd, &rcCaption);
		Ex_ObjSetPos(((wnd_s*)pWnd)->objCaption_, 0, 0, 0, rcCaption.right - rcCaption.left, EOP_DEFAULT, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_EX_NODPISCALE);
		if (((wnd_s*)pWnd)->dwWinState_ != wParam)
		{
			if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_BUTTON_MAX))
			{
				Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, EWS_BUTTON_MAX), 0);
			}
			if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_BUTTON_MIN))
			{
				Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, EWS_BUTTON_MIN), 0);
			}
			((wnd_s*)pWnd)->dwWinState_ = wParam;
		}
	}
	InvalidateRect(hWnd, 0, false);
}

void _wnd_menu_setpos(HWND hWnd, void* pWnd, size_t lParam)
{
	void* pMenuHostWnd = ((wnd_s*)pWnd)->pMenuHostWnd_;
	void* pMenuPrevWnd = nullptr;
	RECT rcParent;
	if (pMenuHostWnd != 0)
	{
		pMenuPrevWnd = ((wnd_s*)pMenuHostWnd)->pMenuPrevWnd_;
		if (pMenuPrevWnd != 0)
		{
			GetWindowRect(((wnd_s*)pMenuPrevWnd)->hWnd_, &rcParent);
		}
	}
	auto offset = 取低位(取高位(((wnd_s*)pWnd)->szItemSeparator_));
	int x = __get_int((void*)lParam, 8);
	int y = __get_int((void*)lParam, 12);
	POINT pt;
	GetCursorPos(&pt);
	if (rcParent.left < x)//子菜单在右边
	{
		x = rcParent.right + offset - 1;
	}
	else {
		//子菜单在左边
		x = rcParent.left;
		GetWindowRect(hWnd, &rcParent);
		x = x - (rcParent.right - rcParent.left) + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 1;
		x = x - offset;
	}
	__set_int((void*)lParam, 8, x);
	if (pMenuPrevWnd != 0)
	{
		size_t hObj = ((wnd_s*)pMenuPrevWnd)->objFocus_;
		void* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
		{
			void* padding_client = ((wnd_s*)pWnd)->padding_client_;
			y = ((obj_s*)pObj)->w_top_ + ((wnd_s*)pMenuPrevWnd)->top_ - __get_int(padding_client, 4);
			GetWindowRect(hWnd, &rcParent);
			int height = rcParent.bottom - rcParent.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
			窗口_取屏幕矩形(hWnd, &rcParent);
			if (y + height > rcParent.bottom)
			{
				y = ((obj_s*)pObj)->w_bottom_ - height + __get_int(padding_client, 12);
			}
		}
	}
	__set_int((void*)lParam, 12, y);
}

void _wnd_menu_createitems(HWND hWnd, void* pWnd)
{
	size_t hMenu = SendMessageW(hWnd, 481, 0, 0);
	void* hTheme = ((wnd_s*)pWnd)->hTheme_;
	void* lpPaddingText = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_TEXT);
	int nCount = GetMenuItemCount((HMENU)hMenu) - 1;
	void* pMenuHostWnd = ((wnd_s*)pWnd)->pMenuHostWnd_;
	HWND hParent = ((wnd_s*)pMenuHostWnd)->hWnd_;
	RECT rcParent;
	GetWindowRect(hParent, &rcParent);
	void* padding_client = ((wnd_s*)pWnd)->padding_client_;
	RECT rcPaddingClient = { 0 };
	if (padding_client != 0)
	{
		RtlMoveMemory(&rcPaddingClient, padding_client, 16);
		if (g_Li.DpiX > 1)
		{
			rcPaddingClient.left = rcPaddingClient.left * g_Li.DpiX;
			rcPaddingClient.top = rcPaddingClient.top * g_Li.DpiX;
			rcPaddingClient.right = rcPaddingClient.right * g_Li.DpiX;
			rcPaddingClient.bottom = rcPaddingClient.bottom * g_Li.DpiX;
		}
	}

	void* pParnet = nullptr;
	int nError = 0;
	size_t objParent = _obj_create_init(hWnd, pWnd, ATOM_PAGE, 0, &pParnet, &nError);
	if (objParent != 0)
	{
		int width = ((wnd_s*)pWnd)->width_ - (rcPaddingClient.left + rcPaddingClient.right);
		int height = ((wnd_s*)pWnd)->height_ - (rcPaddingClient.top + rcPaddingClient.bottom);
		_obj_create_proc(&nError, false, hTheme, pParnet, EOS_EX_FOCUSABLE, ATOM_PAGE, 0, EOS_VISIBLE | EOS_VSCROLL | 条目风格_子菜单, rcPaddingClient.left, rcPaddingClient.top, width, height, 0, 0, 0, 0, 0);
		_obj_create_done(hWnd, pWnd, objParent, pParnet);
		size_t objPP = objParent;
		size_t hLayout = _layout_create(ELT_ABSOLUTE, ((wnd_s*)pWnd)->hexdui_);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.left);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.top);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.right);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.bottom);

		objParent = _obj_create_init(hWnd, pWnd, ATOM_PAGE, 0, &pParnet, &nError);
		if (objParent != 0)
		{
			int heightParent = height;
			_obj_create_proc(&nError, false, hTheme, pParnet, EOS_EX_FOCUSABLE, ATOM_PAGE, 0, EOS_VISIBLE, 0, 0, width, height, objPP, 0, 0, 0, 0);
			_obj_create_done(hWnd, pWnd, objParent, pParnet);
			hLayout = _layout_create(ELT_ABSOLUTE, objPP);
			_layout_absolute_setedge(hLayout, objParent, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 0);
			_layout_absolute_setedge(hLayout, objParent, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 0);
			_layout_absolute_setedge(hLayout, objParent, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, 0);
			_layout_absolute_setedge(hLayout, objParent, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 0);
			height = 0;
			MENUITEMINFOW mii;
			mii.cbSize = sizeof(MENUITEMINFOW);
			mii.fMask = MIIM_FTYPE | MIIM_SUBMENU | MIIM_ID;
			RECT rcItem;
			int eos;
			for (int i = 0; i < nCount; i++)
			{
				if (GetMenuItemRect(hParent, (HMENU)hMenu, i, &rcItem))
				{
					eos = EOS_VISIBLE;
					if (GetMenuItemInfoW((HMENU)hMenu, i, true, &mii))
					{
						if ((mii.fType & MFT_SEPARATOR) != 0)//分隔符
						{
							eos = eos | 条目风格_分隔符;
						}
						else {
							if (mii.hSubMenu != 0)
							{
								eos = eos | 条目风格_子菜单;
							}
						}
					}
					std::vector<wchar_t> bin;
					bin.resize(520);
					OffsetRect(&rcItem, -rcParent.left, -rcParent.top);
					GetMenuStringW((HMENU)hMenu, i, bin.data(), 520, MF_BYPOSITION);
					void* pObj = nullptr;
					size_t hObj = _obj_create_init(hWnd, pWnd, ATOM_ITEM, 0, &pObj, &nError);
					if (hObj != 0)
					{
						_obj_create_proc(&nError, false, hTheme, pObj, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, ATOM_ITEM, bin.data(), eos, rcItem.left, rcItem.top, width, rcItem.bottom - rcItem.top, objParent, mii.wID, 0, i, DT_VCENTER | DT_SINGLELINE);
						__add(pObj, offsetof(obj_s, dwFlags_), eof_bMenuItem);
						_obj_create_done(hWnd, pWnd, hObj, pObj);
						if (lpPaddingText != 0)
						{
							RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, t_left_)), lpPaddingText, 16);
							if (g_Li.DpiX > 1)
							{
								((obj_s*)pObj)->t_left_ = ((obj_s*)pObj)->t_left_ * g_Li.DpiX;
								((obj_s*)pObj)->t_top_ = ((obj_s*)pObj)->t_top_ * g_Li.DpiX;
								((obj_s*)pObj)->t_right_ = ((obj_s*)pObj)->t_right_ * g_Li.DpiX;
								((obj_s*)pObj)->t_bottom_ = ((obj_s*)pObj)->t_bottom_ * g_Li.DpiX;
							}
						}
					}
					height = height + rcItem.bottom - rcItem.top;
				}
			}
			_obj_setpos_org(pParnet, objParent, 0, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_EX_NODPISCALE, 0);
			height = height - 1;
			Ex_objScrollshow(objPP, SB_VERT, height - heightParent > 0);
			Ex_ObjScrollSetInfo(objPP, SB_VERT, SIF_RANGE | SIF_PAGE, 0, height - heightParent, heightParent, 0, true);
		}
	}
	SendMessageW(hWnd, WM_INITMENUPOPUP, hMenu, 0);
}

void _wnd_menu_init(HWND hWnd, void* pWnd)
{
	if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuInited))
	{
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuInited);
		_wnd_menu_createitems(hWnd, pWnd);
		ShowWindow(((wnd_s*)pWnd)->hWndShadow_, SW_SHOWNOACTIVATE);
		SetWindowLongPtrW(hWnd, -16, WS_POPUP | WS_VISIBLE);
		InvalidateRect(hWnd, 0, 0);
	}
}

void _wnd_paint_shadow(void* pWnd, bool bUpdateRgn, bool bFlush)
{
	if (!__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_NOSHADOW))
	{
		if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendered))
		{
			HWND hWnd = ((wnd_s*)pWnd)->hWndShadow_;
			POINT ptDst;
			ptDst.x = ((wnd_s*)pWnd)->left_;
			ptDst.y = ((wnd_s*)pWnd)->top_;
			SIZE sz;
			sz.cx = ((wnd_s*)pWnd)->width_;
			sz.cy = ((wnd_s*)pWnd)->height_;
			void* prcPadding = ((wnd_s*)pWnd)->padding_shadow_;
			RECT rcPadding;
			if (prcPadding != 0)
			{
				RtlMoveMemory(&rcPadding, prcPadding, 16);
				rcPadding.left = Ex_Scale(rcPadding.left);
				rcPadding.top = Ex_Scale(rcPadding.top);
				rcPadding.right = Ex_Scale(rcPadding.right);
				rcPadding.bottom = Ex_Scale(rcPadding.bottom);
				ptDst.x = ptDst.x - rcPadding.left;
				ptDst.y = ptDst.y - rcPadding.top;
				sz.cx = sz.cx + rcPadding.left + rcPadding.right;
				sz.cy = sz.cy + rcPadding.top + rcPadding.bottom;
			}
			MoveWindow(hWnd, ptDst.x, ptDst.y, sz.cx, sz.cy, false);
			if (bUpdateRgn)
			{
				auto rectround = ((wnd_s*)pWnd)->radius_;
				HRGN hRgn = CreateRectRgn(0, 0, sz.cx, sz.cy);
				HRGN hRgnClient;
				if (rectround == 0)
				{
					hRgnClient = CreateRectRgn(rcPadding.left, rcPadding.top, sz.cx - rcPadding.right, sz.cy - rcPadding.bottom);
				}
				else {
					hRgnClient = CreateRoundRectRgn(rcPadding.left, rcPadding.top, sz.cx - rcPadding.right, sz.cy - rcPadding.bottom, rectround, rectround);
				}
				HRGN hRgnNC = CreateRectRgn(0, 0, 0, 0);
				CombineRgn(hRgnNC, hRgn, hRgnClient, 3);
				SetWindowRgn(hWnd, hRgnNC, 1);
				DeleteObject(hRgn);
				DeleteObject(hRgnClient);
				DeleteObject(hRgnNC);
			}
			if (bUpdateRgn || bFlush)
			{
				auto hDC = GetDC(hWnd);
				if (hDC != 0)
				{
					size_t cvShadow = _canvas_createfrompwnd(pWnd, sz.cx, sz.cy, CVF_GDI_COMPATIBLE, 0);
					if (cvShadow != 0)
					{
						int alpha = __query(pWnd, offsetof(wnd_s, dwFlags_), EWF_ACTIVE) ? 255 : 204;
						if (_canvas_begindraw(cvShadow))
						{
							Ex_ThemeDrawControlEx(((wnd_s*)pWnd)->hTheme_, cvShadow, 1, 1, sz.cx - 2, sz.cy - 2, (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU) ? ATOM_MENU : ATOM_WINDOW), ATOM_RECT, 0, 0, ATOM_BACKGROUND_GRID, 0, alpha);
							void* mDC = _canvas_getdc(cvShadow);
							if (mDC != 0)
							{
								POINT ptSrc;
								BLENDFUNCTION pb_;
								pb_.BlendOp = 0;
								pb_.BlendFlags = 0;
								pb_.SourceConstantAlpha = 0xFF;
								pb_.AlphaFormat = 1;
								UpdateLayeredWindow(hWnd, hDC, &ptDst, &sz, (HDC)mDC, &ptSrc, 0, &pb_, ULW_ALPHA);
								_canvas_releasedc(cvShadow);
							}
							_canvas_enddraw(cvShadow);
						}
						_canvas_destroy(cvShadow);
					}
					ReleaseDC(hWnd, hDC);
				}
			}
		}
	}
}

bool _wnd_wm_paint(void* pWnd, HWND hWnd)
{
	PAINTSTRUCT ps;
	
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_INTED))
	{
		
		bool fSized = __query(pWnd, offsetof(wnd_s, dwFlags_), EWF_SIZED);
		if (fSized)
		{
			__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_SIZED);
		}
		bool fLayer = __query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLayered);
		
		if (fLayer)
		{
			if (fSized)
			{
				RtlMoveMemory(&ps.rcPaint.right, (void*)((size_t)pWnd + offsetof(wnd_s, width_)), 8);
			}
			else {
				GetUpdateRect(hWnd, &ps.rcPaint, false);
				if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
				{

				}
			}
		}
		else {
			BeginPaint(hWnd, &ps);
		}
		_wnd_render(hWnd, pWnd, ps.hdc, ps.rcPaint, fLayer, true);
		if (!fLayer)
		{
			EndPaint(hWnd, &ps);
		}

		if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendered))
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRendered);
			_wnd_paint_shadow(pWnd, true, false);
		}
	}
	return false;
}

bool _wnd_wm_getminmaxinfo(void* pWnd, HWND hWnd, size_t lParam)
{
	RECT rcMonitor, rcDesk;
	bool ret = false;
	if (窗口_取屏幕矩形(hWnd, &rcMonitor, &rcDesk))
	{
		int width = ((wnd_s*)pWnd)->width_;
		int height = ((wnd_s*)pWnd)->height_;
		if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_FULLSCREEN))//全屏模式
		{
			RtlMoveMemory(&rcDesk, &rcMonitor, 16);
		}
		OffsetRect(&rcDesk, -rcMonitor.left, -rcMonitor.top);
		//左边，顶边
		__set_int((void*)lParam, 16, rcDesk.left - 1);
		__set_int((void*)lParam, 20, rcDesk.top - 1);

		//最大宽度，高度
		int nMaxWidth = rcDesk.right - rcDesk.left + 2;
		int nMaxHeight = rcDesk.bottom - rcDesk.top + 2;
		__set_int((void*)lParam, 8, nMaxWidth);
		__set_int((void*)lParam, 12, nMaxHeight);

		//允许调整的最小尺寸
		__set_int((void*)lParam, 24, ((wnd_s*)pWnd)->minWidth_);
		__set_int((void*)lParam, 28, ((wnd_s*)pWnd)->minHeight_);
		//允许调整的最大尺寸
		__set_int((void*)lParam, 32, nMaxWidth);
		__set_int((void*)lParam, 36, nMaxHeight);
		ret = true;
	}
	return ret;
}

void _wnd_wm_buttondown(HWND hWnd, void* pWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	if (!__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
	{
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeftTrack | EWF_bRightTrack | EWF_bMidTrack);
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject);
		if (uMsg == WM_LBUTTONDOWN)
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeftTrack);
			_obj_setfocus(hWnd, pWnd, hObj, pObj, true);
		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRightTrack);
		}
		else {
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMidTrack);
		}
		((wnd_s*)pWnd)->objTrack_ = hObj;
		_obj_tooltips_popup(pWnd, 0, 0, -1, -1, -1, 0, false);
		_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
		if (wParam != 0)
		{
			SetCapture(hWnd);//对menu窗口无效
		}
	}
}

void _wnd_obj_untrack(HWND hWnd, void* pWnd, bool fMsgDispatch)
{
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
	{
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject);
		size_t objTrack = ((wnd_s*)pWnd)->objTrack_;
		((wnd_s*)pWnd)->objTrack_ = 0;
		((wnd_s*)pWnd)->objTrackPrev_ = objTrack;
		if (!fMsgDispatch)
		{
			ReleaseCapture();//对menu窗口无效
		}
		void* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objTrack, HT_OBJECT, &pObj, &nError))
		{
			POINT pt;
			GetCursorPos(&pt);
			int uMsg;
			if (fMsgDispatch)
			{
				if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeftTrack))
				{
					uMsg = WM_LBUTTONUP;
				}
				else if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRightTrack))
				{
					uMsg = WM_RBUTTONUP;
				}
				else {
					uMsg = WM_MBUTTONUP;
				}
				_obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, 0);
				_wnd_wm_nchittest(pWnd, hWnd, 合并整数(pt.x, pt.y));
				_wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, (void*)-1, false);
			}
			else {
				_wnd_wm_nchittest(pWnd, hWnd, 合并整数(pt.x, pt.y));
				_wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, (void*)-1, false);
				if (((wnd_s*)pWnd)->objHittest_ == objTrack)
				{
					if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bLeftTrack))
					{
						uMsg = WM_EX_LCLICK;
					}
					else if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRightTrack))
					{
						uMsg = WM_EX_RCLICK;
					}
					else {
						uMsg = WM_EX_MCLICK;
					}
					_obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, ((wnd_s*)pWnd)->dwHitObjPos_Abs_);
				}
			}
			_obj_dispatchnotify(hWnd, pObj, objTrack, 0, NM_RELEASEDCAPTURE, 0, 0);
		}
	}
}

void _wnd_wm_captionchange(HWND hWnd, void* pWnd)
{
	_wnd_obj_untrack(hWnd, pWnd, false);
}

void _wnd_wm_mouse(void* pWnd, HWND hWnd, int uMsg, size_t wParam, size_t lParam)
{
	size_t hObj;
	if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))//是否按住组件
	{
		hObj = ((wnd_s*)pWnd)->objTrack_;
	}
	else {
		hObj = ((wnd_s*)pWnd)->objHittest_;
	}
	int dwHitCode = ((wnd_s*)pWnd)->dwHitCode_;
	void* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
	{
		if (__query(pWnd, offsetof(wnd_s, dwFlags_), EWF_bTrackObject))
		{
			lParam = 合并整数(取低位(lParam) - ((obj_s*)pObj)->w_left_, 取高位(lParam) - ((obj_s*)pObj)->w_top_);
			((wnd_s*)pWnd)->dwHitObjPos_Abs_ = lParam;
		}
		else {
			lParam = ((wnd_s*)pWnd)->dwHitObjPos_Abs_;
		}
	}

	if (uMsg == WM_MOUSEMOVE)
	{
		if (wParam == 0)
		{
			_wnd_wm_leavecheck(hWnd, pWnd, ((wnd_s*)pWnd)->objHittestPrev_, hObj, pObj, true);
		}
		if (hObj != 0)
		{
			_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
	}
	else {
		if (pObj != 0)
		{
			if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN)
			{
				_wnd_wm_buttondown(hWnd, pWnd, hObj, pObj, uMsg, wParam, lParam);
			}
			else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP)
			{
				_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
				_wnd_obj_untrack(hWnd, pWnd, false);
			}
			else if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK)
			{
				_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
				if (uMsg == WM_LBUTTONDBLCLK)
				{
					uMsg = WM_LBUTTONDOWN;
				}
				else if (uMsg == WM_RBUTTONDBLCLK)
				{
					uMsg = WM_RBUTTONDOWN;
				}
				else {
					uMsg = WM_MBUTTONDOWN;
				}
				_wnd_wm_buttondown(hWnd, pWnd, hObj, pObj, uMsg, wParam, lParam);
			}
		}
	}
}

void _wnd_wm_menucontext(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t lParam)
{
	size_t objHittest = ((wnd_s*)pWnd)->objHittest_;
	void* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(objHittest, HT_OBJECT, &pObj, &nError))
	{
		((wnd_s*)pWnd)->objMenucontext_ = objHittest;
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuContext);
		_obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam);
		__del(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuContext);
	}
}

void _wnd_wm_mousewheel(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t lParam)
{
	size_t objHittest = ((wnd_s*)pWnd)->objHittest_;
	void* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(objHittest, HT_OBJECT, &pObj, &nError))
	{
		if (_obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam) == 0)//我认为这样是不完善的，因为存在转发WHEEL消息让别的控件滚动的可能性
		{
			void* pCls = ((obj_s*)pObj)->pCls_;
			if (((class_s*)pCls)->atomName_ == ATOM_SCROLLBAR)
			{
				_sb_parentnotify(hWnd, pObj, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, objHittest, 0, true);
			}
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_VSCROLL))
			{
				_obj_baseproc(hWnd, objHittest, pObj, WM_VSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
			}
			if (__query(pObj, offsetof(obj_s, dwStyle_), EOS_HSCROLL))
			{
				_obj_baseproc(hWnd, objHittest, pObj, WM_HSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
			}
		}
	}
}

void _wnd_wm_command(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t lParam)
{
	if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
	{
		if (wParam == 2)
		{
			if (Ex_ObjIsEnable(Ex_ObjGetFromID(((wnd_s*)pWnd)->objCaption_, EWS_BUTTON_CLOSE)))
			{
				EndDialog(hWnd, IDCANCEL);
			}
		}
	}
	else {
		size_t objMenucontext = ((wnd_s*)pWnd)->objMenucontext_;
		((wnd_s*)pWnd)->objMenucontext_ = 0;
		void* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objMenucontext, HT_OBJECT, &pObj, &nError))
		{
			_obj_baseproc(hWnd, objMenucontext, pObj, uMsg, wParam, lParam);
		}
	}
}

size_t _wnd_obj_setnextfocus(HWND hWnd, void* pWnd, size_t objEntry, size_t objLastFocus)
{
	void* pObjEntry = nullptr;
	int nError = 0;
	while (_handle_validate(objEntry, HT_OBJECT, &pObjEntry, &nError))
	{
		if (__query(pObjEntry, offsetof(obj_s, dwStyleEx_), EOS_EX_TABSTOP))
		{
			if (objLastFocus != objEntry)
			{
				_obj_setfocus(hWnd, pWnd, objEntry, pObjEntry, true);
			}
			return objEntry;
		}
		objEntry = ((obj_s*)pObjEntry)->objNext_;
	}
	return 0;
}

void _wnd_obj_settabstop(HWND hWnd, void* pWnd, size_t objLastFocus)
{
	void* pObjLast = nullptr;
	int nError = 0;
	size_t objNextFocus;
	if (_handle_validate(objLastFocus, HT_OBJECT, &pObjLast, &nError))
	{
		size_t objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, ((obj_s*)pObjLast)->objNext_, objLastFocus);
		if (objNextFocus == 0)
		{
			size_t objParent = ((obj_s*)pObjLast)->objParent_;
			void* pObjParent = nullptr;
			if (_handle_validate(objParent, HT_OBJECT, &pObjParent, &nError))
			{
				objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, ((obj_s*)pObjParent)->objChildFirst_, objLastFocus);
			}
			else {
				objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, ((wnd_s*)pWnd)->objChildFirst_, objLastFocus);
			}
		}
	}
	else {
		objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, ((wnd_s*)pWnd)->objChildFirst_, objLastFocus);
	}
}

void _wnd_wm_keyboard(void* pWnd, HWND hWnd, int uMsg, size_t wParam, size_t lParam)
{
	size_t objFocus = ((wnd_s*)pWnd)->objFocus_;
	void* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(objFocus, HT_OBJECT, &pObj, &nError))
	{
		if (_obj_baseproc(hWnd, objFocus, pObj, uMsg, wParam, lParam) != 0)//先给组件，看他需不需要消息。
		{
			return;
		}
	}
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)
		{
			if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_ESCEXIT))
			{
				PostMessageW(hWnd, WM_CLOSE, 0, 0);
			}
		}
		else if (wParam == VK_SPACE)
		{
			if (窗口_取功能键() == 0)
			{
				if (pObj != 0)
				{
					_wnd_wm_buttondown(hWnd, pWnd, objFocus, pObj, WM_LBUTTONDOWN, 0, 0);
				}
			}
		}
		else if (wParam == VK_RETURN)
		{
			if (窗口_取功能键() == 0)
			{
				if (pObj != 0)
				{
					if (_obj_dispatchnotify(hWnd, pObj, objFocus, 0, NM_RETURN, wParam, lParam) == 0)
					{
						_obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
					}
				}
			}
		}
		else if (wParam == VK_TAB)
		{
			_wnd_obj_settabstop(hWnd, pWnd, objFocus);
		}
		else if (wParam == VK_UP || wParam == VK_DOWN)
		{
			//if (__query(pWnd, offsetof(wnd_s, dwStyle_), EWS_MENU))
			//{

			//}
		}
		else if (wParam == VK_LEFT || wParam == VK_RIGHT)
		{

		}
	}
	else if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_SPACE)
		{
			if (pObj != 0)
			{
				_wnd_wm_mouse(pWnd, hWnd, WM_LBUTTONUP, 0, 0);
				_obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
			}
		}
	}
}

bool _wnd_obj_childtabstop(size_t objEntry, size_t* objFocusable, void** pObjFocusable, size_t objLast, bool* bFounded)
{
	void* pObj = nullptr;
	int nError = 0;
	bool ret = false;
	while (_handle_validate(objEntry, HT_OBJECT, &pObj, &nError))
	{
		if (!*bFounded)
		{
			if (objLast = objEntry) *bFounded = true;
		}
		if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_TABSTOP))
		{
			*objFocusable = objEntry;
			*pObjFocusable = pObj;
			ret = true;
			break;
		}
		size_t objSub = ((obj_s*)pObj)->objChildFirst_;
		if (objSub != 0)
		{
			if (_wnd_obj_childtabstop(objSub, objFocusable, pObjFocusable, objLast, bFounded))
			{
				ret = true;
				break;
			}
		}
		objEntry = ((obj_s*)pObj)->objNext_;
	}
	return ret;
}

void _wnd_wm_ime_composition(HWND hWnd, void* pWnd)
{
	auto hImc = ImmGetContext(hWnd);
	if (hImc != 0)
	{
		POINT pt;
		GetCaretPos(&pt);
		COMPOSITIONFORM cf;
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos.x = pt.x;
		cf.ptCurrentPos.y = pt.y;
		ImmSetCompositionWindow(hImc, &cf);
		size_t hObj = ((wnd_s*)pWnd)->objFocus_;
		if (hObj != 0)
		{
			void* pObj = nullptr;
			int nError = 0;
			if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
			{
				void* lpLogfont = 申请内存(sizeof(LOGFONTW));
				_font_getlogfont(((obj_s*)pObj)->hFont_, lpLogfont);
				ImmSetCompositionFontW(hImc, (LPLOGFONTW)lpLogfont);
				释放内存(lpLogfont);
			}
		}
		ImmReleaseContext(hWnd, hImc);
	}
}

bool _wnd_wm_measureitem_host(void* pWnd, size_t wParam, size_t lParam)
{
	bool ret = false;
	if (wParam == 0)//menu MEASUREITEMSTRUCT https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-measureitemstruct
	{
		if (__get_int((void*)lParam, 0) == ODT_MENU)
		{
			int nID = __get_int((void*)lParam, 8);
			void* ppc = ((wnd_s*)pWnd)->padding_client_;
			int offset = 0;
			if (ppc != 0)
			{
				offset = offset + Ex_Scale(__get_int(ppc, 0) + __get_int(ppc, 8));
			}
			void* ppt = ((wnd_s*)pWnd)->padding_text_;
			if (ppt != 0)
			{
				offset = offset + Ex_Scale(__get_int(ppt, 0) + __get_int(ppt, 8));
			}
			int szItemSeparator = ((wnd_s*)pWnd)->szItemSeparator_;
			char byte = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_));
			int width, height;
			if (nID == 0)//MF_SEPARATOR
			{
				width = byte + offset;
				byte = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_) + 3);
				height = byte;
			}
			else {
				width = ((wnd_s*)pWnd)->menu_maxwidth_ + offset;
				byte = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_) + 1);
				height = byte;
			}
			__set_int((void*)lParam, 12, width);
			__set_int((void*)lParam, 16, Ex_Scale(height));
			ret = true;
		}
	}
	return ret;
}

void _wnd_menu_updatecurrent(void* pWnd)
{
	void* pHost = ((wnd_s*)pWnd)->pMenuHostWnd_;
	if (pHost != 0)
	{
		((wnd_s*)pHost)->pMenuTrackWnd_ = pWnd;
	}
}

bool _wnd_menu_mouse(HWND hWnd, void* pWnd, int uMsg, size_t wParam, size_t* iItem)
{
	*iItem = -1;
	POINT pt;
	GetCursorPos(&pt);
	_wnd_wm_nchittest(pWnd, hWnd, 合并整数(pt.x, pt.y));
	_wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, 合并整数(pt.x, pt.y));
	void* pObj = nullptr;
	int nError = 0;
	bool ret = false;
	if (_handle_validate(((wnd_s*)pWnd)->objHittest_, HT_OBJECT, &pObj, &nError))
	{
		if (__query(pObj, offsetof(obj_s, dwStyleEx_), EOS_EX_FOCUSABLE))
		{
			if (__query(pObj, offsetof(obj_s, dwFlags_), eof_bMenuItem))
			{
				*iItem = ((obj_s*)pObj)->lParam_;
			}
			ret = true;
		}
	}
	_wnd_menu_updatecurrent(pWnd);
	return ret;
}

bool _wnd_menu_item_callback(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, void** lpResult)
{
	return false;
}

bool _wnd_menu_callback_test(HWND hWnd, size_t hExDui, int uMsg, size_t wParam, size_t lParam, void** lpResult)
{
	if (uMsg == WM_INITMENUPOPUP)
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);
		MoveWindow(hWnd, rc.left, rc.top, 300, 300, false);
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_FTYPE | MIIM_SUBMENU | MIIM_ID;
		int nCount = GetMenuItemCount((HMENU)wParam) - 1;
		std::vector<size_t> aryItems;
		aryItems.resize(nCount);
		int index = 0;
		for (int i = 0; i < nCount; i++)
		{
			if (GetMenuItemInfoW((HMENU)wParam, i, true, &mii))
			{
				if ((mii.fType & MFT_SEPARATOR) == 0)//分隔符
				{
					aryItems[index] = Ex_ObjGetFromID(hExDui, mii.wID);
					index = index + 1;
				}
			}
		}
		if (index > 0)
		{
			size_t objEntry = Ex_ObjGetObj(aryItems[0], GW_HWNDFIRST);
		}
		aryItems.resize(index);
		int radian = 2 * 3.1415926 / index;
		int radius = 70;
		for (int j = 0; j < index; j++)
		{
			Ex_ObjMove(aryItems[j], radius + floor(radius * std::cos(j * radian)), radius + floor(radius * std::cos(j * radian)), 44, 44, false);
			Ex_ObjSetLong(aryItems[j], EOL_OBJPROC, (size_t)&_wnd_menu_item_callback);
			Ex_ObjSetColor(aryItems[j], COLOR_EX_BACKGROUND, -872380161, false);
			Ex_ObjSetRadius(aryItems[j], 0, 0, 0, 15, true);
		}
	}
	else if (uMsg == 485)//MN_SELECTITEM 测试一下MenuBar切换菜单项的可行性
	{
		POINT pt;
		GetCursorPos(&pt);
		size_t hdui = Ex_DUIFromWindow(WindowFromPoint(pt));
		void* pdui = nullptr;
		int nError = 0;
		if (_handle_validate(hdui, HT_DUI, &pdui, &nError))
		{
			ScreenToClient(((wnd_s*)pdui)->hWnd_, &pt);
			size_t hObj = _wnd_wm_nchittest_obj((HWND)((wnd_s*)pdui)->hexdui_, pdui, ((wnd_s*)pdui)->objChildLast_, pt.x, pt.y, 0, 0);
			void* pObj = nullptr;
			if (_handle_validate(hObj, HT_OBJECT, &pObj, &nError))
			{
				if (((obj_s*)pObj)->id_ == 1007)
				{
					//先判断控件类型为菜单栏
					// 分发一条消息给控件,判断鼠标命中的位置是不是还是当前的菜单
					//如果不是了,则EndMenu并弹出新菜单
					//PostMessageW (__get (nCount, #_wnd_hWnd), 31, 0, 0)  ' WM_CANCELMODE 关闭菜单
					EndMenu();
				}
			}
		}
	}
	return false;
}

void _wnd_wm_initmenupopup(HWND hWnd, void* pWnd, void* hMenu)
{
	bool fChecked = false;
	if (!Flag_Query(EXGF_MENU_ALL))
	{
		void* lpMenuParams = ((wnd_s*)pWnd)->lpMenuParams_;
		if (!IsBadReadPtr(lpMenuParams, sizeof(menu_s)))
		{
			if (hMenu == (void*)__get(lpMenuParams, 0))
			{
				fChecked = true;
			}
		}
	}
	else {
		fChecked = true;
	}
	if (fChecked)
	{
		int nCount = GetMenuItemCount((HMENU)hMenu) - 1;
		size_t hCanvas = ((wnd_s*)pWnd)->canvas_bkg_;
		void* hFont = ((wnd_s*)pWnd)->hFont_Menu_;
		std::vector<wchar_t> buff;
		buff.resize(520);
		int width, height, nMax = 0;
		for (int i = 0; i < nCount; i++)
		{
			GetMenuStringW((HMENU)hMenu, i, buff.data(), 520, MF_BYPOSITION);
			_canvas_calctextsize(hCanvas, hFont, buff.data(), -1, DT_SINGLELINE, 0, 0, 0, &width, &height);
			if (nMax < width)
			{
				nMax = width;
			}
		}
		((wnd_s*)pWnd)->menu_maxwidth_ = nMax;
		MENUITEMINFOW mii;
		mii.cbSize = sizeof(MENUITEMINFOW);
		mii.fMask = MIIM_FTYPE | MIIM_ID;
		for (int i = 0; i < nCount; i++)
		{
			if (GetMenuItemInfoW((HMENU)hMenu, i, true, &mii))
			{
				if ((mii.fType & MFT_SEPARATOR) != 0)
				{
					mii.wID = 0;
				}
				mii.fType = mii.fType | MFT_OWNERDRAW;
				if (SetMenuItemInfoW((HMENU)hMenu, i, true, &mii))
				{
					continue;
				}
			}
			fChecked = false;
			break;
		}
		if (fChecked)
		{
			if (HashTable_Set(g_Li.hTableLayout, (size_t)hMenu, ((wnd_s*)pWnd)->hexdui_))
			{
				((wnd_s*)pWnd)->hMenuPopup_ = hMenu;
				__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bMenuInited);
			}
		}
	}
}

bool Ex_TrackPopupMenu(void* hMenu, int uFlags, int x, int y, int nReserved, size_t handle, void* lpRC, void* pfnCallback, int dwFlags)
{
	HWND hWnd = 0;
	void* pWnd = nullptr;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd))
	{
		((wnd_s*)pWnd)->lpMenuParams_ = &hMenu;
	}
	bool ret = TrackPopupMenu((HMENU)hMenu, uFlags, x, y, nReserved, hWnd, (RECT*)lpRC);
	return ret;
}

int Ex_MessageBoxEx(size_t handle, void* lpText, void* lpCaption, int uType, void* lpCheckBox, bool* lpCheckBoxChecked, int dwMilliseconds, int dwFlags, void* lpfnMsgProc)
{
	HWND hWnd = 0;
	void* pWnd = nullptr;
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd))
	{
		((wnd_s*)pWnd)->lpMsgParams_ = &handle;
	}
	auto ret = MessageBoxW(hWnd, (LPCWSTR)lpText, (LPCWSTR)lpCaption, uType);
	return ret;
}

int Ex_MessageBox(size_t handle, void* lpText, void* lpCaption, int uType, int dwFlags)
{
	return Ex_MessageBoxEx(handle, lpText, lpCaption, uType, 0, 0, 0, dwFlags, 0);
}

bool Ex_DUISetBlur(size_t hExDui, float fDeviation)
{
	void* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		if (fDeviation > 0)
		{
			__add(pWnd, offsetof(wnd_s, dwStyle_), EWS_BLUR);
			((wnd_s*)pWnd)->fBlur_ = fDeviation;
		}
		else {
			__del(pWnd, offsetof(wnd_s, dwStyle_), EWS_BLUR);
		}
		InvalidateRect(((wnd_s*)pWnd)->hWnd_, 0, false);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool Ex_DUITrayIconPopup(size_t hExDui, void* lpwzInfo, void* lpwzInfoTitle, int dwInfoFlags)
{
	void* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		void* lpNid = ((wnd_s*)pWnd)->nID_;
		if (lpNid != 0)
		{
			((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_INFO;
			RtlZeroMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), 512);
			int len = lstrlenW((LPCWSTR)lpwzInfo);
			if (len > 255) len = 255;
			RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), lpwzInfo, len * 2);
			RtlZeroMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)), 128);
			len = lstrlenW((LPCWSTR)lpwzInfoTitle);
			if (len > 63) len = 63;
			RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)), lpwzInfoTitle, len * 2);
			((NOTIFYICONDATAW*)lpNid)->dwInfoFlags = dwInfoFlags;
			Shell_NotifyIconW(NIM_MODIFY, (PNOTIFYICONDATAW)lpNid);
		}
	}
	return nError == 0;
}

bool Ex_DUITrayIconSet(size_t hExDui, size_t hIcon, void* lpwzTips)
{
	void* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		HWND hWnd = ((wnd_s*)pWnd)->hWnd_;
		int len = lstrlenW((LPCWSTR)lpwzTips);
		void* lpNid = ((wnd_s*)pWnd)->nID_;
		if (lpNid == 0)
		{
			if (hIcon == 0)
			{
				hIcon = 窗口_取图标句柄(hWnd, false);
			}
			void* lpNid = 申请内存(sizeof(NOTIFYICONDATAW));
			((NOTIFYICONDATAW*)lpNid)->cbSize = sizeof(NOTIFYICONDATAW);
			((NOTIFYICONDATAW*)lpNid)->hWnd = hWnd;
			((NOTIFYICONDATAW*)lpNid)->uID = 1;
			((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			((NOTIFYICONDATAW*)lpNid)->uCallbackMessage = g_Li.dwMessage;
			((NOTIFYICONDATAW*)lpNid)->hIcon = (HICON)hIcon;
			if (len != 0)
			{
				if (len > 127) len = 127;
				RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), lpwzTips, len * 2);
			}
			Shell_NotifyIconW(NIM_ADD, (PNOTIFYICONDATAW)lpNid);
			((wnd_s*)pWnd)->nID_ = lpNid;
		}
		else {
			int uFlags = 0;
			if (len != 0)
			{
				if (len > 127) len = 127;
				RtlZeroMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), 127);
				RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), lpwzTips, len * 2);
				uFlags = NIF_TIP;
			}
			if (hIcon != 0)
			{
				((NOTIFYICONDATAW*)lpNid)->hIcon = (HICON)hIcon;
				uFlags = uFlags | NIF_ICON;
			}
			if (uFlags != 0)
			{
				((NOTIFYICONDATAW*)lpNid)->uFlags = uFlags;
				Shell_NotifyIconW(NIM_MODIFY, (PNOTIFYICONDATAW)lpNid);
			}
		}
	}
	Ex_SetLastError(nError);
	return nError = 0;
}

bool Ex_DUIGetBackgroundImage(size_t hExDui, void* lpBackgroundImage)
{
	void* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		if (IsBadWritePtr(lpBackgroundImage, sizeof(bkgimg_s)))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpBackgroundImage, ((wnd_s*)pWnd)->lpBackgroundImage_, sizeof(bkgimg_s));
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _wnd_getclientrect(void* pWnd, void* lpRect, int* nError)
{
	if (IsBadWritePtr(lpRect, 16))
	{
		*nError = ERROR_EX_BAD_SIZE;
	}
	else {
		RtlZeroMemory(lpRect, 16);
		__set_int(lpRect, 8, ((wnd_s*)pWnd)->width_);
		__set_int(lpRect, 12, ((wnd_s*)pWnd)->height_);
		if (Flag_Query(EXGF_DPI_ENABLE))
		{
			__set_int(lpRect, 8, __get_int(lpRect, 8) / g_Li.DpiX);
			__set_int(lpRect, 12, __get_int(lpRect, 12) / g_Li.DpiY);
		}
	}
}

BOOL Ex_DUIGetClientRect(size_t hExDui, void* lpClientRect)
{
	void* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		_wnd_getclientrect(pWnd, lpClientRect, &nError);
	}
	return nError == 0;
}

size_t Ex_DUIGetLong(size_t hExDui, int nIndex)
{
	void* pWnd = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		if (nIndex == EWL_LPARAM)
		{
			ret = ((wnd_s*)pWnd)->lParam_;
		}
		else if (nIndex == EWL_HWND)
		{
			ret = (size_t)((wnd_s*)pWnd)->hWnd_;
		}
		else if (nIndex == EWL_HTHEME)
		{
			ret = (size_t)((wnd_s*)pWnd)->hTheme_;
		}
		else if (nIndex == EWL_CRBKG)
		{
			ret = (size_t)((wnd_s*)pWnd)->crBkg_;
		}
		else if (nIndex == EWL_CRBORDER)
		{
			ret = (size_t)((wnd_s*)pWnd)->crBorder_;
		}
		else if (nIndex == EWL_ALPHA)
		{
			ret = (size_t)((wnd_s*)pWnd)->alpha_;
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_DUISetLong(size_t hExDui, int nIndex, size_t dwNewlong)
{
	void* pWnd = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		bool bRedraw = false;
		if (nIndex == EWL_LPARAM)
		{
			ret = ((wnd_s*)pWnd)->lParam_;
			((wnd_s*)pWnd)->lParam_ = dwNewlong;
		}
		else if (nIndex == EWL_CRBKG)
		{
			ret = (size_t)((wnd_s*)pWnd)->crBkg_;
			((wnd_s*)pWnd)->crBkg_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_CRBORDER)
		{
			ret = (size_t)((wnd_s*)pWnd)->crBorder_;
			((wnd_s*)pWnd)->crBorder_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_ALPHA)
		{
			ret = (size_t)((wnd_s*)pWnd)->alpha_;
			((wnd_s*)pWnd)->alpha_ = dwNewlong;
		}

		if (bRedraw)
		{
			__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_bRedrawBackground);
			_wnd_redraw_bkg(((wnd_s*)pWnd)->hWnd_, pWnd, 0, true, false);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_DUIShowWindowEx(size_t hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags, int uEasing, size_t wParam, size_t lParam)
{
	void* pWnd = nullptr;
	int nError = 0;
	bool ret = false;
	if (_handle_validate(hExDui, HT_DUI, &pWnd, &nError))
	{
		HWND hWnd = ((wnd_s*)pWnd)->hWnd_;
		//刷新组件层
		__add(pWnd, offsetof(wnd_s, dwFlags_), EWF_INTED);
		ShowWindow(hWnd, nCmdShow);
		InvalidateRect(hWnd, NULL, false);
		ret = UpdateWindow(hWnd);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool Ex_DUIShowWindow(size_t hExDui, int nCmdShow, int dwTimer, int dwFrames, int dwFlags)
{
	return Ex_DUIShowWindowEx(hExDui, nCmdShow, dwTimer, dwFrames, dwFlags, 0, 0, 0);
}

size_t Ex_DUIBindWindowEx(HWND hWnd, void* hTheme, int dwStyle, size_t lParam, void* lpfnMsgProc)
{
	int nError = 0;
	void* pWnd = nullptr;
	size_t hExDui = 0;
	if (IsWindow(hWnd))
	{
		pWnd = 申请内存(sizeof(wnd_s));
		if (pWnd != 0)
		{
			hExDui = _handle_create(HT_DUI, pWnd, &nError);
			
			if (hExDui != 0)
			{
				nError = _wnd_create(hExDui, pWnd, hWnd, dwStyle, hTheme, lParam, lpfnMsgProc);
			}
		}
		else {
			nError = ERROR_EX_MEMORY_ALLOC;
		}
	}
	else {
		nError = GetLastError();
	}
	if (nError != 0)
	{
		if (pWnd != 0)
		{
			释放内存(pWnd);
		}
		if (hExDui != 0)
		{
			_handle_destroy(hExDui, &nError);
		}
	}
	Ex_SetLastError(nError);
	return hExDui;
}

size_t Ex_DUIBindWindow(HWND hWnd, void* hTheme, int dwStyle)
{
	return Ex_DUIBindWindowEx(hWnd, hTheme, dwStyle, 0, 0);
}