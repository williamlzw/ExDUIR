#include "test_ani.h"

HWND      m_hWndAni   = 0;
HEXDUI    m_hExDuiAni = 0;
HEXEASING m_hEasing   = nullptr;

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult)
{
    if (uMsg == WM_CLOSE) {
        AniShow(FALSE);
    }
    return 0;
}

size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent,
                            LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2,
                            size_t p3, size_t p4)
{
    INT index = nCurrent * 255;
    Ex_DUISetLong(m_hExDuiAni, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, index));
    Ex_DUISetLong(m_hExDuiAni, ENGINE_LONG_ALPHA, index);
    SetWindowPos((HWND)Ex_DUIGetLong(m_hExDuiAni, ENGINE_LONG_HWND), 0, p1 + p2 * nCurrent,
                 p3 + p4 * nCurrent, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
    return 0;
}

void AniShow(BOOL fShow)
{
    Ex_DUISetLong(m_hExDuiAni, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, fShow ? 0 : 255));
    Ex_DUISetLong(m_hExDuiAni, ENGINE_LONG_ALPHA, fShow ? 0 : 255);
    Ex_DUIShowWindow(m_hExDuiAni, SW_SHOW);

    RECT rc{0};
    GetWindowRect((HWND)Ex_DUIGetLong(m_hExDuiAni, ENGINE_LONG_HWND), &rc);
    _easing_create(
        EASING_TYPE_INOUTQUINT, 0,
        EASING_MODE_SINGLE | EASING_MODE_CALLFUNCTION | (fShow ? 0 : EASING_MODE_REVERSE),
        (size_t)OnAniEasing, 500, 20, EASING_STATE_PLAY, 0, 1, rc.left, 0, rc.top - 100, 100);
    Ex_DUIShowWindow(m_hExDuiAni, fShow ? SW_SHOW : SW_HIDE);
}

size_t CALLBACK OnBtnEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent,
                            LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2,
                            size_t p3, size_t p4)
{
    Ex_ObjSetPos(p1, 0, 0, 0, Ex_Scale(nCurrent), Ex_Scale(50), SWP_NOMOVE | SWP_NOZORDER | SWP_EX_NODPISCALE);
    return 0;
}

size_t CALLBACK OnBtnEasing4(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent,
                             LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2,
                             size_t p3, size_t p4)
{
    RECT rc{0};
    GetWindowRect(m_hWndAni, &rc);
    MoveWindow(m_hWndAni, rc.left, rc.top, Ex_Scale(nCurrent), rc.bottom - rc.top, TRUE);
    return 0;
}

LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 10001) {
        if (nCode == NM_CLICK) {
            _easing_create(EASING_TYPE_INOUTCUBIC, 0,
                           EASING_MODE_SINGLE | EASING_MODE_THREAD | EASING_MODE_CALLFUNCTION,
                           (size_t)OnBtnEasing, 200, 20, EASING_STATE_PLAY, 150, 300, hObj, 0, 0,
                           0);
        }
    }
    else if (nID == 10002) {
        if (nCode == NM_CLICK) {
            _easing_create(EASING_TYPE_INOUTCUBIC, 0,
                           MAKELONG(EASING_MODE_MANYTIMES | EASING_MODE_BACKANDFORTH |
                                        EASING_MODE_THREAD | EASING_MODE_DISPATCHNOTIFY,
                                    10 * 2),
                           hObj, 150, 20, EASING_STATE_PLAY, 150, 300, 0, 0, 0, 0);
        }
        else if (nCode == NM_EASING) {
            EX_EASINGINFO pEasingInfo{0};
            RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
            Ex_ObjSetPos(hObj, 0, 0, 0, Ex_Scale(pEasingInfo.nCurrent), Ex_Scale(50), SWP_NOMOVE | SWP_NOZORDER | SWP_EX_NODPISCALE);
        }
    }
    else if (nID == 10003) {
        if (nCode == NM_CLICK) {
            if (_easing_getstate(m_hEasing) == EASING_STATE_PAUSE) {
                _easing_setstate(m_hEasing, EASING_STATE_PLAY);
            }
            else {
                _easing_setstate(m_hEasing, EASING_STATE_PAUSE);
            }
        }
        else if (nCode == NM_DESTROY) {
            _easing_setstate(m_hEasing, EASING_STATE_STOP);
        }
        else if (nCode == NM_EASING) {
            EX_EASINGINFO pEasingInfo{0};
            RtlMoveMemory(&pEasingInfo, (LPVOID)lParam, sizeof(EX_EASINGINFO));
            Ex_ObjSetPos(hObj, 0, 0, 0, Ex_Scale(pEasingInfo.nCurrent), Ex_Scale(50), SWP_NOMOVE | SWP_NOZORDER | SWP_EX_NODPISCALE);
        }
    }
    else if (nID == 10004) {
        if (nCode == NM_CLICK) {
            _easing_create(
                EASING_TYPE_INOUTCIRC, 0,
                MAKELONG(
                    EASING_MODE_MANYTIMES | EASING_MODE_BACKANDFORTH | EASING_MODE_CALLFUNCTION, 4),
                (size_t)OnBtnEasing4, 400, 25, 0, (400), (150), 0, 0, 0, 0);
        }
    }
    return 0;
}

void test_ani(HWND hWnd)
{
    m_hWndAni   = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试缓动窗口", 0, 0, 400, 300, 0, 0);
    m_hExDuiAni = Ex_DUIBindWindowEx(
        m_hWndAni, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_SIZEABLE |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, OnAniWndMsgProc);
    HEXOBJ hObj_button1 = Ex_ObjCreateEx(-1, L"button", L"点击就动1次", -1, 10, 40, 120, 50,
                                         m_hExDuiAni, 10001, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    HEXOBJ hObj_button2 = Ex_ObjCreateEx(-1, L"button", L"来回", -1, 10, 100, 120, 50, m_hExDuiAni,
                                         10002, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    HEXOBJ hObj_button3 = Ex_ObjCreateEx(-1, L"button", L"点击动/停", -1, 10, 160, 120, 50,
                                         m_hExDuiAni, 10003, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    HEXOBJ hObj_button4 = Ex_ObjCreateEx(-1, L"button", L"点击窗口动", -1, 10, 220, 120, 50,
                                         m_hExDuiAni, 10004, DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(hObj_button1, NM_CLICK, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button2, NM_EASING, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button3, NM_CLICK, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button3, NM_EASING, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button3, NM_DESTROY, OnAniButtonEvent);
    Ex_ObjHandleEvent(hObj_button4, NM_CLICK, OnAniButtonEvent);
    m_hEasing = _easing_create(EASING_TYPE_INOUTCIRC, 0,
                               EASING_MODE_CYCLE | EASING_MODE_BACKANDFORTH | EASING_MODE_THREAD |
                                   EASING_MODE_DISPATCHNOTIFY,
                               hObj_button3, 200, 20, EASING_STATE_PAUSE, 150, 300, 0, 0, 0, 0);
    AniShow(TRUE);
}
