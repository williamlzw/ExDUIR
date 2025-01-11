#include "test_fullscreen.h"

LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_NCLBUTTONDBLCLK) {
        // 禁用标题栏双击最大化消息
        return 1;
    }
    else if (uMsg == WM_NCLBUTTONDOWN) {
        // 禁用标题栏鼠标按下拖动消息
        if (wParam == HTCAPTION) {
            return 1;
        }
    }
    return 0;
}

void test_fullscreen(HWND hWnd)
{
    HWND hWnd_fullscreen =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试全屏,最大化,置顶,不可移动改变大小,只能右上角关闭",
                     0, 0, 200, 200, 0, 0);
    HEXDUI hExDui_fullscreen =
        Ex_DUIBindWindowEx(hWnd_fullscreen, 0,
                           WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_TITLE |
                               WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_FULLSCREEN |
                               WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE,
                           0, OnFullScreenWndMsgProc);
    Ex_DUISetLong(hExDui_fullscreen, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    // 最大化
    PostMessageW(hWnd_fullscreen, 274, 61488, 0);
    // 置顶
    SetWindowPos(hWnd_fullscreen, (HWND)-1, 0, 0, 0, 0, 3);
    Ex_DUIShowWindow(hExDui_fullscreen, SW_SHOWNORMAL, 0, 0, 0);
}
