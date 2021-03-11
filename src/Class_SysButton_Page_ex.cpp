#include "Class_SysButton_Page_ex.h"

size_t _sysbutton_paint(HWND hWnd, EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s ps;
	int atomClass = 0;
	int atomState = 0;
	if (Ex_ObjBeginPaint(hObj, (paintstruct_s*)&ps))
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
			if (((pObj->pWnd_->dwStyle_ & EWS_HASICON) == EWS_HASICON))
			{
				size_t atomClass = 窗口_取图标句柄(hWnd, false);
				if (atomClass != 0)
				{
					EXHANDLE hImg = _img_createfromhicon((void*)atomClass);
					if (hImg != 0)
					{
						_canvas_drawimagerect(ps.hCanvas_, hImg, left, (ps.t_bottom_ - ps.t_top_ - 16) / 2, left + 16, (ps.t_bottom_ - ps.t_top_ - 16) / 2 + 16, 255);
						_img_destroy(hImg);
						left = left + 20;
					}
				}
			}
			
			if (((pObj->pWnd_->dwStyle_ & EWS_TITLE) == EWS_TITLE))
			{
				_canvas_drawtextex(ps.hCanvas_, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), (LPCWSTR)(pObj->pstrTitle_), -1,
					DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_WORD_ELLIPSIS, left, ps.t_top_, ps.t_right_, ps.t_bottom_, pObj->dwShadowSize_, 0, 0, 0);
			}
		}
		Ex_ObjEndPaint(hObj, (paintstruct_s*)&ps);
	}
	return 0;
}

void _sysbutton_remove_proc(obj_s* pObj, int width, int height)
{
	EXHANDLE sObj = pObj->objChildFirst_;
	obj_s* psobj = nullptr;
	int nError = 0;
	bool bReCalced = false;
	int nOffset = 0;
	while (_handle_validate(sObj, HT_OBJECT, (void**)&psobj, &nError))
	{
		if (!bReCalced)
		{
			bReCalced = true;
			nOffset = width - psobj->right_;
		}
		Ex_ObjSetPos(sObj, 0, psobj->left_ + nOffset, EOP_DEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_EX_NODPISCALE);
		sObj = psobj->objNext_;
	}
}

size_t _sysbutton_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	int nError = 0;
	if (uMsg == WM_NCCREATE)
	{
		void* ret = nullptr;
		if (((pObj->dwStyle_ & EWS_BUTTON_MIN) == EWS_BUTTON_MIN))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_min), lstrlenW((LPCWSTR)(g_Li.lpstr_min)));
		}
		else if (((pObj->dwStyle_ & EWS_BUTTON_CLOSE) == EWS_BUTTON_CLOSE))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_close), lstrlenW((LPCWSTR)(g_Li.lpstr_close)));
		}
		else if (((pObj->dwStyle_ & EWS_BUTTON_HELP) == EWS_BUTTON_HELP))
		{
			ret = copytstr((LPCWSTR)(g_Li.lpstr_help), lstrlenW((LPCWSTR)(g_Li.lpstr_help)));
		}
		if (ret != 0)
		{
			pObj->pstrTips_ = ret;
		}
	}
	else if (uMsg == WM_NCHITTEST)
	{
		auto ret = pObj->dwStyle_;
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
		if (!((pObj->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_点燃, false, 0, true, &nError);
			if (((pObj->dwStyle_ & EWS_BUTTON_MAX) == EWS_BUTTON_MAX))
			{
				Ex_MemFree(pObj->pstrTips_);
				if (窗口_查询风格(hWnd, WS_MAXIMIZE, false))
				{
					ret = copytstr((LPCWSTR)g_Li.lpstr_res_max, lstrlenW((LPCWSTR)g_Li.lpstr_res_max));
				}
				else {
					ret = copytstr((LPCWSTR)g_Li.lpstr_max, lstrlenW((LPCWSTR)g_Li.lpstr_max));
				}
				pObj->pstrTips_ = ret;
			}
			else if (((pObj->dwStyle_ & EWS_BUTTON_MIN) == EWS_BUTTON_MIN))
			{
				Ex_MemFree(pObj->pstrTips_);
				if (窗口_查询风格(hWnd, WS_MINIMIZE, false))
				{
					ret = copytstr((LPCWSTR)g_Li.lpstr_res_min, lstrlenW((LPCWSTR)g_Li.lpstr_res_min));
				}
				else {
					ret = copytstr((LPCWSTR)g_Li.lpstr_min, lstrlenW((LPCWSTR)g_Li.lpstr_min));
				}
				pObj->pstrTips_ = ret;
			}
		}
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		if (!((pObj->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		if (!((pObj->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_按下, false, 0, true, &nError);
		}
	}
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP)
	{
		if (!((pObj->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			_obj_setuistate(pObj, 状态_按下, true, 0, true, &nError);
		}
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		int ret = pObj->dwStyle_;
		if ((ret & EWS_BUTTON_CLOSE) != 0)
		{
			if (((pObj->pWnd_->dwStyle_ & EWS_MESSAGEBOX) == EWS_MESSAGEBOX))
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
		if (((pObj->dwStyle_ & EWS_TITLE) == EWS_TITLE))
		{
			_sysbutton_remove_proc(pObj, LOWORD(lParam), HIWORD(lParam));
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _page_paint(EXHANDLE hObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, (paintstruct_s*)&ps))
	{
		Ex_ObjEndPaint(hObj, (paintstruct_s*)&ps);
	}
	return 0;
}

void _page_onvscrollbar(HWND hWnd, EXHANDLE hObj, obj_s* pObj, int uMsg, size_t wParam, size_t lParam)
{
	auto nCode = LOWORD(wParam);
	int oPos = Ex_ObjScrollGetPos(hObj, SB_VERT);
	int height = pObj->c_bottom_ - pObj->c_top_;
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
		nPos = oPos - HIWORD(LOWORD(pObj->pWnd_->szItemSeparator_));
	}
	else if (nCode == SB_LINEDOWN)
	{
		nPos = oPos + HIWORD(LOWORD(pObj->pWnd_->szItemSeparator_));
	}
	else if (nCode == SB_TOP)
	{
		nPos = 0;
	}
	else if (nCode == SB_BOTTOM)
	{
		RECT rect;
		Ex_ObjGetRect(pObj->objChildFirst_, &rect);
		nPos = rect.bottom - rect.top - height;
	}
	else
	{
		return;
	}
	nPos = Ex_ObjScrollSetPos(hObj, SB_VERT, nPos, true);
	Ex_ObjSetPos(pObj->objChildFirst_, 0, EOP_DEFAULT, (-nPos), 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOACTIVATE | SWP_EX_NODPISCALE);
}

size_t _page_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL)
	{
		if (((pObj->dwStyle_ & 条目风格_子菜单) == 条目风格_子菜单))
		{
			_page_onvscrollbar(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}