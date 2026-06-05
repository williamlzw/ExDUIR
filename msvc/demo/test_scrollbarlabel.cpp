#include "test_scrollbarlabel.h"

void test_scrollbarlabel(HWND hWnd)
{
    HWND hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试滚动条标签", 0, 0, 350, 200, 0, 0);
    auto hExDui = Ex_DUIBindWindowEx(hWnd_button, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    auto hObj = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"ScrollbarLabel", L"🐓DirectUI框架是为Windows平台设计的。它支持D3D11和D2D渲染，包括几十个基本组件。它还支持最新的cef3浏览器。编译后的dll可以被各种语言调用，如Python、Go、Java和c#。此外，它还支持Unicode多语言。", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, 10, 30, 300, 50,
        hExDui, 0,  DT_SELECTABLE, 0, 0, NULL);
   // Ex_ObjSetFontFromFamily(hObj, L"宋体", 12, 0, 0);
    Ex_DUIShowWindow(hExDui, SW_SHOWNORMAL);
}