#include "test_titlebar.h"


void test_titlebar(HWND hParent)
{
    HWND   hWnd   = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试标题框", 0, 0, 400, 200, 0, 0);
    HEXDUI hExDui = Ex_DUIBindWindowEx(hWnd, 0,
                                       WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
                                           WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
                                           WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
                                           WINDOW_STYLE_HASICON,
                                       0, 0);
    Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj = 0;
    hObj        = Ex_ObjCreate(L"Titlebar", L"标题框1", -1, 30, 50, 300, 20, hExDui);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, -1, FALSE);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);

    hObj = Ex_ObjCreate(L"Titlebar", L"标题框2", -1, 30, 80, 300, 20, hExDui);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 255), FALSE);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(255, 255), TRUE);

    hObj = Ex_ObjCreate(L"Titlebar", L"标题框3", -1, 30, 110, 300, 20, hExDui);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 255), FALSE);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16711680, 255), TRUE);

    hObj = Ex_ObjCreate(L"Titlebar", L"标题框4", -1, 30, 140, 300, 20, hExDui);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 255), FALSE);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, -1, TRUE);

    Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL);
}
