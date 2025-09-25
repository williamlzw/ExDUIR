#include "test_flow.h"

void test_flow(HWND hWnd)
{
    HWND hWnd_flow = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试流式布局", 0, 0, 600, 400, 0, 0);
    HEXDUI hExDui_flow = Ex_DUIBindWindowEx(hWnd_flow, 0,
                                            WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                                                WINDOW_STYLE_BUTTON_MAX | WINDOW_STYLE_BUTTON_MIN |
                                                WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                                WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE |
                                                WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
                                            0, 0);
    Ex_DUISetLong(hExDui_flow, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_FLOW, hExDui_flow);
    _layout_setprop(hLayout, LAYOUT_PROP_FLOW_DIRECTION,
                    LAYOUT_PROP_DIRECTION_V);   // 设置布局方向为垂直

    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_LEFT, 30);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_TOP, 30);   // 设置距离顶边30
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_RIGHT, 30);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_BOTTOM, 30);

    HEXOBJ hObj = 0;
    for (INT i = 0; i < 20; i++) {
        auto str = L"test" + std::to_wstring(i + 1);
        hObj     = Ex_ObjCreate(L"Static", str.c_str(), -1, 0, 0, Random(50, 150), Random(50, 150),
                                hExDui_flow);
        Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
        _layout_setchildprop(hLayout, hObj, LAYOUT_SUBPROP_MARGIN_RIGHT, 10);
        _layout_setchildprop(hLayout, hObj, LAYOUT_SUBPROP_MARGIN_BOTTOM, 10);
        if (i % 10 == 0) {
            _layout_setchildprop(hLayout, hObj, LAYOUT_SUBPROP_FLOW_NEW_LINE, 1);
        }
    }
    Ex_ObjLayoutSet(hExDui_flow, hLayout, TRUE);

    Ex_DUIShowWindow(hExDui_flow, SW_SHOWNORMAL);
}
