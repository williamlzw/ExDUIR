#include "test_absolute.h"

void test_absolute(HWND hWnd)
{
    HWND hWnd_absolute = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试绝对布局", 0, 0, 800, 300, 0, 0);
    HEXDUI hExDui_absolute = Ex_DUIBindWindowEx(
        hWnd_absolute, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MAX |
            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_HASICON |
            WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_absolute, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_ABSOLUTE, hExDui_absolute);

    /*HEXOBJ hObj = Ex_ObjCreate(L"Static", L"左20顶50底20宽200", -1, 0, 0, 200, 100,
    hExDui_absolute); Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj, LAYOUT_SUBPROP_ABSOLUTE_TOP,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50); _layout_absolute_setedge(hLayout, hObj,
    LAYOUT_SUBPROP_ABSOLUTE_LEFT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 20);
    _layout_absolute_setedge(hLayout, hObj, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 20);

    HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"顶50右20左220高50%内边距10", -1, 0, 0, 200, 100,
    hExDui_absolute); Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_TOP,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50); _layout_absolute_setedge(hLayout, hObj2,
    LAYOUT_SUBPROP_ABSOLUTE_RIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 20);
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 240); _layout_absolute_setedge(hLayout, hObj2,
    LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 50);
    _layout_setchildprop(hLayout, hObj2, LAYOUT_SUBPROP_MARGIN_BOTTOM, 45);

    HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"底20右20左220高50%内边距10", -1, 0, 0, 200, 100,
    hExDui_absolute); Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 20); _layout_absolute_setedge(hLayout, hObj3,
    LAYOUT_SUBPROP_ABSOLUTE_BOTTOM, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 20);
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
    LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 240); _layout_absolute_setedge(hLayout, hObj3,
    LAYOUT_SUBPROP_ABSOLUTE_HEIGHT, LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS, 50);
    _layout_setchildprop(hLayout, hObj3, LAYOUT_SUBPROP_MARGIN_TOP, 45);*/

    HEXOBJ hObj1 = Ex_ObjCreate(L"Static", L"固定在右下角50,50的位置，不变大小", -1, 0, 0, 200, 100,
                                hExDui_absolute);
    Ex_ObjSetColor(hObj1, COLOR_EX_BACKGROUND, ExRGB2ARGB(255, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj1, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX,
                             50);   // 设置距离右边为50像素
    _layout_absolute_setedge(hLayout, hObj1, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX,
                             50);   // 设置距离底边为50像素

    HEXOBJ hObj2 = Ex_ObjCreate(L"Static", L"固定在左下角50,50的位置，宽度为40%,高度为50", -1, 0, 0,
                                0, 0, hExDui_absolute);
    Ex_ObjSetColor(hObj2, COLOR_EX_BACKGROUND, ExRGB2ARGB(0, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX,
                             50);   // 设置距离左边为50像素
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX,
                             50);   // 设置距离底边为50像素
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_WIDTH,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             40);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj2, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 50);

    HEXOBJ hObj3 = Ex_ObjCreate(L"Static", L"距离四边均为20%", -1, 0, 0, 0, 0, hExDui_absolute);
    Ex_ObjSetColor(hObj3, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             20);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_TOP,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             20);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             20);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj3, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             20);   // 注意单位是PS（百分比）

    HEXOBJ hObj4 = Ex_ObjCreate(L"Static", L"居中于窗口,宽度为窗口的30%,高度为100像素", -1, 0, 0, 0,
                                0, hExDui_absolute);
    Ex_ObjSetColor(hObj4, COLOR_EX_BACKGROUND, ExRGB2ARGB(65280, 100), TRUE);
    _layout_absolute_setedge(hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             50);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_TOP,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             50);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_WIDTH,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PS,
                             30);   // 注意单位是PS（百分比）
    _layout_absolute_setedge(hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_HEIGHT,
                             LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 100);   //
    _layout_absolute_setedge(
        hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_H, LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS,
        -50);   //  水平偏移控件-50%的控件宽度 注意单位是OBJ_PS（控件尺寸的百分比）
    _layout_absolute_setedge(
        hLayout, hObj4, LAYOUT_SUBPROP_ABSOLUTE_OFFSET_V, LAYOUT_SUBPROP_ABSOLUTE_TYPE_OBJPS,
        -50);   //  水平偏移控件-50%的控件高度 注意单位是OBJ_PS（控件尺寸的百分比）

    Ex_ObjLayoutSet(hExDui_absolute, hLayout, TRUE);

    Ex_DUIShowWindow(hExDui_absolute, SW_SHOWNORMAL);
}
