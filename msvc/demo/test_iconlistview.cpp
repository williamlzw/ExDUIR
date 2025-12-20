#include "test_iconlistview.h"

HEXOBJ       m_hListViewIcon = 0;
HEXIMAGELIST m_hImageListIcon = 0;

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_SIZE) {
        auto dpix = Ex_DUIGetSystemDpi();
        if (m_hListViewIcon != 0)
        {
            // 因为LOWORD(lParam)是DPI缩放后的窗口坐标,而Ex_ObjMove接受缩放前坐标，因此这里需要除以dpix
            Ex_ObjMove(m_hListViewIcon, 25, 50, (LOWORD(lParam) - 50) / dpix,
                (HIWORD(lParam) - 75) / dpix, TRUE);
        }

    }
    else if (uMsg == WM_DESTROY)
    {
        m_hListViewIcon = 0;
        m_hImageListIcon = 0;
    }
    return 0;
}

void test_iconlistview(HWND hWnd)
{
    HWND hWnd_iconlistview =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试图标列表", 0, 0, 500, 300, 0, 0);
    HEXDUI hExDui_iconlistview = Ex_DUIBindWindowEx(
        hWnd_iconlistview, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
        WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
        WINDOW_STYLE_HASICON,
        0, OnIconWndMsgProc);

    m_hListViewIcon = Ex_ObjCreateEx(-1, L"iconlistview", L"iconlistview",
        OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL |
        OBJECT_STYLE_VSCROLL | ICONLISTVIEW_STYLE_BUTTON,
        25, 50, 400, 225, hExDui_iconlistview, 0, -1, 0, 0, NULL);

    Ex_ObjSendMessage(m_hListViewIcon, ICONLISTVIEW_MESSAGE_SETITEMSIZE, 0,
        MAKELONG(70, 75));   // 设置表项尺寸为70,75
    // 创建添加图片组信息

    m_hImageListIcon = _imglist_create(36, 36);
    std::vector<CHAR> imgdata;
    for (INT i = 1; i <= 3; i++) {
        auto str = L"./res/icon/" + std::to_wstring(i) + L".png";
        Ex_ReadFile(str.c_str(), &imgdata);
        _imglist_add(m_hImageListIcon, 0, imgdata.data(), imgdata.size());
    }
    // 设置列表的图片组
    Ex_ObjSendMessage(m_hListViewIcon, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (size_t)m_hImageListIcon);
    EX_ICONLISTVIEW_ITEMINFO ilvi{ 0 };

    for (INT i = 1; i <= 1000; i++) {
        ilvi.nIndex = i;
        auto str = L"第" + std::to_wstring(i) + L"项";
        ilvi.pwzText = str.c_str();
        ilvi.nImageIndex = i % 3;
        if (ilvi.nImageIndex == 0) {
            ilvi.nImageIndex = 3;
        }
        Ex_ObjSendMessage(m_hListViewIcon, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&ilvi);
    }
    Ex_ObjSendMessage(m_hListViewIcon, LISTVIEW_MESSAGE_UPDATE, 0, 0);

    Ex_DUISetLong(hExDui_iconlistview, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    Ex_DUIShowWindow(hExDui_iconlistview, SW_SHOWNORMAL);
}
