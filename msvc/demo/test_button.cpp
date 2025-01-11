#include "test_button.h"

HEXDUI m_hExDuiButton;

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nID == 201)   // 通过组件ID判断按钮
    {
        Ex_ObjEnable(hObj, FALSE);   // 禁用自身
        Ex_ObjSetPadding(hObj, 0, 20, 5, 5, 5, TRUE);
    }
    else if (nID == 202) {
        Ex_ObjEnable(Ex_ObjGetFromID(m_hExDuiButton, 201),
                     TRUE);   // 通过组件ID取按钮句柄，解除按钮1禁用
    }
    else if (nID == 203) {
        Ex_ObjSetText(hObj, L"自身文本被改动", TRUE);   // 改动自身文本
    }
    else if (nID == 204) {
        auto text_length =
            Ex_ObjGetTextLength(Ex_ObjGetFromID(m_hExDuiButton, 201));   // 取按钮1文本长度
        std::wstring str;
        str.resize(text_length);
        Ex_ObjGetText(Ex_ObjGetFromID(m_hExDuiButton, 201), str.data(), text_length * 2);
        Ex_ObjSetText(hObj, (L"按钮1文本:" + str).c_str(), TRUE);
    }
    else if (nID == 205) {
        if (nCode == NM_CUSTOMDRAW) {
            EX_PAINTSTRUCT ps{0};
            RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
            EXARGB crBkg = 0;
            if ((ps.dwState & STATE_DOWN) != 0) {
                crBkg = ExRGB2ARGB(255, 51);
            }
            else if ((ps.dwState & STATE_HOVER) != 0) {
                crBkg = ExRGB2ARGB(16754943, 51);
            }
            else {
                crBkg = ExRGB2ARGB(16777215, 51);
            }
            HEXBRUSH hBrush = _brush_create(crBkg);
            if (hBrush) {
                _canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
                _brush_destroy(hBrush);
            }
        }
    }
    else if (nID == 206) {
        if (wParam != 0) {
            Ex_MessageBox(hObj, L"开启", L"取开关状态", MB_USERICON, MESSAGEBOX_FLAG_CENTEWINDOW);
        }
        else {
            Ex_MessageBox(hObj, L"关闭", L"取开关状态", MB_ICONWARNING,
                          MESSAGEBOX_FLAG_CENTEWINDOW);
        }
    }
    return 0;
}

LRESULT CALLBACK OnButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND)   // 组件WM_ERASEBKGND消息wParam是hCanvas,lParam是EX_PAINTSTRUCT
    {
        EX_PAINTSTRUCT ps{0};
        RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
        EXARGB crBkg = 0;
        if ((ps.dwState & STATE_DOWN) != 0) {
            crBkg = ExRGB2ARGB(255, 51);
        }
        else if ((ps.dwState & STATE_HOVER) != 0) {
            crBkg = ExRGB2ARGB(16754943, 51);
        }
        else {
            crBkg = ExRGB2ARGB(16777215, 51);
        }
        HEXBRUSH hBrush = _brush_create(crBkg);
        if (hBrush) {
            _canvas_fillrect(ps.hCanvas, hBrush, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom);
            _brush_destroy(hBrush);
        }
        *lpResult = 1;
        return 1;
    }
    return 0;
}

void test_button(HWND hWnd)
{
    HWND hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试按钮开关", 0, 0, 300, 200, 0, 0);
    m_hExDuiButton = Ex_DUIBindWindowEx(hWnd_button, 0,
                                        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                                            WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
                                            WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
                                            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
                                        0, 0);
    Ex_DUISetLong(m_hExDuiButton, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    std::vector<HEXOBJ> buttons;
    buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"禁用自身", -1, 10, 30, 120, 30,
                                     m_hExDuiButton, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL));
    buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"解除按钮1禁用", -1, 10, 70, 120, 30,
                                     m_hExDuiButton, 202, DT_VCENTER | DT_CENTER, 0, 0, NULL));
    buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"改动自身文本", -1, 10, 110, 120, 30,
                                     m_hExDuiButton, 203, DT_VCENTER | DT_CENTER, 0, 0, NULL));
    buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"取按钮1文本", -1, 10, 150, 120, 30,
                                     m_hExDuiButton, 204, DT_VCENTER | DT_CENTER, 0, 0, NULL));

    for (auto button : buttons) {
        Ex_ObjHandleEvent(button, NM_CLICK, OnButtonEvent);
    }

    HEXOBJ hobj_customdrawbutton = Ex_ObjCreateEx(
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED,
        L"button", L"重画按钮1", -1, 150, 30, 120, 30, m_hExDuiButton, 205, DT_VCENTER | DT_CENTER,
        0, 0, NULL);
    Ex_ObjHandleEvent(
        hobj_customdrawbutton, NM_CUSTOMDRAW,
        OnButtonEvent);   // 第一种重画背景方式,全部自带组件都可以采用这样的方式重画,注意带上扩展风格

    Ex_ObjCreateEx(
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED,
        L"button", L"重画按钮2", -1, 150, 70, 120, 30, m_hExDuiButton, 0, DT_VCENTER | DT_CENTER, 0,
        0,
        OnButtonMsgProc);   // 第二种重画背景方式,全部自带组件都可以采用这样的方式重画,注意带上扩展风格

    HEXOBJ hObj_switch =
        Ex_ObjCreate(L"Switch", L"已开启|已关闭", -1, 150, 110, 80, 30, m_hExDuiButton);

    HEXOBJ hObj_switch2 =
        Ex_ObjCreateEx(-1, L"Switch", 0, -1, 150, 150, 60, 30, m_hExDuiButton, 206, -1, 0, 0, 0);
    Ex_ObjSendMessage(hObj_switch2, BM_SETCHECK, 1, 0);   // 设置选中状态
    Ex_ObjHandleEvent(hObj_switch2, NM_CHECK, OnButtonEvent);
    EX_OBJ_PROPS switchprops       = {0};
    switchprops.crBkgNormal        = ExARGB(255, 255, 255, 100);
    switchprops.crBkgDownOrChecked = ExARGB(200, 50, 100, 0);
    switchprops.crBorderNormal     = ExARGB(255, 255, 255, 255);
    switchprops.radius             = 15;
    switchprops.strokeWidth        = 1;
    Ex_ObjSendMessage(hObj_switch2, WM_EX_PROPS, 0, (LPARAM)&switchprops);

    Ex_DUIShowWindow(m_hExDuiButton, SW_SHOWNORMAL, 0, 0, 0);
}
