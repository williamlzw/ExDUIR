#include "test_nativewindow.h"

HWND   m_hWndChild;
HWND   m_hWndChild2;
HWND   m_hWndNativeWindow2;
HEXOBJ m_hObjPage;

LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                          LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        INT  width  = LOWORD(lParam);
        INT  height = HIWORD(lParam);
        auto dpiy   = Ex_DUIGetSystemDpi();
        MoveWindow(m_hWndNativeWindow2, 0, 30 * dpiy, width, height - 30 * dpiy, TRUE);
        MoveWindow(m_hWndChild, 0, 0, width - 30, 300 * dpiy, TRUE);
        MoveWindow(m_hWndChild2, 0, 310 * dpiy, width - 30, 300 * dpiy, TRUE);
        Ex_ObjMove(m_hObjPage, 0, 0, width / dpiy, height / dpiy - 30, TRUE);
        Ex_ObjScrollSetInfo(m_hObjPage, SCROLLBAR_TYPE_VERT, SIF_ALL, 0,
                            (300 + 300 - height / dpiy + 30) * dpiy, 100, 0, TRUE);
    }
    return 0;
}

LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
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

INT Ex_ObjScrollDefaultProc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage,
                            BOOL fRedraw)
{
    auto hObjScroll = Ex_ObjScrollGetControl(hObj, nBar);
    INT  nPos       = 0;
    if (hObjScroll != 0) {
        DWORD nCode = LOWORD(wParam);
        INT   nMin, nMax, oldPos, trackPos;
        Ex_ObjScrollGetInfo(hObj, nBar, &nMin, &nMax, &oldPos, &trackPos);
        if (nCode == SB_PAGEUP) {
            nPos = oldPos - nPage;
        }
        else if (nCode == SB_PAGEDOWN) {
            nPos = oldPos + nPage;
        }
        else if (nCode == SB_LINEUP) {
            nPos = oldPos - nLine;
        }
        else if (nCode == SB_LINEDOWN) {
            nPos = oldPos + nLine;
        }
        else if (nCode == SB_TOP) {
            nPos = nMin;
        }
        else if (nCode == SB_BOTTOM) {
            nPos = nMax;
        }
        else {
            oldPos = nMin - 1;
            nPos   = Ex_ObjScrollGetTrackPos(hObj, nBar);
        }
        if (nPos < nMin) {
            nPos = nMin;
        }
        if (nPos > nMax) {
            nPos = nMax;
        }
        if (nPos != oldPos) {
            Ex_ObjScrollSetPos(hObj, nBar, nPos, fRedraw);
        }
    }
    return nPos;
}

LRESULT CALLBACK OnNativeWndScrollMsg(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_VSCROLL) {
        auto nPos = Ex_ObjScrollDefaultProc(hObj, SCROLLBAR_TYPE_VERT, wParam, 1, 10, TRUE);
        RECT rc;
        GetWindowRect(m_hWndChild, &rc);
        auto dpiy = Ex_DUIGetSystemDpi();
        MoveWindow(m_hWndChild, 0, 0 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top, TRUE);
        MoveWindow(m_hWndChild2, 0, 310 * dpiy - nPos, rc.right - rc.left, rc.bottom - rc.top,
                   TRUE);
    }
    return 0;
}

void test_nativewindow(HWND hParent)
{
    HWND hWnd_nativewindow =
        Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试原生子窗口", 0, 0, 400, 450, 0, 0);
    HEXDUI hExDui_nativewindow = Ex_DUIBindWindowEx(
        hWnd_nativewindow, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_MOVEABLE |
            WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_TITLE,
        0, OnNativeParentWndMsgProc);
    Ex_DUISetLong(hExDui_nativewindow, ENGINE_LONG_RADIUS, 10);
    Ex_DUISetLong(hExDui_nativewindow, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    m_hWndNativeWindow2 = Ex_WndCreate(hWnd_nativewindow, L"Ex_DirectUI", 0, 0, 30, 400, 420,
                                       WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
    HEXDUI hExDui_nativewindow2 =
        Ex_DUIBindWindowEx(m_hWndNativeWindow2, 0, WINDOW_STYLE_NOSHADOW, 0, OnNativeWndMsgProc);
    Ex_DUISetLong(hExDui_nativewindow2, ENGINE_LONG_CRBKG, ExARGB(100, 150, 150, 255));

    m_hObjPage = Ex_ObjCreateEx(-1, L"static", 0, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, 0, 0,
                                400, 420, hExDui_nativewindow2, 0, -1, 0, 0, OnNativeWndScrollMsg);

    Ex_ObjScrollSetInfo(m_hObjPage, SCROLLBAR_TYPE_VERT, SIF_ALL, 0, (300 + 300 - 420 + 30) * 1,
                        100, 0,
                        TRUE);   // 1是文本缩放比例
    Ex_ObjScrollShow(m_hObjPage, SCROLLBAR_TYPE_VERT, TRUE);

    // 子窗口是原生窗口，父窗口需要删除这个WS_EX_LAYERED风格
    SetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE,
                      GetWindowLongPtrW(hWnd_nativewindow, GWL_EXSTYLE) & ~WS_EX_LAYERED);

    m_hWndChild =
        Ex_WndCreate(m_hWndNativeWindow2, 0, 0, 0, 0, 300, 300, WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
    // 注意给子窗口添加一个回调，禁用移动和最大化
    auto hExDui_child =
        Ex_DUIBindWindowEx(m_hWndChild, 0, WINDOW_STYLE_NOSHADOW, 0, OnNativeWndMsgProc);
    // 可以在子窗口创建原生win32组件,mfc组件,第三方组件诸如cef,miniblink,webview2,锐浪报表,aplayer
    // 子窗口也可以设置背景色
    Ex_DUISetLong(hExDui_child, ENGINE_LONG_CRBKG, ExARGB(0, 0, 0, 255));

    m_hWndChild2 =
        Ex_WndCreate(m_hWndNativeWindow2, 0, 0, 0, 0, 300, 300, WS_CHILD | WS_OVERLAPPEDWINDOW, 0);
    auto hExDui_child2 =
        Ex_DUIBindWindowEx(m_hWndChild2, 0, WINDOW_STYLE_NOSHADOW, 0, OnNativeWndMsgProc);
    Ex_DUISetLong(hExDui_child2, ENGINE_LONG_CRBKG, ExARGB(0, 0, 0, 255));

    Ex_DUIShowWindow(hExDui_child, SW_SHOWNORMAL, 0, 0, 0);
    Ex_DUIShowWindow(hExDui_child2, SW_SHOWNORMAL, 0, 0, 0);
    Ex_DUIShowWindow(hExDui_nativewindow, SW_SHOWNORMAL, 0, 0, 0);
    Ex_DUIShowWindow(hExDui_nativewindow2, SW_SHOWNORMAL, 0, 0, 0);
}
