#include "test_linechart.h"

void test_linechart(HWND hWnd)
{
    HWND hWnd_linechart = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试折线图", 0, 0, 700, 500, 0, 0);
    HEXDUI hExDui_linechart = Ex_DUIBindWindowEx(
        hWnd_linechart, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
        WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON |
        WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_linechart, ENGINE_LONG_CRBKG, ExARGB(220, 220, 220, 255));

    HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"LineChart", L"", -1, 50, 50, 600, 300, hExDui_linechart, 0,
        DT_VCENTER, 0, 0, 0);
    Ex_ObjSetTimer(hObj1, 500);
    Ex_DUIShowWindow(hExDui_linechart, SW_SHOWNORMAL, 0, 0, 0);
}