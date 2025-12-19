#include "test_effect.h"

LRESULT CALLBACK OnEffectObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                    LRESULT* lpResult)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjInitPropList(hObj, 3);
        // 注册效果器
        std::wstring      pProperityXml;
        std::vector<CHAR> data1;
        Ex_ReadFile(L"res/effect/effect.xml", &data1);
        pProperityXml = Ex_U2W2(data1);

        EX_EFFECT_PROPERITY_INFO bindings[] = {
            D2D1_VALUE_TYPE_BINDING(L"Color", &MyEffect::SetColor, &MyEffect::GetColor),
            D2D1_VALUE_TYPE_BINDING(L"Time", &MyEffect::SetTime, &MyEffect::GetTime),
            D2D1_VALUE_TYPE_BINDING(L"Resolution", &MyEffect::SetResolution,
                                    &MyEffect::GetResolution)};

        _effect_register(CLSID_MYEFFECT, pProperityXml.c_str(), bindings, ARRAYSIZE(bindings),
                         MyEffect::CreateEffect);
        HEXEFFECT pEffect = nullptr;
        auto      ret     = _effect_create(CLSID_MYEFFECT, &pEffect);

        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        LARGE_INTEGER start;
        QueryPerformanceCounter(&start);
        auto nTimeDeta = start.QuadPart / freq.QuadPart;
        RECT rc;
        Ex_ObjGetRect(hObj, &rc);
        int width  = (rc.right - rc.left);
        int height = (rc.bottom - rc.top);
        // 创建临时图像保存绘制内容
        _effect_create_buffer(pEffect, width, height);
        _effect_set_vector2(pEffect, L"Resolution", width, height);

        Ex_ObjSetProp(hObj, 0, (size_t)pEffect);
        Ex_ObjSetProp(hObj, 1, (size_t)nTimeDeta);
    }
    else if (uMsg == WM_PAINT) {
        HEXEFFECT pEffect   = (HEXEFFECT)Ex_ObjGetProp(hObj, 0);
        ULONGLONG nTimeDeta = Ex_ObjGetProp(hObj, 1);
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        LARGE_INTEGER start;
        QueryPerformanceCounter(&start);
        auto  time2      = (FLOAT)start.QuadPart / freq.QuadPart;
        float timeoffset = time2 - nTimeDeta;

        auto ret = _effect_set_float(pEffect, L"Time", timeoffset);
        // 呈现到组件
        EX_PAINTSTRUCT ps{0};
        Ex_ObjBeginPaint(hObj, &ps);
        _canvas_applyeffect(ps.hCanvas, pEffect);
        Ex_ObjEndPaint(hObj, &ps);
        return 1;
    }
    else if (uMsg == WM_TIMER) {
        //Ex_ObjInvalidateRect(hObj, 0);
        Ex_ObjUpdate(hObj);
    }
    else if (uMsg == WM_DESTROY) {
        Ex_ObjKillTimer(hObj);
        HEXEFFECT pEffect = (HEXEFFECT)Ex_ObjGetProp(hObj, 0);
        _effect_destroy(pEffect);
    }
    return 0;
}

void test_effect(HWND hWnd)
{
    HWND hWnd_effect = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试效果器", 0, 0, 500, 360, 0, 0);
    auto hExDui_effect = Ex_DUIBindWindowEx(hWnd_effect, 0,
                                            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
                                                WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE |
                                                WINDOW_STYLE_TITLE,
                                            0, 0);
    Ex_DUISetLong(hExDui_effect, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    auto hObj = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"static", L"", OBJECT_STYLE_VISIBLE, 50, 50, 400, 260,
                               hExDui_effect, 0, -1, 0, 0, OnEffectObjMsgProc);
    Ex_ObjSetTimer(hObj, 10);
    Ex_DUIShowWindow(hExDui_effect, SW_SHOWNORMAL);
}
