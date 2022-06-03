#include "Class_Carousel_ex.h"

void _carousel_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    Ex_ObjBeginPaint(hObj, &ps);
    HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
    if (hImageList != 0)
    {
        INT index = (INT)Ex_ObjGetLong(hObj, ECL_INDEX);
        HEXIMAGE hImage = _imglist_get(hImageList, index);
        _canvas_drawimagerect(ps.hCanvas, hImage, (FLOAT)ps.rcPaint.left, (FLOAT)ps.rcPaint.top, (FLOAT)ps.rcPaint.right, (FLOAT)ps.rcPaint.bottom, 255);
    }
    
    Ex_ObjEndPaint(hObj, &ps);
}

LRESULT CALLBACK _carousel_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        Ex_ObjSetTimer(hObj, 2000);
        Ex_ObjSetLong(hObj, ECL_HIMAGELIST, 0);
        Ex_ObjSetLong(hObj, ECL_INDEX, 1);
    }
    else if (uMsg == WM_DESTROY)
    {
        Ex_ObjKillTimer(hObj);
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            _imglist_destroy(hImageList);
            Ex_ObjSetLong(hObj, ECL_HIMAGELIST, 0);
            Ex_ObjSetLong(hObj, ECL_INDEX, 1);
        }
    }
    else if (uMsg == WM_TIMER)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, ECL_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index += 1;
                if (index == count + 1)
                {
                    index = 1;
                }
                Ex_ObjSetLong(hObj, ECL_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CM_NEXT)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, ECL_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index += 1;
                if (index == count + 1)
                {
                    index = 1;
                }
                Ex_ObjSetLong(hObj, ECL_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CM_PREV)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            INT index = (INT)Ex_ObjGetLong(hObj, ECL_INDEX);
            INT count = _imglist_count(hImageList);
            if (count > 0)
            {
                index -= 1;
                if (index == 0)
                {
                    index = count;
                }
                Ex_ObjSetLong(hObj, ECL_INDEX, index);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == CM_SIZE)
    {
        Ex_ObjSetLong(hObj, ECL_WIDTH, wParam);
        Ex_ObjSetLong(hObj, ECL_HEIGHT, lParam);
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList == 0)
        {
            HEXIMAGELIST hImageList = _imglist_create(wParam, lParam);
            Ex_ObjSetLong(hObj, ECL_HIMAGELIST, (size_t)hImageList);
        }
    }
    else if (uMsg == CM_ADDIMG)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            _imglist_addimage(hImageList, lParam, 0);
        }
    }
    else if (uMsg == CM_CLEAR)
    {
        HEXIMAGELIST hImageList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ECL_HIMAGELIST);
        if (hImageList != 0)
        {
            Ex_ObjSetLong(hObj, ECL_INDEX, 1);
            _imglist_destroy(hImageList);
            auto width = (INT)Ex_ObjGetLong(hObj, ECL_WIDTH);
            auto height = (INT)Ex_ObjGetLong(hObj, ECL_HEIGHT);
            hImageList = _imglist_create(width, height);
            Ex_ObjSetLong(hObj, ECL_HIMAGELIST, (size_t)hImageList);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == CM_SETTIMER)
    {
        Ex_ObjKillTimer(hObj);
        Ex_ObjSetTimer(hObj, lParam);
    }
    else if (uMsg == WM_PAINT)
    {
        _carousel_paint(hObj);
    }
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _carousel_register()
{
	Ex_ObjRegister(L"Carousel", EOS_VISIBLE, EOS_EX_TABSTOP | EOS_EX_FOCUSABLE, DT_CENTER | DT_VCENTER , 4 * sizeof(size_t), 0, 0, _carousel_proc);
}