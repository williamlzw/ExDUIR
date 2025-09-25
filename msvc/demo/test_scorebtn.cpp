#include "test_scorebtn.h"

std::vector<HEXOBJ> m_hScoreBtn(5);

LRESULT CALLBACK OnScoreButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                         LPARAM lParam)
{
    if (lParam != 0) {
        INT nIndex = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        OUTPUTW(L"选择分数", nIndex);
    }
    return 0;
}

void test_scorebtn(HWND hParent)
{
    HWND hWnd_score = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试打分按钮", 0, 0, 300, 100, 0, 0);
    HEXDUI hExDui_score = Ex_DUIBindWindowEx(hWnd_score, 0,
                                             WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_CENTERWINDOW |
                                                 WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
                                                 WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
                                             0, 0);
    Ex_DUISetLong(hExDui_score, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXIMAGE hImg1 = 0;
    HEXIMAGE hImg2 = 0;
    for (int i = 0; i < 5; i++) {
        m_hScoreBtn[i] = Ex_ObjCreate(L"ScoreButton", 0, -1, 20 + i * 45, 40, 40, 40, hExDui_score);
        _img_createfromfile(L"./res/navbtn/star_normal.png", &hImg1);

        Ex_ObjSendMessage(m_hScoreBtn[i], BM_SETIMAGE, 0, hImg1);

        _img_createfromfile(L"./res/navbtn/star_hover.png", &hImg2);

        Ex_ObjSendMessage(m_hScoreBtn[i], BM_SETIMAGE, 1, hImg2);
        Ex_ObjSetLong(m_hScoreBtn[i], OBJECT_LONG_LPARAM, i);
        Ex_ObjHandleEvent(m_hScoreBtn[i], NM_CHECK, OnScoreButtonCheckEvent);
    }

    Ex_DUIShowWindow(hExDui_score, SW_SHOWNORMAL);
}
