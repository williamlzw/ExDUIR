#include "test_event.h"


LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                     LRESULT* lpResult)
{
    if (uMsg == WM_NOTIFY) {
        if (wParam == 1001)   // wParam表示事件对象的ID，1001最里层按钮
        {
            EX_NMHDR ni{0};
            RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
            if (ni.nCode == NM_CLICK) {
                std::wstring str;
                str.resize(512);
                // 使用 str.data() 而不是 str.c_str()
                auto len = Ex_ObjGetText(hObj, str.data(), 512);
                // 确保字符串以空字符结尾
                str.resize((len - 2) / 2);   // 因为 len 是字节数，而 wstring 的长度是以字符为单位的
                auto title = str + L"收到了单击事件";
                Ex_MessageBox(hObj, title.c_str(), 0, 0, 0);
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                       LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_NOTIFY) {
        if (wParam == 1001)   // wParam表示事件对象的ID，1001最里层按钮
        {
            EX_NMHDR ni{0};
            RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
            if (ni.nCode == NM_CLICK) {
                if (Ex_MessageBox(hObj, L"按钮直接父控件收到了单击事件，是否拦截？", 0, MB_YESNO,
                                  0) == IDYES)   // 选择是
                {
                    *lpResult = 1;   // 返回值置1(真)表示拦截
                    return 1;   // 函数返回值表示是否使用返回值,lpResult才是真正的返回值
                }
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_EX_LCLICK)   // 左键单击消息,拦截这条则不会触发任何事件
    {
        Ex_MessageBox(hObj, L"按钮收到了单击消息", 0, 0, 0);
    }
    else if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{0};
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (ni.hObjFrom == hObj) {
            if (ni.nCode == NM_CLICK) {
                Ex_MessageBox(hObj, L"按钮收到了单击事件", 0, 0, 0);
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == NM_CLICK) {
        Ex_MessageBox(hObj, L"按钮收到了独立的单击事件", 0, 0, 0);
    }
    return 0;
}

LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult)
{
    if (uMsg == WM_NOTIFY) {
        if (wParam == 1001)   // wParam表示事件对象的ID，1001最里层按钮
        {
            EX_NMHDR ni{0};
            RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
            if (ni.nCode == NM_CLICK) {
                Ex_MessageBox(hExDui, L"按钮所属窗口收到了单击事件", 0, 0, 0);
            }
        }
    }
    return 0;
}

void test_event(HWND hWnd)
{
    HWND hWnd_event = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试事件分发", 0, 0, 400, 300, 0, 0);
    HEXDUI hExDui_event = Ex_DUIBindWindowEx(hWnd_event, 0,
                                             WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
                                                 WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
                                                 WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
                                                 WINDOW_STYLE_HASICON,
                                             0, OnEventWndMsgProc);
    Ex_DUISetLong(hExDui_event, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    /*消息(WM_/XXM_)分发的流程是：
    控件产生消息->控件类回调函数->控件回调函数->默认控件回调函数(在此部分消息会产生事件)
    事件(NM_/XXN_)分发的流程是：
    控件产生事件->JS回调函数->控件自身收到WM_NOTIFY->控件直接父控件收到WM_NOTIFY->控件间接父控件从内而外逐层收到WM_NOTIFY消息(需启用消息冒泡)->所属窗口收到WM_NOTIFY消息
    消息和事件在分发途中均可拦截,HandEvent*/

    HEXOBJ hobj_button_outside =
        Ex_ObjCreateEx(-1, L"Static", L"按钮外间接父控件", -1, 25, 35, 350, 250, hExDui_event, 0,
                       DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
    if (hobj_button_outside != 0) {
        Ex_ObjSetColor(hobj_button_outside, COLOR_EX_BACKGROUND, ExRGB2ARGB(65535, 100), TRUE);
        HEXOBJ hobj_button_inside =
            Ex_ObjCreateEx(-1, L"Static", L"按钮内间接父控件", -1, 10, 20, 330, 200,
                           hobj_button_outside, 0, DT_SINGLELINE, 0, 0, OnSideButtonMsgProc);
        if (hobj_button_inside != 0) {
            Ex_ObjSetColor(hobj_button_inside, COLOR_EX_BACKGROUND, ExARGB(100, 100, 100, 100),
                           TRUE);
            HEXOBJ hobj_button_parent =
                Ex_ObjCreateEx(-1, L"Static", L"按钮直接父控件", -1, 10, 20, 310, 160,
                               hobj_button_inside, 0, DT_SINGLELINE, 0, 0, OnParentButtonMsgProc);
            if (hobj_button_parent != 0) {
                Ex_ObjSetColor(hobj_button_parent, COLOR_EX_BACKGROUND, ExARGB(26, 100, 129, 100),
                               TRUE);
                HEXOBJ hobj_button =
                    Ex_ObjCreateEx(-1, L"button", L"按钮", -1, 10, 20, 200, 100, hobj_button_parent,
                                   1001, -1, 0, 0, OnEventButtonMsgProc);
                if (hobj_button != 0) {
                    Ex_ObjHandleEvent(hobj_button, NM_CLICK, OnEventButtonEvent);
                    Ex_ObjEnableEventBubble(
                        hobj_button,
                        TRUE);   // 启用控件的事件冒泡，事件冒泡是指事件将根据父控件逐层传递至窗口
                }
            }
        }
    }
    Ex_DUIShowWindow(hExDui_event, SW_SHOWNORMAL, 0, 0, 0);
}
