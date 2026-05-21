#include "test_vlcplay.h"

HEXOBJ m_hObjMediaVLC;
HEXOBJ m_hObjMediaVLC2;
HEXOBJ m_hObjMediaVLCButton1;

LRESULT CALLBACK OnMediaVLCBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (hObj == m_hObjMediaVLCButton1) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_LOAD, 0,
            (LPARAM)L"./res/test.mp4");
        Ex_ObjSendMessage(m_hObjMediaVLC2, VLCPLAYER_MESSAGE_STATE_LOADFROMURL, 0,
            (LPARAM)L"https://media.w3.org/2010/05/sintel/trailer.mp4");
    }
    return 0;
}

void test_vlcPlay(HWND hWnd)
{
    // 依赖文件 https://gitee.com/william_lzw/ex_libvlc/releases/tag/3.0.21
    // 32位必须用cdecl编译才能使用vlc
    HWND hWndmediavlc = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试VLC播放器", 0, 0, 900, 600, 0, 0);
    HEXDUI hExDui_mediavlc = Ex_DUIBindWindowEx(
        hWndmediavlc, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
        WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
        0, 0);
    Ex_DUISetLong(hExDui_mediavlc, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    m_hObjMediaVLC = Ex_ObjCreate(L"VLCPlayer", NULL, -1, 50, 50, 300, 300, hExDui_mediavlc);
    m_hObjMediaVLC2 = Ex_ObjCreate(L"VLCPlayer", NULL, -1, 400, 50, 400, 300, hExDui_mediavlc);
    m_hObjMediaVLCButton1 = Ex_ObjCreate(L"button", L"播放", -1, 50, 560, 100, 30, hExDui_mediavlc);

    Ex_ObjHandleEvent(m_hObjMediaVLCButton1, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_DUIShowWindow(hExDui_mediavlc, SW_SHOWNORMAL);
}
