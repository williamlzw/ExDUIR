#include "test_tray.h"

HEXDUI m_hExDui_tray;
HICON  m_icon = 0;

LRESULT CALLBACK OnTrayWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_CLOSE) {
        if (m_icon != 0) {
            // 销毁图标
            DeleteObject(m_icon);
            m_icon = 0;
        }
    }
    else if (uMsg == WM_NOTIFY) {
        EX_NMHDR ni{0};
        RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (ni.nCode == NM_TRAYICON) {
            INT16 nType = LOWORD(ni.lParam);
            if (nType == WM_RBUTTONDOWN)   // 右键按下托盘图标
            {
                Ex_DUITrayIconPopup(m_hExDui_tray, L"弹出托盘内容", L"弹出托盘标题", NIIF_INFO);
            }
            else if (nType == WM_LBUTTONDBLCLK)
            {
                // 双击左键显示窗口
                ShowWindow(hWnd, SW_RESTORE);
                SetForegroundWindow(hWnd);
            }
        }
    }
    else if (uMsg == WM_SIZE)
    {
        // 窗口大小改变时处理最小化
        if (wParam == SIZE_MINIMIZED)
        {
            // 最小化时隐藏窗口，并显示托盘图标
            std::vector<CHAR> data;
            Ex_ReadFile(L"res\\icon.ico", &data);
            m_icon = (HICON)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_ICON, 0);
            Ex_DUITrayIconSet(m_hExDui_tray, m_icon, L"ExDuiR");
            ShowWindow(hWnd, SW_HIDE);
        }
    }
    return 0;
}

void test_tray(HWND hWnd)
{
    // 若要创建任务栏不显示的窗口，dwStyleEx填WS_EX_TOOLWINDOW 扩展风格
    HWND hWnd_tray = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试托盘图标", 0, 0, 250, 150, 0, WS_EX_TOOLWINDOW);
    m_hExDui_tray = Ex_DUIBindWindowEx(hWnd_tray, 0,
                                       WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                           WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | 
                                           WINDOW_STYLE_TITLE | WINDOW_STYLE_BUTTON_MIN,
                                       0, OnTrayWndMsgProc);
    Ex_DUISetLong(m_hExDui_tray, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjCreateEx(-1, L"static", L"最小化可以隐藏显示在托盘图标,右击托盘图标可以弹出托盘内容，双击托盘图标可以恢复显示窗口", -1, 10, 50, 200, 60, m_hExDui_tray,
                   0, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_DUIShowWindow(m_hExDui_tray, SW_SHOWNORMAL, 0, 0, 0);
}
