#include "stdafx.h"

LRESULT CALLBACK _hook_proc(INT code, WPARAM wParam, LPARAM lParam)
{
    if (code == 3) //HCBT_CREATEWND
    {
        return _hook_oncreate(code, (HWND)wParam, lParam);
    }
    return CallNextHookEx(g_Li.hHookMsgBox, code, wParam, lParam);
}

LRESULT _hook_oncreate(INT code, HWND hWnd, LPARAM lParam)
{
    auto lpcs = ((CBT_CREATEWND *)lParam)->lpcs;

    EXATOM atomClass = (EXATOM)(lpcs->lpszClass);
    auto hParent = lpcs->hwndParent;

    if (atomClass == ATOM_DIALOG)
    {
        auto hExDui = Ex_DUIFromWindow(hParent);
        wnd_s *pWnd = nullptr;
        INT nError = 0;
        if (_handle_validate(hExDui, HT_DUI, (LPVOID *)&pWnd, &nError))
        {
            mbp_s *pMsg = pWnd->lpMsgParams_;
            pWnd->lpMsgParams_ = 0;
            if (pMsg != 0)
            {
                SetClassLongPtrW(hWnd, GCLP_HCURSOR, (LONG_PTR)g_Li.hCursor);
				SetClassLongPtrW(hWnd, GCLP_HICONSM, (LONG_PTR)g_Li.hIconsm);
                INT style = WINDOW_STYLE_TITLE | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_MESSAGEBOX;

                if (((pMsg->dwFlags_ & MESSAGEBOX_FLAG_WINDOWICON) == MESSAGEBOX_FLAG_WINDOWICON))
                {
                    style = style | WINDOW_STYLE_HASICON;
                }
				if (((pMsg->dwFlags_ & MESSAGEBOX_FLAG_NOINHERITBKG) == MESSAGEBOX_FLAG_NOINHERITBKG))
				{
					style = style | WINDOW_STYLE_NOINHERITBKG;
				}
                Ex_DUIBindWindowEx(hWnd, pWnd->hTheme_, style, (size_t)pMsg, pMsg->lpfnNotifyCallback_);
            }
        }
    }
    else if (atomClass == ATOM_MENU)
    {
        Thunkwindow(hWnd, _menu_proc, 0, 0);
    }
    return CallNextHookEx(g_Li.hHookMsgBox, code, (WPARAM)hWnd, lParam);
}

LRESULT CALLBACK _menu_proc(EX_THUNK_DATA *pData, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = pData->hWnd;
    WNDPROC pOld = pData->Proc;
    wnd_s *pWnd = (wnd_s *)pData->dwData;
    if (uMsg == WM_DESTROY)
    {
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (size_t)pOld);
        VirtualFree(pData, 0, MEM_RELEASE);
    }
    else if (uMsg == 482) //MN_SIZEWINDOW
    {
        _menu_init(hWnd);
    }
    return CallWindowProcW(pOld, hWnd, uMsg, wParam, lParam);
}

void _menu_init(HWND hWnd)
{
    LPVOID hMenu = (LPVOID)SendMessageW(hWnd, MN_GETHMENU, 0, 0);
    if (hMenu != 0)
    {
        size_t hExDui; // 由于HashTable_Get会写入8字节，这里必须是size_t类型
        HashTable_Get(g_Li.hTableLayout, (size_t)hMenu, &hExDui);
        wnd_s *pWnd = nullptr;
        INT nError = 0;
        if (_handle_validate(hExDui, HT_DUI, (LPVOID *)&pWnd, &nError))
        {
            HashTable_Remove(g_Li.hTableLayout, (size_t)hMenu);
            menu_s *lpMenuParams = pWnd->lpMenuParams_;
            SetWindowLongPtrW(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
            SetClassLongPtrW(hWnd, GCL_STYLE, CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS);
            INT dwStyle = WINDOW_STYLE_MENU | WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_ESCEXIT | WINDOW_STYLE_FULLSCREEN;
            MsgPROC pfnCallback = nullptr;
            if (!IsBadReadPtr(lpMenuParams, sizeof(menu_s)))
            {
                if (((lpMenuParams->dwFlags_ & MENU_FLAG_NOSHADOW) == MENU_FLAG_NOSHADOW))
                {
                    dwStyle = dwStyle | WINDOW_STYLE_NOSHADOW;
                }
                pfnCallback = lpMenuParams->pfnCallback_;
            }
            HEXDUI hExDui = Ex_DUIBindWindowEx(hWnd, pWnd->hTheme_, dwStyle, (size_t)pWnd, pfnCallback);
            if (_handle_validate(hExDui, HT_DUI, (LPVOID *)&pWnd, &nError))
            {
                pWnd->lpMenuParams_ = lpMenuParams;
                pWnd->hMenuPopup_ = hMenu;
                pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_INTED;
            }
        }
    }
}

void _msgbox_drawinfo(wnd_s *pWnd, HEXCANVAS cvBkg)
{
    INT w, h;
    _canvas_getsize(cvBkg, &w, &h);
    HEXTHEME hTheme = pWnd->hTheme_;
    Ex_ThemeDrawControl(hTheme, cvBkg, 0, 0, w, h, ATOM_MESSAGEBOX, ATOM_RECT, 255);
    mbp_s *pMsg = pWnd->lpMsgParams_;
    if (pMsg != 0)
    {
        INT wType = pMsg->uType_ & 240;

        INT l = Ex_Scale(15);
        INT t = Ex_Scale(15);
        obj_s *pCaption = nullptr;
        INT nError = 0;
        if (_handle_validate(pWnd->objCaption_, HT_OBJECT, (LPVOID *)&pCaption, &nError))
        {
            t = t + pCaption->bottom_;
        }

        if (wType != 0)
        {
            WCHAR value[10];
            _itow_s(wType, value, 10, 10);
            LPVOID pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, Ex_Atom(value));
            INT r = NULL, b = NULL;
            if (pValue != 0)
            {
                r = l + Ex_Scale(__get_int(pValue, 8) - __get_int(pValue, 0));
                b = t + Ex_Scale(__get_int(pValue, 12) - __get_int(pValue, 4));
                Ex_ThemeDrawControlEx(hTheme, cvBkg, l, t, r, b, ATOM_MESSAGEBOX, Ex_Atom(value), 0, 0, 0, 0, 255);
            }
            l = r + Ex_Scale(15);
        }
        LPCWSTR pText = pMsg->lpText_;
        if (pText != 0)
        {
            HEXFONT hFont = _font_create();
            if (hFont != 0)
            {
                _canvas_drawtext(cvBkg, hFont, -16777216, pText, -1, DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL, l, t, w - Ex_Scale(15), h);
                _font_destroy(hFont);
            }
        }
    }
}

void _msgbox_initdialog(HWND hWnd, wnd_s *pWnd, WPARAM wParam, LPARAM lParam)
{
    mbp_s *pMsg = pWnd->lpMsgParams_;
    if (pMsg == 0)
        return;
    auto lpwzCheckbox = pMsg->lpCheckBox_;
    auto lpCheckboxChecked = pMsg->lpCheckBoxChecked_;
    auto uType = pMsg->uType_;

    auto pfnCallback = pMsg->lpfnNotifyCallback_;
    auto hTheme = pWnd->hTheme_;
    auto hWndChild = GetWindow(hWnd, GW_CHILD);
    RECT rcText = {0};
    std::vector<std::wstring> aryText;
    std::vector<size_t> aryID;
    size_t iDef = 0;
    while (hWndChild != 0)
    {
        auto i = GetWindowLongPtrW(hWndChild, GWL_ID);
        if (i == 65535) //内容
        {
            GetWindowRect(hWndChild, &rcText);
        }
        else if (i == 20) //图标
        {
        }
        else
        {
            aryText.push_back(_wnd_gettitle(hWndChild));
            aryID.push_back(i);
            if (_wnd_querystyle(hWndChild, BS_DEFPUSHBUTTON, FALSE))
                iDef = i;
        }
        DestroyWindow(hWndChild);
        hWndChild = GetWindow(hWnd, GW_CHILD);
    }
    auto n = aryID.size();

    INT maxWidth = Ex_Scale(10) * 2 + Ex_Scale(80) * n + Ex_Scale(5) * n;
    HEXCANVAS hCanvas = pWnd->canvas_bkg_;
    HEXFONT hFont = _font_create();
    INT width = rcText.right - rcText.left;
    FLOAT w = NULL;
    FLOAT h = NULL;
    FLOAT widthCheckbox = NULL;
    FLOAT heightCheckbox = NULL;
    if (hFont != 0 && hCanvas != 0)
    {
        if (lpwzCheckbox != 0)
        {
            if (_canvas_begindraw(hCanvas))
            {
                _canvas_calctextsize(hCanvas, hFont, lpwzCheckbox, -1, DT_LEFT | DT_SINGLELINE, 0, 0, 0, &widthCheckbox, &heightCheckbox);
                _canvas_enddraw(hCanvas);
            }
            widthCheckbox = widthCheckbox + 16 + 8;
            maxWidth = maxWidth + Ex_Scale(widthCheckbox);
        }

        if (_canvas_begindraw(hCanvas))
        {
            _canvas_calctextsize(hCanvas, hFont, pMsg->lpText_, -1, DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL, 0, width, rcText.bottom - rcText.top, &w, &h);
            _canvas_enddraw(hCanvas);
        }
        _font_destroy(hFont);
        if (w > width)
            width = w;
    }
    if (!Flag_Query(ENGINE_FLAG_DPI_ENABLE))
    {
        width = (FLOAT)width / g_Li.DpiX_Real;
    }
    width = width + Ex_Scale(20) * 2;
    WCHAR bin[10];
    _itow_s(MB_ICONERROR, bin, 10, 10);
    auto pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, Ex_Atom(bin));

    if ((uType & 240) != 0)
    {
        width = width + Ex_Scale(__get_int(pValue, 8) - __get_int(pValue, 0)) + Ex_Scale(15);
    }
    if (width > maxWidth)
        maxWidth = width;
    INT height = Ex_Scale(__get_int(pValue, 12) - __get_int(pValue, 4));
    INT maxHeight = h;
    if (maxHeight < height)
        maxHeight = height;
    pValue = Ex_ThemeGetValuePtr(hTheme, ATOM_MESSAGEBOX, ATOM_BACKGROUND_GRID);
    maxHeight = Ex_Scale(__get_int(pValue, 4)) + Ex_Scale(__get_int(pValue, 12)) + maxHeight + Ex_Scale(15) * 2;
    if (maxHeight < Ex_Scale(140))
        maxHeight = Ex_Scale(140);
    if (maxWidth < Ex_Scale(220))
        maxWidth = Ex_Scale(220);
    RECT rcWindow{0};
    GetWindowRect(hWnd, &rcWindow);
    INT left = rcWindow.right - rcWindow.left;
    INT top = rcWindow.bottom - rcWindow.top;
    MoveWindow(hWnd, rcWindow.left - (maxWidth - left) / 2, rcWindow.top - (maxHeight - top) / 2, maxWidth, maxHeight, 0);
    if (pfnCallback != 0)
    {
        _wnd_dispatch_notify(hWnd, pWnd, pWnd->hexdui_, -1, NM_INTDLG, 0, 0, 0);
    }
    width = Ex_Scale(80);
    height = Ex_Scale(24);
    GetWindowRect(hWnd, &rcWindow);
    maxWidth = rcWindow.right - rcWindow.left;
    maxHeight = rcWindow.bottom - rcWindow.top;
    top = (FLOAT)maxHeight / g_Li.DpiY - __get_int(pValue, 12) + (FLOAT)(__get_int(pValue, 12) - 24) / 2;
    left = (FLOAT)maxWidth / g_Li.DpiX - 85;

    obj_s *pObj = nullptr;
    for (INT i = aryID.size() - 1; i >= 0; i--)
    {
        INT nError = 0;
        HEXOBJ hObj = _obj_create_init(hWnd, pWnd, ATOM_BUTTON, 0, &pObj, &nError);
        if (hObj != 0)
        {
            _obj_create_proc(&nError, TRUE, hTheme, pObj, -1, ATOM_BUTTON, aryText[i].c_str(), -1, left, top, 80, 24, 0, ~aryID[i], 0, aryID[i], -1);
            pObj->dwFlags_ = pObj->dwFlags_ | EOF_BMSGBOXCONTROL;
            _obj_create_done(hWnd, pWnd, hObj, pObj);
            if (aryID[i] == iDef)
                _obj_setfocus(hWnd, pWnd, hObj, pObj, TRUE);
        }
        left = left - 85;
    }
    if (lpwzCheckbox != 0)
    {
        INT nError = 0;
        HEXOBJ hObj = _obj_create_init(hWnd, pWnd, ATOM_CHECKBUTTON, 0, &pObj, &nError);
        if (hObj != 0)
        {
            _obj_create_proc(&nError, TRUE, hTheme, pObj, -1, ATOM_CHECKBUTTON, lpwzCheckbox, -1, 4, top, widthCheckbox, 24, 0, (size_t)lpCheckboxChecked, 0, (size_t)lpCheckboxChecked, -1);
            _obj_create_done(hWnd, pWnd, hObj, pObj);
            pMsg->CheckBoxhObj_ = hObj;
            if (__get_int(lpCheckboxChecked, 0) != 0)
            {
                _obj_baseproc(hWnd, hObj, pObj, BM_SETCHECK, 1, 0);
            }
        }
    }

    INT wType = uType & 7;

    if (wType == MB_ABORTRETRYIGNORE || wType == MB_YESNO)
    {
        Ex_ObjEnable(Ex_ObjGetFromID(pWnd->objCaption_, WINDOW_STYLE_BUTTON_CLOSE), FALSE);
    }

    if (((pMsg->dwFlags_ & MESSAGEBOX_FLAG_CENTEWINDOW) == MESSAGEBOX_FLAG_CENTEWINDOW))
    {
        Ex_WndCenterFrom(hWnd, (HWND)GetWindowLongPtrW(hWnd, GWLP_HWNDPARENT), FALSE);
    }
    else
    {
        Ex_WndCenterFrom(hWnd, 0, FALSE); //检查是否同个屏幕
    }
    pWnd->dwFlags_ = pWnd->dwFlags_ | EWF_INTED;
    ShowWindow(hWnd, 1);
    InvalidateRect(hWnd, 0, FALSE);
    UpdateWindow(hWnd);
}