#include "Class_SysButton_Page_ex.h"

size_t _sysbutton_paint(HWND hWnd, size_t hObj, void* pObj)
{
	paintstruct_s ps;
	int atomClass = 0;
	int atomState = 0;
	if (Ex_ObjBeginPaint(hObj, (void*)&ps))
	{
		if ((ps.dwStyle_ & EWS_BUTTON_CLOSE) != 0)
		{
			atomClass = ATOM_SYSBUTTON_CLOSE;
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_MAX) != 0)
		{
			if (窗口_查询风格(hWnd, WS_MAXIMIZE, false))
			{
				atomClass = ATOM_SYSBUTTON_RESTORE;
			}
			else {
				atomClass = ATOM_SYSBUTTON_MAX;
			}
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_MIN) != 0)
		{
			if (窗口_查询风格(hWnd, WS_MINIMIZE, false))
			{
				atomClass = ATOM_SYSBUTTON_RESTORE;
			}
			else {
				atomClass = ATOM_SYSBUTTON_MIN;
			}
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_MENU) != 0)
		{
			atomClass = ATOM_SYSBUTTON_MENU;
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_SETTING) != 0)
		{
			atomClass = ATOM_SYSBUTTON_SETTING;
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_SKIN) != 0)
		{
			atomClass = ATOM_SYSBUTTON_SKN;
		}
		else if ((ps.dwStyle_ & EWS_BUTTON_HELP) != 0)
		{
			atomClass = ATOM_SYSBUTTON_HELP;
		}

		if ((ps.dwState_ & 状态_按下) != 0)
		{
			atomState = ATOM_DOWN;
		}
		else if ((ps.dwState_ & 状态_点燃) != 0)
		{
			atomState = ATOM_HOVER;
		}
		else {
			atomState = ATOM_NORMAL;
		}

		int left;
		Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, atomClass, atomState, 255);
		if ((ps.dwStyle_ & EWS_TITLE) != 0)
		{
			left = ps.t_left_;
			if (__query(((obj_s*)pObj)->pwnd_, offsetof(wnd_s, dwStyle_), EWS_HASICON))
			{
				size_t atomClass = 窗口_取图标句柄(hWnd, false);
				if (atomClass != 0)
				{
					size_t hImg = _img_createfromhicon((void*)atomClass);
					if (hImg != 0)
					{
						_canvas_drawimagerect(ps.hCanvas_, hImg, left, (ps.t_bottom_ - ps.t_top_ - 16) / 2, left + 16, (ps.t_bottom_ - ps.t_top_ - 16) / 2 + 16, 255);
						_img_destroy(hImg);
						left = left + 20;
					}
				}
			}
			if (__query(((obj_s*)pObj)->pwnd_, offsetof(wnd_s, dwStyle_), EWS_TITLE))
			{
				_canvas_drawtextex(ps.hCanvas_, ((obj_s*)pObj)->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)(((obj_s*)pObj)->pstrTitle_), -1,
					DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_WORD_ELLIPSIS, left, ps.t_top_, ps.t_right_, ps.t_bottom_, ((obj_s*)pObj)->dwShadowSize_, 0, 0, 0);
			}
		}
		Ex_ObjEndPaint(hObj, (void*)&ps);
	}
	return 0;
}

void _sysbutton_remove_proc(void* pObj, int width, int height)
{
	ExHandle sObj = ((obj_s*)pObj)->objChildFirst_;
	void* psobj = nullptr;
	int nError = 0;
	bool bReCalced = false;
	int nOffset = 0;
	while (_handle_validate(sObj, HT_OBJECT, &psobj, &nError))
	{
		if (!bReCalced)
		{
			bReCalced = true;
			nOffset = width - ((obj_s*)psobj)->right_;
		}
		Ex_ObjSetPos(sObj, 0, ((obj_s*)psobj)->left_ + nOffset, EOP_DEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_EX_NODPISCALE);
		sObj = ((obj_s*)psobj)->objNext_;
	}
}

size_t _sysbutton_proc(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, void* pObj)
{
	int nError = 0;
	if (uMsg == WM_NCCREATE)
	{
		void* ret = nullptr;
		if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_BUTTON_MIN))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_min), lstrlenW((LPCWSTR)(g_Li.lpstr_min)));
		}
		else if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_BUTTON_CLOSE))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_close), lstrlenW((LPCWSTR)(g_Li.lpstr_close)));
		}
		else if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_BUTTON_HELP))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_help), lstrlenW((LPCWSTR)(g_Li.lpstr_help)));
		}
		if (ret != 0)
		{
			((obj_s*)pObj)->pstrTips_ = ret;
		}
	}
	else if (uMsg == WM_NCHITTEST)
	{
		auto ret = ((obj_s*)pObj)->dwStyle_;
		if ((ret & EWS_TITLE) != 0)
		{
			if ((ret & EWS_HASICON) != 0)
			{
				ret = HTCAPTION;
			}
			else {
				ret = HTCAPTION;
			}
			return ret;
		}
	}
	else if (uMsg == WM_PAINT)
	{
		return _sysbutton_paint(hWnd, hObj, pObj);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		void* ret = nullptr;
		if (!__query(pObj, offsetof(obj_s, dwStyle_), EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_点燃, false, 0, true, &nError);
			if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_BUTTON_MAX))
			{
				释放内存(((obj_s*)pObj)->pstrTips_);
				if (窗口_查询风格(hWnd, WS_MAXIMIZE, false))
				{
					ret = copytstr((LPCWSTR)g_Li.lpstr_res_max, lstrlenW((LPCWSTR)g_Li.lpstr_res_max));
				}
				else {
					ret = copytstr((LPCWSTR)g_Li.lpstr_max, lstrlenW((LPCWSTR)g_Li.lpstr_max));
				}
				((obj_s*)pObj)->pstrTips_ = ret;
			}
			else if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_BUTTON_MIN))
			{
				释放内存(((obj_s*)pObj)->pstrTips_);
				if (窗口_查询风格(hWnd, WS_MINIMIZE, false))
				{
					ret = copytstr((LPCWSTR)g_Li.lpstr_res_min, lstrlenW((LPCWSTR)g_Li.lpstr_res_min));
				}
				else {
					ret = copytstr((LPCWSTR)g_Li.lpstr_min, lstrlenW((LPCWSTR)g_Li.lpstr_min));
				}
				((obj_s*)pObj)->pstrTips_ = ret;
			}
		}
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		if (!__query(pObj, offsetof(obj_s, dwStyle_), EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		if (!__query(pObj, offsetof(obj_s, dwStyle_), EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_按下, false, 0, true, &nError);
		}
	}
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP)
	{
		if (!__query(pObj, offsetof(obj_s, dwStyle_), EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_按下, true, 0, true, &nError);
		}
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		int ret = ((obj_s*)pObj)->dwStyle_;
		if ((ret & EWS_BUTTON_CLOSE) != 0)
		{
			if (__query(((obj_s*)pObj)->pwnd_, offsetof(wnd_s, dwStyle_), EWS_MESSAGEBOX))
			{
				EndDialog(hWnd, IDCANCEL);
			}
			else {
				SendMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
			}
		}
		else if ((ret & EWS_BUTTON_MAX) != 0)
		{
			if (窗口_查询风格(hWnd, WS_MAXIMIZE, false))
			{
				SendMessageW(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			}
			else {
				SendMessageW(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
			_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
		}
		else if ((ret & EWS_BUTTON_MIN) != 0)
		{
			if (窗口_查询风格(hWnd, WS_MINIMIZE, false))
			{
				SendMessageW(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			}
			else {
				SendMessageW(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
		}
	}
	else if (uMsg == WM_SETFOCUS)
	{
		return 0;
	}
	else if (uMsg == WM_SIZE)
	{
		if (__query(pObj, offsetof(obj_s, dwStyle_), EWS_TITLE))
		{
			_sysbutton_remove_proc(pObj, 取低位(lParam), 取高位(lParam));
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _page_paint(size_t hObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, (void*)&ps))
	{
		Ex_ObjEndPaint(hObj, (void*)&ps);
	}
	return 0;
}

void _page_onvscrollbar(HWND hWnd, size_t hObj, void* pObj, int uMsg, size_t wParam, size_t lParam)
{
	auto nCode = 取低位(wParam);
	int oPos = Ex_ObjScrollGetPos(hObj, SB_VERT);
	int height = ((obj_s*)pObj)->c_bottom_ - ((obj_s*)pObj)->c_top_;
	int nPos = 0;
	if (nCode == SB_THUMBPOSITION)
	{
		nPos = oPos;
	}
	else if (nCode == SB_PAGEUP)
	{
		nPos = oPos - height;
	}
	else if (nCode == SB_PAGEDOWN)
	{
		nPos = oPos + height;
	}
	else if (nCode == SB_LINEUP)
	{
		nPos = oPos - 取高位(取低位(((wnd_s*)(((obj_s*)pObj)->pwnd_))->szItemSeparator_));
	}
	else if (nCode == SB_LINEDOWN)
	{
		nPos = oPos + 取高位(取低位(((wnd_s*)(((obj_s*)pObj)->pwnd_))->szItemSeparator_));
	}
	else if (nCode == SB_TOP)
	{
		nPos = 0;
	}
	else if (nCode == SB_BOTTOM)
	{
		RECT rect;
		Ex_ObjGetRect(((obj_s*)pObj)->objChildFirst_, &rect);
		nPos = rect.bottom - rect.top - height;
	}
	else
	{
		return;
	}
	nPos = Ex_ObjScrollSetPos(hObj, SB_VERT, nPos, true);
	Ex_ObjSetPos(((obj_s*)pObj)->objChildFirst_, 0, EOP_DEFAULT, (-nPos), 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOACTIVATE | SWP_EX_NODPISCALE);
}

size_t _page_proc(HWND hWnd, size_t hObj, int uMsg, size_t wParam, size_t lParam, void* pObj)
{
	if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL)
	{
		if (__query(pObj, offsetof(obj_s, dwStyle_), 条目风格_子菜单))
		{
			_page_onvscrollbar(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}