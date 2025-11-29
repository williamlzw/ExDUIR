#include "test_flowscrollview.h"

void test_flowscrollview(HWND hWnd)
{
    HWND hWnd_scrollview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试流式滚动容器", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_scrollview = Ex_DUIBindWindowEx(
        hWnd_scrollview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
        WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_scrollview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj_scrollview =
        Ex_ObjCreate(L"FlowScrollView", 0, -1, 30, 30, 350, 350, hExDui_scrollview);
    // 获取容器句柄
    HEXOBJ hContainer = (HEXOBJ)Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_GET_CONTAINER_HANDLE, 0, 0);
    HEXOBJ hObj_groupbox1 =
        Ex_ObjCreate(L"groupbox", L"分组框1", -1, 30, 30, 230, 230, hContainer);
    Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox1, 0);
    HEXOBJ hObj_groupbox2 =
        Ex_ObjCreate(L"groupbox", L"分组框2", -1, 30, 270, 230, 230, hContainer);
    Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox2, 0);
    HEXOBJ hObj_groupbox3 =
        Ex_ObjCreate(L"groupbox", L"分组框3", -1, 30, 510, 230, 230, hContainer);
    Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox3, 0);
    Ex_DUIShowWindow(hExDui_scrollview, SW_SHOWNORMAL);
}