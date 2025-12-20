#include "test_dragobj.h"


LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, lParam);
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, 0);
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
    }
    else if (uMsg == WM_MOUSEMOVE) {
        if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) == STATE_DOWN) {
            auto userdata = Ex_ObjGetLong(hObj, OBJECT_LONG_USERDATA);
            // 获取按下位置
            POINT ptOrg;
            ptOrg.x = GET_X_LPARAM(userdata);
            ptOrg.y = GET_Y_LPARAM(userdata);
            // 获取当前鼠标位置
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            auto parent = Ex_ObjGetParent(hObj);
            RECT rcParent{ 0 };
            // 获取组件矩形
            RECT rcObj{ 0 };
            Ex_ObjGetRect(hObj, &rcObj);
            if (parent != 0) {
                Ex_ObjGetRect(parent, &rcParent);
                int x = rcObj.left + pt.x - ptOrg.x;
                int y = rcObj.top + pt.y - ptOrg.y;
                if (x <= 0) {
                    x = 0;
                }
                if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left)) {
                    x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
                }
                if (y <= 0) {
                    y = 0;
                }
                if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top)) {
                    y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
                }
                Ex_ObjSetPos(hObj, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
            else {
                HEXDUI hexdui = Ex_DUIFromWindow(hWnd);
                Ex_DUIGetClientRect(hexdui, &rcParent);
                int x = rcObj.left + pt.x - ptOrg.x;
                int y = rcObj.top + pt.y - ptOrg.y;
                if (x <= 0) {
                    x = 0;
                }
                if (x > rcParent.right - rcParent.left - (rcObj.right - rcObj.left)) {
                    x = rcParent.right - rcParent.left - (rcObj.right - rcObj.left);
                }
                if (y <= 0) {
                    y = 0;
                }
                if (y > rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top)) {
                    y = rcParent.bottom - rcParent.top - (rcObj.bottom - rcObj.top);
                }
                Ex_ObjSetPos(hObj, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }
    }
    return 0;
}

void test_dragobj(HWND hWnd)
{
    HWND hWnd_dragobj = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试拖动组件", 0, 0, 500, 500, 0, 0);
    HEXDUI hExDui_dragobj = Ex_DUIBindWindowEx(
        hWnd_dragobj, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
        WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_dragobj, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_label1 =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", L"可拖动组件1",
            OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, 25, 35, 250, 250, hExDui_dragobj,
            0, DT_SINGLELINE | DT_LEFT | DT_TOP, 0, 0, OnDragMsgProc);
    if (hObj_label1 != 0) {
        Ex_ObjSetColor(hObj_label1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
        HEXOBJ hObj_label2 =
            Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", L"可拖动组件2",
                OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, 25, 35, 150, 150,
                hObj_label1, 0, DT_SINGLELINE | DT_LEFT | DT_TOP, 0, 0, OnDragMsgProc);
        if (hObj_label2 != 0) {
            Ex_ObjSetColor(hObj_label2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16722680, 100), TRUE);
        }
    }
    Ex_DUIShowWindow(hExDui_dragobj, SW_SHOWNORMAL);
}
