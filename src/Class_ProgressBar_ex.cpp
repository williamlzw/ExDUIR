#include "stdafx.h"

void _progressbar_register()
{
    WCHAR wzCls[] = L"ProgressBar";
    Ex_ObjRegister(
        wzCls, OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE, NULL,
        6 * sizeof(size_t), NULL, NULL, _progressbar_proc);
}

LRESULT CALLBACK _progressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, 0);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RANGE, 255);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RADIUS, 10);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POINT, 0);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_BACKGROUNDCOLOR, ExARGB(192, 211, 241, 255));
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_FOREGROUNDCOLOR, ExARGB(47, 110, 239, 255));
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
    }
    else if (uMsg == WM_PAINT) {
        _progressbar_paint(hObj);
    }
    return (Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

void _progressbar_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        size_t   Radius = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RADIUS);
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_BACKGROUNDCOLOR));
        _canvas_fillroundedrect(ps.hCanvas, hBrush, ps.rcPaint.left, ps.rcPaint.top,
                                ps.rcPaint.right, ps.rcPaint.bottom, Radius, Radius);
        size_t nPos   = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POS);
        size_t nRange = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
        int    point  = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POINT);
        if (nPos > nRange) {
            nPos = nRange;
            Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, nPos);
        }

        FLOAT pbD;
        if (point == 0) {
            // 水平进度条：从左到右
            if (nRange > 0) {
                pbD = (FLOAT)nPos / nRange * (ps.rcPaint.right - ps.rcPaint.left);
            }
            else {
                pbD = (FLOAT)nPos / 255 * (ps.rcPaint.right - ps.rcPaint.left);
            }
            pbD += ps.rcPaint.left;   // 将相对宽度转换为绝对坐标

            _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_FOREGROUNDCOLOR));
            _canvas_fillroundedrect(ps.hCanvas, hBrush, ps.rcPaint.left, ps.rcPaint.top, pbD,
                                    ps.rcPaint.bottom, Radius, Radius);
            wchar_t buffer[10]; 
            if (nRange == 0) {
                // 避免除以零错误
                swprintf_s(buffer, L"0%%");
            }
            else {
                int percent = (INT)((FLOAT)(nPos) / nRange * 100.0f);
                swprintf_s(buffer, L"%d%%", percent);
            }
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                             Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), buffer, -1,
                             DT_RIGHT | DT_SINGLELINE | DT_VCENTER, ps.rcPaint.left, ps.rcPaint.top,
                             pbD - 5, ps.rcPaint.bottom);
        }
        else {
            // 垂直进度条：从下到上
            FLOAT totalHeight = (FLOAT)(ps.rcPaint.bottom - ps.rcPaint.top);

            if (nRange > 0) {
                pbD = totalHeight * ((FLOAT)nPos / nRange);
            }
            else {
                pbD = totalHeight * ((FLOAT)nPos / 255);
            }

            // 计算填充矩形的底部位置，即总高度减去已填充的高度
            FLOAT filledBottom = ps.rcPaint.bottom - pbD ;

            _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_FOREGROUNDCOLOR));
            _canvas_fillroundedrect(ps.hCanvas, hBrush, ps.rcPaint.left, filledBottom,
                                    ps.rcPaint.right, ps.rcPaint.bottom, Radius, Radius);
            HEXMATRIX  mx = _matrix_create();
            HEXMATRIX old_mx = _matrix_create();
            _canvas_gettransform(ps.hCanvas, old_mx);
            _canvas_gettransform(ps.hCanvas, mx);
            _matrix_translate(mx, (FLOAT)ps.uWidth / 2, (FLOAT)(filledBottom + 10));
            _matrix_rotate(mx, (FLOAT) -90);
            _matrix_translate(mx, -(FLOAT)ps.uWidth / 2, -(FLOAT)(filledBottom + 10 ));
            _canvas_settransform(ps.hCanvas, mx);
            wchar_t buffer[10]; 
            if (nRange == 0) {
                // 避免除以零错误
                swprintf_s(buffer, L"0%%");
            }
            else {
                int percent = (INT)((FLOAT)(nPos) / nRange * 100.0f);
                swprintf_s(buffer, L"%d%%", percent);
            }
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj),
                             Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), buffer, -1,
                             DT_LEFT | DT_SINGLELINE, ps.rcPaint.left - 20, filledBottom ,
                             ps.rcPaint.right, ps.rcPaint.bottom );
  
            _canvas_settransform(ps.hCanvas, old_mx);
            _matrix_destroy(mx);
            _matrix_destroy(old_mx);
        }
       
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
