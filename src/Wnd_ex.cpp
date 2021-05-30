#include "Wnd_ex.h"

LRESULT CALLBACK Wnd_DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor)
{
	WNDCLASSEXW WndClass = { 0 };
	WndClass.cbSize = sizeof(WNDCLASSEXW);
	WndClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//| CS_DBLCLKS
	WndClass.lpfnWndProc = Wnd_DefWindowProcW;
	WndClass.hInstance = g_Li.hInstance;
	WndClass.hCursor = (hCursor == NULL ? g_Li.hCursor : hCursor);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //NULL_BRUSH
	WndClass.hIcon = (hIcon == NULL ? g_Li.hIcon : hIcon);
	WndClass.hIconSm = (hIconsm == NULL ? g_Li.hIconsm : hIconsm);
	WndClass.lpszClassName = lpwzClassName;
	return RegisterClassExW(&WndClass);
}

HEXDUI Ex_DUIFromWindow(HWND hWnd)
{
	return SendMessageW(hWnd, g_Li.dwMessage, EMV_HEXDUI, MAKELONG(EMT_DUI, 0));
}

bool _wnd_getfromhandle(size_t handle, HWND* hWnd, wnd_s** ppWnd, obj_s** ppObj, bool* isObject, int* nError)
{
	if (isObject) {
		*isObject = false;
	}
	if (IsWindow((HWND)handle)) handle = Ex_DUIFromWindow((HWND)handle);

	wnd_s* pWnd = NULL;
	obj_s* pObj = NULL;

	if (_handle_validate(handle, HT_DUI, (void**)&pWnd, nError))
	{
		if (hWnd) {
			*hWnd = pWnd->hWnd_;
		}
		pObj = (obj_s*)pWnd;
	}
	else if (_handle_validate(handle, HT_OBJECT, (void**)&pObj, nError))
	{
		pWnd = (wnd_s*)pObj->pWnd_;
		if (hWnd) {
			*hWnd = pWnd->hWnd_;
		}
		if (isObject) {
			*isObject = true;
		}
	}
	else {
		return false;
	}
	if (ppWnd) {
		*ppWnd = pWnd;
	}
	if (ppObj) {
		*ppObj = pObj;
	}
	return true;
}

size_t _wnd_dispatch_notify(HWND hWnd, wnd_s* pWnd, size_t hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam, obj_s* pObj)
{
	WinMsgPROC pfnMsgProc = pWnd->pfnMsgProc_;
	size_t ret = 0;
	if (pfnMsgProc)
	{
		EX_NMHDR hdr;
		hdr.hObjFrom = hObj;
		hdr.idFrom = nID;
		hdr.lParam = lParam;
		hdr.nCode = nCode;
		hdr.wParam = wParam;
		pfnMsgProc(hWnd, pWnd->hexdui_, WM_NOTIFY, nID, (size_t)&hdr, &ret);
	}

	return ret;
}

void _wnd_redraw_bkg(HWND hWnd, wnd_s* pWnd, void* lpRect, bool bRedrawBkg, bool bUpdate)
{
	if (bRedrawBkg)
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRedrawBackground;
	}
	InvalidateRect(hWnd, (RECT*)lpRect, FALSE);
	if (bUpdate)
	{
		UpdateWindow(hWnd);
	}
}

bool _wnd_querystyle(HWND hWnd, int dwStyle, bool bExStyle)
{
	return (GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE) & dwStyle) != 0;
}

bool _wnd_delstyle(HWND hWnd, int dwStyle, bool bExStyle)
{
	auto ret = GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	if ((ret & dwStyle) != 0)
	{
		ret &= ~dwStyle;
		SetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE, ret);
		return true;
	}
	return false;
}

bool _wnd_addstyle(HWND hWnd, int dwStyle, bool bExStyle)
{
	auto ret = GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE);
	if ((ret & dwStyle) == 0)
	{
		SetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE, ret | dwStyle);
		return true;
	}
	return false;
}

LRESULT _wnd_defaultproc(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

HICON _wnd_geticonhandle(HWND hWnd, bool isbigicon)
{
	HICON ret = (HICON)SendMessageW(hWnd, 127, (isbigicon ? 1 : 0), 0);
	if (ret == 0)
	{
		ret = (HICON)GetClassLongPtrW(hWnd, isbigicon ? -14 : -34);
	}
	return ret;
}

void* Thunkwindow(HWND hWnd, ThunkPROC pfnProc, void* dwData, int* nError)
{
#ifdef _WIN64
	char shellcode[] = "\x48\xB9\x00\x00\x00\x00\x00\x00\x00\x00\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xE0";
		/*
16970E90000 - 48 B9 0000000000000000 - mov rcx,0000000000000000
16970E9000A - 48 B8 0000000000000000 - mov rax,0000000000000000
16970E90014 - FF E0                 - jmp rax
		*/
#else
	char shellcode[] = "\xC7\x44\x24\x04\x00\x00\xC4\x0E\xE9\x73\x47\x09\xF2";
		/*
00970000 - C7 44 24 04 0000C40E  - mov [esp+04],0EC40000
00970008 - E9 734709F2           - jmp F2A04780
		*/
#endif

	size_t len = sizeof(shellcode) + sizeof(EX_THUNK_DATA);
	EX_THUNK_DATA* lpData = (EX_THUNK_DATA*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (lpData != NULL)
	{
		lpData->hWnd = hWnd;
		lpData->Proc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		lpData->dwData = dwData;

		char* lpCode = (char*)(lpData + 1);// lpData + 1 是指向结构体末尾
		RtlMoveMemory(lpCode, shellcode, sizeof(shellcode));
#ifdef _WIN64
		*((PUINT64)(lpCode + 2)) = (UINT64)lpData;
		*((PUINT64)(lpCode + 12)) = (UINT64)pfnProc;
#else
		* ((PUINT32)(lpCode + 4)) = (UINT32)lpData;
		*((PUINT32)(lpCode + 9)) = (UINT32)((UINT32)pfnProc - (UINT32)lpCode - 13);
#endif // _WIN64

		FlushInstructionCache(GetCurrentProcess(), lpData, len);
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (size_t)lpCode);
	}
	else
		*nError = GetLastError();
	return lpData;
}

bool _wnd_getscreenrect(HWND hWnd, RECT* rcMonitor, RECT* rcDesk)
{
	bool ret = false;
	auto hMonitor = MonitorFromWindow(hWnd, 2);
	if (hMonitor != 0)
	{
		MONITORINFO info = { 0 };
		info.cbSize = 40;
		if (GetMonitorInfoW(hMonitor, &info))
		{
			if (rcMonitor) {
				*rcMonitor = info.rcMonitor;
			}
			if (rcDesk) {
				*rcDesk = info.rcWork;
			}
			ret = true;
		}
	}
	if (!ret)
	{
		Ex_SetLastError(GetLastError());
	}
	return ret;
}

std::wstring _wnd_gettitle(HWND hWnd)//OK
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

int _wnd_getfunctionkeys() //OK
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

void IME_Control(HWND hWnd, wnd_s* pWnd, bool bEnable)
{
	ImmAssociateContext(hWnd, (bEnable ? pWnd->hImc_ : 0));
}

void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, BOOL bFullScreen)
{
	RECT rcWindow, rcParent;
	GetWindowRect(hWnd, &rcWindow);
	if (hWndFrom == 0)
	{
		if (bFullScreen)
		{
			_wnd_getscreenrect(hWnd, &rcParent);
		}
		else {
			_wnd_getscreenrect(hWnd, NULL, &rcParent);
		}
	}
	else {
		GetWindowRect(hWndFrom, &rcParent);
	}
	SetWindowPos(hWnd, 0, rcParent.left + (rcParent.right - rcParent.left - (rcWindow.right - rcWindow.left)) / 2, rcParent.top + (rcParent.bottom - rcParent.top - (rcWindow.bottom - rcWindow.top)) / 2, 0, 0, SWP_NOSIZE);
}

WPARAM Ex_WndMsgLoop()
{
	MSG ret = { 0 };
	while (GetMessageW(&ret, 0, 0, 0))
	{
		TranslateMessage(&ret);
		DispatchMessageW(&ret);
	}
	return ret.wParam;
}

HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, INT x, INT y, INT width, INT height, INT dwStyle, INT dwStyleEx)
{
	if (dwStyle == 0) dwStyle = WS_OVERLAPPEDWINDOW;
	if (dwStyleEx == 0) dwStyleEx = 768;
	dwStyleEx = dwStyleEx | WS_EX_LAYERED;
	if (lpwzClassName == 0) lpwzClassName = (LPCWSTR)g_Li.atomClassName;
	HINSTANCE hInst = g_Li.hInstance;

	if (IsWindow(hWndParent)) hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, GWLP_HINSTANCE);


	HWND hWnd = CreateWindowExW(dwStyleEx, lpwzClassName, lpwzWindowName, dwStyle, x, y, width, height, hWndParent, NULL, hInst, NULL);
	if (hWnd != 0)
	{
		SendMessageW(hWnd, 128, 0, (LPARAM)g_Li.hIconsm);
		SendMessageW(hWnd, 128, 1, (LPARAM)g_Li.hIcon);
	}

	return hWnd;
}

void _wnd_recalcclient(wnd_s* pWnd, HWND hWnd, int width, int height)
{
	pWnd->width_ = width;
	pWnd->height_ = height;
	if (_canvas_resize(pWnd->canvas_display_, width, height))
	{
		if (_canvas_resize(pWnd->canvas_bkg_, width, height))//客户区被改变,必定触发背景被改变事件
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRedrawBackground;
		}
	}
	_rgn_destroy(pWnd->hrgn_client_);
	_rgn_destroy(pWnd->hrgn_sizebox_);
	int nSizebox = 0;
	
	if (((pWnd->dwStyle_ & EWS_SIZEABLE) == EWS_SIZEABLE))
	{
		nSizebox = 5;
	}
	auto rectround = pWnd->radius_;
	void* hRgnClient = _rgn_createfromroundrect(nSizebox, nSizebox, width - nSizebox, height - nSizebox, rectround, rectround);
	pWnd->hrgn_client_ = hRgnClient;
	if (nSizebox != 0)
	{
		void* hRgnNC = _rgn_createfromrect(0, 0, width, height);
		void* hRgnSizebox = _rgn_combine(hRgnNC, hRgnClient, RGN_COMBINE_EXCLUDE, 0, 0);
		_rgn_destroy(hRgnNC);
		pWnd->hrgn_sizebox_ = hRgnSizebox;
	}
	if (rectround != 0)//gdi object
	{
		auto hRgn = CreateRoundRectRgn(0, 0, width, height, rectround, rectround);
		SetWindowRgn(hWnd, hRgn, 1);
		DeleteObject(hRgn);
	}
}

bool _wnd_wm_stylechanging(wnd_s* pWnd, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	bool ret = false;
	int styleNew = __get_int((void*)lParam, 4);
	int styleDui = pWnd->dwStyle_;

	if (wParam == -20)
	{
	
		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bLayered;
		}
		else {
			
			if (__query_int((void*)lParam, 4, WS_EX_LAYERED))//待定有问题
			{
				pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bLayered);
			}
			else {
				
				if (((pWnd->dwFlags_ & EWF_bLayered) == EWF_bLayered))
				{
					pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bLayered);
				}
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
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

void _wnd_loadtheme(wnd_s* pWnd, HWND hWnd, HEXTHEME hTheme)
{
	
	EXATOM atom = ((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU) ? ATOM_MENU : ATOM_WINDOW;
	void* pPADDING_SHADOW = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_PADDING_SHADOW);
	if (pPADDING_SHADOW != 0)
	{
		pWnd->padding_shadow_ = pPADDING_SHADOW;
		pWnd->minWidth_ = Ex_Scale(__get_int(pPADDING_SHADOW, 0) + __get_int(pPADDING_SHADOW, 8));
		pWnd->minHeight_ = Ex_Scale(__get_int(pPADDING_SHADOW, 4) + __get_int(pPADDING_SHADOW, 8));
	}
	void* pSIZE = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);

	if (pSIZE != 0)
	{
		pWnd->size_caption_ = pSIZE;
	}
	void* pMARGIN = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_MARGIN);

	if (pMARGIN != 0)
	{
		pWnd->margin_caption_ = pMARGIN;
	}
	void* pRADIUS = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_RADIUS);

	if (pRADIUS != 0)
	{
		pWnd->radius_ = __get_int(pRADIUS, 0);

	}

	void* pBACKGROUND_COLOR = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_BACKGROUND_COLOR);

	if (pBACKGROUND_COLOR != 0)
	{

		pWnd->crBkg_ = __get_int(pBACKGROUND_COLOR, 0);

	}
	//菜单
	void* pPADDING_CLIENT = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_CLIENT);
	if (pPADDING_CLIENT != 0)
	{
		pWnd->padding_client_ = pPADDING_CLIENT;
	}
	void* pPADDING_TEXT = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_TEXT);
	if (pPADDING_TEXT != 0)
	{
		pWnd->padding_text_ = pPADDING_TEXT;
	}
	void* pPADDING_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_SEPARATOR);
	if (pPADDING_SEPARATOR != 0)
	{
		pWnd->padding_separator_ = pPADDING_SEPARATOR;
	}
	void* pSIZE_ITEM = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_ITEM);
	WORD szItem = 0;
	if (pSIZE_ITEM != 0)
	{
		szItem = MAKEWORD(__get_int(pSIZE_ITEM, 0), __get_int(pSIZE_ITEM, 4));
	}
	void* pSIZE_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_SIZE_SEPARATOR);
	WORD szSeparator = 0;
	if (pSIZE_SEPARATOR != 0)
	{
		szSeparator = MAKEWORD(__get_int(pSIZE_SEPARATOR, 0), __get_int(pSIZE_SEPARATOR, 4));
	}

	pWnd->szItemSeparator_ = MAKELONG(szItem, szSeparator);
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

	HEXFONT pfont = _font_createfromfamily((LPWSTR)lpFontFamily, dwFontSize, dwFontStyle);

	if (pfont != 0)
	{
		_font_destroy(pWnd->hFont_Menu_);
		pWnd->hFont_Menu_ = pfont;
	}
	_wnd_recalcclient(pWnd, hWnd, pWnd->width_, pWnd->height_);
}

void _wnd_calc_captionrect(wnd_s* pWnd, RECT* rcCaption)
{
	void* lpValue = pWnd->size_caption_;
	if (lpValue != 0)
	{
		(*rcCaption).right = __get_int(lpValue, 0) / 100 * pWnd->width_;//保证百分比
		(*rcCaption).bottom = Ex_Scale(__get_int(lpValue, 4));//绝对高度
	}
	lpValue = pWnd->margin_caption_;
	if (lpValue != 0)
	{
		(*rcCaption).left = Ex_Scale(__get_int(lpValue, 0));
		(*rcCaption).top = Ex_Scale(__get_int(lpValue, 4));
		(*rcCaption).right = (*rcCaption).right - Ex_Scale(__get_int(lpValue, 8));
	}
	(*rcCaption).bottom = (*rcCaption).bottom + (*rcCaption).top;
}


void CALLBACK _wnd_backgroundimage_timer_inherit(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_BKG_INHERIT);
	
	if (!((pWnd->dwStyle_ & EWS_NOINHERITBKG) == EWS_NOINHERITBKG))
	{
		wnd_s* ppWnd = nullptr;
		int nError = 0;
		if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (void**)&ppWnd, &nError))
		{
			EX_BACKGROUNDIMAGEINFO* lpBI = (EX_BACKGROUNDIMAGEINFO*)ppWnd->lpBackgroundImage_;
			if (lpBI)
			{
				int* pDelay = (int*)lpBI->lpDelay;
				if (pDelay != 0)
				{
					_wnd_redraw_bkg(hWnd, pWnd, 0, true, false);
					UpdateWindow(hWnd);
					SetTimer(hWnd, idEvent, pDelay[lpBI->curFrame] * 10, _wnd_backgroundimage_timer_inherit);
				}
			}
		}
	}
}

size_t _wnd_dispatch_msg(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT16 nType = LOWORD(lParam);
	size_t ret = 0;
	if (nType == EMT_OBJECT)//组件消息
	{
		ret = _wnd_dispatch_msg_obj(hWnd, (mempoolmsg_s*)wParam, 0, 0, 0, 0);
	}
	else if (nType == EMT_DUI)//窗体消息
	{
		if (wParam == EMV_HEXDUI)
		{
			return pWnd->hexdui_;
		}
		else if (wParam == EMV_PFN_PACK)
		{
			///////////return (size_t)(&_res_pack);
		}
	}
	else if (nType >= WM_MOUSEFIRST && nType <= WM_MOUSELAST || nType >= 1024 && nType <= 1027)
	{
		_wnd_dispatch_notify(hWnd, pWnd, pWnd->hexdui_, -1, NM_TRAYICON, wParam, lParam, 0);
	}
	else if (nType == EMT_LAYOUT_UPDATE)
	{
		_layout_update(wParam);
	}
	else if (nType == EMT_EASING)
	{
		_wnd_dispatch_notify(hWnd, pWnd, pWnd->hexdui_, -1, NM_EASING, __get((void*)wParam, 0), wParam, 0);
	}
	return ret;
}

size_t _wnd_dispatch_msg_obj(HWND hWnd, mempoolmsg_s* lpData, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	size_t ret = 0;
	if (MemPool_AddressIsUsed(lpData))
	{
		pObj = lpData->pObj;
		uMsg = lpData->uMsg;
		wParam = lpData->wParam;
		lParam = lpData->lParam;
		if (pObj)
		{
			ret = _obj_msgproc(hWnd, pObj->hObj_, pObj, uMsg, wParam, lParam);
		}
		MemPool_Free(g_Li.hMemPoolMsg, lpData);
	}
	return ret;
}

LRESULT CALLBACK _wnd_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = pData->hWnd;
	WNDPROC pOld = pData->Proc;
	wnd_s* pWnd = (wnd_s*)pData->dwData;
	WinMsgPROC pfnMsgProc = pWnd->pfnMsgProc_;
	if (pfnMsgProc != 0)
	{
		size_t ret = 0;
		if (pfnMsgProc(hWnd, pWnd->hexdui_, uMsg, wParam, lParam, &ret) != 0)
		{
			return ret;
		}
	}

	if (uMsg == g_Li.dwMessage)
	{
		return _wnd_dispatch_msg(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_NCHITTEST)//132
	{
		return _wnd_wm_nchittest(pWnd, hWnd, lParam);
	}
	else if (uMsg == WM_SETCURSOR)//32
	{
		if (_wnd_wm_setcursor(hWnd, pWnd, lParam))
		{
			return 1;
		}
	}
	else if (uMsg == WM_CLOSE)
	{
		HWND hwndParent = (HWND)GetWindowLongPtrW(hWnd, GWLP_HWNDPARENT);//防止消息框子窗口或组合框子窗口关闭使主窗口失去激活
		if (hwndParent)
		{
			if (GetActiveWindow() != hwndParent)
			{
				SetActiveWindow(hwndParent);
			}
		}
	}
	else if (uMsg == WM_DESTROY)//2
	{
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (size_t)pOld);
		if (_wnd_destroy(hWnd, pWnd) != 0)
		{
			PostQuitMessage(0);
		}
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	else if (uMsg == WM_ERASEBKGND)//20
	{
		return 1;
	}
	else if (uMsg == WM_SIZE)
	{
		_wnd_wm_size(pWnd, hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_WINDOWPOSCHANGING)//70
	{
		
		if (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
		{
			tagWINDOWPOS* pos = (tagWINDOWPOS*)lParam;
			if (!(pos->flags & SWP_NOSIZE))
			{
				void* padding_client = pWnd->padding_client_;
				if (padding_client != 0)
				{
					pos->cy += Ex_Scale(__get_int(padding_client, 4) + __get_int(padding_client, 12));
				}
			}

			if (!(pos->flags & SWP_NOMOVE))//被移动了
			{
				if (GetWindow(hWnd, GW_OWNER) != 0)//子菜单
				{
					pWnd->pMenuHostWnd_->dwFlags_ = pWnd->pMenuHostWnd_->dwFlags_ - (pWnd->pMenuHostWnd_->dwFlags_ & EWF_bMenuRepostion);
					_wnd_menu_setpos(hWnd, pWnd, pos);
				}
				pWnd->left_ = pos->x;
				pWnd->top_ = pos->y;
				_wnd_menu_init(hWnd, pWnd);
				return 0;
			}
		}
	}
	else if (uMsg == WM_WINDOWPOSCHANGED)//71
	{
		tagWINDOWPOS* pos = (tagWINDOWPOS*)lParam;
		if (!(pos->flags & SWP_NOMOVE))
		{
			__set_int(pWnd, offsetof(wnd_s, left_), pos->x);
			__set_int(pWnd, offsetof(wnd_s, top_), pos->y);
			_wnd_paint_shadow(pWnd, false, false);
		}
	}
	else if (uMsg == WM_NCCALCSIZE)//131
	{
		return 0;
	}
	else if (uMsg == WM_NCACTIVATE)//134
	{
		_wnd_wm_leavecheck(hWnd, pWnd, pWnd->objHittest_, -1, (obj_s*)-1, true);
		if (wParam == 0)
		{
			HEXOBJ focus = pWnd->objFocus_;
			pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_ACTIVE);
			pWnd->objFocusPrev_ = focus;
			Ex_ObjKillFocus(focus);
		}
		else {
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_ACTIVE;
			HEXOBJ focus = pWnd->objFocusPrev_;
			pWnd->objFocusPrev_ = 0;
			Ex_ObjSetFocus(focus);

			//TODO: 不知道为什么写在这里
			EX_CLASSINFO classInfo;
			if (Ex_ObjGetClassInfo(focus, &classInfo) && classInfo.atomName == ATOM_EDIT) {
				InvalidateRect(hWnd, 0, 0);
			}
		}
		_wnd_paint_shadow(pWnd, false, true);
		if (wParam == 0)
		{
			if (lParam == (size_t)(pWnd->hWndShadow_))
			{
				
				if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
				{
					return 0;
				}
			}
		}
		return 1;//防止出现边框/系统按钮
	}
	else if (uMsg == WM_NCPAINT)//133
	{
		return 0;
	} 
	else if (uMsg == WM_MOUSEACTIVATE) { //防止弹出窗获取焦点
		if (FLAGS_CHECK(pWnd->dwStyle_, EWS_POPUPWINDOW)) {
			return MA_NOACTIVATE;
		}
	}
	else if (uMsg == WM_NCLBUTTONDOWN) {
		_wnd_popupclose(pWnd, hWnd, 0, 0);
	}
	else if (uMsg == WM_PAINT)//15
	{
		if (_wnd_wm_paint(pWnd, hWnd))
		{
			return 0;
		}
	}
	else if (uMsg == WM_SHOWWINDOW)//24
	{
		if (wParam == 0)
		{
			HEXOBJ objHittest = pWnd->objHittest_;
			if (objHittest != 0)
			{
				Ex_ObjDispatchMessage(objHittest, WM_MOUSELEAVE, 0, 0);
				pWnd->objHittest_ = 0;
				UpdateWindow(hWnd);
			}
		}
		HWND hWndShadow = pWnd->hWndShadow_;
		if (!_wnd_querystyle(hWnd, WS_EX_TOPMOST, true))
		{

			SetWindowPos(hWndShadow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
		}

		ShowWindow(hWndShadow, wParam == 0 ? 0 : SW_SHOWNOACTIVATE);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	else if (uMsg == WM_GETMINMAXINFO)//36
	{
		if (_wnd_wm_getminmaxinfo(pWnd, hWnd, lParam))
		{
			return 0;
		}
	}
	else if (uMsg == WM_STYLECHANGING)//124
	{
		if (_wnd_wm_stylechanging(pWnd, hWnd, wParam, lParam))
		{
			return 0;
		}
	}
	else if (uMsg == WM_MOUSEWHEEL)//522  WM_MOUSEFIRST到WM_MOUSELAST包含了WM_MOUSEWHEEL，因此要在下一个判断之前拦截。
	{
		_wnd_wm_mousewheel(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg >= WM_MOUSEFIRST && uMsg < WM_MOUSELAST)//512 - ???
	{
		_wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)//256,264
	{
		_wnd_wm_keyboard(pWnd, hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CAPTURECHANGED)//533
	{
		_wnd_wm_captionchange(hWnd, pWnd);
	}
	else if (uMsg == WM_CONTEXTMENU)//被动接受消息123
	{
		_wnd_wm_menucontext(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_SYSCOMMAND || uMsg == WM_COMMAND)//274,273
	{
		_wnd_wm_command(hWnd, pWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_ENTERSIZEMOVE)//561
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bSizeMoving;
	}
	else if (uMsg == WM_EXITSIZEMOVE)//562
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bSizeMoving);
	}
	else if (uMsg == WM_MEASUREITEM)//44
	{
		if (_wnd_wm_measureitem_host(pWnd, wParam, lParam))
		{
			return 1;
		}
	}
	else if (uMsg == WM_INITDIALOG)//272
	{
		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{
			_msgbox_initdialog(hWnd, pWnd, wParam, lParam);
			auto ret = CallWindowProcW(pOld, hWnd, uMsg, wParam, lParam);
			SetFocus(hWnd);
			return ret;
		}
	}
	else if (uMsg == WM_GETDLGCODE)//135
	{
		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{
			return (DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTALLKEYS | DLGC_WANTMESSAGE | DLGC_HASSETSEL | DLGC_WANTCHARS);
		}
	}
	else if (uMsg == WM_INITMENUPOPUP)//279
	{
	
		if (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
		{
			return 0;
		}
		_wnd_wm_initmenupopup(hWnd, pWnd, (void*)wParam);
	}
	else if (uMsg == WM_EXITMENULOOP)//530
	{
		if (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
		{
			return 0;
		}
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bMenuInited);
		size_t objHittest = pWnd->objHittest_;
		_wnd_wm_nchittest(pWnd, hWnd, -1);
		_wnd_wm_leavecheck(hWnd, pWnd, objHittest, -1, (obj_s*)-1, true);
	}
	else if (uMsg == WM_IME_COMPOSITION)//271
	{
		_wnd_wm_ime_composition(hWnd, pWnd);
	}
	else if (uMsg == WM_SETTEXT)//12
	{
	
		if (((pWnd->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			Ex_ObjSendMessage(_obj_getobjfromidorname(pWnd, EWS_TITLE), uMsg, wParam, lParam);
		}
	}
	else if (uMsg == 485)// mn_selectitem
	{
		if (((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
		{
			GetCursorPos((LPPOINT)&wParam);
			_wnd_wm_mouse(pWnd, hWnd, WM_MOUSEMOVE, 1, MAKELONG(wParam - pWnd->left_, lParam - pWnd->top_));
			return 0;
		}
		else {
			_wnd_menu_mouse(hWnd, pWnd, WM_MOUSEMOVE, 0, &wParam);
			if (wParam == -1)
			{
				return 0;
			}
			else {
				wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
				if (pMenuHostWnd != 0)
				{
					pMenuHostWnd->pMenuPrevWnd_ = pWnd;
				}
			}
		}
	}
	else if (uMsg == 486)
	{
		wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
		if (pMenuHostWnd != 0)
		{
			wnd_s* pMenuTrackWnd = pMenuHostWnd->pMenuTrackWnd_;
			if (pMenuTrackWnd != 0)
			{
				
				if (((pMenuTrackWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
				{
					_wnd_obj_untrack(pMenuTrackWnd->hWnd_, pMenuTrackWnd, true);
					return CallWindowProcW(pOld, hWnd, 495, -1, 0);
				}
			}
		}
	}
	else if (uMsg == 493)//MN_BUTTONDOWN
	{
		if (!_wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONDOWN, 1, &wParam))
		{
			return CallWindowProcW(pOld, hWnd, uMsg, -1, lParam);
		}
		
		if (!((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
		{
			return CallWindowProcW(pOld, hWnd, uMsg, -1, lParam);
		}
	}
	else if (uMsg == 495)//MN_BUTTONUP
	{
		_wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONUP, 0, &wParam);
		if (pWnd->objTrackPrev_ != pWnd->objHittest_)
		{
			return CallWindowProcW(pOld, hWnd, uMsg, -1, lParam);
		}
	}
	else if (uMsg == 497)//MN_BUTTONUP
	{
		return 0;
	}
	return CallWindowProcW(pOld, hWnd, uMsg, wParam, lParam);
}

int _wnd_create(HEXDUI hExDui, wnd_s* pWnd, HWND hWnd, int dwStyle, HEXTHEME hTheme, LPARAM lParam, WinMsgPROC lpfnMsgProc)
{
	ShowWindow(hWnd, 0);
	RECT rcWindow{ 0 };
	GetWindowRect(hWnd, &rcWindow);
	SIZE size;
	size.cx = rcWindow.right - rcWindow.left;
	size.cy = rcWindow.bottom - rcWindow.top;

	if (!FLAGS_CHECK(dwStyle, EWS_MESSAGEBOX) && !FLAGS_CHECK(dwStyle, EWS_COMBOWINDOW))
	{
		int offsetX = 0;
		int offsetY = 0;
		if (Flag_Query(EXGF_DPI_ENABLE))
		{
			size.cx = Ex_Scale(size.cx);
			size.cy = Ex_Scale(size.cy);
			offsetX = (size.cx - (rcWindow.right - rcWindow.left)) / 2;
			offsetY = (size.cy - (rcWindow.bottom - rcWindow.top)) / 2;
		}
		MoveWindow(hWnd, rcWindow.left - offsetX, rcWindow.top - offsetY, size.cx, size.cy, false);
	}
	HWND hWndParent = 0;
	if ((dwStyle & EWS_MESSAGEBOX) != 0)
	{

		_wnd_getfromhandle(((mbp_s*)lParam)->handle_, &hWndParent);
	}
	else {

		hWndParent = (HWND)GetWindowLongPtrW(hWnd, GWLP_HWNDPARENT);
	}
	if ((dwStyle & EWS_CENTERWINDOW) != 0)
	{

		Ex_WndCenterFrom(hWnd, hWndParent, (dwStyle & EWS_FULLSCREEN) != 0);
	}

	GetWindowRect(hWnd, &rcWindow);
	size.cx = rcWindow.right - rcWindow.left;
	size.cy = rcWindow.bottom - rcWindow.top;
	int dwFlags = 0;

	if (_wnd_querystyle(hWnd, WS_EX_LAYERED, true) || (dwStyle & EWS_MESSAGEBOX) != 0)
	{

		dwFlags = EWF_bLayered;
	}
	if (hWndParent != 0)
	{
		pWnd->hExDuiParent_ = Ex_DUIFromWindow(hWndParent);
	}
	if (hTheme == 0) hTheme = g_Li.hThemeDefault;

	pWnd->dwStyle_ = dwStyle;
	pWnd->hImc_ = ImmGetContext(hWnd);
	pWnd->hWnd_ = hWnd;
	pWnd->hexdui_ = hExDui;
	pWnd->hTheme_ = hTheme;
	pWnd->width_ = size.cx;
	pWnd->height_ = size.cy;
	pWnd->alpha_ = 255;
	pWnd->dwFlags_ = dwFlags | EWF_bLeaveSent;
	pWnd->pfnMsgProc_ = lpfnMsgProc;
	pWnd->hTableObjects_ = HashTable_Create(GetNearestPrime(256), NULL);
	//4.1.20.430
	pWnd->hTableEvent_ = HashTable_Create(17, pfnDefaultFreeData);

	if ((dwStyle & EWS_MESSAGEBOX) != 0)
	{
		pWnd->lpMsgParams_ = (mbp_s*)lParam;
	}
	else {
		pWnd->lParam_ = lParam;
	}

	//layer
	pWnd->ulwi_.pptDst = (POINT*)((size_t)pWnd + offsetof(wnd_s, left_));
	pWnd->ulwi_.psize = (SIZE*)((size_t)pWnd + offsetof(wnd_s, width_));
	pWnd->ulwi_.pptSrc = (POINT*)((size_t)pWnd + offsetof(wnd_s, ulwi_pptSrc_x_));
	pWnd->ulwi_.pblend = (BLENDFUNCTION*)((size_t)pWnd + offsetof(wnd_s, ulwi_pblend_));
	pWnd->ulwi_.prcDirty = (RECT*)((size_t)pWnd + offsetof(wnd_s, ulwi_prcDirty_left_));

	pWnd->ulwi_.cbSize = sizeof(pWnd->ulwi_);
	pWnd->ulwi_.dwFlags = 2;
	pWnd->ulwi_pblend_.AlphaFormat = 1;
	pWnd->ulwi_pblend_.BlendFlags = 0;
	pWnd->ulwi_pblend_.BlendOp = 0;
	pWnd->ulwi_pblend_.SourceConstantAlpha = 0xFF;
	pWnd->left_ = rcWindow.left;
	pWnd->top_ = rcWindow.top;
	pWnd->width_ = size.cx;
	pWnd->height_ = size.cy;
	pWnd->ulwi_prcDirty_right_ = size.cx;
	pWnd->ulwi_prcDirty_bottom_ = size.cy;

	//sysshadow
	auto tmp1 = GetClassLongPtrW(hWnd, GCL_STYLE);
	if ((tmp1 & CS_DROPSHADOW) != 0)
	{
		SetClassLongPtrW(hWnd, GCL_STYLE, (tmp1 & ~CS_DROPSHADOW));
	}
	int tmp2 = WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST;
	auto tmp3 = CreateWindowExW(tmp2, (LPCWSTR)g_Li.atomSysShadow, 0, WS_POPUP, 0, 0, 0, 0, hWnd, 0, 0, 0);

	pWnd->hWndShadow_ = tmp3;
	int nError = 0;
	Thunkwindow(tmp3, _wnd_shadow_proc, (void*)1, &nError);
	//tips
	HWND hWndTips = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST, L"tooltips_class32", 0, WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
	SendMessageW(hWndTips, 1048, 0, 2048);//TTM_SETMAXTIPWIDTH 支持多行
	pWnd->hWndTips_ = hWndTips;
	nError = 0;
	ti_s* tmp4 =(ti_s*) _struct_createfromaddr(pWnd, offsetof(wnd_s, ti_auto_), sizeof(ti_s), &nError);

	tmp4->cbSize_ = sizeof(ti_s);
	tmp4->uFlags_ = 16 | 0 | 4096;//TTF_SUBCLASS ,TTF_TRANSPARENT ,TTF_PARSELINKS
	tmp4->hWnd_ = hWnd;
	tmp4->uId_ = tmp4;
	SendMessageW(hWndTips, 1074, 0, (LPARAM)tmp4);//TTM_ADDTOOLW
	nError = 0;
	ti_s* tmp5 = (ti_s*)_struct_createfromaddr(pWnd, offsetof(wnd_s, ti_track_), sizeof(ti_s), &nError);

	tmp5->cbSize_ = sizeof(ti_s);
	tmp5->uFlags_ = 32 | 128 | 0 | 4096;// TTF_TRACK ,TTF_ABSOLUTE ,TTF_TRANSPARENT,TTF_PARSELINKS
	tmp5->hWnd_ = hWnd;
	tmp5->uId_ = tmp5;
	SendMessageW(hWndTips, 1074, 0, (LPARAM)tmp5);//TTM_ADDTOOLW
	nError = 0;
	Thunkwindow(hWndTips, _wnd_tooltips_proc, pWnd, &nError);

	//初始化渲染,以及背景
	_wnd_dx_init(pWnd);
	int w1, h1;
	w1 = rcWindow.right - rcWindow.left;
	h1 = rcWindow.bottom - rcWindow.top;
	nError = 0;
	pWnd->canvas_display_ = _canvas_createfrompwnd(pWnd, w1, h1, CVF_GDI_COMPATIBLE, &nError);

	nError = 0;
	pWnd->canvas_bkg_ = _canvas_createfrompwnd(pWnd, w1, h1, 0, &nError);


	if (nError == 0)
	{
		//加载主题，并确定客户区

		_wnd_loadtheme(pWnd, hWnd, hTheme);

		if ((dwStyle & EWS_MENU) == 0)
		{
			_wnd_sysbutton_create(hWnd, pWnd, dwStyle);

		}

		_wnd_addstyle(hWnd, WS_THICKFRAME, false);//强制触发样式被修改事件
		Thunkwindow(hWnd, _wnd_proc, pWnd, &nError);
		_wnd_delstyle(hWnd, WS_THICKFRAME, false);
		IME_Control(hWnd, pWnd, false);

		if (FLAGS_CHECK(dwStyle, EWS_POPUPWINDOW)) {
			pWnd->lpPopupParams_ = lParam;
			if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (void**)&pWnd, &nError))
			{
				HWND old = pWnd->hWndPopup_;
				pWnd->hWndPopup_ = hWnd;
				DestroyWindow(old);
				FLAGS_ADD(pWnd->dwFlags_, EWF_bPopupWindowShown);
				obj_s* pObj;
				if (_handle_validate(lParam, HT_OBJECT, (void**)&pObj, &nError))
				{
					_obj_baseproc(hWndParent, lParam, pObj, -6, hExDui, 0);
					IME_Control(hWndParent, pWnd, 1);
				}
			}
			nError = 0;
		}
	}

	return nError;
}

LRESULT CALLBACK _wnd_tooltips_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = pData->hWnd;
	WNDPROC pOld = pData->Proc;
	wnd_s* pWnd = (wnd_s*)pData->dwData;
	if (uMsg == WM_DESTROY)
	{
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (size_t)pOld);
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		lParam = CallWindowProcW(pOld, hWnd, uMsg, wParam, lParam);
		SendMessageW(hWnd, 1041, 0, (size_t)pWnd + offsetof(wnd_s, ti_track_));
		return lParam;
	}
	return CallWindowProcW(pOld, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK _wnd_shadow_proc(EX_THUNK_DATA* pData, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	HWND hWnd = pData->hWnd;
	size_t pOld = (size_t)pData->Proc;

	if (uMsg == WM_NCACTIVATE)
	{
		size_t hWndParent = GetWindowLongPtrW(hWnd, GWLP_HWNDPARENT);
		if (!(wParam == 0 && lParam == hWndParent))
		{
			if ((WPARAM)pData->dwData == wParam && lParam == 0)
			{
				SendMessageW((HWND)hWndParent, uMsg, 1, lParam);
				SetFocus((HWND)hWndParent);
				pData->dwData = (void*)1;
				return 0;
			}
			SendMessageW((HWND)hWndParent, uMsg, wParam, lParam);
			pData->dwData = (void*)wParam;
		}
	}
	else if (uMsg == WM_DESTROY)
	{

		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, pOld);
		VirtualFree(pData, 0, MEM_RELEASE);
	}
	return CallWindowProcW((WNDPROC)pOld, hWnd, uMsg, wParam, lParam);
}

void _wnd_dx_unint(wnd_s* pWnd)
{
	void* pDeviceContext = pWnd->dx_context_;

	if (pDeviceContext != 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->Release();
	}
	void* pgdiinterop = pWnd->dx_gdiinterop_;
	if (pgdiinterop != 0)
	{
		((ID2D1GdiInteropRenderTarget*)pgdiinterop)->Release();
	}
}

void _wnd_dx_init(wnd_s* pWnd)
{
	_wnd_dx_unint(pWnd);
	void* pDeviceContext = nullptr;
	if (((ID2D1Device*)g_Ri.pD2DDevice)->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext**)&pDeviceContext) == 0)
	{
		((ID2D1DeviceContext*)pDeviceContext)->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
		pWnd->dx_context_ = pDeviceContext;
		pWnd->dx_gdiinterop_ = _dx_get_gdiInterop(pDeviceContext);
	}
}

HEXOBJ _wnd_wm_nchittest_obj(HWND hWnd, wnd_s* pWnd, HEXOBJ objLast, int x, int y, int* hitCode, obj_s** pObjMouse)
{
	HEXOBJ objPrev = objLast;
	obj_s* pObj = nullptr;
	int nError = 0;
	while (_handle_validate(objPrev, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
		{
			RECT rt = { pObj->left_,pObj->top_,pObj->right_,pObj->bottom_ };
			POINT po = { x,y };
			if (PtInRect(&rt, po))
			{
				int ox = x - pObj->left_;
				int oy = y - pObj->top_;
				auto lParam = MAKELONG(ox, oy);
				
				if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED))//检测是否被禁止，被禁止组件还需检测是否可穿透
				{
					*hitCode = _obj_baseproc(hWnd, objPrev, pObj, WM_NCHITTEST, 0, lParam);
					if (*hitCode == HTTRANSPARENT)//在穿透区域,则继续寻找
					{
						objPrev = pObj->objPrev_;
						continue;
					}
					else {
						*hitCode = HTCLIENT;//不可穿透则直接停止寻找
						*pObjMouse = pObj;
						return objPrev;
					}
				}
				else {
					objLast = pObj->objChildLast_;
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
					objPrev = pObj->objPrev_;
					continue;
				}
				else {
					pWnd->dwHitObjPos_Abs_ = lParam;
					*pObjMouse = pObj;
					return objPrev;
				}
			}
		}
		objPrev = pObj->objPrev_;
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

int _wnd_wm_nchittest(wnd_s* pWnd, HWND hWnd, LPARAM lParam)
{
	int dwHitCode = HTCLIENT;
	
	if (((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject)) return dwHitCode;
	
	if (!((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
	{
		
		if (((pWnd->dwFlags_ & EWF_bMenuInited) == EWF_bMenuInited))
		{
			return dwHitCode;
		}
	}

	POINT pt;
	HEXOBJ objMouse = 0;
	if (lParam == -1)
	{
		GetCursorPos(&pt);
	}
	else {
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
	}
	ScreenToClient(hWnd, &pt);
	if (_rgn_hittest(pWnd->hrgn_client_, pt.x, pt.y) || pWnd->dwWinState_ == 2)
	{
		obj_s* pobjMouse = nullptr;
		objMouse = _wnd_wm_nchittest_obj(hWnd, pWnd, pWnd->objChildLast_, pt.x, pt.y, &dwHitCode, &pobjMouse);
		if (objMouse == 0)
		{
			pobjMouse = 0;
			
			if (((pWnd->dwStyle_ & EWS_MOVEABLE) == EWS_MOVEABLE))//允许随意移动
			{
				dwHitCode = HTCAPTION;
			}
		}
		else {
			pWnd->dwHitCode_ = dwHitCode;
			if (objMouse == pWnd->objCaption_)
			{
				dwHitCode = HTCAPTION;
			}
			else {
				
				if (((pobjMouse->dwStyle_ & EOS_DISABLED) == EOS_DISABLED))
				{
					
					if (((pWnd->dwStyle_ & EWS_MOVEABLE) == EWS_MOVEABLE))//允许随意移动
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
	
		if (((pWnd->dwStyle_ & EWS_SIZEABLE) == EWS_SIZEABLE))//允许调整尺寸
		{
			dwHitCode = _wnd_wm_nchittest_sizebox(pWnd->width_, pWnd->height_, pt.x, pt.y);
		}
		else {
			dwHitCode = HTCAPTION;
		}
	}

	auto objHittest = pWnd->objHittest_;
	pWnd->objHittest_ = objMouse;
	if (objHittest != objMouse)
	{
		pWnd->objHittestPrev_ = objHittest;
		dwHitCode = HTCLIENT;
	}
	return dwHitCode;
}

void _wnd_sysbutton_create(HWND hWnd, wnd_s* pWnd, int dwStyle)
{
	HEXTHEME hTheme = pWnd->hTheme_;
	RECT rcCaption = { 0 };
	_wnd_calc_captionrect(pWnd, &rcCaption);
	obj_s* pObjCaption = nullptr;
	int nError = 0;
	HEXOBJ objCaption = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjCaption, &nError);

	int nMinHeight = 0;
	if (objCaption != 0)
	{
		std::wstring title;
		LPCWSTR lpTitle = nullptr;
		
		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{

			lpTitle = pWnd->lpMsgParams_->lpCaption_;

		}
		else {

			title = _wnd_gettitle(hWnd);
			lpTitle = title.data();
		}

		_obj_create_proc(&nError, false, hTheme, pObjCaption, (dwStyle & EWS_NOCAPTIONTOPMOST) != 0 ? 0 : -1, ATOM_SYSBUTTON, lpTitle,
			EOS_VISIBLE | EWS_TITLE, rcCaption.left, rcCaption.top, rcCaption.right - rcCaption.left, rcCaption.bottom - rcCaption.top, 0, EWS_TITLE, 0, 0, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		void* lpPT = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_PADDING_TEXT);
		if (lpPT != 0)
		{
			RtlMoveMemory((void*)((size_t)pObjCaption + offsetof(obj_s, t_left_)), lpPT, 16);

			if (g_Li.DpiX > 1)
			{
				pObjCaption->t_left_ = pObjCaption->t_left_ * g_Li.DpiX;
				pObjCaption->t_top_ = pObjCaption->t_top_ * g_Li.DpiX;
				pObjCaption->t_right_ = pObjCaption->t_right_ * g_Li.DpiX;
				pObjCaption->t_bottom_ = pObjCaption->t_bottom_ * g_Li.DpiX;
			}
		}
		void* lpCr = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_COLOR_NORMAL);
		if (lpCr != 0)
		{
			pObjCaption->crNormal_ = __get_int(lpCr, 0);
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
			HEXFONT hfont = pObjCaption->hFont_;

			pObjCaption->hFont_ = _font_createfromfamily((LPWSTR)pFamily, Size, Style);

			if (hfont != 0) _font_destroy(hfont);
		}

		_obj_create_done(hWnd, pWnd, objCaption, pObjCaption);
		pWnd->objCaption_ = objCaption;
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

				RECT rcObject{ 0 };
				RtlMoveMemory(&rcObject, lpValuea, 16);
				rcObject.right = Ex_Scale(rcObject.right - rcObject.left);
				rcObject.bottom = Ex_Scale(rcObject.bottom - rcObject.top);

				left = left - rcObject.right;
				obj_s* pObjTmp = nullptr;
				int nError = 0;
				HEXOBJ objTmp = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjTmp, &nError);
				if (objTmp != 0)
				{

					_obj_create_proc(&nError, false, hTheme, pObjTmp, -1, ATOM_SYSBUTTON, 0, EOS_VISIBLE | aryBtn[i], left, 0, rcObject.right, rcObject.bottom, objCaption, aryBtn[i], 0, 0, 0);
					_obj_create_done(hWnd, pWnd, objTmp, pObjTmp);
					nMinWidth = nMinWidth + rcObject.right;
				}
			}
		}
	}
	pObjCaption->t_right_ = pObjCaption->t_right_ + nMinWidth;
	nMinWidth = nMinWidth + rcCaption.left + pWnd->width_ - rcCaption.right;
	
	if (((pWnd->dwStyle_ & EWS_HASICON) == EWS_HASICON))
	{
		nMinWidth = nMinWidth + Ex_Scale(16);
	}
	if (pWnd->minWidth_ < nMinWidth)
	{
		pWnd->minWidth_ = nMinWidth;
	}
	if (pWnd->minHeight_ < nMinHeight)
	{
		pWnd->minHeight_ = nMinHeight;
	}

}

void _wnd_render_obj(HWND hWnd, wnd_s* pWnd, void* pContext, HEXCANVAS cvDisplay, void* pBitmapDisplay, RECT rcPaint,
	EXHANDLE objChildFirst, int offsetX, int offsetY, int pAlpha, bool fDX, void* hBorderBrush)
{
	EXHANDLE objNext = objChildFirst;
	int nError = 0;
	if (objNext != 0)
	{
		do
		{
			obj_s* pObj = nullptr;
			if (_handle_validate(objNext, HT_OBJECT, (void**)&pObj, &nError))
			{
			
				if (((pObj->dwStyle_ & EOS_VISIBLE) == EOS_VISIBLE))
				{
					RECT rcObj{ 0 };
					rcObj.left = pObj->left_;
					rcObj.top = pObj->top_;
					rcObj.right = pObj->right_;
					rcObj.bottom = pObj->bottom_;

					OffsetRect(&rcObj, offsetX, offsetY);
					RECT rcClip{ 0 };
					if (IntersectRect(&rcClip, &rcPaint, &rcObj))
					{
						
						auto fPage = !((pObj->dwFlags_ & eof_bPage) == eof_bPage);
						if (fPage)
						{
							
							if (((pObj->dwFlags_ & eof_bCanRedraw) == eof_bCanRedraw))
							{
								
								if (((pObj->dwFlags_ & eof_bNeedRedraw) == eof_bNeedRedraw))
								{
									pObj->dwFlags_ = pObj->dwFlags_ - (pObj->dwFlags_ & eof_bNeedRedraw);
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
						//组件透明度是否需要被窗口透明度影响？？
						int alpha = pObj->dwAlpha_ * pWnd->alpha_ / 255;
						alpha = alpha * pAlpha / 255;
						
						if (((pObj->dwStyle_ & EOS_DISABLED) == EOS_DISABLED))
						{
							alpha = alpha * pObj->dwAlphaDisable_ / 255;
						}
						if (fPage)//非页面
						{
							
							if (((pObj->dwFlags_ & eof_bPath) == eof_bPath))
							{
								void* hPathBrush = _brush_createfromcanvas2(pObj->canvas_obj_, alpha);
								if (hPathBrush != 0)
								{
									HEXMATRIX matrix = _matrix_create();
									_matrix_translate(matrix, pObj->w_left_, pObj->w_top_);
									_brush_settransform(hPathBrush, matrix);
									_canvas_fillpath(cvDisplay, pObj->hPath_Window_, hPathBrush);
									_brush_destroy(hPathBrush);
									_matrix_destroy(matrix);
								}
							}
							else {
								
								if (((pObj->dwStyleEx_ & EOS_EX_COMPOSITED) == EOS_EX_COMPOSITED))
								{
									_canvas_bitblt(cvDisplay, pObj->canvas_obj_, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom, rcClip.left - rcObj.left, rcClip.top - rcObj.top);
								}
								else {
									_canvas_alphablend(cvDisplay, pObj->canvas_obj_, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom, rcClip.left - rcObj.left, rcClip.top - rcObj.top, rcClip.right - rcObj.left, rcClip.bottom - rcObj.top, alpha);
								}
							}
						}
						if (hBorderBrush != 0)
						{
							_canvas_drawrect(cvDisplay, hBorderBrush, pObj->w_left_, pObj->w_top_, pObj->w_right_, pObj->w_bottom_, 1, D2D1_DASH_STYLE_SOLID);
						}

						HEXOBJ objChild = pObj->objChildFirst_;
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
			objNext = pObj->objNext_;
		} while (objNext != 0);
	}
}

bool _wnd_wm_setcursor(HWND hWnd, wnd_s* pWnd, LPARAM lParam)
{
	auto uHitCode = LOWORD(lParam);
	auto hCursor = pWnd->hCursor_;
	if (uHitCode == HTCLIENT)
	{
		auto objHittest = pWnd->objHittest_;
		obj_s* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objHittest, HT_OBJECT, (void**)&pObj, &nError))
		{
			if (_obj_baseproc(hWnd, objHittest, pObj, WM_SETCURSOR, pWnd->dwHitObjPos_Abs_, MAKELONG(pWnd->dwHitCode_, HIWORD(lParam))) != 0)
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

void CALLBACK _wnd_timer_mousetrack(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hWnd, idEvent);
	wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_MOUSETRACK);
	
	if (!((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
	{
		POINT pt;
		GetCursorPos(&pt);
		if (WindowFromPoint(pt) != hWnd)
		{
			
			if (!((pWnd->dwFlags_ & EWF_bLeaveSent) == EWF_bLeaveSent))
			{
				pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bLeaveSent;
				HEXOBJ hObj = pWnd->objHittestPrev_;
				obj_s* pObj = nullptr;
				int nError = 0;
				if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
				{
					_obj_baseproc(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0);//发送离开
					pWnd->objHittestPrev_ = pWnd->objHittest_;
					pWnd->objHittest_ = 0;
				}
			}
		}
	}
}

void _wnd_wm_leavecheck(HWND hWnd, wnd_s* pWnd, HEXOBJ objCheck, HEXOBJ objHittest, obj_s* pObjHittest, bool fTrack)
{
	int nError = 0;
	if (objHittest == -1)
	{
		objHittest = pWnd->objHittest_;
		_handle_validate(objHittest, HT_OBJECT, (void**)&pObjHittest, &nError);
	}
	if (objCheck != objHittest)
	{
		nError = 0;
		obj_s* pObjCheck = nullptr;
		if (_handle_validate(objCheck, HT_OBJECT, (void**)&pObjCheck, &nError))
		{
			
			if (!((pWnd->dwFlags_ & EWF_bLeaveSent) == EWF_bLeaveSent))
			{
				pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bLeaveSent;
				_obj_baseproc(hWnd, objCheck, pObjCheck, WM_MOUSELEAVE, 0, 0);
			}
		}
		if (objHittest != 0)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bLeaveSent);
			pWnd->objHittestPrev_ = objHittest;
			_obj_baseproc(hWnd, objHittest, pObjHittest, WM_MOUSEHOVER, 0, 0);
		}
	}

	if (objHittest != 0 && fTrack)
	{
		size_t nEvent = (size_t)pWnd + TIMER_MOUSETRACK;
		KillTimer(hWnd, nEvent);
		SetTimer(hWnd, nEvent, 50, _wnd_timer_mousetrack);
	}
}

int _wnd_destroy(HWND hWnd, wnd_s* pWnd)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bDestroyWindow;

	if (FLAGS_CHECK(pWnd->dwStyle_, EWS_POPUPWINDOW))
	{
		wnd_s* pWndParent;
		if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (void**)&pWndParent, &nError))
		{
			FLAGS_DEL(pWndParent->dwFlags_, EWF_bPopupWindowShown);
			pWndParent->hWndPopup_ = 0;

			if (_handle_validate(pWnd->lpPopupParams_, HT_OBJECT, (void**)&pObj, &nError))
			{
				_obj_baseproc(pWndParent->hWnd_, pWnd->lpPopupParams_, pObj, -7, 0, 0);
				IME_Control(pWndParent->hWnd_, pWndParent, 0);
			}
		}
		nError = 0;
		pObj = NULL;
	}

	DestroyWindow(pWnd->hWndTips_);
	DestroyWindow(pWnd->hWndShadow_);

	if (_handle_validate(pWnd->objChildFirst_, HT_OBJECT, (void**)&pObj, &nError))
	{
		_obj_notify_brothers(hWnd, pWnd->objChildFirst_, pObj, WM_DESTROY, 0, 0, false, false);
	}

	//clear MESSAGEBOX
	if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
	{
		if (pWnd->lpMsgParams_ != 0)
		{
			if (pWnd->lpMsgParams_->lpCheckBox_ != 0)
			{
				size_t dwTmp = 0;
				if (HashTable_Get(pWnd->hTableObjects_, (size_t)(pWnd->lpMsgParams_->lpCheckBoxChecked_), &dwTmp))
				{
					if (dwTmp != 0)
					{
						__set_int(pWnd->lpMsgParams_->lpCheckBoxChecked_, 0, Ex_ObjDispatchMessage(dwTmp, BM_GETCHECK, 0, 0));
					}
				}
			}
		}
	}

	//clear NotifyIcon
	if (pWnd->lpNid_ != 0)
	{
		Shell_NotifyIconW(NIM_DELETE, (PNOTIFYICONDATAW)pWnd->lpNid_);
		Ex_MemFree(pWnd->lpNid_);
		pWnd->lpNid_ = 0;
	}

	//clear DropTarget
	if (pWnd->lpIDropTarget_)
	{
		((DropTarget*)pWnd->lpIDropTarget_)->Release();
		delete (DropTarget*)pWnd->lpIDropTarget_;
		pWnd->lpIDropTarget_ = nullptr;
	}

	HashTable_Destroy(pWnd->hTableObjects_);
	HashTable_Destroy(pWnd->hTableEvent_);

	//clear bkgimginfo
	KillTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT);
	_obj_backgroundimage_clear(hWnd, (obj_base*)pWnd);

	//clear font
	_font_destroy(pWnd->hFont_Menu_);
	
	bool bMainWnd = ((pWnd->dwStyle_ & EWS_MAINWINDOW) == EWS_MAINWINDOW);
	_struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_auto_));
	_struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_track_));
	_rgn_destroy(pWnd->hrgn_client_);
	_rgn_destroy(pWnd->hrgn_sizebox_);
	_canvas_destroy(pWnd->canvas_bkg_);
	_canvas_destroy(pWnd->canvas_display_);

	if (pWnd->hLayout_ != 0)
	{
		_layout_destroy(pWnd->hLayout_);
	}

	_wnd_dx_unint(pWnd);
	HEXDUI hExDui = pWnd->hexdui_;
	nError = 0;
	_handle_destroy(hExDui, &nError);
	Ex_MemFree(pWnd);
	Ex_SetLastError(nError);
	return bMainWnd ? 1 : 0;
}

void _wnd_paint_bkg(HWND hWnd, wnd_s* pWnd)
{
	HEXCANVAS cvBkg = pWnd->canvas_bkg_;
	wnd_s* p = pWnd;
	if (_canvas_begindraw(cvBkg))
	{
		size_t result = 0;
		if (!pWnd->pfnMsgProc_ || !pWnd->pfnMsgProc_(hWnd, pWnd->hexdui_, WM_ERASEBKGND, cvBkg, pWnd->hexdui_, &result)) 
		{
			if (!((pWnd->dwStyle_ & EWS_NOINHERITBKG) == EWS_NOINHERITBKG))
			{
				wnd_s* pParent = nullptr;
				int nError = 0;
				if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (void**)&pParent, &nError))
				{
					p = pParent;
				}

				if (!((pWnd->dwFlags_ & EWF_bInheritBkgStarted) == EWF_bInheritBkgStarted))
				{
					pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bInheritBkgStarted;
					SetTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT, 100, &_wnd_backgroundimage_timer_inherit);
				}
			}
			//绘制底色

			_canvas_clear(cvBkg, p->crBkg_);

			//绘制背景
			if (p->lpBackgroundImage_)
			{
				_canvas_drawimagefrombkgimg(cvBkg, p->lpBackgroundImage_);
			}
		}		

		if (pWnd->fBlur_)
		{
			_canvas_blur(cvBkg, pWnd->fBlur_, 0);
		}

		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{
			_msgbox_drawinfo(pWnd, cvBkg);
		}
		if (FLAGS_CHECK(pWnd->dwStyle_, EWS_COMBOWINDOW))
		{
			Ex_ThemeDrawControl(pWnd->hTheme_, cvBkg, 0, 0, pWnd->width_ - 1, pWnd->height_ - 1, -1741080004, ATOM_NORMAL, 255);
		}
	}
	_canvas_enddraw(cvBkg);
	_obj_compostied_all(pWnd->objChildFirst_);
}

void _wnd_render_dc(HWND hWnd, wnd_s* pWnd, void* hDC, HEXCANVAS cvDisplay, RECT rcPaint, bool fLayer)
{
	void* mDC = _canvas_getdc(cvDisplay);
	if (mDC != 0)
	{
		if (fLayer)
		{
			pWnd->ulwi_prcDirty_left_ = rcPaint.left;
			pWnd->ulwi_prcDirty_top_ = rcPaint.top;
			pWnd->ulwi_prcDirty_right_ = rcPaint.right;
			pWnd->ulwi_prcDirty_bottom_ = rcPaint.bottom;

			pWnd->ulwi_.hdcSrc = (HDC)mDC;
			if (g_Li.pfnUpdateLayeredWindowIndirect)
			{
				((UpdateLayeredWindowIndirectPROC)g_Li.pfnUpdateLayeredWindowIndirect)(hWnd, &pWnd->ulwi_);
			}
			else {
				BLENDFUNCTION pb_;
				pb_.BlendOp = 0;
				pb_.BlendFlags = 0;
				pb_.SourceConstantAlpha = 0xFF;
				pb_.AlphaFormat = 1;
				UpdateLayeredWindow(hWnd, 0, 0, (SIZE*)pWnd->ulwi_.psize, pWnd->ulwi_.hdcSrc, (POINT*)pWnd->ulwi_.pptSrc, 0, &pb_, ULW_ALPHA);
			}
		}
		else {
			BitBlt((HDC)hDC, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, (HDC)mDC, rcPaint.left, rcPaint.top, SRCCOPY);
		}
		_canvas_releasedc(cvDisplay);
	}
}

void _wnd_render(HWND hWnd, wnd_s* pWnd, void* hDC, RECT rcPaint, bool fLayer, bool fDX)
{
	pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRendering;
	
	if (((pWnd->dwFlags_ & EWF_bRedrawBackground) == EWF_bRedrawBackground))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bRedrawBackground);
		_wnd_paint_bkg(hWnd, pWnd);
	}
	HEXCANVAS cvDisplay = pWnd->canvas_display_;
	if (_canvas_begindraw(cvDisplay))
	{
		void* pContext = nullptr;
		void* pBitmapDisplay = nullptr;
		if (fDX)
		{
			pContext = pWnd->dx_context_;
			pBitmapDisplay = _canvas_getcontext(cvDisplay, CVC_DX_D2DBITMAP);
			_dx_bmp_copyfrom(&pBitmapDisplay, _canvas_getcontext(pWnd->canvas_bkg_, CVC_DX_D2DBITMAP), rcPaint.left, rcPaint.top, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);

			_dx_settarget(pContext, pBitmapDisplay);
		}
		else {

			_canvas_bitblt(cvDisplay, pWnd->canvas_bkg_, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, rcPaint.left, rcPaint.top);
		}

		void* hBrush = nullptr;
		if (Flag_Query(EXGF_OBJECT_SHOWRECTBORDER))
		{
			hBrush = _brush_create(-65536);
		}
		_wnd_render_obj(hWnd, pWnd, pContext, cvDisplay, pBitmapDisplay, rcPaint, pWnd->objChildFirst_, 0, 0, 255, fDX, hBrush);
		_brush_destroy(hBrush);
		_wnd_render_dc(hWnd, pWnd, hDC, cvDisplay, rcPaint, fLayer);
		_canvas_enddraw(cvDisplay);
	}
	pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bRendering);
}


int _wnd_popupclose(wnd_s* pWnd, HWND hWnd, int wParam, obj_s* pObj)
{
	int result = 0;
	int nError = 0;

	if (FLAGS_CHECK(pWnd->dwFlags_, EWF_bPopupWindowShown))
	{
		if (!pObj
			|| (!_handle_validate(pWnd->lpPopupParams_, HT_OBJECT, (void**)&pObj, &nError))
			|| (result = _obj_baseproc(hWnd, pObj->hObj_, pObj, -7, wParam, 0)) == 0)
		{
			FLAGS_DEL(pWnd->dwFlags_, EWF_bPopupWindowShown);
			result = DestroyWindow(pWnd->hWndPopup_);
		}
	}
	return result;
}

void _wnd_wm_size(wnd_s* pWnd, HWND hWnd, WPARAM wParam, int width, int height)
{
	pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bRendered);
	pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_SIZED | EWF_bRedrawBackground;
	if (width != pWnd->width_ || height != pWnd->height_)
	{
		
		if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
		{
			_wnd_addstyle(hWnd, WS_EX_LAYERED, true);
		}
		
		if (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
		{
			width = width - GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
			height = height - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
		}
		_wnd_recalcclient(pWnd, hWnd, width, height);
		_layout_update(pWnd->hLayout_);
		RECT rcCaption{ 0 };
		_wnd_calc_captionrect(pWnd, &rcCaption);
		Ex_ObjSetPos(pWnd->objCaption_, 0, 0, 0, rcCaption.right - rcCaption.left, EOP_DEFAULT, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_EX_NODPISCALE);
		if (pWnd->dwWinState_ != wParam)
		{
			
			if (((pWnd->dwStyle_ & EWS_BUTTON_MAX) == EWS_BUTTON_MAX))
			{
				Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, EWS_BUTTON_MAX), 0);
			}
		
			if (((pWnd->dwStyle_ & EWS_BUTTON_MIN) == EWS_BUTTON_MIN))
			{
				Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, EWS_BUTTON_MIN), 0);
			}
			pWnd->dwWinState_ = wParam;
		}
	}
	InvalidateRect(hWnd, 0, FALSE);
}

void _wnd_menu_setpos(HWND hWnd, wnd_s* pWnd, tagWINDOWPOS* pos)
{
	wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
	wnd_s* pMenuPrevWnd = nullptr;
	RECT rcParent{ 0 };
	if (pMenuHostWnd != 0)
	{
		pMenuPrevWnd = pMenuHostWnd->pMenuPrevWnd_;
		if (pMenuPrevWnd != 0)
		{
			GetWindowRect(pMenuPrevWnd->hWnd_, &rcParent);
		}
	}
	auto offset = LOBYTE(HIWORD(pWnd->szItemSeparator_));
	int x = pos->x;
	int y = pos->y;
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
	pos->x = x;
	if (pMenuPrevWnd != 0)
	{
		HEXOBJ hObj = pMenuPrevWnd->objFocus_;
		obj_s* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
		{
			void* padding_client = pWnd->padding_client_;
			y = pObj->w_top_ + pMenuPrevWnd->top_ - __get_int(padding_client, 4);
			GetWindowRect(hWnd, &rcParent);
			int height = rcParent.bottom - rcParent.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
			_wnd_getscreenrect(hWnd, &rcParent);
			if (y + height > rcParent.bottom)
			{
				y = pObj->w_bottom_ - height + __get_int(padding_client, 12);
			}
		}
	}
	pos->y = y;
}

void _wnd_menu_createitems(HWND hWnd, wnd_s* pWnd)
{
	size_t hMenu = SendMessageW(hWnd, 481, 0, 0);
	HEXTHEME hTheme = pWnd->hTheme_;
	void* lpPaddingText = Ex_ThemeGetValuePtr(hTheme, ATOM_MENU, ATOM_PADDING_TEXT);
	int nCount = GetMenuItemCount((HMENU)hMenu);
	wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
	HWND hParent = pMenuHostWnd->hWnd_;
	RECT rcParent{ 0 };
	GetWindowRect(hParent, &rcParent);
	void* padding_client = pWnd->padding_client_;
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

	obj_s* pParnet = nullptr;
	int nError = 0;
	HEXOBJ objParent = _obj_create_init(hWnd, pWnd, ATOM_PAGE, 0, &pParnet, &nError);
	if (objParent != 0)
	{
		int width = pWnd->width_ - (rcPaddingClient.left + rcPaddingClient.right);
		int height = pWnd->height_ - (rcPaddingClient.top + rcPaddingClient.bottom);
		_obj_create_proc(NULL, false, hTheme, pParnet, EOS_EX_FOCUSABLE, ATOM_PAGE, 0, EOS_VISIBLE | EOS_VSCROLL | EMIS_SUBMENU, rcPaddingClient.left, rcPaddingClient.top, width, height, 0, 0, 0, 0, 0);
		_obj_create_done(hWnd, pWnd, objParent, pParnet);
		HEXOBJ objPP = objParent;
		HEXLAYOUT hLayout = _layout_create(ELT_ABSOLUTE, pWnd->hexdui_);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.left);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.top);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.right);
		_layout_absolute_setedge(hLayout, objPP, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, rcPaddingClient.bottom);
		nError = 0;
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
			RECT rcItem{ 0 };
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
							eos = eos | EMIS_SEPARATOR;
						}
						else {
							if (mii.hSubMenu != 0)
							{
								eos = eos | EMIS_SUBMENU;
							}
						}
					}
					WCHAR buff[520];
					if (rcItem.left != 0) {
						OffsetRect(&rcItem, -rcParent.left, -rcParent.top);
					}
					GetMenuStringW((HMENU)hMenu, i, buff, 520, MF_BYPOSITION);
					obj_s* pObj = nullptr;
					nError = 0;
					HEXOBJ hObj = _obj_create_init(hWnd, pWnd, ATOM_ITEM, 0, &pObj, &nError);
					if (hObj != 0)
					{
						_obj_create_proc(&nError, false, hTheme, pObj, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, ATOM_ITEM, buff, eos, rcItem.left, rcItem.top, width, rcItem.bottom - rcItem.top, objParent, mii.wID, 0, i, DT_VCENTER | DT_SINGLELINE);
						pObj->dwFlags_ = pObj->dwFlags_ | eof_bMenuItem;
						_obj_create_done(hWnd, pWnd, hObj, pObj);
						if (lpPaddingText != 0)
						{
							RtlMoveMemory((void*)((size_t)pObj + offsetof(obj_s, t_left_)), lpPaddingText, 16);
							if (g_Li.DpiX > 1)
							{
								pObj->t_left_ = pObj->t_left_ * g_Li.DpiX;
								pObj->t_top_ = pObj->t_top_ * g_Li.DpiX;
								pObj->t_right_ = pObj->t_right_ * g_Li.DpiX;
								pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiX;
							}
						}
					}
					height = height + rcItem.bottom - rcItem.top;
				}
			}
			_obj_setpos_org(pParnet, objParent, 0, EOP_DEFAULT, EOP_DEFAULT, EOP_DEFAULT, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_EX_NODPISCALE, 0);
			height = height - 1;
			Ex_ObjScrollShow(objPP, SB_VERT, height - heightParent > 0);
			Ex_ObjScrollSetInfo(objPP, SB_VERT, SIF_RANGE | SIF_PAGE, 0, height - heightParent, heightParent, 0, true);
		}
	}
	SendMessageW(hWnd, WM_INITMENUPOPUP, hMenu, 0);
}

void _wnd_menu_init(HWND hWnd, wnd_s* pWnd)
{
	
	if (!((pWnd->dwFlags_ & EWF_bMenuInited) == EWF_bMenuInited))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bMenuInited;
		_wnd_menu_createitems(hWnd, pWnd);
		ShowWindow(pWnd->hWndShadow_, SW_SHOWNOACTIVATE);

		SetWindowLongPtrW(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void _wnd_paint_shadow(wnd_s* pWnd, bool bUpdateRgn, bool bFlush)
{
	
	if (!((pWnd->dwStyle_ & EWS_NOSHADOW) == EWS_NOSHADOW))
	{
		
		if (((pWnd->dwFlags_ & EWF_bRendered) == EWF_bRendered))
		{
			HWND hWnd = pWnd->hWndShadow_;
			POINT ptDst;
			ptDst.x = pWnd->left_;
			ptDst.y = pWnd->top_;
			SIZE sz;
			sz.cx = pWnd->width_;
			sz.cy = pWnd->height_;
			void* prcPadding = pWnd->padding_shadow_;
			RECT rcPadding{ 0 };
			if (prcPadding != 0)
			{
				RtlMoveMemory(&rcPadding, prcPadding, sizeof(RECT));
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
				auto rectround = pWnd->radius_;
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
					int nError = 0;
					HEXCANVAS cvShadow = _canvas_createfrompwnd(pWnd, sz.cx, sz.cy, CVF_GDI_COMPATIBLE, &nError);
					if (cvShadow != 0)
					{
						
						int alpha = ((pWnd->dwFlags_ & EWF_ACTIVE) == EWF_ACTIVE) ? 255 : 204;
						alpha = alpha * pWnd->alpha_ / 255;
						if (_canvas_begindraw(cvShadow))
						{
							
							Ex_ThemeDrawControlEx(pWnd->hTheme_, cvShadow, 1, 1, sz.cx - 2, sz.cy - 2, (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU) ? ATOM_MENU : ATOM_WINDOW), ATOM_RECT, 0, 0, ATOM_BACKGROUND_GRID, 0, alpha);
							void* mDC = _canvas_getdc(cvShadow);
							if (mDC != 0)
							{
								POINT ptSrc = { 0 };
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

bool _wnd_wm_paint(wnd_s* pWnd, HWND hWnd)
{
	PAINTSTRUCT ps = { 0 };
	
	if (((pWnd->dwFlags_ & EWF_INTED) == EWF_INTED))
	{
		
		bool fSized = ((pWnd->dwFlags_ & EWF_SIZED) == EWF_SIZED);
		if (fSized)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_SIZED);
		}
		
		bool fLayer = ((pWnd->dwFlags_ & EWF_bLayered) == EWF_bLayered);

		if (fLayer)
		{
			if (fSized)
			{
				ps.rcPaint.right = pWnd->width_;
				ps.rcPaint.bottom = pWnd->height_;
			}
			else {
				GetUpdateRect(hWnd, &ps.rcPaint, false);
				
				if (((pWnd->dwStyle_ & EWS_MENU) == EWS_MENU))
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
	}
	if (!((pWnd->dwFlags_ & EWF_bRendered) == EWF_bRendered))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRendered;
		_wnd_paint_shadow(pWnd, true, false);
	}
	return false;
}

bool _wnd_wm_getminmaxinfo(wnd_s* pWnd, HWND hWnd, LPARAM lParam)
{
	RECT rcMonitor{ 0 }, rcDesk{ 0 };
	bool ret = false;
	if (_wnd_getscreenrect(hWnd, &rcMonitor, &rcDesk))
	{
		int width = pWnd->width_;
		int height = pWnd->height_;
		
		if (((pWnd->dwStyle_ & EWS_FULLSCREEN) == EWS_FULLSCREEN))//全屏模式
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
		__set_int((void*)lParam, 24, pWnd->minWidth_);
		__set_int((void*)lParam, 28, pWnd->minHeight_);
		//允许调整的最大尺寸
		__set_int((void*)lParam, 32, nMaxWidth);
		__set_int((void*)lParam, 36, nMaxHeight);
		ret = true;
	}
	return ret;
}

void _wnd_wm_buttondown(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & (EWF_bLeftTrack | EWF_bRightTrack | EWF_bMidTrack));
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bTrackObject;
		if (uMsg == WM_LBUTTONDOWN)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bLeftTrack;
			_obj_setfocus(hWnd, pWnd, hObj, pObj, true);
		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRightTrack;
		}
		else {
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bMidTrack;
		}
		pWnd->objTrack_ = hObj;
		_obj_tooltips_popup(pWnd, 0, 0, -1, -1, -1, 0, false);
		_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
		if (wParam != 0)
		{
			SetCapture(hWnd);//对menu窗口无效
		}
	}
}

void _wnd_obj_untrack(HWND hWnd, wnd_s* pWnd, bool fMsgDispatch)
{
	
	if (((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
	{
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bTrackObject);
		HEXOBJ objTrack = pWnd->objTrack_;
		pWnd->objTrack_ = 0;
		pWnd->objTrackPrev_ = objTrack;
		if (!fMsgDispatch)
		{
			ReleaseCapture();//对menu窗口无效
		}
		obj_s* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objTrack, HT_OBJECT, (void**)&pObj, &nError))
		{
			POINT pt;
			GetCursorPos(&pt);
			INT uMsg;
			if (fMsgDispatch)
			{
				
				if (((pWnd->dwFlags_ & EWF_bLeftTrack) == EWF_bLeftTrack))
				{
					uMsg = WM_LBUTTONUP;
				}
				
				else if (((pWnd->dwFlags_ & EWF_bRightTrack) == EWF_bRightTrack))
				{
					uMsg = WM_RBUTTONUP;
				}
				else {
					uMsg = WM_MBUTTONUP;
				}
				_obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, 0);
				_wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
				_wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, (obj_s*)-1, false);
			}
			else {
				_wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
				_wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, (obj_s*)-1, false);
				if (pWnd->objHittest_ == objTrack)
				{
					
					if (((pWnd->dwFlags_ & EWF_bLeftTrack) == EWF_bLeftTrack))
					{
						uMsg = WM_EX_LCLICK;
					}
				
					else if (((pWnd->dwFlags_ & EWF_bRightTrack) == EWF_bRightTrack))
					{
						uMsg = WM_EX_RCLICK;
					}
					else {
						uMsg = WM_EX_MCLICK;
					}
					_obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, pWnd->dwHitObjPos_Abs_);
				}
			}
			_obj_dispatchnotify(hWnd, pObj, objTrack, 0, NM_RELEASEDCAPTURE, 0, 0);
		}
	}
}

void _wnd_wm_captionchange(HWND hWnd, wnd_s* pWnd)
{
	_wnd_obj_untrack(hWnd, pWnd, false);
}

void _wnd_wm_mouse(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ hObj;
	
	if (((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))//是否按住组件
	{
		hObj = pWnd->objTrack_;
	}
	else {
		hObj = pWnd->objHittest_;
	}
	int dwHitCode = pWnd->dwHitCode_;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		if (((pWnd->dwFlags_ & EWF_bTrackObject) == EWF_bTrackObject))
		{
			lParam = MAKELONG(LOWORD(lParam) - pObj->w_left_, HIWORD(lParam) - pObj->w_top_);
			pWnd->dwHitObjPos_Abs_ = lParam;
		}
		else {
			lParam = pWnd->dwHitObjPos_Abs_;
		}
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		if (wParam == 0)
		{
			_wnd_wm_leavecheck(hWnd, pWnd, pWnd->objHittestPrev_, hObj, pObj, true);
		}
		if (hObj != 0)
		{
			_obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
	}
	else {
		if (pObj != 0)
		{
			if (uMsg == WM_LBUTTONDOWN) {
				_wnd_popupclose(pWnd, hWnd, hObj, pObj);
			}
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

void _wnd_wm_menucontext(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ objHittest = pWnd->objHittest_;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(objHittest, HT_OBJECT, (void**)&pObj, &nError))
	{
		pWnd->objMenucontext_ = objHittest;
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bMenuContext;
		_obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam);
		pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_bMenuContext);
	}
}

void _wnd_wm_mousewheel(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ objHittest = pWnd->objHittest_;
	obj_s* pObj = nullptr;
	int nError = 0;

	if (FLAGS_CHECK(pWnd->dwFlags_, EWF_bPopupWindowShown)) {
		hWnd = pWnd->hWndPopup_;
		SendMessage(hWnd, uMsg, wParam, lParam);
		return;
	}

	if (_handle_validate(objHittest, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (_obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam) == 0)//我认为这样是不完善的，因为存在转发WHEEL消息让别的控件滚动的可能性
		{
			EX_CLASSINFO* pCls = pObj->pCls_;
			if (pCls->atomName == ATOM_SCROLLBAR)
			{
				_sb_parentnotify(hWnd, pObj, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, objHittest, 0, true);
			}
			
			if (((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL))
			{
				_obj_baseproc(hWnd, objHittest, pObj, WM_VSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
			}
			
			if (((pObj->dwStyle_ & EOS_HSCROLL) == EOS_HSCROLL))
			{
				_obj_baseproc(hWnd, objHittest, pObj, WM_HSCROLL, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
			}
		}
	}
}

void _wnd_wm_command(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	if (((pWnd->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
	{
		if (wParam == 2)
		{
			if (Ex_ObjIsEnable(Ex_ObjGetFromID(pWnd->objCaption_, EWS_BUTTON_CLOSE)))
			{
				EndDialog(hWnd, IDCANCEL);
			}
		}
	}
	else {
		HEXOBJ objMenucontext = pWnd->objMenucontext_;
		pWnd->objMenucontext_ = 0;
		obj_s* pObj = nullptr;
		int nError = 0;
		if (_handle_validate(objMenucontext, HT_OBJECT, (void**)&pObj, &nError))
		{
			_obj_baseproc(hWnd, objMenucontext, pObj, uMsg, wParam, lParam);
		}
	}
}

HEXOBJ _wnd_obj_setnextfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ objEntry, HEXOBJ objLastFocus)
{
	obj_s* pObjEntry = nullptr;
	int nError = 0;
	while (_handle_validate(objEntry, HT_OBJECT, (void**)&pObjEntry, &nError))
	{
		
		if (((pObjEntry->dwStyleEx_ & EOS_EX_TABSTOP) == EOS_EX_TABSTOP))
		{
			if (objLastFocus != objEntry)
			{
				_obj_setfocus(hWnd, pWnd, objEntry, pObjEntry, true);
			}
			return objEntry;
		}
		objEntry = pObjEntry->objNext_;
	}
	return 0;
}

void _wnd_obj_settabstop(HWND hWnd, wnd_s* pWnd, HEXOBJ objLastFocus)
{
	obj_s* pObjLast = nullptr;
	int nError = 0;
	HEXOBJ objNextFocus;
	if (_handle_validate(objLastFocus, HT_OBJECT, (void**)&pObjLast, &nError))
	{
		HEXOBJ objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pObjLast->objNext_, objLastFocus);
		if (objNextFocus == 0)
		{
			EXHANDLE objParent = pObjLast->objParent_;
			obj_s* pObjParent = nullptr;
			if (_handle_validate(objParent, HT_OBJECT, (void**)&pObjParent, &nError))
			{
				objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pObjParent->objChildFirst_, objLastFocus);
			}
			else {
				objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pWnd->objChildFirst_, objLastFocus);
			}
		}
	}
	else {
		objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pWnd->objChildFirst_, objLastFocus);
	}
}

void _wnd_wm_keyboard(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	HEXOBJ objFocus = pWnd->objFocus_;
	obj_s* pObj = nullptr;
	int nError = 0;
	bool bPopupWindowShown = false;
	if (_handle_validate(objFocus, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (FLAGS_CHECK(pWnd->dwFlags_, EWF_bPopupWindowShown)) {
			if (uMsg >= WM_KEYDOWN && uMsg <= WM_KEYLAST) {
				SendMessage(pWnd->hWndPopup_, uMsg, wParam, lParam);
				bPopupWindowShown = true;
			}
		}
		if (_obj_baseproc(hWnd, objFocus, pObj, uMsg, wParam, lParam) != 0)//先给组件，看他需不需要消息。
		{
			return;
		}
	}
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)
		{
			
			if (((pWnd->dwStyle_ & EWS_ESCEXIT) == EWS_ESCEXIT) && !bPopupWindowShown)
			{
				PostMessageW(hWnd, WM_CLOSE, 0, 0);
			}
		}
		else if (wParam == VK_SPACE)
		{
			if (_wnd_getfunctionkeys() == 0)
			{
				if (pObj != 0)
				{
					if (!__query(pObj,offsetof(obj_s, dwFlags_), eof_bDisableSpaceAndEnter))
					{
						_wnd_wm_buttondown(hWnd, pWnd, objFocus, pObj, WM_LBUTTONDOWN, 0, 0);
					}
				}
			}
		}
		else if (wParam == VK_RETURN)
		{
			if (_wnd_getfunctionkeys() == 0)
			{
				if (pObj != 0)
				{
					if (!__query(pObj, offsetof(obj_s, dwFlags_), eof_bDisableSpaceAndEnter))
					{
						if (_obj_dispatchnotify(hWnd, pObj, objFocus, 0, NM_RETURN, wParam, lParam) == 0)
						{
							_obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
						}
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
				if (!__query(pObj, offsetof(obj_s, dwFlags_), eof_bDisableSpaceAndEnter))
				{
					_wnd_wm_mouse(pWnd, hWnd, WM_LBUTTONUP, 0, 0);
					_obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
				}
			}
		}
	}
}

bool _wnd_obj_childtabstop(HEXOBJ objEntry, HEXOBJ* objFocusable, void** pObjFocusable, HEXOBJ objLast, bool* bFounded)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	bool ret = false;
	while (_handle_validate(objEntry, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (!*bFounded)
		{
			if (objLast = objEntry) *bFounded = true;
		}
		
		if (((pObj->dwStyleEx_ & EOS_EX_TABSTOP) == EOS_EX_TABSTOP))
		{
			*objFocusable = objEntry;
			*pObjFocusable = pObj;
			ret = true;
			break;
		}
		HEXOBJ objSub = pObj->objChildFirst_;
		if (objSub != 0)
		{
			if (_wnd_obj_childtabstop(objSub, objFocusable, pObjFocusable, objLast, bFounded))
			{
				ret = true;
				break;
			}
		}
		objEntry = pObj->objNext_;
	}
	return ret;
}

void _wnd_wm_ime_composition(HWND hWnd, wnd_s* pWnd)
{
	auto hImc = ImmGetContext(hWnd);
	if (hImc != 0)
	{
		POINT pt;
		GetCaretPos(&pt);
		if (FLAGS_CHECK(pWnd->dwFlags_, EWF_bPopupWindowShown)) {
			RECT rect;
			GetWindowRect(pWnd->hWndPopup_, &rect);
			pt.x += rect.left - pWnd->left_;
			pt.y += rect.top - pWnd->top_;
		}
		COMPOSITIONFORM cf;
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos.x = pt.x;
		cf.ptCurrentPos.y = pt.y;
		ImmSetCompositionWindow(hImc, &cf);
		HEXOBJ hObj = pWnd->objFocus_;
		if (hObj != 0)
		{
			obj_s* pObj = nullptr;
			int nError = 0;
			if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
			{
				LOGFONTW* lpLogfont = (LOGFONTW*)Ex_MemAlloc(sizeof(LOGFONTW));
				_font_getlogfont(pObj->hFont_, lpLogfont);
				ImmSetCompositionFontW(hImc, lpLogfont);
				Ex_MemFree(lpLogfont);
			}
		}
		ImmReleaseContext(hWnd, hImc);
	}
}

bool _wnd_wm_measureitem_host(wnd_s* pWnd, WPARAM wParam, LPARAM lParam)
{
	bool ret = false;
	if (wParam == 0)//menu MEASUREITEMSTRUCT https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-measureitemstruct
	{
		if (__get_int((void*)lParam, 0) == ODT_MENU)
		{
			int nID = __get_int((void*)lParam, 8);
			void* ppc = pWnd->padding_client_;
			int offset = 0;
			if (ppc != 0)
			{
				offset = offset + Ex_Scale(__get_int(ppc, 0) + __get_int(ppc, 8));
			}
			void* ppt = pWnd->padding_text_;
			if (ppt != 0)
			{
				offset = offset + Ex_Scale(__get_int(ppt, 0) + __get_int(ppt, 8));
			}
			int szItemSeparator = pWnd->szItemSeparator_;
			char byte = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_));
			int width, height;
			if (nID == 0)//MF_SEPARATOR
			{
				width = byte + offset;
				byte = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_) + 3);
				height = byte;
			}
			else {
				width = pWnd->menu_maxwidth_ + offset;
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

void _wnd_menu_updatecurrent(wnd_s* pWnd)
{
	wnd_s* pHost = pWnd->pMenuHostWnd_;
	if (pHost != 0)
	{
		pHost->pMenuTrackWnd_ = pWnd;
	}
}

bool _wnd_menu_mouse(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, size_t* iItem)
{
	*iItem = -1;
	POINT pt;
	GetCursorPos(&pt);
	_wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
	_wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, MAKELONG(pt.x, pt.y));
	obj_s* pObj = nullptr;
	int nError = 0;
	bool ret = false;
	if (_handle_validate(pWnd->objHittest_, HT_OBJECT, (void**)&pObj, &nError))
	{
		
		if (((pObj->dwStyleEx_ & EOS_EX_FOCUSABLE) == EOS_EX_FOCUSABLE))
		{
		
			if (((pObj->dwFlags_ & eof_bMenuItem) == eof_bMenuItem))
			{
				*iItem = pObj->lParam_;
			}
			ret = true;
		}
	}
	_wnd_menu_updatecurrent(pWnd);
	return ret;
}

bool _wnd_menu_item_callback(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, void** lpResult)
{
	return false;
}

bool _wnd_menu_callback_test(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, void** lpResult)
{
	if (uMsg == WM_INITMENUPOPUP)
	{
		RECT rc{ 0 };
		GetWindowRect(hWnd, &rc);
		MoveWindow(hWnd, rc.left, rc.top, 300, 300, false);
		MENUITEMINFOW mii;
		mii.cbSize = sizeof(MENUITEMINFOW);
		mii.fMask = MIIM_FTYPE | MIIM_SUBMENU | MIIM_ID;
		int nCount = GetMenuItemCount((HMENU)wParam) - 1;
		std::vector<HEXOBJ> aryItems;
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
			HEXOBJ objEntry = Ex_ObjGetObj(aryItems[0], GW_HWNDFIRST);
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
		HEXDUI hdui = Ex_DUIFromWindow(WindowFromPoint(pt));
		wnd_s* pdui = nullptr;
		int nError = 0;
		if (_handle_validate(hdui, HT_DUI, (void**)&pdui, &nError))
		{
			ScreenToClient(pdui->hWnd_, &pt);
			HEXOBJ hObj = _wnd_wm_nchittest_obj((HWND)pdui->hexdui_, pdui, pdui->objChildLast_, pt.x, pt.y, 0, 0);
			obj_s* pObj = nullptr;
			if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
			{
				if (pObj->id_ == 1007)
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

void _wnd_wm_initmenupopup(HWND hWnd, wnd_s* pWnd, void* hMenu)
{
	bool fChecked = false;
	if (!Flag_Query(EXGF_MENU_ALL))
	{
		void* lpMenuParams = pWnd->lpMenuParams_;
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
		HEXCANVAS hCanvas = pWnd->canvas_bkg_;
		HEXFONT hFont = pWnd->hFont_Menu_;

		WCHAR buff[520];
		float width, height, nMax = 0;
		for (int i = 0; i < nCount; i++)
		{
			GetMenuStringW((HMENU)hMenu, i, buff, 520, MF_BYPOSITION);
			_canvas_calctextsize(hCanvas, hFont, buff, -1, DT_SINGLELINE, 0, 0, 0, &width, &height);
			if (nMax < width)
			{
				nMax = width;
			}
		}
		pWnd->menu_maxwidth_ = nMax;
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
			if (HashTable_Set(g_Li.hTableLayout, (size_t)hMenu, pWnd->hexdui_))
			{
				pWnd->hMenuPopup_ = hMenu;
				pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bMenuInited;
			}
		}
	}
}

BOOL Ex_TrackPopupMenu(HMENU hMenu, DWORD uFlags, INT x, INT y, size_t nReserved, EXHANDLE handle, RECT* lpRC, MsgPROC pfnCallback, DWORD dwFlags)
{
	HWND hWnd = 0;
	wnd_s* pWnd = nullptr;
    menu_s menu{};
	if (_wnd_getfromhandle(handle, &hWnd, &pWnd))
	{
	    menu.hMenu_ = hMenu;
	    menu.uFlags_ = uFlags;
	    menu.x_ = x;
	    menu.y_ = y;
	    menu.nReserved_ = nReserved;
	    menu.handle_ = handle;
	    menu.lpRC_ = lpRC;
	    menu.pfnCallback_ = pfnCallback;
        menu.dwFlags_ = dwFlags;
		pWnd->lpMenuParams_ = &menu;
	}
	BOOL ret = TrackPopupMenu((HMENU)hMenu, uFlags, x, y, 0, hWnd, lpRC);
	return ret;
}

INT Ex_MessageBoxEx(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, LPCWSTR lpCheckBox, BOOL* lpCheckBoxChecked, INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc)
{
    HWND hWnd = 0;
    wnd_s* pWnd = nullptr;
    mbp_s mbp{};
    if (_wnd_getfromhandle(handle, &hWnd, &pWnd))
    {
        mbp.handle_ = handle;
        mbp.lpText_ = lpText;
        mbp.lpCaption_ = lpCaption;
        mbp.uType_ = uType;
        mbp.lpCheckBox_ = lpCheckBox;
        mbp.lpCheckBoxChecked_ = lpCheckBoxChecked;
        mbp.dwMilliseconds_ = dwMilliseconds;
        mbp.dwFlags_ = dwFlags;
        mbp.lpfnNotifyCallback_ = lpfnMsgProc;

        pWnd->lpMsgParams_ = &mbp;
    }
    auto ret = MessageBoxW(hWnd, lpText, lpCaption, uType);
	return ret;
}

INT Ex_MessageBox(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, INT dwFlags)
{
	return Ex_MessageBoxEx(handle, lpText, lpCaption, uType, 0, 0, 0, dwFlags, 0);
}

BOOL Ex_DUITrayIconPopup(HEXDUI hExDui, LPCWSTR lpwzInfo, LPCWSTR lpwzInfoTitle, INT dwInfoFlags)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		void* lpNid = pWnd->lpNid_;
		if (lpNid != 0)
		{
			((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_INFO;
			RtlZeroMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), 512);
			int len = lstrlenW(lpwzInfo);
			if (len > 255) len = 255;
			RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), lpwzInfo, len * 2);
			RtlZeroMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)), 128);
			len = lstrlenW(lpwzInfoTitle);
			if (len > 63) len = 63;
			RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)), lpwzInfoTitle, len * 2);
			((NOTIFYICONDATAW*)lpNid)->dwInfoFlags = dwInfoFlags;
			Shell_NotifyIconW(NIM_MODIFY, (PNOTIFYICONDATAW)lpNid);
		}
	}
	return nError == 0;
}

BOOL Ex_DUITrayIconSet(HEXDUI hExDui, HICON hIcon, LPCWSTR lpwzTips)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		HWND hWnd = pWnd->hWnd_;
		int len = lstrlenW(lpwzTips);
		void* lpNid = pWnd->lpNid_;
		if (lpNid == 0)
		{
			if (hIcon == 0)
			{
				hIcon = _wnd_geticonhandle(hWnd, false);
			}
			void* lpNid = Ex_MemAlloc(sizeof(NOTIFYICONDATAW));
			
			((NOTIFYICONDATAW*)lpNid)->cbSize = sizeof(NOTIFYICONDATAW);
			((NOTIFYICONDATAW*)lpNid)->hWnd = hWnd;
			((NOTIFYICONDATAW*)lpNid)->uID = 1;
			((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			((NOTIFYICONDATAW*)lpNid)->uCallbackMessage = g_Li.dwMessage;
			((NOTIFYICONDATAW*)lpNid)->hIcon = hIcon;
			if (len != 0)
			{
				if (len > 127) len = 127;
				RtlMoveMemory((void*)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), lpwzTips, len * 2);
			}
			Shell_NotifyIconW(NIM_ADD, (PNOTIFYICONDATAW)lpNid);
			pWnd->lpNid_ = lpNid;
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
	return nError == 0;
}

bool Ex_DUIGetBackgroundImage(HEXDUI hExDui, void* lpBackgroundImage)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		if (IsBadWritePtr(lpBackgroundImage, sizeof(EX_BACKGROUNDIMAGEINFO)))
		{
			nError = ERROR_EX_MEMORY_BADPTR;
		}
		else {
			RtlMoveMemory(lpBackgroundImage, pWnd->lpBackgroundImage_, sizeof(EX_BACKGROUNDIMAGEINFO));
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _wnd_getclientrect(wnd_s* pWnd, RECT* lpRect, int* nError)
{
	if (IsBadWritePtr(lpRect, 16))
	{
		*nError = ERROR_EX_BAD_SIZE;
	}
	else {
		RtlZeroMemory(lpRect, 16);
		lpRect->right = pWnd->width_;
		lpRect->bottom = pWnd->height_;
		if (Flag_Query(EXGF_DPI_ENABLE))
		{
			lpRect->right = (float)lpRect->right / g_Li.DpiX;
			lpRect->bottom = (float)lpRect->bottom / g_Li.DpiY;
		}
	}
}

BOOL Ex_DUIGetClientRect(HEXDUI hExDui, RECT* lpClientRect)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		_wnd_getclientrect(pWnd, lpClientRect, &nError);
	}
	return nError == 0;
}

size_t Ex_DUIGetLong(HEXDUI hExDui, INT nIndex)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		if (nIndex == EWL_ALPHA)
		{
			ret = pWnd->alpha_;
		}
		else if (nIndex == EWL_BLUR)
		{
			ret = pWnd->fBlur_;
		}
		else if (nIndex == EWL_CRBKG)
		{
			ret = (size_t)pWnd->crBkg_;
		}
		else if (nIndex == EWL_CRBORDER)
		{
			ret = (size_t)pWnd->crBorder_;
		}
		else if (nIndex == EWL_HTHEME)
		{
			ret = (size_t)pWnd->hTheme_;
		}
		else if (nIndex == EWL_HWND)
		{
			ret = (size_t)pWnd->hWnd_;
		}
		else if (nIndex == EWL_LPARAM)
		{
			ret = pWnd->lParam_;
		}
		else if (nIndex == EWL_MINHEIGHT)
		{
			ret = pWnd->minHeight_;
		}
		else if (nIndex == EWL_MINWIDTH)
		{
			ret = pWnd->minWidth_;
		}
		else if (nIndex == EWL_MSGPROC)
		{
			ret =(size_t) pWnd->pfnMsgProc_;
		}
		else if (nIndex == EWL_OBJCAPTION)
		{
			ret = (size_t)pWnd->objCaption_;
		}
		else if (nIndex == EWL_OBJFOCUS)
		{
			ret = (size_t)pWnd->objFocus_;
		}
		else if (nIndex == EWL_RADIUS)
		{
			ret = (size_t)pWnd->radius_;
		}
		else {
			EX_ASSERT(false, L"Ex_DUIGetLong: unknown EWL index: %ld", nIndex);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

size_t Ex_DUISetLong(HEXDUI hExDui, INT nIndex, size_t dwNewlong)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		bool bRedraw = false;
		if (nIndex == EWL_ALPHA)
		{
			ret = (size_t)pWnd->alpha_;
			pWnd->alpha_ = dwNewlong;
			FLAGS_DEL(pWnd->dwFlags_, EWF_bRendered); //强制阴影重画
		}
		else if (nIndex == EWL_BLUR)
		{
			ret = pWnd->fBlur_;
			pWnd->fBlur_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_CRBKG)
		{
			ret = (size_t)pWnd->crBkg_;
			pWnd->crBkg_ = dwNewlong;
			pWnd->dwStyle_ = pWnd->dwStyle_ | EWS_NOINHERITBKG;
			bRedraw = true;
		}
		else if (nIndex == EWL_CRBORDER)
		{
			ret = (size_t)pWnd->crBorder_;
			pWnd->crBorder_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_HTHEME)
		{
			ret = (size_t)pWnd->hTheme_;
			pWnd->hTheme_ = (HEXTHEME)dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_HWND)
		{
			ret = (size_t)pWnd->hWnd_;
			pWnd->hWnd_ = (HWND)dwNewlong;
		}
		else if (nIndex == EWL_LPARAM)
		{
			ret = pWnd->lParam_;
			pWnd->lParam_ = dwNewlong;
		}
		else if (nIndex == EWL_MINHEIGHT)
		{
			ret = pWnd->minHeight_;
			pWnd->minHeight_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_MINWIDTH)
		{
			ret = pWnd->minWidth_;
			pWnd->minWidth_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_MSGPROC)
		{
			ret = (size_t)pWnd->pfnMsgProc_;
			pWnd->pfnMsgProc_ = (WinMsgPROC)dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_OBJCAPTION)
		{
			ret = pWnd->objCaption_;
			pWnd->objCaption_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_OBJFOCUS)
		{
			ret = pWnd->objFocus_;
			pWnd->objFocus_ = dwNewlong;
			bRedraw = true;
		}
		else if (nIndex == EWL_RADIUS)
		{
			ret = (size_t)pWnd->radius_;
			pWnd->radius_ = dwNewlong;
			bRedraw = true;
		}
		else {
			EX_ASSERT(false, L"Ex_DUISetLong: unknown EWL index: %ld", nIndex);
		}
		if (bRedraw)
		{
			pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_bRedrawBackground;
			_wnd_redraw_bkg(pWnd->hWnd_, pWnd, 0, true, false);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_DUIShowWindowEx(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags, INT uEasing, WPARAM wParam, LPARAM lParam)
{
	wnd_s* pWnd = nullptr;
	int nError = 0;
	BOOL ret = FALSE;
	if (_handle_validate(hExDui, HT_DUI, (void**)&pWnd, &nError))
	{
		HWND hWnd = pWnd->hWnd_;
		//刷新组件层
		pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_INTED;
		if (FLAGS_CHECK(pWnd->dwStyle_, EWS_POPUPWINDOW)) { //弹出窗不获取焦点
			nCmdShow = SW_SHOWNOACTIVATE;
		}
		ret = ShowWindow(hWnd, nCmdShow);
		if (GetFocus() == hWnd) {
			FLAGS_ADD(pWnd->dwFlags_, EWF_ACTIVE);
		}
		ret = InvalidateRect(hWnd, NULL, FALSE);
		ret = UpdateWindow(hWnd);
	}
	Ex_SetLastError(nError);
	return ret;
}

BOOL Ex_DUIShowWindow(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags)
{
	return Ex_DUIShowWindowEx(hExDui, nCmdShow, dwTimer, dwFrames, dwFlags, 0, 0, 0);
}

HEXDUI Ex_DUIBindWindowEx(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle, LPARAM lParam, WinMsgPROC lpfnMsgProc)
{
	int nError = 0;
	wnd_s* pWnd = nullptr;
	HEXDUI hExDui = 0;
	if (IsWindow(hWnd))
	{
		pWnd = (wnd_s*)Ex_MemAlloc(sizeof(wnd_s));
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
		EX_ASSERT(FALSE, L"wnd create error: %02X", nError);
		if (pWnd != 0)
		{
			Ex_MemFree(pWnd);
		}
		if (hExDui != 0)
		{
			nError = 0;
			_handle_destroy(hExDui, &nError);
		}
	}
	Ex_SetLastError(nError);
	return hExDui;
}

HEXDUI Ex_DUIBindWindow(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle)
{
	return Ex_DUIBindWindowEx(hWnd, hTheme, dwStyle, 0, 0);
}

HEXOBJ Ex_DUIGetObjFromPoint(EXHANDLE handle, INT x, INT y)
{
	wnd_s* pWnd;
	RECT rect = { 0 };
	HWND hWnd = 0;
	obj_s* pObj = 0;
	POINT point{ x,y };
	if (!handle)
	{
		GetCursorPos(&point);
		HWND hWnd = WindowFromPoint(point);
		if (hWnd)
		{
			GetWindowRect(hWnd, &rect);
			point.x -= rect.left;
			point.y -= rect.top;
		}
	}
	else if (_wnd_getfromhandle((size_t)handle, &hWnd, &pWnd))
	{
		int hitCode = 0;
		_wnd_wm_nchittest_obj(hWnd, pWnd, pWnd->objChildLast_, point.x, point.y, &hitCode, &pObj);
		if (pObj)
			return pObj->hObj_;
	}
	return 0;
}