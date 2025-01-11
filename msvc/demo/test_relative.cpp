#include "test_relative.h"

void test_relative(HWND hWnd)
{
    HWND hWnd_relative = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试相对布局", 0, 0, 600, 400, 0, 0);
    HEXDUI hExDui_relative = Ex_DUIBindWindowEx(
        hWnd_relative, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX |
            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON |
            WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_relative, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_RELATIVE, hExDui_relative);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_LEFT, 10);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_TOP, 25);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_RIGHT, 10);
    _layout_setprop(hLayout, LAYOUT_PROP_PADDING_BOTTOM, 10);

    HEXOBJ hObj1 = Ex_ObjCreateEx(-1, L"Static", L"控件A：父容器的左下角", -1, 0, 0, 200, 150,
                                  hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExARGB(255, 0, 0, 100), TRUE);
    _layout_setchildprop(hLayout, hObj1, LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF,
                         -1);   // 左侧对齐于父容器
    _layout_setchildprop(hLayout, hObj1, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF,
                         -1);   // 底部对齐于父容器

    HEXOBJ hObj2 = Ex_ObjCreateEx(-1, L"Static", L"控件B：父容器居中顶部", -1, 0, 0, 200, 150,
                                  hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF,
                         -1);   // 顶部对齐于父容器
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_RELATIVE_CENTER_PARENT_H,
                         1);   // 水平居中于父容器

    HEXOBJ hObj3 = Ex_ObjCreateEx(-1, L"Static", L"控件C：右侧与A对齐,宽度150,在A和B之间", -1, 0, 0,
                                  150, 150, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_RELATIVE_TOP_OF, hObj1);   // 在A控件顶部
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF, hObj2);   // 在B控件底部
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF,
                         hObj1);   // 右侧对齐于A控件

    HEXOBJ hObj4 =
        Ex_ObjCreateEx(-1, L"Static", L"控件D：高度100,在A和父控件右边界之间,在父容器底部", -1, 0,
                       0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(16754943, 100), TRUE);
    _layout_setchildprop(hLayout, hObj4, LAYOUT_SUBPROP_RELATIVE_RIGHT_OF, hObj1);   // 在A控件右侧
    _layout_setchildprop(hLayout, hObj4, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF,
                         -1);   // 底部对齐于父容器
    _layout_setchildprop(hLayout, hObj4, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF,
                         -1);   // 右侧对齐于父容器

    HEXOBJ hObj5 = Ex_ObjCreateEx(-1, L"Static", L"控件E：与D同宽,在D和B之间", -1, 0, 0, 150, 100,
                                  hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj5, COLOR_EX_BACKGROUND, ExRGB2ARGB(8445952, 100), TRUE);
    _layout_setchildprop(hLayout, hObj5, LAYOUT_SUBPROP_RELATIVE_TOP_OF, hObj4);      // 在D顶部
    _layout_setchildprop(hLayout, hObj5, LAYOUT_SUBPROP_RELATIVE_BOTTOM_OF, hObj2);   // 在B底部
    _layout_setchildprop(hLayout, hObj5, LAYOUT_SUBPROP_RELATIVE_LEFT_ALIGN_OF,
                         hObj4);   // 左侧对齐于D
    _layout_setchildprop(hLayout, hObj5, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF,
                         hObj4);   // 右侧对齐于D

    HEXOBJ hObj6 = Ex_ObjCreateEx(-1, L"Static", L"控件F：150宽度,垂直方向对齐于DE,右对齐于DE", -1,
                                  0, 0, 150, 100, hExDui_relative, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj6, COLOR_EX_BACKGROUND, ExRGB2ARGB(16777215, 100), TRUE);
    _layout_setchildprop(hLayout, hObj6, LAYOUT_SUBPROP_RELATIVE_TOP_ALIGN_OF,
                         hObj5);   // 顶部对齐于E
    _layout_setchildprop(hLayout, hObj6, LAYOUT_SUBPROP_RELATIVE_BOTTOM_ALIGN_OF,
                         hObj4);   // 底部对齐于D
    _layout_setchildprop(hLayout, hObj6, LAYOUT_SUBPROP_RELATIVE_RIGHT_ALIGN_OF,
                         hObj4);   // 右对齐于D

    Ex_ObjLayoutSet(hExDui_relative, hLayout, TRUE);

    Ex_DUIShowWindow(hExDui_relative, SW_SHOWNORMAL, 0, 0, 0);
}
