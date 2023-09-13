#include "Layout_ex.h"

BOOL _layout_register(INT nType, LayoutPROC lpfnLayoutProc)
{
    BOOL ret = FALSE;
    if (!HashTable_IsExist(g_Li.hTableLayout, nType))
    {
        ret = HashTable_Set(g_Li.hTableLayout, nType, (size_t)lpfnLayoutProc);
    }
    return ret;
}

BOOL _layout_unregister(INT nType)
{
    BOOL ret = FALSE;
    if (!HashTable_IsExist(g_Li.hTableLayout, nType))
    {
        ret = HashTable_Remove(g_Li.hTableLayout, nType);
    }
    return ret;
}

void _layout_free_info(array_s *hArr, INT nIndex, LPVOID pvItem, INT nType)
{
    layout_s *pLayout = (layout_s *)Array_GetExtra(hArr);
    ((LayoutPROC)pLayout->lpfnProc_)(pLayout, LAYOUT_EVENT_UNINITCHILDPROPS, __get(pvItem, 0), (size_t)pvItem);
    Ex_MemFree((LPVOID)((size_t)pvItem - 4 * sizeof(size_t)));
}

HEXLAYOUT _layout_create(INT nType, EXHANDLE hObjBind)
{
    HEXLAYOUT hLayout = 0;
    INT nError = 0;
    if (nType > 0 && hObjBind != 0)
    {
        size_t lpfnProc = 0;
        HashTable_Get(g_Li.hTableLayout, nType, &lpfnProc);
        if (lpfnProc != 0)
        {
            layout_s *pLayout = (layout_s *)Ex_MemAlloc(sizeof(layout_s));
            obj_s *pObj = nullptr;
            if (pLayout != 0)
            {
                if (_handle_validate(hObjBind, HT_OBJECT, (LPVOID *)&pObj, &nError))
                {
                    pLayout->nBindType_ = HT_OBJECT;
                }
                else if (_handle_validate(hObjBind, HT_DUI, (LPVOID *)&pObj, &nError))
                {
                    pLayout->nBindType_ = HT_DUI;
                }
                else
                {
                    nError = ERROR_EX_INVALID_OBJECT;
                }
                if (nError == 0)
                {
                    pLayout->nType_ = nType;
                    pLayout->fUpdateable_ = TRUE;
                    pLayout->lpfnProc_ = (LPVOID)lpfnProc;
                    pLayout->hBind_ = hObjBind;
                    INT nCount = ((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_GETPROPSCOUNT, NULL, NULL);
                    LPVOID pInfo = (LPVOID)((size_t)Ex_MemAlloc((nCount + (size_t)4) * sizeof(size_t)) + (size_t)4 * sizeof(size_t)); //padding
                    pLayout->lpLayoutInfo_ = pInfo;
                    ((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_INITPROPS, NULL, (size_t)pInfo);
                    nCount = ((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_GETCHILDPROPCOUNT, NULL, NULL);
                    pLayout->cbInfoLen_ = (nCount + 5) * sizeof(size_t); // Margin4/hObjChild1
                    array_s *hArr = Array_Create(0);
                    Array_BindEvent(hArr, ARRAY_EVENT_DELMEMBER, &_layout_free_info);
                    Array_SetExtra(hArr, (size_t)pLayout);
                    pLayout->hArrChildrenInfo_ = hArr;
                    hLayout = _handle_create(HT_LAYOUT, pLayout, &nError);
                    pLayout->hLayout_ = hLayout;
                    ((obj_s *)pObj)->hLayout_ = hLayout;
                }
                else
                {
                    Ex_MemFree(pLayout);
                }
            }
            else
            {
                nError = ERROR_EX_MEMORY_ALLOC;
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_INVALID;
        }
    }
    else
    {
        nError = ERROR_EX_LAYOUT_INVALID;
    }
    Ex_SetLastError(nError);
    return hLayout;
}

HEXLAYOUT _layout_get_parent_layout(HEXOBJ hObj)
{
    INT nError = 0;
    obj_s *pObj = nullptr;
    HEXLAYOUT hLayoutParent = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj, &nError))
    {
        EXHANDLE hObj = pObj->objParent_;
        if (hObj == 0)
        {
            wnd_s *pWnd = pObj->pWnd_;
            hObj = pWnd->hexdui_;
        }
        hLayoutParent = Ex_ObjLayoutGet(hObj);
    }
    return hLayoutParent;
}

BOOL _layout_destroy(HEXLAYOUT hLayout)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        LPVOID lpfnProc = pLayout->lpfnProc_;
        ((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_UNINITPROPS, 0, (size_t)pLayout->lpLayoutInfo_);
        Array_Destroy(pLayout->hArrChildrenInfo_);
        Ex_MemFree((LPVOID)((size_t)pLayout->lpLayoutInfo_ - 4 * sizeof(size_t)));
        Ex_MemFree(pLayout);
        nError = 0;
        _handle_destroy(hLayout, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_enum_find_obj(LPVOID hArr, INT nIndex, LPVOID pvItem, INT nType, size_t pvParam)
{
    return (__get(pvItem, 0) == pvParam);
}

LPVOID _layout_get_child(layout_s *pLayout, HEXOBJ hObj)
{
    EXHANDLE hObjP = pLayout->hBind_;
    HEXDUI hExDUI = 0;
    LPVOID pInfo = nullptr;
    if (Ex_ObjGetParentEx(hObj, &hExDUI) == hObjP || hExDUI == hObjP)
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        INT nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
        if (nIndex > 0)
        {
            pInfo = (LPVOID)Array_GetMember(hArr, nIndex);
        }
    }
    return pInfo;
}

BOOL _layout_update(HEXLAYOUT hLayout)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (pLayout->fUpdateable_)
        {
            LPVOID lpfnProc = pLayout->lpfnProc_;
            ((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_UPDATE, pLayout->hBind_, 0);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

INT _layout_gettype(HEXLAYOUT hLayout)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    INT nType = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        nType = pLayout->nType_;
    }
    Ex_SetLastError(nError);
    return nType;
}

BOOL _layout_enableupdate(HEXLAYOUT hLayout, BOOL fUpdateable)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    INT nType = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        pLayout->fUpdateable_ = fUpdateable;
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

LRESULT _layout_notify(HEXLAYOUT hLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    LRESULT ret = 1;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (pLayout->fUpdateable_)
        {
            LPVOID lpfnProc = pLayout->lpfnProc_;
            ret = ((LayoutPROC)lpfnProc)(pLayout, nEvent, (size_t)wParam, (size_t)lParam);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL _layout_table_setinfo(HEXLAYOUT hLayout, INT *aRowHeight, INT cRows, INT *aCellWidth, INT cCells)
{
    INT nError = 0;
    layout_s *pLayout = nullptr;
    INT ret = 1;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (pLayout->nType_ == LAYOUT_TYPE_TABLE)
        {
            LPVOID pInfo = pLayout->lpLayoutInfo_;
            array_s *hArr = (array_s *)__get(pInfo, (LAYOUT_PROP_TABLE_ARRAY_ROW - 1) * sizeof(LPVOID));
            Array_Redefine(hArr, cRows, FALSE);
            for (INT i = 1; i <= cRows; i++)
            {
                Array_SetMember(hArr, i, aRowHeight[i - 1]);
            }
            hArr = (array_s *)__get(pInfo, (LAYOUT_PROP_TABLE_ARRAY_CELL - 1) * sizeof(LPVOID));
            Array_Redefine(hArr, cCells, FALSE);
            for (INT i = 1; i <= cCells; i++)
            {
                Array_SetMember(hArr, i, aCellWidth[i - 1]);
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_INVALID;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_setchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID, size_t pvValue)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (hObj != 0)
        {
            array_s *hArr = pLayout->hArrChildrenInfo_;
            size_t nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
            LPVOID pInfo = nullptr;
            if (nIndex == 0)
            {
                pInfo = Ex_MemAlloc(pLayout->cbInfoLen_);
                if (pInfo != 0)
                {
                    pInfo = (LPVOID)((size_t)pInfo + 4 * sizeof(size_t));
                    __set(pInfo, 0, hObj);
                    ((LayoutPROC)pLayout->lpfnProc_)(pLayout, LAYOUT_EVENT_INITCHILDPROPS, hObj, (size_t)pInfo);
                    nIndex = Array_AddMember(hArr, (size_t)pInfo);
                }
            }
            else
            {
                pInfo = (LPVOID)Array_GetMember(hArr, nIndex);
            }
            if (pInfo != 0)
            {

                if (((LayoutPROC)pLayout->lpfnProc_)(pLayout, LAYOUT_EVENT_CHECKCHILDPROPVALUE, MAKELONG(nIndex, dwPropID), pvValue) == 0)
                {

                    __set(pInfo, dwPropID * sizeof(size_t), pvValue);
                }
            }
            else
            {
                nError = ERROR_EX_MEMORY_ALLOC;
            }
        }
        else
        {
            nError = ERROR_EX_INVALID_OBJECT;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_getchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID, size_t *pvValue)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        size_t nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
        LPVOID pInfo = nullptr;
        if (nIndex > 0)
        {
            pInfo = (LPVOID)Array_GetMember(hArr, nIndex);
        }
        if (pInfo != 0)
        {
            dwPropID = dwPropID * sizeof(LPVOID);
            if (dwPropID >= -16 && dwPropID < pLayout->cbInfoLen_)
            {
                *pvValue = __get(pInfo, dwPropID);
            }
            else
            {
                nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_INVALID;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_setprop(HEXLAYOUT hLayout, INT dwPropID, size_t pvValue)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        LPVOID pInfo = pLayout->lpLayoutInfo_;
        LPVOID lpfnProc = pLayout->lpfnProc_;
        if (((LayoutPROC)lpfnProc)(pLayout, LAYOUT_EVENT_CHECKCHILDPROPVALUE, dwPropID, pvValue) == 0)
        {
            if (dwPropID > 0)
            {
                dwPropID = dwPropID - 1;
            }
            __set(pInfo, dwPropID * sizeof(size_t), pvValue);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

size_t _layout_getprop(HEXLAYOUT hLayout, INT dwPropID)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    size_t ret = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        LPVOID pInfo = pLayout->lpLayoutInfo_;
        if (dwPropID > 0)
        {
            dwPropID = dwPropID - 1;
        }
        ret = __get(pInfo, dwPropID * sizeof(size_t));
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL _layout_absolute_setedge(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT dwEdge, INT dwType, size_t nValue)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (pLayout->nType_ == LAYOUT_TYPE_ABSOLUTE)
        {
            dwEdge = (dwEdge + 1) / 2;
            if (dwEdge >= 1 && dwEdge <= 8)
            {
                _layout_setchildprop(hLayout, hObjChild, dwEdge * 2, dwType);
                _layout_setchildprop(hLayout, hObjChild, dwEdge * 2 - 1, nValue);
            }
            else
            {
                nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _layout_move_margin(HEXOBJ hObj, RECT *lpObjRc, LPVOID lpMargin, INT dwLockFlags, INT dwOrgFlags)
{
    RECT rcObj = *lpObjRc;
    RECT rcMargin{0};
    rcMargin.left = __get(lpMargin, 3 * sizeof(size_t));
    rcMargin.top = __get(lpMargin, 2 * sizeof(size_t));
    rcMargin.right = __get(lpMargin, sizeof(size_t));
    rcMargin.bottom = __get(lpMargin, 0);

    rcObj.left = rcObj.left + rcMargin.left;
    if ((dwLockFlags & 1) == 0)
    {
        rcObj.right = rcObj.right + rcMargin.left;
        lpObjRc->right = lpObjRc->right + rcMargin.left;
    }

    rcObj.top = rcObj.top + rcMargin.top;

    if ((dwLockFlags & 2) == 0)
    {
        rcObj.bottom = rcObj.bottom + rcMargin.top;
        lpObjRc->bottom = lpObjRc->bottom + rcMargin.top;
    }

    if ((dwLockFlags & 4) == 4)
    {
        rcObj.right = rcObj.right - rcMargin.right;
    }
    else
    {
        lpObjRc->right = lpObjRc->right + rcMargin.right;
    }

    if ((dwLockFlags & 8) == 8)
    {
        rcObj.bottom = rcObj.bottom - rcMargin.bottom;
    }
    else
    {
        lpObjRc->bottom = lpObjRc->bottom + rcMargin.bottom;
    }

    if (dwOrgFlags == 15)
    {
        return;
    }

    rcObj.right = rcObj.right - rcObj.left;
    rcObj.bottom = rcObj.bottom - rcObj.top;
    if ((dwOrgFlags & 1) != 0)
    {
        rcObj.left = OBJECT_POSITION_DEFAULT;
    }
    if ((dwOrgFlags & 2) != 0)
    {
        rcObj.top = OBJECT_POSITION_DEFAULT;
    }
    if ((dwOrgFlags & 4) != 0)
    {
        rcObj.right = OBJECT_POSITION_DEFAULT;
    }
    if ((dwOrgFlags & 8) != 0)
    {
        rcObj.bottom = OBJECT_POSITION_DEFAULT;
    }

    Ex_ObjMove(hObj, rcObj.left, rcObj.top, rcObj.right, rcObj.bottom, FALSE);
}

LRESULT CALLBACK __layout_linear_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        return 2;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        return 2;
    }
    else if (nEvent == LAYOUT_EVENT_INITCHILDPROPS)
    {
        __set((LPVOID)lParam, LAYOUT_SUBPROP_LINEAR_SIZE * sizeof(size_t), -1);
    }
    else if (nEvent == LAYOUT_EVENT_CHECKCHILDPROPVALUE)
    {
        INT nSize = HIWORD(wParam);
        if (nSize == LAYOUT_EVENT_CHECKCHILDPROPVALUE)
        {
            return (lParam < LAYOUT_SUBPROP_LINEAR_ALIGN_FILL || lParam > LAYOUT_SUBPROP_LINEAR_ALIGN_RIGHT_BOTTOM);
        }
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        RECT rcClient{0};
        if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
        {
            Ex_ObjGetClientRect(wParam, &rcClient);
        }
        else
        {
            Ex_DUIGetClientRect(wParam, &rcClient);
        }
        LPVOID pInfo = ((layout_s *)pLayout)->lpLayoutInfo_;
        array_s *hArr = ((layout_s *)pLayout)->hArrChildrenInfo_;
        INT nDAlign = __get(pInfo, (LAYOUT_PROP_LINEAR_ALIGN - 1) * sizeof(size_t));
        BOOL fVertical = __get(pInfo, (LAYOUT_PROP_LINEAR_DIRECTION - 1) * sizeof(size_t)) == LAYOUT_PROP_DIRECTION_V;
        rcClient.left = rcClient.left + __get(pInfo, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
        rcClient.top = rcClient.top + __get(pInfo, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
        rcClient.right = rcClient.right - __get(pInfo, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
        rcClient.bottom = rcClient.bottom - __get(pInfo, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
        SIZE szClient;
        szClient.cx = rcClient.right - rcClient.left;
        szClient.cy = rcClient.bottom - rcClient.top;
        RECT rcObj;
        rcObj.left = rcClient.left;
        rcObj.top = rcClient.top;
        std::vector<RECT> arrRect;
        std::vector<INT> arrOrg;
        if (nDAlign != 0 && Array_GetCount(hArr) > 0)
        {
            arrRect.resize(Array_GetCount(hArr));
            arrOrg.resize(Array_GetCount(hArr));
        }
        for (INT i = 1; i <= Array_GetCount(hArr); i++)
        {
            INT orgFlags = 0;
            LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
            HEXOBJ hObj = __get(pInfo, 0);
            if (hObj == 0)
                continue;
            INT nSize = __get(pInfo, LAYOUT_SUBPROP_LINEAR_SIZE * sizeof(size_t));
            RECT rcTmp{0};
            Ex_ObjGetRect(hObj, &rcTmp);
            INT w = rcTmp.right - rcTmp.left;
            INT h = rcTmp.bottom - rcTmp.top;
            if (nSize < 0)
            {
                if (fVertical)
                {
                    nSize = h;
                }
                else
                {
                    nSize = w;
                }
            }
            INT nFill = __get(pInfo, LAYOUT_SUBPROP_LINEAR_ALIGN * sizeof(size_t));
            orgFlags = 0;
            if (fVertical)
            {
                if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_LEFT_TOP)
                {
                    rcObj.left = rcClient.left;
                    rcObj.right = rcObj.left + w;
                }
                else if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER)
                {
                    rcObj.left = rcClient.left + (szClient.cx - w) / 2;
                    rcObj.right = rcObj.left + w;
                    orgFlags = 4;
                }
                else if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_RIGHT_BOTTOM)
                {
                    rcObj.right = rcClient.right;
                    rcObj.left = rcClient.right - w;
                }
                else
                {
                    rcObj.left = rcClient.left;
                    rcObj.right = rcClient.right;
                }
                rcObj.bottom = rcObj.top + nSize;
            }
            else
            {
                if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_LEFT_TOP)
                {
                    rcObj.top = rcClient.top;
                    rcObj.bottom = rcObj.top + rcObj.bottom - rcTmp.top;
                }
                else if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER)
                {
                    rcObj.top = rcClient.top + (szClient.cy - h) / 2;
                    rcObj.bottom = rcObj.top + h;
                    orgFlags = 8;
                }
                else if (nFill == LAYOUT_SUBPROP_LINEAR_ALIGN_RIGHT_BOTTOM)
                {
                    rcObj.bottom = rcClient.bottom;
                    rcObj.top = rcClient.bottom - (rcTmp.bottom - rcTmp.top);
                }
                else
                {
                    rcObj.top = rcClient.top;
                    rcObj.bottom = rcClient.bottom;
                }
                rcObj.right = rcObj.left + nSize;
            }
            if (nDAlign == 0)
            {
                _layout_move_margin(hObj, &rcObj, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), fVertical ? 5 : 10, orgFlags);
            }
            else
            {
                rcObj.left = rcObj.left + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                rcObj.top = rcObj.top + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t));
                rcObj.right = rcObj.right + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t));
                rcObj.bottom = rcObj.bottom + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                arrRect[i - 1] = rcObj;
                arrOrg[i - 1] = orgFlags;
            }
            if (fVertical)
            {
                rcObj.top = rcObj.bottom;
            }
            else
            {
                rcObj.left = rcObj.right;
            }
        }
        if (Array_GetCount(hArr) > 0 && arrRect.size() > 0)
        {
            LPVOID pInfo = ((layout_s *)pLayout)->lpLayoutInfo_;
            INT nDAlign = __get(pInfo, (LAYOUT_PROP_LINEAR_ALIGN - 1) * sizeof(size_t));
            INT w = 0;
            INT h = 0;
            if (fVertical)
            {
                INT nSize = arrRect[arrRect.size() - 1].bottom - arrRect[0].top;
                h = 5;
                if (nDAlign == 2) //bottom
                {
                    w = rcClient.bottom - nSize - arrRect[0].top;
                }
                else if (nDAlign == 1) //CENTER
                {
                    w = rcClient.top + (rcClient.bottom - rcClient.top - nSize) / 2 - arrRect[0].top;
                }
                else
                {
                    w = 0;
                }
            }
            else
            {
                INT nSize = arrRect[arrRect.size() - 1].right - arrRect[0].left;
                h = 10;
                if (nDAlign == 2) //right
                {
                    w = rcClient.right - nSize - arrRect[0].left;
                }
                else if (nDAlign == 1) //center
                {
                    w = rcClient.left + (rcClient.right - rcClient.top - nSize) / 2 - arrRect[0].left;
                }
                else
                {
                    w = 0;
                }
            }
            for (INT i = 1; i <= Array_GetCount(hArr); i++)
            {
                LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
                RECT rcObj = arrRect[i - 1];
                if (fVertical)
                {
                    OffsetRect(&rcObj, 0, w);
                }
                else
                {
                    OffsetRect(&rcObj, w, 0);
                }
                _layout_move_margin(__get(pInfo, 0), &rcObj, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 15, arrOrg[i - 1]);
            }
        }
    }
    return 0;
}

LRESULT CALLBACK __layout_flow_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        return 1;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        return 2;
    }
    else if (nEvent == LAYOUT_EVENT_INITCHILDPROPS)
    {
        __set((LPVOID)lParam, LAYOUT_SUBPROP_FLOW_SIZE * sizeof(size_t), -1);
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        RECT rcClient{0};
        if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
        {
            Ex_ObjGetClientRect(wParam, &rcClient);
        }
        else
        {
            Ex_DUIGetClientRect(wParam, &rcClient);
        }
        LPVOID pInfoa = ((layout_s *)pLayout)->lpLayoutInfo_;
        array_s *hArr = ((layout_s *)pLayout)->hArrChildrenInfo_;
        BOOL fVertical = __get(pInfoa, (LAYOUT_PROP_LINEAR_DIRECTION - 1) * sizeof(size_t)) == LAYOUT_PROP_DIRECTION_V;
        rcClient.left = rcClient.left + __get(pInfoa, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
        rcClient.top = rcClient.top + __get(pInfoa, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
        rcClient.right = rcClient.right - __get(pInfoa, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
        rcClient.bottom = rcClient.bottom - __get(pInfoa, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
        RECT rcObj{0};
        rcObj.left = rcClient.left;
        rcObj.top = rcClient.top;

        INT nMaxSize = 0;
        for (INT i = 1; i <= Array_GetCount(hArr); i++)
        {
            LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
            HEXOBJ hObj = (HEXOBJ)__get(pInfo, 0);
            if (hObj == 0 || !Ex_ObjIsVisible(hObj))
                continue;
            INT nSize = __get(pInfo, LAYOUT_SUBPROP_LINEAR_SIZE * sizeof(size_t));
            RECT rcTmp{0};
            Ex_ObjGetRect(hObj, &rcTmp);
            if (nSize < 0)
            {
                if (fVertical)
                {
                    nSize = rcTmp.bottom - rcTmp.top;
                }
                else
                {
                    nSize = rcTmp.right - rcTmp.left;
                }
            }
            if (fVertical)
            {
                rcObj.right = rcObj.left + rcTmp.right - rcTmp.left;
                rcObj.bottom = rcObj.top + nSize;
                if (rcObj.bottom > rcClient.bottom || __get(pInfo, LAYOUT_SUBPROP_FLOW_NEW_LINE * sizeof(size_t)) != 0)
                {
                    rcObj.top = rcClient.top;
                    rcObj.bottom = rcObj.top + nSize;
                    rcObj.left = rcObj.left + nMaxSize;
                    rcObj.right = rcObj.left + rcTmp.right - rcTmp.left;
                    nMaxSize = 0;
                }
            }
            else
            {
                rcObj.bottom = rcObj.top + rcTmp.bottom - rcTmp.top;
                rcObj.right = rcObj.left + nSize;
                if (rcObj.right > rcClient.right || __get(pInfo, LAYOUT_SUBPROP_FLOW_NEW_LINE * sizeof(size_t)) != 0)
                {
                    rcObj.left = rcClient.left;
                    rcObj.right = rcObj.left + nSize;
                    rcObj.top = rcObj.top + nMaxSize;
                    rcObj.bottom = rcObj.top + rcTmp.bottom - rcTmp.top;
                    nMaxSize = 0;
                }
            }
            _layout_move_margin(hObj, &rcObj, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 0, 0);
            if (fVertical)
            {
                if (rcObj.right - rcObj.left > nMaxSize)
                {
                    nMaxSize = rcObj.right - rcObj.left;
                }
                rcObj.top = rcObj.bottom;
            }
            else
            {
                if (rcObj.bottom - rcObj.top > nMaxSize)
                {
                    nMaxSize = rcObj.bottom - rcObj.top;
                }
                rcObj.left = rcObj.right;
            }
        }
    }
    return 0;
}

LRESULT CALLBACK __layout_page_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        return 1;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        return 1;
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        RECT rcClient{0};
        if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
        {
            Ex_ObjGetClientRect(wParam, &rcClient);
        }
        else
        {
            Ex_DUIGetClientRect(wParam, &rcClient);
        }
        LPVOID pInfoa = ((layout_s *)pLayout)->lpLayoutInfo_;
        array_s *hArr = ((layout_s *)pLayout)->hArrChildrenInfo_;
        BOOL fVertical = __get(pInfoa, (LAYOUT_PROP_LINEAR_DIRECTION - 1) * sizeof(size_t)) == LAYOUT_PROP_DIRECTION_V;
        rcClient.left = rcClient.left + __get(pInfoa, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
        rcClient.top = rcClient.top + __get(pInfoa, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
        rcClient.right = rcClient.right - __get(pInfoa, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
        rcClient.bottom = rcClient.bottom - __get(pInfoa, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
        INT nCurrentPage = __get(pInfoa, (LAYOUT_PROP_PAGE_CURRENT - 1) * sizeof(size_t));
        for (INT i = 1; i <= Array_GetCount(hArr); i++)
        {
            LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
            HEXOBJ hObj = (HEXOBJ)__get(pInfo, 0);
            if (hObj == 0)
                continue;
            if (i == nCurrentPage)
            {
                if (__get(pInfo, LAYOUT_SUBPROP_PAGE_FILL * sizeof(size_t)) != 0)
                {
                    _layout_move_margin(hObj, &rcClient, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 15, 0);
                }
            }
            Ex_ObjShow(hObj, i == nCurrentPage);
        }
    }
    return 0;
}

LRESULT CALLBACK __layout_table_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        //' layout_rows = 行数 = 类似 "100,200,50"
        //' layout_cells = 列数 = 类似 "100,200,50"
        //' LAYOUT_ROW_ARRAY = 正数为像素,负数为百分比
        //' LAYOUT_CELL_ARRAY
        return 2;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        /*' layout_row = 行号
		' layout_cell = 列号
		' layout_rowspan = 跨行数
		' layout_cellspan = 跨列数
		' layout_fill = 是否填充,否则放左上角*/
        return 5;
    }
    else if (nEvent == LAYOUT_EVENT_INITPROPS)
    {
        __set((LPVOID)lParam, (LAYOUT_PROP_TABLE_ARRAY_ROW - 1) * sizeof(size_t), (size_t)Array_Create(0));
        __set((LPVOID)lParam, (LAYOUT_PROP_TABLE_ARRAY_CELL - 1) * sizeof(size_t), (size_t)Array_Create(0));
    }
    else if (nEvent == LAYOUT_EVENT_INITCHILDPROPS)
    {
        __set((LPVOID)lParam, LAYOUT_SUBPROP_TABLE_ROW_SPAN * sizeof(size_t), 1);
        __set((LPVOID)lParam, LAYOUT_SUBPROP_TABLE_CELL_SPAN * sizeof(size_t), 1);
    }
    else if (nEvent == LAYOUT_EVENT_UNINITPROPS)
    {
        Array_Destroy((array_s *)__get((LPVOID)lParam, (LAYOUT_PROP_TABLE_ARRAY_ROW - 1) * sizeof(size_t)));
        Array_Destroy((array_s *)__get((LPVOID)lParam, (LAYOUT_PROP_TABLE_ARRAY_CELL - 1) * sizeof(size_t)));
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        RECT rcClient{0};
        if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
        {
            Ex_ObjGetClientRect(wParam, &rcClient);
        }
        else
        {
            Ex_DUIGetClientRect(wParam, &rcClient);
        }
        LPVOID pInfoa = ((layout_s *)pLayout)->lpLayoutInfo_;
        array_s *hArr = ((layout_s *)pLayout)->hArrChildrenInfo_;
        array_s *hArrRows = (array_s *)__get(pInfoa, (LAYOUT_PROP_TABLE_ARRAY_ROW - 1) * sizeof(size_t));
        array_s *hArrCells = (array_s *)__get(pInfoa, (LAYOUT_PROP_TABLE_ARRAY_CELL - 1) * sizeof(size_t));

        rcClient.left = rcClient.left + __get(pInfoa, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
        rcClient.top = rcClient.top + __get(pInfoa, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
        rcClient.right = rcClient.right - __get(pInfoa, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
        rcClient.bottom = rcClient.bottom - __get(pInfoa, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
        std::vector<std::vector<RECT>> aRects;
        RECT rcTmp{0};
        if (Array_GetCount(hArrRows) > 0 && Array_GetCount(hArrCells) > 0)
        {
            aRects.resize(Array_GetCount(hArrRows));
            for (INT index = 0; index < aRects.size(); index++)
            {
                aRects[index].resize(Array_GetCount(hArrCells));
            }
            rcTmp.top = rcClient.top;
            for (INT i = 1; i <= Array_GetCount(hArrRows); i++)
            {
                rcTmp.left = rcClient.left;
                rcTmp.bottom = Array_GetMember(hArrRows, i);
                if (rcTmp.bottom < 0)
                {
                    rcTmp.bottom = (rcClient.bottom - rcClient.top) * (FLOAT)abs(rcTmp.bottom) / 100;
                }
                for (INT j = 1; j <= Array_GetCount(hArrCells); j++)
                {
                    aRects[i - 1][j - 1].left = rcTmp.left;
                    aRects[i - 1][j - 1].top = rcTmp.top;
                    rcTmp.right = Array_GetMember(hArrCells, j);
                    if (rcTmp.right < 0)
                    {
                        rcTmp.right = (rcClient.right - rcClient.left) * (FLOAT)abs(rcTmp.right) / 100;
                    }
                    aRects[i - 1][j - 1].right = aRects[i - 1][j - 1].left + rcTmp.right;
                    aRects[i - 1][j - 1].bottom = aRects[i - 1][j - 1].top + rcTmp.bottom;
                    rcTmp.left = rcTmp.left + rcTmp.right;
                }
                rcTmp.top = rcTmp.top + rcTmp.bottom;
            }
        }
        else
        {
            Ex_SetLastError(ERROR_EX_LAYOUT_INVALID);
            return -1;
        }
        for (INT i = 1; i <= Array_GetCount(hArr); i++)
        {
            LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
            HEXOBJ hObj = __get(pInfo, 0);
            if (hObj == 0)
                continue;
            rcTmp.left = __get(pInfo, LAYOUT_SUBPROP_TABLE_CELL * sizeof(size_t));
            rcTmp.top = __get(pInfo, LAYOUT_SUBPROP_TABLE_ROW * sizeof(size_t));
            rcTmp.right = __get(pInfo, LAYOUT_SUBPROP_TABLE_CELL_SPAN * sizeof(size_t)) - 1;
            if (rcTmp.right <= 0)
            {
                rcTmp.right = rcTmp.left;
            }
            else
            {
                rcTmp.right = rcTmp.left + rcTmp.right;
            }
            rcTmp.bottom = __get(pInfo, LAYOUT_SUBPROP_TABLE_ROW_SPAN * sizeof(size_t)) - 1;
            if (rcTmp.bottom <= 0)
            {
                rcTmp.bottom = rcTmp.top;
            }
            else
            {
                rcTmp.bottom = rcTmp.top + rcTmp.bottom;
            }
            if (rcTmp.left <= 0)
            {
                rcTmp.left = 1;
            }
            if (rcTmp.top <= 0)
            {
                rcTmp.top = 1;
            }
            if (rcTmp.left > Array_GetCount(hArrCells))
            {
                rcTmp.left = Array_GetCount(hArrCells);
            }
            if (rcTmp.top > Array_GetCount(hArrRows))
            {
                rcTmp.top = Array_GetCount(hArrRows);
            }
            if (rcTmp.right < rcTmp.left)
            {
                rcTmp.right = rcTmp.left;
            }
            if (rcTmp.bottom < rcTmp.top)
            {
                rcTmp.bottom = rcTmp.top;
            }
            if (rcTmp.right > Array_GetCount(hArrCells))
            {
                rcTmp.right = Array_GetCount(hArrCells);
            }
            if (rcTmp.bottom > Array_GetCount(hArrRows))
            {
                rcTmp.bottom = Array_GetCount(hArrRows);
            }
            rcClient.left = aRects[rcTmp.top - 1][rcTmp.left - 1].left;
            rcClient.top = aRects[rcTmp.top - 1][rcTmp.left - 1].top;
            rcClient.right = aRects[rcTmp.bottom - 1][rcTmp.right - 1].right;
            rcClient.bottom = aRects[rcTmp.bottom - 1][rcTmp.right - 1].bottom;
            _layout_move_margin(hObj, &rcClient, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 15, 0);
        }
    }
    return 0;
}

LRESULT CALLBACK __layout_relative_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        return 0;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        return 10;
    }
    else if (nEvent == LAYOUT_EVENT_CHECKCHILDPROPVALUE)
    {
        if (Ex_ObjIsValidate(lParam) && LOWORD(wParam) > 0)
        {
            array_s *pChildrenInfo = ((layout_s *)pLayout)->hArrChildrenInfo_;
            LPVOID pInfo = (LPVOID)Array_GetMember(pChildrenInfo, LOWORD(wParam));
            HEXOBJ hObj = __get(pInfo, 0);
            INT dwPropID = HIWORD(wParam);
            LPVOID pInfoOther = _layout_get_child((layout_s *)pLayout, lParam);
            BOOL nRet = FALSE;

            if (hObj != 0 && dwPropID > 0 && pInfoOther != 0)
            {
                if (dwPropID == LAYOUT_SUBPROP_RELATIVE_LEFT_OF)
                {
                    nRet = __get(pInfoOther, LAYOUT_SUBPROP_RELATIVE_RIGHT_OF * sizeof(size_t)) != hObj;
                }
                else if (dwPropID == LAYOUT_SUBPROP_RELATIVE_RIGHT_OF)
                {
                    nRet = __get(pInfoOther, LAYOUT_SUBPROP_RELATIVE_LEFT_OF * sizeof(size_t)) != hObj;
                }
                else if (dwPropID == LAYOUT_SUBPROP_RELATIVE_TOP_OF)
                {
                    nRet = __get(pInfoOther, LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF * sizeof(size_t)) != hObj;
                }
                else if (dwPropID == LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF)
                {
                    nRet = __get(pInfoOther, LAYOUT_SUBPROP_RELATIVE_TOP_OF * sizeof(size_t)) != hObj;
                }
                else
                {

                    nRet = __get(pInfoOther, dwPropID * sizeof(size_t)) != hObj;
                }
            }
            return nRet == TRUE ? FALSE : TRUE;
        }
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        _layout_relative_update((layout_s *)pLayout, ((layout_s *)pLayout)->lpLayoutInfo_, ((layout_s *)pLayout)->hArrChildrenInfo_, lParam);
    }
    return 0;
}

void _layout_relative_update(layout_s *pLayout, LPVOID pLayoutInfo, array_s *hArrObjs, LPARAM lParam)
{
    INT Prime = GetNearestPrime(Array_GetCount(hArrObjs));
    EX_HASHTABLE *hHashPosInfos = HashTable_Create(Prime, pfnDefaultFreeData);
    EXHANDLE hObjParent = ((layout_s *)pLayout)->hBind_;
    RECT rcClient{0};
    if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
    {
        Ex_ObjGetClientRect(hObjParent, &rcClient);
    }
    else
    {
        Ex_DUIGetClientRect(hObjParent, &rcClient);
    }
    rcClient.left = rcClient.left + __get(pLayoutInfo, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
    rcClient.top = rcClient.top + __get(pLayoutInfo, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
    rcClient.right = rcClient.right - __get(pLayoutInfo, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
    rcClient.bottom = rcClient.bottom - __get(pLayoutInfo, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
    std::vector<INT> Infos(3);
    for (INT i = 1; i <= Array_GetCount(hArrObjs); i++)
    {
        LPVOID pInfo = (LPVOID)Array_GetMember(hArrObjs, i);
        HEXOBJ hObj = (HEXOBJ)__get(pInfo, 0);
        if (hObj == 0)
            continue;
        RECT rcObj{0};
        Ex_ObjGetRect(hObj, &rcObj);

        LPVOID pPosInfo = Ex_MemAlloc(4 * 3 * sizeof(size_t) + sizeof(size_t) + sizeof(size_t)); //多一个放pInfo和orgFlags
        //[是否确定/类型/句柄或坐标]*4个边界,确定以后一定是坐标
        //处理无法确定四边的情况
        BOOL fNoPosInfoH = TRUE;
        for (INT j = 1; j <= 10; j += 2)
        {
            if (__get(pInfo, j * sizeof(size_t)) != 0)
            {
                fNoPosInfoH = FALSE;
                break;
            }
        }
        BOOL fNoPosInfoV = TRUE;
        for (INT j = 2; j <= 10; j += 2)
        {
            if (__get(pInfo, j * sizeof(size_t)) != 0)
            {
                fNoPosInfoV = FALSE;
                break;
            }
        }

        if (fNoPosInfoH)
        {
            __set(pPosInfo, 0, 1);
            __set(pPosInfo, sizeof(size_t), 0);
            __set(pPosInfo, 2 * sizeof(size_t), rcObj.left + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t)));
            __set((LPVOID)((size_t)pPosInfo + 6 * sizeof(size_t)), 0, 1);
            __set((LPVOID)((size_t)pPosInfo + 6 * sizeof(size_t)), sizeof(size_t), 0);
            __set((LPVOID)((size_t)pPosInfo + 6 * sizeof(size_t)), 2 * sizeof(size_t), rcObj.right + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t)));
        }
        if (fNoPosInfoV)
        {

            __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 0, 1);
            __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), sizeof(size_t), 0);
            __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 2 * sizeof(size_t), rcObj.top + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t)));

            __set((LPVOID)((size_t)pPosInfo + 9 * sizeof(size_t)), 0, 1);
            __set((LPVOID)((size_t)pPosInfo + 9 * sizeof(size_t)), sizeof(size_t), 0);
            __set((LPVOID)((size_t)pPosInfo + 9 * sizeof(size_t)), 2 * sizeof(size_t), rcObj.bottom + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t)));
        }
        //rcObj暂时按左顶宽高处理
        rcObj.right = rcObj.right - rcObj.left;
        rcObj.bottom = rcObj.bottom - rcObj.top;
        INT orgFlags = 0;
        //预处理边界定位信息

        HEXDUI hExDUI = 0;
        if (fNoPosInfoH == FALSE)
        { //左边界处理
            Infos[0] = 1;
            if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF * sizeof(size_t)) != 0)
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF * sizeof(size_t));
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF;
                HEXDUI hExDUI = 0;
                if (dwValue == -1) //相对父
                {
                    Infos[2] = rcClient.left + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                }
                else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.right;
                }
            }
            else if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H * sizeof(size_t)) != 0)
            {
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H;
                Infos[2] = rcClient.left + abs(rcClient.right - rcClient.left - rcObj.right) / 2 + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                orgFlags = orgFlags | 4;
            }
            else
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_RIGHT_OF * sizeof(size_t));
                HEXDUI hExDUI = 0;
                if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[1] = LAYOUT_SUBPROP_RELATIVE_RIGHT_OF;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.right;
                }
            }
            __set(pPosInfo, 0, Infos[0]);
            __set(pPosInfo, sizeof(size_t), Infos[1]);
            __set(pPosInfo, 2 * sizeof(size_t), Infos[2]);
            //右边界处理
            Infos[0] = 1;

            if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF * sizeof(size_t)) != 0)
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF * sizeof(size_t));
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF;
                HEXDUI hExDUI = 0;
                if (dwValue == -1) //相对父
                {
                    Infos[2] = rcClient.right + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                }
                else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.right;
                }
            }
            else if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H * sizeof(size_t)) != 0)
            {
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H;
                Infos[2] = rcClient.left + abs(rcClient.right - rcClient.left + rcObj.right) / 2 + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t));
                orgFlags = orgFlags | 4;
            }
            else
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_LEFT_OF * sizeof(size_t));
                HEXDUI hExDUI = 0;
                if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[1] = LAYOUT_SUBPROP_RELATIVE_LEFT_OF;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.right;
                }
            }
            __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 0, Infos[0]);
            __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
            __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);
        }
        if (fNoPosInfoV == FALSE)
        {
            //上边界处理
            Infos[0] = 1;
            if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF * sizeof(size_t)) != 0)
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF * sizeof(size_t));
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF;
                HEXDUI hExDUI = 0;
                if (dwValue == -1) //相对父
                {
                    Infos[2] = rcClient.top;
                }
                else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.bottom;
                }
            }
            else if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V * sizeof(size_t)) != 0)
            {
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V;
                Infos[2] = rcClient.top + abs(rcClient.bottom - rcClient.top - rcObj.bottom) / 2 + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t));
                orgFlags = orgFlags | 8;
            }
            else
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF * sizeof(size_t));
                HEXDUI hExDUI = 0;
                if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {

                    Infos[0] = 0;
                    Infos[1] = LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.bottom;
                }
            }
            __set((LPVOID)((size_t)pPosInfo + 1 * 3 * sizeof(size_t)), 0, Infos[0]);
            __set((LPVOID)((size_t)pPosInfo + 1 * 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
            __set((LPVOID)((size_t)pPosInfo + 1 * 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);

            //下边界处理
            Infos[0] = 1;
            if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF * sizeof(size_t)) != 0)
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF * sizeof(size_t));
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF;
                HEXDUI hExDUI = 0;
                if (dwValue == -1) //相对父
                {
                    Infos[2] = rcClient.bottom + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t)) - __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                }
                else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.bottom;
                }
            }
            else if (__get(pInfo, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V * sizeof(size_t)) != 0)
            {
                Infos[1] = LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_V;
                Infos[2] = rcClient.top + abs(rcClient.bottom - rcClient.top + rcObj.bottom) / 2 + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                orgFlags = orgFlags | 8;
            }
            else
            {
                INT dwValue = __get(pInfo, LAYOUT_SUBPROP_RELATIVE_TOP_OF * sizeof(size_t));
                HEXDUI hExDUI = 0;
                if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hExDUI)) || hObjParent == hExDUI) //同层组件
                {
                    Infos[0] = 0;
                    Infos[1] = LAYOUT_SUBPROP_RELATIVE_TOP_OF;
                    Infos[2] = dwValue;
                }
                else
                {
                    Infos[0] = 0;
                    Infos[1] = 0;
                    Infos[2] = rcObj.bottom;
                }
            }
            __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 0, Infos[0]);
            __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
            __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);
        }
        __set(pPosInfo, 12 * sizeof(size_t), (size_t)pInfo);
        __set(pPosInfo, 12 * sizeof(size_t) + sizeof(size_t), orgFlags);
        HashTable_Set(hHashPosInfos, hObj, (size_t)pPosInfo);
    }

    for (INT index = 0; index < 5; index++)
    {
        INT cNoLockObj = HashTable_GetCounts(hHashPosInfos);
        for (INT i = 1; i <= Array_GetCount(hArrObjs); i++)
        {
            LPVOID pInfo = (LPVOID)Array_GetMember(hArrObjs, i);
            HEXOBJ hObj = __get(pInfo, 0);
            LPVOID pPosInfo = nullptr;
            HashTable_Get(hHashPosInfos, hObj, (size_t *)&pPosInfo);
            if (pPosInfo != 0)
            {
                //找能确定的点
                if (__get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 0), 0) == 0)
                {
                    Infos[0] = __get(pPosInfo, 0);
                    Infos[1] = __get(pPosInfo, sizeof(size_t));
                    Infos[2] = __get(pPosInfo, 2 * sizeof(size_t));

                    if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_RIGHT_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 2 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 2 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 0 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 0 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == 0)
                    {
                        if (__get(pPosInfo, 2 * 3 * sizeof(size_t)) != 0) //已经锁定了
                        {
                            Infos[2] = __get(pPosInfo, 2 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) - Infos[2] - __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t)) - __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t));
                            Infos[0] = 1;
                        }
                    }
                    __set(pPosInfo, 0, Infos[0]);
                    __set(pPosInfo, sizeof(size_t), Infos[1]);
                    __set(pPosInfo, 2 * sizeof(size_t), Infos[2]);
                }
                if (__get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 1), 0) == 0)
                {
                    Infos[0] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 0);
                    Infos[1] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), sizeof(size_t));
                    Infos[2] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 2 * sizeof(size_t));

                    if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 3 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 3 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 1 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 1 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == 0)
                    {
                        if (__get(pPosInfo, 3 * 3 * sizeof(size_t)) != 0) //已经锁定了
                        {
                            Infos[2] = __get(pPosInfo, 3 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) - Infos[2] - __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t)) - __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t));
                            Infos[0] = 1;
                        }
                    }
                    __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 0, Infos[0]);
                    __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
                    __set((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);
                }
                if (__get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 2), 0) == 0)
                {
                    Infos[0] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 2), 0);
                    Infos[1] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 2), sizeof(size_t));
                    Infos[2] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 2), 2 * sizeof(size_t));

                    if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_LEFT_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 0 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 0 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF)
                    {

                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 2 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 2 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == 0)
                    {
                        if (__get(pPosInfo, 0 * 3 * sizeof(size_t)) != 0) //已经锁定了
                        {
                            Infos[2] = __get(pPosInfo, 0 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + Infos[2] + __get(pInfo, LAYOUT_SUBPROP_MARGIN_LEFT * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_RIGHT * sizeof(size_t));
                            Infos[0] = 1;
                        }
                    }
                    __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 0, Infos[0]);
                    __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
                    __set((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);
                }
                if (__get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 3), 0) == 0)
                {
                    Infos[0] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 3), 0);
                    Infos[1] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 3), sizeof(size_t));
                    Infos[2] = __get((LPVOID)((size_t)pPosInfo + 3 * sizeof(size_t) * 3), 2 * sizeof(size_t));

                    if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_TOP_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 1 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 1 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF)
                    {
                        LPVOID pInfoOther = nullptr;
                        if (HashTable_Get(hHashPosInfos, Infos[2], (size_t *)&pInfoOther))
                        {
                            if (__get(pInfoOther, 3 * 3 * sizeof(size_t)) != 0) //已经锁定了
                            {
                                Infos[2] = __get(pInfoOther, 3 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                                Infos[0] = 1;
                            }
                        }
                    }
                    else if (Infos[1] == 0)
                    {
                        if (__get(pPosInfo, 1 * 3 * sizeof(size_t)) != 0) //已经锁定了
                        {
                            Infos[2] = __get(pPosInfo, 1 * 3 * sizeof(size_t) + 2 * sizeof(size_t)) + Infos[2] + __get(pInfo, LAYOUT_SUBPROP_MARGIN_TOP * sizeof(size_t)) + __get(pInfo, LAYOUT_SUBPROP_MARGIN_BOTTOM * sizeof(size_t));
                            Infos[0] = 1;
                        }
                    }
                    __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 0, Infos[0]);
                    __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), sizeof(size_t), Infos[1]);
                    __set((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 2 * sizeof(size_t), Infos[2]);
                }
                if (__get(pInfo, 0) != 0 && __get(pInfo, 3 * sizeof(size_t)) != 0 && __get(pInfo, 6 * sizeof(size_t)) != 0 && __get(pInfo, 9 * sizeof(size_t)) != 0) //已经确定整个组件了
                {
                    cNoLockObj = cNoLockObj - 1;
                }
            }
            else
            {
                cNoLockObj = cNoLockObj - 1;
            }
        }
        if (cNoLockObj <= 0)
        {
            break;
        }
    }

    std::vector<size_t> pInfos;
    std::vector<size_t> hObjs;
    HashTable_GetAllKeysAndValues(hHashPosInfos, hObjs, pInfos);
    for (size_t i = 0; i < hObjs.size(); i++)
    {
        HEXOBJ hObj = hObjs[i];
        LPVOID pPosInfo = (LPVOID)pInfos[i];
        LPVOID pInfo = (LPVOID)__get(pPosInfo, 12 * sizeof(size_t));
        INT orgFlags = __get(pPosInfo, 12 * sizeof(size_t) + sizeof(size_t));
        RECT rcObj{0};
        Ex_ObjGetRect(hObj, &rcObj);

        RECT rcTmp{0};
        if (__get((LPVOID)((size_t)pPosInfo + 0 * 3 * sizeof(size_t)), 0) == 0)
        {
            rcTmp.left = rcObj.left;
        }
        else
        {
            rcTmp.left = __get((LPVOID)((size_t)pPosInfo + 0 * 3 * sizeof(size_t)), 2 * sizeof(size_t));
        }
        if (__get((LPVOID)((size_t)pPosInfo + 1 * 3 * sizeof(size_t)), 0) == 0)
        {
            rcTmp.top = rcObj.top;
        }
        else
        {
            rcTmp.top = __get((LPVOID)((size_t)pPosInfo + 1 * 3 * sizeof(size_t)), 2 * sizeof(size_t));
        }
        if (__get((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 0) == 0)
        {
            rcTmp.right = rcTmp.left + rcObj.right - rcObj.left;
        }
        else
        {
            rcTmp.right = __get((LPVOID)((size_t)pPosInfo + 2 * 3 * sizeof(size_t)), 2 * sizeof(size_t));
        }
        if (__get((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 0) == 0)
        {
            rcTmp.bottom = rcTmp.top + rcObj.bottom - rcObj.top;
        }
        else
        {
            rcTmp.bottom = __get((LPVOID)((size_t)pPosInfo + 3 * 3 * sizeof(size_t)), 2 * sizeof(size_t));
        }

        _layout_move_margin(hObj, &rcTmp, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 15, orgFlags);
    }
    HashTable_Destroy(hHashPosInfos);
}

LRESULT CALLBACK __layout_absolute_proc(layout_s *pLayout, INT nEvent, WPARAM wParam, LPARAM lParam)
{
    if (nEvent == LAYOUT_EVENT_GETPROPSCOUNT)
    {
        return 0;
    }
    else if (nEvent == LAYOUT_EVENT_GETCHILDPROPCOUNT)
    {
        /*layout_left="10/10%"
			layout_top="10/10%"
			layout_right="10/10%"
			layout_bottom="10/10%"
			layout_width="10/10%"
			layout_height="10/10%"
			layout_offsetH="10/10%/10*"
			layout_offsetV="10/10%/10*"*/
        return 16;
    }
    else if (nEvent == LAYOUT_EVENT_UPDATE)
    {
        RECT rcClient{0};
        if (((layout_s *)pLayout)->nBindType_ == HT_OBJECT)
        {
            Ex_ObjGetClientRect(wParam, &rcClient);
        }
        else
        {
            Ex_DUIGetClientRect(wParam, &rcClient);
        }
        LPVOID pInfoa = ((layout_s *)pLayout)->lpLayoutInfo_;
        array_s *hArr = ((layout_s *)pLayout)->hArrChildrenInfo_;
        rcClient.left = rcClient.left + __get(pInfoa, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
        rcClient.top = rcClient.top + __get(pInfoa, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
        rcClient.right = rcClient.right - __get(pInfoa, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
        rcClient.bottom = rcClient.bottom - __get(pInfoa, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
        SIZE szClient{0};
        szClient.cx = rcClient.right - rcClient.left;
        szClient.cy = rcClient.bottom - rcClient.top;
        for (INT i = 1; i <= Array_GetCount(hArr); i++)
        {
            LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
            HEXOBJ hObj = __get(pInfo, 0);
            if (hObj == 0)
                continue;
            RECT rcTmp{0};
            Ex_ObjGetRect(hObj, &rcTmp);
            INT ary1 = 0;
            INT ary2 = 0;
            INT ary3 = 0;
            INT ary4 = 0;
            INT nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE * sizeof(size_t));
            INT tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_WIDTH * sizeof(size_t));
            SIZE szObj{0};
            if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
            {
                szObj.cx = (FLOAT)tmp / 100 * szClient.cx;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
            {
                szObj.cx = tmp;
            }
            else
            {
                szObj.cx = rcTmp.right - rcTmp.left;
            }
            nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE * sizeof(size_t));
            tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT * sizeof(size_t));
            if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
            {
                szObj.cy = (FLOAT)tmp / 100 * szClient.cy;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
            {
                szObj.cy = tmp;
            }
            else
            {
                szObj.cy = rcTmp.bottom - rcTmp.top;
            }

            RECT rcObj{0};
            for (INT index = 0; index < 5; index++)
            {
                if (ary1 == 0)
                {
                    ary1 = 1;
                    nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE * sizeof(size_t));
                    tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_LEFT * sizeof(size_t));
                    if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
                    {
                        rcObj.left = rcClient.left + (FLOAT)tmp / 100 * szClient.cx;
                    }
                    else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
                    {
                        rcObj.left = rcClient.left + tmp;
                    }
                    else if (ary3 == 1)
                    {
                        rcObj.left = rcObj.right - szObj.cx;
                    }
                    else
                    {
                        ary1 = 0;
                    }
                }
                if (ary2 == 0)
                {
                    ary2 = 1;
                    nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE * sizeof(size_t));
                    tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_TOP * sizeof(size_t));
                    if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
                    {
                        rcObj.top = rcClient.top + (FLOAT)tmp / 100 * szClient.cy;
                    }
                    else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
                    {
                        rcObj.top = rcClient.top + tmp;
                    }
                    else if (ary4 == 1)
                    {
                        rcObj.top = rcObj.bottom - szObj.cy;
                    }
                    else
                    {
                        ary2 = 0;
                    }
                }
                if (ary3 == 0)
                {
                    ary3 = 1;
                    nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE * sizeof(size_t));
                    tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_RIGHT * sizeof(size_t));
                    if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
                    {
                        rcObj.right = rcClient.right - (FLOAT)tmp / 100 * szClient.cx;
                    }
                    else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
                    {
                        rcObj.right = rcClient.right - tmp;
                    }
                    else if (ary1 == 1)
                    {
                        rcObj.right = rcObj.left + szObj.cx;
                    }
                    else
                    {
                        ary3 = 0;
                    }
                }
                if (ary4 == 0)
                {
                    ary4 = 1;
                    nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE * sizeof(size_t));
                    tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM * sizeof(size_t));
                    if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
                    {
                        rcObj.bottom = rcClient.bottom - (FLOAT)tmp / 100 * szClient.cy;
                    }
                    else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
                    {
                        rcObj.bottom = rcClient.bottom - tmp;
                    }
                    else if (ary2 == 1)
                    {
                        rcObj.bottom = rcObj.top + szObj.cy;
                    }
                    else
                    {
                        ary4 = 0;
                    }
                }
                if (ary1 == 1 && ary2 == 1 && ary3 == 1 && ary4 == 1)
                {
                    break;
                }
            }
            if (ary1 == 0)
            {
                rcObj.left = rcTmp.left;
            }
            if (ary2 == 0)
            {
                rcObj.top = rcTmp.top;
            }
            if (ary3 == 0)
            {
                rcObj.right = rcTmp.right;
            }
            if (ary4 == 0)
            {
                rcObj.bottom = rcTmp.bottom;
            }
            nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H_TYPE * sizeof(size_t));
            tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H * sizeof(size_t));
            if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
            {
                rcObj.left = rcObj.left + (FLOAT)tmp / 100 * szClient.cx;
                rcObj.right = rcObj.right + (FLOAT)tmp / 100 * szClient.cx;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
            {
                rcObj.left = rcObj.left + tmp;
                rcObj.right = rcObj.right + tmp;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS)
            {
                rcObj.left = rcObj.left + (FLOAT)tmp / 100 * szObj.cx;
                rcObj.right = rcObj.right + (FLOAT)tmp / 100 * szObj.cx;
            }

            nType = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V_TYPE * sizeof(size_t));
            tmp = __get(pInfo, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V * sizeof(size_t));
            if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS)
            {
                rcObj.top = rcObj.top + (FLOAT)tmp / 100 * szClient.cy;
                rcObj.bottom = rcObj.bottom + (FLOAT)tmp / 100 * szClient.cy;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX)
            {
                rcObj.top = rcObj.top + tmp;
                rcObj.bottom = rcObj.bottom + tmp;
            }
            else if (nType == LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS)
            {
                rcObj.top = rcObj.top + (FLOAT)tmp / 100 * szObj.cy;
                rcObj.bottom = rcObj.bottom + (FLOAT)tmp / 100 * szObj.cy;
            }

            _layout_move_margin(hObj, &rcObj, (LPVOID)((size_t)pInfo - 4 * sizeof(size_t)), 15, 0);
        }
    }
    else if (nEvent == LAYOUT_EVENT_CHECKCHILDPROPVALUE)
    {
        array_s *pChildrenInfo = ((layout_s *)pLayout)->hArrChildrenInfo_;
        LPVOID pInfo = (LPVOID)Array_GetMember(pChildrenInfo, LOWORD(wParam));
        INT nType = HIWORD(wParam);
        if (nType % 2 == 1 && nType >= LAYOUT_SUBPROP_ABSOLUTE_LEFT && nType <= LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V)
        {
            if (__get(pInfo, (nType + 1) * sizeof(size_t)) == LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN)
            {
                __set(pInfo, (nType + 1) * sizeof(size_t), LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
        }
    }
    return 0;
}

BOOL _layout_addchild(HEXLAYOUT hLayout, HEXOBJ hObj)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    LPVOID pInfo = nullptr;
    obj_s *pObj = NULL;
    BOOL fIsChild = FALSE;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        if (hObj != 0)
        {
            if (pLayout->nBindType_ == HT_OBJECT)
            {
                if (Ex_ObjGetParent(hObj) == pLayout->hBind_)
                {
                    fIsChild = TRUE;
                    
                }
            }
            else
            {
                if (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj, &nError))
                {
                    if (pObj->objParent_ == 0 && pObj->pWnd_->hexdui_ == pLayout->hBind_)
                    {
                        fIsChild = TRUE;
                    }
                }
            }

            if (fIsChild)
            {
                array_s *hArr = pLayout->hArrChildrenInfo_;
                if (_layout_get_child(pLayout, hObj) == 0)
                {
                    size_t nIndex = 0;
                    nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
                    if (nIndex == 0)
                    {
                        pInfo = Ex_MemAlloc(pLayout->cbInfoLen_);
                        if (pInfo != 0)
                        {
                            pInfo = (LPVOID)((size_t)pInfo + 4 * sizeof(size_t));
                            __set(pInfo, 0, hObj);
                            ((LayoutPROC)pLayout->lpfnProc_)(pLayout, LAYOUT_EVENT_INITCHILDPROPS, hObj, (size_t)pInfo);
                            nIndex = Array_AddMember(hArr, (size_t)pInfo);
                        }
                        else
                        {
                            nError = ERROR_EX_MEMORY_ALLOC;
                        }
                    }
                }
            }
            else
            {
                nError = ERROR_EX_LAYOUT_NOT_CHILD;
            }
        }
        else
        {
            nError = ERROR_EX_INVALID_OBJECT;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_addchildren(HEXLAYOUT hLayout, BOOL fDesc, EXATOM dwObjClassATOM, INT *nCount)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        EXHANDLE hObj = pLayout->hBind_;
        LPVOID pObj = nullptr;
        if (_handle_validate(hObj, pLayout->nBindType_, (LPVOID *)&pObj, &nError))
        {
            *nCount = 0;
            if (pLayout->nBindType_ == HT_OBJECT)
            {
                hObj = fDesc ? ((obj_s *)pObj)->objChildLast_ : ((obj_s *)pObj)->objChildFirst_;
            }
            else
            {
                hObj = fDesc ? ((wnd_s *)pObj)->objChildLast_ : ((wnd_s *)pObj)->objChildFirst_;
            }
            obj_s *pObj2 = nullptr;
            while (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj2, &nError))
            {
                EXATOM clsAtom = pObj2->pCls_->atomName;
                if ((dwObjClassATOM == 0 || dwObjClassATOM == clsAtom) && clsAtom != ATOM_SYSBUTTON)
                {
                    if (_layout_get_child(pLayout, hObj) == 0)
                    {
                        LPVOID pInfo = Ex_MemAlloc(pLayout->cbInfoLen_);
                        if (pInfo)
                        {
                            pInfo = (LPVOID)((size_t)pInfo + 4 * sizeof(size_t));
                            __set(pInfo, 0, hObj);
                            ((LayoutPROC)pLayout->lpfnProc_)(pLayout, LAYOUT_EVENT_INITCHILDPROPS, hObj, (size_t)pInfo);
                            Array_AddMember(hArr, (size_t)pInfo);
                            *nCount = *nCount + 1;
                        }
                        else
                        {
                            nError = ERROR_EX_MEMORY_ALLOC;
                        }
                    }
                }
                hObj = fDesc ? pObj2->objPrev_ : pObj2->objNext_;
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_deletechildren(HEXLAYOUT hLayout, EXATOM dwObjClassATOM)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        if (hArr != 0)
        {
            if (dwObjClassATOM != 0)
            {
                for (INT i = Array_GetCount(hArr); i >= 1; i--)
                {
                    LPVOID pInfo = (LPVOID)Array_GetMember(hArr, i);
                    obj_s *pObj = nullptr;
                    if (_handle_validate(__get(pInfo, 0), HT_OBJECT, (LPVOID *)&pObj, &nError))
                    {
                        if (dwObjClassATOM == pObj->pCls_->atomName)
                        {
                            Array_DelMember(hArr, i);
                        }
                    }
                }
            }
            else
            {
                Array_Clear(hArr);
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_INVALID;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_deletechild(HEXLAYOUT hLayout, HEXOBJ hObj)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        if (hArr != 0)
        {
            size_t nIndex = Array_Emum(hArr, _layout_enum_find_obj, hObj);
            if (nIndex > 0)
            {
                Array_DelMember(hArr, nIndex);
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_INVALID;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _layout_getchildproplist(HEXLAYOUT hLayout, HEXOBJ hObj, LPVOID *lpProps)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        array_s *hArr = pLayout->hArrChildrenInfo_;
        size_t nIndex = Array_Emum(hArr, _layout_enum_find_obj, hObj);
        if (nIndex > 0)
        {
            *lpProps = (LPVOID)Array_GetMember(hArr, nIndex);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

LPVOID _layout_getproplist(HEXLAYOUT hLayout)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    LPVOID pInfo = nullptr;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        pInfo = pLayout->lpLayoutInfo_;
    }
    Ex_SetLastError(nError);
    return pInfo;
}

BOOL _layout_absolute_lock(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT tLeft, INT tTop, INT tRight, INT tBottom, INT tWidth, INT tHeight)
{
    layout_s *pLayout = nullptr;
    INT nError = 0;
    LPVOID pInfo = nullptr;
    if (_handle_validate(hLayout, HT_LAYOUT, (LPVOID *)&pLayout, &nError))
    {
        RECT rcClient{0};
        if (pLayout->nType_ == LAYOUT_TYPE_ABSOLUTE)
        {
            if (pLayout->nBindType_ == HT_OBJECT)
            {
                Ex_ObjGetClientRect(pLayout->hBind_, &rcClient);
            }
            else
            {
                Ex_DUIGetClientRect(pLayout->hBind_, &rcClient);
            }
            LPVOID pInfo = pLayout->lpLayoutInfo_;
            rcClient.left = rcClient.left + __get(pInfo, LAYOUT_PROP_PADDING_LEFT * sizeof(size_t));
            rcClient.top = rcClient.top + __get(pInfo, LAYOUT_PROP_PADDING_TOP * sizeof(size_t));
            rcClient.right = rcClient.right - __get(pInfo, LAYOUT_PROP_PADDING_RIGHT * sizeof(size_t));
            rcClient.bottom = rcClient.bottom - __get(pInfo, LAYOUT_PROP_PADDING_BOTTOM * sizeof(size_t));
            SIZE szClient{0};
            szClient.cx = rcClient.right - rcClient.left;
            szClient.cy = rcClient.bottom - rcClient.top;
            RECT rcObj{0};
            Ex_ObjGetRect(hObjChild, &rcObj);

            if (tLeft == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT, rcObj.left - rcClient.left);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tLeft == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT, (FLOAT)rcObj.left / szClient.cx * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_LEFT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }

            if (tTop == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP, rcObj.top - rcClient.top);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tTop == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP, (FLOAT)rcObj.top / szClient.cy * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_TOP_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }

            if (tRight == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, rcClient.right - rcObj.right);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tRight == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, (FLOAT)(rcClient.right - rcObj.right) / szClient.cx * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_RIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }

            if (tBottom == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, rcClient.bottom - rcObj.bottom);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tBottom == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, (FLOAT)(rcClient.bottom - rcObj.bottom) / szClient.cy * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }

            if (tWidth == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, rcObj.right - rcObj.left);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tWidth == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, (FLOAT)(rcObj.right - rcObj.left) / szClient.cx * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_WIDTH_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }

            if (tHeight == 1) //数值锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, rcObj.bottom - rcObj.top);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX);
            }
            else if (tHeight == 2) //比例锁
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, (FLOAT)(rcObj.bottom - rcObj.top) / szClient.cy * 100);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS);
            }
            else
            {
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, 0);
                _layout_setchildprop(hLayout, hObjChild, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT_TYPE, LAYOUT_SUBPROP_ABSOLUTE_TYPE_UNKNOWN);
            }
        }
        else
        {
            nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _layout_init()
{
    _layout_register(LAYOUT_TYPE_LINEAR, __layout_linear_proc);
    _layout_register(LAYOUT_TYPE_FLOW, __layout_flow_proc);
    _layout_register(LAYOUT_TYPE_PAGE, __layout_page_proc);
    _layout_register(LAYOUT_TYPE_TABLE, __layout_table_proc);
    _layout_register(LAYOUT_TYPE_RELATIVE, __layout_relative_proc);
    _layout_register(LAYOUT_TYPE_ABSOLUTE, __layout_absolute_proc);
}