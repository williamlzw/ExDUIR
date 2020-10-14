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
		FlushInstructionCache(GetCurrentProcess(), lpData,46);//22+24
        #endif
		SetWindowLongPtr(hWnd, -4, (LONG_PTR)lpData);
	}
	else {
		*nError = GetLastError();
	}
	return lpData;
}

bool 窗口_取屏幕矩形(HWND hWnd, RECT *rcMonitor, RECT *rcDesk)
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
	RECT rcWindow, rcParent, rcDest;
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

HWND Ex_WndCreate(HWND hWndParent, void* lpwzClassName, void* lpwzWindowName, int x, int y, int width, int height, int dwStyle, int dwStyleEx)
{
	if (dwStyle == 0) dwStyle = WS_OVERLAPPEDWINDOW;
	if (dwStyleEx == 0) dwStyleEx = 768;
	dwStyleEx = dwStyleEx | WS_EX_LAYERED;
	if (lpwzClassName == 0) lpwzClassName = (void*)g_Li.atomClassName;
	auto hInst = g_Li.hInstance;
	if (IsWindow(hWndParent)) hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, -6);
	auto hWnd = CreateWindowExW(dwStyleEx, (LPCWSTR)lpwzClassName, (LPCWSTR)lpwzWindowName, dwStyle, x, y, width, height, hWndParent, 0, hInst, 0);
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
	void* hRgnClient=_rgn_createfromroundrect(nSizebox, nSizebox, width - nSizebox, height - nSizebox, rectround, rectround);
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
		auto hRgn=CreateRoundRectRgn(0, 0, width, height, rectround, rectround);
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
				if (__query(pWnd, offsetof(wnd_s,dwFlags_), EWF_bLayered))
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
	void* pData = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_PADDING_SHADOW);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->padding_shadow_ = pData;
		((wnd_s*)pWnd)->minWidth_ = Ex_Scale(__get_int(pData, 0) + __get_int(pData, 8));
		((wnd_s*)pWnd)->minHeight_ = Ex_Scale(__get_int(pData, 4) + __get_int(pData, 8));
	}
	pData = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->size_caption_ = pData;
	}
	pData = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_MARGIN);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->margin_caption_ = pData;
	}
	pData = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_RADIUS);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->radius_ = __get_int(pData,0);
	}
	pData = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_BACKGROUND_COLOR);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->crBkg_ = __get_int(pData, 0);
	}
	//菜单
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_CLIENT);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->padding_client_ = pData;
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_TEXT);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->padding_text_ = pData;
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_SEPARATOR);
	if (pData != 0)
	{
		((wnd_s*)pWnd)->padding_separator_ = pData;
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_ITEM);
	WORD szItem;
	if (pData != 0)
	{
		 szItem=MAKEWORD(__get_int(pData, 0), __get_int(pData, 4));
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_SEPARATOR);
	WORD szSeparator;
	if (pData != 0)
	{
		szSeparator = MAKEWORD(__get_int(pData, 0), __get_int(pData, 4));
	}
	((wnd_s*)pWnd)->szItemSeparator_ = 合并整数(szItem, szSeparator);
	void* lpFontFamily = nullptr;
	int dwFontSize = -1;
	int dwFontStyle = -1;
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_SIZE);
	if (pData != 0)
	{
		dwFontSize = __get_int(pData, 0);
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_STYLE);
	if (pData != 0)
	{
		dwFontStyle = __get_int(pData, 0);
	}
	pData = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_FAMILY);
	if (pData != 0)
	{
		lpFontFamily = pData;
	}
	void* pfont=_font_createfromfamily((LPWSTR)lpFontFamily, dwFontSize, dwFontStyle);
	if (pfont != 0)
	{
		_font_destroy(((wnd_s*)pWnd)->hFont_Menu_);
		((wnd_s*)pWnd)->hFont_Menu_ = pfont;
	}
	_wnd_recalcclient(pWnd, hWnd, ((wnd_s*)pWnd)->width_, ((wnd_s*)pWnd)->height_);
}

void _wnd_calc_captionrect(void* pWnd, RECT* rcCaption)
{
	void* lpValue =((wnd_s*)pWnd)->size_caption_;
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
		(*rcCaption).right = (*rcCaption).right- Ex_Scale(__get_int(lpValue, 8));
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
		int nError = 1;
		if (_handle_validate(((wnd_s*)pWnd)->hExDuiParent_, HT_DUI, &ppWnd, &nError))
		{
			void* lpBI = ((wnd_s*)ppWnd)->lpBackgroundImage_;
			if (lpBI != 0)
			{
				void* pDelay = ((bkgimg_s*)lpBI)->lpDelay_;
				if (pDelay != 0)
				{
					_wnd_redraw_bkg(hWnd, pWnd, 0,true, false);
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
			ret=_obj_msgproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
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
		int ret;
		if (((MsgPROC)pfnMsgProc)(hWnd, ((wnd_s*)pWnd)->hexdui_, uMsg, wParam, (void*)lParam, &ret) != 0)
		{
			return ret;
		}
	}
	if (uMsg == g_Li.dwMessage)
	{

	}
	return 0;
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
		int offsetX, offsetY = 0;
		if (Flag_Query(EXGF_DPI_ENABLE) && 取系统主版本号() > 5)
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
	auto tmp1=GetClassLongPtrW(hWnd, -26);
	if ((tmp1 & CS_DROPSHADOW) != 0)
	{
		SetClassLongPtrW(hWnd, -26, (tmp1 & ~CS_DROPSHADOW));
	}
	int tmp2= WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST;
	auto tmp3=CreateWindowExW(tmp2, (LPCWSTR)g_Li.atomSysShadow, 0, WS_POPUP, 0, 0, 0, 0, hWnd, 0, 0, 0);
	((wnd_s*)pWnd)->hWndShadow_ = tmp3;
	int nError = 1;
	Thunkwindow(tmp3, &_wnd_shadow_proc, (void*)(size_t)1, &nError);

	//tips
	HWND hWndTips = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST, L"tooltips_class32", 0, WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
	SendMessageW(hWndTips, 1048, 0, 2048);//TTM_SETMAXTIPWIDTH 支持多行
	((wnd_s*)pWnd)->hWndTips_ = hWndTips;

	void* tmp4=_struct_createfromaddr(pWnd, offsetof(wnd_s, ti_auto_), sizeof(ti_s), &nError);
	((ti_s*)tmp4)->cbSize_ = sizeof(ti_s);
	((ti_s*)tmp4)->uFlags_ = 16 | 0 | 4096;//TTF_SUBCLASS ,TTF_TRANSPARENT ,TTF_PARSELINKS
	((ti_s*)tmp4)->hWnd_ = hWnd;
	((ti_s*)tmp4)->uId_ = tmp4;
	SendMessageW(hWndTips, 1074, 0, (LPARAM)tmp4);//TTM_ADDTOOLW

	void* tmp5 = _struct_createfromaddr(pWnd, offsetof(wnd_s, ti_track_), sizeof(ti_s), &nError);
	((ti_s*)tmp5)->cbSize_ = sizeof(ti_s);
	((ti_s*)tmp5)->uFlags_ = 32 |128| 0 | 4096;// TTF_TRACK ,TTF_ABSOLUTE ,TTF_TRANSPARENT,TTF_PARSELINKS
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
		窗口_添加风格(hWnd, WS_THICKFRAME,false);//强制触发样式被修改事件
		Thunkwindow(hWnd, &_wnd_proc, pWnd, &nError);
		窗口_删除风格(hWnd, WS_THICKFRAME, false);
		IME_Control(hWnd, pWnd, false);
	}
	return nError;
}

size_t Ex_DUIBindWindowEx(HWND hWnd, void* hTheme, int dwStyle, size_t lParam, void* lpfnMsgProc)
{
	int nError = 1;
	if (IsWindow(hWnd))
	{
		void* pWnd = 申请内存(sizeof(wnd_s));
		if (pWnd != 0)
		{
			size_t hExDui=_handle_create(HT_DUI, pWnd, &nError);
			if (hExDui != 0)
			{
				////////
			}
		}
	}
	return 0;
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
	void* pWnd= (void*)__get(pData, 38);
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
		size_t hWndParent =GetWindowLongPtrW(hWnd, -8);
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
	void* pDeviceContext =((wnd_s*)pWnd)->context_;
	((ID2D1DeviceContext*)pDeviceContext)->Release();
}

void _wnd_dx_init(void* pWnd)
{
	_wnd_dx_unint(pWnd);
	void* pDeviceContext = nullptr;
	if (((ID2D1Device*)g_Ri.pD2DDevice)->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext**)&pDeviceContext) == 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
		((wnd_s*)pWnd)->context_ = pDeviceContext;
	}
}

size_t _wnd_wm_nchittest_obj(HWND hWnd, void* pWnd, size_t objLast, int x, int y, int* hitCode, void** pObjMouse)
{
	size_t objPrev = objLast;
	void* pObj = nullptr;
	int nError = 1;
	while (_handle_validate(objPrev,HT_OBJECT,&pObj,&nError))
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
		else if(x>width-nOffset) //鼠标在右边
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
	int nError = 1;
	size_t objCaption = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjCaption, &nError);
	int nMinHeight=0;
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
		_obj_create_proc(&nError, false, hTheme, pObjCaption, (dwStyle & EWS_NOCAPTIONTOPMOST) != 0 ? 0 : -1, ATOM_SYSBUTTON, lpTitle,
			EOS_VISIBLE | EWS_TITLE, rcCaption.left, rcCaption.top, rcCaption.right - rcCaption.left, rcCaption.bottom - rcCaption.top, 0, EWS_TITLE, 0, 0, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		void* lpValue=Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_PADDING_TEXT);
		if (lpValue != 0)
		{
			RtlMoveMemory((void*)((size_t)pObjCaption + offsetof(obj_s, t_left_)), lpValue, 16);
			_scale_n_float(((size_t)pObjCaption + offsetof(obj_s, t_left_)), 4);
		}
		void* lpCr = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_COLOR_NORMAL);
		if (lpCr != 0)
		{
			((obj_s*)pObjCaption)->crNormal_ = __get_int(lpCr,0);
		}
		void* pFamily = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_FAMILY);
		void* pSize = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_SIZE);
		void* pStyle = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_STYLE);
		int Size, Style;
		if (pFamily == 0) pFamily = (void*)-1;
		if (pSize == 0)
		{
			Size =-1;
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
	int nMinWidth=0;
	for (int i = 0; i < 7; i++)
	{
		if ((dwStyle & aryBtn[i]) != 0)
		{
			void* lpValuea=Ex_ThemeGetValuePtr(hTheme, aryAtom[i], ATOM_NORMAL);
			if (lpValuea != 0)
			{
				RECT rcObject;
				RtlMoveMemory(&rcObject, lpValuea, 16);
				rcObject.right = Ex_Scale(rcObject.right - rcObject.left);
				rcObject.bottom = Ex_Scale(rcObject.bottom - rcObject.top);
				left = left - rcObject.right;
				void* pObjTmp = nullptr;
				int nError = 1;
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
	int nError = 1;
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
					RtlMoveMemory(&rcObj,(void*)((size_t)pObj + offsetof(obj_s, left_)), 16);
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
						int alpha=((obj_s*)pObj)->dwAlpha_;
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
									_canvas_alphablend(cvDisplay, ((obj_s*)pObj)->canvas_obj_, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom, rcClip.left - rcObj.left, rcClip.top - rcObj.top,rcClip.right-rcObj.left,rcClip.bottom-rcObj.top,alpha);
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
		auto objHittest=((wnd_s*)pWnd)->objHittest_;
		void* pObj = nullptr;
		int nError = 1;
		if (_handle_validate(objHittest, HT_OBJECT, &pObj, &nError))
		{
			if (_obj_baseproc(hWnd, objHittest, pObj, WM_SETCURSOR, ((wnd_s*)pWnd)->dwHitObjPos_Abs_, 合并整数(((wnd_s*)pWnd)->dwHitCode_, 取高位(lParam))) != 0)
			{
				return true;
			}
		}
	}
	else if(uHitCode == HTCAPTION)
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
				int nError = 1;
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
	int nError = 1;
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

