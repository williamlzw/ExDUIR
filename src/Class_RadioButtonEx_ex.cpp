#include "stdafx.h"

void _radiobuttonex_register()
{
    WCHAR newwzCls[] = L"RadioButtonEx";
    Ex_ObjRegister(newwzCls, OBJECT_STYLE_VISIBLE | BUTTON_STYLE_RADIOBUTTON,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE, sizeof(SIZE_T), 0,
                   0, _radiobuttonex_proc);
}

LRESULT CALLBACK _radiobuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjInitPropList(hObj, 5);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED, ExRGB2ARGB(16777215, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERNORMAL, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED, ExARGB(0, 0, 0, 255));
        Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 0); // 0:正常 1:悬停 2:选中
    }
    else if (uMsg == WM_PAINT) {
        _radiobuttonex_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        if (state == 0) {
            Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        if (state == 1) {
            Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        if (state != 2) {
            // 单选框被点击且未选中时，设置为选中状态并通知兄弟控件
            Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 2);
            Ex_ObjInvalidateRect(hObj, 0);
            if (wParam == 1)
            {
                // 通知兄弟控件取消选中
                INT    nError = 0;
                obj_s* pObj = nullptr;
                if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                    _obj_notify_brothers(hWnd, hObj, pObj, BM_SETCHECK, 0, 0, TRUE, TRUE);
                }
            }
            // 发送通知
            Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);
        }
    }
    else if (uMsg == BM_SETCHECK) {
        if (wParam == 0) {
            Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 0);
        }
        else {
            Ex_ObjSetLong(hObj, RADIOBUTTONEX_LONG_STATE, 2);
        }
        Ex_ObjInvalidateRect(hObj, 0);
        return 0;
    }
    else if (uMsg == BM_GETCHECK) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        return (state == 2 || state == 3) ? 1 : 0;
    }
    else if (uMsg == WM_EX_PROPS) {
        EX_OBJ_PROPS* RadioButtonExprops = (EX_OBJ_PROPS*)lParam;

        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED,
                      RadioButtonExprops->crBkgDownOrChecked);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERNORMAL, RadioButtonExprops->crBorderNormal);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER, RadioButtonExprops->crBorderHover);
        Ex_ObjSetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED,
                      RadioButtonExprops->crBorderDownOrChecked);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _radiobuttonex_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    RECT           rcBlock = {0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER));
        EXARGB   crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);

        if (state == 2) {
            _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERDOWNORCHECKED));
        }
        else if (state == 1) {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
            _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBORDERHOVER));
        }
        /* 计算文本尺寸 */
        FLOAT nTextWidth  = NULL;
        FLOAT nTextHeight = NULL;
        _canvas_calctextsize(
            ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE),
            -1, ps.dwTextFormat, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

        /* 定义单选框圆角矩形 */
        rcBlock.left   = ps.rcPaint.left + (long)Ex_Scale(2);
        rcBlock.top    = (ps.uHeight - (long)nTextHeight) / 2;
        rcBlock.right  = rcBlock.left + (long)nTextHeight;
        rcBlock.bottom = (ps.uHeight + (long)nTextHeight) / 2;
        /* 绘制边框[GDI模式下，圆角度需要缩减一像素] */
        _canvas_drawroundedrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top,
                                (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom, nTextHeight / 2 - 1,
                                nTextHeight / 2 - 1, 1, D2D1_DASH_STYLE_SOLID);

        /* 定义选中色 */
        _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, RADIOBUTTONEX_PROP_CRBKGDOWNORCHECKED));

        if (state == 2) {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED);
            /* 把矩形往里缩3像素 */
            rcBlock.left   = rcBlock.left + (long)Ex_Scale(3);
            rcBlock.top    = rcBlock.top + (long)Ex_Scale(3);
            rcBlock.right  = rcBlock.right - (long)Ex_Scale(3);
            rcBlock.bottom = rcBlock.bottom - (long)Ex_Scale(3);
            FLOAT nRadius  = nTextHeight / 2 - Ex_Scale(3);
            _canvas_fillroundedrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top,
                                    (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom, nRadius, nRadius);
        }

        /* 绘制组件文本 */
        _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText,
                         (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                         DT_LEFT | DT_VCENTER, (FLOAT)ps.rcText.left + nTextHeight + Ex_Scale(7),
                         (FLOAT)ps.rcText.top, (FLOAT)ps.rcText.right, (FLOAT)ps.rcText.bottom);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
