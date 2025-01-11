#include "test_circleprogressbar.h"

LRESULT CALLBACK OnCircleProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_TIMER) {
        size_t nPos   = Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_POS);
        size_t nRange = Ex_ObjGetLong(hObj, CIRCLEPROGRESSBAR_LONG_RANGE);
        Ex_ObjSetLong(hObj, CIRCLEPROGRESSBAR_LONG_POS, nPos + Random(1, 10));
        Ex_ObjInvalidateRect(hObj, 0);
        if (nRange == nPos) {
            Ex_ObjKillTimer(hObj);
        }
    }

    return 0;
}

void test_circleprogressbar(HWND hWnd)
{
    HWND hWnd_progressbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试环形进度条", 0, 0, 400, 500, 0, 0);
    HEXDUI hExDui_progressbar = Ex_DUIBindWindowEx(
        hWnd_progressbar, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_progressbar, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_progressbar =
        Ex_ObjCreate(L"CircleProgressBar", NULL, -1, 50, 100, 100, 100, hExDui_progressbar);
    Ex_ObjSetLong(hObj_progressbar, OBJECT_LONG_OBJPROC, (size_t)OnCircleProgressBarProc);
    Ex_ObjSetColor(hObj_progressbar, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetLong(hObj_progressbar, CIRCLEPROGRESSBAR_LONG_RANGE, 100);
    Ex_ObjSetLong(hObj_progressbar, CIRCLEPROGRESSBAR_LONG_LINECAP, 0);
    Ex_ObjSetLong(hObj_progressbar, CIRCLEPROGRESSBAR_LONG_LINEHEIGHT, 16);
    Ex_ObjInvalidateRect(hObj_progressbar, NULL);
    Ex_ObjSetTimer(hObj_progressbar, 50);


    HEXOBJ hObj_progressbar2 =
        Ex_ObjCreate(L"CircleProgressBar", NULL, -1, 180, 100, 200, 200, hExDui_progressbar);
    Ex_ObjSetLong(hObj_progressbar2, OBJECT_LONG_OBJPROC, (size_t)OnCircleProgressBarProc);
    Ex_ObjSetColor(hObj_progressbar2, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_RANGE, 100);
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_LINECAP, 1);
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_LINEHEIGHT, 40);
    HEXFONT hFont = _font_createfromfamily(L"MicroSoft Yahei", 30, 0);
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_FONT, hFont);
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_BACKGROUNDCOLOR,
                  ExARGB(30, 130, 100, 255));
    Ex_ObjSetLong(hObj_progressbar2, CIRCLEPROGRESSBAR_LONG_FOREGROUNDCOLOR, ExARGB(100, 130, 30, 255));
    Ex_ObjInvalidateRect(hObj_progressbar2, NULL);
    Ex_ObjSetTimer(hObj_progressbar2, 50);


    Ex_DUIShowWindow(hExDui_progressbar, SW_SHOWNORMAL, 0, 0, 0);
}
