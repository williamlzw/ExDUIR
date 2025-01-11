#include "test_loading.h"

void test_loading(HWND hWnd)
{
    HWND hWnd_loading = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试加载动画", 0, 0, 300, 250, 0, 0);
    HEXDUI hExDui_loading = Ex_DUIBindWindowEx(
        hWnd_loading, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_loading, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    // 马赛克加载动画
    HEXOBJ hObj_mosaicloading =
        Ex_ObjCreate(L"MosaicLoading", L"MosaicLoading", -1, 50, 50, 50, 50, hExDui_loading);
    Ex_ObjSetColor(hObj_mosaicloading, COLOR_EX_BACKGROUND, ExRGB(255, 10, 10),
                   TRUE);   // 动画点颜色，只能提供RGB色

    // WIN10加载动画,默认风格为圆形转圈
    HEXOBJ hObj_win10loading1 =
        Ex_ObjCreate(L"Win10Loading", L"加载中，请稍后", -1, 150, 30, 100, 80, hExDui_loading);
    HEXOBJ hObj_win10loading2 =
        Ex_ObjCreate(L"Win10Loading", L"正在读取数据，请稍后",
                     OBJECT_STYLE_VISIBLE | LOADING_STYLE_LINE, 50, 150, 200, 60, hExDui_loading);
    Ex_ObjSetColor(hObj_win10loading2, COLOR_EX_BACKGROUND, ExRGB(100, 236, 255),
                   TRUE);   // 动画点颜色，只能提供RGB色
    Ex_DUIShowWindow(hExDui_loading, SW_SHOWNORMAL, 0, 0, 0);
}
