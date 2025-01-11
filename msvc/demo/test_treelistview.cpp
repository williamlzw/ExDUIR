#include "test_treelistview.h"


void test_treelistview(HWND hWnd)
{
    HWND hWnd_treelistview =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试树形列表", 0, 0, 350, 350, 0, 0);
    HEXDUI hExDui_treelistview = Ex_DUIBindWindowEx(
        hWnd_treelistview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_treelistview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj_treeview =
        Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE, L"treeview", L"test_treeview",
                       OBJECT_STYLE_BORDER | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL |
                           OBJECT_STYLE_VSCROLL | TREEVIEW_STYLE_SHOWADDANDSUB,
                       50, 50, 250, 250, hExDui_treelistview, 0, DT_VCENTER, 0, 0, NULL);
    auto              m_hImageListIcon = _imglist_create(30, 30);
    std::vector<CHAR> imgdata;
    for (INT i = 3; i <= 5; i++) {
        auto str = L"./buttonex/" + std::to_wstring(i) + L".png";
        Ex_ReadFile(str.c_str(), &imgdata);
        _imglist_add(m_hImageListIcon, 0, imgdata.data(), imgdata.size());
    }
    // 设置列表的图片组,wParam可以重置表项高度为图标高度
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_SETIMAGELIST, 1, (size_t)m_hImageListIcon);

    EX_TREEVIEW_INSERTINFO ti = {0};

    Ex_ObjSetColor(hObj_treeview, COLOR_EX_BACKGROUND, ExARGB(255, 255, 255, 125), FALSE);
    Ex_ObjSetColor(hObj_treeview, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), TRUE);
    ti.fExpand           = TRUE;
    ti.pwzText           = L"节点1";
    ti.nImageIndexExpand = 2;   // 无子节点为展开节点
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点2";

    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText           = L"节点3";
    ti.nImageIndexExpand = 1;   // 演示展开图标
    ti.nImageIndex       = 3;   // 演示收缩图标
    ti.itemParent        = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.nImageIndexExpand = 0;   // 重置图标
    ti.nImageIndex       = 0;   // 重置图标
    ti.pwzText           = L"节点3-1";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点3-2";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点3-3";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点3-3-1";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点3-3-2";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.itemParent = 0;
    ti.pwzText    = L"节点4";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点5-1";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点5-2-1";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2-2";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText = L"节点5-2-2-1";
    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2-2-2";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2-2-2-1";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2-2-2-1-1";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    ti.pwzText    = L"节点5-2-2-2-1-1-1";
    ti.itemParent = (EX_TREEVIEW_NODEITEM*)Ex_ObjSendMessage(
        hObj_treeview, TREEVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ti);

    Ex_ObjSendMessage(hObj_treeview, TREEVIEW_MESSAGE_UPDATE, 0, 0);

    Ex_DUIShowWindow(hExDui_treelistview, SW_SHOWNORMAL, 0, 0, 0);
}
