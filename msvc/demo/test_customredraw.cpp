#include "test_customredraw.h"

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        _canvas_setantialias(wParam, TRUE);
        RECT rc{0};
        Ex_DUIGetClientRect(hExDui, &rc);
        FLOAT    arrPts[]    = {0.0f, 1.0f};
        INT      arrColors[] = {ExARGB(10, 127, 213, 220), ExARGB(200, 10, 10, 220)};
        HEXBRUSH hBrush      = _brush_createlinear_ex(0, 0, rc.right, rc.bottom, arrPts, arrColors);
        _canvas_fillellipse(wParam, hBrush, LOWORD(lParam) / 2, HIWORD(lParam) / 2,
                            LOWORD(lParam) / 2 - 2, HIWORD(lParam) / 2 - 2);
        _brush_destroy(hBrush);
        *lpResult = 1;
        return 1;
    }
    return 0;
}

void test_customredraw(HWND hWnd)
{
    // 异型窗口采用重画背景形式，才不会产生锯齿。用于需要圆角，不规则图形的窗口。
    HWND   hWnd_customredraw   = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 300, 200, 0, 0);
    HEXDUI hExDui_customredraw = Ex_DUIBindWindowEx(
        hWnd_customredraw, 0,
        WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW, 0,
        OnCustomRedrawWndMsgProc);
    Ex_DUISetLong(hExDui_customredraw, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCreateEx(OBJECT_STYLE_EX_TOPMOST, L"sysbutton", L"",
                   OBJECT_STYLE_VISIBLE | WINDOW_STYLE_BUTTON_CLOSE, (300 - 32) / 2, (200 - 32) / 2,
                   32, 32, hExDui_customredraw, 0, 0, 0, 0,
                   NULL);   // 改变标题栏按钮位置
    Ex_DUIShowWindow(hExDui_customredraw, SW_SHOWNORMAL, 0, 0, 0);
}
