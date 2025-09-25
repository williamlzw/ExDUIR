#include "test_datebox.h"

LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == DATEBOX_EVENT_DATETIME) {
        EX_DATETIME* dt = (EX_DATETIME*)lParam;
        OUTPUTW(L"日期已更改", dt->Year, dt->Mon, dt->Mday, dt->Wday);
    }
    return 0;
}

void test_datebox(HWND hParent)
{
    HWND hWnd_datebox = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试日期框", 0, 0, 250, 200, 0, 0);
    HEXDUI hExDui_datebox = Ex_DUIBindWindowEx(
        hWnd_datebox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_datebox, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj = Ex_ObjCreate(L"DateBox", 0, -1, 50, 80, 150, 30, hExDui_datebox);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, -1, FALSE);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16711680, 255), TRUE);

    Ex_ObjHandleEvent(hObj, DATEBOX_EVENT_DATETIME, OnDateBoxButtonEvent);

    Ex_DUIShowWindow(hExDui_datebox, SW_SHOWNORMAL);
}
