#include "test_customredraw.h"


HEXCANVAS _canvas_draw(HEXDUI hExDui, INT width, INT height) {
    // 创建独立画布作为双缓冲
    HEXCANVAS hCanvas = _canvas_createindependent(width, height, CANVAS_FLAG_GDI_COMPATIBLE);

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

    return hCanvas;
}

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        INT width = LOWORD(lParam);
        INT height = HIWORD(lParam);
        HEXCANVAS pCanvas = _canvas_draw(hExDui, width, height);
        // 将双缓冲内容输出到窗口
        if (pCanvas) _canvas_alphablend(wParam, pCanvas, 0, 0, width, height, 0, 0, width, height, 255);
        if (pCanvas) _canvas_destroy(pCanvas);
        pCanvas = NULL;
        *lpResult = 1;
        return 1;
    }
    return 0;
}
void test_layered_window(HWND hParent)
{
    INT   exStyle = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
    // 创建分层窗口
    HWND hLayeredWnd = CreateWindowExW(
        exStyle, L"Ex_DirectUI", 0, WS_POPUP, 100, 100, 300, 200, hParent, 0, 0, 0);

    // 创建画布
    HEXCANVAS hCanvas = _canvas_createindependent(300, 200, 0);

    if (hCanvas && _canvas_begindraw(hCanvas)) {
        // 在画布上绘制内容
        HEXBRUSH hBrush = _brush_create(0x80FF0000); // 半透明红色
        _canvas_fillrect(hCanvas, hBrush, 0, 0, 300, 200);
        _brush_destroy(hBrush);

        HEXFONT hFont = _font_createfromfamily(L"Arial", 16, 0);
        if (hFont) {
            _canvas_drawtext(hCanvas, hFont, 0xFFFFFFFF, L"我是分层窗口,未绑定DUI", -1,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                0, 0, 300, 200);
            _font_destroy(hFont);
        }

        // 获取 DC
        HDC hDC = _canvas_getdc(hCanvas);
        // 使用 UpdateLayeredWindow 更新分层窗口
        POINT ptDst = { 0, 0 };
        POINT ptSrc = { 0, 0 };
        SIZE size = { 300, 200 };
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
        UpdateLayeredWindow(hLayeredWnd, hDC, &ptDst, &size, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
        
        
        _canvas_enddraw(hCanvas);
        // 释放 DC
        _canvas_releasedc(hCanvas);

        // 获取屏幕工作区的尺寸
        int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
        // 获取窗口的尺寸
        RECT windowRect;
        GetWindowRect(hLayeredWnd, &windowRect);
        int windowWidth = windowRect.right - windowRect.left;
        int windowHeight = windowRect.bottom - windowRect.top;
        // 计算居中位置
        int x = (screenWidth - windowWidth) / 2;
        int y = (screenHeight - windowHeight) / 2;
        // 移动窗口到居中位置
        MoveWindow(hLayeredWnd, x, y, windowWidth, windowHeight, TRUE);
        // 显示窗口
        ShowWindow(hLayeredWnd, SW_SHOW);
       
        // 清理资源
        _canvas_destroy(hCanvas);
    }
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


    test_layered_window(hWnd_customredraw);


    Ex_DUIShowWindow(hExDui_customredraw, SW_SHOWNORMAL, 0, 0, 0);
}
