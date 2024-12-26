#include "stdafx.h"

void _mosaic_loading_register()
{
    WCHAR wzCls[] = L"MosaicLoading";
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, 0, 2 * sizeof(size_t), 0, 0, _mosaic_loading_proc);
}

LRESULT CALLBACK _mosaic_loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        Ex_ObjSetLong(hObj, 0, 1);
        Ex_ObjSetLong(hObj, 1, (LONG_PTR)malloc(sizeof(MosaicRect[8])));
        Ex_ObjSetTimer(hObj, 180);
    }
    else if (uMsg == WM_PAINT)
    {
        _mosaic_loading_paint((HEXOBJ)hObj);
    }
    else if (uMsg == WM_TIMER)
    {
        int index = Ex_ObjGetLong(hObj, 0);
        Ex_ObjSetLong(hObj, 0, index + 1);
        if (index > 8)
        {
            Ex_ObjSetLong(hObj, 0, 1);
        }
        INT nAlpha = 30;
        MosaicRect* ptr = (MosaicRect*)Ex_ObjGetLong(hObj, 1);
        for (int i = 0; i < 8; i++)
        {
            if (i >= index)
            {
                ptr[i].alpha = nAlpha;
                nAlpha = nAlpha + 10;
            }
        }
        for (int i = 0; i < 8; i++)
        {
            if (i < index)
            {
                ptr[i].alpha = nAlpha;
                nAlpha = nAlpha + 10;
            }
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_DESTROY)
    {
        Ex_ObjKillTimer(hObj);
        MosaicRect* ptr = (MosaicRect*)Ex_ObjGetLong(hObj, 1);
        free(ptr);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _mosaic_loading_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    MosaicRect* ptr = (MosaicRect*)Ex_ObjGetLong(hObj, 1);
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        INT x[3], y[3];

        INT nWidth = ps.uWidth, nHeight = ps.uHeight;
        INT nSize = NULL;
        if (nWidth > nHeight)
        {
            nSize = (nHeight - 2) / 3;
        }
        else
        {
            nSize = (nWidth - 2) / 3;
        }
        x[0] = (nWidth - (nSize * 3 + 2)) / 2;
        y[0] = (nHeight - (nSize * 3 + 2)) / 2;
        x[1] = x[0] + (nSize + 1) * 1;
        y[1] = y[0] + (nSize + 1) * 1;
        x[2] = x[0] + (nSize + 1) * 2;
        y[2] = y[0] + (nSize + 1) * 2;

        ptr[0].rc = RECT{x[0], y[0], x[0] + nSize, y[0] + nSize};
        ptr[1].rc = RECT{x[1], y[0], x[1] + nSize, y[0] + nSize};
        ptr[2].rc = RECT{x[2], y[0], x[2] + nSize, y[0] + nSize};
        ptr[3].rc = RECT{x[2], y[1], x[2] + nSize, y[1] + nSize};
        ptr[4].rc = RECT{x[2], y[2], x[2] + nSize, y[2] + nSize};
        ptr[5].rc = RECT{x[1], y[2], x[1] + nSize, y[2] + nSize};
        ptr[6].rc = RECT{x[0], y[2], x[0] + nSize, y[2] + nSize};
        ptr[7].rc = RECT{x[0], y[1], x[0] + nSize, y[1] + nSize};

        for (INT i = 0; i < 8; i++)
        {
            INT iAlphaValue = ptr[i].alpha * 255 / 100;
            COLORREF ThemeColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
            if (ThemeColor == 0)
            {
                ThemeColor = RGB(58, 144, 210);
            }
            HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(ThemeColor, iAlphaValue));
            _canvas_fillrect(ps.hCanvas, hBrush, ptr[i].rc.left, ptr[i].rc.top, ptr[i].rc.right, ptr[i].rc.bottom);
            _brush_destroy(hBrush);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
}