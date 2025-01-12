#include "test_mask.h"

LRESULT CALLBACK OnMaskObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND) {
        HEXIMAGE hImg1 = 0;
        HEXIMAGE hImg2 = 0;
        _img_createfromfile(L"res/mask/mask4.jpg", &hImg1);
        _img_createfromfile(L"res/mask/mask1.png", &hImg2);
        HEXIMAGE hImg3;
        _img_mask(hImg1, hImg2, 0, FALSE, &hImg3);
        _canvas_drawimagerect(wParam, hImg3, 0, 0, 200, 200, 255);
        _img_destroy(hImg1);
        _img_destroy(hImg2);
        _img_destroy(hImg3);
        *lpResult = 1;
        return 1;
    }
    return 0;
}

void test_mask(HWND hWnd)
{
    HWND hWnd_mask   = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试蒙板", 0, 0, 300, 300, 0, 0);
    auto hExDui_mask = Ex_DUIBindWindowEx(hWnd_mask, 0,
                                          WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                              WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE |
                                              WINDOW_STYLE_TITLE,
                                          0, 0);
    Ex_DUISetLong(hExDui_mask, ENGINE_LONG_CRBKG, ExRGB2ARGB(16711680, 255));

    Ex_ObjCreateEx(-1, L"static", L"", -1, 50, 50, 200, 200, hExDui_mask, 0, DT_VCENTER | DT_CENTER,
                   0, 0, OnMaskObjMsgProc);
    Ex_DUIShowWindow(hExDui_mask, SW_SHOWNORMAL, 0, 0, 0);
}
