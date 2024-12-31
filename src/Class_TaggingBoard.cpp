#include "stdafx.h"

void _taggingboard_register()
{
    WCHAR wzCls[] = L"TaggingBoard";
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, 0, 20 * sizeof(size_t),
                   0, 0, _taggingboard_proc);
}

LRESULT CALLBACK _taggingboard_OnScrollBarMsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                              LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            auto   si        = (si_s*)ps.dwOwnerData;
            INT    httype    = si->httype_;
            BOOL   bHover    = (ps.dwState & STATE_HOVER) != 0;
            BOOL   bDown     = (ps.dwState & STATE_DOWN) != 0;
            BOOL   bVScroll  = (ps.dwStyle & SCROLLBAR_STYLE_VERTICALSCROLL) != 0;
            EXATOM atomClass = bVScroll ? ATOM_VSCROLL : ATOM_HSCROLL;
            auto   brush     = _brush_create(ExARGB(255, 255, 255, 100));
            _canvas_fillrect(ps.hCanvas, brush, ps.rcPaint.left, ps.rcPaint.top + 1,
                             ps.rcPaint.right - 1, ps.rcPaint.bottom - 1);
            _brush_setcolor(brush, ExARGB(120, 120, 120, 255));
            INT atomBtn1 = ATOM_ARROW1_NORMAL;
            INT atomBtn2 = ATOM_ARROW2_NORMAL;

            INT crBar = ExARGB(124, 44, 6, 200);
            if (httype == SCROLLBAR_HITTYPE_CONTROL) {
                if (bDown) {
                    crBar    = ExARGB(138, 49, 6, 200);
                    atomBtn1 = ATOM_ARROW1_DOWN;
                    atomBtn2 = ATOM_ARROW2_DOWN;
                }
                else {
                    if (bHover) {
                        crBar    = ExARGB(100, 36, 5, 200);
                        atomBtn1 = ATOM_ARROW1_HOVER;
                        atomBtn2 = ATOM_ARROW2_HOVER;
                    }
                }
            }
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, si->rcArrow1_left_, si->rcArrow1_top_,
                                  si->rcArrow1_right_, si->rcArrow1_bottom_, atomClass, atomBtn1, 0,
                                  0, 0, 0, 255);
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, si->rcArrow2_left_, si->rcArrow2_top_,
                                  si->rcArrow2_right_, si->rcArrow2_bottom_, atomClass, atomBtn2, 0,
                                  0, 0, 0, 255);

            _brush_setcolor(brush, crBar);
            if (bVScroll) {
                _canvas_fillroundedrect(ps.hCanvas, brush, si->rcThumb_left_, si->rcThumb_top_,
                                        si->rcThumb_right_, si->rcThumb_bottom_,
                                        (si->rcThumb_right_ - si->rcThumb_left_) / 2,
                                        (si->rcThumb_right_ - si->rcThumb_left_) / 2);
            }
            else {
                _canvas_fillroundedrect(ps.hCanvas, brush, si->rcThumb_left_, si->rcThumb_top_,
                                        si->rcThumb_right_, si->rcThumb_bottom_,
                                        (si->rcThumb_bottom_ - si->rcThumb_top_) / 2,
                                        (si->rcThumb_bottom_ - si->rcThumb_top_) / 2);
            }
            _brush_destroy(brush);
            Ex_ObjEndPaint(hObj, &ps);
            *lpResult = 1;
            return 1;
        }
    }
    return 0;
}

void _taggingboard_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL   bHScoll = uMsg == WM_HSCROLL;
    INT    nCode   = LOWORD(wParam);
    INT    nPos    = 0;
    HEXOBJ hVSB    = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
    HEXOBJ hHSB    = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
    RECT   rc;
    Ex_ObjGetClientRect(hObj, &rc);
    INT widthRC  = (INT)(rc.right - rc.left);
    INT heightRC = (INT)(rc.bottom - rc.top);
    widthRC      = Ex_Scale(widthRC);
    heightRC     = Ex_Scale(heightRC);
    INT oPos     = 0;
    INT nView    = 0;
    INT nPage    = 2000;
    INT nLine    = 200;
    if (bHScoll) {
        oPos  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
        nView = widthRC;
    }
    else {
        oPos  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
        nView = heightRC;
    }

    if (nCode == SB_THUMBPOSITION) {
        if (bHScoll) {
            nPos = Ex_ObjScrollGetTrackPos(hHSB, SCROLLBAR_TYPE_CONTROL);
        }
        else {
            nPos = Ex_ObjScrollGetTrackPos(hVSB, SCROLLBAR_TYPE_CONTROL);
        }
    }
    else {
        // 箭头
        if (nCode == SB_PAGEUP) {
            nPos = oPos - nPage;
        }
        else if (nCode == SB_PAGEDOWN) {
            nPos = oPos + nPage;
        }
        else if (nCode == SB_LINEUP) {
            nPos = oPos - nLine;
        }
        else if (nCode == SB_LINEDOWN) {
            nPos = oPos + nLine;
        }
        else if (nCode == SB_TOP) {
            nPos = 0;
        }
        else {
            return;
        }
    }

    if (bHScoll) {
        // 控制滚动条滚动到最顶部和最底部逻辑，因为滚动条range是-imgLeft到imgLeft,所以限制范围从-imgLeft到imgLeft
        auto imgLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
        if (nPos < imgLeft && nPos > 0) {
            nPos = imgLeft;
        }
        if (nPos > -imgLeft && nPos > 0) {
            nPos = -imgLeft;
        }
    }
    else {
        auto imgTop = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
        if (nPos < imgTop && nPos > 0) {
            nPos = imgTop;
        }
        if (nPos > -imgTop && nPos > 0) {
            nPos = -imgTop;
        }
    }


    if (nPos != oPos) {
        if (bHScoll) {
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET, nPos);
            Ex_ObjScrollSetPos(hHSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
        }
        else {
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET, nPos);
            Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
        }

        Ex_ObjInvalidateRect(hObj, 0);
    }
}

LRESULT CALLBACK _taggingboard_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        auto arr                           = malloc(sizeof(EX_POLYGON_ARRAY));
        ((EX_POLYGON_ARRAY*)arr)->polygons = malloc(sizeof(size_t));
        ((EX_POLYGON_ARRAY*)arr)->count    = 1;
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ARRAY, (LONG_PTR)arr);

        auto ptr                   = malloc(sizeof(EX_POLYGON));
        ((EX_POLYGON*)ptr)->points = malloc(sizeof(POINT));
        ((EX_POLYGON*)ptr)->count  = 0;
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_DATA, (LONG_PTR)ptr);
        size_t ptrValue = (size_t)ptr;
        RtlMoveMemory(((EX_POLYGON_ARRAY*)arr)->polygons, &ptrValue, sizeof(size_t));
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);

        HEXBRUSH brush = _brush_create(ExARGB(255, 0, 0, 255));
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_PEN, (LONG_PTR)brush);
        HEXBRUSH brush2 = _brush_create(ExARGB(255, 0, 0, 50));
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_PEN_FILL, (LONG_PTR)brush2);

        RECT rc;
        Ex_ObjGetRect(hObj, &rc);

        auto canvas = _canvas_createfromobj(hObj, Ex_Scale(rc.right), Ex_Scale(rc.bottom), 0);
        _canvas_begindraw(canvas);
        _canvas_clear(canvas, ExARGB(255, 255, 255, 255));
        _canvas_enddraw(canvas);

        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CANVAS, canvas);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET, 0);
        auto scalePtr = malloc(sizeof(float));
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE, (LONG_PTR)scalePtr);

        Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_BOTH, TRUE);
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000,
                            0, TRUE);
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000,
                            0, TRUE);
        HEXOBJ hObj_scroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
        Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC,
                      (size_t)_taggingboard_OnScrollBarMsg);   // 改变滚动条回调
        hObj_scroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
        Ex_ObjSetLong(hObj_scroll, OBJECT_LONG_OBJPROC,
                      (size_t)_taggingboard_OnScrollBarMsg);   // 改变滚动条回调
    }
    else if (uMsg == WM_SIZE) {
        /*auto img = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (img != 0)
        {
            INT width, height;
            _img_getsize(img, &width, &height);
            INT widthRC = (INT)LOWORD(lParam);
            INT heightRC = (INT)HIWORD(lParam);
            INT edge;
            edge = heightRC;
            INT edgeImg;
            if (height > width)
            {
                edgeImg = height;
            }
            else {
                edgeImg = width;
            }
            float scaleImg = (float)edgeImg / edge;
            INT left = (widthRC - width / scaleImg) / 2;
            INT top = (heightRC - height / scaleImg) / 2;
            auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
            float oldscale;
            RtlMoveMemory(&oldscale, scalePtr, 4);
            RtlMoveMemory(scalePtr, &scaleImg, 4);
            INT oldleft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
            INT oldtop = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, left);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, top);

            auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
            if (arr->count > 1)
            {
                for (int i = 0; i < arr->count - 1; i++)
                {
                    size_t ptrValue = 0;
                    RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
        sizeof(size_t)); EX_POLYGON* ptr = (EX_POLYGON*)ptrValue; if (ptr->count > 0)
                    {
                        for (int j = 0; j < ptr->count; j++)
                        {
                            float x = 0, y = 0;
                            RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                            RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                            x = (float)(x - oldleft) * oldscale / scaleImg + left;
                            y = (float)(y - oldtop) * oldscale / scaleImg + top;
                            RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8), &x, 4);
                            RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8 + 4), &y, 4);
                        }
                    }
                }
            }
        }*/
    }
    else if (uMsg == WM_DESTROY) {
        auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
        if (arr->count > 0) {
            for (int i = 0; i < arr->count; i++) {
                size_t ptrValue = 0;
                RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                              sizeof(size_t));
                auto ptr = (EX_POLYGON*)ptrValue;
                free(ptr->points);
                free(ptr);
            }
        }
        free(arr->polygons);
        free(arr);

        auto canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CANVAS);
        _canvas_destroy(canvas);
        HEXBRUSH brush = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN);
        _brush_destroy(brush);
        HEXBRUSH brush2 = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN_FILL);
        _brush_destroy(brush2);

        auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (old != 0) {
            _img_destroy(old);
        }

        auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
        free(scalePtr);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_START) {
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 1);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_STOP) {
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);
        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
        if (ptr->count > 0) {
            // 清空临时点
            ptr->points = realloc(ptr->points, 1 * 8);
            ptr->count  = 0;
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_SET_PEN_COLOR) {
        HEXBRUSH brush  = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN);
        HEXBRUSH brush2 = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN_FILL);
        auto     rgb    = ExARGB2RGB(lParam);
        _brush_setcolor(brush, lParam);
        _brush_setcolor(brush2, ExRGB2ARGB(rgb, 50));
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_CLEAR) {
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, 0);
        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, 0);
        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
        if (ptr->count > 0) {
            // 清空临时点
            ptr->points = realloc(ptr->points, 1 * 8);
            ptr->count  = 0;
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
        }
        auto arr        = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
        arr->polygons   = realloc(arr->polygons, 1 * sizeof(size_t));
        size_t ptrValue = (size_t)ptr;
        RtlMoveMemory(arr->polygons, &ptrValue, sizeof(size_t));
        arr->count = 1;
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_SET_BKG) {
        if (lParam == 0) {
            auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
            if (old != 0) {
                _img_destroy(old);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET, 0);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET, 0);
                Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, 1, TRUE);
                Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, 1, TRUE);
                auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
                if (ptr->count > 0) {
                    // 清空临时点
                    ptr->points = realloc(ptr->points, 1 * 8);
                    ptr->count  = 0;
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
                }
                auto arr        = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
                arr->polygons   = realloc(arr->polygons, 1 * sizeof(size_t));
                size_t ptrValue = (size_t)ptr;
                RtlMoveMemory(arr->polygons, &ptrValue, sizeof(size_t));
                arr->count = 1;
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
        else {
            INT width, height;
            _img_getsize(lParam, &width, &height);
            RECT rc;
            Ex_ObjGetClientRect(hObj, &rc);
            auto widthRC  = rc.right - rc.left;
            auto heightRC = rc.bottom - rc.top;
            widthRC       = (INT)(Ex_Scale(widthRC));
            heightRC      = (INT)(Ex_Scale(heightRC));

            INT edge;
            edge = heightRC;
            INT edgeImg;
            if (height > width) {
                edgeImg = height;
            }
            else {
                edgeImg = width;
            }
            float scaleImg = (float)edgeImg / edge;
            INT   left     = (widthRC - width / scaleImg) / 2;
            INT   top      = (heightRC - height / scaleImg) / 2;

            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, left);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, top);
            auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
            if (old != 0) {
                _img_destroy(old);
            }
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG, lParam);
            auto scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
            RtlMoveMemory(scalePtr, &scaleImg, 4);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_GET_DATA) {
        return Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_SET_DATA) {
        auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (old != 0) {
            auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
            if (arr->count > 0) {
                for (int i = 0; i < arr->count; i++) {
                    size_t ptrValue = 0;
                    RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                                  sizeof(size_t));
                    auto ptr = (EX_POLYGON*)ptrValue;
                    free(ptr->points);
                    free(ptr);
                }
            }
            free(arr->polygons);
            free(arr);

            // 新建临时点
            auto newPtr    = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
            newPtr->points = malloc(sizeof(POINT));
            newPtr->count  = 0;
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_DATA, (LONG_PTR)newPtr);

            // 添加临时点
            auto newArr        = (EX_POLYGON_ARRAY*)lParam;
            newArr->polygons   = realloc(newArr->polygons, (newArr->count + 1) * sizeof(size_t));
            size_t newPtrValue = (size_t)newPtr;
            RtlMoveMemory((LPVOID)((size_t)newArr->polygons + newArr->count * sizeof(size_t)),
                          &newPtrValue, sizeof(size_t));
            newArr->count = newArr->count + 1;

            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ARRAY, lParam);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_GET_IMG_SCALE) {
        return Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_GET_IMG_LEFT_OFFSET) {
        return Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_GET_IMG_TOP_OFFSET) {
        return Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_DELETE_PATH) {
        auto arr   = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
        INT  index = (INT)lParam - 1;
        BOOL ret   = FALSE;
        if (arr->count > index && index >= 0) {
            INT hit = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH);
            if (hit == (INT)lParam) {
                // 移除选中路径
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, 0);
            }
            auto newArr      = (EX_POLYGON_ARRAY*)malloc(sizeof(EX_POLYGON_ARRAY));
            newArr->count    = arr->count - 1;
            newArr->polygons = malloc(newArr->count * sizeof(size_t));

            // 拷贝index前面数据
            for (int i = 0; i < index; i++) {
                size_t ptrValue = 0;
                RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                              sizeof(size_t));
                auto ptr       = (EX_POLYGON*)ptrValue;
                auto newPtr    = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
                newPtr->points = malloc(ptr->count * sizeof(POINT));
                newPtr->count  = ptr->count;
                for (int j = 0; j < ptr->count; j++) {
                    float x = 0, y = 0;
                    RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                    RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                    RtlMoveMemory((LPVOID)((size_t)newPtr->points + j * 8), &x, 4);
                    RtlMoveMemory((LPVOID)((size_t)newPtr->points + j * 8 + 4), &y, 4);
                }
                size_t newPtrValue = (size_t)newPtr;
                RtlMoveMemory((LPVOID)((size_t)newArr->polygons + i * sizeof(size_t)), &newPtrValue,
                              sizeof(size_t));
                free(ptr->points);
                free(ptr);
            }
            // 拷贝index后面数据
            for (int i = index + 1; i < arr->count; i++) {
                size_t ptrValue = 0;
                RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                              sizeof(size_t));
                auto ptr    = (EX_POLYGON*)ptrValue;
                auto newPtr = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
                if (i == arr->count - 1) {
                    // 最后临时点
                    newPtr->points = malloc(1 * sizeof(POINT));
                    newPtr->count  = 0;
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_DATA, (LONG_PTR)newPtr);
                }
                else {
                    newPtr->points = malloc(ptr->count * sizeof(POINT));
                    newPtr->count  = ptr->count;
                }

                for (int j = 0; j < newPtr->count; j++) {
                    float x = 0, y = 0;
                    RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                    RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                    RtlMoveMemory((LPVOID)((size_t)newPtr->points + j * 8), &x, 4);
                    RtlMoveMemory((LPVOID)((size_t)newPtr->points + j * 8 + 4), &y, 4);
                }
                size_t newPtrValue = (size_t)newPtr;
                RtlMoveMemory((LPVOID)((size_t)newArr->polygons + (i - 1) * sizeof(size_t)),
                              &newPtrValue, sizeof(size_t));
                free(ptr->points);
                free(ptr);
            }
            free(arr->polygons);
            free(arr);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ARRAY, (size_t)newArr);
            Ex_ObjInvalidateRect(hObj, 0);
            ret = TRUE;
        }
        return ret;
    }
    else if (uMsg == TAGGINGBOARD_MESSAGE_GET_HIT_PATH) {
        return Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH);
    }
    else if (uMsg == WM_PAINT) {
        _taggingboard_paint(hObj);
    }
    else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) {
        _taggingboard_onvscrollbar(hWnd, hObj, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_MOUSEWHEEL) {
        auto ctrl = (GetAsyncKeyState(VK_CONTROL) & 32768) != 0;
        if (ctrl) {
            auto img = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
            if (img != 0) {
                auto  scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
                float scale;
                RtlMoveMemory(&scale, scalePtr, 4);
                float oldScale = scale;
                auto  zDelta   = (SHORT)HIWORD(wParam);   // 正数往上翻，负数往下翻
                if (zDelta > 0) {
                    scale = scale / 1.5;
                }
                else {
                    scale = scale * 1.5;
                }
                RtlMoveMemory(scalePtr, &scale, 4);

                INT width, height;
                _img_getsize(img, &width, &height);

                RECT rc;
                Ex_ObjGetClientRect(hObj, &rc);
                INT widthRC  = (INT)(rc.right - rc.left);
                INT heightRC = (INT)(rc.bottom - rc.top);
                widthRC      = Ex_Scale(widthRC);
                heightRC     = Ex_Scale(heightRC);
                INT  newLeft = (widthRC - (float)width / scale) / 2;
                INT  newTop  = (heightRC - (float)height / scale) / 2;
                auto oldLeft = (INT)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
                auto oldTop  = (INT)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);

                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET, newLeft);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET, newTop);
                INT vHeight = -newTop;
                INT vWidth  = -newLeft;

                if (vHeight > 0) {
                    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, -vHeight, vHeight, TRUE);
                }
                else {
                    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, 1, TRUE);
                }
                if (vWidth > 0) {
                    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, -vWidth, vWidth, TRUE);
                }
                else {
                    Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, 1, TRUE);
                }

                auto arr = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
                if (arr->count > 1) {
                    for (int i = 0; i < arr->count - 1; i++) {
                        size_t ptrValue = 0;
                        RtlMoveMemory(&ptrValue,
                                      (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                                      sizeof(size_t));
                        EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
                        if (ptr->count > 0) {
                            for (int j = 0; j < ptr->count; j++) {
                                float x = 0, y = 0;
                                RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                                RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                                x = (float)(x - oldLeft) * oldScale / scale + newLeft;
                                y = (float)(y - oldTop) * oldScale / scale + newTop;
                                RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8), &x, 4);
                                RtlMoveMemory((LPVOID)((size_t)ptr->points + j * 8 + 4), &y, 4);
                            }
                        }
                    }
                }

                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == WM_MOUSEMOVE) {
        auto bkg = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (bkg != 0) {
            auto x = ((FLOAT)GET_X_LPARAM(lParam));
            auto y = ((FLOAT)GET_Y_LPARAM(lParam));

            auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
            auto sbOffsetTop  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
            x                 = x + sbOffsetLeft;
            y                 = y + sbOffsetTop;
            int   left        = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
            int   top         = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
            auto  scalePtr    = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
            float scale;
            RtlMoveMemory(&scale, scalePtr, 4);
            INT width, height;
            _img_getsize(bkg, &width, &height);
            int newWidth  = (int)((float)width / scale);
            int newHeight = (int)((float)height / scale);
            if (x > left && x < left + newWidth && y > top && y < top + newHeight) {
                INT retX = (x - left) * scale;
                INT retY = (y - top) * scale;
                Ex_ObjDispatchNotify(hObj, TAGGINGBOARD_EVENT_MOUSEMOVE, retX, retY);
            }

            INT  hitTest = 0;
            auto ret     = _taggingboard_ptinregion(hObj, x, y, &hitTest);
            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH, hitTest);
            auto tagging = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_TAGGING);
            if (tagging == 1) {
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, x);
                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, y);

                auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);

                if (ptr->count > 2) {
                    float startX, startY;
                    RtlMoveMemory(&startX, (LPVOID)((size_t)ptr->points), 4);
                    RtlMoveMemory(&startY, (LPVOID)((size_t)ptr->points + 4), 4);
                    RECT  rc = {startX - 3, startY - 3, startX + 3, startY + 3};
                    POINT pt = {x, y};
                    if (PtInRect(&rc, pt)) {
                        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 1);
                    }
                    else {
                        Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
                    }
                }
            }
            _taggingboard_updatedraw(hObj);
        }
    }
    else if (uMsg == WM_LBUTTONUP) {
        auto old = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (old != 0) {
            auto x            = ((FLOAT)GET_X_LPARAM(lParam));   // 无关dpi缩放
            auto y            = ((FLOAT)GET_Y_LPARAM(lParam));
            auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
            auto sbOffsetTop  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
            x                 = x + sbOffsetLeft;
            y                 = y + sbOffsetTop;

            auto tagging = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_TAGGING);
            if (tagging == 1) {
                int   left     = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
                int   top      = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
                auto  bkg      = (HEXIMAGE)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
                auto  scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
                float scale;
                RtlMoveMemory(&scale, scalePtr, 4);
                INT width, height;
                _img_getsize(bkg, &width, &height);
                int newWidth  = (int)((float)width / scale);
                int newHeight = (int)((float)height / scale);
                // 限制在背景图区域内标注

                if (x > left && x < left + newWidth && y > top && y < top + newHeight) {
                    INT hitTest = 0;
                    if (!_taggingboard_ptinregion(hObj, x, y, &hitTest)) {

                        auto hit = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT);
                        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
                        if (hit == 0) {
                            if (ptr->count == 0) {
                                Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 0);
                                // first point
                                RtlMoveMemory(ptr->points, &x, 4);
                                RtlMoveMemory((LPVOID)((size_t)ptr->points + 4), &y, 4);
                                ptr->count = ptr->count + 1;
                            }
                            else {
                                // add point
                                ptr->points = realloc(ptr->points, (ptr->count + 1) * 8);
                                RtlMoveMemory((LPVOID)((size_t)ptr->points + ptr->count * 8), &x,
                                              4);
                                RtlMoveMemory((LPVOID)((size_t)ptr->points + ptr->count * 8 + 4),
                                              &y, 4);
                                ptr->count = ptr->count + 1;
                            }

                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, (INT)x);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, (INT)y);
                        }
                        else {
                            // close polygon
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_TAGGING, 0);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT, 0);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, 0);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, 0);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDX, 0);
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_ENDY, 0);
                            // add polygon
                            auto arr =
                                (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
                            // realloc point
                            auto newPtr                   = malloc(sizeof(EX_POLYGON));
                            ((EX_POLYGON*)newPtr)->points = malloc(sizeof(POINT));
                            ((EX_POLYGON*)newPtr)->count  = 0;
                            Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_DATA, (LONG_PTR)newPtr);

                            // add polygon
                            arr->polygons =
                                realloc(arr->polygons, (arr->count + 1) * sizeof(size_t));
                            size_t ptrValue = (size_t)newPtr;
                            RtlMoveMemory(
                                (LPVOID)((size_t)arr->polygons + arr->count * sizeof(size_t)),
                                &ptrValue, sizeof(size_t));
                            arr->count = arr->count + 1;
                        }
                    }
                }
            }
            else {
                auto hitPath = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH);
                if (hitPath != 0) {
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH, hitPath);
                    Ex_ObjDispatchNotify(hObj, TAGGINGBOARD_EVENT_HITPATH, 0, hitPath);
                }
            }
        }
    }
    else if (uMsg == WM_RBUTTONUP) {
        auto x = (INT)((FLOAT)GET_X_LPARAM(lParam));
        auto y = (INT)((FLOAT)GET_Y_LPARAM(lParam));

        auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
        auto sbOffsetTop  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
        x                 = x + sbOffsetLeft;
        y                 = y + sbOffsetTop;
        auto tagging      = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_TAGGING);
        if (tagging == 1) {
            auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
            if (ptr->count > 0) {
                // del point
                if (ptr->count > 1) {
                    ptr->points = realloc(ptr->points, (ptr->count - 1) * 8);
                    ptr->count  = ptr->count - 1;
                    float x = 0, y = 0;
                    RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + (ptr->count - 1) * 8), 4);
                    RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + (ptr->count - 1) * 8 + 4), 4);
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINX, (INT)x);
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_BEGINY, (INT)y);
                }
                else {
                    ptr->count = ptr->count - 1;
                    Ex_ObjSetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL, 1);
                }
            }
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

BOOL _taggingboard_ptinregion(HEXOBJ hObj, FLOAT ptx, FLOAT pty, INT* index)
{
    BOOL ret          = FALSE;
    auto arr          = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
    auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
    auto sbOffsetTop  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
    if (arr->count > 1) {
        for (int i = 0; i < arr->count - 1; i++) {
            size_t ptrValue = 0;
            RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                          sizeof(size_t));
            EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
            HEXPATH     path;
            _path_create(1, &path);
            _path_open(path);
            _path_beginfigure(path);
            if (ptr->count > 0) {
                float startX = 0, startY = 0;
                float firstX = 0, firstY = 0;
                for (int j = 0; j < ptr->count; j++) {
                    float x = 0, y = 0;
                    RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                    RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);
                    if (j != 0) {
                        _path_addline(path, startX - sbOffsetLeft, startY - sbOffsetTop,
                                      x - sbOffsetLeft, y - sbOffsetTop);
                    }
                    if (j == 0) {
                        firstX = x;
                        firstY = y;
                    }
                    if (j == ptr->count - 1) {
                        _path_addline(path, x - sbOffsetLeft, y - sbOffsetTop,
                                      firstX - sbOffsetLeft, firstY - sbOffsetTop);
                    }
                    startX = x;
                    startY = y;
                }
            }
            _path_endfigure(path, TRUE);
            _path_close(path);
            ret = _path_hittest(path, ptx - sbOffsetLeft, pty - sbOffsetTop);
            if (ret) {
                *index = i + 1;
            }
            _path_destroy(path);
        }
    }
    return ret;
}

void _taggingboard_updatedraw(HEXOBJ hObj)
{
    HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CANVAS);
    _canvas_begindraw(canvas);

    auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_DATA);
    _canvas_clear(canvas, ExARGB(255, 255, 255, 0));
    auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
    // auto imgOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_LEFT_OFFSET);
    // auto imgOffsetTop = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_TOP_OFFSET);
    auto sbOffsetTop = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);
    auto tagging     = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_TAGGING);
    if (ptr->count > 0 && tagging == 1) {
        int      beginX = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_BEGINX);
        int      beginY = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_BEGINY);
        int      endX   = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ENDX);
        int      endY   = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ENDY);
        HEXBRUSH brush  = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN);

        _canvas_drawline(canvas, brush, beginX - sbOffsetLeft, beginY - sbOffsetTop,
                         endX - sbOffsetLeft, endY - sbOffsetTop, Ex_Scale(2), 0);
    }
    _canvas_enddraw(canvas);
    Ex_ObjInvalidateRect(hObj, 0);
}

void _taggingboard_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        HEXCANVAS canvas = (HEXCANVAS)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CANVAS);
        _canvas_clear(ps.hCanvas, ExARGB(255, 255, 255, 255));

        INT canvasWidth, canvasHeight;
        _canvas_getsize(ps.hCanvas, &canvasWidth, &canvasHeight);
        auto sbOffsetLeft = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_LEFT_OFFSET);
        auto sbOffsetTop  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_SB_TOP_OFFSET);

        auto bkg = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_BKG);
        if (bkg != 0) {
            auto  scalePtr = (LPVOID)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_IMG_SCALE);
            float scale;
            RtlMoveMemory(&scale, scalePtr, 4);
            INT width, height;
            _img_getsize(bkg, &width, &height);
            int newWidth  = (int)((float)width / scale);
            int newHeight = (int)((float)height / scale);
            int newLeft   = (canvasWidth - newWidth) / 2;
            int newTop    = (canvasHeight - newHeight) / 2;

            _canvas_drawimagerect(ps.hCanvas, bkg, newLeft - sbOffsetLeft, newTop - sbOffsetTop,
                                  newLeft + newWidth - sbOffsetLeft,
                                  newTop + newHeight - sbOffsetTop, 255);
        }

        HEXBRUSH brush   = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN);
        HEXBRUSH brush2  = (HEXBRUSH)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_PEN_FILL);
        auto     tagging = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_TAGGING);
        if (tagging == 1) {
            auto pointNull = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_POINT_NULL);
            // 点不为空画临时线
            if (!pointNull) {
                _canvas_drawcanvas(ps.hCanvas, canvas, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 255,
                                   CANVAS_COMPOSITE_MODE_SRCOVER);
            }
        }

        auto arr       = (EX_POLYGON_ARRAY*)Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_ARRAY);
        auto hitPath   = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_HIT_PATH);
        auto checkPath = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_CHECK_PATH);
        auto hitPoint  = Ex_ObjGetLong(hObj, TAGGINGBOARD_LONG_HIT_POINT);
        if (arr->count > 0) {
            for (int i = 0; i < arr->count; i++) {
                size_t ptrValue = 0;
                RtlMoveMemory(&ptrValue, (LPVOID)((size_t)arr->polygons + i * sizeof(size_t)),
                              sizeof(size_t));
                EX_POLYGON* ptr = (EX_POLYGON*)ptrValue;
                HEXPATH     path;
                _path_create(1, &path);
                _path_open(path);
                _path_beginfigure(path);
                if (ptr->count > 0) {
                    float startX = 0, startY = 0;
                    float endX = 0, endY = 0;
                    float firstX = 0, firstY = 0;
                    for (int j = 0; j < ptr->count; j++) {
                        float x = 0, y = 0;
                        RtlMoveMemory(&x, (LPVOID)((size_t)ptr->points + j * 8), 4);
                        RtlMoveMemory(&y, (LPVOID)((size_t)ptr->points + j * 8 + 4), 4);

                        // 第二个点开始画线
                        if (j != 0) {
                            _canvas_drawline(ps.hCanvas, brush, startX - sbOffsetLeft,
                                             startY - sbOffsetTop, x - sbOffsetLeft,
                                             y - sbOffsetTop, Ex_Scale(2), 0);
                            _path_addline(path, startX - sbOffsetLeft, startY - sbOffsetTop,
                                          x - sbOffsetLeft, y - sbOffsetTop);
                        }
                        if (j == 0) {
                            firstX = x;
                            firstY = y;
                        }
                        if (j == ptr->count - 1) {
                            // 最后一条线
                            _path_addline(path, x - sbOffsetLeft, y - sbOffsetTop,
                                          firstX - sbOffsetLeft, firstY - sbOffsetTop);
                        }
                        startX = x;
                        startY = y;

                        // 非最后一个路径的最后一条线
                        if (i != arr->count - 1 && j == ptr->count - 1) {
                            // 最后一条线
                            _canvas_drawline(ps.hCanvas, brush, firstX - sbOffsetLeft,
                                             firstY - sbOffsetTop, x - sbOffsetLeft,
                                             y - sbOffsetTop, Ex_Scale(2), 0);
                        }
                        // 命中临时第一个点
                        if (hitPoint - 1 == j) {
                            if (arr->count == 1) {
                                // 如果只有一个路径
                                _canvas_drawellipse(ps.hCanvas, brush, x - sbOffsetLeft,
                                                    y - sbOffsetTop, Ex_Scale(8), Ex_Scale(8),
                                                    Ex_Scale(2), 0);
                            }
                            else if (i == arr->count - 1) {
                                // 是最后一个路径
                                _canvas_drawellipse(ps.hCanvas, brush, x - sbOffsetLeft,
                                                    y - sbOffsetTop, Ex_Scale(8), Ex_Scale(8),
                                                    Ex_Scale(2), 0);
                            }
                            else {
                                // 不是最后一个路径
                                _canvas_drawellipse(ps.hCanvas, brush, x - sbOffsetLeft,
                                                    y - sbOffsetTop, Ex_Scale(2), Ex_Scale(2),
                                                    Ex_Scale(2), 0);
                            }
                        }
                        else {
                            // 普通点
                            _canvas_drawellipse(ps.hCanvas, brush, x - sbOffsetLeft,
                                                y - sbOffsetTop, Ex_Scale(2), Ex_Scale(2),
                                                Ex_Scale(2), 0);
                        }
                    }
                }
                _path_endfigure(path, TRUE);
                _path_close(path);
                if (hitPath == i + 1 && hitPath != checkPath) {
                    _canvas_fillpath(ps.hCanvas, path, brush2);
                }
                else if (checkPath == i + 1) {
                    _canvas_fillpath(ps.hCanvas, path, brush2);
                }
                _path_destroy(path);
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}