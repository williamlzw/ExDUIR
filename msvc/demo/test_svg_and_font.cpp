#include "test_svg_and_font.h"

LRESULT CALLBACK OnSvgAndFontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        _canvas_clear(wParam, ExARGB(150, 150, 150, 255));
        HEXFONT hFont = _font_createfromfile(L"res/文道灵飞小楷.ttf", 64);
        _canvas_drawtext(wParam, hFont, ExARGB(200, 0, 200, 200), L"我是测试文本", -1, -1, 20, 450,
                         450, 530);
        _font_destroy(hFont);

        _canvas_drawsvgfromfile(wParam, L"./res/niu.svg", 0, 50, 50, 200, 200);
        std::vector<CHAR> data;
        Ex_ReadFile(L"./res/niu1.svg", &data);
        _canvas_drawsvg(wParam, data.data(), ExARGB(55, 250, 20, 255), 250, 50, 400, 200);


        *lpResult = 1;
        return 1;
    }
    return 0;
}

void test_svgAndfont(HWND hWnd)
{
    HWND hWnd_svgAndfont =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"自定义字体和SVG测试", 0, 0, 500, 600, 0, 0);
    HEXDUI hExDui_svgAndfont = Ex_DUIBindWindowEx(
        hWnd_svgAndfont, 0,
        WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
            WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE,
        0, OnSvgAndFontProc);
    Ex_DUISetLong(hExDui_svgAndfont, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    auto              obj = Ex_ObjCreate(L"static", NULL, -1, 50, 250, 150, 150, hExDui_svgAndfont);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res\\niu1.svg", &imgdata);
    Ex_ObjSetBackgroundImageFromSvgBuf(obj, imgdata.data(), ExARGB(55, 0, 250, 255), 0, 0,
                                       BACKGROUND_REPEAT_ZOOM, 0, 0, 255, TRUE);
    Ex_DUIShowWindow(hExDui_svgAndfont, SW_SHOWNORMAL, 0, 0, 0);
}
