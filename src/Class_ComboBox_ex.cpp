#include "Class_ComboBox_ex.h"

void _combobox_regsiter()
{
    Ex_ObjRegister(L"ComboBox", EOS_VISIBLE, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, 14 * sizeof(size_t), 0, 0, _combobox_proc);
}

size_t _combobox_size(HWND hWnd, obj_s *pObj, HEXOBJ hObj, INT width, INT height)
{
    _obj_setextralong(pObj, ECBL_LEFT, width - Ex_Scale(24));
    _obj_setextralong(pObj, ECBL_TOP, 0);
    _obj_setextralong(pObj, ECBL_RIGHT, width);
    _obj_setextralong(pObj, ECBL_BOTTOM, height);
    _obj_baseproc(hWnd, hObj, pObj, CB_SETDROPPEDWIDTH, width, 0);
    return _obj_baseproc(hWnd, hObj, pObj, CB_SETITEMHEIGHT, -1, height);
}

EX_COMBOX_ITEMLIST *_combobox_realloc(obj_s *pObj, INT need, INT nIndex, BOOL add)
{
    EX_COMBOX_ITEMLIST *items = (EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST);
    INT size = items->size;
    if (!add && size > need)
    {
        EX_COMBOX_ITEMLIST *newItems = (EX_COMBOX_ITEMLIST *)Ex_MemAlloc(need * sizeof(EX_COMBOX_ITEM) + sizeof(EX_COMBOX_ITEM));
        if (size >= need)
        {
            size = need;
        }

        if (nIndex > 0 && nIndex <= size)
        {
            RtlMoveMemory(newItems->items, items->items, (nIndex - 1) * sizeof(EX_COMBOX_ITEM));
            RtlMoveMemory(&newItems->items[nIndex - 1], &items->items[nIndex], (size - nIndex + 1) * sizeof(EX_COMBOX_ITEM));
        }
        else
        {
            size_t copySize = 0;
            LPVOID copyDst = 0;
            LPVOID copySrc = 0;
            if (nIndex < 0 && nIndex - -size >= 0)
            {
                RtlMoveMemory(newItems->items, items->items, (abs(nIndex) - 1) * sizeof(EX_COMBOX_ITEM));
                copyDst = &newItems->items[abs(nIndex) - 1];
                copySrc = &items->items[abs(nIndex)];
                copySize = (size - abs(nIndex) + 1) * sizeof(EX_COMBOX_ITEM);
            }
            else
            {
                copyDst = newItems->items;
                copySrc = items->items;
                copySize = size * sizeof(EX_COMBOX_ITEM);
            }
            RtlMoveMemory(copyDst, copySrc, copySize);
        }
        newItems->size = need;
        Ex_MemFree(items);
        _obj_setextralong(pObj, ECBL_ITEMLIST, (size_t)newItems);
        items = newItems;
    }
    else if ((add || size < need) && size != need)
    {
        EX_COMBOX_ITEMLIST *newItems = (EX_COMBOX_ITEMLIST *)Ex_MemAlloc(need * sizeof(EX_COMBOX_ITEM) + sizeof(EX_COMBOX_ITEM));
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
            LPVOID copyDst = 0;
            LPVOID copySrc = 0;
            if (nIndex < 0 && nIndex - -size >= 0)
            {
                RtlMoveMemory(newItems->items, items->items, (abs(nIndex) - 1) * sizeof(EX_COMBOX_ITEM));
                copyDst = &newItems->items[abs(nIndex) - 1];
                copySrc = &items->items[abs(nIndex)];
                copySize = (size - abs(nIndex) + 1) * sizeof(EX_COMBOX_ITEM);
            }
            else
            {
                copyDst = newItems->items;
                copySrc = items->items;
                copySize = size * sizeof(EX_COMBOX_ITEM);
            }
            RtlMoveMemory(copyDst, copySrc, copySize);
        }
        newItems->size = need;
        Ex_MemFree(items);
        _obj_setextralong(pObj, ECBL_ITEMLIST, (size_t)newItems);
        items = newItems;
    }
    return items;
}

INT _combobox_insertstring(obj_s *pObj, size_t nIndex, LPCWSTR lpTitle)
{
    INT size = _obj_getextralong(pObj, ECBL_ITEMCOUNT) + 1;
    if (nIndex <= 0 || nIndex > size)
    {
        return -1;
    }
    EX_COMBOX_ITEMLIST *items = _combobox_realloc(pObj, size, nIndex, TRUE);
    _obj_setextralong(pObj, ECBL_ITEMCOUNT, size);
    items->items[nIndex - 1].lpwzTitle = StrDupW(lpTitle);
    return nIndex;
}

INT _combobox_addstring(obj_s *pObj, LPCWSTR lpTitle)
{
    INT size = _obj_getextralong(pObj, ECBL_ITEMCOUNT);
    return _combobox_insertstring(pObj, size + 1, lpTitle);
}

LONG_PTR _combobox_getitemdata(obj_s *pObj, size_t nIndex)
{
    if (nIndex <= 0 || nIndex > _obj_getextralong(pObj, ECBL_ITEMCOUNT))
    {
        return -1;
    }
    return ((EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST))->items[nIndex - 1].data;
}

INT _combobox_setitemdata(obj_s *pObj, size_t nIndex, LONG_PTR data)
{
    if (nIndex <= 0 || nIndex > _obj_getextralong(pObj, ECBL_ITEMCOUNT))
    {
        return -1;
    }
    ((EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST))->items[nIndex - 1].data = data;
    return 0;
}

INT _combobox_initstorage(obj_s *pObj, size_t size)
{
    INT init = _obj_getextralong(pObj, ECBL_ITEMCOUNT) + size;
    _combobox_realloc(pObj, init, 0, TRUE);
    return init;
}

LPCWSTR _combobox_getitemtitle(obj_s *pObj, size_t nIndex)
{
    if (nIndex <= 0 || nIndex > _obj_getextralong(pObj, ECBL_ITEMCOUNT))
    {
        return 0;
    }
    return ((EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST))->items[nIndex - 1].lpwzTitle;
}

INT _combobox_getleftbuttontextlen(obj_s *pObj, size_t nIndex)
{
    LPCWSTR lpString = _combobox_getitemtitle(pObj, nIndex);
    if (lpString)
    {
        return lstrlenW(lpString);
    }
    return -1;
}

void _combobox_notify(HWND hWnd, obj_s *pObj, HEXOBJ hObj, EX_NMHDR *lParam)
{
    if (lParam)
    {
        HEXOBJ hObjEdit = _obj_getextralong(pObj, ECBL_HOBJEDIT);
        if (hObjEdit == lParam->hObjFrom)
        {
            if (lParam->nCode == NM_HOVER)
            {
                _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
            }
            else if (lParam->nCode == NM_LEAVE)
            {
                _obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, 0);
            }
            else if (lParam->nCode == NM_SETFOCUS)
            {
                _obj_setuistate(pObj, STATE_FOCUS, FALSE, 0, TRUE, 0);
            }
            else if (lParam->nCode == NM_KILLFOCUS)
            {
                if (lParam->wParam != hObj)
                {
                    _obj_setuistate(pObj, STATE_FOCUS, TRUE, 0, TRUE, 0);
                }
            }
            else if (lParam->nCode == WM_CUT)
            {
                _obj_dispatchnotify(hWnd, pObj, hObj, 0, WM_SIZE, 0, 0);
            }
        }
    }
}

INT _combobox_resetcontent(obj_s *pObj)
{
    INT len = _obj_getextralong(pObj, ECBL_ITEMCOUNT);
    EX_COMBOX_ITEMLIST *itemList = (EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST);
    for (INT i = 0; i < len; i++)
    {
        Ex_MemFree((LPVOID)itemList->items[i].lpwzTitle);
    }
    _obj_setextralong(pObj, ECBL_ITEMCOUNT, 0);
    _combobox_realloc(pObj, 0, 0, TRUE);
    return 0;
}

BOOL _combobox_uninit(obj_s *pObj)
{
    _combobox_resetcontent(pObj);
    return Ex_MemFree((LPVOID)_obj_getextralong(pObj, ECBL_ITEMLIST));
}

INT _combobox_setcursel(obj_s *pObj, size_t nIndex)
{
    INT size = _obj_getextralong(pObj, ECBL_ITEMCOUNT);
    if (nIndex <= 0 || nIndex > size)
    {
        return -1;
    }
    INT cur = _obj_setextralong(pObj, ECBL_CURRENTSELECTED, nIndex);
    LPCWSTR title = _combobox_getitemtitle(pObj, nIndex);
    _obj_baseproc(_obj_gethwnd(pObj), pObj->hObj_, pObj, WM_SETTEXT, 1, (size_t)title);
    _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, pObj->hObj_, 0, CBN_SELCHANGE, nIndex, pObj->hObj_);
    return cur;
}

void _combobox_init(obj_s *pObj, HEXOBJ hObj)
{
    if (FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
    {
        HEXLAYOUT hLayout = _layout_create(ELT_ABSOLUTE, hObj);
        HEXOBJ hObjEdit = Ex_ObjCreateEx(EOS_EX_COMPOSITED | EOS_EX_TABSTOP | EOS_EX_FOCUSABLE | EOS_EX_CUSTOMDRAW, (LPCWSTR)ATOM_EDIT, 0, -1, 0, 0, 80, 30, hObj, 0, -1, 0, 0, 0);
        _obj_setextralong(pObj, ECBL_HOBJEDIT, hObjEdit);
        _layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_LEFT, ELCP_ABSOLUTE_TYPE_PX, 2);
        _layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_TOP, ELCP_ABSOLUTE_TYPE_PX, 2);
        _layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_RIGHT, ELCP_ABSOLUTE_TYPE_PX, (size_t)Ex_Scale(24.0));
        _layout_absolute_setedge(hLayout, hObjEdit, ELCP_ABSOLUTE_BOTTOM, ELCP_ABSOLUTE_TYPE_PX, 2);
        _layout_update(hLayout);
    }
    _obj_setextralong(pObj, ECBL_MINVISIBLE, 10);
    _obj_setextralong(pObj, ECBL_ITEMLIST, (size_t)Ex_MemAlloc(sizeof(EX_COMBOX_ITEMLIST)));
}

void _combobox_wnd_customdraw(obj_s *pObj, WPARAM wParam, EX_CUSTOMDRAW *lParam)
{
    if (!_obj_dispatchnotify(_obj_gethwnd(pObj), pObj, pObj->hObj_, 0, NM_CUSTOMDRAW, wParam, (size_t)lParam))
    {
        INT colorAtom = 0;
        EXARGB fontColor = 0;
        if (FLAGS_CHECK(lParam->dwState, STATE_SELECT))
        {
            colorAtom = ATOM_SELECT;
            fontColor = _obj_getcolor(pObj, COLOR_EX_TEXT_SELECT);
        }
        else if (FLAGS_CHECK(lParam->dwState, STATE_HOVER))
        {
            colorAtom = ATOM_HOVER;
            fontColor = _obj_getcolor(pObj, COLOR_EX_TEXT_HOVER);
        }
        else
        {
            fontColor = _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL);
        }
        if (!fontColor)
        {
            fontColor = ExRGB(0, 0, 0);
        }

        if (colorAtom)
        {
            Ex_ThemeDrawControlEx(lParam->hTheme, lParam->hCanvas, lParam->rcPaint.left, lParam->rcPaint.top, lParam->rcPaint.right, lParam->rcPaint.bottom, ATOM_ITEM, colorAtom, 0, 0, ATOM_BACKGROUND_GRID, 0, 255);
        }
        RECT padding = {0};
        RECT *pRect = (RECT *)Ex_ThemeGetValuePtr(lParam->hTheme, -1741080004, ATOM_PADDING_TEXT);
        if (pRect)
        {
            RtlMoveMemory(&padding, pRect, sizeof(RECT));
        }

        _canvas_drawtext(
            lParam->hCanvas, pObj->hFont_, fontColor,
            ((EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST))->items[wParam - 1].lpwzTitle, -1, 0x24u,
            lParam->rcPaint.left + padding.left,
            lParam->rcPaint.top + padding.top,
            lParam->rcPaint.right - padding.right,
            lParam->rcPaint.bottom - padding.bottom);
    }
}

INT _combobox_findstring(obj_s *pObj, size_t startIndex, LPCWSTR lpTitle)
{
    INT ret = 0;
    EX_COMBOX_ITEMLIST *itemList = (EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST);
    for (size_t i = startIndex + 1; i <= _obj_getextralong(pObj, ECBL_ITEMCOUNT); ++i)
    {
        if (!lstrcmpW(itemList->items[i - 1].lpwzTitle, lpTitle))
        {
            ret = i;
            break;
        }
    }
    if (!ret)
    {
        for (INT i = 0; i <= startIndex; ++i)
        {
            if (!lstrcmpW(itemList->items[i - 1].lpwzTitle, lpTitle))
            {
                ret = i;
                break;
            }
        }
    }
    if (!ret)
    {
        ret = -1;
    }
    return ret;
}

size_t _combobox_getleftbuttontext(obj_s *pObj, size_t nIndex, LPVOID buffer)
{
    LPCWSTR lpString = _combobox_getitemtitle(pObj, nIndex);
    size_t len = 0;
    if (!lpString)
    {
        return -1;
    }

    if (buffer)
    {
        len = lstrlenW(lpString);
        RtlMoveMemory(buffer, lpString, lstrlenW(lpString) * 2);
    }
    return len;
}

INT _combobox_delstring(obj_s *pObj, size_t nIndex)
{
    INT len = _obj_getextralong(pObj, ECBL_ITEMCOUNT);
    EX_COMBOX_ITEMLIST *itemList = (EX_COMBOX_ITEMLIST *)_obj_getextralong(pObj, ECBL_ITEMLIST);
    if (nIndex <= 0 || nIndex > len)
    {
        return -1;
    }
    Ex_MemFree((LPVOID)itemList->items[nIndex - 1].lpwzTitle);
    _combobox_realloc(pObj, len - 1, nIndex, FALSE);
    _obj_setextralong(pObj, ECBL_ITEMCOUNT, len - 1);
    return len;
}

LRESULT CALLBACK _combobox_wnd_proc(HWND hWnd, HEXDUI hDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult)
{
    if (uMsg == WM_NOTIFY)
    {
        EX_NMHDR *msg = (EX_NMHDR *)lParam;
        if (msg->nCode == NM_CALCSIZE && hDUI == msg->hObjFrom)
        {
            __set_int((LPVOID)msg->lParam, 4, _obj_getextralong((obj_s *)Ex_ObjGetLong(hDUI, EOL_LPARAM), ECBL_ITEMHEIGHT));
            __set_int((LPVOID)msg->lParam, 8, 0);
            __set_int((LPVOID)msg->lParam, 12, 0);
            *lpResult = 1;
            return 1;
        }
        else if (msg->nCode == NM_CUSTOMDRAW)
        {
            _combobox_wnd_customdraw((obj_s *)Ex_ObjGetLong(hDUI, EOL_LPARAM), msg->wParam, (EX_CUSTOMDRAW *)msg->lParam);
            *lpResult = 1;
            return 1;
        }
        else if (msg->nCode == NM_CLICK && hDUI == msg->hObjFrom)
        {
            INT sel = Ex_ObjSendMessage(hDUI, LVM_GETSELECTIONMARK, 0, 0);
            if (sel)
            {
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
                _combobox_setcursel((obj_s *)Ex_ObjGetLong(hDUI, EOL_LPARAM), sel);
            }
        }
    }
    return 0;
}

void _combobox_btndown(HWND hWnd, HEXOBJ hObj, obj_s *pObj)
{
    wnd_s *pWnd = pObj->pWnd_;
    if (!FLAGS_CHECK(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN))
    {
        if (_obj_queryextra(pObj, ECBL_STATE, 2))
        {
            _obj_delextra(pObj, ECBL_STATE, 2);
        }
        else
        {
            if (!_obj_dispatchnotify(hWnd, pObj, hObj, 0, CBN_DROPDOWN, 0, 0))
            {
                RECT padding = {0};
                RECT unknown;
                LPVOID pThemePadding = Ex_ThemeGetValuePtr(pObj->hTheme_, 0x98393A3C, 0xC7D972B4);
                if (pThemePadding)
                {
                    RtlMoveMemory(&padding, pThemePadding, sizeof(RECT));
                }
                RtlMoveMemory(&unknown, &pObj->c_left_, sizeof(RECT));
                Ex_ObjClientToScreen(hObj, (INT *)&unknown.left, (INT *)&unknown.top);
                size_t count = _obj_getextralong(pObj, ECBL_ITEMCOUNT);
                size_t size = _obj_getextralong(pObj, ECBL_MINVISIBLE);

                if (count == 0)
                {
                    count = 5;
                }
                if (count > size)
                {
                    count = size;
                }
                RECT tmp;
                tmp.left = unknown.left;
                tmp.top = unknown.top + unknown.bottom;
                tmp.right = _obj_getextralong(pObj, ECBL_DROPPEDWIDTH);
                tmp.bottom = count * _obj_getextralong(pObj, ECBL_ITEMHEIGHT) + padding.top + padding.bottom;

                RECT screen;
                _wnd_getscreenrect(hWnd, &screen);
                if (tmp.top - unknown.bottom / 2 - (screen.bottom - screen.top) / 2 > 0)
                {
                    if (tmp.top + tmp.bottom - screen.bottom > 0)
                    {
                        tmp.top = unknown.top - tmp.bottom;
                        if (tmp.top < screen.top)
                        {
                            tmp.top = screen.top;
                            tmp.bottom = unknown.top;
                        }
                    }
                }
                else
                {
                    if (tmp.top + tmp.bottom - screen.bottom > 0)
                    {
                        tmp.bottom = screen.bottom - tmp.top;
                    }
                }

                HWND hWndBox = Ex_WndCreate(hWnd, NULL, NULL, tmp.left, tmp.top, tmp.right, tmp.bottom, WS_POPUP, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
                if (hWnd)
                {
                    HEXDUI hExBox = Ex_DUIBindWindowEx(hWndBox, 0, EWS_ESCEXIT | EWS_NOINHERITBKG | EWS_NOSHADOW | EWS_NOCAPTIONTOPMOST | EWS_POPUPWINDOW | EWS_COMBOWINDOW, hObj, NULL);
                    pWnd->lpPopupParams_ = hObj;
                    wnd_s *pWndBox;
                    INT nError = 0;
                    if (_handle_validate(hExBox, HT_DUI, (LPVOID *)&pWndBox, &nError))
                    {
                        _obj_setextralong(pObj, ECBL_BOXHWND, (size_t)hWndBox);
                        HEXOBJ hObjListView = Ex_ObjCreateEx(-1, (LPCWSTR)ATOM_LISTVIEW, 0, EOS_VISIBLE | EOS_VSCROLL | ELVS_ITEMTRACKING,
                                                             padding.left,
                                                             padding.top,
                                                             (FLOAT)tmp.right / g_Li.DpiX - padding.left - padding.right,
                                                             (FLOAT)tmp.bottom / g_Li.DpiY - padding.top - padding.bottom,
                                                             hExBox, 0, -1, (size_t)pObj, 0, _combobox_wnd_proc);
                      
                        Ex_ObjSendMessage(hObjListView, LVM_SETITEMCOUNT, _obj_getextralong(pObj, ECBL_ITEMCOUNT), 0);
                        _obj_setextralong(pObj, ECBL_HOBJLISTVIEW, hObjListView);
                        size_t current = _obj_getextralong(pObj, ECBL_CURRENTSELECTED);
                        if (current > 0)
                        {
                            Ex_ObjSendMessage(hObjListView, LVM_SETSELECTIONMARK, 0, current);
                            Ex_ObjSendMessage(hObjListView, LVM_ENSUREVISIBLE, 0, current);
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

void _combobox_paint(HEXOBJ hObj, obj_s *pObj)
{
    EX_PAINTSTRUCT ps{0};
    INT atomSrcRect;
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        if (FLAGS_CHECK(ps.dwState, STATE_FOCUS))
        {
            atomSrcRect = ATOM_FOCUS;
        }
        else if (FLAGS_CHECK(ps.dwState, STATE_HOVER))
        {
            atomSrcRect = ATOM_HOVER;
        }
        else
        {
            atomSrcRect = ATOM_NORMAL;
        }
        if (!FLAGS_CHECK(ps.dwStyleEx, EOS_EX_CUSTOMDRAW))
        {
            RECT tmp = {0};
            RECT *pRect;
            RECT tmp2 = {0};

            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, ATOM_COMBOBOX, atomSrcRect, 255);
            if (FLAGS_CHECK(pObj->pWnd_->dwFlags_, EWF_BPOPUPWINDOIWSHOWN))
            {
                atomSrcRect = ATOM_ARROW2_NORMAL;
            }
            else
            {
                atomSrcRect = ATOM_ARROW1_NORMAL;
            }
            pRect = (RECT *)Ex_ThemeGetValuePtr(ps.hTheme, ATOM_COMBOBOX, atomSrcRect);
            tmp.left = _obj_getextralong(pObj, ECBL_LEFT);
            tmp.top = _obj_getextralong(pObj, ECBL_TOP);
            tmp.right = _obj_getextralong(pObj, ECBL_RIGHT);
            tmp.bottom = _obj_getextralong(pObj, ECBL_BOTTOM);
            if (pRect)
            {
                RtlMoveMemory(&tmp2, pRect, sizeof(RECT));
                OffsetRect(&tmp2, -tmp2.left, -tmp2.top);
            }
            tmp.left = tmp.left + (tmp.right - tmp.left - tmp2.right) / 2;
            tmp.top = tmp.top + (tmp.bottom - tmp.top - tmp2.bottom) / 2;
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, tmp.left, tmp.top, tmp.left + tmp2.right, tmp.top + tmp2.bottom, -1916733255, atomSrcRect, 0, 0, 0, 0, 255);
        }
        if (!FLAGS_CHECK(ps.dwStyle, ECS_ALLOWEDIT))
        {
            _canvas_drawtextex(
                ps.hCanvas,
                pObj->hFont_,
                _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL),
                pObj->pstrTitle_,
                -1,
                ps.dwTextFormat,
                ps.rcText.left, ps.rcText.top, ps.rcText.right - Ex_Scale(24), ps.rcText.bottom, pObj->dwShadowSize_, _obj_getcolor(pObj, COLOR_EX_TEXT_SHADOW), 0);
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}

LRESULT CALLBACK _combobox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    INT nError = 0;
    obj_s *pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj, &nError))
    {
        if (uMsg == WM_CREATE)
        {
            _combobox_init(pObj, hObj);
        }
        else if (uMsg == WM_DESTROY)
        {
            _combobox_uninit(pObj);
        }
        else if (uMsg == WM_NCHITTEST)
        {
            POINT tmp;
            tmp.x = LOWORD(lParam);
            tmp.y = HIWORD(lParam);
            RECT rc;
            rc.left = _obj_getextralong(pObj, ECBL_LEFT);
            rc.top = _obj_getextralong(pObj, ECBL_TOP);
            rc.right = _obj_getextralong(pObj, ECBL_RIGHT);
            rc.bottom = _obj_getextralong(pObj, ECBL_BOTTOM);
            if (PtInRect(&rc, tmp))
            {
                _obj_addextra(pObj, ECBL_STATE, ECBF_HOVER);
            }
            else
            {
                _obj_delextra(pObj, ECBL_STATE, ECBF_HOVER);
            }
        }
        else if (uMsg == WM_SIZE)
        {
            _combobox_size(hWnd, pObj, hObj, LOWORD(lParam), HIWORD(lParam));
        }
        else if (uMsg == WM_PAINT)
        {
            _combobox_paint(hObj, pObj);
        }
        else if (uMsg == WM_MOUSEHOVER)
        {
            _obj_setuistate(pObj, STATE_HOVER, FALSE, NULL, TRUE, NULL);
        }
        else if (uMsg == WM_MOUSELEAVE)
        {
            _obj_setuistate(pObj, STATE_HOVER, TRUE, NULL, TRUE, NULL);
        }
        else if (uMsg == WM_SETFOCUS)
        {
            _obj_setuistate(pObj, STATE_FOCUS, FALSE, NULL, TRUE, NULL);
        }
        else if (uMsg == WM_KILLFOCUS)
        {
            _obj_setuistate(pObj, STATE_FOCUS, TRUE, NULL, TRUE, NULL);
        }
        else if (uMsg == WM_NOTIFY)
        {
            _combobox_notify(hWnd, pObj, hObj, (EX_NMHDR *)lParam);
        }
        else if (uMsg == WM_LBUTTONDOWN)
        {
            if (_obj_queryextra(pObj, ECBL_STATE, ECBF_HOVER) || !FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
            {
                _combobox_btndown(hWnd, hObj, pObj);
            }
        }
        else if (uMsg == WM_EX_INITPOPUP)
        {
            INT nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
        }
        else if (uMsg == WM_EX_EXITPOPUP)
        {
            if (wParam)
            {
                if (hObj == wParam)
                {
                    _obj_addextra(pObj, ECBL_STATE, 2);
                }
            }
            else
            {
                INT nError = 0;
                _obj_dispatchnotify(hWnd, pObj, hObj, 0, CBN_CLOSEUP, 0, 0);
                _obj_invalidaterect(pObj, 0, &nError);
            }
        }
        else if (uMsg == WM_SETTEXT || uMsg == WM_GETTEXT || uMsg == WM_GETTEXTLENGTH)
        {
            if (FLAGS_CHECK(pObj->dwStyle_, ECS_ALLOWEDIT))
            {
                return Ex_ObjDispatchMessage((HEXOBJ)_obj_getextralong(pObj, ECBL_HOBJEDIT), uMsg, wParam, lParam);
            }
        }
        else if (uMsg == CB_ADDSTRING)
        {
            return _combobox_addstring(pObj, (LPCWSTR)lParam);
        }
        else if (uMsg == CB_DELETESTRING)
        {
            return _combobox_delstring(pObj, wParam);
        }
        else if (uMsg == CB_FINDSTRING)
        {
            return _combobox_findstring(pObj, wParam, (LPCWSTR)lParam);
        }
        else if (uMsg == CB_GETCOUNT)
        {
            return _obj_getextralong(pObj, ECBL_ITEMCOUNT);
        }
        else if (uMsg == CB_GETCURSEL)
        {
            return _obj_getextralong(pObj, ECBL_CURRENTSELECTED);
        }
        else if (uMsg == CB_GETDROPPEDSTATE)
        {
            return FLAGS_CHECK(pObj->pWnd_->dwFlags_, EWF_BPOPUPWINDOIWSHOWN);
        }
        else if (uMsg == CB_GETDROPPEDWIDTH)
        {
            return _obj_getextralong(pObj, ECBL_DROPPEDWIDTH);
        }
        else if (uMsg == CB_SETDROPPEDWIDTH)
        {
            return _obj_setextralong(pObj, ECBL_DROPPEDWIDTH, wParam);
        }
        else if (uMsg == CB_GETEDITSEL)
        {
            Ex_ObjDispatchMessage((HEXOBJ)_obj_getextralong(pObj, ECBL_HOBJEDIT), 176, (size_t)&wParam, (size_t)&lParam);
            return MAKELONG(wParam, lParam);
        }
        else if (uMsg == CB_GETITEMDATA)
        {
            return _combobox_getitemdata(pObj, wParam);
        }
        else if (uMsg == CB_SETITEMDATA)
        {
            return _combobox_setitemdata(pObj, wParam, lParam);
        }
        else if (uMsg == CB_GETITEMHEIGHT)
        {
            return _obj_getextralong(pObj, ECBL_ITEMHEIGHT);
        }
        else if (uMsg == CB_SETITEMHEIGHT)
        {
            LONG_PTR ret = -1;
            if (wParam == -1)
            {
                ret = _obj_setextralong(pObj, ECBL_ITEMHEIGHT, lParam);
            }
            else
            {
                ret = -1;
            }
            return ret;
        }
        else if (uMsg == CB_GETMINVISIBLE)
        {
            return _obj_getextralong(pObj, ECBL_MINVISIBLE);
        }
        else if (uMsg == CB_SETMINVISIBLE)
        {
            return _obj_setextralong(pObj, ECBL_MINVISIBLE, wParam);
        }
        else if (uMsg == CB_INITSTORAGE)
        {
            return _combobox_initstorage(pObj, wParam);
        }
        else if (uMsg == CB_INSERTSTRING)
        {
            return _combobox_insertstring(pObj, wParam, (LPCWSTR)lParam);
        }
        else if (uMsg == CB_RESETCONTENT)
        {
            return _combobox_resetcontent(pObj);
        }
        else if (uMsg == CB_SELECTSTRING)
        {
            auto ret = _combobox_findstring(pObj, wParam, (LPCWSTR)lParam);
            if (ret)
            {
                Ex_ObjSendMessage((HEXOBJ)_obj_getextralong(pObj, ECBL_HOBJLISTVIEW), LVM_SETSELECTIONMARK, 0, lParam);
                _obj_setuistate(pObj, STATE_FOCUS, TRUE, NULL, TRUE, NULL);
               
            }
            return ret;
        }
        else if (uMsg == CB_SETCURSEL)
        {
            return _combobox_setcursel(pObj, wParam);
        }
        else if (uMsg == CB_SHOWDROPDOWN)
        {
            if (wParam)
            {
                _combobox_btndown(hWnd, hObj, pObj);
            }
            else
            {
                HWND hWndBox = (HWND)_obj_getextralong(pObj, ECBL_BOXHWND);
                if (hWndBox != 0)
                {
                    DestroyWindow(hWndBox);
                    _obj_setextralong(pObj, ECBL_BOXHWND, 0);
                }
            }
            INT nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
            return 1;
        }
        else if (uMsg == CB_GETLBTEXT)
        {
            return _combobox_getleftbuttontext(pObj, wParam, (LPVOID)lParam);
        }
        else if (uMsg == CB_GETLBTEXTLEN)
        {
            return _combobox_getleftbuttontextlen(pObj, wParam);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}