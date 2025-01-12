#include "test_editex.h"


LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == EN_CHANGE) {
        OUTPUTW(L"编辑框内容改变", nID);
    }
    else if (nCode == NM_CHAR) {
        OUTPUTW(L"编辑框输入字符", nID, wParam);
    }
    return 0;
}

void test_editex(HWND hWnd)
{
    HWND hWnd_editex = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试扩展编辑框", 0, 0, 400, 200, 0, 0);
    HEXDUI hExDui_editex = Ex_DUIBindWindowEx(
        hWnd_editex, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_editex, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXIMAGE     hImage;
    EX_OBJ_PROPS EditExprops = {0};
    HEXOBJ hObj_editex1 = Ex_ObjCreate(L"EditEx", NULL, OBJECT_STYLE_VISIBLE | EDIT_STYLE_UNDERLINE,
                                       30, 50, 150, 30, hExDui_editex);
    Ex_ObjSendMessage(hObj_editex1, EDIT_MESSAGE_SETCUEBANNER, ExARGB(255, 255, 255, 100),
                      (LPARAM)L"请输入账户名称");
    Ex_ObjSetColor(hObj_editex1, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
    _img_createfromfile(L"res\\editex\\icon_contacts_normal.png", &hImage);
    Ex_ObjSendMessage(hObj_editex1, WM_SETICON, 0, (LPARAM)hImage);

    HEXOBJ hObj_editex2 = Ex_ObjCreate(
        L"EditEx", NULL, OBJECT_STYLE_VISIBLE | EDIT_STYLE_UNDERLINE | EDIT_STYLE_USEPASSWORD, 30,
        100, 150, 30, hExDui_editex);
    Ex_ObjSendMessage(hObj_editex2, EDIT_MESSAGE_SETCUEBANNER, ExARGB(255, 255, 255, 100),
                      (LPARAM)L"请输入账户密码");
    Ex_ObjSetColor(hObj_editex2, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
    _img_createfromfile(L"res\\editex\\psw_normal.png", &hImage);
    Ex_ObjSendMessage(hObj_editex2, WM_SETICON, 0, (LPARAM)hImage);

    HEXOBJ hObj_editex3 = Ex_ObjCreate(L"EditEx", NULL, -1, 200, 50, 150, 30, hExDui_editex);
    Ex_ObjSendMessage(hObj_editex3, EDIT_MESSAGE_SETCUEBANNER, ExARGB(255, 255, 255, 100),
                      (LPARAM)L"搜索一下");
    Ex_ObjSetColor(hObj_editex3, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);
    EditExprops.crBkgNormal           = ExARGB(20, 20, 120, 150);
    EditExprops.crBorderHover         = ExARGB(255, 77, 77, 150);
    EditExprops.crBorderDownOrChecked = ExARGB(255, 77, 77, 200);
    EditExprops.radius                = 15;
    EditExprops.strokeWidth           = 1;
    EditExprops.nIconPosition         = 1;
    Ex_ObjSendMessage(hObj_editex3, WM_EX_PROPS, 0, (LPARAM)&EditExprops);
    _img_createfromfile(L"res\\editex\\search_normal.png", &hImage);
    Ex_ObjSendMessage(hObj_editex3, WM_SETICON, 0, (LPARAM)hImage);

    HEXOBJ hObj_editex4 = Ex_ObjCreate(L"EditEx", NULL, -1, 200, 100, 150, 30, hExDui_editex);
    Ex_ObjSendMessage(hObj_editex4, EDIT_MESSAGE_SETCUEBANNER, ExARGB(255, 255, 255, 100),
                      (LPARAM)L"搜索一下");
    _img_createfromfile(L"res\\editex\\search_normal.png", &hImage);
    Ex_ObjSendMessage(hObj_editex4, WM_SETICON, 0, (LPARAM)hImage); /* 设置图标; */
    Ex_ObjSetColor(hObj_editex4, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 200), FALSE);

    Ex_ObjHandleEvent(hObj_editex4, EN_CHANGE, OnEditChangeEvent);
    Ex_ObjHandleEvent(hObj_editex4, NM_CHAR, OnEditChangeEvent);

    Ex_DUIShowWindow(hExDui_editex, SW_SHOWNORMAL, 0, 0, 0);
}
