#include "test_groupbox.h"


void test_groupbox(HWND hWnd)
{
    HWND hWnd_groupbox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试分组框", 0, 0, 400, 300, 0, 0);
    HEXDUI hExDui_groupbox = Ex_DUIBindWindowEx(
        hWnd_groupbox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_groupbox, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_groupbox =
        Ex_ObjCreate(L"groupbox", L"分组框", -1, 30, 30, 230, 230, hExDui_groupbox);
    Ex_ObjSetColor(hObj_groupbox, COLOR_EX_TEXT_NORMAL, ExARGB(255, 55, 55, 255), FALSE);
    Ex_ObjSetColor(hObj_groupbox, COLOR_EX_BORDER, ExARGB(55, 0, 250, 255), FALSE);
    Ex_ObjSetLong(hObj_groupbox, GROUPBOX_LONG_TEXT_OFFSET, 50);
    Ex_ObjSetLong(hObj_groupbox, GROUPBOX_LONG_RADIUS, 30);
    Ex_ObjSetLong(hObj_groupbox, GROUPBOX_LONG_STROKEWIDTH, 3);
    Ex_DUIShowWindow(hExDui_groupbox, SW_SHOWNORMAL);
}
