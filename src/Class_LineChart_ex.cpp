#include "stdafx.h"

void _linechart_register()
{
    Ex_ObjRegister(L"LineChart", OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER, sizeof(size_t),
        LoadCursor(0, IDC_HAND), CANVAS_FLAG_TEXTANTIALIAS, _linechart_proc);
}

void _linechart_update_polygon(EX_POLYGON* ptr, int newY)
{
    if (ptr == nullptr || ptr->points == nullptr || ptr->count <= 0)
    {
        return; // 检查指针和数据的有效性
    }

    // 将 LPVOID 转换为 POINT* 以便操作
    POINT* pointsArray = static_cast<POINT*>(ptr->points);
    // 将 Y 坐标往前移动
    for (int i = 0; i < ptr->count - 1; i++)
    {
        pointsArray[i].y = pointsArray[i + 1].y; // 将后一个点的 Y 坐标赋值给前一个点
    }

    // 更新最后一个点
    pointsArray[ptr->count - 1].y = newY;
}

LRESULT CALLBACK _linechart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        auto ptr = (EX_POLYGON*)malloc(sizeof(EX_POLYGON));
        ptr->points = malloc(21 * sizeof(POINT));
        ptr->count = 21;
        RECT rc;
        Ex_ObjGetRect(hObj, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        POINT* pointsArray = static_cast<POINT*>(ptr->points);
        
        for (int i = 0; i < 21; i++)
        {
            float x = i * (width - 1) / 20, y = height;
            pointsArray[i].x = x;
            pointsArray[i].y = y;
        }

        Ex_ObjSetLong(hObj, LINECHART_LONG_DATA, (LONG_PTR)ptr);
    }
    else if (uMsg == WM_DESTROY) {
        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, LINECHART_LONG_DATA);
        free(ptr->points);
        free(ptr);
    }
    else if (uMsg == LINECHART_MESSAGE_SETVALUE)
    {
        if (lParam < 1) lParam = 1;
        if (lParam > 100) lParam = 100;
        RECT rc;
        Ex_ObjGetRect(hObj, &rc);
        int height = rc.bottom - rc.top;
        int y = height - (int)((float)lParam * height / 100);
        if (y < 1) y = 1;
        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, LINECHART_LONG_DATA);
        _linechart_update_polygon(ptr, y);
    }
    else if (uMsg == WM_TIMER)
    {
        int y = rand() % 100 + 1;

        Ex_ObjSendMessage(hObj, LINECHART_MESSAGE_SETVALUE, 0, y);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_PAINT)
    {
        _linechart_paint(hObj);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _linechart_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        _canvas_setantialias(ps.hCanvas, TRUE);
        int intervalX = (int)(ps.uWidth - 1 ) / 20;
        int intervalY = (int)(ps.uHeight - 1) / 10;
        auto hBrush = _brush_create(ExARGB(150, 150, 150, 255));
        for (int i = 0; i < 11; i++)
        {
            _canvas_drawline(ps.hCanvas, hBrush, 0, i * (ps.uHeight - 1) / 10, ps.uWidth, i * (ps.uHeight - 1) / 10, 1, 0);
        }
    
        for (int i = 0; i < 21; i++)
        {
            _canvas_drawline(ps.hCanvas, hBrush, i * (ps.uWidth - 1) / 20 , ps.uHeight, i * (ps.uWidth - 1) / 20, 0, 1, 0);
        }
        HEXPATH path1;
        _path_create(PATH_FLAG_DISABLESCALE, &path1);
        _path_open(path1);
        _path_beginfigure2(path1,0, ps.uHeight - 1);
        auto ptr = (EX_POLYGON*)Ex_ObjGetLong(hObj, LINECHART_LONG_DATA);
        POINT* pointsArray = static_cast<POINT*>(ptr->points);
        std::vector<D2D1_POINT_2F> d2dOutline1;
        for (int i = 0; i < 21; i++)
        {
            d2dOutline1.push_back(D2D1::Point2F(pointsArray[i].x, pointsArray[i].y));
        }

        std::vector<D2D1_BEZIER_SEGMENT> beziers1;
        ConvertCurveToBezier(d2dOutline1, 0.5f, beziers1);
        for (const auto& seg : beziers1)
        {
            _path_addbezier(path1, seg.point1.x, seg.point1.y, seg.point2.x, seg.point2.y,
                seg.point3.x, seg.point3.y);
        }
        _path_addline(path1, ps.uWidth - 1, pointsArray[20].y, ps.uWidth - 1, ps.uHeight - 1);//最右侧
        _path_addline(path1, ps.uWidth - 1, ps.uHeight - 1, 0-1, ps.uHeight - 1);//最底部
        _path_addline(path1, 0 -1, ps.uHeight - 1, 0-1, pointsArray[0].y);//最左侧

        _path_endfigure(path1, TRUE);
        _path_close(path1);

        _brush_setcolor(hBrush, ExARGB(65, 105, 225, 255));
        _canvas_drawpath(ps.hCanvas, path1, hBrush, 2, 0, 1);
        _brush_setcolor(hBrush, ExARGB(135, 206, 250, 200));
        _canvas_fillpath(ps.hCanvas, path1, hBrush);

        _path_destroy(path1);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}