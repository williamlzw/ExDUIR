#include "Class_Button_Item_ex.h"

void _button_regsiter()
{
	Ex_ObjRegister(L"Button", EOS_VISIBLE | EBS_TEXTOFFSET, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_CENTER | DT_SINGLELINE, 0, 0, 0, _button_proc);
	Ex_ObjRegister(L"CheckButton", EOS_VISIBLE | EBS_CHECKBUTTON, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, 0, _button_proc);
	Ex_ObjRegister(L"RadioButton", EOS_VISIBLE | EBS_RADIOBUTTON, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, 0, _button_proc);
}

void _item_regsiter()
{
	Ex_ObjRegister(L"Item", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 0, 0, 0, _item_proc);
}

LRESULT CALLBACK _button_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (uMsg == WM_PAINT)
		{
			_button_paint(hObj, pObj);
		}
		else if (uMsg == WM_EX_LCLICK || uMsg == BM_CLICK)
		{
			if ((pObj->dwFlags_ & EOF_BMSGBOXCONTROL) == EOF_BMSGBOXCONTROL)
			{
				return EndDialog(hWnd, pObj->lParam_);
			}
			else
			{
				if (uMsg == BM_CLICK)
				{
					_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CLICK, wParam, lParam);
				}
				if ((pObj->dwStyle_ & (EBS_CHECKBUTTON | EBS_RADIOBUTTON)) != 0)
				{
					if ((pObj->dwStyle_ & EBS_CHECKBUTTON) == EBS_CHECKBUTTON)
					{
						_obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, !((pObj->dwState_ & STATE_CHECKED) == STATE_CHECKED), 0);
					}
					else {
						if (!((pObj->dwState_ & STATE_CHECKED) == STATE_CHECKED))
						{
							_obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, 1, 0);
						}
					}
				}
			}
		}
		else if (uMsg == WM_MOUSEHOVER)
		{
			INT nError = 0;
			_obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, &nError);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			INT nError = 0;
			_obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, &nError);
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			INT nError = 0;
			_obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, &nError);
		}
		else if (uMsg == WM_LBUTTONUP)
		{
			INT nError = 0;
			_obj_setuistate(pObj, STATE_DOWN, TRUE, 0, TRUE, &nError);
		}
		else if (uMsg == WM_SETFOCUS)
		{
			INT nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
		else if (uMsg == WM_KILLFOCUS)
		{
			INT nError = 0;
			_obj_invalidaterect(pObj, 0, &nError);
		}
		else if (uMsg == BM_SETCHECK)
		{
			if ((pObj->dwStyle_ & (EBS_RADIOBUTTON | EBS_CHECKBUTTON)) != 0)
			{
				if ((pObj->dwStyle_ & EBS_RADIOBUTTON) == EBS_RADIOBUTTON)
				{
					if (wParam == 0)
					{
						if (!((pObj->dwState_ & STATE_CHECKED) == STATE_CHECKED))
						{
							return 0;
						}
					}
					else
					{
						_obj_notify_brothers(hWnd, hObj, pObj, uMsg, 0, 0, TRUE, TRUE);
					}
				}
				else
				{
					if ((pObj->dwStyle_ & EBS_CHECKBUTTON) == EBS_CHECKBUTTON)
					{
						_obj_setuistate(pObj, STATE_HALFSELECT | STATE_SELECT, TRUE, 0, FALSE, 0);
						if (wParam == 2)
						{
							_obj_setuistate(pObj, STATE_HALFSELECT, FALSE, 0, TRUE, 0);
							return 0;
						}
					}
				}
				_obj_setuistate(pObj, STATE_CHECKED, wParam == 0, 0, TRUE, 0);
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CHECK, wParam, lParam);
			}
		}
		else if (uMsg == BM_GETCHECK)
		{
			if ((pObj->dwState_ & STATE_HALFSELECT) == STATE_HALFSELECT)
			{
				return 2;
			}
			else if ((pObj->dwState_ & STATE_CHECKED) == STATE_CHECKED)
			{
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _button_paint(HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT ps = { 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		BOOL fButton = (ps.dwStyle & (EBS_RADIOBUTTON | EBS_CHECKBUTTON)) == 0;
		EXATOM atomClass;
		EXATOM atomProp;
		INT nIndex;
		if (fButton)
		{
			atomClass = ATOM_BUTTON;
			if ((ps.dwState & STATE_DOWN) != 0)
			{
				atomProp = ATOM_DOWN;
				nIndex = COLOR_EX_TEXT_DOWN;
			}
			else if ((ps.dwState & STATE_HOVER) != 0)
			{
				atomProp = ATOM_HOVER;
				nIndex = COLOR_EX_TEXT_HOVER;
			}
			else {
				atomProp = ATOM_NORMAL;
				nIndex = COLOR_EX_TEXT_NORMAL;
			}

			if ((ps.dwStyleEx & EOS_EX_CUSTOMDRAW) == 0)
			{
				Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass, atomProp, 255);
				if ((ps.dwState & STATE_FOCUS) != 0)
				{
					Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass, ATOM_FOCUS, 255);
				}
			}
		}
		else
		{
			fButton = (ps.dwStyle & EBS_RADIOBUTTON) != 0;
			atomClass = fButton ? ATOM_RADIOBUTTON : ATOM_CHECKBUTTON;
			if ((ps.dwState & STATE_DOWN) != 0)
			{
				atomProp = _button_getprop(ps.dwState, fButton, ATOM_DOWN, ATOM_CHECK_DOWN, ATOM_HALF_DOWN);
				nIndex = COLOR_EX_TEXT_DOWN;
			}
			else if ((ps.dwState & STATE_HOVER) != 0)
			{
				atomProp = _button_getprop(ps.dwState, fButton, ATOM_HOVER, ATOM_CHECK_HOVER, ATOM_HALF_HOVER);
				nIndex = COLOR_EX_TEXT_HOVER;
			}
			else if ((ps.dwState & STATE_FOCUS) != 0)
			{
				atomProp = _button_getprop(ps.dwState, fButton, ATOM_FOCUS, ATOM_CHECK_FOCUS, ATOM_HALF_FOCUS);
				nIndex = COLOR_EX_TEXT_NORMAL;
			}
			else
			{
				atomProp = _button_getprop(ps.dwState, fButton, ATOM_NORMAL, ATOM_CHECK_NORMAL, ATOM_HALF_NORMAL);
				nIndex = COLOR_EX_TEXT_NORMAL;
			}

			if ((ps.dwState & STATE_CHECKED) != 0)
			{
				nIndex = COLOR_EX_TEXT_CHECKED;
			}

			if ((ps.dwStyleEx & EOS_EX_CUSTOMDRAW) == 0)
			{
				Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass, atomProp, 255);
			}
		}
		LPCWSTR lptext = pObj->pstrTitle_;
		if (lptext != 0)
		{
			if ((ps.dwState & STATE_DOWN) != 0 && (ps.dwStyle & EBS_TEXTOFFSET) != 0)
			{
				OffsetRect((LPRECT)&ps.rcText.left, Ex_Scale(1), Ex_Scale(1));
			}
			_canvas_drawtextex(ps.hCanvas, pObj->hFont_, _obj_getcolor(pObj, nIndex), lptext, -1, ps.dwTextFormat, ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}

INT _button_getprop(INT state, BOOL fRadio, EXATOM atom_src, EXATOM atom_check, EXATOM atom_half)
{
	EXATOM atomProp;
	if (fRadio)
	{
		atomProp = (state & STATE_CHECKED) != 0 ? atom_check : atom_src;
	}
	else
	{
		if ((state & STATE_HALFSELECT) != 0)
		{
			atomProp = atom_half;
		}
		else if ((state & STATE_CHECKED) != 0)
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
	if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
	{
		if ((pObj->dwStyle_ & EMIS_SEPARATOR) == EMIS_SEPARATOR)
		{

		}
		else if ((pObj->dwStyle_ & EMIS_SUBMENU) == EMIS_SUBMENU)
		{

		}
		else
		{
			wnd_s* pWnd = pObj->pWnd_;
			MENUITEMINFOW mii;
			mii.cbSize = sizeof(MENUITEMINFOW);
			mii.fMask = MIIM_STATE | MIIM_ID;
			if (GetMenuItemInfoW((HMENU)pWnd->hMenuPopup_, pObj->lParam_, TRUE, &mii))
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

void _item_draw(obj_s* pObj, EX_PAINTSTRUCT ps, EXARGB crColor, LPCWSTR lpText)
{
	wnd_s* pWnd = pObj->pWnd_;
	HMENU hMenu = (HMENU)pWnd->hMenuPopup_;
	MENUITEMINFOW mii;
	mii.cbSize = sizeof(MENUITEMINFOW);
	mii.fMask = MIIM_STATE | MIIM_FTYPE | MIIM_DATA | MIIM_SUBMENU | MIIM_BITMAP;
	size_t nID = pObj->lParam_;
	RECT rcItem  { 0 };
	RECT rcPadding  { 0 };
	RECT rcSub  { 0 };
	rcItem.right = ps.uWidth;
	rcItem.bottom = ps.uHeight;
	INT atomProp = 0;
	
	if (GetMenuItemInfoW(hMenu, nID, TRUE, &mii))
	{
		if ((mii.fType & MFT_SEPARATOR) != 0)
		{
			LPVOID lpPadding = pWnd->padding_separator_;
			INT tmp=0;
			if (lpPadding != 0)
			{
				RtlMoveMemory(&rcPadding, lpPadding, 16);
				tmp = Ex_Scale(HIBYTE(HIWORD(pWnd->szItemSeparator_)) - (rcPadding.top + rcPadding.bottom));
			}
			rcItem.left = Ex_Scale(rcPadding.left);
			rcItem.top = (rcItem.bottom - tmp) / 2;
			rcItem.right = rcItem.right - Ex_Scale(rcPadding.right);
			rcItem.bottom = rcItem.top + tmp;
			Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, ATOM_ITEM, ATOM_SEPARATOR, 0, 0, 0, 0, 255);
		}
		else {
			BOOL fHover = ((ps.dwState & STATE_HOVER) != 0 && (mii.fState & MFS_GRAYED) == 0) || ((mii.fState & MFS_HILITE) != 0 && mii.hSubMenu != 0);
			CHAR alpha = 255;
			if (fHover)
			{
				pObj->dwState_ = pObj->dwState_ | STATE_HOVER;
			}
			if ((mii.fState & MFS_GRAYED) == 0)
			{
				if (fHover)
				{
					Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, ATOM_ITEM, ATOM_SELECT, alpha);
				}
			}
			else {
				alpha = 128;
				crColor = _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL);
				*(((CHAR*)&crColor) + 3) = alpha;
			}
			if (mii.hSubMenu != 0)
			{
				LPVOID lpPadding = Ex_ThemeGetValuePtr(ps.hTheme, ATOM_ITEM, ATOM_EXPAND);
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
				Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSub.left, rcSub.top, rcSub.right, rcSub.bottom, ATOM_ITEM, ATOM_EXPAND, 0, 0, 0, 0, alpha);
			}
			if ((mii.fState & MFS_CHECKED) != 0)
			{
				atomProp = ATOM_CHECK;
			}
			else if ((mii.fState & MFT_RADIOCHECK) != 0)
			{
				atomProp = ATOM_RADIO;
			}
			LPVOID lpPadding = Ex_ThemeGetValuePtr(ps.hTheme, ATOM_ITEM, atomProp);
			if (lpPadding != 0)
			{
				RtlMoveMemory(&rcPadding, lpPadding, 16);
			}
			INT tmp = pObj->t_left_;
			rcSub.left = rcItem.left + (tmp - (rcPadding.right - rcPadding.left)) / 2;
			rcSub.right = rcSub.left + rcPadding.right - rcPadding.left;
			rcSub.top = rcItem.top + (rcItem.bottom - rcItem.top - (rcPadding.bottom - rcPadding.top)) / 2;
			rcSub.bottom = rcSub.top + rcPadding.bottom - rcPadding.top;
			Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSub.left, rcSub.top, rcSub.right, rcSub.bottom, ATOM_ITEM, atomProp, 0, 0, 0, 0, alpha);
			if (lpText != 0)
			{
				HEXFONT hFont = pObj->hFont_;
				if ((mii.fState & MFS_DEFAULT) != 0)
				{
					LOGFONTW Logfont{ 0 };
					if (_font_getlogfont(hFont, &Logfont))
					{
						Logfont.lfWeight = 700;
						hFont = _font_createfromlogfont_ex(&Logfont, EFF_DISABLEDPISCALE);
					}
				}
				if (mii.hbmpItem)
				{
					HEXIMAGE img = 0;
					_img_createfromhbitmap(mii.hbmpItem, 0, TRUE, &img);
					if (img != 0)
					{
						INT imgWidth, imgHeight;
						_img_getsize(img, &imgWidth, &imgHeight);
						_canvas_drawimagerectrect(ps.hCanvas, img, 5, 3, ps.rcPaint.left + 5 + ps.rcPaint.bottom - 6, ps.rcPaint.bottom - 3, 0, 0, imgWidth, imgHeight, 255);
						_img_destroy(img);
					}
				}
				LPWSTR tmp1 = (LPWSTR)wcschr(lpText, 9);
				if (tmp1) {
					*tmp1 = 0;
				}
				_canvas_drawtextex(ps.hCanvas, hFont, crColor, lpText, -1, ps.dwTextFormat, ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom, 0, 0, 0, 0);
				if (tmp1)
				{
					*tmp1 = 9;
					if (!fHover)
					{
						*(((CHAR*)&crColor) + 3) = (CHAR)128;
					}
					_canvas_drawtextex(ps.hCanvas, hFont, crColor, (LPCWSTR)(tmp1 + 1), -1, ps.dwTextFormat | DT_RIGHT, ps.rcText.left , ps.rcText.top, ps.rcText.right, ps.rcText.bottom, 0, 0, 0, 0);
				}
				if ((mii.fState & MFS_DEFAULT) != 0)
				{
					_font_destroy(hFont);
				}
			}
		}
	}
}

void _item_paint(HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		INT nIndex = COLOR_EX_TEXT_NORMAL;
		INT atomProp = 0;
		
		if ((ps.dwState & STATE_HOVER) != 0)
		{
			atomProp = ATOM_HOVER;
			nIndex = COLOR_EX_TEXT_HOVER;
		}
		else {
			if ((ps.dwState & STATE_CHECKED) != 0)
			{
				nIndex = COLOR_EX_TEXT_CHECKED;
				atomProp = ATOM_CHECK;
			}
			if ((ps.dwState & STATE_SELECT) != 0)
			{
				atomProp = ATOM_SELECT;
				nIndex = COLOR_EX_TEXT_SELECT;
			}
		}
		EXARGB crColor = _obj_getcolor(pObj, nIndex);
		LPCWSTR lptext = pObj->pstrTitle_;
		if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
		{
			_item_draw(pObj, ps, crColor, lptext);
		}
		else
		{
			if (atomProp != 0)
			{
				if ((ps.dwStyleEx & EOS_EX_CUSTOMDRAW) == 0)
				{
					Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, ATOM_ITEM, atomProp, 255);
				}
			}
			if (lptext != 0)
			{
				_canvas_drawtextex(ps.hCanvas, pObj->hFont_, crColor, lptext, -1, ps.dwTextFormat, ps.rcText.left , ps.rcText.top, ps.rcText.right, ps.rcText.bottom, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0, 0);
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}

LRESULT CALLBACK _item_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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
			if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
			{
				_obj_notify_brothers(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0, TRUE, TRUE);
			}
			_obj_setfocus(hWnd, pObj->pWnd_, hObj, pObj, TRUE);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			if ((pObj->dwState_ & STATE_HOVER) == STATE_HOVER)
			{
				_obj_killfocus(hObj, pObj, TRUE);
				_obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
			}
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			_obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, 0);
		}
		else if (uMsg == WM_LBUTTONUP)
		{
			_obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, 0);
		}
		else if (uMsg == WM_SETFOCUS)
		{
			_obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
		}
		else if (uMsg == WM_KILLFOCUS)
		{
			_obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
		}
		else if (uMsg == WM_TIMER)
		{

		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
