#include "stdafx.h"

ClsPROC _tv_lpfn;

EX_TREEVIEW_NODEITEM* _treeview_getlastitem(EX_TREEVIEW_NODEITEM* item)
{
    EX_TREEVIEW_NODEITEM* last = item;
    while (item) {
        last = item;
        item = item->pNext;
    }
    return last;
}

INT _treeview_itemcount(EX_TREEVIEW_NODEITEM* item, INT* pCount)
{
    while (item) {
        (*pCount)++;
        if (item->fExpand) {
            _treeview_itemcount(item->pChildFirst, pCount);
        }
        item = item->pNext;
    }
    return *pCount;
}

void _treeview_calcsize(obj_s* pObj, EX_NMHDR* nm)
{
    RECT* rect;
    rect = (RECT*)nm->lParam;
    if (_obj_getextralong(pObj, TREEVIEW_LONG_LEFT) > rect->left) {
        rect->left = _obj_getextralong(pObj, TREEVIEW_LONG_LEFT);
    }
    rect->top = _obj_getextralong(pObj, TREEVIEW_LONG_ITEMHEIGHT);
}

EX_TREEVIEW_NODEITEM* _treeview_getnextvisibleitem(EX_TREEVIEW_NODEITEM* item)
{
    EX_TREEVIEW_NODEITEM* ret = NULL;
    if (item) {
        if (item->fExpand) {
            ret = item->pChildFirst;
        }
        if (!ret) {
            ret = item->pNext;
            EX_TREEVIEW_NODEITEM* next = item;
            while (!ret) {
                next = next->pParent;
                if (!next) {
                    break;
                }
                if (next->fExpand) {
                    ret = next->pNext;
                }
            }
        }
    }
    return ret;
}

EX_TREEVIEW_NODEITEM* _treeview_getnextitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, INT type)
{
    if (type == TREEVIEW_NODE_CORRELATION_ROOT) {
        return (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM);
    }
    else if (item) {
        if (type == TREEVIEW_NODE_CORRELATION_PREVIOUS) {
            return item->pPrev;
        }
        else if (type == TREEVIEW_NODE_CORRELATION_NEXT) {
            return item->pNext;
        }
        else if (type == TREEVIEW_NODE_CORRELATION_CHILD) {
            return item->pChildFirst;
        }
        else if (type == TREEVIEW_NODE_CORRELATION_PARENT) {
            return item->pParent;
        }
        else if (type == TREEVIEW_NODE_CORRELATION_NEXTVISIBLE) {
            return _treeview_getnextvisibleitem(item);
        }
    }
    return NULL;
}

INT _treeview_getitemindex(obj_s* pObj, EX_TREEVIEW_NODEITEM* item)
{
    INT height = 0;
    if (item) {
        height = 1;
        EX_TREEVIEW_NODEITEM* tmp = item->pPrev;
        while (tmp) {
            height++;
            if (tmp->fExpand) {
                height += tmp->nCountChild;
            }
            tmp = tmp->pPrev;
        }
        tmp = item->pParent;
        if (tmp) {
            if (!tmp->fExpand) return 0;
            height += _treeview_getitemindex(pObj, tmp);
        }
    }
    return height;
}

EX_TREEVIEW_NODEITEM* _treeview_getitembyindex(EX_TREEVIEW_NODEITEM* item, INT index,
    INT* childIndex)
{
    if (index <= 0) {
        return 0;
    }
    if (*childIndex == 0) {
        *childIndex = 1;
    }
    while (item) {
        if (*childIndex == index) {
            return item;
        }
        (*childIndex)++;
        if (item->fExpand) {
            EX_TREEVIEW_NODEITEM* tmp =
                _treeview_getitembyindex(item->pChildFirst, index, childIndex);
            if (tmp) {
                return tmp;
            }
        }
        item = item->pNext;
    }
    return NULL;
}

INT _treeview_getvisiblecount(EX_TREEVIEW_NODEITEM* item, BOOL expand)
{
    INT count = 0;
    while (item) {
        count++;
        if (!expand || item->fExpand) {
            count += _treeview_getvisiblecount(item->pChildFirst, expand);
        }
        item = item->pNext;
    }
    return count;
}

void _treeview_deleteitemlink(obj_s* pObj, EX_TREEVIEW_NODEITEM* item)
{
    if (item) {
        EX_TREEVIEW_NODEITEM* next = item->pNext;
        EX_TREEVIEW_NODEITEM* prev = item->pPrev;
        item->pNext = NULL;
        item->pPrev = NULL;
        if (next) {
            next->pPrev = prev;
        }
        if (prev) {
            prev->pNext = next;
        }
        EX_TREEVIEW_NODEITEM* parent = item->pParent;
        item->pParent = NULL;
        if (!prev) {
            if (parent) {
                parent->pChildFirst = next;
            }
            else if ((EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM) ==
                item) {
                _obj_setextralong(pObj, TREEVIEW_LONG_NODEITEM, (size_t)next);
            }
        }
    }
}

void _treeview_calcitemmaxwidth(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, INT* pWidth)
{
    INT imgWidth = 0;
    if (item) {
        if (_obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST)) {
            _imglist_size((HEXIMAGELIST)_obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST),
                &imgWidth, NULL);
            imgWidth = Ex_Scale(10) + imgWidth;
        }
        if (pObj->canvas_obj_) {
            while (item) {
                if (item->fExpand && item->pChildFirst) {
                    _treeview_calcitemmaxwidth(pObj, item->pChildFirst, pWidth);
                    item = item->pNext;
                }
                else {
                    FLOAT width = 0;
                    if (_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, item->pwzText, -1, 32,
                        0, 0, &width, NULL)) {
                        width += item->nDepth * _obj_getextralong(pObj, TREEVIEW_LONG_INDENT) +
                            imgWidth + Ex_Scale(35);
                        if (FLAGS_CHECK(pObj->dwStyle_, TREEVIEW_STYLE_SHOWADDANDSUB)) {
                            width += Ex_Scale(20);
                        }
                        if (width > *pWidth) {
                            *pWidth = width;
                        }
                    }
                    item = item->pNext;
                }
            }
        }
    }
}

void _treeview_updateitem(obj_s* pObj)
{
    INT                   currentWidth = _obj_getextralong(pObj, TREEVIEW_LONG_LEFT);
    EX_TREEVIEW_NODEITEM* root =
        (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM);

    // 检查root是否为空
    if (!root) {
        // 如果没有数据，设置最小宽度
        INT minWidth = Ex_Scale(50);
        if (currentWidth != minWidth) {
            _obj_setextralong(pObj, TREEVIEW_LONG_LEFT, minWidth);
            _obj_sendmessage(_obj_gethwnd(pObj), pObj->hObj_, pObj, WM_SIZE, 0,
                MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
        }
        return;
    }

    INT width = 0;
    _treeview_calcitemmaxwidth(pObj, root, &width);
    if (currentWidth != width) {
        _obj_setextralong(pObj, TREEVIEW_LONG_LEFT, width);
        _obj_sendmessage(_obj_gethwnd(pObj), pObj->hObj_, pObj, WM_SIZE, 0,
            MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
    }
}

BOOL _treeview_expanditem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item)
{
    BOOL result = FALSE;
    if (item) {
        EX_TREEVIEW_NODEITEM* tmp = item->pParent;
        while (tmp) {
            if (!tmp->fExpand) {
                tmp->fExpand = TRUE;
                result = TRUE;
            }
            tmp = tmp->pParent;
        }
    }
    return result;
}

BOOL _treeview_inititem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, EX_TREEVIEW_NODEITEM* parent,
    EX_TREEVIEW_NODEITEM* insertAfter)
{
    EX_TREEVIEW_NODEITEM* next = NULL;
    EX_TREEVIEW_NODEITEM* prev = NULL;
    if (!item) {
        return FALSE;
    }
    if ((size_t)parent == TREEVIEW_NODE_ROOT || !parent) {
        parent = 0;
    }
    if ((size_t)insertAfter == TREEVIEW_NODE_ROOT) {
        if (parent) {
            next = parent->pChildFirst;
            parent->pChildFirst = item;
        }
        else {
            next = (EX_TREEVIEW_NODEITEM*)_obj_setextralong(pObj, TREEVIEW_LONG_NODEITEM,
                (size_t)item);
        }
        prev = item->pPrev;
    }
    else if ((size_t)insertAfter == TREEVIEW_NODE_LAST || !insertAfter ||
        (size_t)insertAfter == TREEVIEW_NODE_SORT) {
        if (parent) {
            prev = _treeview_getlastitem(parent->pChildFirst);
        }
        else {
            prev = _treeview_getlastitem(
                (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM));
        }
        if (prev) {
            next = prev->pNext;
        }
        else if (parent) {
            next = parent->pChildFirst;
            parent->pChildFirst = item;
        }
        else {
            next = (EX_TREEVIEW_NODEITEM*)_obj_setextralong(pObj, TREEVIEW_LONG_NODEITEM,
                (size_t)item);
        }
    }
    else {
        if (insertAfter->pParent != parent) {
            return FALSE;
        }
        prev = insertAfter;
        next = insertAfter->pNext;
    }
    item->pPrev = prev;
    if (prev) {
        prev->pNext = item;
    }
    item->pNext = next;
    if (next) {
        next->pPrev = item;
    }
    if (parent) {
        item->nDepth = parent->nDepth + 1;
    }
    else {
        item->nDepth = 0;
    }
    EX_TREEVIEW_NODEITEM* origin = item->pParent;
    item->pParent = parent;
    INT nCountChild = item->nCountChild + 1;
    while (parent) {
        parent->nCountChild += nCountChild;
        parent = parent->pParent;
    }
    while (origin) {
        origin->nCountChild -= nCountChild;
        origin = origin->pParent;
    }
    return TRUE;
}

EX_TREEVIEW_NODEITEM* _treeview_newitem(obj_s* pObj, LPCWSTR wzTitle, EX_TREEVIEW_NODEITEM* parent,
    EX_TREEVIEW_NODEITEM* insertAfter, INT nID, BOOL fExpand)
{
    EX_TREEVIEW_NODEITEM* item = (EX_TREEVIEW_NODEITEM*)Ex_MemAlloc(sizeof(EX_TREEVIEW_NODEITEM));
    if (item) {
        if (_treeview_inititem(pObj, item, parent, insertAfter)) {
            item->pwzText = StrDupW(wzTitle);
            item->fExpand = fExpand;
            item->nID = nID;
        }
        else {
            Ex_MemFree(item);
            item = NULL;
        }
    }
    return item;
}

void _treeview_freeitem(obj_s* pObj, EX_TREEVIEW_NODEITEM* item, BOOL child, BOOL dellink)
{
    if (item) {
        if (dellink) {
            _treeview_deleteitemlink(pObj, item);
        }
        if (child) {
            EX_TREEVIEW_NODEITEM* child = item->pChildFirst;
            while (child) {
                EX_TREEVIEW_NODEITEM* tmp = child->pNext;
                _treeview_freeitem(pObj, child, TRUE, FALSE);
                child = tmp;
            }
        }
        _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, pObj->hObj_, pObj->id_,
            TREEVIEW_EVENT_DELETEITEM, 0, (size_t)item);
        _treeview_updateitem(pObj);
        if (item->pwzText) {
            Ex_MemFree((LPVOID)item->pwzText);
        }
        Ex_MemFree(item);
    }
}

EX_TREEVIEW_NODEITEM* _treeview_insertitem(obj_s* pObj, EX_TREEVIEW_INSERTINFO* item)
{
    LPCWSTR title;
    title = item->pwzText;
    EX_TREEVIEW_NODEITEM* tvitem =
        _treeview_newitem(pObj, title, item->itemParent, item->itemInsertAfter, item->nID, FALSE);
    if (tvitem) {
        tvitem->lParam = item->lParam;
        tvitem->nImageIndex = item->nImageIndex;
        tvitem->nImageIndexExpand = item->nImageIndexExpand;
        tvitem->fExpand = item->fExpand;
        tvitem->dwStyle = item->dwStyle;
        _treeview_updateitem(pObj);
        if (!item->fUpdateLater) {
            INT tmp = 0;
            INT count = _treeview_itemcount(
                (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM), &tmp);
            _obj_sendmessage(_obj_gethwnd(pObj), pObj->hObj_, pObj, LISTVIEW_MESSAGE_SETITEMCOUNT,
                count, 2, 0);
        }
    }
    return tvitem;
}

EX_TREEVIEW_NODEITEM* _treeview_getnodefromindex(obj_s* pObj, INT index)
{
    listview_s* list = (listview_s*)_obj_pOwner(pObj);
    if (index >= list->index_start_ && index <= list->index_end_) {
        array_s* pArray = (array_s*)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMARRAY);

        if (!pArray) {
            return NULL;
        }

        INT arrayIndex = index - list->index_start_ + 1;

        if (arrayIndex <= 0 || arrayIndex >= Array_GetCount(pArray)) {
            return NULL;
        }

        return (EX_TREEVIEW_NODEITEM*)Array_GetMember(pArray, arrayIndex);
    }
    return NULL;
}

BOOL _treeview_generatelist(obj_s* pObj, BOOL bForce)
{
    listview_s* list = (listview_s*)_obj_pOwner(pObj);
    if (_obj_getextralong(pObj, TREEVIEW_LONG_INDEXSTART) != list->index_start_ ||
        _obj_getextralong(pObj, TREEVIEW_LONG_INDEXEND) != list->index_end_ || bForce) {
        array_s* pArray = (array_s*)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMARRAY);
        EX_TREEVIEW_NODEITEM* root =
            (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM);

        if (!root) {
            Array_Resize(pArray, 0, 0);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXSTART, list->index_start_);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXEND, list->index_end_);
            return TRUE;
        }

        INT len = list->index_end_ - list->index_start_;
        if (len > 0) {
            len++;
            Array_Resize(pArray, len + 1, 0);
        }
        else {
            Array_Resize(pArray, 0, 0);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXSTART, list->index_start_);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXEND, list->index_end_);
            return TRUE;
        }

        INT tmp = 0;
        EX_TREEVIEW_NODEITEM* item = _treeview_getitembyindex(root, list->index_start_, &tmp);
        if (!item) {
            Array_Resize(pArray, 0, 0);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXSTART, list->index_start_);
            _obj_setextralong(pObj, TREEVIEW_LONG_INDEXEND, list->index_end_);
            return TRUE;
        }

        INT i = 0;
        while (TRUE) {
            ++i;
            if (i + 1 > len || !item) {
                break;
            }
            Array_SetMember(pArray, i, (size_t)item);
            item = _treeview_getnextvisibleitem(item);
        }
        _obj_setextralong(pObj, TREEVIEW_LONG_INDEXSTART, list->index_start_);
        _obj_setextralong(pObj, TREEVIEW_LONG_INDEXEND, list->index_end_);
        return TRUE;
    }
    return FALSE;
}

void _treeview_drawitem(obj_s* pObj, EX_NMHDR* lParam)
{
    EX_CUSTOMDRAW* ps = (EX_CUSTOMDRAW*)lParam->lParam;
    listview_s* pList = (listview_s*)_obj_pOwner(pObj);
    array_s* pArray = (array_s*)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMARRAY);

    if (!pArray) {
        return;
    }

    INT index = ps->iItem - pList->index_start_ + 1;
    if (index <= 0 || index >= Array_GetCount(pArray)) {
        return;
    }

    EX_TREEVIEW_NODEITEM* item = (EX_TREEVIEW_NODEITEM*)Array_GetMember(pArray, index);
    _canvas_setantialias(ps->hCanvas, 0);
    if (item) {
        ps->iItemParam = (size_t)item;
        if (!_obj_dispatchnotify(_obj_gethwnd(pObj), pObj, pObj->hObj_, pObj->id_,
            TREEVIEW_EVENT_DRAWITEM, index, (size_t)ps)) {
            RECT rect;
            rect.left = ps->rcPaint.left + Ex_Scale(5) +
                item->nDepth * _obj_getextralong(pObj, TREEVIEW_LONG_INDENT);
            rect.right = rect.left + Ex_Scale(8);
            rect.top = (ps->rcPaint.bottom + ps->rcPaint.top - Ex_Scale(8)) / 2;
            rect.bottom = (ps->rcPaint.bottom + ps->rcPaint.top + Ex_Scale(8)) / 2;

            INT iSelected = _obj_baseproc(_obj_gethwnd(pObj), pObj->hObj_, pObj, LISTVIEW_MESSAGE_GETSELECTIONMARK, 0, 0);
            INT iHot = _obj_baseproc(_obj_gethwnd(pObj), pObj->hObj_, pObj, LISTVIEW_MESSAGE_GETHOTITEM, 0, 0);

            if (ps->iItem == iSelected) {
                HEXBRUSH brush = _brush_create((EXARGB)_obj_getextralong(pObj, TREEVIEW_LONG_SELECTEDCOLOR));
                _canvas_fillrect(ps->hCanvas, brush,
                    ps->rcPaint.left, ps->rcPaint.top,
                    ps->rcPaint.right, ps->rcPaint.bottom);
                _brush_destroy(brush);
            }
            else if (ps->iItem == iHot) {
                // 绘制热点背景
                HEXBRUSH brush = _brush_create((EXARGB)_obj_getextralong(pObj, TREEVIEW_LONG_HOTCOLOR));
                _canvas_fillrect(ps->hCanvas, brush,
                    ps->rcPaint.left, ps->rcPaint.top,
                    ps->rcPaint.right, ps->rcPaint.bottom);
                _brush_destroy(brush);
            }

            if (FLAGS_CHECK(pObj->dwStyle_, TREEVIEW_STYLE_SHOWADDANDSUB)) {
                if (item->pChildFirst) {
                    LPVOID brush = _brush_create(_obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL));
                    _canvas_drawline(ps->hCanvas, brush, rect.left, rect.top, rect.left,
                        rect.bottom, 1, D2D1_DASH_STYLE_SOLID);
                    _canvas_drawline(ps->hCanvas, brush, rect.left, rect.top, rect.right, rect.top,
                        1, D2D1_DASH_STYLE_SOLID);
                    _canvas_drawline(ps->hCanvas, brush, rect.right, rect.top, rect.right,
                        rect.bottom, 1, D2D1_DASH_STYLE_SOLID);
                    _canvas_drawline(ps->hCanvas, brush, rect.left, rect.bottom, rect.right,
                        rect.bottom, 1, D2D1_DASH_STYLE_SOLID);

                    _canvas_drawline(ps->hCanvas, brush, rect.left, (rect.top + rect.bottom) / 2,
                        rect.right, (rect.top + rect.bottom) / 2, 1,
                        D2D1_DASH_STYLE_SOLID);
                    if (!item->fExpand) {
                        _canvas_drawline(ps->hCanvas, brush, (rect.left + rect.right) / 2, rect.top,
                            (rect.left + rect.right) / 2, rect.bottom - 1, 1,
                            D2D1_DASH_STYLE_SOLID);
                    }
                    _brush_destroy(brush);
                }
            }

            if (FLAGS_CHECK(ps->dwStyle, TREEVIEW_STYLE_SHOWCABLE)) {
                rect.top = ps->rcPaint.top;
                rect.bottom = ps->rcPaint.bottom;
                LPVOID brush = _brush_create(_obj_getcolor(pObj, 2));

                if (item->pChildFirst) {
                    INT centerX = (rect.left + rect.right) / 2;
                    if (item->pNext) {
                        _canvas_drawline(ps->hCanvas, brush, centerX, (ps->rcPaint.bottom + ps->rcPaint.top) / 2 + Ex_Scale(4), centerX, rect.bottom, 1.0, D2D1_DASH_STYLE_DOT);
                    }
                    if (item->pParent || item->pPrev) {
                        _canvas_drawline(ps->hCanvas, brush, centerX, rect.top, centerX, (ps->rcPaint.bottom + ps->rcPaint.top) / 2 - Ex_Scale(4), 1.0, D2D1_DASH_STYLE_DOT);
                    }
                }

                if (!item->pChildFirst && !item->pParent) {
                    INT centerY = (rect.top + rect.bottom) / 2;
                    INT leftStart = rect.left + Ex_Scale(4);
                    INT leftEnd = rect.left + Ex_Scale(12);

                    _canvas_drawline(ps->hCanvas, brush, leftStart, centerY, leftEnd, centerY, 1.0, D2D1_DASH_STYLE_DOT);

                    if (item->pPrev) centerY = rect.top;
                    if (item->pNext) centerY = rect.bottom;
                    _canvas_drawline(ps->hCanvas, brush, leftStart, rect.top, leftStart, centerY, 1.0, D2D1_DASH_STYLE_DOT);
                }

                EX_TREEVIEW_NODEITEM* parent = item->pParent;
                EX_TREEVIEW_NODEITEM* tmp = item;
                while (parent && tmp) {
                    if (tmp->pParent && tmp->pParent->pNext) {
                        INT left = ps->rcPaint.left + Ex_Scale(9) + tmp->pParent->nDepth * _obj_getextralong(pObj, TREEVIEW_LONG_INDENT);
                        _canvas_drawline(ps->hCanvas, brush, left, rect.top, left, rect.bottom, 1.0, D2D1_DASH_STYLE_DOT);
                    }
                    if (!tmp->nCountChild) {
                        INT centerY = (rect.top + rect.bottom) / 2;
                        INT leftStart = rect.left + Ex_Scale(4);
                        INT leftEnd = rect.left + Ex_Scale(12);

                        _canvas_drawline(ps->hCanvas, brush, leftStart, centerY, leftEnd, centerY, 1.0, D2D1_DASH_STYLE_DOT);
                        if (item->pNext) centerY = rect.bottom;
                        _canvas_drawline(ps->hCanvas, brush, leftStart, rect.top, leftStart, centerY, 1.0, D2D1_DASH_STYLE_DOT);
                    }
                    tmp = tmp->pParent;
                    parent = tmp->pParent;
                }
                _brush_destroy(brush);
            }
            INT nImageIndex = 0;
            if (item->fExpand) {
                nImageIndex = item->nImageIndexExpand;
            }
            if (!nImageIndex) {
                nImageIndex = item->nImageIndex;
            }
            HEXIMAGELIST imageList =
                (HEXIMAGELIST)_obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST);

            if (imageList) {
                if (nImageIndex) {
                    rect.left = rect.right + Ex_Scale(5);
                    rect.right = rect.left + ps->rcPaint.bottom - ps->rcPaint.top;
                    _imglist_draw(imageList, nImageIndex, ps->hCanvas, rect.left, rect.top,
                        rect.right, rect.bottom, 255);
                }
            }
            _canvas_drawtext(ps->hCanvas, pObj->hFont_, _obj_getcolor(pObj, 2), item->pwzText, -1,
                4 | 0 | 32, rect.right + Ex_Scale(5), ps->rcPaint.top,
                ps->rcPaint.right, ps->rcPaint.bottom);
        }
    }
}

EX_TREEVIEW_NODEITEM* _treeview_hittest(obj_s* pObj, POINT pt, INT* pType)
{
    listview_s* pList = (listview_s*)_obj_pOwner(pObj);
    array_s* pArray = (array_s*)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMARRAY);

    if (!pArray) {
        if (pType) {
            *pType = TREEVIEW_HITTYPE_NOWHERE;
        }
        return NULL;
    }

    INT cur = pList->index_mouse_ - pList->index_start_ + 1;
    EX_TREEVIEW_NODEITEM* pItem = NULL;
    INT hitType = TREEVIEW_HITTYPE_NOWHERE;

    if (cur > 0 && cur < Array_GetCount(pArray)) {
        pItem = (EX_TREEVIEW_NODEITEM*)Array_GetMember(pArray, cur);
        if (pItem) {
            RECT rect = { 0 };
            _obj_baseproc(_obj_gethwnd(pObj), pObj->hObj_, pObj, LISTVIEW_MESSAGE_GETITEMRECT,
                pList->index_mouse_, (size_t)&rect);

            RECT tmp = { 0 };
            tmp.left = rect.left;
            tmp.right = rect.left + pItem->nDepth * _obj_getextralong(pObj, TREEVIEW_LONG_INDENT) +
                Ex_Scale(5);
            tmp.top = rect.top;
            tmp.bottom = rect.bottom;

            if (PtInRect(&tmp, pt)) {
                hitType = TREEVIEW_HITTYPE_ONITEMINDENT;
            }
            else {
                tmp.left =
                    Ex_Scale(5) + pItem->nDepth * _obj_getextralong(pObj, TREEVIEW_LONG_INDENT);
                tmp.right = tmp.left + Ex_Scale(8);
                tmp.top = (rect.bottom - rect.top - Ex_Scale(8)) / 2;
                tmp.bottom = (rect.bottom - rect.top + Ex_Scale(8)) / 2;
                if (pItem->pChildFirst) {
                    pt.x -= rect.left;
                    pt.y -= rect.top;
                    if (PtInRect(&tmp, pt)) {
                        hitType = TREEVIEW_HITTYPE_ONITEMSTATEICON;
                    }
                }
                if (hitType == TREEVIEW_HITTYPE_NOWHERE) {
                    INT nImg = NULL;
                    if (pItem->fExpand) {
                        nImg = pItem->nImageIndexExpand;
                    }
                    else {
                        nImg = pItem->nImageIndex;
                    }

                    if (nImg > 0) {
                        INT          width = 0;
                        INT          height = 0;
                        HEXIMAGELIST imageList =
                            (HEXIMAGELIST)_obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST);
                        _imglist_size(imageList, &width, &height);

                        tmp.left = tmp.right + Ex_Scale(5);
                        tmp.right = tmp.left + width;
                        tmp.top = (rect.bottom + rect.top - height) / 2;
                        tmp.bottom = (rect.bottom + rect.top + height) / 2;
                        if (pItem->pChildFirst) {
                            pt.x += rect.left;
                            pt.y += rect.top;
                        }
                        if (PtInRect(&tmp, pt)) {
                            hitType = TREEVIEW_HITTYPE_ONITEMICON;
                        }
                    }
                    if (hitType == TREEVIEW_HITTYPE_NOWHERE) {
                        tmp.left = tmp.right + Ex_Scale(5);
                        tmp.right = rect.right;
                        tmp.top = rect.top;
                        tmp.bottom = rect.bottom;
                        if (PtInRect(&tmp, pt)) {
                            hitType = TREEVIEW_HITTYPE_ONITEMLABEL;
                        }
                    }
                }
            }
        }
    }
    if (pType) {
        *pType = hitType;
    }
    return pItem;
}

LRESULT CALLBACK _treeview_onscrollbarmsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam,
    LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_MOUSEHOVER) {
        Ex_ObjPostMessage(hObj, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 1);   // 显示滚动条
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjPostMessage(hObj, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 0);   // 隐藏滚动条
    }
    else if (uMsg == SCROLLBAR_MESSAGE_SETVISIBLE) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_ALPHA, lParam != 0 ? 255 : 0);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    return 0;
}

LRESULT CALLBACK _treeview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    obj_s* pObj = NULL;
    INT    nError;
    if (!_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
    }
    if (uMsg == WM_NOTIFY) {
        if (hObj == ((EX_NMHDR*)lParam)->hObjFrom) {
            INT nCode = ((EX_NMHDR*)lParam)->nCode;
            if (nCode == NM_CALCSIZE) {
                _treeview_calcsize(pObj, (EX_NMHDR*)lParam);
            }
            else if (nCode == NM_CUSTOMDRAW) {
                _treeview_generatelist(pObj, FALSE);
                _treeview_drawitem(pObj, (EX_NMHDR*)lParam);
            }
        }
    }
    else if (uMsg == WM_CREATE) {
        _obj_setextralong(pObj, TREEVIEW_LONG_INDENT, Ex_Scale(10));
        _obj_setextralong(pObj, TREEVIEW_LONG_ITEMHEIGHT, Ex_Scale(20));
        _obj_setextralong(pObj, TREEVIEW_LONG_ITEMARRAY, (size_t)Array_Create(0));
        _obj_setextralong(pObj, TREEVIEW_LONG_SELECTEDCOLOR, ExRGB2ARGB(60, 60, 60, 255));
        _obj_setextralong(pObj, TREEVIEW_LONG_HOTCOLOR, ExRGB2ARGB(30, 30, 30, 255));
        HEXOBJ hObj_hscroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
        Ex_ObjPostMessage(hObj_hscroll, SCROLLBAR_MESSAGE_SETVISIBLE, 0, 0);       
        Ex_ObjSetLong(hObj_hscroll, OBJECT_LONG_OBJPROC, (size_t)_treeview_onscrollbarmsg);   
    }
    else if (uMsg == WM_DESTROY) {
        Array_Destroy((array_s*)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMARRAY));
        EX_TREEVIEW_NODEITEM* pItem =
            (EX_TREEVIEW_NODEITEM*)_obj_setextralong(pObj, TREEVIEW_LONG_NODEITEM, 0);
        while (pItem) {
            EX_TREEVIEW_NODEITEM* next = pItem->pNext;
            _treeview_freeitem(pObj, pItem, TRUE, FALSE);
            pItem = next;
        }
        HEXIMAGELIST hImgList = (HEXIMAGELIST)_obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST);
        if (hImgList) {
            _imglist_destroy(hImgList);
        }
    }
    else if (uMsg == TREEVIEW_MESSAGE_INSERTITEM) {
        return (size_t)_treeview_insertitem(pObj, (EX_TREEVIEW_INSERTINFO*)lParam);
    }
    else if (uMsg == TREEVIEW_MESSAGE_DELETEITEM) {
        BOOL bDeleted = FALSE;
        if (lParam == TREEVIEW_NODE_ROOT || !lParam) {
            EX_TREEVIEW_NODEITEM* pItem =
                (EX_TREEVIEW_NODEITEM*)_obj_setextralong(pObj, TREEVIEW_LONG_NODEITEM, 0);
            while (pItem) {
                EX_TREEVIEW_NODEITEM* next = pItem->pNext;
                _treeview_freeitem(pObj, pItem, TRUE, FALSE);
                pItem = next;
                bDeleted = TRUE;
            }
        }
        else {
            _treeview_freeitem(pObj, (EX_TREEVIEW_NODEITEM*)lParam, TRUE, TRUE);
            bDeleted = TRUE;
        }
        if (bDeleted && wParam) {
            INT tmp = 0;
            INT count = _treeview_itemcount(
                (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM), &tmp);
            _obj_sendmessage(_obj_gethwnd(pObj), hObj, pObj, LISTVIEW_MESSAGE_SETITEMCOUNT, count,
                2, 0);
            _treeview_generatelist(pObj, TRUE);
        }
        return bDeleted;
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETITEM || uMsg == TREEVIEW_MESSAGE_SETITEM) {
        if (wParam && lParam) {
            if (uMsg == TREEVIEW_MESSAGE_GETITEM) {
                RtlMoveMemory((LPVOID)lParam, (LPVOID)wParam, sizeof(EX_TREEVIEW_ITEMINFO));
            }
            else {
                RtlMoveMemory((LPVOID)wParam, (LPVOID)lParam, sizeof(EX_TREEVIEW_ITEMINFO));
                EX_TREEVIEW_NODEITEM* itemInfo = (EX_TREEVIEW_NODEITEM*)wParam;
                itemInfo->pwzText = StrDupW(itemInfo->pwzText);
                _treeview_updateitem(pObj);
            }
            return TRUE;
        }
        return FALSE;
    }
    else if (uMsg == TREEVIEW_MESSAGE_SETITEMTEXTW) {
        if (wParam) {
            Ex_MemFree((LPVOID)((EX_TREEVIEW_NODEITEM*)wParam)->pwzText);
            ((EX_TREEVIEW_NODEITEM*)wParam)->pwzText = StrDupW((LPCWSTR)lParam);
            _treeview_updateitem(pObj);
            return TRUE;
        }
        return FALSE;
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETITEMTEXTW) {
        return wParam ? (size_t)((EX_TREEVIEW_NODEITEM*)wParam)->pwzText : NULL;
    }
    else if (uMsg == TREEVIEW_MESSAGE_ENSUREVISIBLE) {
        BOOL result = FALSE;
        if (lParam) {
            INT count = 0;
            result = _treeview_expanditem(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
            if (result) {
                INT tmp = 0;
                count = _treeview_itemcount(
                    (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM), &tmp);
                _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_SETITEMCOUNT, count, 0);
            }
            count = _treeview_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
            if (count > 0) {
                return _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_ENSUREVISIBLE, 0, count);
            }
        }
        return result;
    }
    else if (uMsg == TREEVIEW_MESSAGE_EXPAND) {
        if (lParam) {
            EX_TREEVIEW_NODEITEM* pItem = (EX_TREEVIEW_NODEITEM*)lParam;
            if (pItem->fExpand != wParam) {
                if (!Ex_ObjDispatchNotify(hObj, TREEVIEW_EVENT_ITEMEXPANDING, wParam, lParam)) {
                    pItem->fExpand = wParam;
                    Ex_ObjDispatchNotify(hObj, TREEVIEW_EVENT_ITEMEXPANDED, wParam, lParam);
                    INT tmp = 0;
                    INT count = _treeview_itemcount(
                        (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM),
                        &tmp);
                    _treeview_generatelist(pObj, TRUE);
                    _treeview_updateitem(pObj);

                    int index = 0;
                    if (wParam == 0) {
                        index = Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_GETHOTITEM, 0, 0);
                    }
                    _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_SETITEMCOUNT, count,
                        MAKELONG(LVSICF_NOSCROLL, index));
                }
            }
        }
    }
    else if (uMsg == TREEVIEW_MESSAGE_SELECTITEM) {
        if (lParam) {
            INT count = _treeview_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)lParam);
            _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_SETSELECTIONMARK, 1, count);
        }
        else {
            _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_SETSELECTIONMARK, 1, 0);
        }
        return NULL;
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETITEMRECT) {
        INT count = _treeview_getitemindex(pObj, (EX_TREEVIEW_NODEITEM*)wParam);
        if (count > 0) {
            _obj_baseproc(hWnd, hObj, pObj, LISTVIEW_MESSAGE_GETITEMRECT, count, lParam);
            return TRUE;
        }
        return FALSE;
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETNEXTITEM) {
        return (size_t)_treeview_getnextitem(pObj, (EX_TREEVIEW_NODEITEM*)lParam, wParam);
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETCOUNT || uMsg == TREEVIEW_MESSAGE_GETVISIBLECOUNT) {
        return _treeview_getvisiblecount(
            (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM), FALSE);
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETINDENT) {
        return (FLOAT)_obj_getextralong(pObj, TREEVIEW_LONG_INDENT) / g_Li.DpiX;
    }
    else if (uMsg == TREEVIEW_MESSAGE_SETINDENT) {
        _obj_setextralong(pObj, TREEVIEW_LONG_INDENT, Ex_Scale(wParam));
        _treeview_updateitem(pObj);
        _treeview_generatelist(pObj, TRUE);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == TREEVIEW_MESSAGE_UPDATE) {
        INT tmp = 0;
        INT count = _treeview_itemcount(
            (EX_TREEVIEW_NODEITEM*)_obj_getextralong(pObj, TREEVIEW_LONG_NODEITEM), &tmp);
        _obj_sendmessage(hWnd, hObj, pObj, LISTVIEW_MESSAGE_SETITEMCOUNT, count, 2, 0);
        _treeview_generatelist(pObj, TRUE);
    }
    else if (uMsg == TREEVIEW_MESSAGE_HITTEST) {
        return (size_t)_treeview_hittest(pObj, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) },
            (INT*)wParam);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        INT hitType = TREEVIEW_HITTYPE_NOWHERE;
        EX_TREEVIEW_NODEITEM* pItem = _treeview_hittest(pObj, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, &hitType);

        if (pItem && hitType == TREEVIEW_HITTYPE_ONITEMSTATEICON) {
            _obj_baseproc(hWnd, hObj, pObj, TREEVIEW_MESSAGE_EXPAND, !pItem->fExpand, (size_t)pItem);
            _obj_baseproc(hWnd, hObj, pObj, TREEVIEW_MESSAGE_ENSUREVISIBLE, 0, (size_t)pItem);
            _obj_setextralong(pObj, TREEVIEW_LONG_EXPAND, 1);
            return 1;
        }
    }
    else if (uMsg == WM_LBUTTONUP) {
        if (_obj_getextralong(pObj, TREEVIEW_LONG_EXPAND) == 0) {
            _obj_setextralong(pObj, TREEVIEW_LONG_EXPAND, 0);
            return 1;
        }
    }
    else if (uMsg == TREEVIEW_MESSAGE_SETIMAGELIST) {
        HEXIMAGELIST pImgList = (HEXIMAGELIST)_obj_setextralong(pObj, TREEVIEW_LONG_HIMAGELIST, lParam);
        if (pImgList) {
            _imglist_destroy(pImgList);
        }

        if (wParam && lParam) {
            INT height = 0;
            _imglist_size((LPVOID)lParam, NULL, &height);
            _obj_setextralong(pObj, TREEVIEW_LONG_ITEMHEIGHT, height);
            _treeview_updateitem(pObj);
            _obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0,
                MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETIMAGELIST) {
        return _obj_getextralong(pObj, TREEVIEW_LONG_HIMAGELIST);
    }
    else if (uMsg == TREEVIEW_MESSAGE_SETITEMHEIGHT) {
        lParam = Ex_Scale(lParam);
        if (_obj_setextralong(pObj, TREEVIEW_LONG_ITEMHEIGHT, lParam) != lParam) {
            _obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0,
                MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETITEMHEIGHT) {
        return (FLOAT)_obj_getextralong(pObj, TREEVIEW_LONG_ITEMHEIGHT) / g_Li.DpiX;
    }
    else if (uMsg == TREEVIEW_MESSAGE_GETNODEFROMINDEX) {
        return (size_t)_treeview_getnodefromindex(pObj, wParam);
    }
    return Ex_ObjCallProc(_tv_lpfn, hWnd, hObj, uMsg, wParam, lParam);
}

void _treeview_register()
{
    EX_CLASSINFO class_list = { 0 };
    INT          nError = 0;
    Ex_ObjGetClassInfoEx((LPCWSTR)ATOM_LISTVIEW, &class_list);
    _tv_lpfn = class_list.pfnClsProc;
    // TREEVIEW_STYLE_SHOWADDANDSUB | TREEVIEW_STYLE_SHOWCABLE |
    _obj_register(ATOM_TREEVIEW, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_HSCROLL,
        class_list.dwStyleEx, class_list.dwTextFormat, 11 * sizeof(size_t), NULL,
        _treeview_proc, class_list.dwFlags, &nError);
}
