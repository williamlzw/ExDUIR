#include "test_palette.h"


HEXDUI m_hExDuiPalette;

LRESULT CALLBACK OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == PALETTE_EVENT_MOUSEMOVE) {
        Ex_DUISetLong(m_hExDuiPalette, ENGINE_LONG_CRBKG, ExRGB2ARGB(wParam, 255));
    }
    return 0;
}

void test_palette(HWND hParent)
{
    HWND hWnd_palette = Ex_WndCreate(hParent, L"Ex_DirectUI", L"测试调色板", 0, 0, 400, 250, 0, 0);
    m_hExDuiPalette = Ex_DUIBindWindowEx(hWnd_palette, 0,
                                         WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE |
                                             WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
                                             WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
                                             WINDOW_STYLE_HASICON,
                                         0, 0);
    Ex_DUISetLong(m_hExDuiPalette, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj = Ex_ObjCreate(L"Palette", 0, -1, 50, 50, 300, 150, m_hExDuiPalette);
    Ex_ObjHandleEvent(hObj, PALETTE_EVENT_MOUSEMOVE, OnPaletteEvent);
    Ex_DUIShowWindow(m_hExDuiPalette, SW_SHOWNORMAL, 0, 0, 0);
}
