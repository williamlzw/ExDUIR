#include "test_customredraw.h"


HEXCANVAS _canvas_draw(HEXDUI hExDui, INT width, INT height) {
    // 创建独立画布作为双缓冲
    HEXCANVAS hCanvas = _canvas_createindependent(width, height, 0);

    if (hCanvas && _canvas_begindraw(hCanvas))  // 开始绘制独立画布
    {
        // 在独立画布上绘制背景
        _canvas_setantialias(hCanvas, TRUE);
        RECT rc{ 0 };
        Ex_DUIGetClientRect(hExDui, &rc);
        FLOAT    arrPts[] = { 0.0f, 1.0f };
        INT      arrColors[] = { ExARGB(10, 127, 213, 220), ExARGB(200, 10, 10, 220) };
        HEXBRUSH hBrush = _brush_createlinear_ex(0, 0, rc.right, rc.bottom, arrPts, arrColors);
        _canvas_fillellipse(hCanvas, hBrush,
            width / 2, height / 2,
            width / 2 - 2, height / 2 - 2);
        _brush_destroy(hBrush);

        // 在独立画布上绘制红色矩形
        HEXBRUSH hRedBrush = _brush_create(0xFFFF0000);
        _canvas_drawrect(hCanvas, hRedBrush, 50, 50, 250, 150, 2.0f, 1);
        _brush_destroy(hRedBrush);

        _canvas_enddraw(hCanvas);  // 结束独立画布绘制

    }

    //if (hCanvas) _canvas_destroy(hCanvas);
    return hCanvas;
}
HEXCANVAS pCanvas = NULL;

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        INT width = LOWORD(lParam);
        INT height = HIWORD(lParam);
        // 将双缓冲内容输出到窗口
        if (pCanvas) _canvas_alphablend(wParam, pCanvas, 0, 0, width, height, 0, 0, width, height, 255);
        *lpResult = 1;
        return 1;
    }
    if (uMsg == WM_DESTROY)
    {
        if (pCanvas) _canvas_destroy(pCanvas);
        pCanvas = NULL;
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
    pCanvas = _canvas_draw(hExDui_customredraw,300,200);

    Ex_DUISetLong(hExDui_customredraw, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCreateEx(OBJECT_STYLE_EX_TOPMOST, L"sysbutton", L"",
                   OBJECT_STYLE_VISIBLE | WINDOW_STYLE_BUTTON_CLOSE, (300 - 32) / 2, (200 - 32) / 2,
                   32, 32, hExDui_customredraw, 0, 0, 0, 0,
                   NULL);   // 改变标题栏按钮位置
    Ex_DUIShowWindow(hExDui_customredraw, SW_SHOWNORMAL, 0, 0, 0);
}
