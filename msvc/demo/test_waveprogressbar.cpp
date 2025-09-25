#include "test_waveprogressbar.h"

void test_waveprogressbar(HWND hWnd)
{
    HWND hWnd_progressbar =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试水波进度条", 0, 0, 600, 300, 0, 0);
    HEXDUI hExDui_progressbar = Ex_DUIBindWindowEx(
        hWnd_progressbar, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_progressbar, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_progressbar =
        Ex_ObjCreate(L"WaveProgressBar", NULL, -1, 20, 50, 560, 220, hExDui_progressbar);
    Ex_ObjSetColor(hObj_progressbar, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(16777215, 255), FALSE);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_POS, 40);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_WAVE_WIDTH, 210);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_LEFT_X, -210);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_WAVE_HEIGHT, 30);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_BACKGROUNDCOLOR,
                  ExARGB(230, 231, 232, 255));
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_FOREGROUNDCOLOR,
                  ExARGB(30, 159, 255, 200));
    HEXFONT hFont = _font_createfromfamily(L"MicroSoft Yahei", 30, 0);
    Ex_ObjSetLong(hObj_progressbar, WAVEPROGRESSBAR_LONG_FONT, hFont);
    Ex_ObjSetRadius(hObj_progressbar, 10, 10, 10, 10, FALSE);

    Ex_ObjSetTimer(hObj_progressbar, 100);


    Ex_DUIShowWindow(hExDui_progressbar, SW_SHOWNORMAL);
}
