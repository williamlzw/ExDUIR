#include "test_carousel.h"

void test_carousel(HWND hParent)
{
    HWND hWnd_carousel = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试轮播", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_carousel = Ex_DUIBindWindowEx(
        hWnd_carousel, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
        0, 0);
    Ex_DUISetLong(hExDui_carousel, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj = Ex_ObjCreate(L"Carousel", 0, -1, 20, 40, 760, 550, hExDui_carousel);
    // 添加图片前必须设置尺寸
    Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_SIZE, 1600, 1200);
    HEXIMAGE hImg = 0;
    _img_createfromfile(L"res/1.jpeg", &hImg);
    Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_ADDIMG, 0, hImg);
    _img_createfromfile(L"res/2.jpeg", &hImg);
    Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_ADDIMG, 0, hImg);
    _img_createfromfile(L"res/3.jpeg", &hImg);
    Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_ADDIMG, 0, hImg);
    Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_SETTIMER, 0, 5000);   // 设置轮播间隔

    // 全部销毁用下面的
    // Ex_ObjSendMessage(hObj, CAROUSEL_MESSAGE_CLEAR, 0, 0);

    Ex_DUIShowWindow(hExDui_carousel, SW_SHOWNORMAL, 0, 0, 0);
}
