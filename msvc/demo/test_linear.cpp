#include "test_linear.h"

void test_linear(HWND hWnd)
{
    HWND hWnd_linear = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试线性布局", 0, 0, 800, 400, 0, 0);
    HEXDUI hExDui_linear = Ex_DUIBindWindowEx(
        hWnd_linear, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX |
            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON |
            WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_linear, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_LINEAR, hExDui_linear);
    _layout_setprop(hLayout, LAYOUT_PROP_LINEAR_DIRECTION,
                    LAYOUT_PROP_DIRECTION_H);   // 设置布局方向为水平
    _layout_setprop(hLayout, LAYOUT_PROP_LINEAR_ALIGN,
                    LAYOUT_PROP_LINEAR_ALIGN_CENTER);   // 设置布局方向对齐方式为居中

    HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"Static", L"容器1", -1, 0, 0, 200, 300, hExDui_linear, 0,
                                  DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
    _layout_setchildprop(hLayout, hObj1, LAYOUT_SUBPROP_LINEAR_ALIGN,
                         LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);                // 设置居中于父
    _layout_setchildprop(hLayout, hObj1, LAYOUT_SUBPROP_MARGIN_RIGHT, 10);   // 设置右边10像素间隔

    HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"容器2", -1, 0, 0, 400, 200, hExDui_linear, 0,
                                  DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_LINEAR_ALIGN,
                         LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);                // 设置居中于父
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_MARGIN_RIGHT, 10);   // 设置右边10像素间隔

    HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"容器3", -1, 0, 0, 100, 100, hExDui_linear, 0,
                                  DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_LINEAR_ALIGN,
                         LAYOUT_SUBPROP_LINEAR_ALIGN_CENTER);   // 设置居中于父

    Ex_ObjLayoutSet(hExDui_linear, hLayout, TRUE);
    Ex_DUIShowWindow(hExDui_linear, SW_SHOWNORMAL);
}
