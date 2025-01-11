#include "test_buttonex.h"

void test_buttonex(HWND hWnd)
{
    HWND hWnd_buttonex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展按钮", 0, 0, 300, 300, 0, 0);
    HEXDUI hExDui_buttonex = Ex_DUIBindWindowEx(
        hWnd_buttonex, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_buttonex, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj_btnex1 =
        Ex_ObjCreate(L"ButtonEx", L"☏直角纯色按钮", -1, 50, 50, 100, 30, hExDui_buttonex);
    EXARGB       ThemeColor          = RGB(76, 175, 80);
    EX_OBJ_PROPS ButtonExprops       = {0};
    ButtonExprops.crBkgNormal        = ExRGB2ARGB(ThemeColor, 225);
    ButtonExprops.crBkgHover         = ExRGB2ARGB(ThemeColor, 250);
    ButtonExprops.crBkgDownOrChecked = ExRGB2ARGB(ThemeColor, 200);
    Ex_ObjSendMessage(hObj_btnex1, WM_EX_PROPS, 0, (size_t)&ButtonExprops);

    HEXOBJ hObj_btnex2 =
        Ex_ObjCreate(L"ButtonEx", L"点击进入 👉", -1, 50, 100, 100, 30, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops2       = {0};
    ButtonExprops2.crBkgNormal        = ExRGB2ARGB(7037666, 255);
    ButtonExprops2.crBkgHover         = ExRGB2ARGB(6182117, 255);
    ButtonExprops2.crBkgDownOrChecked = ExRGB2ARGB(4865258, 255);
    ButtonExprops2.radius             = 15;
    Ex_ObjSendMessage(hObj_btnex2, WM_EX_PROPS, 0, (size_t)&ButtonExprops2);

    HEXOBJ hObj_btnex3 =
        Ex_ObjCreate(L"ButtonEx", L"Metro按钮☪", -1, 50, 150, 100, 30, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops3          = {0};
    ButtonExprops3.crBkgNormal           = ExARGB(130, 130, 130, 255);
    ButtonExprops3.crBkgHover            = ExARGB(130, 130, 130, 235);
    ButtonExprops3.crBkgDownOrChecked    = ExARGB(200, 200, 200, 100);
    ButtonExprops3.crBorderNormal        = ExARGB(130, 130, 130, 255);
    ButtonExprops3.crBorderHover         = ExARGB(0, 0, 0, 135);
    ButtonExprops3.crBorderDownOrChecked = ExARGB(0, 0, 0, 150);
    ButtonExprops3.strokeWidth           = 2;
    Ex_ObjSendMessage(hObj_btnex3, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops3);

    HEXOBJ hObj_btnex4 =
        Ex_ObjCreate(L"ButtonEx", L"图标在左", -1, 50, 200, 100, 30, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops4       = {0};
    ButtonExprops4.crBkgNormal        = ExRGB2ARGB(10066176, 255);
    ButtonExprops4.crBkgHover         = ExRGB2ARGB(10066176, 220);
    ButtonExprops4.crBkgDownOrChecked = ExRGB2ARGB(10066176, 200);
    Ex_ObjSendMessage(hObj_btnex4, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops4);
    Ex_ObjSetFontFromFamily(hObj_btnex4, L"楷体", 16, -1, TRUE);
    HEXIMAGE hImg;
    _img_createfromfile(L"buttonex\\5.png", &hImg);
    Ex_ObjSendMessage(hObj_btnex4, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

    EX_IMAGEINFO IMG0 = {0};
    _img_createfromfile(L"buttonex/4正常.png", &IMG0.imgNormal);
    _img_createfromfile(L"buttonex/4点燃.png", &IMG0.imgHover);
    _img_createfromfile(L"buttonex/4按下.png", &IMG0.imgDownOrChecked);
    HEXOBJ hObj_btnex5 =
        Ex_ObjCreate(L"ButtonEx", NULL, -1, 50, 250, 100, 30, hExDui_buttonex);   // 图片按钮
    Ex_ObjSendMessage(hObj_btnex5, BM_SETIMAGE, 0, (LPARAM)&IMG0);

    HEXOBJ hObj_btnex6 =
        Ex_ObjCreate(L"ButtonEx", L"背景按钮", -1, 180, 50, 100, 30, hExDui_buttonex);
    Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
    Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
    Ex_ObjSetColor(hObj_btnex6, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
    EX_IMAGEINFO IMG = {0};
    _img_createfromfile(L"buttonex/正常.png", &IMG.imgNormal);
    _img_createfromfile(L"buttonex/进入.png", &IMG.imgHover);
    _img_createfromfile(L"buttonex/按下.png", &IMG.imgDownOrChecked);
    Ex_ObjSendMessage(hObj_btnex6, BM_SETIMAGE, 0, (LPARAM)&IMG);

    HEXOBJ hObj_btnex7 =
        Ex_ObjCreate(L"ButtonEx", L"渐变按钮🔊", -1, 180, 100, 100, 30, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops7 = {0};
    ButtonExprops7.crBkgBegin   = ExARGB(0, 173, 241, 255);
    ButtonExprops7.crBkgEnd     = ExARGB(100, 25, 129, 255);
    ButtonExprops7.strokeWidth  = 2;
    Ex_ObjSendMessage(hObj_btnex7, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops7);
    Ex_ObjSetFontFromFamily(hObj_btnex7, L"楷体", 16, -1, TRUE);

    HEXOBJ hObj_btnex8 =
        Ex_ObjCreate(L"ButtonEx", L"线框按钮", -1, 180, 150, 100, 30, hExDui_buttonex); /*图标按钮*/
    EX_OBJ_PROPS ButtonExprops8  = {0};
    ButtonExprops8.crBorderBegin = ExARGB(227, 34, 103, 255);
    ButtonExprops8.crBorderEnd   = ExRGB2ARGB(16746496, 255);
    ButtonExprops8.strokeWidth   = 2;
    Ex_ObjSendMessage(hObj_btnex8, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops8);
    Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(65535, 180), FALSE);
    Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(65535, 255), FALSE);
    Ex_ObjSetColor(hObj_btnex8, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(65535, 255), FALSE);
    Ex_ObjSetFontFromFamily(hObj_btnex8, L"楷体", 16, -1, TRUE);

    HEXOBJ hObj_btnex9 =
        Ex_ObjCreate(L"ButtonEx", L"图标在上", -1, 180, 200, 100, 40, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops9       = {0};
    ButtonExprops9.crBkgNormal        = ExRGB2ARGB(10061616, 255);
    ButtonExprops9.crBkgHover         = ExRGB2ARGB(10061616, 220);
    ButtonExprops9.crBkgDownOrChecked = ExRGB2ARGB(10061616, 200);
    ButtonExprops9.nIconPosition      = 2;
    Ex_ObjSendMessage(hObj_btnex9, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops9);
    Ex_ObjSetFontFromFamily(hObj_btnex9, L"楷体", 16, -1, TRUE);
    _img_createfromfile(L"buttonex\\4.png", &hImg);
    Ex_ObjSendMessage(hObj_btnex9, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

    HEXOBJ hObj_btnex10 =
        Ex_ObjCreate(L"ButtonEx", L"图标在右", -1, 180, 250, 100, 30, hExDui_buttonex);
    EX_OBJ_PROPS ButtonExprops10          = {0};
    ButtonExprops10.crBkgNormal           = ExARGB(255, 255, 255, 50);
    ButtonExprops10.crBkgHover            = ExARGB(255, 255, 255, 80);
    ButtonExprops10.crBkgDownOrChecked    = ExARGB(255, 255, 255, 100);
    ButtonExprops10.crBorderNormal        = ExARGB(0, 0, 0, 150);
    ButtonExprops10.crBorderHover         = ExARGB(0, 0, 0, 180);
    ButtonExprops10.crBorderDownOrChecked = ExARGB(0, 0, 0, 200);
    ButtonExprops10.strokeWidth           = 1;
    ButtonExprops10.nIconPosition         = 1;
    ButtonExprops10.radius                = 7;
    Ex_ObjSendMessage(hObj_btnex10, WM_EX_PROPS, 0, (LPARAM)&ButtonExprops10);
    Ex_ObjSetFontFromFamily(hObj_btnex10, L"楷体", 16, -1, TRUE);
    _img_createfromfile(L"buttonex\\3.png", &hImg);
    Ex_ObjSendMessage(hObj_btnex10, WM_SETICON, 0, (LPARAM)hImg); /* 设置图标; */

    Ex_DUIShowWindow(hExDui_buttonex, SW_SHOWNORMAL, 0, 0, 0);
}
