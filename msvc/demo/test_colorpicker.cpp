#include "test_colorpicker.h"


LRESULT CALLBACK OnColorPickerButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                          LPARAM lParam)
{
    if (nCode == COLORPICKER_EVENT_COLORCHANGE) {
        OUTPUTW(L"颜色已更改", (int)lParam);
    }
    return 0;
}

void test_colorpicker(HWND hParent)
{
    HWND hWnd_colorpicker =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试颜色选择器", 0, 0, 250, 300, 0, 0);
    HEXDUI hExDui_colorpicker = Ex_DUIBindWindowEx(
        hWnd_colorpicker, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_colorpicker, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj = Ex_ObjCreate(L"ColorPicker", 0, -1, 50, 40, 80, 30, hExDui_colorpicker);

    Ex_ObjHandleEvent(hObj, COLORPICKER_EVENT_COLORCHANGE, OnColorPickerButtonEvent);
    Ex_DUIShowWindow(hExDui_colorpicker, SW_SHOWNORMAL);
}
