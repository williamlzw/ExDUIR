#include "test_custombkg.h"

void test_custombkg(HWND hWnd)
{
    HWND   hWnd_custombkg = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"", 0, 0, 175, 200, 0, 0);
    HEXDUI hExDui_custombkg =
        Ex_DUIBindWindowEx(hWnd_custombkg, 0,
                           WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
                               WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW,
                           0, 0);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/custombkg.png", &imgdata);
    RECT grid = { 45, 40, 15, 15};
    Ex_ObjSetBackgroundImage(hExDui_custombkg, imgdata.data(), imgdata.size(), 0, 0,
                             BACKGROUND_REPEAT_ZOOM, &grid, BACKGROUND_FLAG_DEFAULT, 255, TRUE);
    Ex_ObjCreateEx(OBJECT_STYLE_EX_TOPMOST, L"sysbutton", L"",
                   OBJECT_STYLE_VISIBLE | WINDOW_STYLE_BUTTON_CLOSE, 140, 8, 30, 30,
                   hExDui_custombkg, 0, 0, 0, 0, NULL);
    Ex_DUIShowWindow(hExDui_custombkg, SW_SHOWNORMAL, 0, 0, 0);
}
