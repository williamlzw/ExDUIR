#include "test_svg_and_font.h"

LRESULT CALLBACK OnSvgAndFontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        _canvas_clear(wParam, ExARGB(150, 150, 150, 255));
        HEXFONT hFont = _font_createfromfile(L"res/fonts/文道灵飞小楷.ttf", 48);
        _canvas_drawtext(wParam, hFont, ExARGB(200, 0, 200, 200), L"我是测试文本", -1, DT_SELECTABLE, 20, 150,
            600, 350);
        _font_destroy(hFont);

        hFont = _font_createfromfile(L"res/fonts/Aa双鱼座.ttf", 48);
        _canvas_drawtext(wParam, hFont, ExARGB(200, 200, 200, 200),
            L"我是测试文本", -1, DT_SELECTABLE, 20, 350, 600, 550);
        _font_destroy(hFont);

        std::vector<CHAR> fontdata;
        Ex_ReadFile(L"res/fonts/ColorToolbarIcons.ttf", &fontdata);
        HEXFONT ColorToolbarIcons = _font_createfrommem(
            (const uint8_t*)fontdata.data(), fontdata.size(), 48);
        _canvas_drawtext(wParam, ColorToolbarIcons, ExARGB(0, 0, 0, 255),
            L"\uE000 \uE001 \uE002 \uE003 \uE004 \uE005 \uE006 "
            L"\uE007 \uE008 \uE009 \uE00A \uE00B \uE00C \uE00D",
            -1, -1, 20, 550, LOWORD(lParam), 680);
        _font_destroy(ColorToolbarIcons);

        HEXSVG svg = 0;
        _svg_createfromfile(L"res/微信.svg", &svg);
        //_svg_setfillcolor(svg, ExARGB(255, 255, 255, 255));
        _svg_setelementfillcolor(svg, L"1208", ExARGB(255, 0, 255, 255));
        _canvas_drawsvg(wParam, svg, 50, 50, 100, 100);
        _svg_destroy(svg);

        _svg_createfromfile(L"res/微博.svg", &svg);
        //_svg_setfillcolor(svg, ExARGB(0, 255, 255, 255));
        _svg_setelementfillcolor(svg, L"1372", ExARGB(0, 20, 255, 255));
        _canvas_drawsvg(wParam, svg, 200, 50, 100, 100);
        _svg_destroy(svg);

        _svg_createfromfile(L"res/QQ.svg", &svg);
        //_svg_setfillcolor(svg, ExARGB(255, 0, 255, 255));
        _canvas_drawsvg(wParam, svg, 350, 50, 100, 100);
        _svg_destroy(svg);

        *lpResult = 1;
        return 1;
    }
    else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_MOUSEMOVE || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK)
    {
        // 获取画布关联的窗口，拿到画布句柄 (需根据你的实际框架情况获取hCanvas，这里假设通过某种方式拿到)
        HEXCANVAS hCanvas = Ex_DUIGetLong(hExDui, ENGINE_LONG_HCANVAS);

        FLOAT mouseX = (FLOAT)GET_X_LPARAM(lParam);
        FLOAT mouseY = (FLOAT)GET_Y_LPARAM(lParam);

        _canvas_handle_mouse_event_for_text(hCanvas, uMsg, mouseX, mouseY);
        *lpResult = 0;
        return 0;
    }
    else if (uMsg == WM_KEYDOWN)
    {// 检测 Ctrl+C (虚拟键码 'C' 是 0x43)
        if (wParam == 0x43 && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
        {
            HEXCANVAS hCanvas = Ex_DUIGetLong(hExDui, ENGINE_LONG_HCANVAS);
            if (_canvas_copy_selected_text(hCanvas))
            {
                // 复制成功，可以选择给个提示或阻止进一步传递
                *lpResult = 1;
                return 1;
            }
        }
        
    }
    return 0;
}

void test_svgAndfont(HWND hWnd)
{
    HWND hWnd_svgAndfont =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"自定义字体和SVG测试", 0, 0, 800, 600, 0, 0);
    HEXDUI hExDui_svgAndfont = Ex_DUIBindWindowEx(
        hWnd_svgAndfont, 0,
        WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW |
        WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE,
        0, OnSvgAndFontProc);
    Ex_DUISetLong(hExDui_svgAndfont, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    auto              obj = Ex_ObjCreate(L"static", NULL, -1, 450, 250, 150, 150, hExDui_svgAndfont);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res\\niu.svg", &imgdata);
    std::string svg = std::string(imgdata.begin(), imgdata.end());
    HEXSVG hSvg;
    auto ret = _svg_create(svg.data(), &hSvg);
    Ex_ObjSetBackgroundImageFromSvg(obj, hSvg, 0, 0, BACKGROUND_REPEAT_ZOOM, 0, 0, 255, TRUE);
    _svg_destroy(hSvg);
    Ex_DUIShowWindow(hExDui_svgAndfont, SW_SHOWNORMAL);
}
