#include "test_sliderbar.h"


HEXOBJ m_hSliderBarLabel;

LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam)
{
    std::wstring text = L"滑块条,当前值是：" + std::to_wstring(lParam);
    Ex_ObjSetText(m_hSliderBarLabel, text.c_str(), TRUE);
    return 0;
}

void test_sliderbar(HWND hWnd)
{
    HWND hWnd_sliderbar = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试滑块条", 0, 0, 400, 200, 0, 0);
    HEXDUI hExDui_sliderbar = Ex_DUIBindWindowEx(
        hWnd_sliderbar, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_sliderbar, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    WCHAR  ATOM_sliderbarEx[] = L"sliderbar"; /*滑块条*/
    HEXOBJ hObj_sliderbarex1 =
        Ex_ObjCreate(ATOM_sliderbarEx, NULL, -1, 80, 50, 250, 20, hExDui_sliderbar);
    Ex_ObjSetColor(hObj_sliderbarex1, COLOR_EX_BACKGROUND, ExARGB(100, 36, 255, 250), TRUE);
    Ex_ObjHandleEvent(hObj_sliderbarex1, SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);

    HEXOBJ hObj_sliderbarex2 =
        Ex_ObjCreate(ATOM_sliderbarEx, NULL, OBJECT_STYLE_VISIBLE | SLIDERBAR_STYLE_VERTICAL, 50,
                     60, 20, 100, hExDui_sliderbar);
    Ex_ObjSetColor(hObj_sliderbarex2, COLOR_EX_BACKGROUND, ExARGB(100, 236, 255, 250), TRUE);
    Ex_ObjHandleEvent(hObj_sliderbarex2, SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
    Ex_ObjSetLong(hObj_sliderbarex2, SLIDERBAR_LONG_BLOCK_DIRECTION, 1);

    HEXOBJ hObj_sliderbarex3 =
        Ex_ObjCreate(ATOM_sliderbarEx, NULL, OBJECT_STYLE_VISIBLE | SLIDERBAR_STYLE_VERTICAL, 350,
                     60, 20, 100, hExDui_sliderbar);
    Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_BACKGROUND, ExARGB(100, 136, 255, 250), TRUE);
    Ex_ObjHandleEvent(hObj_sliderbarex3, SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
    Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 125),
                   TRUE);   // 滑块条底色
    Ex_ObjSetColor(hObj_sliderbarex3, COLOR_EX_TEXT_CHECKED, ExARGB(200, 16, 25, 250),
                   TRUE);   // 滑块条走过的颜色

    HEXOBJ hObj_sliderbarex4 =
        Ex_ObjCreate(ATOM_sliderbarEx, NULL, -1, 80, 150, 250, 20, hExDui_sliderbar);
    Ex_ObjSetColor(hObj_sliderbarex4, COLOR_EX_BACKGROUND, ExARGB(0, 136, 255, 250), TRUE);
    Ex_ObjHandleEvent(hObj_sliderbarex4, SLIDERBAR_EVENT_VALUE, OnSliderBarPosChangeEvent);
    Ex_ObjSetLong(hObj_sliderbarex4, SLIDERBAR_LONG_BLOCK_DIRECTION, 1);
    m_hSliderBarLabel =
        Ex_ObjCreate(L"static", L"滑块条,当前值是：", -1, 80, 80, 300, 30, hExDui_sliderbar);
    Ex_DUIShowWindow(hExDui_sliderbar, SW_SHOWNORMAL);
}
