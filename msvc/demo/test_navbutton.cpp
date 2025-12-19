#include "test_navbutton.h"

std::vector<HEXOBJ> m_hNavBtn(4);
std::vector<HEXOBJ> m_hPageNavBtn(4);
INT                 m_nCurIndex = 1;
HEXLAYOUT           m_hLayout;

LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                       LPARAM lParam)
{
    if (lParam != 0) {
        m_nCurIndex = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        _layout_setprop(m_hLayout, LAYOUT_PROP_PAGE_CURRENT, m_nCurIndex);
        _layout_update(m_hLayout);
    }
    return 0;
}


void test_navbutton(HWND hWnd)
{
    HWND hWnd_navbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试选项卡", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_navbutton = Ex_DUIBindWindowEx(
        hWnd_navbutton, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_navbutton, ENGINE_LONG_CRBKG, ExRGB2ARGB(0, 255));

    for (INT i = 0; i < 4; i++) {
        HEXIMAGE hImage = 0;
        HEXIMAGE hImg1  = 0;
        HEXIMAGE hImg2  = 0;
        auto     str    = L"Tab" + std::to_wstring(i);
        m_hNavBtn[i] =
            Ex_ObjCreate(L"NavButton", str.c_str(), -1, 20 + i * 85, 40, 80, 80, hExDui_navbutton);

        auto file = L"./res/navbtn/大图标" + std::to_wstring(i + 1) + L".png";
        _img_createfromfile(file.c_str(), &hImage);

        Ex_ObjSendMessage(m_hNavBtn[i], WM_SETICON, 0, hImage);
        Ex_ObjSetColor(m_hNavBtn[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);

        _img_createfromfile(L"./res/navbtn/顶部按钮背景C.png", &hImg1);

        Ex_ObjSendMessage(m_hNavBtn[i], BM_SETIMAGE, 1, hImg1);

        _img_createfromfile(L"./res/navbtn/顶部按钮背景D.png", &hImg2);

        Ex_ObjSendMessage(m_hNavBtn[i], BM_SETIMAGE, 2, hImg2);

        Ex_ObjInvalidateRect(m_hNavBtn[i], 0);
        Ex_ObjSetLong(m_hNavBtn[i], OBJECT_LONG_LPARAM, i + 1);
        Ex_ObjHandleEvent(m_hNavBtn[i], NM_CHECK, OnNavButtonCheckEvent);
    }
    HEXOBJ hPageNavbtnContainer =
        Ex_ObjCreate(L"page", 0, -1, 20, 120, 760, 600 - 120 - 20, hExDui_navbutton);
    // 创建一个页面布局,绑定容器页面
    m_hLayout = _layout_create(LAYOUT_TYPE_PAGE, hPageNavbtnContainer);
    for (INT i = 0; i < 4; i++) {
        auto str         = L"页面" + std::to_wstring(i);
        m_hPageNavBtn[i] = Ex_ObjCreateEx(-1, L"static", str.c_str(), -1, 0, 0, 760, 600 - 120 - 20,
                                          hPageNavbtnContainer, 0, DT_CENTER | DT_VCENTER, 0, 0, 0);
        Ex_ObjSetFontFromFamily(m_hPageNavBtn[i], 0, 40, 0, TRUE);
        Ex_ObjSetColor(m_hPageNavBtn[i], COLOR_EX_BACKGROUND,
                       ExRGB2ARGB(Random(1000000, 16777215), 255), TRUE);
        Ex_ObjSetColor(m_hPageNavBtn[i], COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), TRUE);
        // 布局添加子页面
        _layout_addchild(m_hLayout, m_hPageNavBtn[i]);
    }
    // 容器页面设置布局对象
    Ex_ObjLayoutSet(hPageNavbtnContainer, m_hLayout, TRUE);
    // 设置布局显示页面索引,从1开始
    m_nCurIndex = 1;
    _layout_setprop(m_hLayout, LAYOUT_PROP_PAGE_CURRENT, m_nCurIndex);
    Ex_ObjSendMessage(m_hNavBtn[0], BM_SETCHECK, 1, 1);


    HEXOBJ hObj_scrollview =
        Ex_ObjCreate(L"FlowScrollView", 0, -1, 30, 30, 350, 350, m_hPageNavBtn[1]);
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


    Ex_DUIShowWindow(hExDui_navbutton, SW_SHOWNORMAL);
}
