#include "Class_Carousel_ex.h"

void _carousel_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    Ex_ObjBeginPaint(hObj, &ps);
	_canvas_setantialias(ps.hCanvas, TRUE);
	_canvas_setimageantialias(ps.hCanvas, TRUE);
    HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
    if (hImageList != 0)
    {
        INT index = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_INDEX);
        HEXIMAGE hImage = _imglist_get(hImageList, index);
        _canvas_drawimagerect(ps.hCanvas, hImage, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 255);
        INT count = _imglist_count(hImageList);
        for (int i = 1; i < count + 1; i++)
        {
            int x = ps.rcPaint.right - (count + 1) * Ex_Scale(20) + i * Ex_Scale(20) - Ex_Scale(2);
            int y = ps.rcPaint.bottom - Ex_Scale(17);
            if (i == index)
            {
                _carousel_drawcircle(ps.hCanvas, x, y, Ex_Scale(5), ExRGB2ARGB(16777215, 255));
            }
            else {
                _carousel_drawcircle(ps.hCanvas, x, y, Ex_Scale(5), ExRGB2ARGB(0, 70));
            }
        }
    }
    Ex_ObjEndPaint(hObj, &ps);
}

void _carousel_drawcircle(HEXCANVAS canvas, INT x, INT y, INT radius, EXARGB color)
{
    HEXBRUSH brush = _brush_create(color);
    _canvas_fillellipse(canvas, brush, x - radius, y - radius, radius, radius);
    _brush_destroy(brush);
}

LRESULT CALLBACK _carousel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        Ex_ObjSetTimer(hObj, 2000);
        Ex_ObjSetLong(hObj, CAROUSEL_LONG_HIMAGELIST, 0);
        Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, 1);
    }
    else if (uMsg == WM_DESTROY)
    {
        Ex_ObjKillTimer(hObj);
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            _imglist_destroy(hImageList);
            Ex_ObjSetLong(hObj, CAROUSEL_LONG_HIMAGELIST, 0);
            Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, 1);
        }
    }
    else if (uMsg == WM_TIMER)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index += 1;
                if (index == count + 1)
                {
                    index = 1;
                }
                Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_NEXT)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index += 1;
                if (index == count + 1)
                {
                    index = 1;
                }
                Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_PREV)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index -= 1;
                if (index == 0)
                {
                    index = count;
                }
                Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_SIZE)
    {
        Ex_ObjSetLong(hObj, CAROUSEL_LONG_WIDTH, wParam);
        Ex_ObjSetLong(hObj, CAROUSEL_LONG_HEIGHT, lParam);
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList == 0)
        {
            HEXIMAGELIST hImageList = _imglist_create(wParam, lParam);
            Ex_ObjSetLong(hObj, CAROUSEL_LONG_HIMAGELIST, (size_t)hImageList);
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_ADDIMG)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            _imglist_addimage(hImageList, 0, lParam);
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_CLEAR)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, 1);
            _imglist_destroy(hImageList);
            auto width = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_WIDTH);
            auto height = (INT)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HEIGHT);
            hImageList = _imglist_create(width, height);
            Ex_ObjSetLong(hObj, CAROUSEL_LONG_HIMAGELIST, (size_t)hImageList);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == CAROUSEL_MESSAGE_SETTIMER)
    {
        Ex_ObjKillTimer(hObj);
        Ex_ObjSetTimer(hObj, lParam);
    }
    else if (uMsg == WM_PAINT)
    {
        _carousel_paint(hObj);
    }
    else if (uMsg == WM_LBUTTONDOWN)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, CAROUSEL_LONG_HIMAGELIST);
        if (hImageList != 0)
        {
            INT count = _imglist_count(hImageList);
            RECT rc;
            Ex_ObjGetClientRect(hObj, &rc);
            auto x = GET_X_LPARAM(lParam);
            auto y = GET_Y_LPARAM(lParam);
            auto dpi = Ex_DUIGetSystemDpi();
            auto width = rc.right - rc.left;
            auto height = rc.bottom - rc.top;
            auto index = (INT)(count - (FLOAT)(width - x / dpi + 8) / 20 + 2);
            if (index > 0 && index <= count && (y / dpi) >= (height - 27))
            {
                Ex_ObjSetLong(hObj, CAROUSEL_LONG_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _carousel_register()
{
	Ex_ObjRegister(L"Carousel", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_FOCUSABLE, DT_CENTER | DT_VCENTER , 4 * sizeof(size_t), 0, 0, _carousel_proc);
}