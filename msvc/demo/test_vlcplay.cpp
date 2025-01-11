#include "test_vlcplay.h"

HEXOBJ m_hObjMediaVLC;
HEXOBJ m_hObjMediaVLCButton1;
HEXOBJ m_hObjMediaVLCButton2;
HEXOBJ m_hObjMediaVLCButton3;
HEXOBJ m_hObjMediaVLCButton4;
HEXOBJ m_hObjMediaVLCButton5;
HEXOBJ m_hObjMediaVLCButton6;
HEXOBJ m_hObjMediaVLCButton7;

LRESULT CALLBACK OnMediaVLCBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (hObj == m_hObjMediaVLCButton1) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_PLAY, 0,
                          (LPARAM)L"./res/test.mp4");
        // Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_PLAYFROMURL, 0,
        // (LPARAM)L"https://media.w3.org/2010/05/sintel/trailer.mp4");
    }
    else if (hObj == m_hObjMediaVLCButton2) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_PAUSE, 0, 0);
    }
    else if (hObj == m_hObjMediaVLCButton3) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_RESUME, 0, 0);
        auto curTime   = Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_GET_MEDIATIME, 0, 0);
        auto totalTime = Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_GET_DURATION, 0, 0);
        auto rate      = Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_GET_RATE, 0, 0);
        auto volume    = Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_GET_VOLUME, 0, 0);
        OUTPUTW(L"当前时间(ms):", curTime, L"总时间(ms):", totalTime, L"速率:", rate, L"音量:",
                volume);
    }
    else if (hObj == m_hObjMediaVLCButton4) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_STATE_STOP, 0, 0);
    }
    else if (hObj == m_hObjMediaVLCButton5) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_SET_MEDIATIME, 0, 20000);
    }
    else if (hObj == m_hObjMediaVLCButton6) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_SET_VOLUME, 0, 100);
    }
    else if (hObj == m_hObjMediaVLCButton7) {
        Ex_ObjSendMessage(m_hObjMediaVLC, VLCPLAYER_MESSAGE_SET_RATE, 0, 2);
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
    m_hObjMediaVLC = Ex_ObjCreate(L"VLCPlayer", NULL, -1, 50, 50, 800, 500, hExDui_mediavlc);
    m_hObjMediaVLCButton1 = Ex_ObjCreate(L"button", L"播放", -1, 50, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton2 =
        Ex_ObjCreate(L"button", L"暂停", -1, 160, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton3 =
        Ex_ObjCreate(L"button", L"继续", -1, 270, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton4 =
        Ex_ObjCreate(L"button", L"停止", -1, 380, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton5 =
        Ex_ObjCreate(L"button", L"置媒体时间", -1, 490, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton6 =
        Ex_ObjCreate(L"button", L"置音量大小", -1, 600, 560, 100, 30, hExDui_mediavlc);
    m_hObjMediaVLCButton7 =
        Ex_ObjCreate(L"button", L"置播放速率", -1, 710, 560, 100, 30, hExDui_mediavlc);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton1, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton2, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton3, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton4, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton5, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton6, NM_CLICK, OnMediaVLCBtnEnevt);
    Ex_ObjHandleEvent(m_hObjMediaVLCButton7, NM_CLICK, OnMediaVLCBtnEnevt);

    Ex_DUIShowWindow(hExDui_mediavlc, SW_SHOWNORMAL, 0, 0, 0);
}
