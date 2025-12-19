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
    HEXOBJ body_a2 = Ex_ObjCreate(L"FlowScrollView", 0, -1, 40, 40, 350, 350, hExDui_scrollview);//创建流式容器2


    EX_FLOWSCROLLVIEW_LAYOUT_CONFIG layoutConfig;
    layoutConfig.nHorizontalSpacing = 50;
    layoutConfig.nVerticalSpacing = 50;
    Ex_ObjSendMessage(body_a2, FLOWSCROLLVIEW_MESSAGE_SET_LAYOUT_CONFIG, 0, (LPARAM)&layoutConfig);

    HEXOBJ body_nav_c2 = (HEXOBJ)Ex_ObjSendMessage(body_a2, FLOWSCROLLVIEW_MESSAGE_GET_CONTAINER_HANDLE, 0, 0);


    EXARGB argb_class = ExARGB(97, 175, 239, 240);
    HEXOBJ config_fishing_map = Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VISIBLE, 22, 22, 250, 200, body_nav_c2, 1001, 0, 0, 0, 0); //绑定到第二页
    Ex_ObjSetRadius(config_fishing_map, 20, 20, 20, 20, TRUE);
    Ex_ObjSetColor(config_fishing_map, COLOR_EX_BACKGROUND, argb_class, TRUE);
    Ex_ObjSendMessage(body_a2, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)config_fishing_map, 0);


    HEXOBJ config_fishing_map2 = Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VISIBLE, 22, 22, 250, 200, body_nav_c2, 1002, 0, 0, 0, 0);//绑定到第二页
    //Ex_ObjSetRadius(config_fishing_map2, 10, 10, 10, 10, TRUE);
    Ex_ObjSetColor(config_fishing_map2, COLOR_EX_BACKGROUND, argb_class, TRUE);
    Ex_ObjSendMessage(body_a2, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)config_fishing_map2, 0);


    HEXOBJ config_fishing_map3 = Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_VISIBLE, 22, 22, 250, 200, body_nav_c2, 1003, 0, 0, 0, 0);//绑定到第二页
    //Ex_ObjSetRadius(config_fishing_map3, 10, 10, 10, 10, TRUE);
    Ex_ObjSetColor(config_fishing_map3, COLOR_EX_BACKGROUND, argb_class, TRUE);
    Ex_ObjSendMessage(body_a2, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)config_fishing_map3, 0);
    //HEXOBJ hObj_scrollview =
    //    Ex_ObjCreate(L"FlowScrollView", 0, -1, 30, 30, 350, 350, hExDui_scrollview);
    //// 获取容器句柄
    //HEXOBJ hContainer = (HEXOBJ)Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_GET_CONTAINER_HANDLE, 0, 0);
    //HEXOBJ hObj_groupbox1 =
    //    Ex_ObjCreate(L"static", L"分组框1", OBJECT_STYLE_VISIBLE, 30, 30, 230, 230, hContainer);
    //Ex_ObjSetColor(hObj_groupbox1, COLOR_EX_BACKGROUND, ExARGB(97, 175, 239, 240), TRUE);
    //Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox1, 0);
    //HEXOBJ hObj_groupbox2 =
    //    Ex_ObjCreate(L"static", L"分组框2", OBJECT_STYLE_VISIBLE, 30, 270, 230, 230, hContainer);
    //Ex_ObjSetColor(hObj_groupbox2, COLOR_EX_BACKGROUND, ExARGB(97, 175, 239, 240), TRUE);
    //Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox2, 0);
    //HEXOBJ hObj_groupbox3 =
    //    Ex_ObjCreate(L"static", L"分组框3", OBJECT_STYLE_VISIBLE, 30, 510, 230, 230, hContainer);
    //Ex_ObjSetColor(hObj_groupbox3, COLOR_EX_BACKGROUND, ExARGB(97, 175, 239, 240), TRUE);
    //Ex_ObjSendMessage(hObj_scrollview, FLOWSCROLLVIEW_MESSAGE_ADD_COMPONENT, (WPARAM)hObj_groupbox3, 0);
    Ex_DUIShowWindow(hExDui_scrollview, SW_SHOWNORMAL);
}