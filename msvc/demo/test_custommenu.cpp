#include "test_custommenu.h"

HMENU m_hMenu;
HMENU m_hMenuRight;

LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps{0};
        Ex_ObjBeginPaint(hObj, &ps);
        if ((ps.dwState & STATE_DOWN) != 0) {
            _canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 50));
        }
        else if ((ps.dwState & STATE_HOVER) != 0) {
            _canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 20));
        }
        else {
            _canvas_clear(ps.hCanvas, ExRGB2ARGB(0, 0));
        }
        FLOAT nWidthText  = 0;
        FLOAT nHeightText = 0;
        _canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj),
                             (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
                             ps.dwTextFormat, 0, ps.uWidth, ps.uHeight, &nWidthText, &nHeightText);
        HEXIMAGE hImg        = Ex_ObjGetLong(hObj, OBJECT_LONG_LPARAM);
        INT      nWidthIcon  = 0;
        INT      nHeightIcon = 0;
        if (hImg != 0) {
            _img_getsize(hImg, &nWidthIcon, &nHeightIcon);
            _canvas_drawimage(ps.hCanvas, hImg, (ps.uWidth - nWidthIcon) / 2,
                              (ps.uHeight - nHeightIcon - nHeightText - 3) / 2, 255);
        }

        _canvas_drawtext(
            ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL),
            (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE), -1,
            ps.dwTextFormat | DT_CENTER | DT_VCENTER, (ps.uWidth - nWidthText) / 2,
            (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3,
            (ps.uWidth + nWidthText) / 2,
            (ps.uHeight - nHeightIcon - nHeightText - 3) / 2 + nHeightIcon + 3 + nHeightText);

        Ex_ObjEndPaint(hObj, &ps);
        *lpResult = 1;
        return 1;
    }
    else if (uMsg == WM_EX_LCLICK) {
        auto id = Ex_ObjGetLong(hObj, OBJECT_LONG_ID);
        OUTPUTW(L"菜单按钮点击,id:", id);
        EndMenu();
        *lpResult = 1;
        return 1;
    }
    return 0;
}

LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                   LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND) {
        if (__get((LPVOID)lParam, 0) == wParam) {
            EX_PAINTSTRUCT ps{0};
            RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
            if (ps.uHeight > 10) {
                if ((ps.dwState & STATE_HOVER) != 0) {
                    _canvas_clear(ps.hCanvas, ExRGB2ARGB(16711680, 100));
                }
                else {
                    _canvas_clear(ps.hCanvas, 0);
                }
                *lpResult = 1;
                return 1;
            }
        }
    }
    else if (uMsg == WM_EX_LCLICK) {
        auto id = Ex_ObjGetLong(hObj, OBJECT_LONG_ID);
        OUTPUTW(L"菜单项目点击,id:", id);
    }
    return 0;
}

LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult)
{
    if (uMsg == WM_INITMENUPOPUP) {
        RECT rc{0};
        if (wParam == (size_t)m_hMenu)   // 主菜单
        {
            size_t value = 1;
            SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
            GetWindowRect(hWnd, &rc);
            SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10),
                         Ex_Scale(rc.bottom - rc.top + 10 + 108),
                         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            rc.right  = rc.right - rc.left - Ex_Scale(10);
            rc.bottom = rc.bottom - rc.top - Ex_Scale(10) + Ex_Scale(108);
            rc.left   = Ex_Scale(6);
            rc.top    = 40;
            // 创建顶部按钮
            HEXIMAGE hImg;
            _img_createfromfile(L"custommenu/btn1.png", &hImg);

            Ex_ObjCreateEx(-1, L"button", L"消息", OBJECT_STYLE_VISIBLE, rc.left, rc.top,
                           rc.right * 0.333, Ex_Scale(70), hExDUI, 100, -1, hImg, 0,
                           OnMenuBtnMsgProc);

            _img_createfromfile(L"custommenu/btn2.png", &hImg);
            Ex_ObjCreateEx(-1, L"button", L"收藏", OBJECT_STYLE_VISIBLE, rc.left + rc.right * 0.333,
                           rc.top, rc.right * 0.333, Ex_Scale(70), hExDUI, 101, -1, hImg, 0,
                           OnMenuBtnMsgProc);

            _img_createfromfile(L"custommenu/btn3.png", &hImg);
            Ex_ObjCreateEx(-1, L"button", L"文件", OBJECT_STYLE_VISIBLE, rc.left + rc.right * 0.666,
                           rc.top, rc.right * 0.333, Ex_Scale(70), hExDUI, 102, -1, hImg, 0,
                           OnMenuBtnMsgProc);

            HEXOBJ hObj =
                Ex_ObjCreateEx(OBJECT_STYLE_EX_TRANSPARENT | OBJECT_STYLE_EX_TOPMOST, L"Static", 0,
                               OBJECT_STYLE_VISIBLE, 0, 0, 45, 38, hExDUI, 0, -1, 0, 0, 0);
            std::vector<CHAR> data;
            Ex_ReadFile(L"custommenu/Icon.png", &data);
            Ex_ObjSetBackgroundImage(hObj, data.data(), data.size(), 0, 0,
                                     BACKGROUND_REPEAT_NO_REPEAT, 0, 0, 255, TRUE);
            rc.top    = rc.top + Ex_Scale(75);
            rc.bottom = rc.bottom - Ex_Scale(75);
        }
        else {
            // 子菜单
            size_t value = 0;
            SetPropW(hWnd, L"IsMainMenu", (HANDLE)value);
            GetWindowRect(hWnd, &rc);
            SetWindowPos(hWnd, 0, 0, 0, Ex_Scale(rc.right - rc.left + 10),
                         Ex_Scale(rc.bottom - rc.top + 10),
                         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            rc.right  = rc.right - rc.left - Ex_Scale(10);
            rc.bottom = rc.bottom - rc.top - Ex_Scale(10);
            rc.left   = Ex_Scale(6);
            rc.top    = Ex_Scale(8);
        }
        HEXOBJ hObjfind = Ex_ObjFind(hExDUI, 0, L"Item", 0);
        INT    t        = rc.top;
        RECT   rcObj{0};

        while (hObjfind != 0) {
            Ex_ObjGetClientRect(hObjfind, &rcObj);
            Ex_ObjMove(hObjfind, rc.left, t, rc.right, rcObj.bottom - rcObj.top, TRUE);
            Ex_ObjSetColor(hObjfind, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
            Ex_ObjSetLong(hObjfind, OBJECT_LONG_OBJPROC, (size_t)OnMenuItemMsgProc);
            t        = t + rcObj.bottom - rcObj.top;
            hObjfind = Ex_ObjGetObj(hObjfind, GW_HWNDNEXT);
        }
    }
    else if (uMsg == WM_ERASEBKGND)   // wParam画布句柄, LOWORD(lParam)为宽度,HIWORD(lParam)为高度
    {
        RECT rc{0};
        auto dpix = Ex_DUIGetSystemDpi();
        _canvas_clear(wParam, 0);
        HEXIMAGE hImg;
        if (GetPropW(hWnd, L"IsMainMenu") != 0) {
            _img_createfromfile(L"custommenu/Main.png", &hImg);
            _canvas_drawimagefromgrid(wParam, hImg, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 68,
                                      68, 46, 42, 13, 12, 0, 230);
        }
        else {
            _img_createfromfile(L"custommenu/Sub.png", &hImg);
            _canvas_drawimagefromgrid(wParam, hImg, 0, 0, LOWORD(lParam), HIWORD(lParam), 0, 0, 24,
                                      24, 8, 9, 10, 10, 0, 230);
        }
        _img_destroy(hImg);
        *lpResult = 1;
        return 1;
    }
    return 0;
}

LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
    POINT pt;
    GetCursorPos(&pt);
    Ex_TrackPopupMenu(m_hMenu, 0, pt.x, pt.y, 0, hObj, 0, OnMenuWndMsgProc, MENU_FLAG_NOSHADOW);
    return 0;
}

LRESULT CALLBACK OnMenuItemRightMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                                        LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_ERASEBKGND) {
        if (__get((LPVOID)lParam, 0) == wParam) {
            EX_PAINTSTRUCT ps{0};
            RtlMoveMemory(&ps, (LPVOID)lParam, sizeof(EX_PAINTSTRUCT));
            auto nodeID = Ex_ObjGetLong(hObj, OBJECT_LONG_NODEID);
            if (ps.uHeight > 10) {
                if ((ps.dwState & STATE_HOVER) == STATE_HOVER) {
                    _canvas_clear(ps.hCanvas, ExARGB(79, 125, 164, 255));
                }
                else {
                    _canvas_clear(ps.hCanvas, ExARGB(120, 120, 120, 255));
                }
                if (nodeID == 0) {
                    HEXIMAGE hImg      = 0;
                    HEXIMAGE hImgSmall = 0;
                    _img_createfromfile(L"res/rotateimgbox.jpg", &hImg);
                    _img_scale(hImg, 20, 20,
                               &hImgSmall);   // 注意菜单条目高度跟图像高度有关，因此缩放到24

                    _canvas_drawimage(ps.hCanvas, hImgSmall, 2, 2, 255);
                    _img_destroy(hImg);
                    _img_destroy(hImgSmall);
                }
                *lpResult = 1;
                return 1;
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnMenuWndRightMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam,
                                       LPARAM lParam, LRESULT* lpResult)
{
    if (uMsg == WM_INITMENUPOPUP) {
        if (wParam == (size_t)m_hMenuRight) {
            auto obj = Ex_ObjFind(hExDUI, 0, L"Item", 0);
            int  i   = 0;
            while (obj != 0) {
                Ex_ObjSetColor(obj, COLOR_EX_TEXT_NORMAL, ExRGB2ARGB(0, 255), TRUE);
                Ex_ObjSetLong(obj, OBJECT_LONG_OBJPROC, (LONG_PTR)OnMenuItemRightMsgProc);
                Ex_ObjSetLong(obj, OBJECT_LONG_NODEID, i);
                obj = Ex_ObjGetObj(obj, GW_HWNDNEXT);
                i++;
            }
        }
    }
    return 0;
}

LRESULT CALLBACK OnMenuButtonRightEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                                        LPARAM lParam)
{
    if (nCode == NM_CLICK) {
        POINT pt;
        GetCursorPos(&pt);
        Ex_TrackPopupMenu(m_hMenuRight, 0, pt.x, pt.y, 0, hObj, 0, OnMenuWndRightMsgProc,
                          MENU_FLAG_NOSHADOW);
    }
    return 0;
}

void test_custommenu(HWND hWnd)
{
    HWND hWnd_custommenu =
        Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试弹出菜单", 0, 0, 300, 200, 0, 0);
    HEXDUI hExDui_custommenu = Ex_DUIBindWindowEx(
        hWnd_custommenu, 0,
        WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW |
            WINDOW_STYLE_NOSHADOW | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_TITLE |
            WINDOW_STYLE_HASICON,
        0, 0);
    Ex_DUISetLong(hExDui_custommenu, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));

    HEXOBJ hObj_button =
        Ex_ObjCreate(L"button", L"弹出菜单", -1, 50, 50, 100, 30, hExDui_custommenu);
    Ex_ObjHandleEvent(hObj_button, NM_CLICK, OnMenuButtonEvent);

    m_hMenu = CreatePopupMenu();
    AppendMenuW(m_hMenu, MF_STRING | MF_ENABLED, 301, L"项目1");
    CheckMenuItem(m_hMenu, 301, MF_BYCOMMAND | MF_CHECKED);   // 选中
    AppendMenuW(m_hMenu, MF_STRING, 302, L"禁用项目");
    EnableMenuItem(m_hMenu, 302, MF_DISABLED);   // 置灰色
    AppendMenuW(m_hMenu, MF_SEPARATOR, 0, L"横线");
    AppendMenuW(m_hMenu, MF_STRING, 666, L"待删除项目");
    DeleteMenu(m_hMenu, 666, MF_BYCOMMAND);   // 删除666菜单

    // 创建一个子菜单
    HMENU hSubMenu = CreateMenu();
    AppendMenuW(hSubMenu, MF_STRING | MF_ENABLED, 3001, L"子项目1");   // 添加项目
    AppendMenuW(m_hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"更多");


    // 以下是通过item组件改变菜单项目=====================
    HEXOBJ hObj_button2 = Ex_ObjCreateEx(-1, L"BUTTON", L"弹出菜单2", -1, 170, 50, 100, 30,
                                         hExDui_custommenu, 0, -1, 0, 0, 0);
    Ex_ObjHandleEvent(hObj_button2, NM_CLICK, OnMenuButtonRightEvent);
    m_hMenuRight = CreatePopupMenu();
    AppendMenuW(m_hMenuRight, MF_STRING, 401, L"项目一");
    AppendMenuW(m_hMenuRight, MF_STRING, 402, L"项目二");

    Ex_DUIShowWindow(hExDui_custommenu, SW_SHOWNORMAL, 0, 0, 0);
}
