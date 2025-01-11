#include "test_colorbutton.h"


LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_MOUSEHOVER) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_SETFOCUS) {
        Ex_ObjSetUIState(hObj, STATE_FOCUS, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_KILLFOCUS) {
        Ex_ObjSetUIState(hObj, STATE_FOCUS, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_SYSCOLORCHANGE) {
        if (wParam == COLOR_EX_CBTN_CRBKG_NORMAL) {
            Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_NORMAL, lParam);
        }
        else if (wParam == COLOR_EX_CBTN_CRBKG_HOVER) {
            Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_HOVER, lParam);
        }
        else if (wParam == COLOR_EX_CBTN_CRBKG_DOWN) {
            Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_DOWN, lParam);
        }
        else if (wParam == COLOR_EX_CBTN_CRBKG_FOCUS) {
            Ex_ObjSetLong(hObj, EOUL_CBTN_CRBKG_FOCUS, lParam);
        }
    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            INT crText = 0;
            INT crBkg  = 0;
            if ((ps.dwState & STATE_DOWN) == STATE_DOWN) {
                crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_DOWN);
                crBkg  = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_DOWN);
            }
            else if ((ps.dwState & STATE_HOVER) == STATE_HOVER) {
                crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_HOVER);
                crBkg  = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_HOVER);
            }
            else if ((ps.dwState & STATE_FOCUS) == STATE_FOCUS) {
                crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_FOCUS);
                crBkg  = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_FOCUS);
            }
            if (crBkg == 0) {
                crBkg = Ex_ObjGetLong(hObj, EOUL_CBTN_CRBKG_NORMAL);
            }
            if (crText == 0) {
                crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
            }
            _canvas_clear(ps.hCanvas, crBkg);
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), crText,
                             (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                             ps.dwTextFormat, 0, 0, ps.uWidth, ps.uHeight);
            Ex_ObjEndPaint(hObj, &ps);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void ColorButton2::Create(EXHANDLE handle, INT left, INT top, INT width, INT height, LPCWSTR title,
                          INT style, INT styleEx, INT nID, INT dwTextFormat, LPARAM lParam,
                          MsgPROC lpMsgProc)
{
    mhObj = Ex_ObjCreateEx(styleEx, L"colorbutton", title, style, left, top, width, height, handle,
                           nID, dwTextFormat, lParam, 0, lpMsgProc);
}

void ColorButton2::SetBkgNormalColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_NORMAL, color, redraw);
}

void ColorButton2::SetBkgHoverColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_HOVER, color, redraw);
}

void ColorButton2::SetBkgDownColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_DOWN, color, redraw);
}

void ColorButton2::SetBkgFocusColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_CBTN_CRBKG_FOCUS, color, redraw);
}

void ColorButton2::SetTextHoverColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_HOVER, color, redraw);
}

void ColorButton2::SetTextDownColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_DOWN, color, redraw);
}

void ColorButton2::SetTextFocusColor(EXARGB color, BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetColor(mhObj, COLOR_EX_TEXT_FOCUS, color, redraw);
}

void ColorButton2::SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft,
                             BOOL redraw)
{
    if (mhObj == 0) return;
    Ex_ObjSetRadius(mhObj, topleft, topright, bottomright, bottomleft, redraw);
}

void test_colorbutton(HWND hWnd)
{
    HWND hWnd_colorbutton =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试自定义按钮", 0, 0, 300, 150, 0, 0);
    HEXDUI hExDui_colorbutton = Ex_DUIBindWindowEx(
        hWnd_colorbutton, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_colorbutton, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_ObjRegister(
        L"colorbutton", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_FOCUSABLE,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER, 4 * sizeof(size_t), 0, 0, OnColorButtonProc);

    HEXOBJ hObj_colorbutton1 = Ex_ObjCreateEx(-1, L"colorbutton", L"按钮1", -1, 50, 50, 100, 50,
                                              hExDui_colorbutton, 0, -1, 0, 0, NULL);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_HOVER, ExRGB2ARGB(65535, 255), FALSE);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_DOWN, ExRGB2ARGB(65280, 255), FALSE);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_CBTN_CRBKG_FOCUS, ExRGB2ARGB(16777680, 255), FALSE);

    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(255, 255), FALSE);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16754943, 255), FALSE);
    Ex_ObjSetColor(hObj_colorbutton1, COLOR_EX_TEXT_FOCUS, ExRGB2ARGB(65280, 255), TRUE);

    Ex_ObjSetRadius(hObj_colorbutton1, 25, 25, 25, 25, TRUE);

    // 下面演示类封装组件
    ColorButton2 button;
    button.Create(hExDui_colorbutton, 160, 50, 100, 50, L"按钮2");
    button.SetBkgNormalColor(ExRGB2ARGB(16777215, 255), FALSE);
    button.SetBkgHoverColor(ExRGB2ARGB(65535, 255), FALSE);
    button.SetBkgDownColor(ExRGB2ARGB(65280, 255), FALSE);
    button.SetBkgFocusColor(ExRGB2ARGB(16777680, 255), FALSE);
    button.SetTextHoverColor(ExRGB2ARGB(255, 255), FALSE);
    button.SetTextDownColor(ExRGB2ARGB(16754943, 255), FALSE);
    button.SetTextFocusColor(ExRGB2ARGB(65280, 255), TRUE);
    button.SetRadius(25, 25, 25, 25, TRUE);
    Ex_DUIShowWindow(hExDui_colorbutton, SW_SHOWNORMAL, 0, 0, 0);
}
