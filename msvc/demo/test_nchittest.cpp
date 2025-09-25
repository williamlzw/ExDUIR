#include "test_nchittest.h"

LRESULT CALLBACK OnNchitTestMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                    LRESULT* lpResult)
{
    if (uMsg == WM_NCHITTEST) {
        if (!(GET_X_LPARAM(lParam) >= 20 && GET_X_LPARAM(lParam) <= 60 &&
              GET_Y_LPARAM(lParam) >= 20 && GET_Y_LPARAM(lParam) <= 60)) {
            return HTTRANSPARENT;
        }
    }
    else if (uMsg == WM_ERASEBKGND)   // wParam画布句柄
    {
        RECT rc{0};
        Ex_ObjGetRect(hObj, &rc);
        HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(16746496, 255));
        _canvas_drawrect(wParam, hBrush, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, 0);
        _brush_setcolor(hBrush, ExRGB2ARGB(255, 255));
        _canvas_fillrect(wParam, hBrush, 20, 20, 60, 60);
        _brush_destroy(hBrush);
        return 1;
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        OUTPUTW(L"被按下");
    }
    else if (uMsg == WM_LBUTTONUP) {
        OUTPUTW(L"被放开");
    }
    return 0;
}

void test_nchittest(HWND hWnd)
{
    HWND hWnd_nchittest =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试限制通知区域", 0, 0, 400, 200, 0, 0);
    HEXDUI hExDui_nchittest = Ex_DUIBindWindowEx(
        hWnd_nchittest, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_nchittest, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", L"鼠标只能在红色区域里响应", -1, 50, 50,
                   300, 100, hExDui_nchittest, 0, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0,
                   OnNchitTestMsgProc);
    Ex_DUIShowWindow(hExDui_nchittest, SW_SHOWNORMAL);
}
