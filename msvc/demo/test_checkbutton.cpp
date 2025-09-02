#include "test_checkbutton.h"


LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                           LPARAM lParam)
{
    if (wParam != 0) {
        BOOL fChecked = TRUE;   // 弹出的信息框里面复选框是选中状态
        Ex_MessageBoxEx(hObj, L"选中啦", L"取CheckButton状态", MB_USERICON, L"不再提示", &fChecked,
                        2000, MESSAGEBOX_FLAG_CENTEWINDOW, 0);
    }
    else {
        Ex_MessageBox(hObj, L"取消选中啦", L"取CheckButton状态", MB_ICONWARNING,
                      MESSAGEBOX_FLAG_CENTEWINDOW);
    }
    return 0;
}

void test_checkbutton(HWND hWnd)
{
    HWND hWnd_checkbutton =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试单选框复选框", 0, 0, 300, 230, 0, 0);
    HEXDUI hExDui_checkbutton = Ex_DUIBindWindowEx(
        hWnd_checkbutton, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_checkbutton, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));
  
    auto hobj = Ex_ObjCreateEx(-1, L"checkbutton", L"复选框", -1, 10, 30, 60, 20, hExDui_checkbutton, 0,
                   DT_VCENTER, 0, 0, NULL);
    Ex_ObjSetLong(hobj, CHECKBOXEX_LONG_STATE, 2);
    Ex_ObjCreateEx(-1, L"radiobutton", L"单选框1", -1, 10, 60, 80, 20, hExDui_checkbutton, 0,
                   DT_VCENTER, 0, 0, NULL);
    Ex_ObjCreateEx(-1, L"radiobutton", L"单选框2", -1, 100, 60, 80, 20, hExDui_checkbutton, 0,
                   DT_VCENTER, 0, 0, NULL);

    EX_OBJ_PROPS CheckButtonExProps = {0};
    HEXOBJ       hObj_checkbutton1 =
        Ex_ObjCreate(L"CheckButtonEx", L"扩展复选框1", -1, 10, 90, 120, 30, hExDui_checkbutton);
    Ex_ObjSetColor(hObj_checkbutton1, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), TRUE);
    CheckButtonExProps.crBkgDownOrChecked    = ExARGB(215, 117, 23, 250);
    CheckButtonExProps.crBorderNormal        = ExARGB(222, 222, 222, 150);
    CheckButtonExProps.crBorderHover         = ExARGB(222, 222, 222, 200);
    CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 222, 222, 250);
    Ex_ObjSendMessage(hObj_checkbutton1, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
    Ex_ObjSetLong(hObj_checkbutton1, CHECKBOXEX_LONG_STATE, 2);

    HEXOBJ hObj_checkbutton2 =
        Ex_ObjCreate(L"CheckButtonEx", L"扩展复选框2", -1, 10, 120, 120, 30, hExDui_checkbutton);
    CheckButtonExProps.crBkgDownOrChecked    = ExARGB(251, 171, 213, 250);
    CheckButtonExProps.crBorderNormal        = ExARGB(22, 222, 222, 150);
    CheckButtonExProps.crBorderHover         = ExARGB(22, 222, 222, 200);
    CheckButtonExProps.crBorderDownOrChecked = ExARGB(222, 22, 222, 250);
    Ex_ObjSendMessage(hObj_checkbutton2, WM_EX_PROPS, 0, (LPARAM)&CheckButtonExProps);
    Ex_ObjHandleEvent(hObj_checkbutton2, NM_CHECK, OnCheckButtonCheckedEvent);

    Ex_ObjCreate(L"RadioButtonEx", L"扩展单选框", -1, 150, 90, 120, 30, hExDui_checkbutton);
    HEXOBJ hObj_radiobutton2 =
        Ex_ObjCreate(L"RadioButtonEx", L"扩展单选框2", -1, 150, 120, 120, 30, hExDui_checkbutton);
    Ex_ObjSendMessage(hObj_radiobutton2, BM_SETCHECK, 1, 0);

    HEXOBJ hObj_checkbox1 =
        Ex_ObjCreate(L"CheckBox", L"记住账号", -1, 10, 170, 100, 30, hExDui_checkbutton);
    Ex_ObjHandleEvent(hObj_checkbox1, NM_CHECK, OnCheckButtonCheckedEvent);
    Ex_ObjSetProp(hObj_checkbox1, CHECKBOX_PROP_TEXT, ExARGB(120, 40, 160, 255));
    Ex_ObjSetProp(hObj_checkbox1, CHECKBOX_PROP_CRHOVERCHECK, ExARGB(66, 66, 66, 255));
    EX_OBJ_PROPS prop;
    prop.crBkgNormal        = ExARGB(120, 80, 55, 255);
    prop.crBkgHover         = ExARGB(150, 100, 80, 255);
    prop.crBkgDownOrChecked = ExARGB(250, 150, 180, 255);
    Ex_ObjSendMessage(hObj_checkbox1, WM_EX_PROPS, 0, (size_t)&prop);
    HEXOBJ hObj_checkbox2 =
        Ex_ObjCreate(L"CheckBox", L"记住密码", -1, 150, 170, 100, 30, hExDui_checkbutton);
    Ex_ObjSendMessage(hObj_checkbox2, BM_SETCHECK, 1, 0);

    Ex_DUIShowWindow(hExDui_checkbutton, SW_SHOWNORMAL, 0, 0, 0);
}
