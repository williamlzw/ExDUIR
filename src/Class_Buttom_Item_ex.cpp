#include "Class_Button_Item_ex.h"


size_t _button_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_PAINT)
	{
		_button_paint(hObj, pObj);
	}
	else if (uMsg == WM_EX_LCLICK | uMsg == BM_CLICK)
	{
		if ((pObj->dwFlags_ & eof_bMsgBoxControl) == eof_bMsgBoxControl)
		{
			return EndDialog(hWnd, pObj->lParam_);
		}
		else
		{
			if (uMsg == BM_CLICK)
			{
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CLICK, wParam, lParam);
			}
			if ((pObj->dwStyle_ & (按钮风格_复选按钮 | 按钮风格_单选按钮)) != 0)
			{
				if ((pObj->dwStyle_ & 按钮风格_复选按钮) == 按钮风格_复选按钮)
				{
					_obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, !((pObj->dwState_ & 状态_选中) == 状态_选中), 0);
				}
				else {
					if (!((pObj->dwState_ & 状态_选中) == 状态_选中))
					{
						_obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, 1, 0);
					}
				}
			}
			else {
				//未完成Ex_TrackPopupMenu(g_)
			}
		}
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		int nError = 0;
		_obj_setuistate(pObj, 状态_点燃, false, 0, true, &nError);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		int nError = 0;
		_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, &nError);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		int nError = 0;
		_obj_setuistate(pObj, 状态_按下, false, 0, true, &nError);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		int nError = 0;
		_obj_setuistate(pObj, 状态_按下, true, 0, true, &nError);
	}
	else if (uMsg == WM_SETFOCUS)
	{
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
	else if (uMsg == BM_SETCHECK)
	{
		if ((pObj->dwStyle_ & (按钮风格_单选按钮, 按钮风格_复选按钮)) != 0)
		{
			if ((pObj->dwStyle_ & 按钮风格_单选按钮) == 按钮风格_单选按钮)
			{
				if (wParam == 0)
				{
					if (!((pObj->dwState_ & 状态_选中) == 状态_选中))
					{
						return 0;
					}
				}
				else
				{
					_obj_notify_brothers(hWnd, hObj, pObj, uMsg, 0, 0, true, true);
				}
			}
			else
			{
				if ((pObj->dwStyle_ & 按钮风格_复选按钮) == 按钮风格_复选按钮)
				{
					_obj_setuistate(pObj, 状态_半选中 | 状态_选择, true, 0, false, 0);
					if (wParam == 2)
					{
						_obj_setuistate(pObj, 状态_半选中, false, 0, true, 0);
						return 0;
					}
				}
			}
			_obj_setuistate(pObj, 状态_选中, wParam == 0, 0, true, 0);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CHECK, wParam, lParam);
		}
	}
	else if (uMsg == BM_GETCHECK)
	{
		if ((pObj->dwState_ & 状态_半选中) == 状态_半选中)
		{
			return 2;
		}
		else if ((pObj->dwState_ & 状态_选中) == 状态_选中)
		{
			return 1;
		}
		else {
			return 0;
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _button_paint(EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s ps = { 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		bool fButton = (ps.dwStyle_ & (按钮风格_单选按钮 | 按钮风格_复选按钮)) == 0;
		int atomClass;
		int atomProp;
		int nIndex;
		if (fButton)
		{
			atomClass = ATOM_BUTTON;
			if ((ps.dwState_ & 状态_按下) != 0)
			{
				atomProp = ATOM_DOWN;
				nIndex = COLOR_EX_TEXT_DOWN;
			}
			else if ((ps.dwState_ & 状态_点燃) != 0)
			{
				atomProp = ATOM_HOVER;
				nIndex = COLOR_EX_TEXT_HOVER;
			}
			else {
				atomProp = ATOM_NORMAL;
				nIndex = COLOR_EX_TEXT_NORMAL;
			}

			if ((ps.dwStyleEx_ & EOS_EX_CUSTOMDRAW) == 0)
			{
				Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, atomClass, atomProp, 255);
				if ((ps.dwState_ & 状态_焦点) != 0)
				{
					Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, atomClass, ATOM_FOCUS, 255);
				}
			}
		}
		else
		{
			fButton = (ps.dwStyle_ & 按钮风格_单选按钮) != 0;
			atomClass = fButton ? ATOM_RADIOBUTTON : ATOM_CHECKBUTTON;
			if ((ps.dwState_ & 状态_按下) != 0)
			{
				atomProp = _button_getprop(ps.dwState_, fButton, ATOM_DOWN, ATOM_CHECK_DOWN, ATOM_HALF_DOWN);
				nIndex = COLOR_EX_TEXT_DOWN;
			}
			else if ((ps.dwState_ & 状态_点燃) != 0)
			{
				atomProp = _button_getprop(ps.dwState_, fButton, ATOM_HOVER, ATOM_CHECK_HOVER, ATOM_HALF_HOVER);
				nIndex = COLOR_EX_TEXT_HOVER;
			}
			else if ((ps.dwState_ & 状态_焦点) != 0)
			{
				atomProp = _button_getprop(ps.dwState_, fButton, ATOM_FOCUS, ATOM_CHECK_FOCUS, ATOM_HALF_FOCUS);
				nIndex = COLOR_EX_TEXT_NORMAL;
			}
			else
			{
				atomProp = _button_getprop(ps.dwState_, fButton, ATOM_NORMAL, ATOM_CHECK_NORMAL, ATOM_HALF_NORMAL);
				nIndex = COLOR_EX_TEXT_NORMAL;
			}

			if ((ps.dwState_ & 状态_选中) != 0)
			{
				nIndex = COLOR_EX_TEXT_CHECKED;
			}

			if ((ps.dwStyleEx_ & EOS_EX_CUSTOMDRAW) == 0)
			{
				Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, atomClass, atomProp, 255);
			}
		}
		void* lptext = pObj->pstrTitle_;
		if (lptext != 0)
		{
			if ((ps.dwState_ & 状态_按下) != 0 && (ps.dwStyle_ & 按钮风格_文本偏移) != 0)
			{
				OffsetRect((LPRECT)&ps.t_left_, Ex_Scale(1), Ex_Scale(1));
			}
			_canvas_drawtextex(ps.hCanvas_, pObj->hFont_, _obj_getcolor(pObj, nIndex), (LPCWSTR)lptext, -1, ps.dwTextFormat_, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}

int _button_getprop(int state, bool fRadio, int atom_src, int atom_check, int atom_half)
{
	int atomProp;
	if (fRadio)
	{
		atomProp = (state & 状态_选中) != 0 ? atom_check : atom_src;
	}
	else
	{
		if ((state & 状态_半选中) != 0)
		{
			atomProp = atom_half;
		}
		else if ((state & 状态_选中) != 0)
		{
			atomProp = atom_check;
		}
		else
		{
			atomProp = atom_src;
		}
	}
	return atomProp;
}

void _item_click(HWND hWnd, obj_s* pObj)
{
	if ((pObj->dwFlags_ & eof_bMenuItem) == eof_bMenuItem)
	{
		if ((pObj->dwStyle_ & 条目风格_分隔符) == 条目风格_分隔符)
		{

		}
		else if ((pObj->dwStyle_ & 条目风格_子菜单) == 条目风格_子菜单)
		{

		}
		else
		{
			wnd_s* pWnd = pObj->pWnd_;
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STATE | MIIM_ID;
			if (GetMenuItemInfoW((HMENU)pWnd->hMenuPopup_, pObj->lParam_, true, &mii))
			{
				if ((mii.fState & MFS_GRAYED) == 0)
				{
					PostMessageW(pWnd->pMenuHostWnd_->hWnd_, WM_COMMAND, mii.wID, 0);
					ShowWindow(hWnd, 0);
					EndMenu();
				}
			}
		}
	}
}

void _item_draw(obj_s* pObj, paintstruct_s ps, int crColor, void* lpText)
{
	wnd_s* pWnd = pObj->pWnd_;
	HMENU hMenu = (HMENU)pWnd->hMenuPopup_;
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_FTYPE | MIIM_DATA | MIIM_SUBMENU;
	size_t nID = pObj->lParam_;
	RECT rcItem;
	RECT rcPadding;
	RECT rcSub;
	rcItem.right = ps.width_;
	rcItem.bottom = ps.height_;
	int atomProp = 0;
	if (GetMenuItemInfoW(hMenu, nID, true, &mii))
	{
		if ((mii.fType & MFT_SEPARATOR) != 0)
		{
			void* lpPadding = pWnd->padding_separator_;
			int tmp=0;
			if (lpPadding != 0)
			{
				RtlMoveMemory(&rcPadding, lpPadding, 16);
				tmp=Ex_Scale(HIWORD(HIWORD(pWnd->szItemSeparator_)) - (rcPadding.top + rcPadding.bottom));
			}
			rcItem.left = Ex_Scale(rcPadding.left);
			rcItem.top = (rcItem.bottom - tmp) / 2;
			rcItem.right = rcItem.right - Ex_Scale(rcPadding.right);
			rcItem.bottom = rcItem.top + tmp;
			Ex_ThemeDrawControlEx(ps.hTheme_, ps.hCanvas_, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, ATOM_ITEM, ATOM_SEPARATOR, 0, 0, 0, 0, 255);
		}
		else {
			bool fHover = (ps.dwState_ & 状态_点燃) != 0 && (mii.fState & MFS_GRAYED) == 0 && (mii.fState & MFS_HILITE) != 0 && mii.hSubMenu != 0;
			int alpha = 255;
			if (fHover)
			{
				pObj->dwState_ = pObj->dwState_ | 状态_点燃;
			}
			if ((mii.fState & MFS_GRAYED) == 0)
			{
				if (fHover)
				{
					Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, ATOM_ITEM, ATOM_SELECT, alpha);
				}
			}
			else {
				alpha = 128;
				int crColor = _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL);
				*(&crColor + 3) = alpha;
			}
			if (mii.hSubMenu != 0)
			{
				void* lpPadding = Ex_ThemeGetValuePtr(ps.hTheme_, ATOM_ITEM, ATOM_EXPAND);
				if (lpPadding != 0)
				{
					RtlMoveMemory(&rcPadding, lpPadding, 16);
				}
				rcPadding.right = rcPadding.right - rcPadding.left;
				rcPadding.bottom = rcPadding.bottom - rcPadding.top;
				rcSub.left = rcItem.right - Ex_Scale(rcPadding.right);
				rcSub.top = rcItem.top + (rcItem.bottom - rcItem.top - rcPadding.bottom) / 2;
				rcSub.right = rcSub.left + rcPadding.right;
				rcSub.bottom = rcSub.top + rcPadding.bottom;
				Ex_ThemeDrawControlEx(ps.hTheme_, ps.hCanvas_, rcSub.left, rcSub.top, rcSub.right, rcSub.bottom, ATOM_ITEM, ATOM_EXPAND, 0, 0, 0, 0, alpha);
			}
			if ((mii.fState & MFS_CHECKED) != 0)
			{
				atomProp = ATOM_CHECK;
			}
			else if ((mii.fState & MFT_RADIOCHECK) != 0)
			{
				atomProp = ATOM_RADIO;
			}
			void* lpPadding = Ex_ThemeGetValuePtr(ps.hTheme_, ATOM_ITEM, atomProp);
			if (lpPadding != 0)
			{
				RtlMoveMemory(&rcPadding, lpPadding, 16);
			}
			int tmp = pObj->t_left_;
			rcSub.left = rcItem.left + (tmp - (rcPadding.right - rcPadding.left)) / 2;
			rcSub.right = rcSub.left + rcPadding.right - rcPadding.left;
			rcSub.top = rcItem.top + (rcItem.bottom - rcItem.top - (rcPadding.bottom - rcPadding.top)) / 2;
			rcSub.bottom = rcSub.top + rcPadding.bottom - rcPadding.top;
			Ex_ThemeDrawControlEx(ps.hTheme_, ps.hCanvas_, rcSub.left, rcSub.top, rcSub.right, rcSub.bottom, ATOM_ITEM, atomProp, 0, 0, 0, 0, alpha);
			if (lpText != 0)
			{
				void* hFont = pObj->hFont_;
				void* lpLogfont = 0;
				if ((mii.fState & MFS_DEFAULT) != 0)
				{
					 lpLogfont = Ex_MemAlloc(sizeof(LOGFONT));
					if (_font_getlogfont(hFont, lpLogfont))
					{
						((LOGFONT*)lpLogfont)->lfWeight = 700;
						hFont = _font_createfromlogfont_ex(lpLogfont, EFF_DISABLEDPISCALE);
					}
					else {
						Ex_MemFree(lpLogfont);
					}
				}
				_canvas_drawtextex(ps.hCanvas_, hFont, crColor, (LPCWSTR)lpText, -1, ps.dwTextFormat_, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_, 0, 0, 0, 0);
				if (tmp != -1)
				{
					if (!fHover)
					{
						*(&crColor + 3) = 128;
					}
					_canvas_drawtextex(ps.hCanvas_, hFont, crColor, (LPCWSTR)lpText, -1, ps.dwTextFormat_ | DT_RIGHT, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_, 0, 0, 0, 0);
				}
				if ((mii.fState & MFS_DEFAULT) != 0)
				{
					if (lpLogfont != 0)
					{
						_font_destroy(hFont);
						Ex_MemFree(lpLogfont);
					}
				}
			}
		}
	}
}

void _item_paint(EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		int nIndex = COLOR_EX_TEXT_NORMAL;
		int atomProp;
		
		if ((ps.dwState_ & 状态_点燃) != 0)
		{
			atomProp = ATOM_HOVER;
			nIndex = COLOR_EX_TEXT_HOVER;
		}
		else {
			if ((ps.dwState_ & 状态_选中) != 0)
			{
				nIndex = COLOR_EX_TEXT_CHECKED;
			}
			if ((ps.dwState_ & 状态_选择) != 0)
			{
				atomProp = ATOM_SELECT;
			}
		}
		int crColor = _obj_getcolor(pObj, nIndex);
		void* lptext = pObj->pstrTitle_;
		if ((pObj->dwFlags_ & eof_bMenuItem) == eof_bMenuItem)
		{
			_item_draw(pObj, ps, crColor, lptext);
		}
		else
		{
			if (atomProp != 0)
			{
				if ((ps.dwStyleEx_ & EOS_EX_CUSTOMDRAW) == 0)
				{
					Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, ATOM_ITEM, atomProp, 255);
				}
			}
			if (lptext != 0)
			{
				_canvas_drawtextex(ps.hCanvas_, pObj->hFont_, crColor, (LPCWSTR)lptext, -1, ps.dwTextFormat_, ps.t_left_, ps.t_top_, ps.t_right_, ps.t_bottom_, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}

size_t _item_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_PAINT)
	{
		_item_paint(hObj, pObj);
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		_item_click(hWnd, pObj);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		if ((pObj->dwFlags_ & eof_bMenuItem) == eof_bMenuItem)
		{
			_obj_notify_brothers(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0, true, true);
		}
		_obj_setfocus(hWnd, pObj->pWnd_, hObj, pObj, true);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		if ((pObj->dwState_ & 状态_点燃) == 状态_点燃)
		{
			_obj_killfocus(hObj, pObj, true);
			_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, 0);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		_obj_setuistate(pObj, 状态_按下, false, 0, true, 0);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		_obj_setuistate(pObj, 状态_按下, true, 0, true, 0);
	}
	else if (uMsg == WM_SETFOCUS)
	{
		_obj_setuistate(pObj, 状态_点燃, false, 0, true, 0);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		_obj_setuistate(pObj, 状态_点燃 | 状态_按下, true, 0, true, 0);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
