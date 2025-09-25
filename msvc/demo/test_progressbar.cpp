#include "test_progressbar.h"


LRESULT CALLBACK OnProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult)
{
    if (uMsg == WM_TIMER) {
        size_t nPos   = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_POS);
        size_t nRange = Ex_ObjGetLong(hObj, PROGRESSBAR_LONG_RANGE);
        Ex_ObjSetLong(hObj, PROGRESSBAR_LONG_POS, nPos + Random(1, 20));
        Ex_ObjInvalidateRect(hObj, 0);
        if (nRange == nPos) {
            Ex_ObjKillTimer(hObj);
        }
    }

    return 0;
}

void test_progressbar(HWND hWnd)
{
    HWND hWnd_progressbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试进度条", 0, 0, 400, 500, 0, 0);
    HEXDUI hExDui_progressbar = Ex_DUIBindWindowEx(
        hWnd_progressbar, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_progressbar, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_progressbar =
        Ex_ObjCreate(L"ProgressBar", NULL, -1, 50, 100, 20, 300, hExDui_progressbar);
    Ex_ObjSetLong(hObj_progressbar, OBJECT_LONG_OBJPROC, (size_t)OnProgressBarProc);
    Ex_ObjSetColor(hObj_progressbar, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetLong(hObj_progressbar, PROGRESSBAR_LONG_RADIUS, 10);
    Ex_ObjSetLong(hObj_progressbar, PROGRESSBAR_LONG_RANGE, 255);
    Ex_ObjSetLong(hObj_progressbar, PROGRESSBAR_LONG_POINT, 1);
    Ex_ObjInvalidateRect(hObj_progressbar, NULL);
    Ex_ObjSetTimer(hObj_progressbar, 50);

    HEXOBJ hObj_progressbar2 =
        Ex_ObjCreate(L"ProgressBar", NULL, -1, 50, 50, 300, 20, hExDui_progressbar);
    Ex_ObjSetLong(hObj_progressbar2, OBJECT_LONG_OBJPROC, (size_t)OnProgressBarProc);
    Ex_ObjSetColor(hObj_progressbar2, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetLong(hObj_progressbar2, PROGRESSBAR_LONG_RADIUS, 10);
    Ex_ObjSetLong(hObj_progressbar2, PROGRESSBAR_LONG_RANGE, 255);
    Ex_ObjSetLong(hObj_progressbar2, PROGRESSBAR_LONG_POINT, 0);
    Ex_ObjInvalidateRect(hObj_progressbar2, NULL);
    Ex_ObjSetTimer(hObj_progressbar2, 50);
    Ex_DUIShowWindow(hExDui_progressbar, SW_SHOWNORMAL);
}
