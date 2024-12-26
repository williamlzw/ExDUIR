#include "stdafx.h"

void _sliderbar_register()
{
    WCHAR wzCls[] = L"SliderBar";
    Ex_ObjRegister(wzCls,
                   OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                   NULL,
                   5 * sizeof(size_t),
                   NULL,
                   NULL,
                   _sliderbar_proc);
}

LRESULT CALLBACK _sliderbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    long tmp, tmp2;
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_MIN, 0);
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_MAX, 100);
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_POS, 0);
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE, 5);
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_CHECKED, ExARGB(255, 255, 255, 250), TRUE);
        Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(0, 0, 0, 125), TRUE);
    }
    else if (uMsg == WM_PAINT) {
        _sliderbar_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
        tmp  = (long)_sliderbar_getvalueofthepoint(hObj, lParam);
        tmp2 = Ex_ObjSetLong(hObj, SLIDERBAR_LONG_POS, tmp);
        if (tmp2 != tmp)   // 若当前位置变化,则发送通知
        {
            Ex_ObjDispatchNotify(hObj, SLIDERBAR_EVENT_VALUE, 0, (LPARAM)tmp);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSEMOVE) /*鼠标移动若为按下态,则更新当前位置*/
    {
        if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) != 0) {
            tmp  = (long)_sliderbar_getvalueofthepoint(hObj, lParam);
            tmp2 = Ex_ObjSetLong(hObj, SLIDERBAR_LONG_POS, tmp);
            if (tmp2 != tmp) {
                Ex_ObjDispatchNotify(hObj, SLIDERBAR_EVENT_VALUE, 0, (LPARAM)tmp);
                Ex_ObjInvalidateRect(hObj, 0);
            }
        }
    }
    else if (uMsg == WM_MOUSEWHEEL) /*鼠标滚轮滚动更新位置*/
    {
        lParam = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_POS) - (short)HIWORD(wParam) / 120;
        tmp    = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN);
        if (lParam < tmp) {
            lParam = tmp;
        }
        tmp = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MAX);
        if (lParam > tmp) {
            lParam = tmp;
        }
        tmp2 = Ex_ObjSetLong(hObj, SLIDERBAR_LONG_POS, lParam);
        if (tmp2 != lParam) {
            Ex_ObjDispatchNotify(hObj, SLIDERBAR_EVENT_VALUE, 0, lParam);
            Ex_ObjInvalidateRect(hObj, 0);
        }
    }
    else if (uMsg == SBM_GETPOS) {
        return (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_POS));
    }
    else if (uMsg == SBM_SETPOS) {
        tmp = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN);
        if (lParam < tmp) {
            lParam = tmp;
        }
        tmp = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MAX);
        if (lParam > tmp) {
            lParam = tmp;
        }
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_POS, lParam);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == SBM_SETRANGE) {
        if ((long)wParam > (long)lParam)   // 逆序则交换
        {
            tmp    = wParam;
            wParam = lParam;
            lParam = tmp;
        }
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_MIN, wParam);
        Ex_ObjSetLong(hObj, SLIDERBAR_LONG_MAX, lParam);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == SLIDERBAR_MESSAGE_GETBLOCKRECT) {
        _sliderbar_getrect(hObj, (RECT*)lParam);
    }
    else if (uMsg == SLIDERBAR_MESSAGE_PT2VALUE) {
        return ((LONG)_sliderbar_getvalueofthepoint(hObj, lParam));
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _sliderbar_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{0};
    RECT           RC = {0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL));
        Ex_ObjGetClientRect(hObj, &RC);
        FLOAT x, y;
        if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STYLE) & SLIDERBAR_STYLE_VERTICAL) ==
            SLIDERBAR_STYLE_VERTICAL) {
            _canvas_drawline(
                ps.hCanvas,
                hBrush,
                Ex_Scale((FLOAT)RC.right / 2),
                Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE),
                Ex_Scale((FLOAT)RC.right / 2),
                Ex_Scale((FLOAT)(RC.bottom)) - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE),
                Ex_Scale(2),
                D2D1_DASH_STYLE_SOLID);
            x = Ex_Scale((FLOAT)RC.right / 2);
            if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
                y = Ex_Scale((FLOAT)RC.bottom);
            }
            else {
                y = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE);
            }
        }
        else {
            _canvas_drawline(
                ps.hCanvas,
                hBrush,
                Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE),
                Ex_Scale((FLOAT)RC.bottom / 2),
                Ex_Scale((FLOAT)(RC.right)) - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE),
                Ex_Scale((FLOAT)RC.bottom / 2),
                Ex_Scale(2),
                D2D1_DASH_STYLE_SOLID);
            if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
                x = Ex_Scale((FLOAT)RC.right);
            }
            else {
                x = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE);
            }
            y = Ex_Scale((FLOAT)RC.bottom / 2);
        }
        _brush_setcolor(hBrush, Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED));
        _sliderbar_getrect(hObj, &RC);
        _canvas_fillellipse(ps.hCanvas,
                            hBrush,
                            Ex_Scale((FLOAT)(RC.left + RC.right)) / 2,
                            Ex_Scale((FLOAT)(RC.top + RC.bottom)) / 2,
                            Ex_Scale(Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                            Ex_Scale(Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)));
        _canvas_drawline(ps.hCanvas,
                         hBrush,
                         Ex_Scale((FLOAT)(RC.left + RC.right)) / 2,
                         Ex_Scale((FLOAT)(RC.top + RC.bottom)) / 2,
                         x,
                         y,
                         Ex_Scale(2),
                         D2D1_DASH_STYLE_SOLID);
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
    }
}

/*滑块条_取滑块矩形*/
void _sliderbar_getrect(HEXOBJ hObj, RECT* rc)
{
    FLOAT value = NULL;
    Ex_ObjGetClientRect(hObj, rc);
    value =
        (FLOAT)(Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MAX) - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN));
    value = (FLOAT)((Ex_ObjGetLong(hObj, SLIDERBAR_LONG_POS) -
                     Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN)) /
                    value);   // ' 得到百分比

    if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STYLE) & SLIDERBAR_STYLE_VERTICAL) ==
        SLIDERBAR_STYLE_VERTICAL) {
        if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
            *rc = RECT{
                (long)(rc->right / 2 - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) +
                       Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2 -
                       (rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value),
                (long)(rc->right / 2 + Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) -
                       (rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value)};
        }
        else {
            *rc = RECT{
                (long)(rc->right / 2 - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value),
                (long)(rc->right / 2 + Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value +
                       Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2)};
        }
    }
    else {
        if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
            *rc = RECT{
                (long)((rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) +
                       Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2 -
                       (rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value),
                (long)(rc->bottom / 2 - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) -
                       (rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value),
                (long)(rc->bottom / 2 + Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE))};
        }
        else {
            *rc = RECT{
                (long)((rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value),
                (long)(rc->bottom / 2 - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE)),
                (long)((rc->right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) * value +
                       Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2),
                (long)(rc->bottom / 2 + Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE))};
        }
    }
}

/*滑块条_取点所在值*/
FLOAT _sliderbar_getvalueofthepoint(HEXOBJ hObj, LPARAM lParam)
{
    RECT  RC    = {0};
    FLOAT value = NULL;
    Ex_ObjGetClientRect(hObj, &RC);

    if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STYLE) & SLIDERBAR_STYLE_VERTICAL) ==
        SLIDERBAR_STYLE_VERTICAL) {
        if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
            value =
                (Ex_Scale((FLOAT)RC.bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) -
                 (short)HIWORD(lParam)) /
                Ex_Scale((FLOAT)RC.bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2);
        }
        else {
            value =
                ((short)HIWORD(lParam) - Ex_Scale(Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE))) /
                Ex_Scale((FLOAT)RC.bottom - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2);
        }
    }
    else {
        if (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_DIRECTION) == 1) {
            value =
                (Ex_Scale((FLOAT)RC.right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2) -
                 (short)LOWORD(lParam)) /
                Ex_Scale((FLOAT)RC.right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2);
        }
        else {
            value =
                ((short)LOWORD(lParam) - Ex_Scale(Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE))) /
                Ex_Scale((FLOAT)RC.right - Ex_ObjGetLong(hObj, SLIDERBAR_LONG_BLOCK_SIZE) * 2);
        }
    }
    value = Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN) +
            value * (Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MAX) -
                     Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN));   // ' 得到百分比

    FLOAT tmp = (FLOAT)Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MIN);
    if (value < tmp) {
        value = tmp;
    }
    tmp = (FLOAT)Ex_ObjGetLong(hObj, SLIDERBAR_LONG_MAX);
    if (value > tmp) {
        value = tmp;
    }
    return value;
}