#include "stdafx.h"

void _checkbuttonex_register()
{
    WCHAR newwzCls[] = L"CheckButtonEx";
    Ex_ObjRegister(newwzCls, OBJECT_STYLE_VISIBLE | BUTTON_STYLE_CHECKBUTTON, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
        DT_VCENTER | DT_SINGLELINE, 1 * sizeof(SIZE_T), 0,
                   0, _checkbuttonex_proc);
}

LRESULT CALLBACK _checkbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjInitPropList(hObj, 5);
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBKGDOWNORCHECKED, ExRGB2ARGB(16777215, 255));
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERNORMAL, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERHOVER, ExARGB(0, 0, 0, 255));
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERDOWNORCHECKED, ExARGB(0, 0, 0, 255));
        Ex_ObjSetLong(hObj, CHECKBOXEX_LONG_STATE, 0);
    }
    /*销毁时释放资源*/
    else if (uMsg == WM_DESTROY) {}
    else if (uMsg == WM_PAINT) {
        _checkbuttonex_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOXEX_LONG_STATE);
        if (check == 0)
        {
            Ex_ObjSetLong(hObj, CHECKBOXEX_LONG_STATE, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOXEX_LONG_STATE);
        if (check == 1)
        {
            Ex_ObjSetLong(hObj, CHECKBOXEX_LONG_STATE, 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOXEX_LONG_STATE);
        if (check == 2)
        {
            Ex_ObjSetLong(hObj, CHECKBOXEX_LONG_STATE, 1);
        }
        else if (check == 1) {
            Ex_ObjSetLong(hObj, CHECKBOXEX_LONG_STATE, 2);
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
        EX_OBJ_PROPS* CheckButtonExprops = (EX_OBJ_PROPS*)lParam;
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBKGDOWNORCHECKED,
                      CheckButtonExprops->crBkgDownOrChecked);
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERNORMAL, CheckButtonExprops->crBorderNormal);
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERHOVER, CheckButtonExprops->crBorderHover);
        Ex_ObjSetProp(hObj, CHECKBOXEX_PROP_CRBORDERDOWNORCHECKED,
                      CheckButtonExprops->crBorderDownOrChecked);
    }

    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _checkbuttonex_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    RECT           rcBlock = {0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        HEXBRUSH hBrush =
            _brush_create((EXARGB)Ex_ObjGetProp(hObj, CHECKBOXEX_PROP_CRBORDERNORMAL));
        EXARGB crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBOXEX_LONG_STATE);
        if (check == 2) {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED);
            _brush_setcolor(hBrush,
                (EXARGB)Ex_ObjGetProp(hObj, CHECKBOXEX_PROP_CRBORDERDOWNORCHECKED));
        }
        else if(check == 1) {
            crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
            _brush_setcolor(hBrush, (EXARGB)Ex_ObjGetProp(hObj, CHECKBOXEX_PROP_CRBORDERHOVER));
        }

        
        /* 计算文本尺寸 */
        FLOAT nTextWidth  = NULL;
        FLOAT nTextHeight = NULL;
        _canvas_calctextsize(
            ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE),
            -1, ps.dwTextFormat, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight, &nTextWidth, &nTextHeight);

        /* 定义选择框矩形 */
        rcBlock.left   = ps.rcPaint.left + (long)Ex_Scale(2);
        rcBlock.top    = (ps.uHeight - (long)nTextHeight) / 2;
        rcBlock.right  = rcBlock.left + (long)nTextHeight;
        rcBlock.bottom = (ps.uHeight + (long)nTextHeight) / 2;
        /* 绘制边框 */
        _canvas_drawrect(ps.hCanvas, hBrush, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top,
                         (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom, 1, D2D1_DASH_STYLE_SOLID);

        /* 定义选中色 */
        EXARGB CHECKCLR = (EXARGB)Ex_ObjGetProp(hObj, CHECKBOXEX_PROP_CRBKGDOWNORCHECKED);

        _brush_setcolor(hBrush, CHECKCLR);

   
        if (check == 2) {
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), CHECKCLR, L"✔", -1,
                             DT_CENTER | DT_VCENTER, (FLOAT)rcBlock.left, (FLOAT)rcBlock.top,
                             (FLOAT)rcBlock.right, (FLOAT)rcBlock.bottom);
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
