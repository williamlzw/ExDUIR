#include "stdafx.h"

void _checkbox_register()
{
    WCHAR newwzCls[] = L"checkbox";
    Ex_ObjRegister(newwzCls, OBJECT_STYLE_VISIBLE | BUTTON_STYLE_CHECKBUTTON, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
        DT_VCENTER | DT_SINGLELINE, sizeof(SIZE_T), 0,
                   0, _checkbox_proc);
}

LRESULT CALLBACK _checkbox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

    if (uMsg == WM_CREATE) {
        Ex_ObjInitPropList(hObj, 6);
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRNORMAL, ExARGB(190, 190, 190, 255));
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRHOVER, ExARGB(200, 200, 200, 255));
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRCHECKED, ExARGB(0, 200, 200, 255));
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_TEXT, ExARGB(255, 255, 255, 255));
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRHOVERCHECK, ExARGB(194, 194, 194, 255));
        Ex_ObjSetLong(hObj, CHECKBOX_LONG_STATE, 0);
    }
    else if (uMsg == WM_DESTROY) {}
    else if (uMsg == WM_PAINT) {
        _checkbox_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOX_LONG_STATE);
        if (check == 0)
        {
            Ex_ObjSetLong(hObj, CHECKBOX_LONG_STATE, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOX_LONG_STATE);
        if (check == 1)
        {
            Ex_ObjSetLong(hObj, CHECKBOX_LONG_STATE, 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSEMOVE) {
        SetCursor(LoadCursorW(0, IDC_HAND));
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        SetCursor(LoadCursorW(0, IDC_HAND));
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOX_LONG_STATE);
        if (check == 2)
        {
            Ex_ObjSetLong(hObj, CHECKBOX_LONG_STATE, 1);
        }
        else if(check == 1) {
            Ex_ObjSetLong(hObj, CHECKBOX_LONG_STATE, 2);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_EX_LCLICK)
    {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOXEX_LONG_STATE);
        Ex_ObjSendMessage(hObj, BM_SETCHECK, check == 2, 0);
    }
    else if (uMsg == BM_SETCHECK) {
        Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);
    }
    else if (uMsg == BM_GETCHECK) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTONEX_LONG_STATE);
        return (state == 2 || state == 3) ? 1 : 0;
    }
    else if (uMsg == WM_EX_PROPS) {
        EX_OBJ_PROPS* checkboxprops = (EX_OBJ_PROPS*)lParam;
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRNORMAL, checkboxprops->crBkgNormal);
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRHOVER, checkboxprops->crBkgHover);
        Ex_ObjSetProp(hObj, CHECKBOX_PROP_CRCHECKED, checkboxprops->crBkgDownOrChecked);
    }
    else if (uMsg == WM_SETCURSOR)   // 禁控件自身再次设置光标产生闪烁
    {
        return 1;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _checkbox_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    RECT           rcBlock = {0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {

        HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, CHECKBOX_PROP_CRNORMAL));
        EXARGB   crText = Ex_ObjGetProp(hObj, CHECKBOX_PROP_CRNORMAL);
        /* 计算文本尺寸 */
        FLOAT nTextWidth  = NULL;
        FLOAT nTextHeight = NULL;
        _canvas_calctextsize(
            ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE),
            -1, ps.dwTextFormat, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

        /* 定义选择框矩形 */
        rcBlock.left   = ps.uWidth - ps.uHeight;
        rcBlock.top    = ps.rcPaint.top;
        rcBlock.right  = ps.rcPaint.right;
        rcBlock.bottom = ps.rcPaint.bottom;
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOX_LONG_STATE);
        if (check == 2) {
            /* 定义选中色 */
            crText = Ex_ObjGetProp(hObj, CHECKBOX_PROP_CRCHECKED);
            _brush_setcolor(hBrush, crText);
        }
        else if (check == 1) {
            _brush_setcolor(hBrush, Ex_ObjGetProp(hObj, CHECKBOX_PROP_CRHOVER));
            crText = Ex_ObjGetProp(hObj, CHECKBOX_PROP_CRHOVERCHECK);
        }
        /* 填充矩形和绘制边框 */
        _canvas_fillrect(ps.hCanvas, hBrush, 0, 0, (FLOAT)rcBlock.left, (FLOAT)ps.uHeight);
        _canvas_drawrect(ps.hCanvas, hBrush, 0, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, Ex_Scale(1),
                         D2D1_DASH_STYLE_SOLID);

        _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText, L"✔", -1, DT_CENTER | DT_VCENTER,
                         (FLOAT)rcBlock.left, (FLOAT)rcBlock.top, (FLOAT)rcBlock.right,
                         (FLOAT)rcBlock.bottom);

        /* 绘制组件文本 */
        _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetProp(hObj, CHECKBOX_PROP_TEXT),
                         (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                         DT_CENTER | DT_VCENTER | DT_SINGLELINE, (FLOAT)ps.rcText.left,
                         (FLOAT)ps.rcText.top, (FLOAT)rcBlock.left, (FLOAT)ps.rcText.bottom);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
