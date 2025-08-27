#include "stdafx.h"

LRESULT CALLBACK _wnd_defwindowprocW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor)
{
    WNDCLASSEXW WndClass   = {0};
    WndClass.cbSize        = sizeof(WNDCLASSEXW);
    WndClass.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;   //| CS_DBLCLKS
    WndClass.lpfnWndProc   = _wnd_defwindowprocW;
    WndClass.hInstance     = g_Li.hInstance;
    WndClass.hCursor       = (hCursor == NULL ? g_Li.hCursor : hCursor);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);   // NULL_BRUSH
    WndClass.hIcon         = (hIcon == NULL ? g_Li.hIcon : hIcon);
    WndClass.hIconSm       = (hIconsm == NULL ? g_Li.hIconsm : hIconsm);
    WndClass.lpszClassName = lpwzClassName;
    return RegisterClassExW(&WndClass);
}

HEXDUI Ex_DUIFromWindow(HWND hWnd)
{
    return SendMessageW(hWnd, g_Li.dwMessage, EMV_HEXDUI, MAKELONG(EMT_DUI, 0));
}

BOOL _wnd_getfromhandle(size_t handle, HWND* hWnd, wnd_s** ppWnd, obj_s** ppObj, BOOL* isObject,
                        INT* nError)
{
    if (isObject) {
        *isObject = FALSE;
    }
    if (IsWindow((HWND)handle)) handle = Ex_DUIFromWindow((HWND)handle);

    wnd_s* pWnd = NULL;
    obj_s* pObj = NULL;

    if (_handle_validate(handle, HT_DUI, (LPVOID*)&pWnd, nError)) {
        if (hWnd) {
            *hWnd = pWnd->hWnd_;
        }
        pObj = (obj_s*)pWnd;
    }
    else if (_handle_validate(handle, HT_OBJECT, (LPVOID*)&pObj, nError)) {
        pWnd = (wnd_s*)pObj->pWnd_;
        if (hWnd) {
            *hWnd = pWnd->hWnd_;
        }
        if (isObject) {
            *isObject = TRUE;
        }
    }
    else {
        return FALSE;
    }
    if (ppWnd) {
        *ppWnd = pWnd;
    }
    if (ppObj) {
        *ppObj = pObj;
    }
    return TRUE;
}

LRESULT _wnd_dispatch_notify(HWND hWnd, wnd_s* pWnd, size_t hObj, INT nID, INT nCode, WPARAM wParam,
                             LPARAM lParam, obj_s* pObj)
{
    WinMsgPROC pfnMsgProc = pWnd->pfnMsgProc_;
    LRESULT    ret        = 0;
    if (pfnMsgProc) {
        EX_NMHDR hdr;
        hdr.hObjFrom = hObj;
        hdr.idFrom   = nID;
        hdr.lParam   = lParam;
        hdr.nCode    = nCode;
        hdr.wParam   = wParam;
        pfnMsgProc(hWnd, pWnd->hexdui_, WM_NOTIFY, nID, (size_t)&hdr, &ret);
    }

    return ret;
}

void _wnd_redraw_bkg(HWND hWnd, wnd_s* pWnd, LPVOID lpRect, BOOL bRedrawBkg, BOOL bUpdate)
{
    if (bRedrawBkg) {
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BREDRAWBACKGROUND;
    }
    InvalidateRect(hWnd, (RECT*)lpRect, FALSE);
    if (bUpdate) {
        UpdateWindow(hWnd);
    }
}

BOOL _wnd_querystyle(HWND hWnd, INT dwStyle, BOOL bExStyle)
{
    return (GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE) & dwStyle) != 0;
}

BOOL _wnd_delstyle(HWND hWnd, INT dwStyle, BOOL bExStyle)
{
    auto ret = GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE);
    if ((ret & dwStyle) != 0) {
        ret &= ~dwStyle;
        SetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE, ret);
        return TRUE;
    }
    return FALSE;
}

BOOL _wnd_addstyle(HWND hWnd, INT dwStyle, BOOL bExStyle)
{
    auto ret = GetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE);
    if ((ret & dwStyle) == 0) {
        SetWindowLongPtrW(hWnd, bExStyle ? GWL_EXSTYLE : GWL_STYLE, ret | dwStyle);
        return TRUE;
    }
    return FALSE;
}

LRESULT _wnd_defaultproc(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

HICON _wnd_geticonhandle(HWND hWnd, BOOL isbigicon)
{
    HICON ret = (HICON)SendMessageW(hWnd, WM_GETICON, (isbigicon ? 1 : 0), 0);
    if (ret == 0) {
        ret = (HICON)GetClassLongPtrW(hWnd, isbigicon ? GCLP_HICON : GCLP_HICONSM);
    }
    return ret;
}

BOOL _wnd_getscreenrect(HWND hWnd, RECT* rcMonitor, RECT* rcDesk)
{
    BOOL     ret      = FALSE;
    HMONITOR hMonitor = MonitorFromWindow(hWnd, 2);
    if (hMonitor != 0) {
        MONITORINFO info = {0};
        info.cbSize      = 40;
        if (GetMonitorInfoW(hMonitor, &info)) {
            if (rcMonitor) {
                *rcMonitor = info.rcMonitor;
            }
            if (rcDesk) {
                *rcDesk = info.rcWork;
            }
            ret = TRUE;
        }
    }
    if (!ret) {
        Ex_SetLastError(GetLastError());
    }
    return ret;
}

std::wstring _wnd_gettitle(HWND hWnd)
{
    auto len = SendMessageW(hWnd, WM_GETTEXTLENGTH, 0, 0);
    len      = (len + 1) * 2;
    std::wstring ret;
    ret.resize(len);
    SendMessageW(hWnd, WM_GETTEXT, len, (LPARAM)(ret.data()));
    return ret;
}

INT _wnd_classtoatom(HWND hWnd)
{
    std::wstring ret;
    ret.resize(64);
    GetClassNameW(hWnd, (LPWSTR)ret.data(), 64);
    return Ex_Atom(ret.data());
}

INT _wnd_getfunctionkeys()
{
    INT ret = 0;
    if ((GetAsyncKeyState(VK_CONTROL) & 32768) != 0) {
        ret = ret | 1;
    }
    if ((GetAsyncKeyState(VK_SHIFT) & 32768) != 0) {
        ret = ret | 2;
    }
    if ((GetAsyncKeyState(VK_MENU) & 32768) != 0) {
        ret = ret | 4;
    }
    return ret;
}

void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, BOOL bFullScreen)
{
    RECT rcWindow, rcParent;
    GetWindowRect(hWnd, &rcWindow);
    if (hWndFrom == 0) {
        if (bFullScreen) {
            _wnd_getscreenrect(hWnd, &rcParent);
        }
        else {
            _wnd_getscreenrect(hWnd, NULL, &rcParent);
        }
    }
    else {
        GetWindowRect(hWndFrom, &rcParent);
    }
    SetWindowPos(
        hWnd, 0,
        rcParent.left + (rcParent.right - rcParent.left - (rcWindow.right - rcWindow.left)) / 2,
        rcParent.top + (rcParent.bottom - rcParent.top - (rcWindow.bottom - rcWindow.top)) / 2, 0,
        0, SWP_NOSIZE);
}

WPARAM Ex_WndMsgLoop()
{
    MSG msg = {0};
    while (GetMessageW(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return msg.wParam;
}

HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, INT x, INT y,
                  INT width, INT height, INT dwStyle, INT dwStyleEx)
{
    if (dwStyle == 0) {
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    if (dwStyleEx == 0) {
        dwStyleEx = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;
    }
    if ((dwStyle & WS_CHILD) != WS_CHILD) {
        dwStyleEx = dwStyleEx | WS_EX_LAYERED;
    }

    if (lpwzClassName == 0) {
        lpwzClassName = (LPCWSTR)g_Li.atomClassName;
    }

    HINSTANCE hInst = g_Li.hInstance;
    if (IsWindow(hWndParent)) {
        hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, GWLP_HINSTANCE);
    }

    HWND hWnd = CreateWindowExW(dwStyleEx, lpwzClassName, lpwzWindowName, dwStyle, x, y, width,
                                height, hWndParent, NULL, hInst, NULL);
    if (hWnd != 0) {
        SendMessageW(hWnd, 128, 0, (LPARAM)g_Li.hIconsm);
        SendMessageW(hWnd, 128, 1, (LPARAM)g_Li.hIcon);
    }
    return hWnd;
}

void _wnd_recalcclient(wnd_s* pWnd, HWND hWnd, INT width, INT height)
{
    pWnd->width_  = width;
    pWnd->height_ = height;
    if (_canvas_resize(pWnd->canvas_display_, width, height)) {
        if (_canvas_resize(pWnd->canvas_bkg_, width,
                           height))   // 客户区被改变,必定触发背景被改变事件
        {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BREDRAWBACKGROUND;
        }
    }
    _rgn_destroy(pWnd->hrgn_client_);
    _rgn_destroy(pWnd->hrgn_sizebox_);
    INT nSizebox = 0;

    if (((pWnd->dwStyle_ & WINDOW_STYLE_SIZEABLE) == WINDOW_STYLE_SIZEABLE)) {
        nSizebox = 5;
    }
    LPVOID hRgnClient =
        _rgn_createfromrect(nSizebox, nSizebox, width - nSizebox, height - nSizebox);
    pWnd->hrgn_client_ = hRgnClient;
    if (nSizebox != 0) {
        LPVOID hRgnNC      = _rgn_createfromrect(0, 0, width, height);
        LPVOID hRgnSizebox = _rgn_combine(hRgnNC, hRgnClient, REGION_COMBINE_EXCLUDE, 0, 0);
        _rgn_destroy(hRgnNC);
        pWnd->hrgn_sizebox_ = hRgnSizebox;
    }
}

BOOL _wnd_wm_stylechanging(wnd_s* pWnd, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    BOOL ret      = FALSE;
    INT  styleNew = __get_int((LPVOID)lParam, 4);
    INT  styleDui = pWnd->dwStyle_;

    if (wParam == GWL_EXSTYLE) {

        if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BLAYERED;
        }
        else {
            if (__query_int((LPVOID)lParam, 4, WS_EX_LAYERED))   // 待定有问题
            {

                pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BLAYERED);
            }
            else {
                if (((pWnd->dwFlags_ & EWF_BLAYERED) == EWF_BLAYERED)) {
                    pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BLAYERED);
                }
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
    }
    else {
        styleNew = (styleNew & ~(WS_DLGFRAME | WS_BORDER));
        if ((styleDui & WINDOW_STYLE_BUTTON_MIN) == 0 ||
            (styleDui & WINDOW_STYLE_FULLSCREEN) == WINDOW_STYLE_FULLSCREEN) {
            styleNew = (styleNew & ~WS_MINIMIZEBOX);
        }
        else {
            styleNew = styleNew | WS_MINIMIZEBOX;
        }

        if ((styleDui & WINDOW_STYLE_BUTTON_MAX) == 0 ||
            (styleDui & WINDOW_STYLE_FULLSCREEN) == WINDOW_STYLE_FULLSCREEN) {
            styleNew = (styleNew & ~WS_MAXIMIZEBOX);
        }
        else {
            styleNew = styleNew | WS_MAXIMIZEBOX;
        }

        if ((styleDui & WINDOW_STYLE_FULLSCREEN) == WINDOW_STYLE_FULLSCREEN) {
            styleNew = (styleNew & ~WS_SYSMENU);
        }
        else {
            styleNew = styleNew | WS_SYSMENU;
        }

        if ((styleDui & WINDOW_STYLE_SIZEABLE) == WINDOW_STYLE_SIZEABLE) {
            styleNew = styleNew | WS_THICKFRAME;
        }
        else {
            styleNew = styleNew & ~WS_THICKFRAME;
        }

        if (styleNew != ((STYLESTRUCT*)lParam)->styleNew) {
            ((STYLESTRUCT*)lParam)->styleNew = styleNew;
            ret                              = TRUE;
        }
    }
    return ret;
}

void _wnd_loadtheme(wnd_s* pWnd, HWND hWnd, HEXTHEME hTheme)
{

    EXATOM atom =
        ((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU) ? ATOM_DUIMENU : ATOM_WINDOW;
    LPVOID pPADDING_SHADOW = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_PADDING_SHADOW);
    if (pPADDING_SHADOW != 0) {
        pWnd->padding_shadow_ = pPADDING_SHADOW;
        pWnd->minWidth_  = Ex_Scale(__get_int(pPADDING_SHADOW, 0) + __get_int(pPADDING_SHADOW, 8));
        pWnd->minHeight_ = Ex_Scale(__get_int(pPADDING_SHADOW, 4) + __get_int(pPADDING_SHADOW, 8));
    }
    LPVOID pSIZE = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);

    if (pSIZE != 0) {
        pWnd->size_caption_ = pSIZE;
    }
    LPVOID pMARGIN = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_MARGIN);

    if (pMARGIN != 0) {
        pWnd->margin_caption_ = pMARGIN;
    }
    // 20241214修改圆角 使其能够 圆角 shadow
    void* pRADIUS = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_RADIUS);

    if (pRADIUS != 0) {
        pWnd->Radius_ = __get_int(pRADIUS, 0);
    }
    LPVOID pBACKGROUND_COLOR = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_BACKGROUND_COLOR);

    if (pBACKGROUND_COLOR != 0) {
        pWnd->crBkg_ = __get_int(pBACKGROUND_COLOR, 0);
    }

    LPVOID pSHADOW_COLOR = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SHADOW_COLOR);

    if (pSHADOW_COLOR != 0) {
        pWnd->crSD_ = __get_int(pSHADOW_COLOR, 0);
    }
    // 菜单
    LPVOID pPADDING_CLIENT = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_PADDING_CLIENT);
    if (pPADDING_CLIENT != 0) {
        pWnd->padding_client_ = pPADDING_CLIENT;
    }
    LPVOID pPADDING_TEXT = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_PADDING_TEXT);
    if (pPADDING_TEXT != 0) {
        pWnd->padding_text_ = pPADDING_TEXT;
    }
    LPVOID pPADDING_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_PADDING_SEPARATOR);
    if (pPADDING_SEPARATOR != 0) {
        pWnd->padding_separator_ = pPADDING_SEPARATOR;
    }
    LPVOID pSIZE_ITEM = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_SIZE_ITEM);
    WORD   szItem     = 0;
    if (pSIZE_ITEM != 0) {
        szItem = MAKEWORD(__get_int(pSIZE_ITEM, 0), __get_int(pSIZE_ITEM, 4));
    }
    LPVOID pSIZE_SEPARATOR = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_SIZE_SEPARATOR);
    WORD   szSeparator     = 0;
    if (pSIZE_SEPARATOR != 0) {
        szSeparator = MAKEWORD(__get_int(pSIZE_SEPARATOR, 0), __get_int(pSIZE_SEPARATOR, 4));
    }

    pWnd->szItemSeparator_ = MAKELONG(szItem, szSeparator);
    LPVOID lpFontFamily    = nullptr;
    INT    dwFontSize      = -1;
    INT    dwFontStyle     = -1;
    LPVOID pFONT_SIZE      = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_SIZE);
    if (pFONT_SIZE != 0) {
        dwFontSize = __get_int(pFONT_SIZE, 0);
    }
    LPVOID pFONT_STYLE = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_STYLE);
    if (pFONT_STYLE != 0) {
        dwFontStyle = __get_int(pFONT_STYLE, 0);
    }
    LPVOID pFONT_FAMILY = Ex_ThemeGetValuePtr(hTheme, ATOM_ITEM, ATOM_FONT_FAMILY);
    if (pFONT_FAMILY != 0) {
        lpFontFamily = pFONT_FAMILY;
    }

    HEXFONT pfont = _font_createfromfamily((LPWSTR)lpFontFamily, dwFontSize, dwFontStyle);

    if (pfont != 0) {
        _font_destroy(pWnd->hFont_Menu_);
        pWnd->hFont_Menu_ = pfont;
    }
    _wnd_recalcclient(pWnd, hWnd, pWnd->width_, pWnd->height_);
}

void _wnd_calc_captionrect(wnd_s* pWnd, RECT* rcCaption)
{
    LPVOID lpValue = pWnd->size_caption_;
    if (lpValue != 0) {
        (*rcCaption).right  = __get_int(lpValue, 0) / 100 * pWnd->width_;   // 保证百分比
        (*rcCaption).bottom = Ex_Scale(__get_int(lpValue, 4));              // 绝对高度
    }
    lpValue = pWnd->margin_caption_;
    if (lpValue != 0) {
        (*rcCaption).left  = Ex_Scale(__get_int(lpValue, 0));
        (*rcCaption).top   = Ex_Scale(__get_int(lpValue, 4));
        (*rcCaption).right = (*rcCaption).right - Ex_Scale(__get_int(lpValue, 8));
    }
    (*rcCaption).bottom = (*rcCaption).bottom + (*rcCaption).top;
}

void CALLBACK _wnd_backgroundimage_timer_inherit(HWND hWnd, UINT uMsg, UINT_PTR idEvent,
                                                 DWORD dwTime)
{
    wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_BKG_INHERIT);

    if (!((pWnd->dwStyle_ & WINDOW_STYLE_NOINHERITBKG) == WINDOW_STYLE_NOINHERITBKG)) {
        wnd_s* ppWnd  = nullptr;
        INT    nError = 0;
        if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (LPVOID*)&ppWnd, &nError)) {
            EX_BACKGROUNDIMAGEINFO* lpBI = (EX_BACKGROUNDIMAGEINFO*)ppWnd->lpBackgroundImage_;
            if (lpBI) {
                INT* pDelay = (INT*)lpBI->lpDelay;
                if (pDelay != 0) {
                    _wnd_redraw_bkg(hWnd, pWnd, 0, TRUE, FALSE);
                    UpdateWindow(hWnd);
                }
            }
        }
    }
}

size_t _wnd_dispatch_msg(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT16  nType = LOWORD(lParam);
    size_t ret   = 0;
    if (nType == EMT_OBJECT)   // 组件消息
    {
        ret = _wnd_dispatch_msg_obj(hWnd, (mempoolmsg_s*)wParam, 0, 0, 0, 0);
    }
    else if (nType == EMT_DUI)   // 窗体消息
    {
        if (wParam == EMV_HEXDUI) {
            return pWnd->hexdui_;
        }
        else if (wParam == EMV_PFN_PACK) {}
    }
    else if (nType >= WM_MOUSEFIRST && nType <= WM_MOUSELAST ||
             nType >= NIN_SELECT && nType <= NIN_BALLOONUSERCLICK) {
        _wnd_dispatch_notify(hWnd, pWnd, pWnd->hexdui_, -1, NM_TRAYICON, wParam, lParam, 0);
    }
    else if (nType == EMT_LAYOUT_UPDATE) {
        _layout_update(wParam);
    }
    else if (nType == EMT_EASING) {
        _wnd_dispatch_notify(hWnd, pWnd, pWnd->hexdui_, -1, NM_EASING, __get((LPVOID)wParam, 0),
                             wParam, 0);
    }
    return ret;
}

size_t _wnd_dispatch_msg_obj(HWND hWnd, mempoolmsg_s* lpData, obj_s* pObj, INT uMsg, WPARAM wParam,
                             LPARAM lParam)
{
    size_t ret = 0;
    if (MemPool_AddressIsUsed(lpData)) {
        pObj   = lpData->pObj;
        uMsg   = lpData->uMsg;
        wParam = lpData->wParam;
        lParam = lpData->lParam;
        if (pObj) {
            ret = _obj_msgproc(hWnd, pObj->hObj_, pObj, uMsg, wParam, lParam);
        }
        MemPool_Free(g_Li.hMemPoolMsg, lpData);
    }
    return ret;
}

LRESULT CALLBACK _wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    wnd_s* pWnd = (wnd_s*)dwRefData;
    WinMsgPROC pfnMsgProc = pWnd->pfnMsgProc_;
    if (pfnMsgProc != 0) {
        LRESULT ret = 0;
        if (pfnMsgProc(hWnd, pWnd->hexdui_, uMsg, wParam, lParam, &ret) != 0) {
            return ret;
        }
    }

    if (uMsg == g_Li.dwMessage) {
        return _wnd_dispatch_msg(hWnd, pWnd, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_NCHITTEST)   // 132
    {
        return _wnd_wm_nchittest(pWnd, hWnd, lParam);
    }
    else if (uMsg == WM_SETCURSOR)   // 32
    {
        if (_wnd_wm_setcursor(hWnd, pWnd, lParam)) {
            return 1;
        }
    }
    else if (uMsg == WM_CLOSE) {
        HWND hwndParent = (HWND)GetWindowLongPtrW(
            hWnd, GWLP_HWNDPARENT);   // 防止消息框子窗口或组合框子窗口关闭使主窗口失去激活
        if (hwndParent) {
            if (GetActiveWindow() != hwndParent) {
                SetActiveWindow(hwndParent);
            }
        }
    }
    else if (uMsg == WM_DESTROY) {
        if (_wnd_destroy(hWnd, pWnd) != 0) {
            PostQuitMessage(0);
        }
        RemoveWindowSubclass(hWnd, _wnd_proc, 0);
    }
    else if (uMsg == WM_ERASEBKGND) {
        return 1;
    }
    else if (uMsg == WM_SIZE) {
        INT width  = LOWORD(lParam);
        INT height = HIWORD(lParam);
        _wnd_wm_size(pWnd, hWnd, wParam, width, height);
    }
    else if (uMsg == WM_WINDOWPOSCHANGING)   // 70
    {
        if (((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {
            tagWINDOWPOS* pos = (tagWINDOWPOS*)lParam;
            if (!(pos->flags & SWP_NOSIZE)) {
                LPVOID padding_client = pWnd->padding_client_;
                if (padding_client != 0) {
                    pos->cy +=
                        Ex_Scale(__get_int(padding_client, 4) + __get_int(padding_client, 12));
                }
            }

            if ((pos->flags & SWP_NOMOVE) != SWP_NOMOVE)   // 被移动了
            {

                if (GetWindow(hWnd, GW_OWNER) != 0)   // 子菜单
                {

                    pWnd->pMenuHostWnd_->dwFlags_ =
                        pWnd->pMenuHostWnd_->dwFlags_ -
                        (pWnd->pMenuHostWnd_->dwFlags_ & EWF_BMENUREPOSTION);
                    _wnd_menu_setpos(hWnd, pWnd, pos);
                }
                pWnd->left_ = pos->x;
                pWnd->top_  = pos->y;
                _wnd_menu_init(hWnd, pWnd);
                return 0;
            }
        }
    }
    else if (uMsg == WM_WINDOWPOSCHANGED)   // 71
    {
        tagWINDOWPOS* pos = (tagWINDOWPOS*)lParam;
        if (!(pos->flags & SWP_NOMOVE)) {
            pWnd->left_ = pos->x;
            pWnd->top_  = pos->y;
            _wnd_paint_shadow(pWnd, FALSE, FALSE);
        }
    }
    else if (uMsg == WM_NCCALCSIZE)   // 131
    {
        return 0;
    }
    else if (uMsg == WM_NCACTIVATE)   // 134
    {
        obj_s Obj;
        _wnd_wm_leavecheck(hWnd, pWnd, pWnd->objHittest_, -1, &Obj, lParam, TRUE);
        if (wParam == 0) {
            HEXOBJ focus        = pWnd->objFocus_;
            pWnd->dwFlags_      = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_ACTIVE);
            pWnd->objFocusPrev_ = focus;
            Ex_ObjKillFocus(focus);
        }
        else {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_ACTIVE;
            HEXOBJ focus   = pWnd->objFocusPrev_;
            Ex_ObjSetFocus(focus);
            pWnd->objFocusPrev_ = 0;
        }
        _wnd_paint_shadow(pWnd, FALSE, TRUE);
        if (wParam == 0) {
            if (lParam == (size_t)(pWnd->hWndShadow_)) {
                if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {

                    return 0;
                }
            }
        }
        return 1;   // 防止出现边框/系统按钮
    }
    else if (uMsg == WM_NCPAINT)   // 133
    {
        return 0;
    }
    else if (uMsg == WM_MOUSEACTIVATE) {
        // 防止弹出窗获取焦点
        if (FLAGS_CHECK(pWnd->dwStyle_, WINDOW_STYLE_POPUPWINDOW)) {
            return MA_NOACTIVATE;
        }
    }
    else if (uMsg == WM_NCLBUTTONDOWN) {
        _wnd_popupclose(pWnd, hWnd, 0, 0);
    }
    else if (uMsg == WM_PAINT)   // 15
    {
        if (_wnd_wm_paint(pWnd, hWnd)) {
            return 0;
        }
    }
    else if (uMsg == WM_SHOWWINDOW)   // 24
    {
        if (wParam == 0) {
            HEXOBJ objHittest = pWnd->objHittest_;
            if (objHittest != 0) {
                Ex_ObjDispatchMessage(objHittest, WM_MOUSELEAVE, 0, 0);
                pWnd->objHittest_ = 0;
                UpdateWindow(hWnd);
            }
        }
        HWND hWndShadow = pWnd->hWndShadow_;
        if (!_wnd_querystyle(hWnd, WS_EX_TOPMOST, TRUE)) {
            SetWindowPos(
                hWndShadow, HWND_NOTOPMOST, 0, 0, 0, 0,
                SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
        }
        ShowWindow(hWndShadow, wParam == 0 ? 0 : SW_SHOWNOACTIVATE);
        InvalidateRect(hWnd, NULL, FALSE);
    }
    else if (uMsg == WM_GETMINMAXINFO)   // 36
    {
        if (_wnd_wm_getminmaxinfo(pWnd, hWnd, lParam)) {
            return 0;
        }
    }
    else if (uMsg == WM_STYLECHANGING)   // 124
    {
        if (_wnd_wm_stylechanging(pWnd, hWnd, wParam, lParam)) {
            return 0;
        }
    }
    else if (
        uMsg ==
        WM_MOUSEWHEEL)   // 522
                         // WM_MOUSEFIRST到WM_MOUSELAST包含了WM_MOUSEWHEEL，因此要在下一个判断之前拦截。
    {
        _wnd_wm_mousewheel(hWnd, pWnd, uMsg, wParam, lParam);
    }
    else if (uMsg >= WM_MOUSEFIRST && uMsg < WM_MOUSELAST)   // 512 - 526
    {
        _wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, lParam);
    }
    else if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)   // 256,264
    {
        _wnd_wm_keyboard(pWnd, hWnd, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_CAPTURECHANGED)   // 533
    {
        _wnd_wm_captionchange(hWnd, pWnd, lParam);
    }
    else if (uMsg == WM_CONTEXTMENU)   // 被动接受消息123
    {
        _wnd_wm_menucontext(hWnd, pWnd, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_SYSCOMMAND || uMsg == WM_COMMAND)   // 274,273
    {
        _wnd_wm_command(hWnd, pWnd, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_ENTERSIZEMOVE)   // 561
    {
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BSIZEMOVING;
    }
    else if (uMsg == WM_EXITSIZEMOVE)   // 562
    {
        pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BSIZEMOVING);
    }
    else if (uMsg == WM_MEASUREITEM)   // 44
    {
        if (_wnd_wm_measureitem_host(pWnd, wParam, lParam)) {
            return 1;
        }
    }
    else if (uMsg == WM_INITDIALOG)   // 272
    {
        if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
            _msgbox_initdialog(hWnd, pWnd, wParam, lParam);
            SetFocus(hWnd);
            return 0;
        }
    }
    else if (uMsg == WM_GETDLGCODE)   // 135
    {
        if (((pWnd->dwFlags_ & EWF_BMODAL) == EWF_BMODAL)) {
            return (DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTALLKEYS | DLGC_WANTMESSAGE |
                    DLGC_HASSETSEL | DLGC_WANTCHARS);
        }
    }
    else if (uMsg == WM_INITMENUPOPUP)   // 279
    {
        if (((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {
            return 0;
        }
        _wnd_wm_initmenupopup(hWnd, pWnd, (HMENU)wParam);
    }
    else if (uMsg == WM_EXITMENULOOP)   // 530
    {
        if (((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {
            return 0;
        }
        pWnd->dwFlags_    = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BMENUINITED);
        size_t objHittest = pWnd->objHittest_;
        _wnd_wm_nchittest(pWnd, hWnd, -1);
        obj_s Obj;
        _wnd_wm_leavecheck(hWnd, pWnd, objHittest, -1, &Obj, lParam, TRUE);
        pWnd->dx_counts_ = 0;
    }
    else if (uMsg == WM_IME_COMPOSITION)   // 271
    {
        _wnd_wm_ime_composition(hWnd, pWnd);
    }
    else if (uMsg == WM_SETTEXT)   // 12
    {
        if (((pWnd->dwStyle_ & WINDOW_STYLE_TITLE) == WINDOW_STYLE_TITLE)) {
            Ex_ObjSendMessage(_obj_getobjfromidorname(pWnd, WINDOW_STYLE_TITLE), uMsg, wParam,
                              lParam);
        }
    }
    else if (uMsg == MENU_MESSAGE_SELECTITEM)   // MENU_MESSAGE_SELECTITEM
    {
        if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT)) {
            POINT pt;
            GetCursorPos(&pt);
            _wnd_wm_mouse(pWnd, hWnd, WM_MOUSEMOVE, 1,
                          MAKELONG(pt.x - pWnd->left_, pt.y - pWnd->top_));
            return 0;
        }
        else {
            LONG_PTR item = 0;
            _wnd_menu_mouse(hWnd, pWnd, WM_MOUSEMOVE, 0, (LONG_PTR*)&wParam);
            if (LODWORD(wParam) == -1) {
                return 0;
            }
            else {

                wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
                if (pMenuHostWnd != 0) {
                    pMenuHostWnd->pMenuPrevWnd_ = pWnd;
                }
            }
        }
    }
    else if (uMsg == 0x1E6)   // MN_CANCELMENUS
    {
        wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
        if (pMenuHostWnd != 0) {
            wnd_s* pMenuTrackWnd = pMenuHostWnd->pMenuTrackWnd_;
            if (pMenuTrackWnd != 0) {
                if (((pMenuTrackWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT)) {
                    _wnd_obj_untrack(pMenuTrackWnd->hWnd_, pMenuTrackWnd, lParam, TRUE);
                    return 0;
                }
            }
        }
    }
    else if (uMsg == 0x1ED)   // MN_BUTTONDOWN
    {
        if (!_wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONDOWN, 1, (LONG_PTR*)&wParam)) {
            return 0;
        }

        if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) != EWF_BTRACKOBJECT)) {
            return 0;
        }
    }
    else if (uMsg == 0x1EF)   // MN_BUTTONUP
    {
        _wnd_menu_mouse(hWnd, pWnd, WM_LBUTTONUP, 0, (LONG_PTR*)&wParam);
        if (pWnd->objTrackPrev_ != pWnd->objHittest_) {
            return 0;
        }
    }
    else if (uMsg == 0x01F1)   // MN_DBLCLK
    {
        return 0;
    }
    else if (uMsg == WM_COPY) {
        if (_wnd_wm_keyboard(pWnd, hWnd, uMsg, wParam, lParam)) {
            return 1;
        }
    }
    else if (uMsg == WM_IME_CHAR) {
        if (_wnd_wm_keyboard(pWnd, hWnd, uMsg, wParam, lParam)) {
            return 1;
        }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

INT _wnd_create(HEXDUI hExDui, wnd_s* pWnd, HWND hWnd, INT dwStyle, HEXTHEME hTheme, LPARAM lParam,
                WinMsgPROC lpfnMsgProc)
{
    ShowWindow(hWnd, 0);
    RECT rcWindow{0};
    GetWindowRect(hWnd, &rcWindow);
    SIZE size;
    size.cx = rcWindow.right - rcWindow.left;
    size.cy = rcWindow.bottom - rcWindow.top;

    if (!FLAGS_CHECK(dwStyle, WINDOW_STYLE_MESSAGEBOX) &&
        !FLAGS_CHECK(dwStyle, WINDOW_STYLE_COMBOWINDOW)) {
        INT offsetX = 0;
        INT offsetY = 0;
        if (Flag_Query(ENGINE_FLAG_DPI_ENABLE)) {
            size.cx = Ex_Scale(size.cx);
            size.cy = Ex_Scale(size.cy);
            offsetX = (size.cx - (rcWindow.right - rcWindow.left)) / 2;
            offsetY = (size.cy - (rcWindow.bottom - rcWindow.top)) / 2;
        }
        MoveWindow(hWnd, rcWindow.left - offsetX, rcWindow.top - offsetY, size.cx, size.cy, FALSE);
    }
    HWND hWndParent = 0;
    if ((dwStyle & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX) {
        _wnd_getfromhandle(((mbp_s*)lParam)->handle_, &hWndParent);
    }
    else {

        hWndParent = (HWND)GetWindowLongPtrW(hWnd, GWLP_HWNDPARENT);
    }
    if ((dwStyle & WINDOW_STYLE_CENTERWINDOW) == WINDOW_STYLE_CENTERWINDOW) {
        Ex_WndCenterFrom(hWnd, hWndParent, (dwStyle & WINDOW_STYLE_FULLSCREEN) != 0);
    }

    GetWindowRect(hWnd, &rcWindow);
    size.cx     = rcWindow.right - rcWindow.left;
    size.cy     = rcWindow.bottom - rcWindow.top;
    INT dwFlags = 0;

    if (_wnd_querystyle(hWnd, WS_EX_LAYERED, TRUE) ||
        ((dwStyle & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {

        dwFlags = EWF_BLAYERED;
    }

    if (hWndParent != 0) {
        pWnd->hExDuiParent_ = Ex_DUIFromWindow(hWndParent);
    }
    auto atomName = GetClassLongPtrW(hWnd, GCW_ATOM);
    if (atomName == ATOM_DIALOG) {
        dwFlags = dwFlags | EWF_BMODAL;
        dwFlags = dwFlags | EWF_ACTIVE;
    }
    if (hTheme == 0) hTheme = g_Li.hThemeDefault;

    pWnd->dwStyle_       = dwStyle;
    pWnd->hImc_          = ImmGetContext(hWnd);
    pWnd->hWnd_          = hWnd;
    pWnd->hexdui_        = hExDui;
    pWnd->hTheme_        = hTheme;
    pWnd->width_         = size.cx;
    pWnd->height_        = size.cy;
    pWnd->alpha_         = 255;
    pWnd->dwFlags_       = dwFlags | EWF_BLEAVESENT;
    pWnd->pfnMsgProc_    = lpfnMsgProc;
    pWnd->hTableObjects_ = HashTable_Create(GetNearestPrime(256), NULL);

    pWnd->hTableEvent_ = HashTable_Create(17, pfnDefaultFreeData);

    if ((dwStyle & WINDOW_STYLE_MESSAGEBOX) != 0) {
        pWnd->lpMsgParams_ = (mbp_s*)lParam;
    }
    else {
        pWnd->lParam_ = lParam;
    }

    // layer
    pWnd->ulwi_.pptDst   = (POINT*)((size_t)pWnd + offsetof(wnd_s, left_));
    pWnd->ulwi_.psize    = (SIZE*)((size_t)pWnd + offsetof(wnd_s, width_));
    pWnd->ulwi_.pptSrc   = (POINT*)((size_t)pWnd + offsetof(wnd_s, ulwi_pptSrc_x_));
    pWnd->ulwi_.pblend   = (BLENDFUNCTION*)((size_t)pWnd + offsetof(wnd_s, ulwi_pblend_));
    pWnd->ulwi_.prcDirty = (RECT*)((size_t)pWnd + offsetof(wnd_s, ulwi_prcDirty_left_));

    pWnd->ulwi_.cbSize                     = sizeof(pWnd->ulwi_);
    pWnd->ulwi_.dwFlags                    = 2;
    pWnd->ulwi_pblend_.AlphaFormat         = 1;
    pWnd->ulwi_pblend_.BlendFlags          = 0;
    pWnd->ulwi_pblend_.BlendOp             = 0;
    pWnd->ulwi_pblend_.SourceConstantAlpha = 0xFF;
    pWnd->left_                            = rcWindow.left;
    pWnd->top_                             = rcWindow.top;
    pWnd->width_                           = size.cx;
    pWnd->height_                          = size.cy;
    pWnd->ulwi_prcDirty_right_             = size.cx;
    pWnd->ulwi_prcDirty_bottom_            = size.cy;
    INT nError                             = 0;
    // sysshadow,有阴影才创建阴影窗口
    if (!((pWnd->dwStyle_ & WINDOW_STYLE_NOSHADOW) == WINDOW_STYLE_NOSHADOW)) {
        auto style = GetClassLongPtrW(hWnd, GCL_STYLE);
        if ((style & CS_DROPSHADOW) != 0) {
            SetClassLongPtrW(hWnd, GCL_STYLE, (style & ~CS_DROPSHADOW));
        }
        INT exStyle =
            WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_TOPMOST;
        HWND hwndShadow   = CreateWindowExW(exStyle, (LPCWSTR)g_Li.atomSysShadow, 0, WS_POPUP, 0, 0,
                                            0, 0, hWnd, 0, 0, 0);
        pWnd->hWndShadow_ = hwndShadow;
        SetWindowSubclass(hwndShadow, _wnd_shadow_proc, 0, 0);
    }

    // tips
    HWND hWndTips = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                                    L"tooltips_class32", 0, WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
    SendMessageW(hWndTips, 1048, 0, 2048);   // TTM_SETMAXTIPWIDTH 支持多行
    pWnd->hWndTips_ = hWndTips;
    nError          = 0;
    ti_s* toolauto =
        (ti_s*)_struct_createfromaddr(pWnd, offsetof(wnd_s, ti_auto_), sizeof(ti_s), &nError);

    toolauto->cbSize_ = sizeof(ti_s);
    toolauto->uFlags_ = 16 | 256 | 4096;   // TTF_SUBCLASS,TTF_TRANSPARENT,TTF_PARSELINKS
    toolauto->hWnd_   = hWnd;
    toolauto->uId_    = toolauto;
    SendMessageW(hWndTips, 1074, 0, (LPARAM)toolauto);   // TTM_ADDTOOLW
    nError = 0;
    ti_s* tooltrack =
        (ti_s*)_struct_createfromaddr(pWnd, offsetof(wnd_s, ti_track_), sizeof(ti_s), &nError);

    tooltrack->cbSize_ = sizeof(ti_s);
    tooltrack->uFlags_ =
        32 | 128 | 256 | 4096;   // TTF_TRACK,TTF_ABSOLUTE,TTF_TRANSPARENT,TTF_PARSELINKS
    tooltrack->hWnd_ = hWnd;
    tooltrack->uId_  = tooltrack;
    SendMessageW(hWndTips, 1074, 0, (LPARAM)tooltrack);   // TTM_ADDTOOLW
    nError = 0;
    SetWindowSubclass(hWndTips, _wnd_tooltips_proc, 0, (DWORD_PTR)pWnd);
    // 初始化渲染,以及背景
    _wnd_dx_init(pWnd);
    INT w1, h1;
    w1     = rcWindow.right - rcWindow.left;
    h1     = rcWindow.bottom - rcWindow.top;
    nError = 0;
    pWnd->canvas_display_ =
        _canvas_createfrompwnd(pWnd, w1, h1, CANVAS_FLAG_GDI_COMPATIBLE, &nError);

    nError            = 0;
    pWnd->canvas_bkg_ = _canvas_createfrompwnd(pWnd, w1, h1, 0, &nError);

    if (nError == 0) {
        // 加载主题，并确定客户区

        _wnd_loadtheme(pWnd, hWnd, hTheme);

        if ((dwStyle & WINDOW_STYLE_MENU) == 0) {
            _wnd_sysbutton_create(hWnd, pWnd, dwStyle);
        }

        _wnd_addstyle(hWnd, WS_THICKFRAME, FALSE);   // 强制触发样式被修改事件
        SetWindowSubclass(hWnd, _wnd_proc, 0, (DWORD_PTR)pWnd);
        _wnd_delstyle(hWnd, WS_THICKFRAME, FALSE);
        IME_Control(hWnd, pWnd, FALSE);

        if (FLAGS_CHECK(dwStyle, WINDOW_STYLE_POPUPWINDOW)) {
            pWnd->lpPopupParams_ = lParam;
            if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (LPVOID*)&pWnd, &nError)) {
                HWND old         = pWnd->hWndPopup_;
                pWnd->hWndPopup_ = hWnd;
                DestroyWindow(old);
                FLAGS_ADD(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN);
                obj_s* pObj;
                if (_handle_validate(lParam, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                    _obj_baseproc(hWndParent, lParam, pObj, WM_EX_INITPOPUP, hExDui, 0);
                    IME_Control(hWndParent, pWnd, TRUE);
                }
            }
            nError = 0;
        }
    }

    return nError;
}

LRESULT CALLBACK _wnd_tooltips_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    wnd_s* pWnd = (wnd_s*)dwRefData;
    if (uMsg == WM_DESTROY) {
        RemoveWindowSubclass(hWnd, _wnd_tooltips_proc, 0);
    }
    else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN) {
        SendMessageW(hWnd, 1041, 0, (size_t)pWnd + offsetof(wnd_s, ti_track_));
        return 0;
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK _wnd_shadow_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if (uMsg == WM_DESTROY) {
        RemoveWindowSubclass(hWnd, _wnd_shadow_proc, 0);
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void _wnd_dx_unint(wnd_s* pWnd)
{
    ID2D1DeviceContext* pDeviceContext = pWnd->dx_context_;

    if (pDeviceContext) {
        pDeviceContext->Release();
    }
    ID2D1GdiInteropRenderTarget* pgdiinterop = pWnd->dx_gdiinterop_;
    if (pgdiinterop) {
        pgdiinterop->Release();
    }
}

void _wnd_dx_init(wnd_s* pWnd)
{
    _wnd_dx_unint(pWnd);
    ID2D1DeviceContext* pDeviceContext = nullptr;
    if (g_Ri.pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pDeviceContext) ==
        0) {
        pDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
        pWnd->dx_context_    = pDeviceContext;
        pWnd->dx_gdiinterop_ = _dx_get_gdiInterop(pDeviceContext);
    }
}

HEXOBJ _wnd_wm_nchittest_obj(HWND hWnd, wnd_s* pWnd, HEXOBJ objLast, INT x, INT y, INT* hitCode,
                             obj_s** pObjMouse)
{
    HEXOBJ objPrev = objLast;
    obj_s* pObj    = nullptr;
    INT    nError  = 0;
    while (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

        if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
            RECT  rt = {pObj->left_, pObj->top_, pObj->right_, pObj->bottom_};
            POINT po = {x, y};
            if (PtInRect(&rt, po)) {
                INT  ox     = x - pObj->left_;
                INT  oy     = y - pObj->top_;
                auto lParam = MAKELONG(ox, oy);

                if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) ==
                     OBJECT_STYLE_DISABLED))   // 检测是否被禁止，被禁止组件还需检测是否可穿透
                {
                    *hitCode = _obj_baseproc(hWnd, objPrev, pObj, WM_NCHITTEST, 0, lParam);
                    if (*hitCode == HTTRANSPARENT)   // 在穿透区域,则继续寻找
                    {
                        objPrev = pObj->objPrev_;
                        continue;
                    }
                    else {
                        *hitCode   = HTCLIENT;   // 不可穿透则直接停止寻找
                        *pObjMouse = pObj;
                        return objPrev;
                    }
                }
                else {
                    objLast = pObj->objChildLast_;
                    if (objLast != 0) {
                        auto ret =
                            _wnd_wm_nchittest_obj(hWnd, pWnd, objLast, ox, oy, hitCode, pObjMouse);
                        if (ret != 0 && *hitCode != HTTRANSPARENT)   // 找到目标子组件
                        {
                            return ret;
                        }
                    }
                }
                *hitCode = _obj_baseproc(hWnd, objPrev, pObj, WM_NCHITTEST, 0, lParam);
                if (*hitCode == HTTRANSPARENT)   // 如果可穿透，则继续寻找
                {
                    objPrev = pObj->objPrev_;
                    continue;
                }
                else {
                    pWnd->dwHitObjPos_Abs_ = lParam;
                    *pObjMouse             = pObj;
                    return objPrev;
                }
            }
        }
        objPrev = pObj->objPrev_;
    }
    return 0;
}

INT _wnd_wm_nchittest_sizebox(INT width, INT height, INT x, INT y)
{
    INT ret     = 0;
    INT nOffset = 5 * 2;
    if (y < nOffset)   // 鼠标在顶部
    {
        if (x < nOffset)   // 鼠标在左边
        {
            ret = HTTOPLEFT;
        }
        else if (x > width - nOffset)   // 鼠标在右边
        {

            ret = HTTOPRIGHT;
        }
        else {
            ret = HTTOP;
        }
    }
    else {
        if (y > height - nOffset)   // 鼠标在底部
        {
            if (x < nOffset)   // 鼠标在左边
            {
                ret = HTBOTTOMLEFT;
            }
            else if (x > width - nOffset)   // 鼠标在右边
            {
                ret = HTBOTTOMRIGHT;
            }
            else {
                ret = HTBOTTOM;
            }
        }
        else {
            if (x < nOffset)   // 鼠标在左边
            {
                if (y < nOffset)   // 鼠标在上面
                {
                    ret = HTTOPLEFT;
                }
                else {
                    if (y > height - nOffset)   // 鼠标在下面
                    {
                        ret = HTBOTTOMLEFT;
                    }
                    else {
                        ret = HTLEFT;
                    }
                }
            }
            else {
                // 鼠标在右边
                if (y < nOffset)   // 鼠标在上面
                {
                    ret = HTTOPRIGHT;
                }
                else {
                    if (y > height - nOffset)   // 鼠标在下面
                    {
                        ret = HTBOTTOMRIGHT;
                    }
                    else {
                        ret = HTRIGHT;
                    }
                }
            }
        }
    }
    return ret;
}

INT _wnd_wm_nchittest(wnd_s* pWnd, HWND hWnd, LPARAM lParam)
{
    INT dwHitCode = HTCLIENT;

    if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT)) return dwHitCode;

    if (!((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {

        if (((pWnd->dwFlags_ & EWF_BMENUINITED) == EWF_BMENUINITED)) {
            return dwHitCode;
        }
    }

    POINT  pt;
    HEXOBJ objMouse = 0;
    if (lParam == -1) {
        GetCursorPos(&pt);
    }
    else {
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
    }
    ScreenToClient(hWnd, &pt);
    if (_rgn_hittest(pWnd->hrgn_client_, pt.x, pt.y) || pWnd->dwWinState_ == 2) {
        obj_s* pobjMouse = nullptr;
        objMouse = _wnd_wm_nchittest_obj(hWnd, pWnd, pWnd->objChildLast_, pt.x, pt.y, &dwHitCode,
                                         &pobjMouse);
        if (objMouse == 0) {
            pobjMouse = 0;

            if (((pWnd->dwStyle_ & WINDOW_STYLE_MOVEABLE) ==
                 WINDOW_STYLE_MOVEABLE))   // 允许随意移动
            {
                dwHitCode = HTCAPTION;
            }
        }
        else {
            pWnd->dwHitCode_ = dwHitCode;
            if (objMouse == pWnd->objCaption_) {
                dwHitCode = HTCAPTION;
            }
            else {

                if (((pobjMouse->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED)) {

                    if (((pWnd->dwStyle_ & WINDOW_STYLE_MOVEABLE) ==
                         WINDOW_STYLE_MOVEABLE))   // 允许随意移动
                    {
                        dwHitCode = HTCAPTION;
                    }
                    objMouse = 0;
                }
            }
        }
    }
    else {
        // 在尺寸边框内

        if (((pWnd->dwStyle_ & WINDOW_STYLE_SIZEABLE) == WINDOW_STYLE_SIZEABLE))   // 允许调整尺寸
        {
            dwHitCode = _wnd_wm_nchittest_sizebox(pWnd->width_, pWnd->height_, pt.x, pt.y);
        }
        else {
            dwHitCode = HTCAPTION;
        }
    }

    auto objHittest   = pWnd->objHittest_;
    pWnd->objHittest_ = objMouse;
    if (objHittest != objMouse) {
        pWnd->objHittestPrev_ = objHittest;
        dwHitCode             = HTCLIENT;
    }
    return dwHitCode;
}

void _wnd_sysbutton_create(HWND hWnd, wnd_s* pWnd, INT dwStyle)
{
    HEXTHEME hTheme    = pWnd->hTheme_;
    RECT     rcCaption = {0};
    _wnd_calc_captionrect(pWnd, &rcCaption);
    obj_s* pObjCaption = nullptr;
    INT    nError      = 0;
    HEXOBJ objCaption  = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjCaption, &nError);

    INT nMinHeight = 0;
    if (objCaption != 0) {
        std::wstring title;
        LPCWSTR      lpTitle = nullptr;

        if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
            lpTitle = pWnd->lpMsgParams_->lpCaption_;
        }
        else {
            title   = _wnd_gettitle(hWnd);
            lpTitle = title.data();
        }

        _obj_create_proc(&nError, FALSE, hTheme, pObjCaption,
                         (dwStyle & WINDOW_STYLE_NOCAPTIONTOPMOST) != 0 ? 0 : -1, ATOM_SYSBUTTON,
                         lpTitle, OBJECT_STYLE_VISIBLE | WINDOW_STYLE_TITLE, rcCaption.left,
                         rcCaption.top, rcCaption.right - rcCaption.left,
                         rcCaption.bottom - rcCaption.top, 0, WINDOW_STYLE_TITLE, 0, 0,
                         DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        LPVOID lpPT = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_PADDING_TEXT);
        if (lpPT != 0) {
            RtlMoveMemory((LPVOID)((size_t)pObjCaption + offsetof(obj_s, t_left_)), lpPT, 16);

            if (g_Li.DpiX > 1) {
                pObjCaption->t_left_   = pObjCaption->t_left_ * g_Li.DpiX;
                pObjCaption->t_top_    = pObjCaption->t_top_ * g_Li.DpiX;
                pObjCaption->t_right_  = pObjCaption->t_right_ * g_Li.DpiX;
                pObjCaption->t_bottom_ = pObjCaption->t_bottom_ * g_Li.DpiX;
            }
        }
        LPVOID lpCr = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_COLOR_NORMAL);
        if (lpCr != 0) {
            pObjCaption->crNormal_ = __get_int(lpCr, 0);
        }
        LPVOID pFamily = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_FAMILY);
        LPVOID pSize   = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_SIZE);
        LPVOID pStyle  = Ex_ThemeGetValuePtr(hTheme, ATOM_WINDOW, ATOM_FONT_STYLE);

        INT Size, Style;
        if (pFamily == 0) pFamily = (LPVOID)-1;
        if (pSize == 0) {
            Size = -1;
        }
        else {
            Size = __get_int(pSize, 0);
        }
        if (pStyle == 0) {
            Style = -1;
        }
        else {
            Style = __get_int(pStyle, 0);
        }
        if ((size_t)pFamily != -1 || Size != -1 || Style != -1) {
            HEXFONT hfont = pObjCaption->hFont_;

            pObjCaption->hFont_ = _font_createfromfamily((LPWSTR)pFamily, Size, Style);

            if (hfont != 0) _font_destroy(hfont);
        }

        _obj_create_done(hWnd, pWnd, objCaption, pObjCaption);
        pWnd->objCaption_ = objCaption;
        nMinHeight        = rcCaption.bottom + rcCaption.top;
    }
    std::vector<INT> aryBtn    = {WINDOW_STYLE_BUTTON_CLOSE, WINDOW_STYLE_BUTTON_MAX,
                                  WINDOW_STYLE_BUTTON_MIN,   WINDOW_STYLE_BUTTON_MENU,
                                  WINDOW_STYLE_BUTTON_SKIN,  WINDOW_STYLE_BUTTON_SETTING,
                                  WINDOW_STYLE_BUTTON_HELP};
    std::vector<INT> aryAtom   = {ATOM_SYSBUTTON_CLOSE, ATOM_SYSBUTTON_MAX, ATOM_SYSBUTTON_MIN,
                                  ATOM_SYSBUTTON_MENU,  ATOM_SYSBUTTON_SKN, ATOM_SYSBUTTON_SETTING,
                                  ATOM_SYSBUTTON_HELP};
    INT              left      = rcCaption.right - rcCaption.left;
    INT              nMinWidth = 0;

    for (INT i = 0; i < 7; i++) {
        if ((dwStyle & aryBtn[i]) != 0) {
            LPVOID lpValuea = Ex_ThemeGetValuePtr(hTheme, aryAtom[i], ATOM_NORMAL);
            if (lpValuea != 0) {

                RECT rcObject{0};
                RtlMoveMemory(&rcObject, lpValuea, 16);
                rcObject.right  = Ex_Scale(rcObject.right - rcObject.left);
                rcObject.bottom = Ex_Scale(rcObject.bottom - rcObject.top);

                left           = left - rcObject.right;
                obj_s* pObjTmp = nullptr;
                INT    nError  = 0;
                HEXOBJ objTmp  = _obj_create_init(hWnd, pWnd, ATOM_SYSBUTTON, 0, &pObjTmp, &nError);
                if (objTmp != 0) {

                    _obj_create_proc(&nError, FALSE, hTheme, pObjTmp, -1, ATOM_SYSBUTTON, 0,
                                     OBJECT_STYLE_VISIBLE | aryBtn[i], left, 0, rcObject.right,
                                     rcObject.bottom, objCaption, aryBtn[i], 0, 0, 0);
                    _obj_create_done(hWnd, pWnd, objTmp, pObjTmp);
                    nMinWidth = nMinWidth + rcObject.right;
                }
            }
        }
    }
    pObjCaption->t_right_ = pObjCaption->t_right_ + nMinWidth;
    nMinWidth             = nMinWidth + rcCaption.left + pWnd->width_ - rcCaption.right;

    if (((pWnd->dwStyle_ & WINDOW_STYLE_HASICON) == WINDOW_STYLE_HASICON)) {
        nMinWidth = nMinWidth + Ex_Scale(16);
    }
    if (pWnd->minWidth_ < nMinWidth) {
        pWnd->minWidth_ = nMinWidth;
    }
    if (pWnd->minHeight_ < nMinHeight) {
        pWnd->minHeight_ = nMinHeight;
    }
}

void _wnd_render_obj(HWND hWnd, wnd_s* pWnd, ID2D1DeviceContext* pContext, HEXCANVAS cvDisplay,
                     LPVOID pBitmapDisplay, RECT rcPaint, EXHANDLE objChildFirst, INT offsetX,
                     INT offsetY, INT pAlpha, BOOL fDX, LPVOID hBorderBrush)
{
    EXHANDLE objNext = objChildFirst;
    INT      nError  = 0;
    if (objNext != 0) {
        do {
            obj_s* pObj = nullptr;
            if (_handle_validate(objNext, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

                if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
                    RECT rcObj{0};
                    rcObj.left   = pObj->left_;
                    rcObj.top    = pObj->top_;
                    rcObj.right  = pObj->right_;
                    rcObj.bottom = pObj->bottom_;

                    OffsetRect(&rcObj, offsetX, offsetY);
                    RECT rcClip{0};
                    if (IntersectRect(&rcClip, &rcPaint, &rcObj)) {
                        auto fPage = (pObj->dwFlags_ & EOF_BPAGE) != EOF_BPAGE;
                        if (fPage) {
                            if (((pObj->dwFlags_ & EOF_BCANREDRAW) == EOF_BCANREDRAW)) {
                                if (((pObj->dwFlags_ & EOF_BNEEDREDRAW) == EOF_BNEEDREDRAW)) {
                                    pObj->dwFlags_ =
                                        pObj->dwFlags_ - (pObj->dwFlags_ & EOF_BNEEDREDRAW);
                                    if (!IsRectEmpty(
                                            (RECT*)((size_t)pObj + offsetof(obj_s, d_left_)))) {
                                        _obj_baseproc(hWnd, objNext, pObj, WM_PAINT, 0, 0);
                                        if (fDX) {
                                            _dx_settarget(pContext, (ID2D1Bitmap*)pBitmapDisplay);
                                        }
                                    }
                                }
                            }
                        }
                        // 组件透明度是否需要被窗口透明度影响？？
                        INT alpha = pObj->dwAlpha_ * pWnd->alpha_ / 255;
                        alpha     = alpha * pAlpha / 255;

                        if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED)) {
                            alpha = alpha * pObj->dwAlphaDisable_ / 255;
                        }
                        if (fPage)   // 非页面
                        {

                            if (((pObj->dwFlags_ & EOF_BPATH) == EOF_BPATH)) {
                                HEXBRUSH hPathBrush =
                                    _brush_createfromcanvas2(pObj->canvas_obj_, alpha);
                                if (hPathBrush != 0) {
                                    HEXMATRIX matrix = _matrix_create();
                                    _matrix_translate(matrix, pObj->w_left_, pObj->w_top_);
                                    _brush_settransform(hPathBrush, matrix);
                                    _canvas_fillpath(cvDisplay, pObj->hPath_Window_, hPathBrush);
                                    _brush_destroy(hPathBrush);
                                    _matrix_destroy(matrix);
                                }
                            }
                            else {

                                if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_COMPOSITED) ==
                                     OBJECT_STYLE_EX_COMPOSITED)) {
                                    _canvas_bitblt(cvDisplay, pObj->canvas_obj_, rcClip.left,
                                                   rcClip.top, rcClip.right, rcClip.bottom,
                                                   rcClip.left - rcObj.left,
                                                   rcClip.top - rcObj.top);
                                }
                                else {
                                    _canvas_alphablend(
                                        cvDisplay, pObj->canvas_obj_, rcClip.left, rcClip.top,
                                        rcClip.right, rcClip.bottom, rcClip.left - rcObj.left,
                                        rcClip.top - rcObj.top, rcClip.right - rcObj.left,
                                        rcClip.bottom - rcObj.top, alpha);
                                }
                            }
                        }
                        if (hBorderBrush != 0) {
                            _canvas_drawrect(cvDisplay, hBorderBrush, pObj->w_left_, pObj->w_top_,
                                             pObj->w_right_, pObj->w_bottom_, 1,
                                             D2D1_DASH_STYLE_SOLID);
                        }

                        HEXOBJ objChild = pObj->objChildFirst_;
                        if (objChild != 0) {
                            _wnd_render_obj(hWnd, pWnd, pContext, cvDisplay, pBitmapDisplay, rcClip,
                                            objChild, rcObj.left, rcObj.top, alpha, fDX,
                                            hBorderBrush);
                        }
                    }
                }
            }
            else {
                break;
            }
            objNext = pObj->objNext_;
        } while (objNext != 0);
    }
}

BOOL _wnd_wm_setcursor(HWND hWnd, wnd_s* pWnd, LPARAM lParam)
{
    auto uHitCode = LOWORD(lParam);
    auto hCursor  = pWnd->hCursor_;
    if (uHitCode == HTCLIENT) {
        auto   objHittest = pWnd->objHittest_;
        obj_s* pObj       = nullptr;
        INT    nError     = 0;
        if (_handle_validate(objHittest, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            if (_obj_baseproc(hWnd, objHittest, pObj, WM_SETCURSOR, pWnd->dwHitObjPos_Abs_,
                              MAKELONG(pWnd->dwHitCode_, HIWORD(lParam))) != 0) {
                return TRUE;
            }
        }
    }
    else if (uHitCode == HTCAPTION) {
        hCursor = g_Li.hCursor;
    }
    else if (uHitCode == HTTOPLEFT) {
        hCursor = LoadCursorW(0, IDC_SIZENWSE);
    }
    else if (uHitCode == HTTOPRIGHT) {
        hCursor = LoadCursorW(0, IDC_SIZENESW);
    }
    else if (uHitCode == HTTOP) {
        hCursor = LoadCursorW(0, IDC_SIZENS);
    }
    else if (uHitCode == HTBOTTOMLEFT) {
        hCursor = LoadCursorW(0, IDC_SIZENESW);
    }
    else if (uHitCode == HTBOTTOMRIGHT) {
        hCursor = LoadCursorW(0, IDC_SIZENWSE);
    }
    else if (uHitCode == HTBOTTOM) {
        hCursor = LoadCursorW(0, IDC_SIZENS);
    }
    else if (uHitCode == HTTOPLEFT) {
        hCursor = LoadCursorW(0, IDC_SIZENWSE);
    }
    else if (uHitCode == HTBOTTOMLEFT) {
        hCursor = LoadCursorW(0, IDC_SIZENESW);
    }
    else if (uHitCode == HTLEFT) {
        hCursor = LoadCursorW(0, IDC_SIZEWE);
    }
    else if (uHitCode == HTTOPRIGHT) {
        hCursor = LoadCursorW(0, IDC_SIZENESW);
    }
    else if (uHitCode == HTBOTTOMRIGHT) {
        hCursor = LoadCursorW(0, IDC_SIZENWSE);
    }
    else if (uHitCode == HTRIGHT) {
        hCursor = LoadCursorW(0, IDC_SIZEWE);
    }

    if (hCursor != 0) {
        SetCursor((HCURSOR)hCursor);
        return TRUE;
    }
    return FALSE;
}

void CALLBACK _wnd_timer_mousetrack(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    KillTimer(hWnd, idEvent);
    wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_MOUSETRACK);

    if ((pWnd->dwFlags_ & EWF_BTRACKOBJECT) != EWF_BTRACKOBJECT) {
        POINT pt;
        GetCursorPos(&pt);
        if (WindowFromPoint(pt) != hWnd) {
            if ((pWnd->dwFlags_ & EWF_BLEAVESENT) != EWF_BLEAVESENT) {
                pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BLEAVESENT;
                HEXOBJ hObj    = pWnd->objHittestPrev_;
                obj_s* pObj    = nullptr;
                INT    nError  = 0;
                if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                    _obj_baseproc(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0);   // 发送离开
                    pWnd->objHittestPrev_ = pWnd->objHittest_;
                    pWnd->objHittest_     = 0;
                }
            }
        }
    }
}

void _wnd_wm_leavecheck(HWND hWnd, wnd_s* pWnd, HEXOBJ objCheck, HEXOBJ objHittest,
                        obj_s* pObjHittest, LPARAM lParam, BOOL fTrack)
{
    INT nError = 0;
    if (objHittest == -1) {
        objHittest = pWnd->objHittest_;
        _handle_validate(objHittest, HT_OBJECT, (LPVOID*)&pObjHittest, &nError);
    }
    if (objCheck != objHittest) {
        nError           = 0;
        obj_s* pObjCheck = nullptr;
        if (_handle_validate(objCheck, HT_OBJECT, (LPVOID*)&pObjCheck, &nError)) {

            if ((pWnd->dwFlags_ & EWF_BLEAVESENT) != EWF_BLEAVESENT) {
                pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BLEAVESENT;
                _obj_baseproc(hWnd, objCheck, pObjCheck, WM_MOUSELEAVE, 0, lParam);
            }
        }
        if (objHittest != 0) {
            pWnd->dwFlags_        = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BLEAVESENT);
            pWnd->objHittestPrev_ = objHittest;
            _obj_baseproc(hWnd, objHittest, pObjHittest, WM_MOUSEHOVER, 0, lParam);
        }
    }

    if (objHittest != 0 && fTrack) {
        size_t nEvent = (size_t)pWnd + TIMER_MOUSETRACK;
        KillTimer(hWnd, nEvent);
        SetTimer(hWnd, nEvent, 50, _wnd_timer_mousetrack);
    }
}

INT _wnd_destroy(HWND hWnd, wnd_s* pWnd)
{
    obj_s* pObj    = nullptr;
    INT    nError  = 0;
    pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BDESTROYWINDOW;

    if (FLAGS_CHECK(pWnd->dwStyle_, WINDOW_STYLE_POPUPWINDOW)) {
        wnd_s* pWndParent;
        if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (LPVOID*)&pWndParent, &nError)) {
            FLAGS_DEL(pWndParent->dwFlags_, EWF_BPOPUPWINDOIWSHOWN);
            pWndParent->hWndPopup_ = 0;

            if (_handle_validate(pWnd->lpPopupParams_, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                _obj_baseproc(pWndParent->hWnd_, pWnd->lpPopupParams_, pObj, -7, 0, 0);
                IME_Control(pWndParent->hWnd_, pWndParent, FALSE);
            }
        }
        nError = 0;
        pObj   = NULL;
    }

    // clear MESSAGEBOX
    if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
        if (pWnd->lpMsgParams_ != 0) {
            if (pWnd->lpMsgParams_->lpCheckBox_ != 0) {
                if (pWnd->lpMsgParams_->lpCheckBoxChecked_) {
                    __set_int(pWnd->lpMsgParams_->lpCheckBoxChecked_, 0,
                              Ex_ObjDispatchMessage(pWnd->lpMsgParams_->CheckBoxhObj_, BM_GETCHECK,
                                                    0, 0));
                }
            }
        }
    }

    DestroyWindow(pWnd->hWndTips_);
    DestroyWindow(pWnd->hWndShadow_);

    if (_handle_validate(pWnd->objChildFirst_, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_notify_brothers(hWnd, pWnd->objChildFirst_, pObj, WM_DESTROY, 0, 0, FALSE, FALSE);
    }

    // clear NotifyIcon
    if (pWnd->lpNid_) {
        Shell_NotifyIconW(NIM_DELETE, (PNOTIFYICONDATAW)pWnd->lpNid_);
        Ex_MemFree(pWnd->lpNid_);
        pWnd->lpNid_ = 0;
    }

    // clear DropTarget
    if (pWnd->lpIDropTarget_) {
        ((DropTarget*)pWnd->lpIDropTarget_)->Release();
        delete (DropTarget*)pWnd->lpIDropTarget_;
        pWnd->lpIDropTarget_ = nullptr;
    }

    HashTable_Destroy(pWnd->hTableObjects_);
    HashTable_Destroy(pWnd->hTableEvent_);

    // clear bkgimginfo
    KillTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT);
    _obj_backgroundimage_clear(hWnd, (obj_base*)pWnd);

    // clear font
    _font_destroy(pWnd->hFont_Menu_);

    BOOL bMainWnd = ((pWnd->dwStyle_ & WINDOW_STYLE_MAINWINDOW) == WINDOW_STYLE_MAINWINDOW);
    _struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_auto_));
    _struct_destroyfromaddr(pWnd, offsetof(wnd_s, ti_track_));
    _rgn_destroy(pWnd->hrgn_client_);
    _rgn_destroy(pWnd->hrgn_sizebox_);
    _canvas_destroy(pWnd->canvas_bkg_);
    _canvas_destroy(pWnd->canvas_display_);

    if (pWnd->hLayout_ != 0) {
        _layout_destroy(pWnd->hLayout_);
    }

    _wnd_dx_unint(pWnd);
    HEXDUI hExDui = pWnd->hexdui_;
    nError        = 0;
    _handle_destroy(hExDui, &nError);
    Ex_MemFree(pWnd);
    Ex_SetLastError(nError);
    return bMainWnd ? 1 : 0;
}

void _wnd_paint_bkg(HWND hWnd, wnd_s* pWnd)
{
    HEXCANVAS cvBkg = pWnd->canvas_bkg_;
    wnd_s*    p     = pWnd;
    if (_canvas_begindraw(cvBkg)) {
        _canvas_setantialias(cvBkg, TRUE);
        _canvas_setimageantialias(cvBkg, TRUE);
        LRESULT result = 0;
        if (!pWnd->pfnMsgProc_ ||
            !pWnd->pfnMsgProc_(hWnd, pWnd->hexdui_, WM_ERASEBKGND, cvBkg,
                               MAKELONG(pWnd->width_, pWnd->height_), &result)) {
            if (!((pWnd->dwStyle_ & WINDOW_STYLE_NOINHERITBKG) == WINDOW_STYLE_NOINHERITBKG)) {
                wnd_s* pParent = nullptr;
                INT    nError  = 0;
                if (_handle_validate(pWnd->hExDuiParent_, HT_DUI, (LPVOID*)&pParent, &nError)) {
                    p = pParent;
                }

                if ((pWnd->dwFlags_ & EWF_BINHERITBKGSTARTED) != EWF_BINHERITBKGSTARTED) {
                    pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BINHERITBKGSTARTED;
                    SetTimer(hWnd, (size_t)pWnd + TIMER_BKG_INHERIT, 100,
                             &_wnd_backgroundimage_timer_inherit);
                }
            }
            // 绘制底色

            _canvas_clear(cvBkg, p->crBkg_);

            // 绘制背景
            if (p->lpBackgroundImage_) {
                _canvas_drawimagefrombkgimg(cvBkg, p->lpBackgroundImage_);
            }
        }

        if (pWnd->fBlur_) {
            _canvas_blur(cvBkg, pWnd->fBlur_, 0);
        }

        if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
            _msgbox_drawinfo(pWnd, cvBkg);
        }
        if (FLAGS_CHECK(pWnd->dwStyle_, WINDOW_STYLE_COMBOWINDOW)) {
            Ex_ThemeDrawControl(pWnd->hTheme_, cvBkg, 0, 0, pWnd->width_ - 1, pWnd->height_ - 1,
                                -1741080004, ATOM_NORMAL, 255);
        }
    }
    _canvas_enddraw(cvBkg);
    _obj_compostied_all(pWnd->objChildFirst_);
}

void _wnd_render_dc(HWND hWnd, wnd_s* pWnd, LPVOID hDC, HEXCANVAS cvDisplay, RECT rcPaint,
                    BOOL fLayer)
{
    LPVOID mDC = _canvas_getdc(cvDisplay);
    if (mDC != 0) {
        if (fLayer) {
            pWnd->ulwi_prcDirty_left_   = rcPaint.left;
            pWnd->ulwi_prcDirty_top_    = rcPaint.top;
            pWnd->ulwi_prcDirty_right_  = rcPaint.right;
            pWnd->ulwi_prcDirty_bottom_ = rcPaint.bottom;

            pWnd->ulwi_.hdcSrc = (HDC)mDC;
            if (g_Li.pfnUpdateLayeredWindowIndirect) {
                ((UpdateLayeredWindowIndirectPROC)g_Li.pfnUpdateLayeredWindowIndirect)(
                    hWnd, &pWnd->ulwi_);
            }
            else {
                BLENDFUNCTION pb_;
                pb_.BlendOp             = 0;
                pb_.BlendFlags          = 0;
                pb_.SourceConstantAlpha = 0xFF;
                pb_.AlphaFormat         = 1;
                UpdateLayeredWindow(hWnd, 0, 0, (SIZE*)pWnd->ulwi_.psize, pWnd->ulwi_.hdcSrc,
                                    (POINT*)pWnd->ulwi_.pptSrc, 0, &pb_, ULW_ALPHA);
            }
        }
        else {
            BitBlt((HDC)hDC, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left,
                   rcPaint.bottom - rcPaint.top, (HDC)mDC, rcPaint.left, rcPaint.top, SRCCOPY);
        }
        _canvas_releasedc(cvDisplay);
    }
}

void _wnd_render(HWND hWnd, wnd_s* pWnd, LPVOID hDC, RECT rcPaint, BOOL fLayer, BOOL fDX)
{
    pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BRENDERING;

    if ((pWnd->dwFlags_ & EWF_BREDRAWBACKGROUND) == EWF_BREDRAWBACKGROUND) {
        pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BREDRAWBACKGROUND);
        _wnd_paint_bkg(hWnd, pWnd);
    }
    HEXCANVAS cvDisplay = pWnd->canvas_display_;
    if (_canvas_begindraw(cvDisplay)) {
        ID2D1DeviceContext* pContext       = nullptr;
        ID2D1Bitmap*        pBitmapDisplay = nullptr;
        if (fDX) {
            pContext = pWnd->dx_context_;
            pBitmapDisplay = (ID2D1Bitmap*)_canvas_getcontext(cvDisplay, CANVAS_DX_D2DBITMAP);
            
            if (pWnd->Radius_ == 0  || !_wnd_querystyle(hWnd, WS_EX_LAYERED, TRUE)) {  
                _dx_bmp_copyfrom(
                    &pBitmapDisplay,
                    (ID2D1Bitmap*)_canvas_getcontext(pWnd->canvas_bkg_, CANVAS_DX_D2DBITMAP),
                    rcPaint.left, rcPaint.top, rcPaint.left, rcPaint.top, rcPaint.right,
                    rcPaint.bottom);
            }
            else {
                int       nError = 0;
                canvas_s* pCanvas = nullptr;
                if (_handle_validate(pWnd->canvas_bkg_, HT_CANVAS, (LPVOID*)&pCanvas, &nError)) {
                    ID2D1DeviceContext* pContext = pCanvas->pContext_;
                    pContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                    pContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
                    // 创建兼容渲染目标
                    ID2D1BitmapRenderTarget* bmpRenderTarget = nullptr;
                    pContext->CreateCompatibleRenderTarget(&bmpRenderTarget);
                    ID2D1BitmapBrush* br = nullptr;
                    HRESULT           hr = g_Ri.pD2DDeviceContext->CreateBitmapBrush(
                        (ID2D1Bitmap*)_canvas_getcontext(pWnd->canvas_bkg_, CANVAS_DX_D2DBITMAP),
                        &br);
                    if (SUCCEEDED(hr)) {
                        bmpRenderTarget->BeginDraw();
                        bmpRenderTarget->Clear(0);
                        D2D1_ROUNDED_RECT rc = {0};
                        rc.radiusX           = pWnd->Radius_;
                        rc.radiusY           = pWnd->Radius_;
                        rc.rect              = D2D1::RectF(0, 0, pWnd->width_, pWnd->height_);
                        bmpRenderTarget->FillRoundedRectangle(rc, br);
                        br->Release();
                        hr = bmpRenderTarget->EndDraw();
                    }
                    
                    if (SUCCEEDED(hr)) {
                        ID2D1Bitmap* bitmap;
                        bmpRenderTarget->GetBitmap(&bitmap);
                        _dx_bmp_copyfrom(&pBitmapDisplay, bitmap, rcPaint.left, rcPaint.top,
                                         rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
                        bitmap->Release();
                    }
                    else {
                        _dx_bmp_copyfrom(&pBitmapDisplay,
                                         (ID2D1Bitmap*)_canvas_getcontext(pWnd->canvas_bkg_,
                                                                          CANVAS_DX_D2DBITMAP),
                                         rcPaint.left, rcPaint.top, rcPaint.left, rcPaint.top,
                                         rcPaint.right, rcPaint.bottom);
                    }

                    bmpRenderTarget->Release();
                }
            }
            _dx_settarget(pContext, (ID2D1Bitmap*)pBitmapDisplay);
        }
        else {
            _canvas_bitblt(cvDisplay, pWnd->canvas_bkg_, rcPaint.left, rcPaint.top, rcPaint.right,
                           rcPaint.bottom, rcPaint.left, rcPaint.top);
        }
        ID2D1Layer* pLayer = nullptr;
        ID2D1RoundedRectangleGeometry* pClipGeometry = nullptr;
        // 在调用_wnd_render_obj前插入：
        if (pWnd->Radius_ != 0 && fDX) {
            pContext->CreateLayer(nullptr, &pLayer);
            D2D1_ROUNDED_RECT clipRect;
            clipRect.rect = D2D1::RectF(0, 0, pWnd->width_, pWnd->height_);
            clipRect.radiusX = pWnd->Radius_;
            clipRect.radiusY = pWnd->Radius_;
            // 创建圆角矩形几何对象
            g_Ri.pD2Dfactory->CreateRoundedRectangleGeometry(clipRect, &pClipGeometry);
            // 配置图层参数
            D2D1_LAYER_PARAMETERS1 layerParams = D2D1::LayerParameters1();
            layerParams.geometricMask = pClipGeometry;  // 直接赋值几何对象
            // 应用图层裁剪
            pContext->PushLayer(&layerParams, pLayer);
        }
        LPVOID hBrush = nullptr;
        if (Flag_Query(ENGINE_FLAG_OBJECT_SHOWRECTBORDER)) {
            hBrush = _brush_create(-65536);
        }
        if (pWnd->Radius_ != 0 && fDX) {
            // 使用后释放几何对象
            if (pClipGeometry)pClipGeometry->Release();
            if (pLayer)pContext->PopLayer();
            if (pLayer)pLayer->Release();
        }
        _wnd_render_obj(hWnd, pWnd, pContext, cvDisplay, pBitmapDisplay, rcPaint,
                        pWnd->objChildFirst_, 0, 0, 255, fDX, hBrush);
        _brush_destroy(hBrush);
        
        _wnd_render_dc(hWnd, pWnd, hDC, cvDisplay, rcPaint, fLayer);
        _canvas_enddraw(cvDisplay);
    }
    pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BRENDERING);
}

INT _wnd_popupclose(wnd_s* pWnd, HWND hWnd, INT wParam, obj_s* pObj)
{
    INT result = 0;
    INT nError = 0;

    if (FLAGS_CHECK(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN)) {
        if (!pObj ||
            (!_handle_validate(pWnd->lpPopupParams_, HT_OBJECT, (LPVOID*)&pObj, &nError)) ||
            (result = _obj_baseproc(hWnd, pObj->hObj_, pObj, -7, wParam, 0)) == 0) {
            FLAGS_DEL(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN);
            result = DestroyWindow(pWnd->hWndPopup_);
        }
    }
    return result;
}

void _wnd_wm_size(wnd_s* pWnd, HWND hWnd, WPARAM wParam, INT width, INT height)
{
    pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BRENDERED);
    pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_SIZED | EWF_BREDRAWBACKGROUND;
    if (width != pWnd->width_ || height != pWnd->height_) {

        if (((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX)) {
            _wnd_addstyle(hWnd, WS_EX_LAYERED, TRUE);
        }

        if (((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {
            width  = width - GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
            height = height - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
        }
        _wnd_recalcclient(pWnd, hWnd, width, height);
        _layout_update(pWnd->hLayout_);
        RECT rcCaption{0};
        _wnd_calc_captionrect(pWnd, &rcCaption);
        Ex_ObjSetPos(
            pWnd->objCaption_, 0, 0, 0, rcCaption.right - rcCaption.left, OBJECT_POSITION_DEFAULT,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_EX_NODPISCALE);
        if (pWnd->dwWinState_ != wParam) {
            if (((pWnd->dwStyle_ & WINDOW_STYLE_BUTTON_MAX) == WINDOW_STYLE_BUTTON_MAX)) {
                Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, WINDOW_STYLE_BUTTON_MAX), 0);
            }

            if (((pWnd->dwStyle_ & WINDOW_STYLE_BUTTON_MIN) == WINDOW_STYLE_BUTTON_MIN)) {
                Ex_ObjInvalidateRect(_obj_getobjfromidorname(pWnd, WINDOW_STYLE_BUTTON_MIN), 0);
            }
            pWnd->dwWinState_ = wParam;
        }
    }
    InvalidateRect(hWnd, 0, FALSE);
}

void _wnd_menu_setpos(HWND hWnd, wnd_s* pWnd, tagWINDOWPOS* pos)
{
    wnd_s* pMenuHostWnd = pWnd->pMenuHostWnd_;
    wnd_s* pMenuPrevWnd = nullptr;
    RECT   rcParent{0};
    if (pMenuHostWnd != 0) {
        pMenuPrevWnd = pMenuHostWnd->pMenuPrevWnd_;
        if (pMenuPrevWnd != 0) {
            GetWindowRect(pMenuPrevWnd->hWnd_, &rcParent);
        }
    }
    auto  offset = LOBYTE(HIWORD(pWnd->szItemSeparator_));
    INT   x      = pos->x;
    INT   y      = pos->y;
    POINT pt;
    GetCursorPos(&pt);
    if (rcParent.left < x)   // 子菜单在右边
    {
        x = rcParent.right + offset - 1;
    }
    else {
        // 子菜单在左边
        x = rcParent.left;
        GetWindowRect(hWnd, &rcParent);
        x = x - (rcParent.right - rcParent.left) + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 1;
        x = x - offset;
    }
    pos->x = x;
    if (pMenuPrevWnd != 0) {
        HEXOBJ hObj   = pMenuPrevWnd->objFocus_;
        obj_s* pObj   = nullptr;
        INT    nError = 0;
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            LPVOID padding_client = pWnd->padding_client_;
            y = pObj->w_top_ + pMenuPrevWnd->top_ - __get_int(padding_client, 4);
            GetWindowRect(hWnd, &rcParent);
            INT height = rcParent.bottom - rcParent.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
            _wnd_getscreenrect(hWnd, &rcParent);

            if (y + height > rcParent.bottom) {
                y = pObj->w_bottom_ - height + __get_int(padding_client, 12);
            }
        }
    }
    pos->y = y;
}

void _wnd_menu_createitems(HWND hWnd, wnd_s* pWnd)
{
    size_t   hMenu         = SendMessageW(hWnd, MN_GETHMENU, 0, 0);
    HEXTHEME hTheme        = pWnd->hTheme_;
    LPVOID   lpPaddingText = Ex_ThemeGetValuePtr(hTheme, ATOM_DUIMENU, ATOM_PADDING_TEXT);
    INT      nCount        = GetMenuItemCount((HMENU)hMenu);
    wnd_s*   pMenuHostWnd  = pWnd->pMenuHostWnd_;
    HWND     hParent       = pMenuHostWnd->hWnd_;
    RECT     rcParent{0};
    GetWindowRect(hParent, &rcParent);
    LPVOID padding_client  = pWnd->padding_client_;
    RECT   rcPaddingClient = {0};
    if (padding_client != 0) {
        RtlMoveMemory(&rcPaddingClient, padding_client, 16);
        if (g_Li.DpiX > 1) {
            rcPaddingClient.left   = rcPaddingClient.left * g_Li.DpiX;
            rcPaddingClient.top    = rcPaddingClient.top * g_Li.DpiX;
            rcPaddingClient.right  = rcPaddingClient.right * g_Li.DpiX;
            rcPaddingClient.bottom = rcPaddingClient.bottom * g_Li.DpiX;
        }
    }

    obj_s* pParnet   = nullptr;
    INT    nError    = 0;
    HEXOBJ objParent = _obj_create_init(hWnd, pWnd, ATOM_PAGE, 0, &pParnet, &nError);
    if (objParent != 0) {
        INT width  = pWnd->width_ - (rcPaddingClient.left + rcPaddingClient.right);
        INT height = pWnd->height_ - (rcPaddingClient.top + rcPaddingClient.bottom);
        _obj_create_proc(&nError, FALSE, hTheme, pParnet, OBJECT_STYLE_EX_FOCUSABLE, ATOM_PAGE, 0,
            OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_MENUITEM_SUBMENU,
                         rcPaddingClient.left, rcPaddingClient.top, width, height, 0, 0, 0, 0, 0);
        _obj_create_done(hWnd, pWnd, objParent, pParnet);
        HEXOBJ    objPP   = objParent;
        HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_ABSOLUTE, pWnd->hexdui_);
        _layout_absolute_setedge(hLayout, objPP, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                                 LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, rcPaddingClient.left);
        _layout_absolute_setedge(hLayout, objPP, LAYOUT_SUBPROP_ABSOLUTE_TOP,
                                 LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, rcPaddingClient.top);
        _layout_absolute_setedge(hLayout, objPP, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
                                 LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, rcPaddingClient.right);
        _layout_absolute_setedge(hLayout, objPP, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                                 LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, rcPaddingClient.bottom);
        nError    = 0;
        objParent = _obj_create_init(hWnd, pWnd, ATOM_PAGE, 0, &pParnet, &nError);
        if (objParent != 0) {
            INT heightParent = height;
            _obj_create_proc(&nError, FALSE, hTheme, pParnet, OBJECT_STYLE_EX_FOCUSABLE, ATOM_PAGE,
                             0, OBJECT_STYLE_VISIBLE, 0, 0, width, height, objPP, 0, 0, 0, 0);
            _obj_create_done(hWnd, pWnd, objParent, pParnet);
            hLayout = _layout_create(LAYOUT_TYPE_ABSOLUTE, objPP);
            _layout_absolute_setedge(hLayout, objParent, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                                     LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 0);
            _layout_absolute_setedge(hLayout, objParent, LAYOUT_SUBPROP_ABSOLUTE_TOP,
                                     LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 0);
            _layout_absolute_setedge(hLayout, objParent, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
                                     LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 0);
            _layout_absolute_setedge(hLayout, objParent, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                                     LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 0);
            height = 0;
            MENUITEMINFOW mii;
            mii.cbSize = sizeof(MENUITEMINFOW);
            mii.fMask  = MIIM_FTYPE | MIIM_SUBMENU | MIIM_ID;
            RECT rcItem{0};
            INT  eos;
            INT  offsetTop = 0;
            for (INT i = 0; i < nCount; i++) {
                if (GetMenuItemRect(hParent, (HMENU)hMenu, i, &rcItem)) {
                    
                    eos = OBJECT_STYLE_VISIBLE;
                    if (GetMenuItemInfoW((HMENU)hMenu, i, TRUE, &mii)) {
                        if ((mii.fType & MFT_SEPARATOR) != 0)   // 分隔符
                        {
                            eos = eos | OBJECT_STYLE_MENUITEM_SEPARATOR;
                        }
                        else {
                            if (mii.hSubMenu != 0) {
                                eos = eos | OBJECT_STYLE_MENUITEM_SUBMENU;
                            }
                        }
                    }
                    WCHAR buff[520];
                    OffsetRect(&rcItem, -rcParent.left, -rcParent.top);
                    // 组件超出屏幕左边会出现菜单项目左边负数
                    if (rcItem.left < 0) {
                        INT offset   = abs(rcItem.left);
                        rcItem.left  = rcItem.left + offset;
                        rcItem.right = rcItem.right + offset;
                    }
                    else if (rcItem.left > 0) {
                        INT offset   = abs(rcItem.left);
                        rcItem.left  = rcItem.left - offset;
                        rcItem.right = rcItem.right - offset;
                    }
                    // 判断第一项，取第一项顶边偏移,组件移到屏幕最顶端二级子菜单第一项会负数
                    if (rcItem.top < 0 && i == 0) {
                        offsetTop = abs(rcItem.top);
                    }
                    
                    rcItem.top    = rcItem.top + offsetTop;
                    rcItem.bottom = rcItem.bottom + offsetTop;
                    
                    GetMenuStringW((HMENU)hMenu, i, buff, 520, MF_BYPOSITION);
                    obj_s* pObj = nullptr;
                    nError      = 0;
                    HEXOBJ hObj = _obj_create_init(hWnd, pWnd, ATOM_ITEM, 0, &pObj, &nError);
                    if (hObj != 0) {
                        _obj_create_proc(&nError, FALSE, hTheme, pObj,
                                         OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                                         ATOM_ITEM, buff, eos, rcItem.left, rcItem.top, width,
                                         rcItem.bottom - rcItem.top, objParent, mii.wID, 0, i,
                                         DT_VCENTER | DT_SINGLELINE);
                        
                        pObj->dwFlags_ = pObj->dwFlags_ | EOF_BMENUITEM;
                        _obj_create_done(hWnd, pWnd, hObj, pObj);
                        if (lpPaddingText != 0) {
                            RtlMoveMemory((LPVOID)((size_t)pObj + offsetof(obj_s, t_left_)),
                                          lpPaddingText, 16);
                            if (g_Li.DpiX > 1) {
                                pObj->t_left_   = pObj->t_left_ * g_Li.DpiX;
                                pObj->t_top_    = pObj->t_top_ * g_Li.DpiY;
                                pObj->t_right_  = pObj->t_right_ * g_Li.DpiX;
                                pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiY;
                            }
                        }
                    }
                    height = height + rcItem.bottom - rcItem.top;
                }
            }
            _obj_setpos_org(pParnet, objParent, 0, OBJECT_POSITION_DEFAULT, OBJECT_POSITION_DEFAULT,
                            OBJECT_POSITION_DEFAULT, height,
                            SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE |
                                SWP_NOCOPYBITS | SWP_EX_NODPISCALE,
                            0);
            height = height - 1;
            Ex_ObjScrollShow(objPP, SCROLLBAR_TYPE_VERT, height - heightParent > 0);
            Ex_ObjScrollSetInfo(objPP, SCROLLBAR_TYPE_VERT, SIF_RANGE | SIF_PAGE, 0,
                                height - heightParent, heightParent, 0, TRUE);
        }
    }
    SendMessageW(hWnd, WM_INITMENUPOPUP, hMenu, 0);
}

void _wnd_menu_init(HWND hWnd, wnd_s* pWnd)
{
    if ((pWnd->dwFlags_ & EWF_BMENUINITED) != EWF_BMENUINITED) {
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BMENUINITED;
        _wnd_menu_createitems(hWnd, pWnd);
        ShowWindow(pWnd->hWndShadow_, SW_SHOWNOACTIVATE);

        SetWindowLongPtrW(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        InvalidateRect(hWnd, NULL, FALSE);
    }
}

void _wnd_paint_shadow(wnd_s* pWnd, BOOL bUpdateRgn, BOOL bFlush)
{
    if (!((pWnd->dwStyle_ & WINDOW_STYLE_NOSHADOW) == WINDOW_STYLE_NOSHADOW)) {

        if ((pWnd->dwFlags_ & EWF_BRENDERED) == EWF_BRENDERED) {
            HWND  hWnd = pWnd->hWndShadow_;
            POINT ptDst;
            ptDst.x = pWnd->left_;
            ptDst.y = pWnd->top_;
            SIZE sz;
            sz.cx             = pWnd->width_;
            sz.cy             = pWnd->height_;
            LPVOID prcPadding = pWnd->padding_shadow_;
            RECT   rcPadding{0};
            if (prcPadding != 0) {
                RtlMoveMemory(&rcPadding, prcPadding, sizeof(RECT));
                ptDst.x = ptDst.x - rcPadding.left;
                ptDst.y = ptDst.y - rcPadding.top;
                sz.cx   = sz.cx + rcPadding.left + rcPadding.right;
                sz.cy   = sz.cy + rcPadding.top + rcPadding.bottom;
            }
            MoveWindow(hWnd, ptDst.x, ptDst.y, sz.cx, sz.cy, FALSE);

            if (bUpdateRgn || bFlush) {
                auto hDC = GetDC(hWnd);
                if (hDC != 0) {
                    INT       nError   = 0;
                    HEXCANVAS cvShadow = _canvas_createfrompwnd(
                        pWnd, sz.cx, sz.cy, CANVAS_FLAG_GDI_COMPATIBLE, &nError);
                    if (cvShadow != 0) {
                        INT alpha = ((pWnd->dwFlags_ & EWF_ACTIVE) == EWF_ACTIVE) ? 255 : 204;
                        alpha     = alpha * pWnd->alpha_ / 255;
                        if (_canvas_begindraw(cvShadow)) {
                            _canvas_setantialias(cvShadow, TRUE);
                            _canvas_setimageantialias(cvShadow, TRUE);
                            _canvas_drawshadow(cvShadow, 11, 11, sz.cx - 12, sz.cy - 12, 11,
                                               pWnd->crSD_, pWnd->Radius_, pWnd->Radius_,
                                               pWnd->Radius_, pWnd->Radius_, 0, 0);
                            LPVOID mDC = _canvas_getdc(cvShadow);
                            if (mDC != 0) {
                                POINT         ptSrc = {0};
                                BLENDFUNCTION pb_;
                                pb_.BlendOp             = 0;
                                pb_.BlendFlags          = 0;
                                pb_.SourceConstantAlpha = 0xFF;
                                pb_.AlphaFormat         = 1;
                                UpdateLayeredWindow(hWnd, hDC, &ptDst, &sz, (HDC)mDC, &ptSrc, 0,
                                                    &pb_, ULW_ALPHA);
                                _canvas_releasedc(cvShadow);
                            }
                            _canvas_enddraw(cvShadow);
                        }
                        _canvas_destroy(cvShadow);
                    }
                    ReleaseDC(hWnd, hDC);
                }
            }
        }
    }
}

BOOL _wnd_wm_paint(wnd_s* pWnd, HWND hWnd)
{
    PAINTSTRUCT ps = {0};

    if (((pWnd->dwFlags_ & EWF_INTED) == EWF_INTED)) {

        BOOL fSized = ((pWnd->dwFlags_ & EWF_SIZED) == EWF_SIZED);
        if (fSized) {
            pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_SIZED);
        }

        BOOL fLayer = ((pWnd->dwFlags_ & EWF_BLAYERED) == EWF_BLAYERED);

        if (fLayer) {
            if (fSized) {
                ps.rcPaint.right  = pWnd->width_;
                ps.rcPaint.bottom = pWnd->height_;
            }
            else {
                GetUpdateRect(hWnd, &ps.rcPaint, FALSE);

                if (((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU)) {}
            }
        }
        else {
            BeginPaint(hWnd, &ps);
        }

        _wnd_render(hWnd, pWnd, ps.hdc, ps.rcPaint, fLayer, TRUE);
        if (!fLayer) {
            EndPaint(hWnd, &ps);
        }
    }
    if ((pWnd->dwFlags_ & EWF_BRENDERED) != EWF_BRENDERED) {
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BRENDERED;
        _wnd_paint_shadow(pWnd, TRUE, FALSE);
    }
    return FALSE;
}

BOOL _wnd_wm_getminmaxinfo(wnd_s* pWnd, HWND hWnd, LPARAM lParam)
{
    RECT rcMonitor{0}, rcDesk{0};
    BOOL ret = FALSE;
    if (_wnd_getscreenrect(hWnd, &rcMonitor, &rcDesk)) {
        INT width  = pWnd->width_;
        INT height = pWnd->height_;

        if (((pWnd->dwStyle_ & WINDOW_STYLE_FULLSCREEN) == WINDOW_STYLE_FULLSCREEN))   // 全屏模式
        {
            RtlMoveMemory(&rcDesk, &rcMonitor, 16);
        }

        OffsetRect(&rcDesk, -rcMonitor.left, -rcMonitor.top);

        // 左边，顶边
        ((MINMAXINFO*)lParam)->ptMaxPosition.x = rcDesk.left - 1;
        ((MINMAXINFO*)lParam)->ptMaxPosition.y = rcDesk.top - 1;

        // 最大宽度，高度
        INT nMaxWidth                      = rcDesk.right - rcDesk.left + 2;
        INT nMaxHeight                     = rcDesk.bottom - rcDesk.top + 2;
        ((MINMAXINFO*)lParam)->ptMaxSize.x = nMaxWidth;
        ((MINMAXINFO*)lParam)->ptMaxSize.y = nMaxHeight;

        // 允许调整的最小尺寸
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = pWnd->minWidth_;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = pWnd->minHeight_;

        // 允许调整的最大尺寸
        ((MINMAXINFO*)lParam)->ptMaxTrackSize.x = nMaxWidth;
        ((MINMAXINFO*)lParam)->ptMaxTrackSize.y = nMaxHeight;
        ret                                     = TRUE;
    }
    return ret;
}

void _wnd_wm_buttondown(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam,
                        LPARAM lParam)
{
    if ((pWnd->dwFlags_ & EWF_BTRACKOBJECT) != EWF_BTRACKOBJECT) {
        pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & (EWF_BLEFTTRACK | EWF_BRIGHTTRACK |
                                                             EWF_BMIDTRACK | EWF_BLEFTDTRACK));
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BTRACKOBJECT;
        if (uMsg == WM_LBUTTONDOWN) {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BLEFTTRACK;
            _obj_setfocus(hWnd, pWnd, hObj, pObj, TRUE);
        }
        else if (uMsg == WM_LBUTTONDBLCLK) {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BLEFTDTRACK;
            _obj_setfocus(hWnd, pWnd, hObj, pObj, TRUE);
            uMsg = WM_LBUTTONDOWN;
        }
        else if (uMsg == WM_RBUTTONDOWN) {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BRIGHTTRACK;
        }
        else {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BMIDTRACK;
        }
        pWnd->objTrack_ = hObj;
        _obj_tooltips_popup(pWnd, 0, 0, -1, -1, -1, 0, FALSE);
        _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
        if (wParam != 0) {
            SetCapture(hWnd);   // 对menu窗口无效
        }
    }
}

void _wnd_obj_untrack(HWND hWnd, wnd_s* pWnd, LPARAM lParam, BOOL fMsgDispatch)
{

    if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT)) {
        pWnd->dwFlags_      = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BTRACKOBJECT);
        HEXOBJ objTrack     = pWnd->objTrack_;
        pWnd->objTrack_     = 0;
        pWnd->objTrackPrev_ = objTrack;
        if (!fMsgDispatch) {
            ReleaseCapture();   // 对menu窗口无效
        }
        obj_s* pObj   = nullptr;
        INT    nError = 0;
        if (_handle_validate(objTrack, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            POINT pt;
            GetCursorPos(&pt);
            INT uMsg;
            if (fMsgDispatch) {

                if ((pWnd->dwFlags_ & EWF_BLEFTTRACK) == EWF_BLEFTTRACK) {
                    uMsg = WM_LBUTTONUP;
                }
                else if ((pWnd->dwFlags_ & EWF_BRIGHTTRACK) == EWF_BRIGHTTRACK) {
                    uMsg = WM_RBUTTONUP;
                }
                else if ((pWnd->dwFlags_ & EWF_BMIDTRACK) == EWF_BMIDTRACK) {
                    uMsg = WM_MBUTTONUP;
                }
                _obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, 0);

                _wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
                obj_s Obj;
                _wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, &Obj, lParam, FALSE);
            }
            else {
                _wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
                obj_s Obj;
                _wnd_wm_leavecheck(hWnd, pWnd, objTrack, -1, &Obj, lParam, FALSE);
                if (pWnd->objHittest_ == objTrack) {
                    if (((pWnd->dwFlags_ & EWF_BLEFTTRACK) == EWF_BLEFTTRACK)) {
                        uMsg = WM_EX_LCLICK;
                    }
                    else if (((pWnd->dwFlags_ & EWF_BLEFTDTRACK) == EWF_BLEFTDTRACK)) {
                        uMsg = WM_EX_LDCLICK;
                    }
                    else if (((pWnd->dwFlags_ & EWF_BRIGHTTRACK) == EWF_BRIGHTTRACK)) {
                        uMsg = WM_EX_RCLICK;
                    }
                    else {
                        uMsg = WM_EX_MCLICK;
                    }
                    _obj_baseproc(hWnd, objTrack, pObj, uMsg, 0, pWnd->dwHitObjPos_Abs_);
                }
            }
            _obj_dispatchnotify(hWnd, pObj, objTrack, 0, NM_RELEASEDCAPTURE, 0, 0);
        }
    }
}

void _wnd_wm_captionchange(HWND hWnd, wnd_s* pWnd, LPARAM lParam)
{
    _wnd_obj_untrack(hWnd, pWnd, lParam, FALSE);
}

void _wnd_wm_mouse(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ hObj;
    LPARAM OldlParam = lParam;
    if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT))   // 是否按住组件
    {
        hObj = pWnd->objTrack_;
    }
    else {
        hObj = pWnd->objHittest_;
    }
    INT    dwHitCode = pWnd->dwHitCode_;
    obj_s* pObj      = nullptr;
    INT    nError    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

        if (((pWnd->dwFlags_ & EWF_BTRACKOBJECT) == EWF_BTRACKOBJECT)) {
            lParam =
                MAKELONG(GET_X_LPARAM(lParam) - pObj->w_left_, GET_Y_LPARAM(lParam) - pObj->w_top_);
            pWnd->dwHitObjPos_Abs_ = lParam;
        }
        else {
            lParam = pWnd->dwHitObjPos_Abs_;
        }
    }
    if (uMsg == WM_MOUSEMOVE) {
        if (wParam == 0) {
            _wnd_wm_leavecheck(hWnd, pWnd, pWnd->objHittestPrev_, hObj, pObj, OldlParam, TRUE);
        }
        if (hObj != 0) {
            _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
        }
    }
    else {

        if (pObj != 0) {
            if (uMsg == WM_LBUTTONDOWN) {
                _wnd_popupclose(pWnd, hWnd, hObj, pObj);
            }
            if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) {
                _wnd_wm_buttondown(hWnd, pWnd, hObj, pObj, uMsg, wParam, lParam);
            }
            else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) {
                _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
                _wnd_obj_untrack(hWnd, pWnd, lParam, FALSE);
            }
            else if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK ||
                     uMsg == WM_MBUTTONDBLCLK) {
                _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
                INT newMsg = uMsg;
                if (uMsg == WM_LBUTTONDBLCLK) {
                    newMsg = WM_LBUTTONDBLCLK;
                }
                else if (uMsg == WM_RBUTTONDBLCLK) {
                    newMsg = WM_RBUTTONDOWN;
                }
                else {
                    newMsg = WM_MBUTTONDOWN;
                }
                _wnd_wm_buttondown(hWnd, pWnd, hObj, pObj, newMsg, wParam, lParam);
            }
        }
    }
}

void _wnd_wm_menucontext(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ objHittest = pWnd->objHittest_;
    obj_s* pObj       = nullptr;
    INT    nError     = 0;
    if (_handle_validate(objHittest, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pWnd->objMenucontext_ = objHittest;
        pWnd->dwFlags_        = pWnd->dwFlags_ | EWF_BMENUCONTEXT;
        _obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam);
        pWnd->dwFlags_ = pWnd->dwFlags_ - (pWnd->dwFlags_ & EWF_BMENUCONTEXT);
    }
}

void _wnd_wm_mousewheel(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ objHittest = pWnd->objHittest_;
    obj_s* pObj       = nullptr;
    INT    nError     = 0;

    if (FLAGS_CHECK(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN)) {
        hWnd = pWnd->hWndPopup_;
        SendMessage(hWnd, uMsg, wParam, lParam);
        return;
    }

    if (_handle_validate(objHittest, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (_obj_baseproc(hWnd, objHittest, pObj, uMsg, wParam, lParam) ==
            0)   // 我认为这样是不完善的，因为存在转发WHEEL消息让别的控件滚动的可能性
        {
            EX_CLASSINFO* pCls = pObj->pCls_;
            if (pCls->atomName == ATOM_SCROLLBAR) {
                _scrollbar_parentnotify(hWnd, pObj, wParam > 0 ? SB_LINEUP : SB_LINEDOWN,
                                        objHittest, 0, TRUE);
            }

            if (((pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) == OBJECT_STYLE_VSCROLL)) {
                _obj_baseproc(hWnd, objHittest, pObj, WM_VSCROLL,
                              wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
            }

            if (((pObj->dwStyle_ & OBJECT_STYLE_HSCROLL) == OBJECT_STYLE_HSCROLL)) {
                _obj_baseproc(hWnd, objHittest, pObj, WM_HSCROLL,
                              wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
            }
        }
    }
}

void _wnd_wm_command(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (((pWnd->dwFlags_ & EWF_BMODAL) == EWF_BMODAL)) {
        if (wParam == 2) {
            if (Ex_ObjIsEnable(
                    Ex_ObjGetFromID(pWnd->objCaption_,
                                    WINDOW_STYLE_BUTTON_CLOSE)))   // 判断按下关闭按钮,退出模态窗口
            {
                EndDialog(hWnd, IDCANCEL);
            }
        }
    }
    else {
        HEXOBJ objMenucontext = pWnd->objMenucontext_;
        pWnd->objMenucontext_ = 0;
        obj_s* pObj           = nullptr;
        INT    nError         = 0;

        if (_handle_validate(objMenucontext, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            _obj_baseproc(hWnd, objMenucontext, pObj, uMsg, wParam, lParam);
        }
    }
}

HEXOBJ _wnd_obj_setnextfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ objEntry, HEXOBJ objLastFocus)
{
    obj_s* pObjEntry = nullptr;
    INT    nError    = 0;
    while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObjEntry, &nError)) {

        if (((pObjEntry->dwStyleEx_ & OBJECT_STYLE_EX_TABSTOP) == OBJECT_STYLE_EX_TABSTOP)) {
            if (objLastFocus != objEntry) {
                _obj_setfocus(hWnd, pWnd, objEntry, pObjEntry, TRUE);
            }
            return objEntry;
        }
        objEntry = pObjEntry->objNext_;
    }
    return 0;
}

void _wnd_obj_settabstop(HWND hWnd, wnd_s* pWnd, HEXOBJ objLastFocus)
{
    obj_s* pObjLast = nullptr;
    INT    nError   = 0;
    HEXOBJ objNextFocus;
    if (_handle_validate(objLastFocus, HT_OBJECT, (LPVOID*)&pObjLast, &nError)) {
        HEXOBJ objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pObjLast->objNext_, objLastFocus);
        if (objNextFocus == 0) {
            EXHANDLE objParent  = pObjLast->objParent_;
            obj_s*   pObjParent = nullptr;
            if (_handle_validate(objParent, HT_OBJECT, (LPVOID*)&pObjParent, &nError)) {
                objNextFocus =
                    _wnd_obj_setnextfocus(hWnd, pWnd, pObjParent->objChildFirst_, objLastFocus);
            }
            else {
                objNextFocus =
                    _wnd_obj_setnextfocus(hWnd, pWnd, pWnd->objChildFirst_, objLastFocus);
            }
        }
    }
    else {
        objNextFocus = _wnd_obj_setnextfocus(hWnd, pWnd, pWnd->objChildFirst_, objLastFocus);
    }
}

BOOL _wnd_wm_keyboard(wnd_s* pWnd, HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    HEXOBJ objFocus          = pWnd->objFocus_;
    obj_s* pObj              = nullptr;
    INT    nError            = 0;
    BOOL   bPopupWindowShown = FALSE;
    if (_handle_validate(objFocus, HT_OBJECT, (LPVOID*)&pObj, &nError) && uMsg != WM_IME_CHAR) {
        if (FLAGS_CHECK(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN)) {

            if (uMsg >= WM_KEYDOWN && uMsg <= WM_KEYLAST) {
                SendMessageW(pWnd->hWndPopup_, uMsg, wParam, lParam);
                bPopupWindowShown = TRUE;
            }
        }
        if (_obj_baseproc(hWnd, objFocus, pObj, uMsg, wParam, lParam) !=
            0)   // 先给组件，看他需不需要消息。
        {
            return 1;
        }
    }
    if (uMsg == WM_KEYDOWN) {
        if (wParam == VK_ESCAPE) {
            if (((pWnd->dwStyle_ & WINDOW_STYLE_ESCEXIT) == WINDOW_STYLE_ESCEXIT) &&
                !bPopupWindowShown) {
                PostMessageW(hWnd, WM_CLOSE, 0, 0);
            }
        }
        else if (wParam == VK_SPACE) {
            if (_wnd_getfunctionkeys() == 0) {
                if (pObj != 0) {
                    if (!__query(pObj, offsetof(obj_s, dwFlags_), EOF_BDISABLESPACEANDENTER)) {
                        _wnd_wm_buttondown(hWnd, pWnd, objFocus, pObj, WM_LBUTTONDOWN, 0, 0);
                    }
                }
            }
        }
        else if (wParam == VK_RETURN) {
            if (_wnd_getfunctionkeys() == 0) {
                if (pObj != 0) {
                    if (!__query(pObj, offsetof(obj_s, dwFlags_), EOF_BDISABLESPACEANDENTER)) {
                        
                        if (_obj_dispatchnotify(hWnd, pObj, objFocus, 0, NM_RETURN, wParam,
                                                lParam) == 0) {
                            _obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
                        }
                    }
                }
            }
        }
        else if (wParam == VK_TAB) {
            _wnd_obj_settabstop(hWnd, pWnd, objFocus);
        }
        else if (wParam == VK_UP || wParam == VK_DOWN) {
            if ((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU) {
                return 1;
            }
        }
        else if (wParam == VK_LEFT || wParam == VK_RIGHT) {
            if ((pWnd->dwStyle_ & WINDOW_STYLE_MENU) == WINDOW_STYLE_MENU) {
                return 1;
            }
        }

        if ((pWnd->dwFlags_ & EWF_BMODAL) == EWF_BMODAL) {
            UINT retvalue = MapVirtualKeyW(wParam, 2);   // 判断是否不是输入法
            if (retvalue) {
                BYTE bs[256] = {0};
                GetKeyboardState(bs);
                WCHAR charactersPressed[2] = {};
                ToUnicode(wParam, lParam, bs, charactersPressed, 2, 0);
                if (pObj != 0) {
                    _obj_baseproc(hWnd, objFocus, pObj, WM_CHAR, (WPARAM)charactersPressed[0],
                                  lParam);
                }
            }
        }
    }
    else if (uMsg == WM_KEYUP) {
        if (wParam == VK_SPACE) {
            if (pObj != 0) {
                if (!__query(pObj, offsetof(obj_s, dwFlags_), EOF_BDISABLESPACEANDENTER)) {
                    _wnd_wm_mouse(pWnd, hWnd, WM_LBUTTONUP, 0, 0);
                    _obj_baseproc(hWnd, objFocus, pObj, WM_EX_LCLICK, 0, 0);
                }
            }
        }
    }
    else if (uMsg == WM_COPY) {
        if ((pWnd->dwStyle_ & WINDOW_STYLE_MESSAGEBOX) == WINDOW_STYLE_MESSAGEBOX && pObj) {
            if (pObj->pCls_->atomName == ATOM_EDIT) {
                return 1;
            }
        }
    }
    else if (uMsg == WM_IME_CHAR) {
        if ((pWnd->dwFlags_ & EWF_BMODAL) == EWF_BMODAL && pObj) {
            _obj_baseproc(hWnd, objFocus, pObj, WM_CHAR, wParam, lParam);
        }
    }
    return 0;
}

BOOL _wnd_obj_childtabstop(HEXOBJ objEntry, HEXOBJ* objFocusable, LPVOID* pObjFocusable,
                           HEXOBJ objLast, BOOL* bFounded)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    BOOL   ret    = FALSE;
    while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (!*bFounded) {
            if (objLast = objEntry) *bFounded = TRUE;
        }

        if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_TABSTOP) == OBJECT_STYLE_EX_TABSTOP)) {
            *objFocusable  = objEntry;
            *pObjFocusable = pObj;
            ret            = TRUE;
            break;
        }
        HEXOBJ objSub = pObj->objChildFirst_;
        if (objSub != 0) {
            if (_wnd_obj_childtabstop(objSub, objFocusable, pObjFocusable, objLast, bFounded)) {
                ret = TRUE;
                break;
            }
        }
        objEntry = pObj->objNext_;
    }
    return ret;
}

void _wnd_wm_ime_composition(HWND hWnd, wnd_s* pWnd)
{
    auto hImc = ImmGetContext(hWnd);
    if (hImc != 0) {
        POINT pt;
        GetCaretPos(&pt);
        if (FLAGS_CHECK(pWnd->dwFlags_, EWF_BPOPUPWINDOIWSHOWN)) {
            RECT rect;
            GetWindowRect(pWnd->hWndPopup_, &rect);
            pt.x += rect.left - pWnd->left_;
            pt.y += rect.top - pWnd->top_;
        }
        COMPOSITIONFORM cf;
        cf.dwStyle        = CFS_POINT;
        cf.ptCurrentPos.x = pt.x;
        cf.ptCurrentPos.y = pt.y;
        ImmSetCompositionWindow(hImc, &cf);
        HEXOBJ hObj = pWnd->objFocus_;
        if (hObj != 0) {
            obj_s* pObj   = nullptr;
            INT    nError = 0;
            if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                LOGFONTW Logfont{0};
                _font_getlogfont(pObj->hFont_, &Logfont);
                ImmSetCompositionFontW(hImc, &Logfont);
                _obj_baseproc(hWnd, hObj, pObj, WM_IME_COMPOSITION, 0, 0);
            }
        }
        ImmReleaseContext(hWnd, hImc);
    }
}

BOOL _wnd_wm_measureitem_host(wnd_s* pWnd, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (wParam ==
        0)   // menu MEASUREITEMSTRUCT
             // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-measureitemstruct
    {
        if (((MEASUREITEMSTRUCT*)lParam)->CtlType == ODT_MENU) {
            INT   nID    = ((MEASUREITEMSTRUCT*)lParam)->itemID;
            RECT* ppc    = (RECT*)pWnd->padding_client_;
            INT   offset = 0;
            if (ppc != 0) {
                offset = offset + Ex_Scale(ppc->left + ppc->right);
            }
            RECT* ppt = (RECT*)pWnd->padding_text_;
            if (ppt != 0) {
                offset = offset + Ex_Scale(ppt->left + ppt->right);
            }
            INT  szItemSeparator = pWnd->szItemSeparator_;
            CHAR byte            = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_));
            INT  width, height;
            
            if (nID == 0)   // MF_SEPARATOR
            {
                width  = byte + offset;
                byte   = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_) + 3);
                height = Ex_Scale(byte);
            }
            else {
                width  = pWnd->menu_maxwidth_ + offset;
                byte   = __get_char(pWnd, offsetof(wnd_s, szItemSeparator_) + 1);
                height = Ex_Scale(byte);
            }
            ((MEASUREITEMSTRUCT*)lParam)->itemWidth  = width;
            ((MEASUREITEMSTRUCT*)lParam)->itemHeight = height;
            ret                                      = TRUE;
        }
    }
    return ret;
}

void _wnd_menu_updatecurrent(wnd_s* pWnd)
{

    wnd_s* pHost = pWnd->pMenuHostWnd_;
    if (pHost != 0) {
        pHost->pMenuTrackWnd_ = pWnd;
    }
}

BOOL _wnd_menu_mouse(HWND hWnd, wnd_s* pWnd, INT uMsg, WPARAM wParam, LONG_PTR* iItem)
{
    *iItem = -1;
    POINT pt;
    GetCursorPos(&pt);
    _wnd_wm_nchittest(pWnd, hWnd, MAKELONG(pt.x, pt.y));
    _wnd_wm_mouse(pWnd, hWnd, uMsg, wParam, MAKELONG(pt.x, pt.y));
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    BOOL   ret    = FALSE;
    if (_handle_validate(pWnd->objHittest_, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

        if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_FOCUSABLE) == OBJECT_STYLE_EX_FOCUSABLE)) {

            if (((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)) {
                *iItem = pObj->lParam_;
            }
            ret = TRUE;
        }
    }
    _wnd_menu_updatecurrent(pWnd);
    return ret;
}

void _wnd_wm_initmenupopup(HWND hWnd, wnd_s* pWnd, HMENU hMenu)
{
    BOOL fChecked = FALSE;
    if (!Flag_Query(ENGINE_FLAG_MENU_ALL)) {
        LPVOID lpMenuParams = pWnd->lpMenuParams_;
        if (!IsBadReadPtr(lpMenuParams, sizeof(menu_s))) {
            if (hMenu == (LPVOID)__get(lpMenuParams, 0)) {
                fChecked = TRUE;
            }
        }
    }
    else {
        fChecked = TRUE;
    }
    if (fChecked) {
        INT       nCount  = GetMenuItemCount(hMenu);
        HEXCANVAS hCanvas = pWnd->canvas_bkg_;
        HEXFONT   hFont   = pWnd->hFont_Menu_;

        WCHAR buff[520];
        FLOAT width, height, nMax = 0;
        for (INT i = 0; i < nCount; i++) {
            GetMenuStringW(hMenu, i, buff, 520, MF_BYPOSITION);
            _canvas_calctextsize(hCanvas, hFont, buff, -1, DT_SINGLELINE, 0, 0, &width, &height);
            if (nMax < width) {
                nMax = width;
            }
        }
        pWnd->menu_maxwidth_ = nMax;
        MENUITEMINFOW mii;
        mii.cbSize = sizeof(MENUITEMINFOW);
       
        mii.fMask  = MIIM_FTYPE | MIIM_ID;
        for (INT i = 0; i < nCount; i++) {
            if (GetMenuItemInfoW(hMenu, i, TRUE, &mii)) {
                if ((mii.fType & MFT_SEPARATOR) != 0) {
                    mii.wID = 0;
                }
                mii.fType = mii.fType | MFT_OWNERDRAW;
                if (SetMenuItemInfoW(hMenu, i, TRUE, &mii)) {
                    continue;
                }
            }
            fChecked = FALSE;
            break;
        }
        if (fChecked) {
            if (HashTable_Set(g_Li.hTableLayout, (size_t)hMenu, pWnd->hexdui_)) {
                pWnd->hMenuPopup_ = hMenu;
                pWnd->dwFlags_    = pWnd->dwFlags_ | EWF_BMENUINITED;
            }
        }
    }
}

BOOL Ex_TrackPopupMenu(HMENU hMenu, DWORD uFlags, INT x, INT y, size_t nReserved, EXHANDLE handle,
                       RECT* lpRC, MsgPROC pfnCallback, DWORD dwFlags)
{
    HWND   hWnd = 0;
    wnd_s* pWnd = nullptr;
    menu_s menu{};
    if (_wnd_getfromhandle(handle, &hWnd, &pWnd)) {
        menu.hMenu_         = hMenu;
        menu.uFlags_        = uFlags;
        menu.x_             = x;
        menu.y_             = y;
        menu.nReserved_     = nReserved;
        menu.handle_        = handle;
        menu.lpRC_          = lpRC;
        menu.pfnCallback_   = pfnCallback;
        menu.dwFlags_       = dwFlags;
        pWnd->lpMenuParams_ = &menu;
    }
    BOOL ret = TrackPopupMenu((HMENU)hMenu, uFlags, x, y, nReserved, hWnd, lpRC);
    return ret;
}

INT Ex_MessageBoxEx(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, LPCWSTR lpCheckBox,
                    BOOL* lpCheckBoxChecked, INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc)
{
    HWND   hWnd = 0;
    wnd_s* pWnd = nullptr;
    mbp_s  mbp{};
    if (_wnd_getfromhandle(handle, &hWnd, &pWnd)) {
        mbp.handle_             = handle;
        mbp.lpText_             = lpText;
        mbp.lpCaption_          = lpCaption;
        mbp.uType_              = uType;
        mbp.lpCheckBox_         = lpCheckBox;
        mbp.lpCheckBoxChecked_  = lpCheckBoxChecked;
        mbp.dwMilliseconds_     = dwMilliseconds;
        mbp.dwFlags_            = dwFlags;
        mbp.lpfnNotifyCallback_ = lpfnMsgProc;

        pWnd->lpMsgParams_ = &mbp;
    }
    auto ret = 0;
    if (dwMilliseconds) {
        ret = MessageBoxTimeoutW(hWnd, lpText, lpCaption, uType, 0, dwMilliseconds);
    }
    else {
        ret = MessageBoxW(hWnd, lpText, lpCaption, uType);
    }
    return ret;
}

INT Ex_MessageBox(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, INT dwFlags)
{
    return Ex_MessageBoxEx(handle, lpText, lpCaption, uType, 0, 0, 0, dwFlags, 0);
}

BOOL Ex_DUITrayIconPopup(HEXDUI hExDui, LPCWSTR lpwzInfo, LPCWSTR lpwzInfoTitle, DWORD dwInfoFlags)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        LPVOID lpNid = pWnd->lpNid_;
        if (lpNid != 0) {
            ((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_INFO;
            RtlZeroMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), 512);
            INT len = lstrlenW(lpwzInfo);
            if (len > 255) len = 255;
            RtlMoveMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfo)), lpwzInfo,
                          len * 2);
            RtlZeroMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)), 128);
            len = lstrlenW(lpwzInfoTitle);
            if (len > 63) len = 63;
            RtlMoveMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szInfoTitle)),
                          lpwzInfoTitle, len * 2);
            ((NOTIFYICONDATAW*)lpNid)->dwInfoFlags = dwInfoFlags;
            Shell_NotifyIconW(NIM_MODIFY, (PNOTIFYICONDATAW)lpNid);
        }
    }
    return nError == 0;
}

BOOL Ex_DUITrayIconSet(HEXDUI hExDui, HICON hIcon, LPCWSTR lpwzTips)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        HWND   hWnd  = pWnd->hWnd_;
        INT    len   = lstrlenW(lpwzTips);
        LPVOID lpNid = pWnd->lpNid_;
        if (lpNid == 0) {
            if (hIcon == 0) {
                hIcon = _wnd_geticonhandle(hWnd, FALSE);
            }
            LPVOID lpNid = Ex_MemAlloc(sizeof(NOTIFYICONDATAW));

            ((NOTIFYICONDATAW*)lpNid)->cbSize = sizeof(NOTIFYICONDATAW);
            ((NOTIFYICONDATAW*)lpNid)->hWnd   = hWnd;
            ((NOTIFYICONDATAW*)lpNid)->uID    = 1;
            ((NOTIFYICONDATAW*)lpNid)->uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
            ((NOTIFYICONDATAW*)lpNid)->uCallbackMessage = g_Li.dwMessage;
            ((NOTIFYICONDATAW*)lpNid)->hIcon            = hIcon;
            ((NOTIFYICONDATAW*)lpNid)->uVersion         = NOTIFYICON_VERSION_4;
            if (len != 0) {
                if (len > 127) len = 127;
                RtlMoveMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), lpwzTips,
                              len * 2);
            }
            Shell_NotifyIconW(NIM_ADD, (PNOTIFYICONDATAW)lpNid);
            pWnd->lpNid_ = lpNid;
        }
        else {
            INT uFlags = 0;
            if (len != 0) {
                if (len > 127) len = 127;
                RtlZeroMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), 127);
                RtlMoveMemory((LPVOID)((size_t)lpNid + offsetof(NOTIFYICONDATAW, szTip)), lpwzTips,
                              len * 2);
                uFlags = NIF_TIP;
            }
            if (hIcon != 0) {
                ((NOTIFYICONDATAW*)lpNid)->hIcon = (HICON)hIcon;
                uFlags                           = uFlags | NIF_ICON;
            }
            if (uFlags != 0) {
                ((NOTIFYICONDATAW*)lpNid)->uFlags = uFlags;
                Shell_NotifyIconW(NIM_MODIFY, (PNOTIFYICONDATAW)lpNid);
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_DUIGetBackgroundImage(HEXDUI hExDui, LPVOID lpBackgroundImage)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        if (IsBadWritePtr(lpBackgroundImage, sizeof(EX_BACKGROUNDIMAGEINFO))) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            RtlMoveMemory(lpBackgroundImage, pWnd->lpBackgroundImage_,
                          sizeof(EX_BACKGROUNDIMAGEINFO));
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _wnd_getclientrect(wnd_s* pWnd, RECT* lpRect, INT* nError)
{
    if (IsBadWritePtr(lpRect, 16)) {
        *nError = ERROR_EX_BAD_SIZE;
    }
    else {
        RtlZeroMemory(lpRect, 16);
        lpRect->right  = pWnd->width_;
        lpRect->bottom = pWnd->height_;
        if (Flag_Query(ENGINE_FLAG_DPI_ENABLE)) {
            lpRect->right  = (FLOAT)lpRect->right / g_Li.DpiX;
            lpRect->bottom = (FLOAT)lpRect->bottom / g_Li.DpiY;
        }
    }
}

BOOL Ex_DUIGetClientRect(HEXDUI hExDui, RECT* lpClientRect)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        _wnd_getclientrect(pWnd, lpClientRect, &nError);
    }
    return nError == 0;
}

size_t Ex_DUIGetLong(HEXDUI hExDui, INT nIndex)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    size_t ret    = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        if (nIndex == ENGINE_LONG_ALPHA) {
            ret = pWnd->alpha_;
        }
        else if (nIndex == ENGINE_LONG_BLUR) {
            ret = pWnd->fBlur_;
        }
        else if (nIndex == ENGINE_LONG_CRBKG) {
            ret = (size_t)pWnd->crBkg_;
        }
        else if (nIndex == ENGINE_LONG_CRBORDER) {
            ret = (size_t)pWnd->crBorder_;
        }
        else if (nIndex == ENGINE_LONG_CRSD) {
            ret = (size_t)pWnd->crSD_;
        }
        else if (nIndex == ENGINE_LONG_HTHEME) {
            ret = (size_t)pWnd->hTheme_;
        }
        else if (nIndex == ENGINE_LONG_HWND) {
            ret = (size_t)pWnd->hWnd_;
        }
        else if (nIndex == ENGINE_LONG_LPARAM) {
            ret = pWnd->lParam_;
        }
        else if (nIndex == ENGINE_LONG_MINHEIGHT) {
            ret = pWnd->minHeight_;
        }
        else if (nIndex == ENGINE_LONG_MINWIDTH) {
            ret = pWnd->minWidth_;
        }
        else if (nIndex == ENGINE_LONG_MSGPROC) {
            ret = (size_t)pWnd->pfnMsgProc_;
        }
        else if (nIndex == ENGINE_LONG_OBJCAPTION) {
            ret = (size_t)pWnd->objCaption_;
        }
        else if (nIndex == ENGINE_LONG_OBJFOCUS) {
            ret = (size_t)pWnd->objFocus_;
        }
        else if (nIndex == ENGINE_LONG_RADIUS) {
            ret = (size_t)pWnd->Radius_;
        }
        else if (nIndex == -143) {
            ret = (size_t)pWnd->vol_wndptr_;
        }
        else {
            EX_ASSERT(false, L"Ex_DUIGetLong: unknown EWL index: %ld", nIndex);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

size_t Ex_DUISetLong(HEXDUI hExDui, INT nIndex, size_t dwNewLong)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    size_t ret    = 0;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        BOOL bRedraw = FALSE;
        if (nIndex == ENGINE_LONG_ALPHA) {
            ret          = (size_t)pWnd->alpha_;
            pWnd->alpha_ = dwNewLong;
            FLAGS_DEL(pWnd->dwFlags_, EWF_BRENDERED);   // 强制阴影重画
        }
        else if (nIndex == ENGINE_LONG_BLUR) {
            ret          = pWnd->fBlur_;
            pWnd->fBlur_ = dwNewLong;
            bRedraw      = TRUE;
        }
        else if (nIndex == ENGINE_LONG_CRBKG) {
            ret            = (size_t)pWnd->crBkg_;
            pWnd->crBkg_   = dwNewLong;
            pWnd->dwStyle_ = pWnd->dwStyle_ | WINDOW_STYLE_NOINHERITBKG;
            bRedraw        = TRUE;
        }
        else if (nIndex == ENGINE_LONG_CRBORDER) {
            ret             = (size_t)pWnd->crBorder_;
            pWnd->crBorder_ = dwNewLong;
            bRedraw         = TRUE;
        }
        else if (nIndex == ENGINE_LONG_HTHEME) {
            ret           = (size_t)pWnd->hTheme_;
            pWnd->hTheme_ = (HEXTHEME)dwNewLong;
            bRedraw       = TRUE;
        }
        else if (nIndex == ENGINE_LONG_HWND) {
            ret         = (size_t)pWnd->hWnd_;
            pWnd->hWnd_ = (HWND)dwNewLong;
        }
        else if (nIndex == ENGINE_LONG_LPARAM) {
            ret           = pWnd->lParam_;
            pWnd->lParam_ = dwNewLong;
        }
        else if (nIndex == ENGINE_LONG_MINHEIGHT) {
            ret              = pWnd->minHeight_;
            pWnd->minHeight_ = dwNewLong;
            bRedraw          = TRUE;
        }
        else if (nIndex == ENGINE_LONG_MINWIDTH) {
            ret             = pWnd->minWidth_;
            pWnd->minWidth_ = dwNewLong;
            bRedraw         = TRUE;
        }
        else if (nIndex == ENGINE_LONG_MSGPROC) {
            ret               = (size_t)pWnd->pfnMsgProc_;
            pWnd->pfnMsgProc_ = (WinMsgPROC)dwNewLong;
            bRedraw           = TRUE;
        }
        else if (nIndex == ENGINE_LONG_OBJCAPTION) {
            ret               = pWnd->objCaption_;
            pWnd->objCaption_ = dwNewLong;
            bRedraw           = TRUE;
        }
        else if (nIndex == ENGINE_LONG_OBJFOCUS) {
            ret             = pWnd->objFocus_;
            pWnd->objFocus_ = dwNewLong;
            bRedraw         = TRUE;
        }
        else if (nIndex == ENGINE_LONG_CRSD) {
            ret         = (size_t)pWnd->crSD_;
            pWnd->crSD_ = dwNewLong;
            bRedraw     = TRUE;
        }
        else if (nIndex == ENGINE_LONG_RADIUS) {
            ret           = (size_t)pWnd->Radius_;
            pWnd->Radius_ = dwNewLong;
            bRedraw       = TRUE;
        }
        else if (nIndex == -143) {
            ret               = (size_t)pWnd->vol_wndptr_;
            pWnd->vol_wndptr_ = (void*)dwNewLong;
            bRedraw           = TRUE;
        }
        else {
            EX_ASSERT(false, L"Ex_DUISetLong: unknown EWL index: %ld", nIndex);
        }
        if (bRedraw) {
            pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_BREDRAWBACKGROUND;
            _wnd_redraw_bkg(pWnd->hWnd_, pWnd, 0, TRUE, FALSE);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_DUIShowWindowEx(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags,
                        INT uEasing, WPARAM wParam, LPARAM lParam)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    BOOL   ret    = FALSE;
    if (_handle_validate(hExDui, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        HWND hWnd = pWnd->hWnd_;
        // 刷新组件层
        pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_INTED;
        if (FLAGS_CHECK(pWnd->dwStyle_, WINDOW_STYLE_POPUPWINDOW)) {   // 弹出窗不获取焦点
            nCmdShow = SW_SHOWNOACTIVATE;
        }
        ret = ShowWindow(hWnd, nCmdShow);
        if (GetFocus() == hWnd) {
            FLAGS_ADD(pWnd->dwFlags_, EWF_ACTIVE);
        }
        auto atomName = GetClassLongPtrW(hWnd, GCW_ATOM);
        if (atomName == ATOM_DIALOG)   // 判断是模态窗口,添加模态窗口风格
        {
            SetFocus(hWnd);
        }
        ret = InvalidateRect(hWnd, NULL, FALSE);
        ret = UpdateWindow(hWnd);
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_DUIShowWindow(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags)
{
    return Ex_DUIShowWindowEx(hExDui, nCmdShow, dwTimer, dwFrames, dwFlags, 0, 0, 0);
}

HEXDUI Ex_DUIBindWindowEx(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle, LPARAM lParam,
                          WinMsgPROC lpfnMsgProc)
{
    INT    nError = 0;
    wnd_s* pWnd   = nullptr;
    HEXDUI hExDui = 0;
    if (IsWindow(hWnd)) {
        pWnd = (wnd_s*)Ex_MemAlloc(sizeof(wnd_s));
        if (pWnd != 0) {
            hExDui = _handle_create(HT_DUI, pWnd, &nError);

            if (hExDui != 0) {
                nError = _wnd_create(hExDui, pWnd, hWnd, dwStyle, hTheme, lParam, lpfnMsgProc);
            }
        }
        else {
            nError = ERROR_EX_MEMORY_ALLOC;
        }
    }
    else {
        nError = GetLastError();
    }
    if (nError != 0) {
        EX_ASSERT(FALSE, L"wnd create error: %02X", nError);
        if (pWnd != 0) {
            Ex_MemFree(pWnd);
        }
        if (hExDui != 0) {
            nError = 0;
            _handle_destroy(hExDui, &nError);
        }
    }
    Ex_SetLastError(nError);
    return hExDui;
}

HEXDUI Ex_DUIBindWindow(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle)
{
    return Ex_DUIBindWindowEx(hWnd, hTheme, dwStyle, 0, 0);
}

HEXOBJ Ex_DUIGetObjFromPoint(EXHANDLE handle, INT x, INT y)
{
    wnd_s* pWnd;
    RECT   rect = {0};
    HWND   hWnd = 0;
    obj_s* pObj = 0;
    POINT  point{x, y};
    if (!handle) {
        GetCursorPos(&point);
        HWND hWnd = WindowFromPoint(point);
        if (hWnd) {
            GetWindowRect(hWnd, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
        }
    }
    else if (_wnd_getfromhandle((size_t)handle, &hWnd, &pWnd)) {
        INT hitCode = 0;
        _wnd_wm_nchittest_obj(hWnd, pWnd, pWnd->objChildLast_, point.x, point.y, &hitCode, &pObj);
        if (pObj) {
            return pObj->hObj_;
        }
    }
    return 0;
}
