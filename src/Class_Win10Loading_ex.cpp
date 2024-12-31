#include "stdafx.h"

void _win10_loading_register()
{
    Ex_ObjRegister(L"Win10Loading", OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, NULL, 2 * sizeof(size_t),
                   NULL, NULL, _win10_loading_proc);
}

LRESULT CALLBACK _win10_loading_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        float* ptr = (float*)malloc(sizeof(float[2][720]));
        Ex_ObjSetLong(hObj, 0, (LONG_PTR)ptr);
        float* floatPtr = (float*)malloc(sizeof(size_t));
        floatPtr[0]     = 0;
        Ex_ObjSetLong(hObj, 1, (LONG_PTR)floatPtr);
        if ((Ex_ObjGetLong(hObj, OBJECT_LONG_STYLE) & LOADING_STYLE_LINE) != LOADING_STYLE_LINE) {
            RECT rc = {0};
            INT  r  = NULL; /*半径*/
            /*获取用户矩形*/
            Ex_ObjGetClientRect(hObj, &rc);

            if ((rc.right - rc.left) < (rc.bottom - rc.top)) {
                r = (INT)(rc.right - rc.left) / 3;
            }
            else {
                r = (INT)(rc.bottom - rc.top) / 3;
            }

            DOUBLE pi = 3.1415926f;
            for (INT i = 0, a = 90; i < 720; i++, a++) {
                ((FLOAT(*)[720])ptr)[0][i] = (FLOAT)(r * cos(a * pi / 180));
                ((FLOAT(*)[720])ptr)[1][i] = (FLOAT)(r * sin(a * pi / 180));
            }
        }
        Ex_ObjSetTimer(hObj, 10);
    }
    else if (uMsg == WM_DESTROY) {
        Ex_ObjKillTimer(hObj);
        float* ptr      = (float*)Ex_ObjGetLong(hObj, 0);
        float* floatPtr = (float*)Ex_ObjGetLong(hObj, 1);
        free(ptr);
        free(floatPtr);
    }
    else if (uMsg == WM_PAINT) {
        _win10_loading_paint(hObj);
    }
    else if (uMsg == WM_TIMER) {
        Ex_ObjInvalidateRect(hObj, 0);
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _win10_loading_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps;
    float*         ptr = (float*)Ex_ObjGetLong(hObj, 0);
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        _canvas_setantialias(ps.hCanvas, TRUE);
        EXARGB ThemeColor = Ex_ObjGetColor(hObj, COLOR_EX_BACKGROUND);
        if (ThemeColor == 0) {
            ThemeColor = RGB(0, 136, 255);
        }
        HEXBRUSH hBrush = _brush_create(ExRGB2ARGB(ThemeColor, 255));
        if ((ps.dwStyle & LOADING_STYLE_LINE) == LOADING_STYLE_LINE) {
            FLOAT  a;
            float* i = (float*)Ex_ObjGetLong(hObj, 1);
            i[0]     = i[0] + 0.005;
            if (i[0] > 1 + 0.3) {
                i[0] = 0;
            }
            a = i[0];

            for (INT j = 0; j < 6; j++) {
                FLOAT n = (FLOAT)(4 * (i[0] - 0.5) * (i[0] - 0.5) * (i[0] - 0.5) + 0.5 + i[0]) / 2;
                i[0]    = (FLOAT)(i[0] - 0.045);
                _canvas_fillellipse(ps.hCanvas, hBrush, (FLOAT)(ps.uWidth * n),
                                    (FLOAT)(ps.uHeight * 3 / 5), (FLOAT)Ex_Scale(3),
                                    (FLOAT)Ex_Scale(3));
            }
            i[0] = a;
            Ex_ObjSetLong(hObj, 1, (LONG_PTR)i);
        }
        else {
            FLOAT  n = NULL;
            FLOAT  a;
            float* i = (float*)Ex_ObjGetLong(hObj, 1);
            i[0]     = i[0] + 0.01;
            if (i[0] > 3.2) {
                i[0] = 0;
            }
            a = i[0];

            for (INT j = 1; j <= 5; j++) {
                if (i[0] <= 1) {
                    n = (FLOAT)(4 * (i[0] - 0.5) * (i[0] - 0.5) * (i[0] - 0.5) + 0.5 + i[0]) / 2;
                }

                if (i[0] > 1) {
                    i[0] = i[0] - 1;
                    n = (FLOAT)((4 * (i[0] - 0.5) * (i[0] - 0.5) * (i[0] - 0.5) + 0.5 + i[0]) / 2 +
                                1);
                    i[0] = i[0] + 1;
                }
                i[0] = i[0] - 0.1;

                if ((INT)(360 * n) > 0 && (INT)(360 * n) < 720) {
                    _canvas_fillellipse(ps.hCanvas, hBrush,
                                        (FLOAT)((ps.uWidth - ptr[(INT)(360 * n)]) / 2 +
                                                Ex_Scale(ptr[(INT)(360 * n)])),
                                        (FLOAT)((ps.uHeight - ptr[(INT)(360 * n) + 720]) / 2 +
                                                Ex_Scale(ptr[(INT)(360 * n) + 720])),
                                        (FLOAT)Ex_Scale(3), (FLOAT)Ex_Scale(3));
                }
            }
            i[0] = a;
            Ex_ObjSetLong(hObj, 1, (LONG_PTR)i);
        }

        _canvas_drawtext(ps.hCanvas, (HEXFONT)Ex_ObjGetLong(hObj, OBJECT_LONG_HFONT),
                         Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
                         (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                         DT_BOTTOM | DT_VCENTER | DT_CENTER | DT_SINGLELINE, (FLOAT)ps.rcText.left,
                         (FLOAT)(ps.uHeight * 3 / 5), (FLOAT)ps.rcText.right,
                         (FLOAT)ps.rcText.bottom);
        _brush_destroy(hBrush);

        Ex_ObjEndPaint(hObj, &ps);
    }
}