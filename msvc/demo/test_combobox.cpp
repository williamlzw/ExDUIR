#include "test_combobox.h"

std::vector<HEXOBJ> m_hComboBoxButton(7);
HEXOBJ              m_hComboBox;

HEXOBJ m_hComboBox1;
HEXOBJ m_hComboBox2;

LRESULT CALLBACK OnComboBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                       LPARAM lParam)
{
    if (nCode == NM_CLICK) {
        if (hObj == m_hComboBoxButton[0]) {
            size_t       count = Ex_ObjSendMessage(m_hComboBox, CB_GETCOUNT, 0, 0);
            std::wstring text  = L"测试添加" + std::to_wstring(count + 1);
            Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (LPARAM)text.c_str());
        }
        else if (hObj == m_hComboBoxButton[1]) {
            std::wstring text = L"插入项目";
            Ex_ObjSendMessage(m_hComboBox, CB_INSERTSTRING, 2, (LPARAM)text.c_str());
        }
        else if (hObj == m_hComboBoxButton[2]) {
            size_t       len = Ex_ObjGetTextLength(m_hComboBox);
            std::wstring text;
            text.resize(len);
            Ex_ObjGetText(m_hComboBox, text.data(), len * 2);
            OUTPUTW(text);
        }
        else if (hObj == m_hComboBoxButton[3]) {
            std::wstring text = L"置内容";
            Ex_ObjSetText(m_hComboBox, text.c_str(), TRUE);
        }
        else if (hObj == m_hComboBoxButton[4]) {
            Ex_ObjSendMessage(m_hComboBox, CB_RESETCONTENT, 0, 0);
        }
        else if (hObj == m_hComboBoxButton[5]) {
            Ex_ObjSendMessage(m_hComboBox, CB_SHOWDROPDOWN, TRUE, 0);
        }
        else if (hObj == m_hComboBoxButton[6]) {
            Ex_ObjSendMessage(m_hComboBox, CB_DELETESTRING, 2, 0);
        }
    }
    else if (nCode == COMBOBOX_EVENT_SELCHANGE) {
        if (hObj == m_hComboBox) {
            OUTPUTW(L"组合框1表项被改变", wParam);
        }
        else if (hObj == m_hComboBox1) {
            OUTPUTW(L"组合框2表项被改变", wParam);
        }
        else if (hObj == m_hComboBox2) {
            OUTPUTW(L"组合框3表项被改变", wParam);
        }
    }
    return 0;
}

void test_combobox(HWND hWnd)
{
    HWND hWnd_combobox = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试组合框", 0, 0, 450, 300, 0, 0);
    HEXDUI hExDui_combobox = Ex_DUIBindWindowEx(
        hWnd_combobox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_combobox, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    m_hComboBox  = Ex_ObjCreateEx(-1, L"combobox", L"测试组合框1",
                                  OBJECT_STYLE_VISIBLE | COMBOBOX_STYLE_ALLOWEDIT, 10, 30, 200, 30,
                                  hExDui_combobox, 0, DT_VCENTER, 0, 0, NULL);
    m_hComboBox1 = Ex_ObjCreateEx(-1, L"combobox", L"测试组合框2",
                                  OBJECT_STYLE_VISIBLE | COMBOBOX_STYLE_ALLOWEDIT, 10, 230, 200, 30,
                                  hExDui_combobox, 0, DT_VCENTER, 0, 0, NULL);
    m_hComboBox2 = Ex_ObjCreateEx(-1, L"combobox", L"测试组合框3",
                                  -1 , 230, 230, 200, 30,
                       hExDui_combobox, 0, DT_VCENTER| DT_CENTER, 0, 0, NULL);
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"英文字母abc");
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"数字123");
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"中文");
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"特殊字符[！（）☏");
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"にほんご");
    Ex_ObjSendMessage(m_hComboBox, CB_ADDSTRING, 0, (size_t)L"한국어");

    Ex_ObjSendMessage(m_hComboBox1, CB_ADDSTRING, 0, (size_t)L"英文字母abc");
    Ex_ObjSendMessage(m_hComboBox1, CB_ADDSTRING, 0, (size_t)L"数字123");
    Ex_ObjSendMessage(m_hComboBox1, CB_ADDSTRING, 0, (size_t)L"中文");
    Ex_ObjSendMessage(m_hComboBox1, CB_ADDSTRING, 0, (size_t)L"特殊字符[！（）");

    Ex_ObjSendMessage(m_hComboBox2, CB_ADDSTRING, 0, (size_t)L"英文字母abc");
    Ex_ObjSendMessage(m_hComboBox2, CB_ADDSTRING, 0, (size_t)L"数字123");
    Ex_ObjSendMessage(m_hComboBox2, CB_ADDSTRING, 0, (size_t)L"中文");
    Ex_ObjSendMessage(m_hComboBox2, CB_ADDSTRING, 0, (size_t)L"特殊字符[！（）");

    Ex_ObjHandleEvent(m_hComboBox, COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);
    Ex_ObjHandleEvent(m_hComboBox1, COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);
    Ex_ObjHandleEvent(m_hComboBox2, COMBOBOX_EVENT_SELCHANGE, OnComboBoxButtonEvent);

    m_hComboBoxButton[0] =
        Ex_ObjCreate(L"button", L"添加项目", -1, 230, 30, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[0], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[1] =
        Ex_ObjCreate(L"button", L"插入项目", -1, 230, 70, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[1], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[2] =
        Ex_ObjCreate(L"button", L"取内容", -1, 230, 110, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[2], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[3] =
        Ex_ObjCreate(L"button", L"置内容", -1, 340, 30, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[3], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[4] =
        Ex_ObjCreate(L"button", L"清空表项", -1, 340, 70, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[4], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[5] =
        Ex_ObjCreate(L"button", L"弹出列表", -1, 340, 110, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[5], NM_CLICK, OnComboBoxButtonEvent);

    m_hComboBoxButton[6] =
        Ex_ObjCreate(L"button", L"删除项目", -1, 230, 150, 100, 30, hExDui_combobox);
    Ex_ObjHandleEvent(m_hComboBoxButton[6], NM_CLICK, OnComboBoxButtonEvent);


    Ex_DUIShowWindow(hExDui_combobox, SW_SHOWNORMAL, 0, 0, 0);
}
