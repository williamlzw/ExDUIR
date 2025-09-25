#include "test_rotateimgbox.h"

void test_rotateimgbox(HWND hWnd)
{
    HWND hWnd_rotateimgbox =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试旋转图片框", 0, 0, 200, 200, 0, 0);
    HEXDUI hExDui_rotateimgbox = Ex_DUIBindWindowEx(
        hWnd_rotateimgbox, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_rotateimgbox, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HEXOBJ hObj_imagebox =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"RotateImageBox", L"旋转背景但文字不变",
                       OBJECT_STYLE_VISIBLE, 50, 50, 100, 100, hExDui_rotateimgbox, 0, -1, 0, 0, 0);
    Ex_ObjSetColor(hObj_imagebox, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(255, 255), TRUE);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/rotateimgbox.jpg", &imgdata);
    Ex_ObjSetBackgroundImage(hObj_imagebox, imgdata.data(), imgdata.size(), 0, 0,
                             BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_PLAYIMAGE, 255, TRUE);
    Ex_ObjSetRadius(hObj_imagebox, 50, 50, 50, 50, TRUE);
    Ex_DUIShowWindow(hExDui_rotateimgbox, SW_SHOWNORMAL);
}
