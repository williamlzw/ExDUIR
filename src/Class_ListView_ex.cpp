#include "stdafx.h"

ClsPROC m_pfnTListView;

void _listview_regsiter()
{
    Ex_ObjRegister(L"ListView", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
        DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, 0, 0, _listview_proc);
    EX_CLASSINFO clsInfo{ 0 };
    Ex_ObjGetClassInfoEx(L"ListView", &clsInfo);
    m_pfnTListView = clsInfo.pfnClsProc;
    _obj_register(Ex_Atom(L"TListView"), clsInfo.dwStyle, clsInfo.dwStyleEx, clsInfo.dwTextFormat,
        8 * sizeof(size_t), clsInfo.hCursor, _tlv_proc, clsInfo.dwFlags, 0);
    _obj_register(Ex_Atom(L"LISTITEM"), OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
        DT_VCENTER | DT_SINGLELINE | DT_CENTER, 10 * sizeof(size_t), clsInfo.hCursor,
        _tlvi_proc, clsInfo.dwFlags, 0);
}

void _tlv_array_del(array_s* hArray, int index, HEXOBJ pvData, int type)
{
    if (type == 0 && hArray && pvData) {
        Ex_ObjSendMessage(Array_GetExtra(hArray), TEMPLATELISTVIEW_MESSAGE_ITEM_DESTROY, 0, pvData);
    }
}

void _tlv_refill(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, LONG_PTR iStart,
    LONG_PTR iStartOld, LONG_PTR iEnd, LONG_PTR iEndOld, BOOL NeedUpdate, BOOL update)
{
    if (!pOwner || !pObj || !hObj) return;
    LPVOID   lpItems = pOwner->lpItems_;
    int      nIndex = 0;
    array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEMARRAY);
    int      nItemCount = pOwner->count_items_;
    int      nCount = iEnd - iStart + 1;
    int      nCountOld = iEndOld - iStartOld + 1;
    int      oldnum = arr ? Array_GetCount(arr) : 0;
    // fix: 校验nCount合法性，防止创建负数个item
    if (nCount > oldnum && nCount > 0 && arr) {
        for (int i = 1; i <= (nCount - oldnum); i++) {
            HEXOBJ hNewItem = _obj_baseproc(hWnd, hObj, pObj, TEMPLATELISTVIEW_MESSAGE_ITEM_CREATE, 0, 0);
            if (hNewItem) Array_AddMember(arr, hNewItem);
        }
    }
    // fix: 缓存颜色值，减少循环内函数调用，提升性能
    DWORD dwHoverColor = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_HOVERCOLOR);
    DWORD dwSelectColor = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_SELECTCOLOR);
    DWORD dwBorderColor = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_BORDERCOLOR);
    DWORD dwSelectHoverColor = ExRGB2ARGB(ExARGB2RGB(dwSelectColor), 150);
    BOOL bItemTracking = ((pObj->dwStyle_ & LISTVIEW_STYLE_ITEMTRACKING) == LISTVIEW_STYLE_ITEMTRACKING);

    INT iSelect = pOwner->index_select_;
    Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_INDEX_START, (LONG_PTR)iStart);
    Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_INDEX_END, (LONG_PTR)iEnd);
    int newnum = arr ? Array_GetCount(arr) : 0;
    for (int i = 1; i <= newnum; i++) { // Array索引从1开始 ✔️
        HEXOBJ hObjItem = Array_GetMember(arr, i);
        nIndex = iStart + i - 1;
        // fix: 双重边界校验，防止nIndex越界
        if (nIndex >= iStart && nIndex <= iEnd && nIndex >= 1 && nIndex <= nItemCount && lpItems) {
            bool   is_disable = _listview_queryitemstate(lpItems, nIndex, STATE_DISABLE);
            bool   is_select = _listview_queryitemstate(lpItems, nIndex, STATE_SELECT);
            bool   ishover = _listview_queryitemstate(lpItems, nIndex, STATE_HOVER);
            obj_s* pObjItem = nullptr;
            if (_handle_validate(hObjItem, HT_OBJECT, (LPVOID*)&pObjItem, 0) && pObjItem) {
                // fix: 禁用项不绘制悬浮/选中效果，直接重置颜色
                if (is_disable)
                {
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR, 0);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR, 0);
                }
                else if (ishover && !is_select && bItemTracking) {
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR, dwHoverColor);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR, dwBorderColor);
                }
                else if (is_select && !ishover) {
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR, dwSelectColor);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR, dwBorderColor);
                }
                else if (is_select && ishover) {
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR, dwSelectHoverColor);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR, dwBorderColor);
                }
                else {
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR, 0);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR, 0);
                }
                Ex_ObjInvalidateRect(hObjItem, 0); // 合并重绘，减少调用

                if (NeedUpdate || update) {
                    _obj_baseproc(hWnd, hObj, pObj, TEMPLATELISTVIEW_MESSAGE_ITEM_FILL, nIndex, hObjItem);
                    Ex_ObjSetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_INDEX, nIndex); // fix: 替换硬编码0，使用宏定义，防止数据覆盖
                }
                if (((pObjItem->dwStyle_ & OBJECT_STYLE_VISIBLE) != OBJECT_STYLE_VISIBLE)) {
                    _obj_visable(hWnd, hObjItem, pObjItem, true);
                }
            }
        }
        else {
            obj_s* pObjItem = nullptr;
            if (_handle_validate(hObjItem, HT_OBJECT, (LPVOID*)&pObjItem, 0) && pObjItem) {
                _obj_visable(hWnd, hObjItem, pObjItem, false);
            }
        }
    }
}

void _tlv_repos_items(HWND hWnd, HEXOBJ hObj, obj_s* pObj, EX_PAINTSTRUCT& ps, BOOL update)
{
    if (!pObj) return;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    if (!pOwner) return;
    INT         iStart = pOwner->index_start_;
    INT         iEnd = pOwner->index_end_;
    INT         nOffsetX = pOwner->nOffsetX_;
    INT         nOffsetY = pOwner->nOffsetY_;
    INT         nError = 0;
    bool        bHView = (((pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST));
    INT  iStartOld = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_INDEX_START);
    INT  iEndOld = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_INDEX_END);
    INT  nOffsetXOld = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETX);
    INT  nOffsetYOld = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETY);
    BOOL NeedUpdate = (nOffsetX != nOffsetXOld || nOffsetY != nOffsetYOld);

    _tlv_refill(hWnd, hObj, pObj, pOwner, iStart, iStartOld, iEnd, iEndOld, NeedUpdate, update);

    if (NeedUpdate || update) {
        Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETX, nOffsetX);
        Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETY, nOffsetY);

        array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEMARRAY);
        int      count = arr ? Array_GetCount(arr) : 0;
        for (int i = 1; i <= count; i++) { // Array索引从1开始 ✔️
            HEXOBJ hObjItem = Array_GetMember(arr, i);
            obj_s* pObjItem = nullptr;
            if (_handle_validate(hObjItem, HT_OBJECT, (LPVOID*)&pObjItem, &nError) && pObjItem) {
                int nIndex = Ex_ObjGetLong(hObjItem, TEMPLATELISTVIEW_LONG_ITEM_INDEX);
                if (_listview_checkitem_view(pOwner, nIndex)) {
                    RECT rcItem = { 0 };
                    _listview_rectfromiitem(pObj, pOwner, nIndex, bHView, &rcItem);
                    Ex_ObjSetPos(hObjItem, 0, rcItem.left, rcItem.top, rcItem.right - rcItem.left,
                        rcItem.bottom - rcItem.top,
                        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS | SWP_EX_NODPISCALE);
                }
            }
        }
    }
}

void _tlv_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam)
{
    if (!pObj) return;
    INT         x = GET_X_LPARAM(lParam);
    INT         y = GET_Y_LPARAM(lParam);
    listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
    LPVOID      lpItems = pOwner ? pOwner->lpItems_ : nullptr;
    INT         ox = 0, oy = 0;
    INT         iCur = pOwner ? _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy) : 0;
    INT         iHitTest = LISTVIEW_HITTYPE_NOWHERE;

    if (iCur != pOwner->index_mouse_ && lpItems) {
        if ((pObj->dwStyle_ & LISTVIEW_STYLE_ITEMTRACKING) == LISTVIEW_STYLE_ITEMTRACKING) {
            if (_listview_checkitem(pOwner, pOwner->index_mouse_))
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, pOwner->index_mouse_, STATE_HOVER, TRUE, 0, 0, 0);
            pOwner->index_mouse_ = iCur;
        }
        BOOL vCur = _listview_checkitem(pOwner, iCur);
        if (vCur && !_listview_queryitemstate(lpItems, iCur, STATE_DISABLE)) {
            _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_HOVER, FALSE, 0, 0, 0);
            INT tmp = _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_HITTEST, iCur, MAKELONG(ox, oy));
            iHitTest = tmp == 0 ? LISTVIEW_HITTYPE_ONITEM : tmp;
        }
    }
    if (pOwner) pOwner->nHittest_ = iHitTest;
}

void _tlv_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj, LPARAM lParam)
{
    if (!pObj) return;
    INT         x = (GET_X_LPARAM(lParam) - pObj->w_left_);
    INT         y = (GET_Y_LPARAM(lParam) - pObj->w_top_);
    listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
    LPVOID      lpItems = pOwner ? pOwner->lpItems_ : nullptr;
    INT         ox = 0, oy = 0;
    INT         iCur = pOwner ? _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy) : 0;
    INT         imouse = pOwner ? pOwner->index_mouse_ : 0;
    if (!iCur && _listview_checkitem(pOwner, imouse) && lpItems) {
        _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, imouse, STATE_HOVER, TRUE, 0, 0, 0);
        pOwner->index_mouse_ = 0;
    }
}

LRESULT CALLBACK _tlv_scrollbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_MOUSELEAVE || uMsg == WM_MOUSEMOVE) {
        obj_s* pObj = nullptr;
        HEXOBJ parent = Ex_ObjGetParent(hObj);
        if (_handle_validate(parent, HT_OBJECT, (LPVOID*)&pObj, 0) && pObj) {
            if (uMsg == WM_MOUSELEAVE) {
                _obj_baseproc(hWnd, parent, pObj, uMsg, wParam, lParam);
            }
            else {
                lParam = MAKELONG(-1, -1);
                _obj_baseproc(hWnd, parent, pObj, WM_MOUSELEAVE, wParam, lParam);
            }
        }
    }
    return 0;
}

LRESULT CALLBACK _tlv_childhObj_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_EX_RCLICK || uMsg == WM_EX_LCLICK || uMsg == WM_EX_LDCLICK ||
        uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSELEAVE || uMsg == WM_VSCROLL || uMsg == WM_HSCROLL || uMsg == WM_MOUSEWHEEL) {
        LPARAM newlParam = lParam;
        if (uMsg != WM_MOUSELEAVE && uMsg != WM_MOUSEWHEEL) {
            obj_s* pObj = nullptr;
            if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0) && pObj)
            {
                INT x = GET_X_LPARAM(lParam) + pObj->left_;
                INT y = GET_Y_LPARAM(lParam) + pObj->top_;
                newlParam = MAKELONG(x, y);
            }
        }
        HEXOBJ parent = Ex_ObjGetParent(hObj);
        obj_s* pObj = nullptr;
        if (_handle_validate(parent, HT_OBJECT, (LPVOID*)&pObj, 0) && pObj) {
            _obj_baseproc(hWnd, parent, pObj, uMsg, wParam, newlParam);
        }
    }
    return 0;
}

INT CALLBACK _tlv_EnumChild(size_t hObj, size_t lParam)
{
    if (hObj) Ex_ObjSetLong(hObj, OBJECT_LONG_OBJPROC, (LONG_PTR)_tlv_childhObj_proc);
    return 0;
}

LRESULT CALLBACK _tlv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT      nError = 0;
    obj_s* pObj = nullptr;
    array_s* arr = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError) && pObj) {
        if (uMsg == WM_CREATE) {
            if (pObj->objVScroll_) Ex_ObjSetLong(pObj->objVScroll_, OBJECT_LONG_OBJPROC, (LONG_PTR)_tlv_scrollbar_proc);
            if (pObj->objHScroll_) Ex_ObjSetLong(pObj->objHScroll_, OBJECT_LONG_OBJPROC, (LONG_PTR)_tlv_scrollbar_proc);
            arr = Array_Create(0);
            if (arr)
            {
                Array_BindEvent(arr, ARRAY_EVENT_DELMEMBER, _tlv_array_del);
                Array_SetExtra(arr, hObj);
                Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_ITEMARRAY, (LONG_PTR)arr);
                // fix: 修正ExRGB2ARGB传参错误，正确格式：(R,G,B,透明度)，原参数导致颜色异常
                Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_HOVERCOLOR, ExRGB2ARGB(220, 220, 220, 200));  // 悬浮浅灰
                Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_SELECTCOLOR, ExRGB2ARGB(66, 133, 244, 250));  // 选中蓝
                Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_BORDERCOLOR, ExRGB2ARGB(200, 200, 200, 250));  // 边框浅灰
            }
        }
        else if (uMsg == WM_DESTROY) {
            arr = (array_s*)Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEMARRAY);
            if (arr) Array_Destroy(arr);
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_ITEMARRAY, 0);
        }
        else if (uMsg == WM_SIZE) {
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETX, -123321);
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_OFFSETY, -321123);
        }
        else if (uMsg == WM_PAINT) {
            EX_PAINTSTRUCT ps{ 0 };
            if (Ex_ObjBeginPaint(hObj, &ps)) {
                _tlv_repos_items(hWnd, hObj, pObj, ps, lParam);
                Ex_ObjEndPaint(hObj, &ps);
            }
            return 0;
        }
        else if (uMsg == WM_MOUSEMOVE) {
            _tlv_mousemove(hWnd, hObj, pObj, wParam, lParam);
        }
        else if (uMsg == WM_MOUSELEAVE) {
            _tlv_mouseleave(hWnd, hObj, pObj, lParam);
        }
        else if (uMsg == TEMPLATELISTVIEW_MESSAGE_ITEM_CREATE) {
            HEXOBJ handle = Ex_ObjCreateEx(OBJECT_STYLE_EX_TRANSPARENT, L"LISTITEM", 0, -1, 0, 0, 0, 0, hObj, 0, -1, 0, 0, 0);
            if (handle)
            {
                _obj_baseproc(hWnd, hObj, pObj, TEMPLATELISTVIEW_MESSAGE_ITEM_CREATED, 0, handle);
                Ex_ObjEnumChild(handle, _tlv_EnumChild, handle);
            }
            return handle;
        }
        else if (uMsg == TEMPLATELISTVIEW_MESSAGE_GETITEMOBJ)
        {
            listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
            array_s* arr = (array_s*)_obj_getextralong(pObj, TEMPLATELISTVIEW_LONG_ITEMARRAY);
            if (!pOwner || !arr) return 0;
            INT         iStart = pOwner->index_start_;
            int         num = Array_GetCount(arr);
            int         index = wParam - iStart + 1;
            if (index > 0 && index <= num) return Array_GetMember(arr, index);
            return 0;
        }
        else if (uMsg == LISTVIEW_MESSAGE_GETITEMSTATE)
        {
            listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
            if (!pOwner || !pOwner->lpItems_) return 0;
            INT         nCount = pOwner->count_items_;
            LPVOID lpItems = pOwner->lpItems_;
            if (_listview_checkitem(pOwner, wParam)) {
                return _listview_getitemstate(lpItems, wParam);
            }
            return 0;
        }
        else if (uMsg == LISTVIEW_MESSAGE_GETITEMSELECT)
        {
            listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
            if (!pOwner || !pOwner->lpItems_) return 0;
            if (_listview_checkitem(pOwner, wParam)) {
                return _listview_queryitemstate(pOwner->lpItems_, wParam, STATE_SELECT);
            }
            return 0;
        }
        else if (uMsg == LISTVIEW_MESSAGE_SETSELECTIONMARK) {
            listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
            LPVOID      lpItems = pOwner ? pOwner->lpItems_ : nullptr;
            if (!pOwner || !lpItems) return 0;
            INT         nCount = pOwner->count_items_;
            if (lParam > 0 && lParam <= nCount) {
                for (INT i = 1; i <= nCount; i++) {
                    if (_listview_queryitemstate(lpItems, i, STATE_SELECT)) {
                        _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, i, pOwner->index_select_);
                    }
                }
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, lParam, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, lParam, pOwner->index_select_);
                if (wParam != 0) _listview_showitem(hWnd, hObj, pObj, lParam, FALSE);
                pOwner->index_select_ = lParam;
                pOwner->count_selects_ = 1;
            }
        }
        else if (uMsg == TEMPLATELISTVIEW_MESSAGE_SET_ITEM_HOVERCOLOR) {
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_HOVERCOLOR, lParam);
        }
        else if (uMsg == TEMPLATELISTVIEW_MESSAGE_SET_ITEM_SELECTCOLOR) {
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_SELECTCOLOR, lParam);
        }
        else if (uMsg == TEMPLATELISTVIEW_MESSAGE_GETCUIINDEX) {
            listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
            if (!pOwner) return 0;
            INT ox = 0, oy = 0;
            return _listview_itemfrompos(pObj, pOwner, wParam, lParam, &ox, &oy);
        }
    }
    return m_pfnTListView(hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _tlvi_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError) && pObj) {
        if (uMsg == WM_CREATE) {
            Ex_ObjSetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_BORDERTHICKNESS, 2);
        }
        else if (uMsg == WM_PAINT) {
            EX_PAINTSTRUCT ps{ 0 };
            if (Ex_ObjBeginPaint(hObj, &ps)) {
                DWORD dwFillColor = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_FILLCOLOR);
                auto hBrush = _brush_create(dwFillColor);
                if (hBrush) {
                    _canvas_clear(ps.hCanvas, 0);
                    INT rad = Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_ENABLERADIUS);
                    if (rad > 0) {
                        HEXPATH hPath = NULL;
                        _path_create(PATH_FLAG_DISABLESCALE, &hPath);
                        if (hPath)
                        {
                            _path_open(hPath);
                            _path_beginfigure2(hPath, pObj->c_left_, pObj->c_top_ - Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_RADIUS1) * g_Li.DpiX);
                            _path_addroundedrect(hPath, pObj->c_left_, pObj->c_top_, pObj->c_right_, pObj->c_bottom_,
                                Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_RADIUS1) * g_Li.DpiX,
                                Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_RADIUS2) * g_Li.DpiX,
                                Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_RADIUS3) * g_Li.DpiX,
                                Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_RADIUS4) * g_Li.DpiX);
                            _path_endfigure(hPath, TRUE);
                            _path_close(hPath);
                            _canvas_fillpath(ps.hCanvas, hPath, hBrush);
                            if (Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_ENABLEBORDER))
                            {
                                _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR));
                                _canvas_drawpath(ps.hCanvas, hPath, hBrush, Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_BORDERTHICKNESS), D2D1_DASH_STYLE_SOLID);
                            }
                            _path_destroy(hPath);
                        }
                    }
                    else {
                        _canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.uWidth, ps.uHeight);
                        if (Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_ENABLEBORDER))
                        {
                            _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_BORDERCOLOR));
                            _canvas_drawrect(ps.hCanvas, hBrush, 0, 0, ps.uWidth, ps.uHeight, Ex_ObjGetLong(hObj, TEMPLATELISTVIEW_LONG_ITEM_BORDERTHICKNESS), D2D1_DASH_STYLE_SOLID);
                        }
                    }
                    _brush_destroy(hBrush);
                }
                LPCWSTR lpText = pObj->pstrTitle_;
                if (lpText) {
                    _canvas_drawtextex(ps.hCanvas, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), lpText, -1, ps.dwTextFormat, ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom);
                }
                Ex_ObjEndPaint(hObj, &ps);
            }
            return 0;
        }
        else if (uMsg == WM_MOUSEWHEEL) {
            return Ex_ObjDispatchMessage(pObj->objParent_, uMsg, wParam, lParam);
        }
        else if (uMsg == WM_EX_RCLICK || uMsg == WM_EX_LCLICK || uMsg == WM_EX_LDCLICK ||
            uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSELEAVE || uMsg == WM_VSCROLL || uMsg == WM_HSCROLL || uMsg == WM_MOUSEWHEEL) {
            LPARAM newlParam = lParam;
            if (uMsg != WM_MOUSELEAVE && uMsg != WM_MOUSEWHEEL) {
                INT x = GET_X_LPARAM(lParam) + pObj->left_;
                INT y = GET_Y_LPARAM(lParam) + pObj->top_;
                newlParam = MAKELONG(x, y);
            }
            Ex_ObjDispatchMessage(pObj->objParent_, uMsg, wParam, newlParam);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _listview_setitemstate(LPVOID lpItems, INT iItem, INT dwState, BOOL bRemove)
{
    // fix: 校验iItem合法性，防止(iItem-1)*4 负数偏移量，内存越界
    if (!lpItems || iItem < 1) return;
    INT offset = (iItem - 1) * 4;
    if (bRemove) __del(lpItems, offset, dwState);
    else __add(lpItems, offset, dwState);
}

void _listview_updatesbvalue(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, RECT* lpRcClient)
{
    if (!pObj || !pOwner || !lpRcClient) return;
    HEXOBJ hHSB = pObj->objHScroll_;
    HEXOBJ hVSB = pObj->objVScroll_;
    INT    width = lpRcClient->right - lpRcClient->left;
    INT  height = lpRcClient->bottom - lpRcClient->top;
    BOOL bHView = ((pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST);
    INT nCount = pOwner->count_items_;
    INT iWidth = (pOwner->width_item_ + pOwner->width_split_) * g_Li.DpiX;
    INT iHeight = (pOwner->height_item_ + pOwner->height_split_) * g_Li.DpiX;
    INT nVS = 0;
    INT nHS = 0;
    // fix: 【致命】防止除以0，除数必须>0，否则赋值1，杜绝崩溃 ✔️
    if (iHeight > 0) nVS = height / iHeight;
    if (nVS <= 0) nVS = 1;
    if (iWidth > 0) nHS = width / iWidth;
    if (nHS <= 0) nHS = 1;

    if (bHView) nHS = nHS + 1;
    else nVS = nVS + 1;

    INT vWidth = 0;
    INT vHeight = 0;
    if (bHView)
    {
        vWidth = (nCount / nVS + (nCount % nVS == 0 ? 0 : 1)) * iWidth;
        if (hHSB && vWidth > width) {
            if (nVS > 1 && height < nVS * iHeight) nVS--;
            vWidth = (nCount / nVS + (nCount % nVS == 0 ? 0 : 1)) * iWidth;
        }
        if (vWidth < width) vWidth = width;
    }
    else {
        vHeight = (nCount / nHS + (nCount % nHS == 0 ? 0 : 1)) * iHeight;
        if (vHeight > height && nHS > 1 && width < nHS * iWidth) nHS--;
        vHeight = (nCount / nHS + (nCount % nHS == 0 ? 0 : 1)) * iHeight;
        vWidth = nHS * iWidth;
    }
    pOwner->count_view_h_ = nHS;
    pOwner->count_view_v_ = nVS;
    pOwner->count_view_ = nVS * nHS;
    pOwner->width_view_ = vWidth;
    pOwner->height_view_ = vHeight;
    // fix: 滚动条句柄判空，防止无效调用
    if (hVSB) {
        Ex_ObjScrollShow(hVSB, SCROLLBAR_TYPE_CONTROL, vHeight - height > 0);
        Ex_ObjScrollSetInfo(hVSB, SCROLLBAR_TYPE_CONTROL, SIF_PAGE | SIF_RANGE, 0, vHeight - height, height, 0, FALSE);
    }
    if (hHSB) {
        Ex_ObjScrollShow(hHSB, SCROLLBAR_TYPE_CONTROL, vWidth - width > 0);
        Ex_ObjScrollSetInfo(hHSB, SCROLLBAR_TYPE_CONTROL, SIF_PAGE | SIF_RANGE, 0, vWidth - width, width, 0, FALSE);
    }
}

void _listview_updateviewindex(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, BOOL bHView, RECT* lpRcClient)
{
    if (!pObj || !pOwner || !lpRcClient) return;
    INT nWidth = (pOwner->width_item_ + pOwner->width_split_) * g_Li.DpiX;
    INT nHeight = (pOwner->height_item_ + pOwner->height_split_) * g_Li.DpiY;
    // fix: 【致命】防止除以0 ✔️
    if (nWidth <= 0) nWidth = 1;
    if (nHeight <= 0) nHeight = 1;

    INT nOffset = 0, iStart = 0, iEnd = 0;
    if (bHView) {
        nOffset = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
        if (nOffset != pOwner->nOffsetX_) pOwner->nOffsetX_ = nOffset;
        iStart = (nOffset / nWidth) * pOwner->count_view_v_ + 1;
    }
    else {
        nOffset = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
        if (nOffset != pOwner->nOffsetY_) pOwner->nOffsetY_ = nOffset;
        iStart = (nOffset / nHeight) * pOwner->count_view_h_ + 1;
    }
    iEnd = iStart + pOwner->count_view_ - 1;
    // fix: 边界裁剪，防止iEnd超过总条目数，绘制越界
    if (iEnd > pOwner->count_items_) iEnd = pOwner->count_items_;
    if (iStart < 1) iStart = 1;
    pOwner->index_start_ = iStart;
    pOwner->index_end_ = iEnd;
}

BOOL _listview_checkitem(listview_s* pOwner, INT iItem)
{
    return (pOwner && iItem > 0 && pOwner->count_items_ >= iItem);
}

BOOL _listview_checkitem_view(listview_s* pOwner, INT iItem)
{
    BOOL ret = FALSE;
    if (_listview_checkitem(pOwner, iItem)) {
        ret = iItem >= pOwner->index_start_ && iItem <= pOwner->index_end_;
    }
    return ret;
}

void _listview_rectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* rcItem)
{
    if (!pObj || !pOwner || !rcItem || iItem < 1) return;
    INT i = iItem - 1;
    INT nWidth = (pOwner->width_item_ + pOwner->width_split_) * g_Li.DpiX;
    INT nHeight = (pOwner->height_item_ + pOwner->height_split_) * g_Li.DpiY;
    if (bHView) {
        rcItem->left = pObj->c_left_ + (i / pOwner->count_view_v_) * nWidth - pOwner->nOffsetX_;
        rcItem->top = pObj->c_top_ + (i % pOwner->count_view_v_) * nHeight - pOwner->nOffsetY_;
    }
    else {
        rcItem->left = pObj->c_left_ + (i % pOwner->count_view_h_) * nWidth - pOwner->nOffsetX_;
        rcItem->top = pObj->c_top_ + (i / pOwner->count_view_h_) * nHeight - pOwner->nOffsetY_;
    }
    rcItem->right = rcItem->left + pOwner->width_item_ * g_Li.DpiX;
    rcItem->bottom = rcItem->top + pOwner->height_item_ * g_Li.DpiY;
}

void _listview_lprectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* lpRc)
{
    if (lpRc == 0) return;
    RECT rcItem{ 0 };
    _listview_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
    RtlMoveMemory(lpRc, &rcItem, sizeof(RECT));
}

INT _listview_itemfrompos(obj_s* pObj, listview_s* pOwner, INT x, INT y, INT* offsetPosX, INT* offsetPosY)
{
    if (!pObj || !pOwner || !offsetPosX || !offsetPosY) return 0;
    *offsetPosX = 0;
    *offsetPosY = 0;
    POINT pt = { x,y };
    INT uItem = 0;
    RECT rcClient = { pObj->c_left_, pObj->c_top_, pObj->c_right_, pObj->c_bottom_ };
    if (PtInRect(&rcClient, pt)) {
        INT nWidth = (pOwner->width_item_ + pOwner->width_split_) * g_Li.DpiX;
        INT nHeight = (pOwner->height_item_ + pOwner->height_split_) * g_Li.DpiY;
        // fix: 【致命】防止取模0/除以0 ✔️
        if (nWidth <= 0 || nHeight <= 0) return 0;

        INT realleft = 0, realtop = 0, offsetx = 0, offsety = 0;
        if ((pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST) {
            realleft = x - pObj->c_left_ + pOwner->nOffsetX_;
            offsetx = realleft % nWidth;
            if (offsetx <= pOwner->width_item_ * g_Li.DpiX) {
                realtop = y - pObj->c_top_ + pOwner->nOffsetY_;
                offsety = realtop % nHeight;
                if (offsety <= pOwner->height_item_ * g_Li.DpiY) {
                    INT tmp = realtop / nHeight;
                    if (tmp < pOwner->count_view_v_) {
                        uItem = (realleft / nWidth) * pOwner->count_view_v_ + tmp + 1;
                        if (uItem > pOwner->count_items_) uItem = 0;
                        else { *offsetPosX = offsetx; *offsetPosY = offsety; }
                    }
                }
            }
        }
        else {
            realtop = y - pObj->c_top_ + pOwner->nOffsetY_;
            offsety = realtop % nHeight;
            if (offsety <= pOwner->height_item_ * g_Li.DpiY) {
                realleft = x - pObj->c_left_ + pOwner->nOffsetX_;
                offsetx = realleft % nWidth;
                if (offsetx <= pOwner->width_item_ * g_Li.DpiX) {
                    INT tmp = realleft / nWidth;
                    if (tmp < pOwner->count_view_h_) {
                        uItem = (realtop / nHeight) * pOwner->count_view_h_ + tmp + 1;
                        if (uItem > pOwner->count_items_) uItem = 0;
                        else { *offsetPosX = offsetx; *offsetPosY = offsety; }
                    }
                }
            }
        }
    }
    return uItem;
}

INT _listview_getitemstate(LPVOID lpItems, INT iItem)
{
    if (!lpItems || iItem < 1) return 0;
    return __get_int(lpItems, (iItem - 1) * 4);
}

BOOL _listview_queryitemstate(LPVOID lpItems, INT iItem, INT dwState)
{
    if (!lpItems || iItem < 1 || dwState == 0) return FALSE;
    return __query(lpItems, (iItem - 1) * 4, dwState);
}

void _listview_getscrollbarvalue(obj_s* pObj, listview_s* pOwner, BOOL bHSB, HEXOBJ* hSB, INT* oPos, INT* nLine, INT* nPage, INT* nView)
{
    if (!pObj || !pOwner || !hSB || !oPos || !nLine || !nPage || !nView) return;
    if (bHSB) {
        *oPos = pOwner->nOffsetX_;
        *hSB = pObj->objHScroll_;
        *nPage = pObj->c_right_ - pObj->c_left_;
        *nLine = (pOwner->width_item_ + pOwner->width_split_) * g_Li.DpiX;
        *nView = pOwner->width_view_;
    }
    else {
        *oPos = pOwner->nOffsetY_;
        *hSB = pObj->objVScroll_;
        *nPage = pObj->c_bottom_ - pObj->c_top_;
        *nLine = (pOwner->height_item_ + pOwner->height_split_) * g_Li.DpiX;
        *nView = pOwner->height_view_;
    }
    // fix: 防止nLine为0 ✔️
    if (*nLine <= 0) *nLine = 1;
}

INT _listview_checkpos(INT nPos, INT nView, INT nPage)
{
    if (nPos > nView - nPage) nPos = nView - nPage;
    if (nPos < 0) nPos = 0;
    return nPos;
}

void _listview_item_changestate(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, LPVOID lpItems, INT iItem, INT state, BOOL bRemove, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (!lpItems || !_listview_checkitem(pOwner, iItem)) return;
    _listview_setitemstate(lpItems, iItem, state, bRemove);
    _listview_redrawitem(pObj, pOwner, iItem);
    if (nEvent != 0) _obj_dispatchnotify(hWnd, pObj, hObj, 0, nEvent, wParam, lParam);
}

void _listview_redrawitem(obj_s* pObj, listview_s* pOwner, INT iItem)
{
    if (!pObj || !pOwner || !_listview_checkitem_view(pOwner, iItem)) return;
    RECT rcItem{ 0 };
    INT  nError = 0;
    _listview_rectfromiitem(pObj, pOwner, iItem, (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST, &rcItem);
    _obj_invalidaterect(pObj, &rcItem, &nError);
}

size_t _listview_setitemcount(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT nCount, LPARAM lParam)
{
    if (!pObj) return 0;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    BOOL bHView = (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST;
    LPVOID pOld = pOwner->lpItems_;
    // fix: 释放旧内存，防止内存泄漏
    if (pOld) { Ex_MemFree(pOld); pOld = nullptr; }
    int index = HIWORD(lParam);
    int index_select = pOwner->index_select_;
    if (nCount < 1) { index = 0; index_select = 0; }
    // fix: 内存申请判空，防止空指针 ✔️
    LPVOID pNew = nCount > 0 ? Ex_MemAlloc(nCount * 4) : nullptr;
    pOwner->lpItems_ = pNew;
    pOwner->count_items_ = nCount;
    pOwner->index_select_ = 0;
    pOwner->index_mouse_ = 0;
    pOwner->count_selects_ = 0;

    _listview_updatesbvalue(hObj, pObj, pOwner, (RECT*)&pObj->c_left_);
    INT nPosX = 0, nPosY = 0, nPage = 0, nView = 0, nLine = 0;
    if ((LOWORD(lParam) & LVSICF_NOSCROLL) != LVSICF_NOSCROLL) {
        HEXOBJ hSB = 0;
        _listview_getscrollbarvalue(pObj, pOwner, TRUE, &hSB, &nPosX, &nLine, &nPage, &nView);
        nPosX = _listview_checkpos(nPosX, nView, nPage);
        _listview_getscrollbarvalue(pObj, pOwner, FALSE, &hSB, &nPosY, &nLine, &nPage, &nView);
        nPosY = _listview_checkpos(nPosY, nView, nPage);
    }
    pOwner->nOffsetX_ = nPosX;
    pOwner->nOffsetY_ = nPosY;
    Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, nPosX, TRUE);
    Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, nPosY, TRUE);
    _listview_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
    INT nError = 0;
    _obj_invalidaterect(pObj, 0, &nError);
    if (nCount > 0 && index > 0) {
        if (index >= nCount) _listview_reselect(hWnd, hObj, pObj, pOwner, index, TRUE);
    }
    return 0;
}

void _listview_onvscrollbar(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!pObj) return;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    INT         nCode = LOWORD(wParam);
    BOOL bHView = (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST;
    BOOL bHScoll = uMsg == WM_HSCROLL;
    HEXOBJ hSB = 0; INT oPos = 0, nPage = 0, nLine = 0, nView = 0;
    _listview_getscrollbarvalue(pObj, pOwner, bHScoll, &hSB, &oPos, &nLine, &nPage, &nView);
    INT nPos = 0;
    if (nCode == SB_THUMBPOSITION) nPos = Ex_ObjScrollGetTrackPos(hSB, SCROLLBAR_TYPE_CONTROL);
    else {
        switch (nCode)
        {
        case SB_PAGEUP: nPos = oPos - nPage; break;
        case SB_PAGEDOWN: nPos = oPos + nPage; break;
        case SB_LINEUP: nPos = oPos - nLine; break;
        case SB_LINEDOWN: nPos = oPos + nLine; break;
        case SB_TOP: nPos = 0; break;
        case SB_BOTTOM: nPos = nView - nPage; break;
        default: return;
        }
    }
    nPos = _listview_checkpos(nPos, nView, nPage);
    if (nPos != oPos) {
        if (bHScoll) pOwner->nOffsetX_ = nPos;
        else pOwner->nOffsetY_ = nPos;
        if (hSB) Ex_ObjScrollSetPos(hSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
        _listview_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
        INT nError = 0; _obj_invalidaterect(pObj, 0, &nError);
    }
}

void _listview_itemselectone(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, LPVOID lpItems, INT nCount, INT iCur, listview_s* pOwner, INT iSelect)
{
    if (!lpItems || nCount < 1 || !_listview_checkitem(pOwner, iCur)) return;
    for (INT i = 1; i <= nCount; i++) {
        if (_listview_queryitemstate(lpItems, i, STATE_SELECT) && i != iCur) {
            _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, i, iSelect);
        }
    }
    _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, iCur, iSelect);
    _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMCHANGED, iCur, iSelect);
    if (uMsg == WM_EX_LDCLICK) _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMDCLICK, iCur, iSelect);
    pOwner->count_selects_ = 1;
    pOwner->index_select_ = iCur;
}

void _listview_itemselectzero(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, LPVOID lpItems, INT nCount, INT iCur, listview_s* pOwner, INT iSelect)
{
    if (!lpItems || nCount < 1) return;
    for (INT i = 1; i <= nCount; i++) {
        if (_listview_queryitemstate(lpItems, i, STATE_SELECT)) {
            _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, i, iSelect);
        }
    }
    if (uMsg == WM_EX_LDCLICK) _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMDCLICK, iCur, iSelect);
    pOwner->count_selects_ = 0;
    pOwner->index_select_ = 0;
}

// fix: 修正参数名拼写错误 wParram → wParam
void _listview_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!pObj) return;
    listview_s* pOwner = (listview_s*)pObj->dwOwnerData_;
    INT         nCount = pOwner ? pOwner->count_items_ : 0;
    if (nCount < 1) return;
    INT    x = GET_X_LPARAM(lParam);
    INT    y = GET_Y_LPARAM(lParam);
    LPVOID lpItems = pOwner->lpItems_;
    INT    ox = 0, oy = 0, iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
    BOOL   vCur = _listview_checkitem(pOwner, iCur);
    if (vCur && _listview_queryitemstate(lpItems, iCur, STATE_DISABLE)) return;

    INT iSelect = pOwner->index_select_;
    BOOL vSelect = _listview_checkitem(pOwner, iSelect);
    BOOL bSingleSel = !((pObj->dwStyle_ & LISTVIEW_STYLE_ALLOWMULTIPLE) == LISTVIEW_STYLE_ALLOWMULTIPLE);
    BOOL bShowAllwasy = ((pObj->dwStyle_ & LISTVIEW_STYLE_SHOWSELALWAYS) == LISTVIEW_STYLE_SHOWSELALWAYS);
    INT iKey = _wnd_getfunctionkeys();

    if (bSingleSel)
    {
        if (iCur != iSelect) {
            if (vSelect && (!bShowAllwasy || vCur)) {
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, iCur, iSelect);
            }
            else if (vCur == false) iCur = iSelect;
            if (vCur) {
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, iCur, iSelect);
                _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMCHANGED, iCur, iSelect);
            }
            pOwner->index_select_ = iCur;
            pOwner->count_selects_ = 1;
        }
        if (uMsg == WM_EX_RCLICK && vCur) _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMRCLICK, iCur, 1);
        if (uMsg == WM_EX_LDCLICK && vCur) _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMDCLICK, iCur, iSelect);
    }
    else
    {
        INT& nSelects = pOwner->count_selects_;
        if (uMsg == WM_EX_RCLICK && vCur && (iKey & 3) != 0) return;
        if ((iKey & 1) == 0 && (iKey & 2) == 0)
        {
            if (bShowAllwasy)
            {
                if (((uMsg == WM_EX_LCLICK || uMsg == WM_EX_LDCLICK) && vCur) || (uMsg == WM_EX_RCLICK && vCur && !_listview_queryitemstate(lpItems, iCur, STATE_SELECT)))
                    _listview_itemselectone(hWnd, hObj, pObj, uMsg, lpItems, nCount, iCur, pOwner, iSelect);
                else if (uMsg == WM_EX_RCLICK && !vCur)
                    _listview_itemselectzero(hWnd, hObj, pObj, uMsg, lpItems, nCount, iCur, pOwner, iSelect);
                else pOwner->index_select_ = iSelect;
            }
            else
            {
                if (vCur) _listview_itemselectone(hWnd, hObj, pObj, uMsg, lpItems, nCount, iCur, pOwner, iSelect);
                else _listview_itemselectzero(hWnd, hObj, pObj, uMsg, lpItems, nCount, iCur, pOwner, iSelect);
            }
        }
        else if ((iKey & 1) && !(iKey & 2) && uMsg == WM_EX_LCLICK && vCur)
        {
            if (!vSelect) pOwner->index_select_ = iCur;
            bool is_select = _listview_queryitemstate(lpItems, iCur, STATE_SELECT);
            if (is_select) { _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, iCur, iSelect); nSelects--; }
            else { _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, iCur, iSelect); nSelects++; }
        }
        else if (!(iKey & 1) && (iKey & 2) && uMsg == WM_EX_LCLICK && vCur)
        {
            if (!vSelect)
            {
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, iCur, iSelect);
                pOwner->index_select_ = iCur; nSelects = 1;
            }
            else
            {
                nSelects = 0;
                for (INT i = 1; i <= nCount; i++)
                {
                    bool inside = pOwner->index_select_ <= iCur ? (pOwner->index_select_ <= i && i <= iCur) : (iCur <= i && i <= pOwner->index_select_);
                    if (inside)
                    {
                        if (!_listview_queryitemstate(lpItems, i, STATE_SELECT)) _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMSELECTD, i, iSelect);
                        nSelects++;
                    }
                    else
                    {
                        if (_listview_queryitemstate(lpItems, i, STATE_SELECT)) _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, LISTVIEW_EVENT_ITEMSELECTC, i, iSelect);
                    }
                }
            }
        }
        if (uMsg == WM_EX_RCLICK && vCur) _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMRCLICK, iCur, pOwner->count_selects_);
    }
}

size_t _listview_reselect(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bShow)
{
    if (!pOwner || !pObj) return 0;
    INT    nCount = pOwner->count_items_;
    LPVOID lpItems = pOwner->lpItems_;
    if (nCount > 0 && lpItems) {
        for (INT i = 1; i <= nCount; i++) {
            if (_listview_queryitemstate(lpItems, i, STATE_SELECT)) {
                _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, 0, 0, 0);
            }
        }
        pOwner->count_selects_ = 0;
    }
    if (iItem >= 0 && _listview_checkitem(pOwner, iItem) && lpItems) {
        INT oldSelect = pOwner->index_select_;
        pOwner->index_select_ = iItem;
        pOwner->count_selects_ = 1;
        _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iItem, STATE_SELECT, FALSE, LISTVIEW_EVENT_ITEMCHANGED, iItem, oldSelect);
        if (bShow) _listview_showitem(hWnd, hObj, pObj, iItem, false);
    }
    return 0;
}

size_t _listview_showitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT iItem, BOOL bCheck)
{
    if (!pObj || !_listview_checkitem((listview_s*)_obj_pOwner(pObj), iItem)) return 0;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    RECT rcClient = { pObj->c_left_,pObj->c_top_,pObj->c_right_,pObj->c_bottom_ };
    BOOL bHView = ((pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST);
    RECT rcItem{ 0 };
    _listview_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
    OffsetRect(&rcItem, -rcClient.left, -rcClient.top);
    HEXOBJ hSB = 0; INT oPos = 0, nPage = 0, nLine = 0, nView = 0;
    _listview_getscrollbarvalue(pObj, pOwner, bHView, &hSB, &oPos, &nLine, &nPage, &nView);
    INT nPos = 0;
    if (bHView)
    {
        if (rcItem.left < 0) nPos = oPos + rcItem.left;
        else if (rcItem.right > nPage) nPos = oPos + rcItem.right - nPage;
        else return 0;
    }
    else
    {
        if (rcItem.top < 0) nPos = oPos + rcItem.top;
        else if (rcItem.bottom > nPage) nPos = oPos + rcItem.bottom - nPage;
        else return 0;
    }
    nPos = _listview_checkpos(nPos, nView, nPage);
    if (bHView) pOwner->nOffsetX_ = nPos;
    else pOwner->nOffsetY_ = nPos;
    if (hSB) Ex_ObjScrollSetPos(hSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
    _listview_updateviewindex(hObj, pObj, pOwner, bHView, &rcClient);
    INT nError = 0; _obj_invalidaterect(pObj, 0, &nError);
    return 0;
}

void _listview_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    if (!pObj) return;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    LPVOID      lpItems = pOwner ? pOwner->lpItems_ : nullptr;
    INT         iLast = pOwner->index_mouse_;
    if (_listview_checkitem(pOwner, iLast) && lpItems) {
        _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, TRUE, 0, 0, 0);
        pOwner->index_mouse_ = 0;
    }
}

void _listview_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam)
{
    if (!pObj) return;
    INT         x = GET_X_LPARAM(lParam);
    INT         y = GET_Y_LPARAM(lParam);
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    LPVOID      lpItems = pOwner ? pOwner->lpItems_ : nullptr;
    INT         iSelect = pOwner->index_select_;
    INT         iLast = pOwner->index_mouse_;
    BOOL        vLast = _listview_checkitem(pOwner, iLast);
    INT         ox = 0, oy = 0, iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
    BOOL        vCur = _listview_checkitem(pOwner, iCur);
    INT         iHitTest = LISTVIEW_HITTYPE_NOWHERE;
    INT         tmp = 0;
    if (vCur) { tmp = _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_HITTEST, iCur, MAKELONG(ox, oy)); iHitTest = tmp == 0 ? LISTVIEW_HITTYPE_ONITEM : tmp; }
    pOwner->nHittest_ = iHitTest;
    BOOL bSingelSelect = !((pObj->dwStyle_ & LISTVIEW_STYLE_ALLOWMULTIPLE) == LISTVIEW_STYLE_ALLOWMULTIPLE);
    BOOL bItemTracking = ((pObj->dwStyle_ & LISTVIEW_STYLE_ITEMTRACKING) == LISTVIEW_STYLE_ITEMTRACKING);
    BOOL bShowAllwasy = ((pObj->dwStyle_ & LISTVIEW_STYLE_SHOWSELALWAYS) == LISTVIEW_STYLE_SHOWSELALWAYS);

    if (wParam != 1 || bSingelSelect) {
        if (bItemTracking) {
            if (iCur != iSelect) {
                if (vCur || !bShowAllwasy) {
                    if (iSelect != 0) _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, TRUE, 0, 0, 0);
                    pOwner->index_select_ = iCur;
                }
                if (vCur && !_listview_queryitemstate(lpItems, iCur, STATE_DISABLE)) {
                    _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LISTVIEW_EVENT_HOTTRACK, iCur, iSelect);
                }
                _obj_dispatchnotify(hWnd, pObj, hObj, 0, LISTVIEW_EVENT_ITEMCHANGED, iCur, iSelect);
            }
        }
        else {
            if (iCur != iLast) {
                if (vLast) _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, TRUE, 0, 0, 0);
                if (vCur && !_listview_queryitemstate(lpItems, iCur, STATE_DISABLE)) _listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_HOVER, FALSE, 0, 0, 0);
            }
        }
    }
    pOwner->index_mouse_ = iCur;
}

void _listview_drawitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, EX_PAINTSTRUCT ps, INT iItem, RECT rcClip, RECT rcItem)
{
    if (!pOwner) return;
    INT atomRect = 0;
    EX_CUSTOMDRAW ecd{ 0 };
    ecd.hTheme = ps.hTheme; ecd.hCanvas = ps.hCanvas; ecd.dwState = _listview_getitemstate(pOwner->lpItems_, iItem);
    ecd.dwStyle = ps.dwStyle; ecd.rcPaint = rcItem; ecd.iItem = iItem;
    if (_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CUSTOMDRAW, iItem, (size_t)&ecd) == 0) {
        if (ecd.dwState & STATE_SELECT) atomRect = ATOM_SELECT;
        else if (ecd.dwState & STATE_HOVER) atomRect = ATOM_HOVER;
        if (atomRect && pObj->crChecked_ != TRUE) {
            Ex_ThemeDrawControlEx(ecd.hTheme, ecd.hCanvas, ecd.rcPaint.left, ecd.rcPaint.top, ecd.rcPaint.right, ecd.rcPaint.bottom, ATOM_ITEM, atomRect, 0, 0, 0, 0, 255);
        }
    }
}

size_t _listview_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
        INT         iStart = pOwner->index_start_;
        INT         iEnd = pOwner->index_end_;
        BOOL        bHView = (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST;
        for (INT i = iStart; i <= iEnd; i++) {
            if (_listview_checkitem_view(pOwner, i)) {
                RECT rcItem{ 0 }, rcClip{ 0 };
                _listview_rectfromiitem(pObj, pOwner, i, bHView, &rcItem);
                if (IntersectRect(&rcClip, &ps.rcPaint, &rcItem)) {
                    _listview_drawitem(hWnd, hObj, pObj, pOwner, ps, i, rcClip, rcItem);
                }
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
    return 0;
}

size_t _listview_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    if (!pObj) return 0;
    listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
    BOOL bHView = (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST;
    RECT rcClient = { pObj->c_left_,pObj->c_top_,pObj->c_right_,pObj->c_bottom_ };
    FLOAT iWidth = 0, iHeight = 0;
    _canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, L"S", 1, 0, rcClient.right, rcClient.bottom, &iWidth, &iHeight);
    if (bHView) { iWidth = 60; iHeight = 60; }
    else { iWidth = (FLOAT)(rcClient.right - rcClient.left - Ex_Scale(1)) / g_Li.DpiX; iHeight = (FLOAT)iHeight / g_Li.DpiY + 8; }
    // fix: 防止尺寸为0 ✔️
    if (iWidth <= 0) iWidth = 1;
    if (iHeight <= 0) iHeight = 1;

    pOwner->width_item_ = Ex_Scale(iWidth);
    pOwner->height_item_ = Ex_Scale(iHeight);
    pOwner->width_spec_ = pOwner->width_item_;
    pOwner->height_spec_ = pOwner->height_item_;
    pOwner->width_split_ = Ex_Scale(1);
    pOwner->height_split_ = Ex_Scale(1);
    _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CALCSIZE, 0, (size_t)pOwner + offsetof(listview_s, width_item_));
    _listview_updatesbvalue(hObj, pObj, pOwner, &rcClient);
    _listview_updateviewindex(hObj, pObj, pOwner, bHView, &rcClient);
    return 0;
}

LRESULT CALLBACK _listview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError) && pObj) {
        if (uMsg == WM_CREATE) {
            pObj->dwOwnerData_ = new listview_s{ 0 };
            pObj->crChecked_ = FALSE;
        }
        else if (uMsg == WM_DESTROY) {
            if (pObj->dwOwnerData_) { delete (listview_s*)pObj->dwOwnerData_; pObj->dwOwnerData_ = nullptr; }
        }
        else if (uMsg == WM_SIZE) { _listview_size(hWnd, hObj, pObj); }
        else if (uMsg == WM_PAINT) { return _listview_paint(hWnd, hObj, pObj); }
        else if (uMsg == WM_MOUSELEAVE) { _listview_mouseleave(hWnd, hObj, pObj); }
        else if (uMsg == WM_MOUSEMOVE) { _listview_mousemove(hWnd, hObj, pObj, wParam, lParam); }
        else if (uMsg == LISTVIEW_MESSAGE_HITTEST) { return ((listview_s*)_obj_pOwner(pObj))->nHittest_; }
        else if (uMsg == WM_EX_LCLICK || uMsg == WM_EX_RCLICK || uMsg == WM_EX_LDCLICK) { _listview_btndown(hWnd, hObj, pObj, uMsg, wParam, lParam); }
        else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) { _listview_onvscrollbar(hWnd, hObj, pObj, uMsg, wParam, lParam); }
        else if (uMsg == LISTVIEW_MESSAGE_SETITEMCOUNT) { return _listview_setitemcount(hWnd, hObj, pObj, wParam, lParam); }
        else if (uMsg == LISTVIEW_MESSAGE_DELETEALLITEMS) { return _listview_setitemcount(hWnd, hObj, pObj, 0, 0); }
        else if (uMsg == LISTVIEW_MESSAGE_ENSUREVISIBLE) { return _listview_showitem(hWnd, hObj, pObj, lParam, TRUE); }
        else if (uMsg == LISTVIEW_MESSAGE_GETITEMSTATE) { return _listview_checkitem((listview_s*)_obj_pOwner(pObj), wParam) ? _listview_getitemstate(((listview_s*)_obj_pOwner(pObj))->lpItems_, wParam) : 0; }
        else if (uMsg == LISTVIEW_MESSAGE_SETITEMSTATE) { _listview_lvm_setitemstate(pObj, (listview_s*)_obj_pOwner(pObj), wParam, lParam); }
        else if (uMsg == LISTVIEW_MESSAGE_GETCOUNTPERPAGE) { return ((listview_s*)_obj_pOwner(pObj))->count_view_; }
        else if (uMsg == LISTVIEW_MESSAGE_GETITEMCOUNT) { return ((listview_s*)_obj_pOwner(pObj))->count_items_; }
        else if (uMsg == LISTVIEW_MESSAGE_GETITEMRECT) { if (_listview_checkitem((listview_s*)_obj_pOwner(pObj), wParam)) _listview_lprectfromiitem(pObj, (listview_s*)_obj_pOwner(pObj), wParam, (pObj->dwStyle_ & LISTVIEW_STYLE_HORIZONTALLIST) == LISTVIEW_STYLE_HORIZONTALLIST, (RECT*)lParam); }
        else if (uMsg == LISTVIEW_MESSAGE_GETSELECTEDCOUNT) { return ((listview_s*)_obj_pOwner(pObj))->count_selects_; }
        else if (uMsg == LISTVIEW_MESSAGE_GETSELECTIONMARK) { return ((listview_s*)_obj_pOwner(pObj))->index_select_; }
        else if (uMsg == LISTVIEW_MESSAGE_SETSELECTIONMARK) { return _listview_reselect(hWnd, hObj, pObj, (listview_s*)_obj_pOwner(pObj), lParam, wParam != 0); }
        else if (uMsg == LISTVIEW_MESSAGE_GETTOPINDEX) { return ((listview_s*)_obj_pOwner(pObj))->index_start_; }
        else if (uMsg == LISTVIEW_MESSAGE_GETHOTITEM) { return ((listview_s*)_obj_pOwner(pObj))->index_mouse_; }
        else if (uMsg == LISTVIEW_MESSAGE_REDRAWITEMS) { for (INT i = wParam; i <= lParam; i++) _listview_redrawitem(pObj, (listview_s*)_obj_pOwner(pObj), i); return 0; }
        else if (uMsg == LISTVIEW_MESSAGE_CALCITEMSIZE) { _obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0); }
        else if (uMsg == LISTVIEW_MESSAGE_CANCELTHEME) { pObj->crChecked_ = wParam; }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// fix: 核心修复 - 原代码直接__set_int覆盖状态，导致悬浮/禁用状态丢失，改为增量修改
void _listview_lvm_setitemstate(obj_s* pObj, listview_s* pOwner, INT iItem, INT dwState)
{
    if (!_listview_checkitem(pOwner, iItem) || !pOwner->lpItems_) return;
    LPVOID lpItems = pOwner->lpItems_;
    INT oldState = _listview_getitemstate(lpItems, iItem);

    // 先清除所有状态位，再设置新状态
    __set_int(lpItems, (iItem - 1) * 4, 0);
    if (dwState & STATE_SELECT) __add(lpItems, (iItem - 1) * 4, STATE_SELECT);
    if (dwState & STATE_HOVER) __add(lpItems, (iItem - 1) * 4, STATE_HOVER);
    if (dwState & STATE_DISABLE) __add(lpItems, (iItem - 1) * 4, STATE_DISABLE);

    if (iItem == pOwner->index_select_ && (dwState & STATE_SELECT) == 0) pOwner->index_select_ = 0;
    _listview_redrawitem(pObj, pOwner, iItem);
}