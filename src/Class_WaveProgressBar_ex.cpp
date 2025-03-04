#include "stdafx.h"

void _waveprogressbar_register()
{
    WCHAR wzCls[] = L"WaveProgressBar";
    Ex_ObjRegister(
        wzCls, OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE, NULL,
        7 * sizeof(size_t), NULL, NULL, _waveprogressbar_proc);
}

LRESULT CALLBACK _waveprogressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_POS, 0);
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_WIDTH, 30);
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_HEIGHT, 15);
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_LEFT_X, -30);
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_FONT, 0);
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_BACKGROUNDCOLOR, ExARGB(208, 211, 217, 255));
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_FOREGROUNDCOLOR, ExARGB(20, 126, 255, 255));
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
    }
    else if (uMsg == WM_DESTROY)
    {
        HEXFONT hFont = (HEXFONT)Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_FONT);
        if (hFont != 0) {
            _font_destroy(hFont);
        }
    }
    else if (uMsg == WM_PAINT)
    {
        _waveprogressbar_paint(hObj);
    }
    else if (uMsg == WM_TIMER)
    {
        int intX = Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_LEFT_X);
        intX -= 10;
        if (intX == (INT)Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_WIDTH) * -2)
        {
            intX = (INT)Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_WIDTH) * -1;
        }
        Ex_ObjSetLong(hObj, WAVEPROGRESSBAR_LONG_LEFT_X, intX);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    return (Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

void ConvertCurveToBezier(const std::vector<D2D1_POINT_2F>& points, float tension,
                          std::vector<D2D1_BEZIER_SEGMENT>& beziers)
{
    if (points.size() < 2) return;

    // 首尾添加虚拟点以保证连续性
    std::vector<D2D1_POINT_2F> paddedPoints;
    paddedPoints.push_back(points[0]);   // 复制第一个点作为虚拟起点
    paddedPoints.insert(paddedPoints.end(), points.begin(), points.end());
    paddedPoints.push_back(points.back());   // 复制最后一个点作为虚拟终点

    for (size_t i = 1; i < paddedPoints.size() - 2; ++i) {
        const auto& p0 = paddedPoints[i - 1];
        const auto& p1 = paddedPoints[i];
        const auto& p2 = paddedPoints[i + 1];
        const auto& p3 = paddedPoints[i + 2];

        // 计算三次贝塞尔控制点
        float         s  = tension * 0.3f;
        D2D1_POINT_2F c1 = {p1.x + s * (p2.x - p0.x), p1.y + s * (p2.y - p0.y)};
        D2D1_POINT_2F c2 = {p2.x - s * (p3.x - p1.x), p2.y - s * (p3.y - p1.y)};

        beziers.push_back(D2D1::BezierSegment(c1, c2, p2));
    }
}

void _waveprogressbar_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        _canvas_setantialias(ps.hCanvas, TRUE);
        int intX = Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_LEFT_X);
        int waveHeight = Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_HEIGHT);
        int pos        = Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_POS);
        if (pos > 100) {
            pos = 100;
            Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, pos);
        }
        int      value      = (int)((float)pos / 100.0 * ps.uHeight - waveHeight);
 
        int waveWidth = Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_WAVE_WIDTH);
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_BACKGROUNDCOLOR));
        _canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.uWidth, ps.uHeight);

        HEXPATH path1;
        _path_create(PATH_FLAG_DISABLESCALE, &path1);
        _path_open(path1);
        _path_beginfigure2(path1, intX, ps.uHeight - value);

        HEXPATH path2;
        _path_create(PATH_FLAG_DISABLESCALE, &path2);
        _path_open(path2);
        _path_beginfigure2(path2, intX + (float)waveWidth / 2, ps.uHeight - value);

        std::vector<D2D1_POINT_2F> d2dOutline1;
        std::vector<D2D1_POINT_2F> d2dOutline2;

        while (intX < (int)ps.uWidth + waveWidth) {

            d2dOutline1.push_back(D2D1::Point2F(intX, ps.uHeight - value));
            d2dOutline1.push_back(
                D2D1::Point2F(intX + (float)waveWidth / 2, ps.uHeight - value - waveHeight));
            d2dOutline2.push_back(D2D1::Point2F(intX + (float)waveWidth / 2, ps.uHeight - value));
            d2dOutline2.push_back(D2D1::Point2F(intX + (float)waveWidth / 2 + (float)waveWidth / 2,
                                                ps.uHeight - value - waveHeight));
            intX += waveWidth;
        }
        
        std::vector<D2D1_BEZIER_SEGMENT> beziers1;
        ConvertCurveToBezier(d2dOutline1, 0.5f, beziers1);

        std::vector<D2D1_BEZIER_SEGMENT> beziers2;
        ConvertCurveToBezier(d2dOutline2, 0.5f, beziers2);
        
        for (const auto& seg : beziers1)
        {
            _path_addbezier(path1, seg.point1.x, seg.point1.y, seg.point2.x, seg.point2.y,
                            seg.point3.x, seg.point3.y);
        }
        _path_addline(path1, ps.uWidth + 1, -1, ps.uWidth + 1, ps.uHeight + value);
        _path_addline(path1, ps.uWidth + 1, ps.uHeight + value, -1, ps.uHeight + value);
        _path_addline(path1, -1, ps.uHeight + value, -1, -1);
        _path_endfigure(path1, TRUE);
        _path_close(path1);

         
        for (const auto& seg : beziers2) {
            _path_addbezier(path2, seg.point1.x, seg.point1.y, seg.point2.x, seg.point2.y,
                            seg.point3.x, seg.point3.y);
            
        }
        _path_addline(path2, ps.uWidth + 1, -1, ps.uWidth + 1, ps.uHeight + value);
        _path_addline(path2, ps.uWidth + 1, ps.uHeight + value, -1, ps.uHeight + value);
        _path_addline(path2, -1, ps.uHeight + value, -1, -1);
        _path_endfigure(path2, TRUE);
        _path_close(path2);

        _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_FOREGROUNDCOLOR));
        _canvas_fillpath(ps.hCanvas, path1, hBrush);
        _canvas_fillpath(ps.hCanvas, path2, hBrush);

        _path_destroy(path1);
        _path_destroy(path2);
        _brush_destroy(hBrush);
        std::wstring text  = std::to_wstring((INT)((FLOAT)pos / 100 * 100)) + L"%";
        HEXFONT      hFont = (HEXFONT)Ex_ObjGetLong(hObj, WAVEPROGRESSBAR_LONG_FONT);
        if (hFont == 0) {
            hFont = Ex_ObjGetFont(hObj);
        }
        _canvas_drawtext(ps.hCanvas, hFont, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                         text.c_str(), -1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, 4,
                         4, ps.uWidth - 4, ps.uHeight - 4);
        Ex_ObjEndPaint(hObj, &ps);
    }
}
