#include "test_label.h"

void test_label(HWND hWnd)
{
    HWND hWnd_label = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试基础组件", 0, 0, 400, 400, 0, 0);
    HEXDUI hExDui_label = Ex_DUIBindWindowEx(hWnd_label, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
        WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
                                             0, 0);
    Ex_DUISetLong(hExDui_label, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
    OUTPUTW(L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    HEXOBJ hObj_label =
        Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE, 10, 30, 180,
                       150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/Loading.gif", &imgdata);
    Ex_ObjSetBackgroundImage(hObj_label, imgdata.data(), imgdata.size(), 0, 0,
                             BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_PLAYIMAGE, 255, TRUE);

    Ex_ObjSetRadius(hObj_label, 10, 10, 15, 10, TRUE);

    EX_BACKGROUNDIMAGEINFO bkg{0};
    Ex_ObjGetBackgroundImage(hObj_label, &bkg);
    OUTPUTW(L"背景信息:", bkg.x, bkg.y, bkg.dwAlpha, bkg.dwRepeat, bkg.hImage, bkg.curFrame,
            bkg.maxFrame);
    RECT rect;
    Ex_ObjGetRect(hObj_label, &rect);
    OUTPUTW(L"标签矩形:", rect.right, rect.bottom);
   
    HEXOBJ hObj_label2 =
        Ex_ObjCreateEx(-1, L"static", NULL, OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE, 200, 30,
                       180, 150, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
    Ex_ReadFile(L"res/409.dds", &imgdata);
    Ex_ObjSetBackgroundImage(hObj_label2, imgdata.data(), imgdata.size(), 0, 0,
                             BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_PLAYIMAGE, 255, TRUE);

    HEXOBJ hObj_label3 = Ex_ObjCreateEx(
        -1, L"static", L"基础组件可以填充动画,支持PNG,GIF,JPG,BMP,DDS,ICO,标签可以自动换行", -1,
        200, 200, 180, 90, hExDui_label, 0, DT_WORDBREAK, 0, 0, NULL);
    Ex_ObjSetFontFromFamily(hObj_label3, L"宋体", 14, FONT_STYLE_BOLD, FALSE);
    Ex_ObjSetColor(hObj_label3, COLOR_EX_TEXT_NORMAL, ExARGB(133, 33, 53, 255), TRUE);

    HEXOBJ hObj_label22 = Ex_ObjCreateEx(-1, L"static", L"标签可以填充动画", OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE, 10, -20, 80, 50, hObj_label, 0, DT_VCENTER, 0, 0, 0);
    Ex_ObjSetColor(hObj_label22, COLOR_EX_BACKGROUND, ExARGB(255, 33, 53, 255), TRUE);
    Ex_ObjSetRadius(hObj_label22, 20, 20, 25, 20, TRUE);

    Ex_DUIShowWindow(hExDui_label, SW_SHOWNORMAL);
}
