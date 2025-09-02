#include "stdafx.h"

void _button_regsiter()
{
    Ex_ObjRegister(L"Button", OBJECT_STYLE_VISIBLE | BUTTON_STYLE_TEXTOFFSET,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                   DT_VCENTER | DT_CENTER | DT_SINGLELINE, sizeof(SIZE_T), 0, 0, _button_proc);
    Ex_ObjRegister(L"CheckButton", OBJECT_STYLE_VISIBLE | BUTTON_STYLE_CHECKBUTTON,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE,
        sizeof(SIZE_T), 0, 0, _checkbutton_proc);
    Ex_ObjRegister(L"RadioButton", OBJECT_STYLE_VISIBLE | BUTTON_STYLE_RADIOBUTTON,
                   OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE,
        sizeof(SIZE_T), 0, 0, _radiobutton_proc);
}

// Button处理函数
LRESULT CALLBACK _button_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, BUTTON_LONG_STATE, 0); // 0:正常 1:悬停 2:按下
    }
    else if (uMsg == WM_PAINT) {
        _button_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto state = (INT)Ex_ObjGetLong(hObj, BUTTON_LONG_STATE);
        if (state == 0) {
            Ex_ObjSetLong(hObj, BUTTON_LONG_STATE, 1);
            Ex_ObjInvalidateRect(hObj, 0);
        }
        Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, TRUE);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto state = (INT)Ex_ObjGetLong(hObj, BUTTON_LONG_STATE);
        if (state == 1) {
            Ex_ObjSetLong(hObj, BUTTON_LONG_STATE, 0);
            Ex_ObjInvalidateRect(hObj, 0);
        }
        Ex_ObjSetUIState(hObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetLong(hObj, BUTTON_LONG_STATE, 2);
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_LBUTTONUP) {
        auto state = (INT)Ex_ObjGetLong(hObj, BUTTON_LONG_STATE);
        if (state == 2) {
            Ex_ObjSetLong(hObj, BUTTON_LONG_STATE, 1);
            Ex_ObjInvalidateRect(hObj, 0);
        }
        INT    nError = 0;
        obj_s* pObj = nullptr;
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            if ((pObj->dwFlags_ & EOF_BMSGBOXCONTROL) == EOF_BMSGBOXCONTROL) {
                //处理信息框按钮关闭弹窗
                return EndDialog(hWnd, pObj->lParam_);
            }
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// CheckButton处理函数
LRESULT CALLBACK _checkbutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, CHECKBUTTON_LONG_STATE, 0); // 0:正常 1:悬停 2:选中
    }
    else if (uMsg == WM_PAINT) {
        _checkbutton_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto state = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        if (state == 0) {
            Ex_ObjSetLong(hObj, CHECKBUTTON_LONG_STATE, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto state = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        if (state == 1) {
            Ex_ObjSetLong(hObj, CHECKBUTTON_LONG_STATE, 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        auto state = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        if (state == 2) {
            Ex_ObjSetLong(hObj, CHECKBUTTON_LONG_STATE, 1); // 取消选中
        }
        else if (state == 1) {
            Ex_ObjSetLong(hObj, CHECKBUTTON_LONG_STATE, 2); // 选中
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_EX_LCLICK)
    {
        auto check = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        Ex_ObjDispatchNotify(hObj, NM_CHECK, check == 2, lParam);
    }
    else if (uMsg == BM_GETCHECK) {
        auto state = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        return (state == 2) ? 1 : 0;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// RadioButton处理函数
LRESULT CALLBACK _radiobutton_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 0); // 0:正常 1:悬停 2:选中
    }
    else if (uMsg == WM_PAINT) {
        _radiobutton_paint(hObj);
    }
    else if (uMsg == WM_MOUSEHOVER) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTON_LONG_STATE);
        if (state == 0) {
            Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 1);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_MOUSELEAVE) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTON_LONG_STATE);
        if (state == 1) {
            Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 0);
        }
        Ex_ObjInvalidateRect(hObj, 0);
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTON_LONG_STATE);
        if (state != 2) {
            // 单选框被点击且未选中时，设置为选中状态并通知兄弟控件
            Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 2);
            Ex_ObjInvalidateRect(hObj, 0);
            if (wParam == 1)
            {
                // 通知兄弟控件取消选中
                INT    nError = 0;
                obj_s* pObj = nullptr;
                if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
                    _obj_notify_brothers(hWnd, hObj, pObj, BM_SETCHECK, 0, 0, TRUE, TRUE);
                }
            }
            // 发送通知
            Ex_ObjDispatchNotify(hObj, NM_CHECK, wParam, lParam);
        }
    }
    else if (uMsg == BM_SETCHECK) {
        if (wParam == 0) {
            Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 0);
        }
        else {
            Ex_ObjSetLong(hObj, RADIOBUTTON_LONG_STATE, 2);
        }
        Ex_ObjInvalidateRect(hObj, 0);
        return 0;
    }
    else if (uMsg == BM_GETCHECK) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTON_LONG_STATE);
        return (state == 2 || state == 3) ? 1 : 0;
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// Button绘制函数
void _button_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        auto state = (INT)Ex_ObjGetLong(hObj, BUTTON_LONG_STATE);
        INT atomClass = ATOM_BUTTON;
        INT atomProp;
        INT nIndex;
        // 根据状态选择颜色
        if (state == 2) { // 按下
            atomProp = ATOM_DOWN;
            nIndex = COLOR_EX_TEXT_DOWN;
        }
        else if (state == 1) { // 悬停
            atomProp = ATOM_HOVER;
            nIndex = COLOR_EX_TEXT_HOVER;
        }
        else { // 正常
            atomProp = ATOM_NORMAL;
            nIndex = COLOR_EX_TEXT_NORMAL;
        }

        if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass,
                atomProp, 255);
            if ((ps.dwState & STATE_FOCUS) != 0) {
                Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight,
                    atomClass, ATOM_FOCUS, 255);
            }
        }

        // 绘制文本
        LPCWSTR lptext = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);
        if (lptext != 0) {
            if (state == 2 && (ps.dwStyle & BUTTON_STYLE_TEXTOFFSET) != 0)
            {
                OffsetRect((LPRECT)&ps.rcText.left, Ex_Scale(1), Ex_Scale(1));
            }
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, nIndex), lptext, -1,
                ps.dwTextFormat, ps.rcText.left, (FLOAT)ps.rcText.top,
                (FLOAT)ps.rcText.right, (FLOAT)ps.rcText.bottom);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
}

// CheckButton绘制函数
void _checkbutton_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        auto state = (INT)Ex_ObjGetLong(hObj, CHECKBUTTON_LONG_STATE);
        INT atomClass =  ATOM_CHECKBUTTON;
        INT atomProp;
        INT nIndex;
        // 根据状态选择颜色
        if (state == 2) { // 选中
            atomProp = ATOM_CHECK_DOWN;
            nIndex = COLOR_EX_TEXT_CHECKED;
        }
        else if (state == 1) { // 悬停
            atomProp = ATOM_HOVER;
            nIndex = COLOR_EX_TEXT_HOVER;
        }
        else { // 正常
            atomProp = ATOM_NORMAL;
            nIndex = COLOR_EX_TEXT_NORMAL;
        }

        if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass,
                atomProp, 255);
        }

        // 绘制文本
        LPCWSTR lptext = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);
       
        if (lptext != 0) {
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, nIndex), lptext, -1,
                ps.dwTextFormat, ps.rcText.left, (FLOAT)ps.rcText.top,
                (FLOAT)ps.rcText.right, (FLOAT)ps.rcText.bottom);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
}

// RadioButton绘制函数
void _radiobutton_paint(HEXOBJ hObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        auto state = (INT)Ex_ObjGetLong(hObj, RADIOBUTTON_LONG_STATE);
        INT atomClass = ATOM_RADIOBUTTON;
        INT atomProp;
        INT nIndex;
        // 根据状态选择颜色
        if (state == 2) { // 选中
            atomProp = ATOM_CHECK_DOWN;
            nIndex = COLOR_EX_TEXT_CHECKED;
        }
        else if (state == 1) { // 悬停
            atomProp = ATOM_HOVER;
            nIndex = COLOR_EX_TEXT_HOVER;
        }
        else { // 正常
            atomProp = ATOM_NORMAL;
            nIndex = COLOR_EX_TEXT_NORMAL;
        }

        if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, atomClass,
                atomProp, 255);
        }

        // 绘制文本
        LPCWSTR lptext = (LPCWSTR)Ex_ObjGetLong(hObj, OBJECT_LONG_LPWZTITLE);

        if (lptext != 0) {
            _canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, nIndex), lptext, -1,
                ps.dwTextFormat, ps.rcText.left, (FLOAT)ps.rcText.top,
                (FLOAT)ps.rcText.right, (FLOAT)ps.rcText.bottom);
        }

        Ex_ObjEndPaint(hObj, &ps);
    }
}

void _item_regsiter()
{
    Ex_ObjRegister(L"Item", OBJECT_STYLE_VISIBLE,
        OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, DT_VCENTER | DT_SINGLELINE,
        0, 0, 0, _item_proc);
}

void _item_click(HWND hWnd, obj_s* pObj)
{
    if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM) {
        if ((pObj->dwStyle_ & OBJECT_STYLE_MENUITEM_SEPARATOR) == OBJECT_STYLE_MENUITEM_SEPARATOR) {
        }
        else if ((pObj->dwStyle_ & OBJECT_STYLE_MENUITEM_SUBMENU) ==
                 OBJECT_STYLE_MENUITEM_SUBMENU) {}
        else {
            wnd_s*        pWnd = pObj->pWnd_;
            MENUITEMINFOW mii;
            mii.cbSize = sizeof(MENUITEMINFOW);
            mii.fMask  = MIIM_STATE | MIIM_ID;
            if (GetMenuItemInfoW((HMENU)pWnd->hMenuPopup_, pObj->lParam_, TRUE, &mii)) {
                if ((mii.fState & MFS_GRAYED) == 0) {
                    PostMessageW(pWnd->pMenuHostWnd_->hWnd_, WM_COMMAND, mii.wID, 0);
                    ShowWindow(hWnd, 0);
                    EndMenu();
                }
            }
        }
    }
}

void _item_draw(obj_s* pObj, EX_PAINTSTRUCT ps, EXARGB crColor, LPCWSTR lpText)
{
    wnd_s*        pWnd  = pObj->pWnd_;
    HMENU         hMenu = (HMENU)pWnd->hMenuPopup_;
    MENUITEMINFOW mii;
    mii.cbSize = sizeof(MENUITEMINFOW);
    mii.fMask  = MIIM_STATE | MIIM_FTYPE | MIIM_DATA | MIIM_SUBMENU | MIIM_BITMAP;
    size_t nID = pObj->lParam_;
    RECT   rcItem{0};
    RECT   rcPadding{0};
    RECT   rcSub{0};
    rcItem.right  = ps.uWidth;
    rcItem.bottom = ps.uHeight;
    INT atomProp  = 0;
    if (GetMenuItemInfoW(hMenu, nID, TRUE, &mii)) {
        if ((mii.fType & MFT_SEPARATOR) != 0) {
            LPVOID lpPaddingSeparator = pWnd->padding_separator_;
            INT    tmp       = 0;
            RECT rcPaddingSeparator;
            if (lpPaddingSeparator != 0) {
                RtlMoveMemory(&rcPaddingSeparator, lpPaddingSeparator, 16);
                tmp = HIBYTE(HIWORD(pWnd->szItemSeparator_) -
                               (rcPaddingSeparator.top + rcPaddingSeparator.bottom));
            }
            RECT rcSeparator;
            rcSeparator.left   = rcPaddingSeparator.left;
            rcSeparator.top    = (rcItem.bottom - tmp) / 2;
            rcSeparator.right  = rcItem.right - rcPaddingSeparator.right;
            rcSeparator.bottom = rcItem.top + tmp;
            
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSeparator.left, rcSeparator.top, rcSeparator.right,
                rcSeparator.bottom, ATOM_ITEM, ATOM_SEPARATOR, 0, 0, 0, 0, 255);
        }
        else {
            
            BOOL fHover = ((ps.dwState & STATE_HOVER) != 0 && (mii.fState & MFS_GRAYED) == 0) ||
                          ((mii.fState & MFS_HILITE) != 0 && mii.hSubMenu != 0);
            CHAR alpha = 255;
            if (fHover) {
                pObj->dwState_ = pObj->dwState_ | STATE_HOVER;
            }
            if ((mii.fState & MFS_GRAYED) == 0) {
                if (fHover) {
                    Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, rcItem.left, rcItem.top,
                                        rcItem.right, rcItem.bottom, ATOM_ITEM, ATOM_SELECT, alpha);
                }
            }
            else {
                alpha                    = 128;
                crColor                  = _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL);
                *(((CHAR*)&crColor) + 3) = alpha;
            }
            if (mii.hSubMenu != 0) {
                LPVOID lpPaddingSub = Ex_ThemeGetValuePtr(ps.hTheme, ATOM_ITEM, ATOM_EXPAND);
                RECT rcPaddingSub;
                if (lpPaddingSub != 0) {
                    RtlMoveMemory(&rcPaddingSub, lpPaddingSub, 16);
                }
                int subWidth = rcPaddingSub.right - rcPaddingSub.left;
                int subHeight = rcPaddingSub.bottom - rcPaddingSub.top;
                rcSub.left       = rcItem.right - subWidth;
                rcSub.top        = rcItem.top + (rcItem.bottom - rcItem.top - subHeight) / 2;
                rcSub.right      = rcSub.left + subWidth;
                rcSub.bottom     = rcSub.top + subHeight;
                
                Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSub.left, rcSub.top, rcSub.right,
                                      rcSub.bottom, ATOM_ITEM, ATOM_EXPAND, 0, 0, 0, 0, alpha);
            }
           
            if ((mii.fState & MFS_CHECKED) != 0) {
                atomProp = ATOM_CHECK;
            }
            else if ((mii.fState & MFT_RADIOCHECK) != 0) {
                atomProp = ATOM_RADIO;
            }
            LPVOID lpPadding = Ex_ThemeGetValuePtr(ps.hTheme, ATOM_ITEM, atomProp);
            if (lpPadding != 0) {
                RtlMoveMemory(&rcPadding, lpPadding, 16);
            }
            INT tmp     = pObj->t_left_;
            rcSub.left  = rcItem.left + (tmp - (rcPadding.right - rcPadding.left)) / 2;
            rcSub.right = rcSub.left + rcPadding.right - rcPadding.left;
            rcSub.top =
                rcItem.top + (rcItem.bottom - rcItem.top - (rcPadding.bottom - rcPadding.top)) / 2;
            rcSub.bottom = rcSub.top + rcPadding.bottom - rcPadding.top;
            
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSub.left, rcSub.top, rcSub.right,
                                  rcSub.bottom, ATOM_ITEM, atomProp, 0, 0, 0, 0, alpha);
            if (lpText != 0) {
                HEXFONT hFont = pObj->hFont_;
                if ((mii.fState & MFS_DEFAULT) != 0) {
                    LOGFONTW Logfont{0};
                    if (_font_getlogfont(hFont, &Logfont)) {
                        Logfont.lfWeight = 700;
                        hFont = _font_createfromlogfont_ex(&Logfont, EFF_DISABLEDPISCALE);
                    }
                }
                if (mii.hbmpItem) {
                    HEXIMAGE img = 0;
                    _img_createfromhbitmap(mii.hbmpItem, 0, TRUE, &img);
                    if (img != 0) {
                        INT imgWidth, imgHeight;
                        _img_getsize(img, &imgWidth, &imgHeight);
                        _canvas_drawimagerectrect(
                            ps.hCanvas, img, Ex_Scale(5), Ex_Scale(3), ps.rcPaint.left + Ex_Scale(5) + ps.rcPaint.bottom - Ex_Scale(6),
                            ps.rcPaint.bottom - Ex_Scale(3), 0, 0, imgWidth, imgHeight, 255);
                        _img_destroy(img);
                    }
                }
                LPWSTR tmp1 = (LPWSTR)wcschr(lpText, 9);
                if (tmp1) {
                    *tmp1 = 0;
                }
                _canvas_drawtextex(ps.hCanvas, hFont, crColor, lpText, -1, ps.dwTextFormat,
                                   ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom);
                if (tmp1) {
                    *tmp1 = 9;
                    if (!fHover) {
                        *(((CHAR*)&crColor) + 3) = (CHAR)128;
                    }
                    _canvas_drawtextex(ps.hCanvas, hFont, crColor, (LPCWSTR)(tmp1 + 1), -1,
                                       ps.dwTextFormat | DT_RIGHT, ps.rcText.left, ps.rcText.top,
                                       ps.rcText.right, ps.rcText.bottom);
                }
                if ((mii.fState & MFS_DEFAULT) != 0) {
                    _font_destroy(hFont);
                }
            }
        }
    }
}

void _item_paint(HEXOBJ hObj, obj_s* pObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        INT nIndex   = COLOR_EX_TEXT_NORMAL;
        INT atomProp = 0;

        if ((ps.dwState & STATE_HOVER) != 0) {
            atomProp = ATOM_HOVER;
            nIndex   = COLOR_EX_TEXT_HOVER;
        }
        else {
            if ((ps.dwState & STATE_CHECKED) != 0) {
                nIndex   = COLOR_EX_TEXT_CHECKED;
                atomProp = ATOM_CHECK;
            }
            if ((ps.dwState & STATE_SELECT) != 0) {
                atomProp = ATOM_SELECT;
                nIndex   = COLOR_EX_TEXT_SELECT;
            }
        }
        EXARGB  crColor = _obj_getcolor(pObj, nIndex);
        LPCWSTR lptext  = pObj->pstrTitle_;
        if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM) {
            _item_draw(pObj, ps, crColor, lptext);
        }
        else {
            if (atomProp != 0) {
                if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
                    Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight,
                                        ATOM_ITEM, atomProp, 255);
                }
            }
            if (lptext != 0) {
                _canvas_drawtextex(ps.hCanvas, pObj->hFont_, crColor, lptext, -1, ps.dwTextFormat,
                                   ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom);
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}

LRESULT CALLBACK _item_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (uMsg == WM_PAINT) {
            _item_paint(hObj, pObj);
        }
        else if (uMsg == WM_EX_LCLICK) {
            _item_click(hWnd, pObj);
        }
        else if (uMsg == WM_MOUSEHOVER) {
            if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM) {
                _obj_notify_brothers(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0, TRUE, TRUE);
            }
            _obj_setfocus(hWnd, pObj->pWnd_, hObj, pObj, TRUE);
        }
        else if (uMsg == WM_MOUSELEAVE) {
            if ((pObj->dwState_ & STATE_HOVER) == STATE_HOVER) {
                _obj_killfocus(hObj, pObj, TRUE);
                _obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
            }
        }
        else if (uMsg == WM_LBUTTONDOWN) {
            _obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, 0);
        }
        else if (uMsg == WM_LBUTTONUP) {
            _obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, 0);
        }
        else if (uMsg == WM_SETFOCUS) {
            _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
        }
        else if (uMsg == WM_KILLFOCUS) {
           // _obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
        }
        else if (uMsg == WM_TIMER) {}
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
