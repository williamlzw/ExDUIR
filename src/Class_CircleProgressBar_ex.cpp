#include "stdafx.h"

void _circleprogressbar_register()
{
    WCHAR wzCls[] = L"CircleProgressBar";
    Ex_ObjRegister(
        wzCls, OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE, NULL,
        7 * sizeof(size_t), NULL, NULL, _circleprogressbar_proc);
}

LRESULT CALLBACK _circleprogressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                         LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_POS, 0);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_RANGE, 100);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_LINEHEIGHT, 10);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_LINECAP, 0);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_FONT, 0);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_BACKGROUNDCOLOR, ExARGB(192, 211, 241, 255));
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_FOREGROUNDCOLOR, ExARGB(47, 110, 239, 255));
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
    }
    else if (uMsg == WM_DESTROY)
    {
        HEXFONT hFont = (HEXFONT)Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_FONT);
        if (hFont != 0)
        {
            _font_destroy(hFont);
        }
    }
    else if (uMsg == WM_PAINT) {
        _circleprogressbar_paint(hObj);
    }

    return (Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

void _circleprogressbar_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        size_t nPos   = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POS);
        size_t nRange = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
        if (nPos > nRange) {
            nPos = nRange;
            Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, nPos);
        }
        HEXBRUSH hBrush =
            _brush_create(Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_BACKGROUNDCOLOR));
        UINT lineHeight = Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_LINEHEIGHT);
        UINT lineCap       = Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_LINECAP);
        UINT halfLineWidth = ceil(lineHeight / 2);
        UINT circleRadius  = floor(min(ps.uWidth, ps.uHeight) / 2);

        //画背景圆环
        HEXPATH path;
        _path_create(PATH_FLAG_DISABLESCALE, &path);
        _path_open(path);
        float left     = halfLineWidth;
        float top      = halfLineWidth;
        float right    = circleRadius * 2 - halfLineWidth;
        float bottom   = circleRadius * 2 - halfLineWidth;
        float center_y = (top + bottom) / 2;
        _path_beginfigure2(path, left, center_y);
        _path_addellipse(path, left, top, right, bottom);
        _path_endfigure(path, TRUE);
        _path_close(path);
        _canvas_drawpath(ps.hCanvas, path, hBrush, lineHeight - 2, 0);
        _path_destroy(path);

        //画前景进度
        _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_FOREGROUNDCOLOR));
        HEXPATH path2;
        _path_create(PATH_FLAG_DISABLESCALE, &path2);
        _path_open(path2);
        FLOAT angle      = 360 * (FLOAT)nPos / nRange;   // 角度制
        FLOAT startAngle = -90;                          // 起始角度从 12 点钟方向开始,D2D默认0角度从3点钟方向
        FLOAT endAngle   = startAngle + angle;           // 结束角度
        // 动态调整 arcSize
        BOOL bArcSize = (angle >= 180) ? TRUE : FALSE;   // 大于等于 180 度时使用大弧
        // 计算圆弧的起点和终点
        FLOAT startX = circleRadius + (circleRadius - halfLineWidth) * cos(startAngle * PI / 180);
        FLOAT startY = circleRadius + (circleRadius - halfLineWidth) * sin(startAngle * PI / 180);
        FLOAT endX   = circleRadius + (circleRadius - halfLineWidth) * cos(endAngle * PI / 180);
        FLOAT endY   = circleRadius + (circleRadius - halfLineWidth) * sin(endAngle * PI / 180);
        _path_beginfigure2(path2, startX , startY );
        _path_addarc3(path2, endX, endY, circleRadius - halfLineWidth, circleRadius - halfLineWidth,
                      startAngle,
                      endAngle, TRUE,
                      bArcSize);
        _path_endfigure(path2, FALSE);
        _path_close(path2);
        _canvas_drawpath(ps.hCanvas, path2, hBrush, lineHeight , 0,
                         lineCap == 0 ? D2D1_CAP_STYLE_SQUARE : D2D1_CAP_STYLE_ROUND);
        _path_destroy(path2);
        _brush_destroy(hBrush);

        wchar_t buffer[10];
        if (nRange == 0) {
            // 避免除以零错误
            swprintf_s(buffer, L"0%%");
        }
        else {
            int percent = (INT)((FLOAT)(nPos) / nRange * 100.0f);
            swprintf_s(buffer, L"%d%%", percent); 
        }
        HEXFONT      hFont = (HEXFONT)Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_FONT);
        if (hFont == 0)
        {
            hFont = Ex_ObjGetFont(hObj);
        }
        _canvas_drawtext(ps.hCanvas, hFont,
                         Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), buffer, -1,
                         DT_CENTER | DT_SINGLELINE | DT_VCENTER, lineHeight, lineHeight,
                         ps.uWidth - lineHeight, ps.uHeight - lineHeight);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
