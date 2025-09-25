#include "test_modal.h"

HWND m_hWndModal;

INT_PTR CALLBACK OnDialgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG) {
        MoveWindow(hWnd, 0, 0, 300, 200, FALSE);
        SetWindowTextW(hWnd, L"我是模态对话框 我在的时候别的窗口不能动");
        HEXDUI hExDui = Ex_DUIBindWindow(hWnd, 0,
                                         WINDOW_STYLE_TITLE | WINDOW_STYLE_SIZEABLE |
                                             WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_BUTTON_CLOSE |
                                             WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_CENTERWINDOW);
        if (hExDui) {
            HEXOBJ hEdit = Ex_ObjCreate(L"edit", 0, -1, 20, 50, 250, 30, hExDui);
            if (hEdit) {
                Ex_ObjSendMessage(hEdit, EDIT_MESSAGE_SETCUEBANNER, ExRGB2ARGB(8421504, 200),
                                  (LPARAM)L"编辑框输入文字正常");
            }
            Ex_DUISetLong(hExDui, ENGINE_LONG_CRBKG, ExRGB2ARGB(16711680, 220));
            Ex_DUIShowWindow(hExDui, SW_SHOW);
        }
    }
    return 0;
}

LRESULT CALLBACK OnModalButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    HRSRC hrsrc =
        FindResourceW(GetModuleHandleW(0), MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), RT_DIALOG);
    if (hrsrc) {
        LPCDLGTEMPLATEW hglobal = (LPCDLGTEMPLATEW)LoadResource(GetModuleHandleW(0), hrsrc);
        if (hglobal) {
            DialogBoxIndirectParamW(GetModuleHandleW(0), hglobal, m_hWndModal, OnDialgWndProc, 0);
        }
    }
    return 0;
}

void test_modal(HWND hWnd)
{
    m_hWndModal = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试模态窗口", 0, 0, 400, 200, 0, 0);
    HEXDUI hExDui_modal = Ex_DUIBindWindowEx(m_hWndModal, 0,
                                             WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
                                                 WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
                                                 WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
                                                 WINDOW_STYLE_HASICON,
                                             0, 0);
    Ex_DUISetLong(hExDui_modal, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj = Ex_ObjCreate(L"button", L"弹出模态对话框", -1, 50, 50, 300, 100, hExDui_modal);
    Ex_ObjHandleEvent(hObj, NM_CLICK, OnModalButtonEvent);
    Ex_DUIShowWindow(hExDui_modal, SW_SHOWNORMAL);
}
