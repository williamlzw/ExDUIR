#include "Class_ComboBox_ex.h"

void _ComboBox_regsiter()
{
	Ex_ObjRegister(L"ComboBox", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 16 * sizeof(size_t), 0, 0, _cb_proc);
}

size_t _cb_size(HWND hWnd, obj_s* pObj, HEXOBJ hObj, int width, int height) {
	RECT* rect = (RECT*)_obj_getExtraPtr(pObj, 2);
	rect->left = width - Ex_Scale(24);
	rect->top = 0;
	rect->right = width;
	rect->bottom = height;
	_obj_baseproc(hWnd, hObj, pObj, CB_SETDROPPEDWIDTH, width, 0);
	return _obj_baseproc(hWnd, hObj, pObj, CB_SETITEMHEIGHT, -1, height);
}

EX_COMBOX_ITEMLIST* _cb_realloc(obj_s* pObj, int need, int nIndex, bool add) {
	EX_COMBOX_ITEMLIST* items = (EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10);
	int size = items->size;
	if ((add || size < need) && size != need) {
		EX_COMBOX_ITEMLIST* newItems = (EX_COMBOX_ITEMLIST*)Ex_MemAlloc(need * sizeof(EX_COMBOX_ITEM) + sizeof(EX_COMBOX_ITEM));
		if (size >= need)
		{
			size = need;
		}
		if (nIndex > 0 && nIndex <= size)
		{
			RtlMoveMemory(newItems->items, items->items, (nIndex - 1) * sizeof(EX_COMBOX_ITEM));
			RtlMoveMemory(&newItems->items[nIndex], &items->items[nIndex - 1], (size - nIndex + 1) * sizeof(EX_COMBOX_ITEM));
		}
		else
		{
			size_t copySize = 0;
			void* copyDst = 0;
			void* copySrc = 0;
			if (nIndex < 0 && nIndex - -size >= 0) {
				RtlMoveMemory(newItems->items, items->items, (abs(nIndex) - 1) * sizeof(EX_COMBOX_ITEM));
				copyDst = &newItems->items[abs(nIndex) - 1];
				copySrc = &items->items[abs(nIndex)];
				copySize = (size - abs(nIndex) + 1) * sizeof(EX_COMBOX_ITEM);
			}
			else {
				copyDst = newItems->items;
				copySrc = items->items;
				copySize = size * sizeof(EX_COMBOX_ITEM);
			}
			RtlMoveMemory(copyDst, copySrc, copySize);
		}
		newItems->size = need;
		Ex_MemFree(items);
		_obj_setExtraLong(pObj, 10, (size_t)newItems);
		items = newItems;
	}
	return items;
}

int _cb_insertstring(obj_s* pObj, size_t nIndex, LPCWSTR lpTitle) {
	int size = _obj_getExtraLong(pObj, 9) + 1;
	if (nIndex <= 0 || nIndex > size)
	{
		return -1;
	}
	EX_COMBOX_ITEMLIST* items = _cb_realloc(pObj, size, nIndex, FALSE);
	_obj_setExtraLong(pObj, 9, size);
	items->items[nIndex - 1].lpwzTitle = copytstr(lpTitle, lstrlenW(lpTitle));
	return nIndex;
}

int _cb_addstring(obj_s* pObj, LPCWSTR lpTitle) {
	int size = _obj_getExtraLong(pObj, 9);
	return _cb_insertstring(pObj, size + 1, lpTitle);
}

size_t _cb_getitemdata(obj_s* pObj, size_t nIndex) {
	if (nIndex <= 0 || nIndex > _obj_getExtraLong(pObj, 9))
	{
		return -1;
	}
	return ((EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10))->items[nIndex - 1].data;
}

int _cb_setitemdata(obj_s* pObj, size_t nIndex, size_t data) {
	if (nIndex <= 0 || nIndex > _obj_getExtraLong(pObj, 9))
	{
		return -1;
	}
	((EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10))->items[nIndex - 1].data = data;
	return 0;
}

int _cb_initstorage(obj_s* pObj, size_t size) {
	int init = _obj_getExtraLong(pObj, 9) + size;
	_cb_realloc(pObj, init, 0, TRUE);
	return init;
}

LPCWSTR _cb_getitemtitle(obj_s* pObj, size_t nIndex) {
	if (nIndex <= 0 || nIndex > _obj_getExtraLong(pObj, 9))
	{
		return 0;
	}
	return ((EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10))->items[nIndex - 1].lpwzTitle;
}

int _cb_getlbtextlen(obj_s* pObj, size_t nIndex) {
	LPCWSTR lpString = _cb_getitemtitle(pObj, nIndex);
	if (lpString)
	{
		return lstrlenW(lpString);
	}
	return -1;
}

void _cb_notify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, EX_NMHDR* lParam)
{
	if (lParam)
	{
		HEXOBJ hObjEdit = _obj_getExtraLong(pObj, 1);
		if (hObjEdit == lParam->hObjFrom)
		{
			switch (lParam->nCode)
			{
			case NM_HOVER:
				_obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
				break;
			case NM_LEAVE:
				_obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, 0);
				break;
			case NM_SETFOCUS:
				_obj_setuistate(pObj, STATE_FOCUS, FALSE, 0, TRUE, 0);
				break;
			case NM_KILLFOCUS:
				if (lParam->wParam != hObj)
					_obj_setuistate(pObj, STATE_FOCUS, TRUE, 0, TRUE, 0);
				break;
			case 768:
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, WM_SIZE, 0, 0);
				break;
			}
		}
	}
}


int _cb_resetcontent(obj_s* pObj) {
	int len = _obj_getExtraLong(pObj, 9);
	EX_COMBOX_ITEMLIST* itemList = (EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10);
	for (int i = 0; i < len; i++) {
		Ex_MemFree((void*)itemList->items[i].lpwzTitle);
	}
	_obj_setExtraLong(pObj, 9, 0);
	_cb_realloc(pObj, 0, 0, TRUE);
	return 0;
}

bool _cb_uninit(obj_s* pObj)
{
	_cb_resetcontent(pObj);
	return Ex_MemFree((void*)_obj_getExtraLong(pObj, 10));
}

int _cb_setcursel(obj_s* pObj, size_t nIndex)
{
	int size = _obj_getExtraLong(pObj, 9);
	if (nIndex <= 0 || nIndex > size)
	{
		return -1;
	}
	int cur = _obj_setExtraLong(pObj, 8, nIndex);
	LPCWSTR title = _cb_getitemtitle(pObj, nIndex);
	_obj_baseproc(_obj_gethWnd(pObj), pObj->hObj_, pObj, 12, 1, (size_t)title);
	_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, pObj->hObj_, 0, 1, 0, pObj->hObj_);
	return cur;
}

void _cb_init(obj_s* pObj, HEXOBJ hObj)
{
	if (FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
	{
		HEXLAYOUT hLayout = _layout_create(ELT_ABSOLUTE, hObj);
		HEXOBJ hObjEdit = Ex_ObjCreateEx(EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW, (LPCWSTR)ATOM_EDIT, 0, -1, 0, 0, 80, 30, hObj, 0, -1, 0, 0, 0);
		_obj_setExtraLong(pObj, 1, hObjEdit);
		_layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 2);
		_layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 2);
		_layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, (size_t)Ex_Scale(24.0));
		_layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 2);
		_layout_update(hLayout);
	}
	_obj_setExtraLong(pObj, 13, 10);
	_obj_setExtraLong(pObj, 10, (size_t)Ex_MemAlloc(sizeof(EX_COMBOX_ITEMLIST)));
}


void _cb_wnd_customdraw(obj_s* pObj, WPARAM wParam, EX_CUSTOMDRAW* lParam) {
	if (!_obj_dispatchnotify(_obj_gethWnd(pObj), pObj, pObj->hObj_, 0, -12, wParam, (size_t)lParam)) {
		int colorAtom = 0;
		EXARGB fontColor = 0;
		if (FLAGS_CHECK(lParam->dwState, 2)) {
			colorAtom = -1569548963;
			fontColor = _obj_getcolor(pObj, 7);
		}
		else if (FLAGS_CHECK(lParam->dwState, 128)) {
			colorAtom = -453284740;
			fontColor = _obj_getcolor(pObj, 3);
		}
		else {
			fontColor = _obj_getcolor(pObj, 2);
		}
		if (!fontColor) {
			fontColor = ExRGB(0, 0, 0);
		}

		if (colorAtom) {
			Ex_ThemeDrawControlEx(lParam->hTheme, lParam->hCanvas, lParam->rcPaint.left, lParam->rcPaint.top, lParam->rcPaint.right, lParam->rcPaint.bottom, -425838091, colorAtom, 0, 0, -1414696419, 0, 255);
		}
		RECT padding = { 0 };
		RECT* pRect = (RECT*)Ex_ThemeGetValuePtr(lParam->hTheme, -1741080004, ATOM_PADDING_TEXT);
		if (pRect) {
			RtlMoveMemory(&padding, pRect, sizeof(RECT));
		}

		_canvas_drawtext(
			lParam->hCanvas, pObj->hFont_, fontColor,
			((EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10))->items[wParam - 1].lpwzTitle
			, -1, 0x24u,
			lParam->rcPaint.left + padding.left,
			lParam->rcPaint.top + padding.top,
			lParam->rcPaint.right - padding.right,
			lParam->rcPaint.bottom - padding.bottom);
	}
}


int _cb_findstring(obj_s* pObj, size_t startIndex, LPCWSTR lpTitle) {
	int ret = 0;
	EX_COMBOX_ITEMLIST* itemList = (EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10);
	for (size_t i = startIndex + 1; i <= _obj_getExtraLong(pObj, 9); ++i) {
		if (!lstrcmpW(itemList->items[i - 1].lpwzTitle, lpTitle)) {
			ret = i;
			break;
		}
	}
	if (!ret) {
		for (int i = 0; i <= startIndex; ++i) {
			if (!lstrcmpW(itemList->items[i - 1].lpwzTitle, lpTitle)) {
				ret = i;
				break;
			}
		}
	}
	if (!ret) {
		ret = -1;
	}
	return ret;
}

size_t _cb_getlbtext(obj_s* pObj, size_t nIndex, void* buffer)
{
	LPCWSTR lpString = _cb_getitemtitle(pObj, nIndex);
	size_t len = 0;
	if (!lpString)
		return -1;
	if (buffer)
	{
		len = lstrlenW(lpString);
		RtlMoveMemory(buffer, lpString, lstrlenW(lpString) * 2);
	}
	return len;
}

int _cb_delstring(obj_s* pObj, size_t nIndex) {
	int len = _obj_getExtraLong(pObj, 9);
	EX_COMBOX_ITEMLIST* itemList = (EX_COMBOX_ITEMLIST*)_obj_getExtraLong(pObj, 10);
	if (nIndex <= 0 || nIndex > len)
		return -1;
	Ex_MemFree((void*)itemList->items[nIndex - 1].lpwzTitle);
	_cb_realloc(pObj, len - 1, 0, TRUE);
	_obj_setExtraLong(pObj, 9, --len);
	return len;
}

LRESULT CALLBACK _cb_wnd_proc(HWND hWnd, HEXDUI hDUI, INT uMsg, WPARAM wParam, LPARAM lParam, size_t* lpResult) {
	if (uMsg == WM_NOTIFY) {
		EX_NMHDR* msg = (EX_NMHDR*)lParam;
		if (msg->nCode == NM_CALCSIZE && hDUI == msg->hObjFrom) {
			__set_int((void*)msg->lParam, 4, _obj_getExtraLong((obj_s*)Ex_ObjGetLong(hDUI, -7), 12));
			__set_int((void*)msg->lParam, 8, 0);
			__set_int((void*)msg->lParam, 12, 0);
			*lpResult = 1;
			return 1;
		}
		else if (msg->nCode == NM_CUSTOMDRAW) {
			_cb_wnd_customdraw((obj_s*)Ex_ObjGetLong(hDUI, -7), msg->wParam, (EX_CUSTOMDRAW*)msg->lParam);
			*lpResult = 1;
			return 1;
		}
		else if (msg->nCode == NM_CLICK && hDUI == msg->hObjFrom) {
			int sel = Ex_ObjSendMessage(hDUI, 4162, 0, 0);
			if (sel) {
				PostMessageW(hWnd, WM_CLOSE, 0, 0);
				_cb_setcursel((obj_s*)Ex_ObjGetLong(hDUI, -7), sel);
			}
		}
	}
	return 0;
}

void _cb_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj) {
	wnd_s* pWnd = pObj->pWnd_;
	if (!FLAGS_CHECK(pWnd->dwFlags_, EWF_bPopupWindowShown)) {
		if (_obj_queryExtra(pObj, 0, 2)) {
			_obj_delExtra(pObj, 0, 2);
		}
		else {
			if (!_obj_dispatchnotify(hWnd, pObj, hObj, 0, CBN_DROPDOWN, 0, 0)) {
				RECT padding = { 0 };
				RECT unknown;

				void* pThemePadding = Ex_ThemeGetValuePtr(pObj->hTheme_, 0x98393A3C, 0xC7D972B4);
				if (pThemePadding) {
					RtlMoveMemory(&padding, pThemePadding, sizeof(RECT));
				}
				RtlMoveMemory(&unknown, &pObj->c_left_, sizeof(RECT));
				Ex_ObjClientToScreen(hObj, (int*)&unknown.left, (int*)&unknown.top);
				size_t count = _obj_getExtraLong(pObj, 9);
				size_t size = _obj_getExtraLong(pObj, 13);

				if (!count) {
					count = 5;
				}
				if (count > size) {
					count = size;
				}
				RECT tmp;
				tmp.left = unknown.left;
				tmp.top = unknown.top + unknown.bottom;
				tmp.right = _obj_getExtraLong(pObj, 11);
				tmp.bottom = count * _obj_getExtraLong(pObj, 12) + padding.top + padding.bottom;

				RECT screen;
				_wnd_getscreenrect(hWnd, &screen);
				if (tmp.top - unknown.bottom / 2 - (screen.bottom - screen.top) / 2 > 0) {
					if (tmp.top + tmp.bottom - screen.bottom > 0) {
						tmp.top = unknown.top - tmp.bottom;
						if (tmp.top < screen.top) {
							tmp.top = screen.top;
							tmp.bottom = unknown.top;
						}
					}
				}
				else {
					if (tmp.top + tmp.bottom - screen.bottom > 0) {
						tmp.bottom = screen.bottom - tmp.top;
					}
				}
				HWND hWndBox = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED, (LPCWSTR)GetClassLongW(hWnd, -32), 0, 0x80880000, tmp.left, tmp.top, tmp.right, tmp.bottom, hWnd, 0, g_Li.hInstance, 0);
				if (hWnd) {
					HEXDUI hExBox = Ex_DUIBindWindowEx(hWndBox, 0, EWS_ESCEXIT | EWS_NOINHERITBKG | EWS_NOSHADOW | EWS_NOCAPTIONTOPMOST | EWS_POPUPWINDOW | EWS_COMBOWINDOW, hObj, NULL);
					pWnd->lpPopupParams_ = hObj;
					wnd_s* pWndBox;
					if (_handle_validate(hExBox, HT_DUI, (void**)&pWndBox, NULL)) {
						_obj_setExtraLong(pObj, 6, (size_t)hWndBox);
						HEXOBJ hObjListView = Ex_ObjCreateEx(-1, (LPCWSTR)ATOM_LISTVIEW, 0, EOS_VISIBLE | EOS_VSCROLL | ELS_ITEMTRACKING,
							padding.left,
							padding.top,
							tmp.right / g_Li.DpiX - padding.left - padding.right,
							tmp.bottom / g_Li.DpiY - padding.top - padding.bottom,
							hExBox, 0, -1, (size_t)pObj, 0, _cb_wnd_proc);
						//TODO: Ex_ObjSendMessage(hObjListView, 48, _font_copy(pObj->hFont_), 0);
						Ex_ObjSendMessage(hObjListView, LVM_SETITEMCOUNT, _obj_getExtraLong(pObj, 9), 0);
						_obj_setExtraLong(pObj, 7, hObjListView);
						count = _obj_getExtraLong(pObj, 8);
						if (count > 0) {
							Ex_ObjSendMessage(hObjListView, LVM_SETSELECTIONMARK, 0, count);
							Ex_ObjSendMessage(hObjListView, LVM_ENSUREVISIBLE, 0, count);
						}
						Ex_ObjSetColor(hObjListView, COLOR_EX_TEXT_NORMAL, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), FALSE);
						Ex_ObjSetColor(hObjListView, COLOR_EX_TEXT_HOVER, _obj_getcolor(pObj, COLOR_EX_TEXT_HOVER), FALSE);
						Ex_ObjSetColor(hObjListView, COLOR_EX_TEXT_DOWN, _obj_getcolor(pObj, COLOR_EX_TEXT_DOWN), FALSE);
						Ex_ObjSetColor(hObjListView, COLOR_EX_TEXT_SELECT, _obj_getcolor(pObj, COLOR_EX_TEXT_SELECT), TRUE);
						_obj_dispatchnotify(hWnd, pObj, hObj, 0, CBN_POPUPLISTWINDOW, (size_t)hWndBox, hExBox);
						Ex_DUIShowWindow(hExBox, SW_SHOWNOACTIVATE, 0, 0, 0);
					}
				}
			}
		}
	}
}

int _cb_paint(HEXOBJ hObj, obj_s* pObj) {
	EX_PAINTSTRUCT2 ps;
	int atomSrcRect;
	if (Ex_ObjBeginPaint(hObj, &ps)) {
		if (FLAGS_CHECK(ps.dwState, STATE_FOCUS)) {
			atomSrcRect = ATOM_FOCUS;
		}
		else if (FLAGS_CHECK(ps.dwState, STATE_HOVER)) {
			atomSrcRect = ATOM_HOVER;
		}
		else {
			atomSrcRect = ATOM_NORMAL;
		}
		if (!FLAGS_CHECK(ps.dwStyleEx, EOS_EX_CUSTOMDRAW)) {
			RECT tmp = { 0 };
			RECT* pRect;
			RECT tmp2 = { 0 };

			Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.width, ps.height, ATOM_COMBOBOX, atomSrcRect, 255);
			if (FLAGS_CHECK(pObj->pWnd_->dwFlags_, EWF_bPopupWindowShown)) {
				atomSrcRect = ATOM_ARROW2_NORMAL;
			}
			else {
				atomSrcRect = ATOM_ARROW1_NORMAL;
			}
			pRect = (RECT*)Ex_ThemeGetValuePtr(ps.hTheme, ATOM_COMBOBOX, atomSrcRect);
			RtlMoveMemory(&tmp, _obj_getExtraPtr(pObj, 2), sizeof(RECT));
			if (pRect)
			{
				RtlMoveMemory(&tmp2, pRect, sizeof(RECT));
				OffsetRect(&tmp2, -tmp2.left, -tmp2.top);
			}
			tmp.left = tmp.left + (tmp.right - tmp.left - tmp2.right) / 2;
			tmp.top = tmp.top + (tmp.bottom - tmp.top - tmp2.bottom) / 2;
			Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, tmp.left, tmp.top, tmp.left + tmp2.right, tmp.top + tmp2.bottom, -1916733255, atomSrcRect, 0, 0, 0, 0, 255);
		}
		if (!FLAGS_CHECK(ps.dwStyle, ECS_ALLOWEDIT)) {
			_canvas_drawtextex(
				ps.hCanvas,
				pObj->hFont_,
				_obj_getcolor(pObj, 2),
				pObj->pstrTitle_,
				-1,
				ps.dwTextFormat,
				ps.t_left, ps.t_top, ps.t_right - Ex_Scale(24), ps.t_bottom, pObj->dwShadowSize_, _obj_getcolor(pObj, 10), 0, 0);
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}


LRESULT CALLBACK _cb_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		switch (uMsg)
		{
		case WM_CREATE:
			_cb_init(pObj, hObj);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_DESTROY:
			_cb_uninit(pObj);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_NCHITTEST:
			POINT tmp;
			tmp.x = LOWORD(lParam);
			tmp.y = HIWORD(lParam);
			if (PtInRect((RECT*)_obj_getExtraPtr(pObj, 2), tmp))
				_obj_addExtra(pObj, 0, 1);
			else
				_obj_delExtra(pObj, 0, 1);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_SIZE:
			_cb_size(hWnd, pObj, hObj, LOWORD(lParam), HIWORD(lParam));
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_PAINT:
			return _cb_paint(hObj, pObj);
		case WM_MOUSEHOVER:
			_obj_setuistate(pObj, STATE_HOVER, FALSE, NULL, TRUE, NULL);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_MOUSELEAVE:
			_obj_setuistate(pObj, STATE_HOVER, TRUE, NULL, TRUE, NULL);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_SETFOCUS:
			_obj_setuistate(pObj, STATE_FOCUS, FALSE, NULL, TRUE, NULL);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_KILLFOCUS:
			_obj_setuistate(pObj, STATE_FOCUS, TRUE, NULL, TRUE, NULL);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_NOTIFY:
			_cb_notify(hWnd, pObj, hObj, (EX_NMHDR*)lParam);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_LBUTTONDOWN:
			if (_obj_queryExtra(pObj, 0, 1) || !FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
				_cb_btndown(hWnd, hObj, pObj);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case 4294967290:
			_obj_invalidaterect(pObj, 0, 0);
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case 4294967289:
			if (wParam)
			{
				if (hObj == wParam)
					_obj_addExtra(pObj, 0, 2);
			}
			else
			{
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, 8, 0, 0);
				_obj_invalidaterect(pObj, 0, 0);
			}
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case WM_SETTEXT:
		case WM_GETTEXT:
		case WM_GETTEXTLENGTH:
			if (FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
			{
				return Ex_ObjDispatchMessage((HEXOBJ)_obj_getExtraLong(pObj, 1), uMsg, wParam, lParam);
			}
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		case CB_ADDSTRING:
			return _cb_addstring(pObj, (LPCWSTR)lParam);
		case CB_DELETESTRING:
			return _cb_delstring(pObj, wParam);
		case CB_FINDSTRING:
			return _cb_findstring(pObj, wParam, (LPCWSTR)lParam);
		case CB_GETCOUNT:
			return _obj_getExtraLong(pObj, 9);
		case CB_GETCURSEL:
			return _obj_getExtraLong(pObj, 8);
		case CB_GETDROPPEDSTATE:
			return FLAGS_CHECK(pObj->pWnd_->dwFlags_, EWF_bPopupWindowShown);
		case CB_GETDROPPEDWIDTH:
			return _obj_getExtraLong(pObj, 11);
		case CB_SETDROPPEDWIDTH:
			return _obj_setExtraLong(pObj, 11, wParam);
		case CB_GETEDITSEL:
			Ex_ObjDispatchMessage((HEXOBJ)_obj_getExtraLong(pObj, 1), 176, (size_t)&wParam, (size_t)&lParam);
			return MAKELONG(wParam, lParam);
		case CB_GETITEMDATA:
			return _cb_getitemdata(pObj, wParam);
		case CB_SETITEMDATA:
			return _cb_setitemdata(pObj, wParam, lParam);
		case CB_GETITEMHEIGHT:
			return _obj_getExtraLong(pObj, 12);
		case CB_SETITEMHEIGHT:
			if (wParam == -1)
				result = _obj_setExtraLong(pObj, 12, lParam);
			else
				result = -1;
			break;
		case CB_GETMINVISIBLE:
			return _obj_getExtraLong(pObj, 13);
		case CB_SETMINVISIBLE:
			return _obj_setExtraLong(pObj, 13, wParam);
		case CB_INITSTORAGE:
			return _cb_initstorage(pObj, wParam);
		case CB_INSERTSTRING:
			return _cb_insertstring(pObj, wParam, (LPCWSTR)lParam);
		case CB_LIMITTEXT:
			return _obj_setExtraLong(pObj, 14, wParam);
		case CB_RESETCONTENT:
			return _cb_resetcontent(pObj);
		case CB_SELECTSTRING:
			lParam = _cb_findstring(pObj, wParam, (LPCWSTR)lParam);
			if (lParam)
			{
				Ex_ObjSendMessage((HEXOBJ)_obj_getExtraLong(pObj, 7), LVM_SETSELECTIONMARK, 0, lParam);
			}
			result = lParam;
			break;
		case CB_SETCURSEL:
			return _cb_setcursel(pObj, wParam);
		case CB_SHOWDROPDOWN:
			if (wParam)
			{
				_cb_btndown(hWnd, hObj, pObj);
			}
			else
			{
				DestroyWindow((HWND)_obj_getExtraLong(pObj, 6));
			}
			_obj_invalidaterect(pObj, 0, 0);
			result = 1;
			break;
		case CB_GETLBTEXT:
			return _cb_getlbtext(pObj, wParam, (void*)lParam);
		case CB_GETLBTEXTLEN:
			return _cb_getlbtextlen(pObj, wParam);
		default:
			return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
		}
	}
	return result;
}

