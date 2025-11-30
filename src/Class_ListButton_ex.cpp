#include "stdafx.h"

void _listbuttonex_register()
{
    DWORD cbObjExtra = 5 * sizeof(size_t);
    Ex_ObjRegister(L"Menubar", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT, cbObjExtra,
        NULL, 1, _listbuttonex_proc);
    Ex_ObjRegister(L"Toolbar", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT,
        7 * sizeof(size_t), NULL, 2, _listbuttonex_proc);
    Ex_ObjRegister(L"Statusbar", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT,
        7 * sizeof(size_t), NULL, 3, _listbuttonex_proc);
}

LRESULT CALLBACK _listbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    if (uMsg == WM_CREATE) {
        INT    nType = 0;
        obj_s* pObj = nullptr;
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            nType = pObj->pCls_->dwFlags;
        }
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_TYPE, nType);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_CTCS, 0);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_INDEX, -1);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_HIMAGELIST, 0);
        array_s* hArr = Array_Create(0);
        if (hArr) {
            Array_SetExtra(hArr, hObj);
            Array_SetType(hArr, nType);
            Array_BindEvent(hArr, ARRAY_EVENT_DELMEMBER, _listbuttonex_arr_del);
            Ex_ObjSetLong(hObj, LISTBUTTON_LONG_ITEMINFO, (size_t)hArr);
        }
    }
    else if (uMsg == WM_DESTROY) {
        Array_Destroy((array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO));
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_HIMAGELIST);
        if (hImageList) {
            _imglist_destroy(hImageList);
        }
    }
    else if (uMsg == WM_SIZE) {
        _listbuttonex_update(hObj);
    }
    else if (uMsg == WM_PAINT) {
        _listbuttonex_paint(hObj);
    }
    else if (uMsg == WM_MOUSEMOVE) {
        INT nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        if ((nType == 1 || nType == 2) && wParam != WM_MOUSEMOVE)
            _listbuttonex_mousemove(hObj, nType, lParam);
        if (wParam == WM_MOUSEMOVE)
            Ex_ObjDispatchMessage(hObj, LISTBUTTON_MESSAGE_SELECTITEM, 0, lParam);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        INT nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        if (nType == 1 || nType == 2) {
            _listbuttonex_mousedown(hObj, nType, lParam);
        }
    }
    else if (uMsg == WM_LBUTTONUP) {
        INT nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        if (nType == 2) {
            _listbuttonex_mouseup(hObj, nType, lParam);
        }
    }
    else if (uMsg == WM_MOUSELEAVE) {
        INT nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        if (nType == 1 || nType == 2) {
            _listbuttonex_mouseleave(hObj, nType);
        }

    }
    else if (uMsg == LISTVIEW_MESSAGE_INSERTITEM) {
        INT                     nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        EX_LISTBUTTON_ITEMINFO* pItemInfo = (EX_LISTBUTTON_ITEMINFO*)lParam;

        INT                     nCount = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_CTCS);
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        EX_LISTBUTTON_ITEMINFO* pTR =
            (EX_LISTBUTTON_ITEMINFO*)Ex_MemAlloc(sizeof(EX_LISTBUTTON_ITEMINFO));
        if (pItemInfo->wzText) {
            pTR->wzText = StrDupW(pItemInfo->wzText);
        }
        else {
            pTR->wzText = NULL;
        }
        if (nType == 1)   // 菜单条
        {
            pTR->nType = 1;
            pTR->nMenu = pItemInfo->nMenu;
            pTR->nWidth = _listbuttonex_itemWidth(hObj, nType, 0, pTR->wzText);
        }
        else if (nType == 2)   // 工具条
        {
            pTR->nType = pItemInfo->nType;
            pTR->nImage = pItemInfo->nImage;
            if (pItemInfo->wzTips) {
                pTR->wzTips = StrDupW(pItemInfo->wzTips);
            }
            else {
                pTR->wzTips = NULL;
            }
            pTR->dwState = pItemInfo->dwState;
            UINT nWidth = Ex_Scale(11);
            if (pTR->nType > 0) {
                nWidth = _listbuttonex_itemWidth(hObj, nType, pTR->nImage, pTR->wzText);
            }
            pTR->nWidth = nWidth;
        }
        else if (nType == 3)   // 状态条
        {
            pTR->TextFormat = pItemInfo->TextFormat;
            pTR->nWidth = pItemInfo->nWidth;
        }
        nCount = Array_AddMember(hArr, (size_t)pTR, (size_t)pItemInfo->nIndex);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_CTCS, Array_GetCount(hArr));

        if (wParam != 0) {
            _listbuttonex_update(hObj);
        }
        return nCount;
    }
    else if (uMsg == LISTVIEW_MESSAGE_GETITEM) {
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        INT      nCount = Array_GetCount(hArr);
        if ((INT)wParam < 0 || (INT)wParam > nCount) return 0;

        EX_LISTBUTTON_ITEMINFO* pTR =
            (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, (size_t)wParam);
        if (pTR && lParam) {
            EX_LISTBUTTON_ITEMINFO* pItemInfo = (EX_LISTBUTTON_ITEMINFO*)lParam;
            pItemInfo->nType = pTR->nType;
            pItemInfo->nImage = pTR->nImage;
            pItemInfo->nIndex = pTR->nIndex;
            pItemInfo->wzText = pTR->wzText;
            pItemInfo->wzTips = pTR->wzTips;
            pItemInfo->dwState = pTR->dwState;
            pItemInfo->nMenu = pTR->nMenu;
            pItemInfo->TextFormat = pTR->TextFormat;
            return 1;
        }
        return 0;
    }
    else if (uMsg == LISTVIEW_MESSAGE_SETITEM) {
        INT      nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        INT      nCount = Array_GetCount(hArr);
        INT      nIndex = ((EX_LISTBUTTON_ITEMINFO*)lParam)->nIndex;
        if (nIndex < 0 || nIndex > nCount) return 0;

        EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, nIndex);
        if (pTR && lParam) {
            EX_LISTBUTTON_ITEMINFO* pItemInfo = (EX_LISTBUTTON_ITEMINFO*)lParam;
            if (pItemInfo->dwMask > 0) {
                if ((pItemInfo->dwMask & 1) == 1) {
                    pTR->nImage = pItemInfo->nImage;
                }
                if ((pItemInfo->dwMask & 2) == 2) {
                    if (lstrlenW(pTR->wzText) > 0) {
                        Ex_MemFree((LPVOID)pTR->wzText);
                    }
                    if (pItemInfo->wzText) {
                        pTR->wzText = StrDupW(pItemInfo->wzText);
                    }
                    else {
                        pTR->wzText = NULL;
                    }
                }
                if ((pItemInfo->dwMask & 4) == 4) {
                    if (lstrlenW(pTR->wzTips) > 0) {
                        Ex_MemFree((LPVOID)pTR->wzTips);
                    }
                    if (pItemInfo->wzTips) {
                        pTR->wzTips = StrDupW(pItemInfo->wzTips);
                    }
                    else {
                        pTR->wzTips = NULL;
                    }
                }
                if ((pItemInfo->dwMask & 8) == 8) {
                    pTR->dwState = pItemInfo->dwState;
                }
                if ((pItemInfo->dwMask & 16) == 16) {
                    pTR->nMenu = pItemInfo->nMenu;
                }
                if ((pItemInfo->dwMask & 32) == 32) {
                    pTR->TextFormat = pItemInfo->TextFormat;
                }
                if ((pItemInfo->dwMask & 64) == 64) {
                    if (nType == 3) {
                        pTR->nWidth = pItemInfo->nWidth;
                    }
                }

                if (nType < 3) {
                    if ((pItemInfo->dwMask & 1) == 1 && (pItemInfo->dwMask & 2) == 2) {
                        pTR->nWidth =
                            _listbuttonex_itemWidth(hObj, nType, pTR->nImage, pTR->wzText);
                    }
                }
                if (wParam != 0) {
                    _listbuttonex_update(hObj);
                }

                return 1;
            }
        }
        return 0;
    }
    else if (uMsg == LISTVIEW_MESSAGE_DELETEITEM) {
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        INT      nCount = Array_GetCount(hArr);
        INT      nIndex = lParam;
        if (nIndex < 0 || nIndex > nCount) return 0;

        if (Array_DelMember(hArr, nIndex) == TRUE) {
            Ex_ObjSetLong(hObj, LISTBUTTON_LONG_CTCS, nCount - 1);
            if (wParam != 0) {
                _listbuttonex_update(hObj);
            }
            return 1;
        }
        return 0;
    }
    else if (uMsg == TREEVIEW_MESSAGE_UPDATE) {
        // _listbuttonex_update(hObj);
    }
    else if (uMsg == LISTVIEW_MESSAGE_SETIMAGELIST) {
        INT imgWidth = 0;
        INT imgHeight = 0;
        _imglist_size((HEXIMAGELIST)lParam, &imgWidth, &imgHeight);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_HIMAGELIST, lParam);
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_HIMAGWIDTH, Ex_Scale(imgWidth));
        Ex_ObjSetLong(hObj, LISTBUTTON_LONG_HIMAGHEIGHT, Ex_Scale(imgHeight));
    }
    else if (uMsg == LISTBUTTON_MESSAGE_DOWNITEM) {
        INT    nError = 0;
        obj_s* pObj = nullptr;
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)lParam;//wParam
            if (Ex_MenuIsMenu(pTR->nMenu))
            {
                Ex_MenuEnd();
                RECT lpRect = { 0 };
                GetWindowRect(hWnd, &lpRect);
                INT Pos_x = lpRect.left + pObj->w_left_ + pTR->nLeft * g_Li.DpiX;
                INT Pos_y = lpRect.top + pObj->w_bottom_ + 2;
                //RECT RC = { Pos_x, lpRect.top + pObj->w_top_, Pos_x + (LONG)(pTR->nWidth * g_Li.DpiX), lpRect.top + pObj->w_bottom_ };
                Ex_TrackPopupMenu(pTR->nMenu, 0, Pos_x, Pos_y, 0, hObj, NULL);
            }
        }
        return 0;
    }
    else if (uMsg == LISTBUTTON_MESSAGE_SELECTITEM) {
        POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        INT      Index = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_INDEX);
        INT      Heatindex = _listbuttonex_hittest(hObj, hArr, point);
        if (Heatindex > 0) {
            if (Heatindex != Index) {
                _listbuttonex_recovery(hObj, 1, hArr, Index, FALSE);
                EX_LISTBUTTON_ITEMINFO* pTR =
                    (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
                pTR->dwState = STATE_DOWN;
                Ex_ObjSetLong(hObj, LISTBUTTON_LONG_INDEX, Heatindex);
                Ex_ObjInvalidateRect(hObj, 0);
                Ex_MenuEnd();
                Ex_ObjPostMessage(hObj, LISTBUTTON_MESSAGE_DOWNITEM, 0,
                    (LPARAM)pTR);
            }
        }
        return 0;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _listbuttonex_arr_del(array_s* hArr, INT nIndex, EX_LISTBUTTON_ITEMINFO* pvData, INT nType)
{
    //HEXOBJ hObj = Array_GetExtra(hArr);
    if (nType == 1)   // 菜单条
    {
        if (pvData->nMenu) {
            Ex_MenuDestroy(pvData->nMenu);
            pvData->nMenu = 0;
        }
    }
    else if (nType == 2)   // 工具条
    {
        if (lstrlenW(pvData->wzTips) > 0) {
            Ex_MemFree((LPVOID)pvData->wzTips);
        }
    }
    if (lstrlenW(pvData->wzText) > 0) {
        Ex_MemFree((LPVOID)pvData->wzText);
    }
    Ex_MemFree(pvData);
}

INT _listbuttonex_itemWidth(HEXOBJ hObj, INT nType, UINT nImage, LPCWSTR wzText)
{
    INT    nError = 0;
    FLOAT  iWidth = 0;
    FLOAT  iHeight = 0;
    INT    nWidth = 0;
    obj_s* pObj = nullptr;

    if (nType == 1) {
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            _canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, wzText, -1, 0, pObj->c_right_,
                pObj->c_bottom_, &iWidth, &iHeight);
            nWidth = (UINT)iWidth + Ex_Scale(15);
        }
    }
    else if (nType == 2) {
        if (nImage > 0) {
            nWidth = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_HIMAGWIDTH) + Ex_Scale(10);
        }
        if (wzText != NULL) {
            if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                _canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, wzText, -1, 0,
                    pObj->c_right_, pObj->c_bottom_, &iWidth, &iHeight);
                nWidth += (UINT)iWidth + Ex_Scale(10);
            }
        }
    }
    return nWidth;
}

void _listbuttonex_paint(HEXOBJ hObj)
{
    INT            nType = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_TYPE);
    INT            nCount = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_CTCS);
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        if (nCount > 0) {
            if (nType == 1) {
                HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
                for (INT i = 1; i <= nCount; i++) {
                    EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(
                        (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO), i);
                    if (pTR->dwState > 0) {
                        if (pTR->dwState == STATE_FOCUS) {
                            _brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER));
                        }
                        else {
                            _brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN));
                        }
                        _canvas_fillrect(ps.hCanvas, hBrush, pTR->nLeft, 0,
                            pTR->nLeft + pTR->nWidth, ps.uHeight);
                    }

                    _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), pTR->wzText, -1,
                        DT_CENTER | DT_VCENTER, pTR->nLeft, 0,
                        pTR->nLeft + pTR->nWidth, ps.uHeight);
                }
                _brush_destroy(hBrush);
            }
            else if (nType == 2) {
                HEXIMAGELIST hImageList =
                    (HEXIMAGELIST)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_HIMAGELIST);
                INT      hImageWidth = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_HIMAGWIDTH);
                INT      hImageHeight = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_HIMAGHEIGHT);
                INT      hImageTop = (ps.uHeight - hImageHeight) / 2;
                HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));
                for (INT i = 1; i <= nCount; i++) {
                    EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(
                        (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO), i);
                    if (pTR->nType == 0) {
                        _brush_setcolor(hBrush, ExRGB2ARGB(8421504, 255));
                        _canvas_drawrect(ps.hCanvas, hBrush, pTR->nLeft + Ex_Scale(6), Ex_Scale(2), pTR->nLeft + Ex_Scale(6),
                            ps.uHeight - Ex_Scale(4), Ex_Scale(1), D2D1_DASH_STYLE_SOLID);
                    }
                    else {
                        if (pTR->dwState == STATE_FOCUS) {
                            _brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER));
                            _canvas_fillrect(ps.hCanvas, hBrush, pTR->nLeft, 0,
                                pTR->nLeft + pTR->nWidth, ps.uHeight);
                        }
                        else if (pTR->dwState == STATE_DOWN) {
                            _brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN));
                            _canvas_fillrect(ps.hCanvas, hBrush, pTR->nLeft, 0,
                                pTR->nLeft + pTR->nWidth, ps.uHeight);
                        }
                        UINT nLeft = pTR->nLeft;
                        UINT nWidth = pTR->nWidth;
                        if (pTR->nImage > 0) {
                            HEXIMAGE hImage = _imglist_get(hImageList, pTR->nImage);
                            if (hImage != 0) {
                                _canvas_drawimagerectrect(ps.hCanvas, hImage, nLeft + Ex_Scale(5), hImageTop,
                                    nLeft + Ex_Scale(5) + hImageWidth,
                                    hImageTop + hImageHeight, 0, 0, hImageWidth,
                                    hImageHeight,
                                    (pTR->dwState == STATE_DISABLE) ? 128 : 255);
                                nLeft += Ex_Scale(5) + hImageWidth;
                                nWidth -= hImageWidth;
                            }
                        }

                        nLeft += Ex_Scale(5);
                        nWidth -= Ex_Scale(10);

                        if (pTR->wzText != NULL) {
                            EXARGB crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
                            if (pTR->dwState == STATE_DISABLE) {   // 禁止
                                crText = ExRGB2ARGB(crText, 128);
                            }
                            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText, pTR->wzText,
                                -1, DT_VCENTER | DT_SINGLELINE, nLeft, 0,
                                nLeft + nWidth, ps.uHeight);
                        }
                    }
                }
                _brush_destroy(hBrush);
            }
            else if (nType == 3) {
                HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BORDER));
                for (INT i = 1; i <= nCount; i++) {
                    EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(
                        (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO), i);
                    _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                        Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), pTR->wzText, -1,
                        DT_VCENTER | DT_SINGLELINE | pTR->TextFormat, pTR->nLeft + Ex_Scale(5),
                        0, pTR->nLeft + pTR->nWidth - Ex_Scale(10), ps.uHeight);
                    if (i < nCount) {
                        _canvas_drawline(ps.hCanvas, hBrush, pTR->nLeft + pTR->nWidth, 0,
                            pTR->nLeft + pTR->nWidth, ps.uHeight, 1,
                            D2D1_DASH_STYLE_SOLID);
                    }
                }
                _brush_destroy(hBrush);
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}

INT _listbuttonex_hittest(HEXOBJ hObj, array_s* hArr, POINT pt)
{
    INT Index = 0;
    INT nCount = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_CTCS);
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        for (INT i = 1; i <= nCount; i++) {
            EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, i);
            RECT rc{ (LONG)(pTR->nLeft * g_Li.DpiX), pObj->c_top_, (LONG)((pTR->nLeft + pTR->nWidth) * g_Li.DpiX), pObj->c_bottom_ };
            if (PtInRect(&rc, pt))
            {
                Index = i;
                break;
            }
        }
    }
    return Index;
}

void _listbuttonex_recovery(HEXOBJ hObj, INT nType, array_s* hArr, INT Index, BOOL Update)
{
    if (Index > 0) {
        Ex_ObjTooltipsPop(hObj, 0);
        EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Index);
        if (nType == 1) {
            pTR->dwState = STATE_NORMAL;
        }
        else if (nType == 2) {
            if (pTR->dwState == STATE_FOCUS) {
                pTR->dwState = STATE_NORMAL;
            }
            else {
                return;
            }
        }
        if (Update) {
            Ex_ObjSetLong(hObj, LISTBUTTON_LONG_INDEX, -1);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
}

void _listbuttonex_mousemove(HEXOBJ hObj, INT nType, LPARAM lParam)
{
    POINT    pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
    INT      Index = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_INDEX);
    INT      Heatindex = _listbuttonex_hittest(hObj, hArr, pt);

    if (nType == 1)   // 菜单条
    {

        if (Heatindex > 0) {
            if (Heatindex != Index) {
                if (Index > 0) {
                    _listbuttonex_recovery(hObj, nType, hArr, Index, FALSE);
                }

                EX_LISTBUTTON_ITEMINFO* pTR =
                    (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
                pTR->dwState = STATE_FOCUS;
                Ex_ObjSetLong(hObj, LISTBUTTON_LONG_INDEX, Heatindex);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        else {
            _listbuttonex_recovery(hObj, nType, hArr, Index, TRUE);
        }
    }
    else if (nType == 2)   // 工具条
    {
        if (Heatindex > 0) {
            if (Heatindex != Index) {
                EX_LISTBUTTON_ITEMINFO* pTR;
                if (Index > 0) {
                    pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Index);
                    if (pTR->nType > 0 && pTR->dwState == STATE_FOCUS) {
                        _listbuttonex_recovery(hObj, nType, hArr, Index, FALSE);
                    }
                }
                pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
                if (pTR->nType == 0) {
                    Ex_ObjTooltipsPop(hObj, 0);
                }
                Ex_ObjSetLong(hObj, LISTBUTTON_LONG_INDEX, Heatindex);
                if (pTR->nType > 0 && pTR->dwState == STATE_NORMAL) {
                    pTR->dwState = STATE_FOCUS;
                }
                Ex_ObjInvalidateRect(hObj, 0);

                if (pTR->wzTips != NULL) {
                    Ex_ObjTooltipsPop(hObj, pTR->wzTips);
                }
            }
        }
        else {
            _listbuttonex_recovery(hObj, nType, hArr, Index, TRUE);
        }
    }
}

void _listbuttonex_mousedown(HEXOBJ hObj, INT nType, LPARAM lParam)
{
    POINT    pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
    INT      Heatindex = _listbuttonex_hittest(hObj, hArr, pt);

    if (Heatindex > 0) {
        if (nType == 1)   // 菜单条
        {
            EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
            if (pTR->dwState == STATE_DOWN) {
                Ex_MenuEnd();
                pTR->dwState = STATE_FOCUS;
                Ex_ObjInvalidateRect(hObj, 0);
                return;
            }
            pTR->dwState = STATE_DOWN;
            Ex_ObjInvalidateRect(hObj, 0);

            Ex_ObjPostMessage(hObj, LISTBUTTON_MESSAGE_DOWNITEM, 0, (LPARAM)pTR);
        }
        else if (nType == 2)   // 工具条
        {
            EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
            if (pTR->nType > 0 && pTR->dwState != STATE_DISABLE) {
                if (pTR->nType == 1)   // 普通
                {
                    pTR->dwState = STATE_DOWN;
                    Ex_ObjInvalidateRect(hObj, 0);
                    Ex_ObjDispatchNotify(hObj, LISTBUTTON_EVENT_CLICK, (WPARAM)Heatindex, 0);
                }
                else if (pTR->nType == 2)   // 选择
                {
                    if (pTR->dwState == STATE_FOCUS)   // 焦点
                    {
                        pTR->dwState = STATE_DOWN;
                    }
                    else if (pTR->dwState == STATE_DOWN)   // 选中
                    {
                        pTR->dwState = STATE_FOCUS;
                    }
                    Ex_ObjInvalidateRect(hObj, 0);
                    Ex_ObjDispatchNotify(hObj, LISTBUTTON_EVENT_CHECK, (WPARAM)Heatindex,
                        pTR->dwState);
                }
            }
        }
    }
}

void _listbuttonex_mouseup(HEXOBJ hObj, INT nType, LPARAM lParam)
{
    POINT    pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
    INT      Heatindex = _listbuttonex_hittest(hObj, hArr, pt);

    if (Heatindex > 0) {
        EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, Heatindex);
        if (pTR->nType > 0 && pTR->dwState != STATE_DISABLE) {
            // 普通
            if (pTR->nType == 1) {
                pTR->dwState = STATE_FOCUS;
                Ex_ObjInvalidateRect(hObj, 0);
            }
            // 选择
            else if (pTR->nType == 2) {}
        }
    }
}

void _listbuttonex_mouseleave(HEXOBJ hObj, INT nType)
{
    array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
    INT      Index = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_INDEX);
    if (Index > 0) {
        _listbuttonex_recovery(hObj, nType, hArr, Index, TRUE);
    }
}

void _listbuttonex_update(HEXOBJ hObj)
{
    INT nLeft = 0;
    INT nCount = Ex_ObjGetLong(hObj, LISTBUTTON_LONG_CTCS);
    if (nCount > 0) {
        array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, LISTBUTTON_LONG_ITEMINFO);
        for (INT i = 1; i <= nCount; i++) {
            EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)Array_GetMember(hArr, i);
            if (i == 1) {
                pTR->nLeft = 0;
            }
            else {
                pTR->nLeft = nLeft;
            }
            nLeft += pTR->nWidth;
        }
    }
    Ex_ObjInvalidateRect(hObj, 0);
}

