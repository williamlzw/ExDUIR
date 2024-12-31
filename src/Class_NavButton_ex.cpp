#include "stdafx.h"

void _navbtn_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    Ex_ObjBeginPaint(hObj, &ps);
    HEXIMAGE hImage = 0;
    if ((ps.dwState & STATE_DOWN) != 0 || (ps.dwState & STATE_CHECKED) != 0) {
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 2);
    }
    else if ((ps.dwState & STATE_HOVER) != 0) {
        hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
    }

    INT nImageWidth  = 0;
    INT nImageHeight = 0;
    if (hImage != 0) {
        _canvas_drawimagerect(ps.hCanvas, hImage, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top,
                              (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, 255);
    }
    FLOAT nTextWidth  = 0;
    FLOAT nTextHeight = 0;
    _canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj),
                         (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1, ps.dwTextFormat,
                         0, ps.uWidth, ps.uHeight, &nTextWidth, &nTextHeight);
    HEXIMAGE hImage2 = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
    if (hImage2 != 0) {
        _img_getsize(hImage2, &nImageWidth, &nImageHeight);
    }
    else {
        nImageWidth  = 0;
        nImageHeight = 0;
    }
    RECT rc;
    rc.top    = (ps.uHeight - (nTextHeight + nImageHeight)) / 2;
    rc.bottom = (ps.uHeight + nTextHeight + nImageHeight) / 2;
    if (hImage2 != 0) {
        _canvas_drawimage(ps.hCanvas, hImage2, (FLOAT)(ps.uWidth - nImageWidth) / 2, (FLOAT)rc.top,
                          255);
    }
    _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                     (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1, ps.dwTextFormat,
                     (ps.uWidth - nTextWidth) / 2, rc.bottom - nTextHeight,
                     (ps.uWidth + nTextWidth) / 2, rc.bottom);
    Ex_ObjEndPaint(hObj, &ps);
}

void _navbtn_notify_brother(HEXOBJ hObj)
{
    EX_CLASSINFO cls{0};
    EX_CLASSINFO clsTemp{0};
    Ex_ObjGetClassInfo(hObj, &cls);
    HEXOBJ hObjTmp = Ex_ObjGetObj(hObj, GW_HWNDFIRST);
    while (hObjTmp != 0) {
        if (hObj != hObjTmp) {
            Ex_ObjGetClassInfo(hObjTmp, &clsTemp);
            if (cls.atomName == clsTemp.atomName) {
                Ex_ObjDispatchMessage(hObjTmp, BM_SETCHECK, 1, 0);   // 发送一个取消选中的消息
            }
        }
        hObjTmp = Ex_ObjGetObj(hObjTmp, GW_HWNDNEXT);   // 找下一个兄弟组件
    }
}

LRESULT CALLBACK _navbtn_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {
        HEXIMAGE hImg1 = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
        if (hImg1 != 0) {
            _img_destroy(hImg1);
            Ex_ObjSetLong(hObj, 1, 0);
        }
        HEXIMAGE hImg2 = (HEXIMAGE)Ex_ObjGetLong(hObj, 2);
        if (hImg2 != 0) {
            _img_destroy(hImg2);
            Ex_ObjSetLong(hObj, 2, 0);
        }
        HEXIMAGE hImg0 = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
        if (hImg0 != 0) {
            _img_destroy(hImg0);
            Ex_ObjSetLong(hObj, 0, 0);
        }
    }
    else if (uMsg == WM_PAINT) {
        _navbtn_paint(hObj);
    }
    else if (uMsg == WM_SETICON) {
        HEXIMAGE hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 0);
        if (hImage != 0) {
            _img_destroy(hImage);
        }
        Ex_ObjSetLong(hObj, 0, lParam);
        if (wParam != 0) {
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == BM_SETIMAGE) {
        HEXIMAGE hImage = 0;

        if (wParam == 1) {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 1);
            Ex_ObjSetLong(hObj, 1, lParam);
        }
        else if (wParam == 2) {
            hImage = (HEXIMAGE)Ex_ObjGetLong(hObj, 2);
            Ex_ObjSetLong(hObj, 2, lParam);
        }
        if (hImage != 0) {
            _img_destroy(hImage);
        }
    }
    else if (uMsg == WM_MOUSEHOVER) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_EX_LCLICK) {
        Ex_ObjDispatchMessage(hObj, BM_SETCHECK, 1, 1);
    }
    else if (uMsg == BM_SETCHECK) {
        Ex_ObjSetUIState(hObj, STATE_CHECKED, lParam == 0, 0, wParam != 0);
        if (lParam != 0) {
            _navbtn_notify_brother(hObj);
        }
        Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _navbtn_register()
{
    Ex_ObjRegister(L"NavButton", OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_FOCUSABLE,
                   DT_CENTER | DT_VCENTER | DT_SINGLELINE, 3 * sizeof(size_t), 0, 0, _navbtn_proc);
}