#include "test_listbutton.h"

HEXMENU hMenu = 0;
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
        EX_NMHDR notify{ 0 };
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
        POINT point = { 0 };
        GetCursorPos(&point);
        HWND currentWnd = WindowFromPoint(point);
        ScreenToClient(currentWnd, &point);
        HEXOBJ hObj = Ex_DUIGetObjFromPoint((EXHANDLE)currentWnd, point.x, point.y);
        Ex_ObjPostMessage(hObj, LISTBUTTON_MESSAGE_SELECTITEM, 0, 0);
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
LRESULT CALLBACK OnListMenuMainWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == WM_DESTROY) {
        Ex_MenuDestroy(hMenu);
        HEXIMAGE hImg = Ex_DUIGetLong(hExDui, ENGINE_LONG_LPARAM);
        _img_destroy(hImg);
    }
    return 0;
}


LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
    LRESULT* lpResult)
{
    if (uMsg == LISTBUTTON_MESSAGE_DOWNITEM) {
        EX_LISTBUTTON_ITEMINFO* pTR = (EX_LISTBUTTON_ITEMINFO*)lParam;//wParam
        if (Ex_MenuIsMenu(pTR->nMenu))
        {
            RECT rcWindow{ 0 };
            RECT rcObj{ 0 };
            GetWindowRect(hWnd, &rcWindow);
            Ex_ObjGetRectEx(hObj, &rcObj, 2);
            INT Pos_x = rcWindow.left + rcObj.left + pTR->nLeft;
            Ex_TrackPopupMenu(pTR->nMenu, TPM_RECURSE, Pos_x,
                rcWindow.top + Ex_Scale(rcObj.bottom), 0, hObj, NULL);
        }
        *lpResult = 1;
        return 1;
    }
    return 0;
}
LRESULT CALLBACK button_clickpopmenu(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode == MN_CREATE)
    {
        INT      Index = Ex_ObjGetLong(hObj, 3);//LISTBUTTON_LONG_INDEX //列表按钮_热点项目
        EX_LISTBUTTON_ITEMINFO pItemInfo = { 0 };
        Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_GETITEM, Index, (size_t)&pItemInfo);
        if (pItemInfo.nMenu == lParam)
            Ex_DUISetLong(wParam, ENGINE_LONG_CRBKG, ExRGB2ARGB(0xFFD488, 155));//修改菜单背景色

    }
    else if (nCode == NM_COMMAND)
    {
        OUTPUTW(L"菜单项目点击,id:", wParam, L"菜单句柄:", lParam);
        UINT uState = Ex_MenuGetState(lParam, 0, MF_BYPOSITION);
        if (!(uState & MF_SEPARATOR || uState & MF_POPUP))
        {
            if ((uState & MF_CHECKED) == MF_CHECKED)
                Ex_MenuCheckItem(lParam, wParam, MF_BYCOMMAND | MF_UNCHECKED);// (bOptionChecked ? MF_CHECKED : MF_UNCHECKED)
            else
                Ex_MenuCheckItem(lParam, wParam, MF_BYCOMMAND | MF_CHECKED);// (bOptionChecked ? MF_CHECKED : MF_UNCHECKED)

        }

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
        0, OnListMenuMainWndMsgProc);
    Ex_DUISetLong(hExDui_listbutton, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    hMenu = Ex_MenuLoadW(GetModuleHandleW(0), (LPWSTR)IDR_MENU1);

    // 创建正常菜单条，显示Resource.rc资源文件里的菜单,若出现中文乱码将Resource.rc文件编码格式转为UCS-2
    // LE BOM
    HEXOBJ hObj = 0;
    hObj = Ex_ObjCreate(L"Menubar", 0, -1, 0, 30, 220, 24, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);         // 文本色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE);   // 点燃背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);   // 按下背景色
    Ex_ObjHandleEvent(hObj, NM_COMMAND, button_clickpopmenu);
    EX_LISTBUTTON_ITEMINFO item1 = { 0 };
    item1.wzText = L"文件(&F)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 0);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"编辑(&E)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 1);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"选项(&O)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 2);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"帮助(&H)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 3);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item1);

    // 创建自定义回调菜单条

    hObj = Ex_ObjCreateEx(-1, L"Menubar", 0, -1, 0, 60, 220, 24, hExDui_listbutton, 0, -1, 0, 0,
        OnListButtonMsgProc);
    Ex_ObjHandleEvent(hObj, MN_CREATE, button_clickpopmenu);
    //Ex_ObjHandleEvent(hObj, NM_CLICK, button_clickpopmenu);
    Ex_ObjHandleEvent(hObj, NM_COMMAND, button_clickpopmenu);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExARGB(110, 120, 55, 255),
        FALSE);   // 改变菜单按钮背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255),
        FALSE);   // 改变菜单按钮字体正常色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExARGB(255, 255, 255, 55),
        FALSE);   // 改变菜单按钮字体热点色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExARGB(255, 255, 255, 100),
        FALSE);   // 改变菜单按钮字体按下色
    item1.wzText = L"文件(&F)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 0);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"编辑(&E)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 1);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"选项(&O)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 2);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item1);
    item1.wzText = L"帮助(&H)";
    item1.nMenu = Ex_MenuGetSubMenu(hMenu, 3);
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item1);

    // 创建工具条
    hObj = Ex_ObjCreate(L"Toolbar", 0, -1, 0, 90, 400, 24, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), FALSE);         // 文本色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGB2ARGB(16774117, 255), FALSE);   // 点燃背景色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGB2ARGB(16765337, 255), FALSE);   // 按下背景色
    HEXIMAGELIST      hImageList = _imglist_create(18, 18);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/buttonex/4.png", &imgdata);
    size_t nImageIndex = _imglist_add(hImageList, 0, imgdata.data(), imgdata.size());
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_SETIMAGELIST, 0, (LPARAM)hImageList);
    EX_LISTBUTTON_ITEMINFO item2 = { 0 };
    item2.nType = 1;
    item2.nImage = nImageIndex;
    item2.wzText = NULL;
    item2.wzTips = L"普通按钮1";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType = 1;
    item2.nImage = 0;
    item2.wzText = L"普通按钮不带图标";
    item2.wzTips = L"普通按钮不带图标";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType = 2;
    item2.nImage = nImageIndex;
    item2.wzText = NULL;
    item2.wzTips = L"选择按钮1";
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType = 2;
    item2.nImage = 0;
    item2.wzText = L"选择按钮不带图标";
    item2.wzTips = L"选择按钮不带图标";
    item2.dwState = STATE_DOWN;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType = 0;
    item2.nImage = 0;
    item2.wzText = NULL;
    item2.wzTips = NULL;
    item2.dwState = STATE_NORMAL;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item2);
    item2.nType = 1;
    item2.nImage = nImageIndex;
    item2.wzText = L"禁用按钮带图标";
    item2.wzTips = L"禁用按钮带图标";
    item2.dwState = STATE_DISABLE;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item2);
    Ex_ObjHandleEvent(hObj, LISTBUTTON_EVENT_CLICK, OnListButtonEvent);
    Ex_ObjHandleEvent(hObj, LISTBUTTON_EVENT_CHECK, OnListButtonEvent);

    // 创建状态条
    hObj = Ex_ObjCreate(L"Statusbar", 0, -1, 0, 120, 300, 24, hExDui_listbutton);
    Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, ExRGB2ARGB(12557930, 255), FALSE);     // 背景色
    Ex_ObjSetColor(hObj, COLOR_EX_BORDER, ExARGB(255, 255, 255, 255), FALSE);        // 分割色
    Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExARGB(255, 255, 255, 255), FALSE);   // 文本色
    EX_LISTBUTTON_ITEMINFO item3 = { 0 };
    item3.wzText = L"左对齐";
    item3.nWidth = 100;
    item3.TextFormat = DT_LEFT;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item3);
    item3.wzText = L"居中";
    item3.nWidth = 100;
    item3.TextFormat = DT_CENTER;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 0, (size_t)&item3);
    item3.wzText = L"右对齐";
    item3.nWidth = 100;
    item3.nIndex = 2;
    item3.TextFormat = DT_RIGHT;
    Ex_ObjSendMessage(hObj, LISTVIEW_MESSAGE_INSERTITEM, 1, (size_t)&item3);

    // 设置菜单条目图标
    if (hMenu) {
        HEXMENU hMenu_sub = Ex_MenuGetSubMenu(hMenu, 0);
        if (hMenu_sub) {
            EXMENUITEMINFOW minfo{ 0 };
            minfo.cbSize = sizeof(EXMENUITEMINFOW);
            minfo.fMask = MIIM_BITMAP;
            Ex_MenuGetItemInfoW(hMenu_sub, 1, TRUE, &minfo);
            if (minfo.hbmpItem) {
                _img_destroy(minfo.hbmpItem);
            }
            std::vector<CHAR> imgdata1;
            //HBITMAP           hbitmap   = 0;
            HEXIMAGE          hImg = 0;
            HEXIMAGE          hImgSmall = 0;
            _img_createfromfile(L"res/rotateimgbox.jpg", &hImg);
            _img_scale(hImg, 24, 24, &hImgSmall);   // 注意菜单条目高度跟图像高度有关，因此缩放到24
            imgdata1.resize(24 * 24 * 4);
            _img_savetomemory(hImgSmall, imgdata1.data());
            _img_destroy(hImg);
            _img_destroy(hImgSmall);
            hImg = 0;
            _img_createfrommemory(imgdata1.data(), imgdata1.size(), &hImg);
            minfo.hbmpItem = hImg;
            Ex_MenuSetItemInfoW(hMenu_sub, 1, TRUE, &minfo);
            Ex_DUISetLong(hExDui_listbutton, ENGINE_LONG_LPARAM, (size_t)hImg);
        }
    }
    Ex_DUIShowWindow(hExDui_listbutton, SW_SHOWNORMAL);
}
