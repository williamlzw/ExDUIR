#include "test_listbutton.h"

LRESULT CALLBACK OnListButtonMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                             LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_EX_LCLICK) {
        auto id = Ex_ObjGetLong(hObj, OBJECT_LONG_ID);
        OUTPUTW(L"菜单项目点击,id:", id);
    }
    return 0;
}

LRESULT CALLBACK OnListButtonWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_INITMENUPOPUP) {
        // 响应菜单项目事件
        HEXOBJ hObjfind = Ex_ObjFind(hExDui, 0, L"Item", 0);
        while (hObjfind != 0) {
            Ex_ObjSetLong(hObjfind, OBJECT_LONG_OBJPROC, (size_t)OnListButtonMenuItemMsgProc);
            hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
        }
    }
    else if (uMsg == WM_NOTIFY) {
        EX_NMHDR notify{0};
        RtlMoveMemory(&notify, (LPVOID)lParam, sizeof(EX_NMHDR));
        if (notify.nCode == NM_CREATE) {
            Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_TEXT_NORMAL, ExARGB(210, 120, 55, 255),
                           TRUE);   // 改变菜单项目字体正常颜色
            Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16711680, 255),
                           TRUE);   // 改变菜单项目字体悬浮颜色
            Ex_ObjSetColor(notify.hObjFrom, COLOR_EX_BACKGROUND, ExARGB(110, 120, 55, 255),
                           TRUE);   // 改变菜单项目背景颜色
        }
    }
    else if (uMsg == MENU_MESSAGE_SELECTITEM && (DWORD)wParam == -1)   // 恢复正常状态
    {
        POINT point = {0};
        GetCursorPos(&point);
        HWND currentWnd = WindowFromPoint(point);
        ScreenToClient(currentWnd, &point);
        HEXOBJ hObj = Ex_DUIGetObjFromPoint((EXHANDLE)currentWnd, point.x, point.y);
        Ex_ObjPostMessage(hObj, LISTBUTTON_MESSAGE_SELECTITEM, 0, 0);
    }
    return 0;
}

LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                     LRESULT* lpResult)
{
    if (uMsg == LISTBUTTON_MESSAGE_DOWNITEM) {
        RECT rcWindow{0};
        RECT rcObj{0};
        GetWindowRect(hWnd, &rcWindow);
        Ex_ObjGetRectEx(hObj, &rcObj, 2);
        Ex_TrackPopupMenu((HMENU)lParam, TPM_RECURSE, rcWindow.left + rcObj.left + wParam,
                          rcWindow.top + Ex_Scale(rcObj.bottom), 0, hObj, NULL,
                          OnListButtonWndMsgProc, MENU_FLAG_NOSHADOW);
        *lpResult = 1;
        return 1;
    }
    return 0;
}

LRESULT CALLBACK OnListButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == LISTBUTTON_EVENT_CLICK) {
        OUTPUTW(L"点击", wParam, lParam);
    }
    else if (nCode == LISTBUTTON_EVENT_CHECK) {
        OUTPUTW(L"选择", wParam, lParam);
    }
    return 0;
}

void test_listbutton(HWND hWnd)
{
    HWND hWnd_listbutton =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表按钮", 0, 0, 480, 200, 0, 0);
    HEXDUI hExDui_listbutton = Ex_DUIBindWindowEx(
        hWnd_listbutton, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN |
            WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW,
        0, 0);
    Ex_DUISetLong(hExDui_listbutton, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    HMENU hMenu = LoadMenuW(GetModuleHandleW(0), (LPWSTR)IDR_MENU1);

    // 创建正常菜单条，显示Resource.rc资源文件里的菜单,若出现中文乱码将Resource.rc文件编码格式转为UCS-2
    // LE BOM
    HEXOBJ hObj = 0;
    hObj        = Ex_ObjCreate(L"Menubar", 0, -1, 0, 30, 220, 22, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);         // 文本色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE);   // 点燃背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);   // 按下背景色

    EX_LISTBUTTON_ITEMINFO item1 = {0};
    item1.wzText                 = L"文件(&F)";
    item1.nMenu                  = GetSubMenu(hMenu, 0);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"编辑(&E)";
    item1.nMenu  = GetSubMenu(hMenu, 1);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"选项(&O)";
    item1.nMenu  = GetSubMenu(hMenu, 2);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"帮助(&H)";
    item1.nMenu  = GetSubMenu(hMenu, 3);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item1);

    // 创建自定义回调菜单条

    hObj = Ex_ObjCreateEx(-1, L"Menubar", 0, -1, 0, 60, 220, 22, hExDui_listbutton, 0, -1, 0, 0,
                          OnListButtonMsgProc);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(110, 120, 55, 255),
                   FALSE);   // 改变菜单按钮背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255),
                   FALSE);   // 改变菜单按钮字体正常色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExARGB(255, 255, 255, 55),
                   FALSE);   // 改变菜单按钮字体热点色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExARGB(255, 255, 255, 100),
                   FALSE);   // 改变菜单按钮字体按下色
    item1.wzText = L"文件(&F)";
    item1.nMenu  = GetSubMenu(hMenu, 0);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"编辑(&E)";
    item1.nMenu  = GetSubMenu(hMenu, 1);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"选项(&O)";
    item1.nMenu  = GetSubMenu(hMenu, 2);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"帮助(&H)";
    item1.nMenu  = GetSubMenu(hMenu, 3);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item1);

    // 创建工具条
    hObj = Ex_ObjCreate(L"Toolbar", 0, -1, 0, 90, 400, 22, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);         // 文本色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE);   // 点燃背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);   // 按下背景色
    HEXIMAGELIST      hImageList = _imglist_create(18, 18);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/buttonex/4.png", &imgdata);
    size_t nImageIndex = _imglist_add(hImageList, 0, imgdata.data(), imgdata.size());
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (LPARAM)hImageList);
    EX_LISTBUTTON_ITEMINFO item2 = {0};
    item2.nType                  = 1;
    item2.nImage                 = nImageIndex;
    item2.wzText                 = NULL;
    item2.wzTips                 = L"普通按钮1";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType  = 1;
    item2.nImage = 0;
    item2.wzText = L"普通按钮不带图标";
    item2.wzTips = L"普通按钮不带图标";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType  = 2;
    item2.nImage = nImageIndex;
    item2.wzText = NULL;
    item2.wzTips = L"选择按钮1";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType   = 2;
    item2.nImage  = 0;
    item2.wzText  = L"选择按钮不带图标";
    item2.wzTips  = L"选择按钮不带图标";
    item2.dwState = STATE_DOWN;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType   = 0;
    item2.nImage  = 0;
    item2.wzText  = NULL;
    item2.wzTips  = NULL;
    item2.dwState = STATE_NORMAL;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType   = 1;
    item2.nImage  = nImageIndex;
    item2.wzText  = L"禁用按钮带图标";
    item2.wzTips  = L"禁用按钮带图标";
    item2.dwState = STATE_DISABLE;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item2);
    Ex_ObjHandleEvent(hObj, LISTBUTTON_EVENT_CLICK, OnListButtonEvent);
    Ex_ObjHandleEvent(hObj, LISTBUTTON_EVENT_CHECK, OnListButtonEvent);

    // 创建状态条
    hObj = Ex_ObjCreate(L"Statusbar", 0, -1, 0, 120, 300, 22, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(12557930, 255), FALSE);     // 背景色
    Ex_ObjSetColor(hObj, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), FALSE);        // 分割色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), FALSE);   // 文本色
    EX_LISTBUTTON_ITEMINFO item3 = {0};
    item3.wzText                 = L"左对齐";
    item3.nWidth                 = 100;
    item3.TextFormat             = DT_LEFT;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item3);
    item3.wzText     = L"居中";
    item3.nWidth     = 100;
    item3.TextFormat = DT_CENTER;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item3);
    item3.wzText     = L"右对齐";
    item3.nWidth     = 100;
    item3.nIndex     = 2;
    item3.TextFormat = DT_RIGHT;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item3);

    // 设置菜单条目图标
    if (hMenu) {
        HMENU hMenu_sub = GetSubMenu(hMenu, 0);
        if (hMenu_sub) {
            MENUITEMINFOW minfo{0};
            minfo.cbSize = sizeof(MENUITEMINFOW);
            minfo.fMask  = MIIM_BITMAP;
            GetMenuItemInfoW(hMenu_sub, 1, TRUE, &minfo);
            if (minfo.hbmpItem) {
                DeleteObject(minfo.hbmpItem);
            }
            std::vector<CHAR> imgdata;
            HBITMAP           hbitmap   = 0;
            HEXIMAGE          hImg      = 0;
            HEXIMAGE          hImgSmall = 0;
            _img_createfromfile(L"res/rotateimgbox.jpg", &hImg);
            _img_scale(hImg, 24, 24, &hImgSmall);   // 注意菜单条目高度跟图像高度有关，因此缩放到24
            imgdata.resize(24 * 24 * 4);
            _img_savetomemory(hImgSmall, imgdata.data());
            Ex_LoadBitMapFromMemory(imgdata.data(), imgdata.size(), &hbitmap);
            _img_destroy(hImg);
            _img_destroy(hImgSmall);
            minfo.hbmpItem = hbitmap;
            SetMenuItemInfoW(hMenu_sub, 1, TRUE, &minfo);
        }
    }
    Ex_DUIShowWindow(hExDui_listbutton, SW_SHOWNORMAL, 0, 0, 0);
}
