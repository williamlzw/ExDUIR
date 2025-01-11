#include "test_matrix.h"


LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult)
{
    if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        Ex_ObjBeginPaint(hObj, &ps);
        _canvas_clear(ps.hCanvas, ExRGB2ARGB(16777215, 100));
        DOUBLE nCurrent = (DOUBLE)Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM) / 100;

        HEXMATRIX mx = _matrix_create();

        _matrix_translate(mx, (FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2);
        _matrix_rotate(mx, (FLOAT)nCurrent * 90);
        _matrix_scale(mx, 1 + (FLOAT)nCurrent * 0.25, 1 + (FLOAT)nCurrent * 0.25);
        _matrix_translate(mx, -(FLOAT)ps.uWidth / 2, -(FLOAT)ps.uHeight / 2);
        _canvas_settransform(ps.hCanvas, mx);

        HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND));

        _canvas_fillellipse(ps.hCanvas, hBrush, (FLOAT)ps.uWidth / 2, (FLOAT)ps.uHeight / 2, 75.f,
                            50.f);
        _brush_destroy(hBrush);

        _canvas_settransform(ps.hCanvas, 0);
        _matrix_destroy(mx);
        Ex_ObjEndPaint(hObj, &ps);
        *lpResult = 1;
        return 1;
    }
    else if (uMsg == WM_EX_EASING) {
        EX_EASINGINFO es{0};
        RtlMoveMemory(&es, (void*)lParam, sizeof(EX_EASINGINFO));
        if (es.nProgress == 1)   // 如果进度=1则表示缓动结束
        {
            Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, 0);   // 设置控件当前缓动指针为空
        }
        Ex_ObjSetLong(hObj, OBJECT_LONG_LPARAM, es.nCurrent);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        HEXEASING hEasing =
            _easing_create(EASING_TYPE_OUTELASTIC, 0,
                           EASING_MODE_SINGLE | EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY,
                           hObj, 500, 20, EASING_STATE_PLAY, 0, 100, 0, 0, 0, 0);
        HEXEASING oldhEasing =
            (HEXEASING)Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, (size_t)hEasing);
        if (oldhEasing != 0)   // 如果前一个缓动未结束,则停止前面的缓动
        {
            _easing_setstate(oldhEasing, EASING_STATE_STOP);
        }
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, FALSE);   // 设置悬浮状态
    }
    else if (uMsg == WM_MOUSELEAVE) {
        HEXEASING hEasing = _easing_create(EASING_TYPE_OUTELASTIC, 0,
                                           EASING_MODE_SINGLE | EASING_MODE_THREAD |
                                               EASING_MODE_DISPATCHNOTIFY | EASING_MODE_REVERSE,
                                           hObj, 500, 20, EASING_STATE_PLAY, 0, 100, 0, 0, 0, 0);
        HEXEASING oldhEasing =
            (HEXEASING)Ex_ObjSetLong(hObj, OBJECT_LONG_USERDATA, (size_t)hEasing);
        if (oldhEasing != 0)   // 如果前一个缓动未结束,则停止前面的缓动
        {
            _easing_setstate(oldhEasing, EASING_STATE_STOP);
        }
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, FALSE);   // 删除悬浮状态
    }
    return 0;
}

void test_matrix(HWND hWnd)
{
    HWND   hWnd_matrix = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试矩阵", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_matrix = Ex_DUIBindWindowEx(
        hWnd_matrix, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_matrix, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", 0, -1, 50, 50, 200, 250,
                                 hExDui_matrix, 0, DT_VCENTER, 0, 0, OnMatrixMsgProc);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 255), TRUE);

    Ex_DUIShowWindow(hExDui_matrix, SW_SHOWNORMAL, 0, 0, 0);
}
