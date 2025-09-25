#include "test_rollmenu.h"

HEXOBJ       m_hObjRM = 0;
HEXIMAGELIST m_hImageListRollMenu;

LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                       LPARAM lParam)
{
    if (nID == 101)   // 删除分组
    {
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_DELGROUP, 3, 0);
    }
    else if (nID == 102)   // 删除子项
    {
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_DELITEM, 4, 2);
    }
    else if (nID == 103)   // 设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
    {
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_SETEXPAND, 2, 1);
    }
    else if (nID == 104)   // 设置分组状态(展开/收缩)  wParam :分组索引(从1开始)  lParam: 状态(BOOL)
    {
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_SETEXPAND, 2, 0);
    }
    else if (nID == 105)   // 取
    {
        int group = 0, item = 0;
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_GETSEL, (WPARAM)&group, (LPARAM)&item);
        OUTPUTW(L"ROLLMENU_MESSAGE_GETSEL 所在分组:", group, L"选中子项:", item);
    }
    else if (nID == 106)   // 设置
    {
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_SETSEL, 4, 2);
    }
    return 0;
}

LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == ROLLMENU_EVENT_CLICK) {
        OUTPUTW(L"RollMenu单击子项  子项:", wParam, L"所在分组: ", lParam);
    }
    return 0;
}

LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_CLOSE) {
        _imglist_destroy(m_hImageListRollMenu);
    }
    return 0;
}

void test_rollmenu(HWND hWnd)
{
    HWND   hWndRM = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试卷帘菜单", 0, 0, 500, 500, 0, 0);
    HEXDUI hExDui_RM = Ex_DUIBindWindowEx(
        hWndRM, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_SIZEABLE |
            WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE |
            WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_SIZEABLE,
        0, OnRollMenuWndMsgProc);
    Ex_DUISetLong(hExDui_RM, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
    m_hObjRM = Ex_ObjCreateEx(-1, L"RollMenu", NULL, -1, 30, 50, 220, 400, hExDui_RM, 0,
                              DT_VCENTER | DT_CENTER, 0, 0,
                              NULL);   // DT_LEFT
    Ex_ObjHandleEvent(m_hObjRM, ROLLMENU_EVENT_CLICK, OnRollMenuBtnEvent);

    auto btn = Ex_ObjCreateEx(-1, L"button", L"删除分组", -1, 280, 150, 100, 30, hExDui_RM, 101,
                              DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
    btn = Ex_ObjCreateEx(-1, L"button", L"删除子项", -1, 280, 200, 100, 30, hExDui_RM, 102,
                         DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
    btn = Ex_ObjCreateEx(-1, L"button", L"展开分组", -1, 280, 250, 100, 30, hExDui_RM, 103,
                         DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
    btn = Ex_ObjCreateEx(-1, L"button", L"收缩分组", -1, 280, 300, 100, 30, hExDui_RM, 104,
                         DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
    btn = Ex_ObjCreateEx(-1, L"button", L"取现行选中子项", -1, 280, 350, 100, 30, hExDui_RM, 105,
                         DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);
    btn = Ex_ObjCreateEx(-1, L"button", L"置现行选中子项", -1, 280, 400, 100, 30, hExDui_RM, 106,
                         DT_VCENTER | DT_CENTER, 0, 0, NULL);
    Ex_ObjHandleEvent(btn, NM_CLICK, OnRollMenuWndBtnEvent);

    m_hImageListRollMenu = _imglist_create(32, 32);
    std::vector<CHAR> imgdata;
    Ex_ReadFile(L"res/account.png", &imgdata);
    size_t nImageAccountIndex =
        _imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size());
    Ex_ReadFile(L"res/video.png", &imgdata);
    size_t nImageVideoIndex = _imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size());
    Ex_ReadFile(L"res/info.png", &imgdata);
    size_t nImageInfoIndex = _imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size());
    Ex_ReadFile(L"res/fans.png", &imgdata);
    size_t nImageFansIndex = _imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size());
    Ex_ReadFile(L"res/authorization.png", &imgdata);
    size_t nImageAuthIndex = _imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size());

    std::vector<INT> emoji;
    Ex_ReadFile(L"res/navbtn/大图标1.png", &imgdata);
    emoji.push_back(_imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size()));
    Ex_ReadFile(L"res/navbtn/大图标2.png", &imgdata);
    emoji.push_back(_imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size()));
    Ex_ReadFile(L"res/navbtn/大图标3.png", &imgdata);
    emoji.push_back(_imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size()));
    Ex_ReadFile(L"res/navbtn/大图标4.png", &imgdata);
    emoji.push_back(_imglist_add(m_hImageListRollMenu, 0, imgdata.data(), imgdata.size()));

    EX_ROLLMENU_DATA rollmenu = {0};
    rollmenu.title            = L"账号信息";
    rollmenu.stateico.eicon   = _imglist_get(m_hImageListRollMenu, nImageAccountIndex);
    rollmenu.stateico.sicon   = _imglist_get(m_hImageListRollMenu, nImageAccountIndex);
    rollmenu.stateico.rc      = {40, 4, 72, 36};
    Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&rollmenu);

    rollmenu.title          = L"视频管理";
    rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageVideoIndex);
    rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageVideoIndex);
    rollmenu.stateico.rc    = {40, 4, 72, 36};
    size_t groupVideoIndex =
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&rollmenu);
    EX_ROLLMENU_ITEM rollitemAuth = {0};
    rollitemAuth.title            = L"视频权限";
    rollitemAuth.stateico.sicon   = _imglist_get(m_hImageListRollMenu, nImageAuthIndex);
    rollitemAuth.stateico.eicon   = _imglist_get(m_hImageListRollMenu, nImageAuthIndex);
    rollitemAuth.stateico.rc      = {55, 0, 83, 28};
    Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDITEM, groupVideoIndex, (LPARAM)&rollitemAuth);
    EX_ROLLMENU_ITEM rollitem = {0};
    rollitem.title            = L"视频列表";
    Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDITEM, groupVideoIndex, (LPARAM)&rollitem);

    rollmenu.title          = L"数据分析";
    rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageInfoIndex);
    rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageInfoIndex);
    rollmenu.stateico.rc    = {40, 4, 72, 36};
    Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&rollmenu);

    rollmenu.title          = L"粉丝画像分析";
    rollmenu.stateico.eicon = _imglist_get(m_hImageListRollMenu, nImageFansIndex);
    rollmenu.stateico.sicon = _imglist_get(m_hImageListRollMenu, nImageFansIndex);
    rollmenu.stateico.rc    = {40, 4, 72, 36};
    size_t groupFansIndex =
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDGROUP, 0, (LPARAM)&rollmenu);

    rollitem.extraico.rc = {160, 2, 188, 30};
    std::wstring title;
    for (int i = 0; i < 20; i++) {
        title                  = L"粉丝_" + std::to_wstring(i + 1);
        rollitem.title         = title.c_str();
        rollitem.extraico.icon = _imglist_get(m_hImageListRollMenu, emoji[Random(0, 3)]);
        Ex_ObjSendMessage(m_hObjRM, ROLLMENU_MESSAGE_ADDITEM, groupFansIndex, (LPARAM)&rollitem);
    }

    Ex_DUIShowWindow(hExDui_RM, SW_SHOWNORMAL);
}
