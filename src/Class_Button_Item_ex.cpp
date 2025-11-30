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
        INT atomClass = ATOM_CHECKBUTTON;
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
        sizeof(void*) * 3, 0, 0, _item_proc);
}
static HBITMAP get_arrow_bitmap(void)
{
    static HBITMAP arrow_bitmap;

    if (!arrow_bitmap)
        arrow_bitmap = (HBITMAP)LoadImageW(0, MAKEINTRESOURCEW(OBM_MNARROW), IMAGE_BITMAP, 0, 0, 0);
    return arrow_bitmap;
}
static void draw_bitmap_item(HWND hwnd, obj_s* pObj, struct menu_item* item, const RECT* rect,
    struct menu* menu, HWND owner, UINT odaction)
{
    int w = rect->right - rect->left;
    int h = rect->bottom - rect->top;
    int bmp_xoffset = 0; //, left, top;
    HEXIMAGE bmp_to_draw = item->hbmpItem;
    HEXIMAGE bmp = bmp_to_draw;
    //BITMAP bm;
    INT        bmWidth;
    INT        bmHeight;
    //DWORD rop;

    /* Check if there is a magic menu item associated with this item */
    if (IS_MAGIC_BITMAP(bmp_to_draw))
    {
        BOOL down = FALSE, grayed = FALSE;
        enum NONCLIENT_BUTTON_TYPE type;
        WCHAR bmchr = 0;
        RECT r;

        switch ((INT_PTR)bmp_to_draw)
        {
            //case (INT_PTR)HBMMENU_SYSTEM:
            //    if (item->dwItemData)
            //    {
            //        bmp = (HEXIMAGE)item->dwItemData;
            //        if (!_img_getsize(bmp, &bmWidth, &bmHeight))
            //            return;
            //    }
            //    else
            //    {
            //        static HBITMAP sys_menu_bmp;
            //
            //        if (!sys_menu_bmp)
            //            sys_menu_bmp = LoadImageW(0, MAKEINTRESOURCEW(OBM_CLOSE), IMAGE_BITMAP, 0, 0, 0);
            //        bmp = sys_menu_bmp;
            //        if (!NtGdiExtGetObjectW(bmp, sizeof(bm), &bm)) return;
            //        /* only use right half of the bitmap */
            //        bmp_xoffset = bm.bmWidth / 2;
            //        bm.bmWidth -= bmp_xoffset;
            //    }
            //    goto got_bitmap;
        case (INT_PTR)HBMMENU_MBAR_RESTORE:
            type = MENU_RESTORE_BUTTON;
            break;
        case (INT_PTR)HBMMENU_MBAR_MINIMIZE:
            type = MENU_MIN_BUTTON;
            break;
        case (INT_PTR)HBMMENU_MBAR_MINIMIZE_D:
            type = MENU_MIN_BUTTON;
            grayed = TRUE;
            break;
        case (INT_PTR)HBMMENU_MBAR_CLOSE:
            type = MENU_CLOSE_BUTTON;
            break;
        case (INT_PTR)HBMMENU_MBAR_CLOSE_D:
            type = MENU_CLOSE_BUTTON;
            grayed = TRUE;
            break;
        case (INT_PTR)HBMMENU_CALLBACK:
        {
            DRAWITEMSTRUCT drawItem;
            drawItem.CtlType = ODT_MENU;
            drawItem.CtlID = 0;
            drawItem.itemID = item->wID;
            drawItem.itemAction = odaction;
            drawItem.itemState = 0;
            if (item->fState & MF_CHECKED)  drawItem.itemState |= ODS_CHECKED;
            if (item->fState & MF_DEFAULT)  drawItem.itemState |= ODS_DEFAULT;
            if (item->fState & MF_DISABLED) drawItem.itemState |= ODS_DISABLED;
            if (item->fState & MF_GRAYED)   drawItem.itemState |= ODS_GRAYED | ODS_DISABLED;
            if (item->fState & MF_HILITE)   drawItem.itemState |= ODS_SELECTED;
            drawItem.hwndItem = (HWND)menu->obj.handle;
            drawItem.hDC = (HDC)(LONG_PTR)pObj->canvas_obj_;// hdc;
            drawItem.itemData = item->dwItemData;
            drawItem.rcItem = *rect;
            if (menu->hOwnerObj)
                Ex_ObjSendMessage(menu->hOwnerObj, WM_DRAWITEM, 0, (LPARAM)&drawItem);
            else
                SendMessageW(owner, WM_DRAWITEM, 0, (LPARAM)&drawItem);
            return;
        }
        break;
        case (INT_PTR)HBMMENU_POPUP_CLOSE:
            bmchr = 0x72;
            break;
        case (INT_PTR)HBMMENU_POPUP_RESTORE:
            bmchr = 0x32;
            break;
        case (INT_PTR)HBMMENU_POPUP_MAXIMIZE:
            bmchr = 0x31;
            break;
        case (INT_PTR)HBMMENU_POPUP_MINIMIZE:
            bmchr = 0x30;
            break;
        default:
            return;
        }

        if (bmchr)
        {
            //使用Marlett字体字符绘制魔法位图  draw the magic bitmaps using marlett font characters 
            // FIXME: 字体大小和位置（x，y）可能可以优化   fontsize and the position (x,y) could probably be better 
            LOGFONTW logfont = { 0, 0, 0, 0, FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0,
                                 {'M','a','r','l','e','t','t'} };
            logfont.lfHeight = min(h, w) - 5;
            //HFONT hfont, prev_font;
            //hfont = NtGdiHfontCreate(&logfont, sizeof(logfont), 0, 0, NULL);
            //prev_font = NtGdiSelectFont(hdc, hfont);
            //NtGdiExtTextOutW(hdc, rect->left, rect->top + 2, 0, NULL, &bmchr, 1, NULL, 0);
            //NtGdiSelectFont(hdc, prev_font);
            //NtGdiDeleteObjectApp(hfont);
        }
        else
        {
            r = *rect;
            InflateRect(&r, -1, -1);
            if (item->fState & MF_HILITE)
                down = TRUE;
            //draw_menu_button(hwnd, hdc, &r, type, down, grayed);//画按钮
        }
        return;
    }

    if (!bmp || !_img_getsize(bmp, &bmWidth, &bmHeight))
        return;

    //got_bitmap:
        //HDC mem_hdc;
        //mem_hdc = NtGdiCreateCompatibleDC(hdc);
        //NtGdiSelectBitmap(mem_hdc, bmp);

        /* handle fontsize > bitmap_height */
        //top = (h > bmHeight) ? rect->top + (h - bmHeight) / 2 : rect->top;
        //left = rect->left;
        //rop = ((item->fState & MF_HILITE) && !IS_MAGIC_BITMAP(bmp_to_draw)) ? NOTSRCCOPY : SRCCOPY;
        //if ((item->fState & MF_HILITE) && item->hbmpItem)
    {
        //NtGdiGetAndSetDCDword(hdc, NtGdiSetBkColor, get_sys_color(COLOR_HIGHLIGHT), NULL);
    }
    //BitBlt(hdc, left, top, w, h, mem_hdc, bmp_xoffset, 0, rop, 0, 0);
    //NtGdiDeleteObjectApp(mem_hdc);
    EX_RECTF drc{ (float)rect->left, (float)rect->top, (float)rect->right, (float)rect->bottom };
    _canvas_drawimagefrombkgimg_ex(pObj->canvas_obj_, bmp, 0, 0, BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_DEFAULT, 255, 0, &drc);
}

void  _item_draw(obj_s* pObj, EXARGB crColor, LPCWSTR lpText, struct menu_item* item, struct menu* menu, BOOL menu_bar)
{
    HWND hwnd = pObj->pWnd_->hWnd_;
    UINT arrow_width = 0, arrow_height = 0;
    //HRGN old_clip = NULL, clip;
    BOOL flat_menu = FALSE;
    RECT rect, bmprc;
    UINT format = 0;
    if (!menu_bar)
    {
        BITMAP bmp;
        GetObjectW(get_arrow_bitmap(), sizeof(bmp), &bmp);
        arrow_width = bmp.bmWidth;
        arrow_height = bmp.bmHeight;
    }

    if (item->fType & MF_SYSMENU)
    {
        //if (!is_iconic(hwnd))
            //draw_nc_sys_button(hwnd, hdc, item->fState & (MF_HILITE | MF_MOUSESELECT));
        return;
    }

    //rect = item->rect;
    //adjust_menu_item_rect(menu, &rect);
    //if (!intersect_rect(&bmprc, &rect, &menu->items_rect)) //  bmprc用作占位符   bmprc is used as a dummy
    //    return;
    rect = *(RECT*)&pObj->c_left_;

    SystemParametersInfoW(SPI_GETFLATMENU, 0, &flat_menu, 0);
    int bkgnd = (menu_bar && flat_menu) ? COLOR_MENUBAR : COLOR_MENU;

    EXARGB NtGdiSetTextColor = 0;
    EXARGB NtGdiSetBkColor = 0;
    /* Setup colors */
    if (item->fState & MF_HILITE)
    {
        if (menu_bar && !flat_menu)
        {
            NtGdiSetTextColor = GetSysColor(COLOR_MENUTEXT);
            NtGdiSetBkColor = GetSysColor(COLOR_MENU);
        }
        else
        {
            if (item->fState & MF_GRAYED)
                NtGdiSetTextColor = GetSysColor(COLOR_GRAYTEXT);
            else
                NtGdiSetTextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
            NtGdiSetBkColor = GetSysColor(COLOR_HIGHLIGHT);
        }
    }
    else
    {
        if (item->fState & MF_GRAYED)
            NtGdiSetTextColor = GetSysColor(COLOR_GRAYTEXT);
        else
            NtGdiSetTextColor = GetSysColor(COLOR_MENUTEXT);
        NtGdiSetBkColor = GetSysColor(bkgnd);
    }

    HEXBRUSH brush = _brush_create(0);
    HEXFONT prev_font = 0;
    if (item->fState & MFS_DEFAULT)
    {
        prev_font = get_menu_font(TRUE);
    }
    else
        prev_font = pObj->hFont_;

    //old_clip = NtGdiCreateRectRgn(0, 0, 0, 0);
    //if (NtGdiGetRandomRgn(hdc, old_clip, NTGDI_RGN_MIRROR_RTL | 1) <= 0)
    //{
    //    NtGdiDeleteObjectApp(old_clip);
    //    old_clip = NULL;
    //}
    //clip = NtGdiCreateRectRgn(menu->items_rect.left, menu->items_rect.top, menu->items_rect.right, menu->items_rect.bottom);
    //NtGdiExtSelectClipRgn(hdc, clip, RGN_AND);
    //NtGdiDeleteObjectApp(clip);

    if (item->fType & MF_OWNERDRAW)
    {
        /*
         * Experimentation under Windows reveals that an owner-drawn
         * menu is given the rectangle which includes the space it requested
         * in its response to WM_MEASUREITEM _plus_ width for a checkmark
         * and a popup-menu arrow.  This is the value of item->rect.
         * Windows will leave all drawing to the application except for
         * the popup-menu arrow.  Windows always draws that itself, after
         * the menu owner has finished drawing.
         */
        DRAWITEMSTRUCT dis;
        DWORD old_bk = NtGdiSetBkColor, old_text = NtGdiSetTextColor;

        dis.CtlType = ODT_MENU;
        dis.CtlID = 0;
        dis.itemID = item->wID;
        dis.itemData = item->dwItemData;
        dis.itemState = 0;
        if (item->fState & MF_CHECKED) dis.itemState |= ODS_CHECKED;
        if (item->fState & MF_GRAYED)  dis.itemState |= ODS_GRAYED | ODS_DISABLED;
        if (item->fState & MF_HILITE)  dis.itemState |= ODS_SELECTED;
        dis.itemAction = _obj_getextralong(pObj, 2);// odaction; /* ODA_DRAWENTIRE | ODA_SELECT | ODA_FOCUS; */
        dis.hwndItem = (HWND)menu->obj.handle;
        dis.hDC = (HDC)(LONG_PTR)pObj->canvas_obj_;// hdc;
        dis.rcItem = rect;
        if (menu->hOwnerObj)
            Ex_ObjSendMessage(menu->hOwnerObj, WM_DRAWITEM, 0, (LPARAM)&dis);
        else
            SendMessageW(menu->hwndOwner, WM_DRAWITEM, 0, (LPARAM)&dis);
        /* Draw the popup-menu arrow */
        //NtGdiGetAndSetDCDword(hdc, NtGdiGetBkColor, old_bk, NULL);
        //NtGdiGetAndSetDCDword(hdc, NtGdiGetTextColor, old_text, NULL);
        if (item->fType & MF_POPUP)
            _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 255), L">", 1, DT_RIGHT | DT_VCENTER | DT_SINGLELINE,
                rect.left, rect.top, rect.right, rect.bottom); //draw_popup_arrow(pObj, rect, arrow_width, arrow_height);
        goto done;
    }

    if (menu_bar && (item->fType & MF_SEPARATOR))
        goto done;

    if (item->fState & MF_HILITE)
    {
        //if (flat_menu)
        //{
        //    InflateRect(&rect, -1, -1);
        //    //fill_rect(hdc, &rect, GetSysColor(COLOR_MENUHILIGHT));
        //    RECT grid{ 4,4,4,4 };
        //    RECT src{ 0,0,16,16 };
        //    RECTF drc{ (float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom };
        //    _canvas_drawimagefrombkgimg_ex(ps.hCanvas, g_Li.hImg_item, 0, 0, BIR_DEFAULT, &grid, BIF_DEFAULT, 255, &src, &drc);
        //    InflateRect(&rect, 1, 1);
        //    //fill_rect(hdc, &rect, GetSysColor(COLOR_HIGHLIGHT));
        //}
        //else
        {
            //if (menu_bar)
            //    draw_rect_edge(hdc, &rect, BDR_SUNKENOUTER, BF_RECT, 1);
            //else
                //fill_rect(hdc, &rect, GetSysColor_brush(COLOR_HIGHLIGHT));
            //RECT grid{ 4,4,4,4 };
            //RECT src{ 0,0,16,16 };
            //EX_RECTF drc{ (float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom };
            //_canvas_drawimagefrombkgimg_ex(pObj->canvas_obj_, g_Li.hImg_item, 0, 0, BACKGROUND_REPEAT_ZOOM, &grid, BACKGROUND_FLAG_DEFAULT, 255, &src, &drc);
            _brush_setcolor(brush, ExRGB2ARGB(102531, 200));
            _canvas_fillrect(pObj->canvas_obj_, brush, rect.left, rect.top, rect.right, rect.bottom);
        }
    }
    else
    {
        //fill_rect(hdc, &rect, GetSysColor_brush(bkgnd));
    }

    /* vertical separator */
    if (!menu_bar && (item->fType & MF_MENUBARBREAK))
    {
        RECT rc = rect;

        rc.left = 1;// -= MENU_COL_SPACE / 2 + 1;
        rc.top = 3;
        rc.bottom = menu->Height - 3;
        if (flat_menu)
        {
            //oldPen = NtGdiSelectPen(hdc, GetSysColor_pen(COLOR_BTNSHADOW));
            //NtGdiMoveTo(hdc, rc.left, rc.top, NULL);
            //NtGdiLineTo(hdc, rc.left, rc.bottom);
            _brush_setcolor(brush, ExRGB2ARGB(GetSysColor(COLOR_BTNSHADOW), 255));
            _canvas_drawline(pObj->canvas_obj_, brush, rc.left, rc.top, rc.left, rc.bottom, 1, 0);

        }
        //else
        //    draw_rect_edge(hdc, &rc, EDGE_ETCHED, BF_LEFT, 1);
    }

    /* horizontal separator */
    if (item->fType & MF_SEPARATOR)
    {
        RECT rc = rect;

        InflateRect(&rc, -1, 0);
        rc.top = (rc.top + rc.bottom) / 2;
        if (flat_menu)
        {
            //oldPen = NtGdiSelectPen(hdc, GetSysColor_pen(COLOR_BTNSHADOW));
            //NtGdiMoveTo(hdc, rc.left, rc.top, NULL);
            //NtGdiLineTo(hdc, rc.right, rc.top);
            _brush_setcolor(brush, ExRGB2ARGB(GetSysColor(COLOR_BTNSHADOW), 255));
            _canvas_drawline(pObj->canvas_obj_, brush, rc.left + 4, rc.top, rc.right - 8, rc.top, 1, 0);
        }
        //else
        //    draw_rect_edge(hdc, &rc, EDGE_ETCHED, BF_TOP, 1);
        goto done;
    }

    //if (item->hbmpItem)
    {
        //计算相对于项目矩形的位图矩形坐标
        if (menu_bar)
        {
            if (item->hbmpItem == (-1)) //HBMMENU_CALLBACK
                bmprc.left = 3;
            else
                bmprc.left = item->text ? menucharsize.cx : 0;
        }
        else if (menu->dwStyle & MNS_NOCHECK) // 项左侧没有为检查标记保留空间。 仍然可以选择该项，但项目旁边不会显示检查标记
            bmprc.left = 4;
        else if (menu->dwStyle & MNS_CHECKORBMP)//为检查标记和位图保留相同的空间.如果绘制检查标记,则位图不是. 所有复选标记和位图都对齐.用于菜单,其中一些项使用复选标记,一些项使用位图
            bmprc.left = 2;
        else
            bmprc.left = 4; //+ GetSystemMetrics(SM_CXMENUCHECK)

        if (menu_bar && !(item->hbmpItem == (-1))) // HBMMENU_CALLBACK
            bmprc.top = 0;
        else
            bmprc.top = 2;// (rect.right - rect.left - (menucharsize.cy + 4)) / 2;
        //bmprc.top = (rect.bottom - rect.top - item->bmpsize.cy) / 2;
        bmprc.right = bmprc.left + (rect.bottom - rect.top - 4);
        //bmprc.right = bmprc.left + item->bmpsize.cx;
        bmprc.bottom = rect.bottom - 2;
        //bmprc.bottom = bmprc.top + item->bmpsize.cy;
    }


    if (!menu_bar)
    {
        HEXIMAGE bm;
        INT y = rect.top + rect.bottom;
        BOOL checked = FALSE;
        UINT check_bitmap_width = GetSystemMetrics(SM_CXMENUCHECK);
        UINT check_bitmap_height = GetSystemMetrics(SM_CYMENUCHECK);

        // Draw the check mark
        if (!(menu->dwStyle & MNS_NOCHECK))
        {
            bm = (item->fState & MF_CHECKED) ? item->hCheckBit : item->hUnCheckBit;
            if (bm)  /* we have a custom bitmap */
            {
                //HDC mem_hdc = NtGdiCreateCompatibleDC(hdc);
                //NtGdiSelectBitmap(mem_hdc, bm);
                //NtGdiBitBlt(hdc, rect.left, (y - check_bitmap_height) / 2,
                //    check_bitmap_width, check_bitmap_height,
                //    mem_hdc, 0, 0, SRCCOPY, 0, 0);
                //NtGdiDeleteObjectApp(mem_hdc);
                EX_RECTF drc{ (float)bmprc.left, (float)bmprc.top, (float)bmprc.right, (float)bmprc.bottom };
                _canvas_drawimagefrombkgimg_ex(pObj->canvas_obj_, bm, 0, 0, BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_DEFAULT, 255, 0, &drc);
                checked = TRUE;
            }
            else if (item->fState & MF_CHECKED) /* standard bitmaps */
            {
                RECT r;
                //HBITMAP bm = NtGdiCreateBitmap(check_bitmap_width,
                //    check_bitmap_height, 1, 1, NULL);
                //HDC mem_hdc = NtGdiCreateCompatibleDC(hdc);
                //
                //NtGdiSelectBitmap(mem_hdc, bm);
                SetRect(&r, 0, 0, check_bitmap_width, check_bitmap_height);
                //draw_frame_menu(mem_hdc, &r,
                //    (item->fType & MFT_RADIOCHECK) ? DFCS_MENUBULLET : DFCS_MENUCHECK);
                //NtGdiBitBlt(hdc, rect.left, (y - r.bottom) / 2, r.right, r.bottom,
                //    mem_hdc, 0, 0, SRCCOPY, 0, 0);
                //NtGdiDeleteObjectApp(mem_hdc);
                //NtGdiDeleteObjectApp(bm);
                EX_RECTF drc{ (float)bmprc.left, (float)bmprc.top, (float)bmprc.right, (float)bmprc.bottom };
                _canvas_drawimagefrombkgimg_ex(pObj->canvas_obj_, bm, 0, 0, BACKGROUND_REPEAT_ZOOM, 0, BACKGROUND_FLAG_DEFAULT, 255, 0, &drc);
                checked = TRUE;
            }
        }
        if (item->hbmpItem && !(checked && (menu->dwStyle & MNS_CHECKORBMP)))
        {
            /* some applications make this assumption on the DC's origin */
            draw_bitmap_item(hwnd, pObj, item, &bmprc, menu, menu->hwndOwner, _obj_getextralong(pObj, 2));

        }
        /* Draw the popup-menu arrow */
        if (item->fType & MF_POPUP)
            _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 180), L"▶", -1, DT_RIGHT | DT_VCENTER | DT_SINGLELINE,
                rect.left, rect.top, rect.right - 8, rect.bottom); //draw_popup_arrow(pObj, rect, arrow_width, arrow_height);// 画 > 箭头

        rect.left += 4;
        if (!(menu->dwStyle & MNS_NOCHECK))
            rect.left += 4; //check_bitmap_width;
        rect.right -= arrow_width;
    }
    else if (item->hbmpItem) // menu_bar 菜单条 暂未实现
    {
        draw_bitmap_item(hwnd, pObj, item, &bmprc, menu, menu->hwndOwner, _obj_getextralong(pObj, 2));
    }
    /* process text if present */
    if (item->text)
    {
        int i;
        format = menu_bar ?
            DT_CENTER | DT_VCENTER | DT_SINGLELINE :
            DT_LEFT | DT_VCENTER | DT_SINGLELINE;

        if (!(menu->dwStyle & MNS_CHECKORBMP))
            rect.left += menu->textOffset;


        if (menu_bar)
        {
            if (item->hbmpItem)
                rect.left += item->bmpsize.cx;
            if (item->hbmpItem != (-1)) // HBMMENU_CALLBACK
                rect.left += menucharsize.cx;
            rect.right -= menucharsize.cx;
        }

        for (i = 0; item->text[i]; i++)
            if ((item->text[i] == '\t') || (item->text[i] == '\b'))
                break;

        if (item->fState & MF_GRAYED)
        {
            //if (!(item->fState & MF_HILITE))
            //{
            //    ++rect.left; ++rect.top; ++rect.right; ++rect.bottom;
            //
            //    //NtGdiGetAndSetDCDword(hdc, NtGdiSetTextColor, RGB(0xff, 0xff, 0xff), NULL);
            //    //DrawTextW(hdc, item->text, i, &rect, format);
            //    NtGdiSetTextColor = 0xffffff;
            //    _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 255), item->text, i, format,
            //        rect.left, rect.top, rect.right, rect.bottom, 0, 0, 0);
            //    --rect.left; --rect.top; --rect.right; --rect.bottom;
            //}
            // NtGdiGetAndSetDCDword(hdc, NtGdiSetTextColor, RGB(0x80, 0x80, 0x80), NULL);
            NtGdiSetTextColor = 0x808080;
        }

        //DrawTextW(hdc, item->text, i, &rect, format);
        _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 255), item->text, i, format,
            rect.left, rect.top, rect.right, rect.bottom);


        /* paint the shortcut text */
        if (!menu_bar && item->text[i])  /* There's a tab or flush-right char */
        {
            if (item->text[i] == '\t')
            {
                rect.left = item->xTab;
                format = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
            }
            else
            {
                rect.right = item->xTab;
                format = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
            }

            if (item->fState & MF_GRAYED)
            {
                //if (!(item->fState & MF_HILITE))
                //{
                //    ++rect.left; ++rect.top; ++rect.right; ++rect.bottom;
                //    //NtGdiGetAndSetDCDword(hdc, NtGdiSetTextColor, RGB(0xff, 0xff, 0xff), NULL);
                //    //DrawTextW(hdc, item->text + i + 1, -1, &rect, format);
                //    _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 255), item->text + i + 1, -1, format,
                //        rect.left, rect.top, rect.right, rect.bottom, 0, 0, 0);
                //    --rect.left; --rect.top; --rect.right; --rect.bottom;
                //}
                    //NtGdiGetAndSetDCDword(hdc, NtGdiSetTextColor, RGB(0x80, 0x80, 0x80), NULL);
            }
            //DrawTextW(hdc, item->text + i + 1, -1, &rect, format);
            _canvas_drawtextex(pObj->canvas_obj_, prev_font, ExRGB2ARGB(NtGdiSetTextColor, 255), item->text + i + 1, -1, format,
                rect.left, rect.top, rect.right, rect.bottom);
        }



    done:
        {
            //NtGdiExtSelectClipRgn(hdc, old_clip, RGN_COPY);
            //if (old_clip) NtGdiDeleteObjectApp(old_clip);
        }
    }
    if (item->fState & MFS_DEFAULT)
        _font_destroy(prev_font);

    _brush_destroy(brush);
}

void _item_paint(HEXOBJ hObj, obj_s* pObj)
{
    EX_PAINTSTRUCT ps{ 0 };
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        INT nIndex = COLOR_EX_TEXT_NORMAL;
        RECT* atomProp = 0;
        RECT atomProp_HOVER{ 0,0,16,16 };
        RECT atomProp_CHECKED{ 0,21,16,37 };
        RECT atomProp_SELECT{ 0,0,16,16 };

        if ((ps.dwState & STATE_HOVER) != 0)
        {
            atomProp = &atomProp_HOVER;
            nIndex = COLOR_EX_TEXT_HOVER;
        }
        else
        {
            if ((ps.dwState & STATE_CHECKED) != 0)
            {
                nIndex = COLOR_EX_TEXT_CHECKED;
                atomProp = &atomProp_CHECKED;
            }
            if ((ps.dwState & STATE_SELECT) != 0)
            {
                atomProp = &atomProp_SELECT;
                nIndex = COLOR_EX_TEXT_SELECT;
            }
        }
        EXARGB crColor = _obj_getcolor(pObj, nIndex);
        LPCWSTR lptext = pObj->pstrTitle_;
        if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
        {
            _item_draw(pObj, crColor, lptext, (menu_item*)_obj_getextralong(pObj, 0), (struct menu*)_obj_getextralong(pObj, 1), FALSE);
        }
        else
        {
            if (atomProp != 0)
            {
                if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0)
                {
                    RECT grid{ 4,4,4,4 };
                    //RECTF drc{ 0, 0, (FLOAT)ps.uWidth, (FLOAT)ps.uHeight };
                    //_canvas_drawimagefrombkgimg_ex(ps.hCanvas, g_Li.hImg_item, 0, 0, BIR_DEFAULT, &grid, BIF_DEFAULT, 255, atomProp, &drc);
                }
            }
            if (lptext != 0)
            {
                _canvas_drawtextex(ps.hCanvas, pObj->hFont_, crColor, lptext, -1, ps.dwTextFormat, ps.rcText.left, ps.rcText.top, ps.rcText.right, ps.rcText.bottom);
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
}

LRESULT CALLBACK _item_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (!_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
    }
    switch (uMsg)
    {
    case WM_PAINT:
    {
        _item_paint(hObj, pObj);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
        {
            //_button_obj_notify_brothers(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0, TRUE, TRUE);
            menu_item* item = (menu_item*)_obj_getextralong(pObj, 0);
            item->fState |= MF_HILITE;

        }
        _obj_setfocus(hWnd, pObj->pWnd_, hObj, pObj, TRUE);//只有具有子菜单的 才可以具有焦点
        _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
        break;
    }
    case WM_MOUSELEAVE:
    {
        if ((pObj->dwFlags_ & EOF_BMENUITEM) == EOF_BMENUITEM)
        {
            //_button_obj_notify_brothers(hWnd, hObj, pObj, WM_MOUSELEAVE, 0, 0, TRUE, TRUE);
            menu_item* item = (menu_item*)_obj_getextralong(pObj, 0);
            item->fState &= ~MF_HILITE;
        }
        if ((pObj->dwState_ & STATE_HOVER) == STATE_HOVER)
        {
            _obj_killfocus(hObj, pObj, TRUE);
            _obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        _obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, 0);
        break;
    }
    case WM_LBUTTONUP:
    {
        _obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
        break;
    }
    case WM_SETFOCUS:
    {
        _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, 0);
        break;
    }
    case WM_KILLFOCUS:
    {
        _obj_setuistate(pObj, STATE_HOVER | STATE_DOWN, TRUE, 0, TRUE, 0);
        break;
    }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
