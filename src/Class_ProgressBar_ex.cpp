#include "Class_ProgressBar_ex.h"

void _progressbar_register()
{
    WCHAR wzCls[] = L"ProgressBar";
    Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE, NULL, 5 * sizeof(size_t), NULL, NULL, _progressbar_proc);
}

LRESULT CALLBACK _progressbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, 0);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RANGE, 255);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RADIUS, 10);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_BKCOLOR, ExARGB(192, 211, 241, 255));
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_BARCOLOR, ExARGB(47, 110, 239, 255));
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_GETPOS)
    {
        return Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POS);
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_GETRANGE)
    {
        return Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_SETPOS)
    {
        size_t nRange = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
        if (wParam >= nRange)
        {
            wParam = nRange;
        }
        return (Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, wParam));
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_SETRANGE)
    {
        return (Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RANGE, wParam));
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_SETRADIUS)
    {
        return (Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_RADIUS, wParam));
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_SETBARCOLOR)
    {
        return (Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_BARCOLOR, wParam));
    }
    else if (uMsg == PROGRESSBAR_MESSAGE_SETBKCOLOR)
    {
        return (Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_BKCOLOR, wParam));
    }
    else if (uMsg == WM_PAINT)
    {
        _progressbar_paint(hObj);
    }
    return (Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam));
}

void _progressbar_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        size_t Radius = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RADIUS);
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_BKCOLOR));
        _canvas_fillroundedrect(ps.hCanvas, hBrush, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, Radius, Radius);
        size_t nPos = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POS);
        size_t nRange = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
        FLOAT pbD;
        if (nRange > 0)
        {
            pbD = (FLOAT)nPos / nRange * ps.rcPaint.right;
        }
        else
        {
            pbD = (FLOAT)nPos / 255 * ps.rcPaint.right;
        }
        if (pbD < ps.rcPaint.bottom)
        {
            pbD = ps.rcPaint.bottom;
        }
        _brush_setcolor(hBrush, Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_BARCOLOR));
        _canvas_fillroundedrect(ps.hCanvas, hBrush, ps.rcPaint.left, ps.rcPaint.top, pbD, ps.rcPaint.bottom, Radius, Radius);
        std::wstring text = std::to_wstring((INT)((FLOAT)nPos / nRange * 100)) + L"%";
        _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), text.c_str(), -1, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, ps.uHeight / 2, 0, pbD - 5, ps.uHeight);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}