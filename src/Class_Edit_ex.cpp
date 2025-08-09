#include "stdafx.h"

class TextHost : public ITextHost
{
    edit_s* m_pOwner;

public:
    TextHost(edit_s* pOwner) { m_pOwner = pOwner; };

    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID                                           riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
    {
        return E_NOINTERFACE;
    };

    ULONG STDMETHODCALLTYPE AddRef(void) { return E_NOINTERFACE; };

    ULONG STDMETHODCALLTYPE Release(void) { return E_NOINTERFACE; };

    //@cmember Get the DC for the host
    HDC TxGetDC() { return m_pOwner->mDc_; };

    //@cmember Release the DC gotten from the host
    INT TxReleaseDC(HDC hdc) { return ReleaseDC(0, hdc); };

    //@cmember Show the scroll bar
    BOOL TxShowScrollBar(INT fnBar, BOOL fShow)
    {
        Ex_ObjScrollShow(m_pOwner->pObj_->hObj_, fnBar, fShow);
        return TRUE;
    };

    //@cmember Enable the scroll bar
    BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)
    {
        Ex_ObjScrollEnable(m_pOwner->pObj_->hObj_, fuSBFlags, fuArrowflags);
        return TRUE;
    };

    //@cmember Set the scroll range
    BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
    {
        PRECT prcText   = (PRECT)m_pOwner->prctext_;
        LONG  nMaxFixed = 0;
        if (fnBar == SCROLLBAR_TYPE_VERT) {
            nMaxFixed = nMaxPos - (prcText->bottom - prcText->top);
        }
        else {
            nMaxFixed = nMaxPos - (prcText->right - prcText->left);
        }
        return Ex_ObjScrollSetRange(m_pOwner->pObj_->hObj_, fnBar, nMinPos, nMaxFixed, fRedraw);
    };

    //@cmember Set the scroll position
    BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)
    {
        obj_s* pObj = m_pOwner->pObj_;
        HEXOBJ hObj = pObj->hObj_;
        Ex_ObjScrollSetPos(hObj, fnBar, nPos, fRedraw);
        return TRUE;
    };

    //@cmember InvalidateRect
    void TxInvalidateRect(LPCRECT prc, BOOL fMode)
    {
        INT nError = 0;
        _obj_invalidaterect(m_pOwner->pObj_, (RECT*)prc, &nError);
    };

    //@cmember Send a WM_PAINT to the window
    void TxViewChange(BOOL fUpdate){};

    //@cmember Create the caret
    BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
    {
        if (!FLAGS_CHECK(m_pOwner->pObj_->dwStyle_, EDIT_STYLE_HIDDENCARET)) {
            xWidth                    = 2;
            m_pOwner->rcCaret_left_   = 0;
            m_pOwner->rcCaret_top_    = 0;
            m_pOwner->rcCaret_right_  = xWidth;
            m_pOwner->rcCaret_bottom_ = yHeight;
            return CreateCaret(_obj_gethwnd(m_pOwner->pObj_), (HBITMAP)-1, xWidth, yHeight);
        }
        return FALSE;
    };

    //@cmember Show the caret
    BOOL TxShowCaret(BOOL fShow)
    {
        if (fShow) {
            FLAGS_ADD(m_pOwner->flags_, EDIT_FLAG_BSHOWCARET);
            ShowCaret(_obj_gethwnd(m_pOwner->pObj_));
        }
        else {
            FLAGS_DEL(m_pOwner->flags_, EDIT_FLAG_BSHOWCARET);
            HideCaret(_obj_gethwnd(m_pOwner->pObj_));
        }
        return TRUE;
    };

    //@cmember Set the caret position
    BOOL TxSetCaretPos(INT x, INT y)
    {
        INT    nError = 0;
        obj_s* pObj   = m_pOwner->pObj_;
        OffsetRect((LPRECT)&m_pOwner->rcCaret_left_, x, y);
        if (FLAGS_CHECK(pObj->dwState_, STATE_FOCUS)) {
            FLAGS_ADD(m_pOwner->flags_, EDIT_FLAG_BCARETCONTEXT | EDIT_FLAG_BSHOWCARET);
            FLAGS_DEL(m_pOwner->flags_, EDIT_FLAG_BCARETSHHOWED);
            _obj_invalidaterect(pObj, (LPRECT)&m_pOwner->rcCaret_left_, &nError);
        }
        x += pObj->w_left_;
        y += pObj->w_top_;
        SetCaretPos(x, y);
        return TRUE;
    };

    //@cmember Create a timer with the specified timeout
    BOOL TxSetTimer(UINT idTimer, UINT uTimeout)
    {
        return SetTimer(_obj_gethwnd(m_pOwner->pObj_), idTimer, uTimeout, NULL);
    };

    //@cmember Destroy a timer
    void TxKillTimer(UINT idTimer) { KillTimer(_obj_gethwnd(m_pOwner->pObj_), idTimer); };

    //@cmember Scroll the content of the specified window's client area
    void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate,
                          LPRECT lprcUpdate, UINT fuScroll){};

    //@cmember Get mouse capture
    void TxSetCapture(BOOL fCapture){};

    //@cmember Set the focus to the text window
    void TxSetFocus() { Ex_ObjSetFocus(m_pOwner->pObj_->hObj_); };

    //@cmember Establish a new cursor shape
    void TxSetCursor(HCURSOR hcur, BOOL fText){};

    //@cmember Converts screen coordinates of a specified point to the client coordinates
    BOOL TxScreenToClient(LPPOINT lppt) { return FALSE; };

    //@cmember Converts the client coordinates of a specified point to screen coordinates
    BOOL TxClientToScreen(LPPOINT lppt) { return FALSE; };

    //@cmember Request host to activate text services
    HRESULT TxActivate(LONG* plOldState) { return E_FAIL; };

    //@cmember Request host to deactivate text services
    HRESULT TxDeactivate(LONG lNewState) { return E_FAIL; };

    //@cmember Retrieves the coordinates of a window's client area
    HRESULT TxGetClientRect(LPRECT prc)
    {
        CopyMemory(prc, m_pOwner->prctext_, sizeof(RECT));
        return S_OK;
    };

    //@cmember Get the view rectangle relative to the inset
    HRESULT TxGetViewInset(LPRECT prc)
    {
        // 请求文本宿主窗口中文本周围的空白区域的尺寸。
        CopyMemory(prc, m_pOwner->prcinset_, sizeof(RECT));
        return S_OK;
    };

    //@cmember Get the default character format for the text
    HRESULT TxGetCharFormat(const CHARFORMATW** ppCF)
    {
        *ppCF = (CHARFORMATW*)m_pOwner->pcf_;
        return S_OK;
    };

    //@cmember Get the default paragraph format for the text
    HRESULT TxGetParaFormat(const PARAFORMAT** ppPF)
    {
        *ppPF = (PARAFORMAT*)m_pOwner->ppf_;
        return S_OK;
    };

    //@cmember Get the background color for the window
    COLORREF TxGetSysColor(INT nIndex)
    {
        if (nIndex == COLOR_WINDOWTEXT) {
            return ExARGB2RGB(_obj_getcolor(m_pOwner->pObj_, COLOR_EX_TEXT_NORMAL));
        }
        else {
            return GetSysColor(nIndex);
        }
    };

    //@cmember Get the background (either opaque or transparent)
    HRESULT TxGetBackStyle(TXTBACKSTYLE* pstyle)
    {
        *pstyle = TXTBACK_TRANSPARENT;
        return S_OK;
    };

    //@cmember Get the maximum length for the text
    HRESULT TxGetMaxLength(DWORD* plength) { return S_OK; };

    //@cmember Get the bits representing requested scroll bars for the window
    HRESULT TxGetScrollBars(DWORD* pdwScrollBar)
    {
        obj_s* pObj   = m_pOwner->pObj_;
        *pdwScrollBar = ES_AUTOHSCROLL | ES_AUTOVSCROLL;
        if (FLAGS_CHECK(pObj->dwStyle_, OBJECT_STYLE_HSCROLL)) {
            *pdwScrollBar |= WS_HSCROLL;
        }
        if (FLAGS_CHECK(pObj->dwStyle_, OBJECT_STYLE_VSCROLL)) {
            *pdwScrollBar |= WS_VSCROLL;
        }
        if (FLAGS_CHECK(pObj->dwStyle_, OBJECT_STYLE_DISABLENOSCROLL)) {
            *pdwScrollBar |= ES_DISABLENOSCROLL;
        }
        return S_OK;
    };

    //@cmember Get the character to display for password input
    HRESULT TxGetPasswordChar(_Out_ TCHAR* pch)
    {
        *pch = m_pOwner->charPsw_;
        return S_OK;
    };

    //@cmember Get the accelerator character
    HRESULT TxGetAcceleratorPos(LONG* pcp) { return S_OK; };

    //@cmember Get the native size
    HRESULT TxGetExtent(LPSIZEL lpExtent)
    {
        lpExtent->cx = m_pOwner->width_;
        lpExtent->cy = m_pOwner->height_;
        return S_OK;
    };

    //@cmember Notify host that default character format has changed
    HRESULT OnTxCharFormatChange(const CHARFORMATW* pCF) { return E_FAIL; };

    //@cmember Notify host that default paragraph format has changed
    HRESULT OnTxParaFormatChange(const PARAFORMAT* pPF) { return E_FAIL; };

    //@cmember Bulk access to bit properties
    HRESULT TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits)
    {
        *pdwBits = dwMask & m_pOwner->dwPropBits_;
        return S_OK;
    };

    //@cmember Notify host of events
    HRESULT TxNotify(DWORD iNotify, LPVOID pv)
    {
        obj_s* pObj = m_pOwner->pObj_;
        if (iNotify != EN_UPDATE) {
            _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, pObj->hObj_, 0, iNotify, 0, (size_t)pv);
            if (iNotify == EDIT_EVENT_SELCHANGE) {
                INT nError = 0;
                _obj_setuistate(pObj, STATE_FOCUS, TRUE, 0, TRUE, &nError);
                SELCHANGE* pSelChange = (SELCHANGE*)pv;
                if (pSelChange->chrg.cpMin == pSelChange->chrg.cpMax) {
                    FLAGS_DEL(m_pOwner->flags_, EDIT_FLAG_BSELECTED);
                }
                else {
                    FLAGS_ADD(m_pOwner->flags_, EDIT_FLAG_BSELECTED);
                }
            }
        }
        return S_OK;
    };

    // East Asia Methods for getting the Input Context
    HIMC TxImmGetContext() { return NULL; };

    void TxImmReleaseContext(HIMC himc){};

    //@cmember Returns HIMETRIC size of the control bar.
    HRESULT TxGetSelectionBarWidth(LONG* lSelBarWidth)
    {
        *lSelBarWidth = 0;
        return S_OK;
    };
};

void _edit_register()
{
    Ex_ObjRegister(L"Edit", OBJECT_STYLE_VISIBLE,
                   OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP,
                   DT_NOPREFIX | DT_SINGLELINE, 0, LoadCursorW(0, MAKEINTRESOURCEW(32513)),
                   CANVAS_FLAG_GDI_COMPATIBLE, _edit_proc);
}

void _edit_init(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    INT nError = 0;

    edit_s* pOwner = (edit_s*)_struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_),
                                                     sizeof(edit_s), &nError);
    if (pOwner != 0) {
        nError            = 0;
        pOwner->pObj_     = pObj;
        pOwner->crBanner_ = ExARGB(0, 0, 0, 128);
        pOwner->charPsw_  = 9679;
        pOwner->crCaret_  = ExARGB(0, 0, 0, 255);

        _struct_createfromaddr(pOwner, offsetof(edit_s, prctext_), sizeof(RECT), &nError);
        _struct_createfromaddr(pOwner, offsetof(edit_s, prcinset_), sizeof(RECT), &nError);
        _edit_setppf(pObj, pOwner);
        _edit_setpcf(pObj, pOwner, 0);
        _edit_setpropbits(pObj, pOwner);
        _edit_size(hWnd, hObj, pObj);

        TextHost* pITH = new TextHost(pOwner);

        CTSFunc CreateTextServicesL = NULL;
        CreateTextServicesL         = (CTSFunc)GetProcAddress(g_Ri.hRiched20, "CreateTextServices");
        if (pITH != 0) {
            IUnknown* pIUnk = nullptr;
            if (CreateTextServicesL(NULL, pITH, &pIUnk) == 0) {
                ITextServices* pITS = nullptr;
                IID*           pIID_ITextServices =
                    (IID*)(VOID*)GetProcAddress(g_Ri.hRiched20, "IID_ITextServices");
                pIUnk->QueryInterface(*pIID_ITextServices, (LPVOID*)&pITS);
                COleCallBack* ole_callback = new COleCallBack;
                pOwner->ole_               = ole_callback;
                pOwner->ith_               = pITH;
                pOwner->its_               = pITS;
                pITS->OnTxInPlaceActivate(0);
                LRESULT ret;
                pITS->TxSendMessage(EM_SETOLECALLBACK, 0, (LPARAM)ole_callback, &ret);
                pITS->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &ret);
                pITS->TxSendMessage(EM_LIMITTEXT, 0x7FFFFFFE, 0, &ret);
                pITS->TxSendMessage(EM_SETEVENTMASK, 0,
                                    ENM_CHANGE | ENM_SELCHANGE | ENM_LINK | ENM_DRAGDROPDONE, &ret);
                pITS->TxSendMessage(EM_AUTOURLDETECT,
                                    FLAGS_CHECK(pObj->dwStyle_, EDIT_STYLE_PARSEURL), 0, &ret);
            }
        }
        if (g_Li.hMenuEdit == 0) {
            g_Li.hMenuEdit = LoadMenuW(GetModuleHandleW(L"user32.dll"), MAKEINTRESOURCEW(1));
        }
    }
}

void _edit_unint(obj_s* pObj)
{
    edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
    if (pOwner != 0) {
        ((ITextServices*)pOwner->its_)->Release();
        ((ITextHost*)pOwner->ith_)->Release();
        delete pOwner->ith_;
        ((COleCallBack*)pOwner->ole_)->Release();
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, pcf_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, ppf_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, prctext_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, prcinset_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, pBanner_));
        _md_destroy(pOwner, offsetof(edit_s, mDc_), offsetof(edit_s, hBmp_),
                    offsetof(edit_s, pBits_));
        _struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
    }
}

void _edit_setpcf(obj_s* pObj, edit_s* pOwner, INT height)
{
    INT           nError = 0;
    CHARFORMAT2W* pcf    = (CHARFORMAT2W*)_struct_createfromaddr(pOwner, offsetof(edit_s, pcf_),
                                                                 sizeof(CHARFORMAT2W), &nError);
    if (pcf != 0) {
        LOGFONTW logfont = {};
        _font_getlogfont(pObj->hFont_, &logfont);

        pcf->cbSize  = sizeof(CHARFORMAT2W);
        DWORD dwMask = EDIT_SELECT_CHARFORMAT_BOLD | CFE_ITALIC | EDIT_SELECT_CHARFORMAT_UNDERLINE |
                       EDIT_SELECT_CHARFORMAT_STRIKEOUT | EDIT_SELECT_CHARFORMAT_SIZE |
                       EDIT_SELECT_CHARFORMAT_COLOR | EDIT_SELECT_CHARFORMAT_FACE | CFM_CHARSET |
                       EDIT_SELECT_CHARFORMAT_OFFSET;
        DWORD dwEffects = 0;
        if (logfont.lfWeight != 400) {
            dwEffects = dwEffects | CFE_BOLD;
        }

        if (logfont.lfItalic != 0) {
            dwEffects = dwEffects | CFE_ITALIC;
        }

        if (logfont.lfUnderline != 0) {
            dwEffects = dwEffects | CFE_UNDERLINE;
        }

        if (logfont.lfStrikeOut != 0) {
            dwEffects = dwEffects | CFE_STRIKEOUT;
        }
        pcf->dwEffects       = dwEffects;
        pcf->yHeight         = -logfont.lfHeight * 1440 / 96;
        pcf->bCharSet        = logfont.lfCharSet;
        pcf->bPitchAndFamily = logfont.lfPitchAndFamily;
        pcf->crTextColor     = ExARGB2RGB(_obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL));
        RtlMoveMemory(pcf->szFaceName, logfont.lfFaceName,
                      lstrlenW((LPCWSTR)logfont.lfFaceName) * (size_t)2);
        pcf->dwMask = dwMask;
    }
}

void _edit_setppf(obj_s* pObj, edit_s* pOwner)
{
    INT         nError = 0;
    PARAFORMAT* ppf    = (PARAFORMAT*)_struct_createfromaddr(pOwner, offsetof(edit_s, ppf_),
                                                             sizeof(PARAFORMAT), &nError);
    if (ppf != 0) {
        ppf->cbSize  = sizeof(PARAFORMAT);
        DWORD dwMask = EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT;
        WORD  tmp;
        if ((pObj->dwTextFormat_ & DT_CENTER) == DT_CENTER) {
            tmp = EDIT_PARAGRAPHFALIGN_CENTER;
        }
        else {
            if ((pObj->dwTextFormat_ & DT_RIGHT) == DT_RIGHT) {
                tmp = EDIT_PARAGRAPHFALIGN_RIGHT;
            }
            else {
                tmp = EDIT_PARAGRAPHFALIGN_LEFT;
            }
        }
        ppf->wAlignment = tmp;
        ppf->cTabCount  = 1;
        ppf->rgxTabs[0] = 720;
        ppf->dwMask     = dwMask;
    }
}

void _edit_setpropbits(obj_s* pObj, edit_s* pOwner)
{
    INT dwStyle    = pObj->dwStyle_;
    INT dwProperty = 0;
    if (!((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE)) {
        dwProperty = dwProperty | TXTBIT_MULTILINE;
        if ((dwStyle & OBJECT_STYLE_VSCROLL) != 0) {
            if ((dwStyle & OBJECT_STYLE_HSCROLL) == 0) {
                dwProperty = dwProperty | TXTBIT_WORDWRAP;
            }
        }
    }
    if ((dwStyle & EDIT_STYLE_DISABLEDRAG) == 0) {
        dwProperty = dwProperty | TXTBIT_DISABLEDRAG;
    }
    if ((dwStyle & EDIT_STYLE_READONLY) != 0) {
        dwProperty = dwProperty | TXTBIT_READONLY;
    }
    if ((dwStyle & EDIT_STYLE_USEPASSWORD) != 0) {
        dwProperty = dwProperty | TXTBIT_USEPASSWORD;
    }
    if ((dwStyle & EDIT_STYLE_HIDESELECTION) == 0) {
        dwProperty = dwProperty | TXTBIT_HIDESELECTION;
    }
    if ((dwStyle & EDIT_STYLE_ALLOWBEEP) != 0) {
        dwProperty = dwProperty | TXTBIT_ALLOWBEEP;
    }
    if ((dwStyle & EDIT_STYLE_RICHTEXT) != 0) {
        dwProperty = dwProperty | TXTBIT_RICHTEXT;
    }
    if ((dwStyle & EDIT_STYLE_AUTOWORDSEL) != 0) {
        dwProperty = dwProperty | TXTBIT_AUTOWORDSEL;
    }
    pOwner->dwPropBits_ = dwProperty;
}

void _edit_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
    Ex_ObjGetTextRect(hObj, pOwner->prctext_);
    INT width  = pOwner->prctext_->right - pOwner->prctext_->left;
    INT height = pOwner->prctext_->bottom - pOwner->prctext_->top;
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    INT nError = 0;
    if (_md_create(pOwner, offsetof(edit_s, mDc_), offsetof(edit_s, hBmp_),
                   offsetof(edit_s, pBits_), width, height, &nError)) {
        for (INT i = 0; i < width; i++) {
            for (INT j = 0; j < height; j++) {
                ((INT*)pOwner->pBits_)[i * height + j] = ExARGB(0, 0, 0, 255);
            }
        }
    }
    pOwner->height_ = DtoHimetric(height, 96);
    pOwner->width_  = DtoHimetric(width, 96);
    DWORD tmp       = TXTBIT_CLIENTRECTCHANGE | TXTBIT_EXTENTCHANGE;

    if ((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE ||
        (pObj->dwTextFormat_ & DT_VCENTER) == DT_VCENTER &&
            (pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) != OBJECT_STYLE_VSCROLL) {
        TEXTMETRICW tmrc = {};
        if (pOwner->mDc_) {
            LOGFONTW logfont = {};
            if (_font_getlogfont(pObj->hFont_, &logfont)) {
                HFONT   hfont   = CreateFontIndirectW(&logfont);
                HGDIOBJ hgdiobj = SelectObject(pOwner->mDc_, hfont);
                GetTextMetricsW(pOwner->mDc_, &tmrc);
                SelectObject(pOwner->mDc_, hgdiobj);
                DeleteObject(hfont);
                tmp |= TXTBIT_VIEWINSETCHANGE;
            }
        }
        pOwner->prcinset_->top = DtoHimetric((height - tmrc.tmHeight) / 2, 96);
    }
    else {
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE, 0, 0, height, 0, FALSE);
        Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE, 0, 0, width, 0, FALSE);
    }
    if (pOwner->its_) {
        ((ITextServices*)pOwner->its_)->OnTxPropertyBitsChange(tmp, tmp);
    }
}

void CALLBACK _edit_timer_caret(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    INT     nError = 0;
    obj_s*  pObj   = (obj_s*)(idEvent - TIMER_EDIT_CARET);
    edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
    if ((pOwner->flags_ & EDIT_FLAG_BCARETCONTEXT) == EDIT_FLAG_BCARETCONTEXT) {
        pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & EDIT_FLAG_BCARETCONTEXT);
        pOwner->flags_ = pOwner->flags_ | EDIT_FLAG_BCARETSHHOWED;
    }
    else {
        pOwner->flags_ = pOwner->flags_ | (EDIT_FLAG_BSHOWCARET | EDIT_FLAG_BCARETCONTEXT);
        pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & EDIT_FLAG_BCARETSHHOWED);
    }
    _obj_invalidaterect(pObj, (RECT*)((size_t)pOwner + offsetof(edit_s, rcCaret_left_)), &nError);
}

void _edit_txpaint(LPVOID pits, DWORD dwDrawAspect, LONG lindex, LPVOID pvAspect, LPVOID ptd,
                   HDC hdcDraw, HDC hicTargetDev, RECT* lprcBounds, RECT* lprcWBounds,
                   RECT* lprcUpdate, DWORD dwContinue, LONG lViewId)
{
    ((ITextServices*)pits)
        ->TxDraw(dwDrawAspect, lindex, pvAspect, (DVTARGETDEVICE*)ptd, hdcDraw, hicTargetDev,
                 (LPCRECTL)lprcBounds, (LPCRECTL)lprcWBounds, (LPRECT)lprcUpdate, NULL, dwContinue,
                 lViewId);
}

size_t _edit_scrollmsg(HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret;
    _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
    return 0;
}

LRESULT _edit_sendmessage(obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam, BOOL* sOK)
{
    LRESULT ret  = 0;
    LPVOID  pits = _edit_its(pObj);
    if (pits != nullptr) {
        auto lret = ((ITextServices*)pits)->TxSendMessage(uMsg, wParam, lParam, &ret);
        *sOK      = lret == 0;
    }
    return ret;
}

LPVOID _edit_its(obj_s* pObj)
{
    LPVOID pOwner = _obj_pOwner(pObj);
    LPVOID ret    = nullptr;
    if (pOwner != nullptr) {
        ret = ((edit_s*)pOwner)->its_;
    }
    return ret;
}

void _edit_contextmenu(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, INT x,
                       INT y)
{
    if ((pObj->dwStyle_ & EDIT_STYLE_DISABLEMENU) == EDIT_STYLE_DISABLEMENU) {
        return;
    }
    if ((pObj->dwStyle_ & EDIT_STYLE_USEPASSWORD) == EDIT_STYLE_USEPASSWORD) {
        return;
    }
    if (_obj_setfocus(hWnd, pWnd, hObj, pObj, TRUE)) {
        _obj_baseproc(hWnd, hObj, pObj, WM_COMMAND, EM_SETSEL, 0);
    }
    HMENU   hMenu = GetSubMenu(g_Li.hMenuEdit, 0);
    BOOL    sOK;
    LRESULT tmp = _edit_sendmessage(pObj, EM_CANUNDO, 0, 0, &sOK);   // 撤销
    EnableMenuItem(hMenu, 0, tmp ? 1024 : 1026);
    SIZE sz;
    _edit_sendmessage(pObj, EM_EXGETSEL, 0, (size_t)&sz, &sOK);
    LONG index = sz.cy - sz.cx;
    index      = index > 0 ? index : 0;
    EnableMenuItem(hMenu, 2, index != 0 ? 1024 : 1026);                // 剪切
    EnableMenuItem(hMenu, 3, index != 0 ? 1024 : 1026);                // 复制
    EnableMenuItem(hMenu, 5, index != 0 ? 1024 : 1026);                // 删除
    tmp = _edit_sendmessage(pObj, EM_CANPASTE, 0, 0, &sOK);            // 粘贴
    EnableMenuItem(hMenu, 4, tmp != 0 ? 1024 : 1026);                  // 剪切
    EnableMenuItem(hMenu, 7, _edit_getlen(pObj) != 0 ? 1024 : 1026);   // 全选
    Ex_TrackPopupMenu(hMenu, 0, x, y, 0, hObj, 0, NULL, 0);
}

LRESULT _edit_getlen(obj_s* pObj)
{
    SIZE sz;
    sz.cx = GTL_DEFAULT;
    sz.cy = CP_WINUNICODE;
    BOOL ret;
    return _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (size_t)&sz, 0, &ret);
}

void _edit_command(obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL sOK;
    if (wParam == WM_UNDO || wParam == WM_CUT || wParam == WM_COPY || wParam == WM_PASTE ||
        wParam == WM_CLEAR) {
        uMsg   = wParam;
        wParam = 0;

        _edit_sendmessage(pObj, uMsg, wParam, lParam, &sOK);
    }
    else {
        if (wParam == EM_SETSEL) {
            uMsg          = EDIT_MESSAGE_EXSETSEL;
            wParam        = 0;
            CHARRANGE* lp = (CHARRANGE*)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
            lp->cpMin     = 0;
            lp->cpMax     = -1;
            _edit_sendmessage(pObj, uMsg, wParam, (size_t)lp, &sOK);
            MemPool_Free(g_Li.hMemPoolMsg, lp);
        }
    }
}

size_t _edit_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
    EX_PAINTSTRUCT ps{0};
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        INT    atom;
        LPVOID pITS = ((edit_s*)ps.dwOwnerData)->its_;
        ;
        if ((ps.dwState & STATE_FOCUS) != 0) {
            atom = ATOM_FOCUS;
        }
        else if ((ps.dwState & STATE_HOVER) != 0) {
            atom = ATOM_HOVER;
        }
        else {
            atom = ATOM_NORMAL;
        }
        if ((ps.dwStyleEx & OBJECT_STYLE_EX_CUSTOMDRAW) == 0) {
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, 0, 0, ps.uWidth, ps.uHeight, ATOM_EDIT, atom,
                                255);
        }

        LPCWSTR lpBanner    = ((edit_s*)ps.dwOwnerData)->pBanner_;
        BOOL    bDrawBanner = FALSE;
        if (lpBanner != 0) {
            if (pITS == 0) {
                bDrawBanner = TRUE;
            }
            else {
                if (_edit_getlen(pObj) == 0) {
                    bDrawBanner = TRUE;
                }
            }
            if (bDrawBanner) {
                bDrawBanner = FALSE;
                if (!((ps.dwState & STATE_FOCUS) != 0 &&
                      (ps.dwStyle & EDIT_STYLE_SHOWTIPSALWAYS) == 0)) {
                    RECT* rcText = ((edit_s*)ps.dwOwnerData)->prctext_;
                    INT   dt     = 0;
                    if ((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE) {
                        dt = DT_VCENTER;
                    }
                    _canvas_drawtext(ps.hCanvas, pObj->hFont_, ((edit_s*)ps.dwOwnerData)->crBanner_,
                                     lpBanner, -1, dt, rcText->left, rcText->top, rcText->right,
                                     rcText->bottom);
                    bDrawBanner = TRUE;
                }
            }
        }

        if (pITS != 0) {
            RECT rcTmp{0};
            IntersectRect(&rcTmp, (RECT*)&ps.rcText.left, (RECT*)&ps.rcPaint.left);
            HDC    mDc    = ((edit_s*)ps.dwOwnerData)->mDc_;
            wnd_s* pWnd   = pObj->pWnd_;
            BOOL   ismove = (pWnd->dwFlags_ & EWF_BSIZEMOVING) == EWF_BSIZEMOVING;
            HDC    hDc    = _canvas_getdc(ps.hCanvas);
            _edit_txpaint(pITS, DVASPECT_CONTENT, 0, NULL, NULL, hDc, NULL, NULL, NULL, &rcTmp,
                          NULL, ismove ? TXTVIEW_INACTIVE : TXTVIEW_ACTIVE);
            BitBlt(hDc, rcTmp.left, rcTmp.top, rcTmp.right - rcTmp.left, rcTmp.bottom - rcTmp.top,
                   mDc, 0, 0, SRCPAINT);
            _canvas_releasedc(ps.hCanvas);
            if (!((pObj->dwStyle_ & EDIT_STYLE_HIDDENCARET) == EDIT_STYLE_HIDDENCARET)) {
                if (!((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BSELECTED) ==
                      EDIT_FLAG_BSELECTED)) {
                    if ((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BCARETCONTEXT) ==
                        EDIT_FLAG_BCARETCONTEXT) {
                        if (!((((edit_s*)ps.dwOwnerData)->flags_ & EDIT_FLAG_BCARETSHHOWED) ==
                              EDIT_FLAG_BCARETSHHOWED)) {
                            rcTmp.left   = ((edit_s*)ps.dwOwnerData)->rcCaret_left_;
                            rcTmp.top    = ((edit_s*)ps.dwOwnerData)->rcCaret_top_;
                            rcTmp.right  = ((edit_s*)ps.dwOwnerData)->rcCaret_right_;
                            rcTmp.bottom = ((edit_s*)ps.dwOwnerData)->rcCaret_bottom_;
                            if (rcTmp.right > 0 && rcTmp.bottom > 0) {
                                HEXBRUSH hCaretBrush =
                                    _brush_create(((edit_s*)ps.dwOwnerData)->crCaret_);
                                _canvas_fillrect(ps.hCanvas, hCaretBrush, (FLOAT)rcTmp.left,
                                                 (FLOAT)rcTmp.top, (FLOAT)rcTmp.right,
                                                 (FLOAT)rcTmp.bottom);
                                _brush_destroy(hCaretBrush);
                            }
                        }
                    }
                }
            }
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
    return 0;
}

LRESULT CALLBACK _edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL   bFree  = FALSE;
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (uMsg == WM_CREATE) {
            _edit_init(hWnd, hObj, pObj);
        }
        else if (uMsg == WM_SETCURSOR) {
            edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
            POINT   pt;
            pt.x = GET_X_LPARAM(wParam);
            pt.y = GET_Y_LPARAM(wParam);
            if (PtInRect((RECT*)pOwner->prctext_, pt) ? FALSE : TRUE) {
                return 0;
            }
        }
        else if (uMsg == WM_DESTROY) {
            BOOL ret = FALSE;
            _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
            _edit_unint(pObj);
        }
        else if (uMsg == WM_SETPARENTAFTER) {
            _edit_size(hWnd, hObj, pObj);
        }
        else if (uMsg == WM_SIZE) {
            _edit_size(hWnd, hObj, pObj);
        }
        else if (uMsg == WM_PAINT) {
            return _edit_paint(hWnd, hObj, pObj);
        }
        else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) {
            LRESULT ret  = 0;
            LPVOID  pits = _edit_its(pObj);
            if (pits != nullptr) {
                if (LOWORD(wParam) == SB_THUMBTRACK || LOWORD(wParam) == SB_THUMBPOSITION) {
                    POINT scrollPos;

                    ((ITextServices*)pits)
                        ->TxSendMessage(EM_GETSCROLLPOS, 0, (LPARAM)&scrollPos, NULL);
                    if (uMsg == WM_VSCROLL) {
                        scrollPos.y = Ex_ObjScrollGetTrackPos(hObj, SB_VERT);
                    }
                    else {
                        scrollPos.x = Ex_ObjScrollGetTrackPos(hObj, SB_HORZ);
                    }
                    ((ITextServices*)pits)
                        ->TxSendMessage(EM_SETSCROLLPOS, 0, reinterpret_cast<LPARAM>(&scrollPos),
                                        &ret);
                }
                else {
                    return _edit_scrollmsg(hObj, pObj, uMsg, wParam, lParam);
                }
            }
            return ret;
        }
        else if (uMsg == WM_MOUSEWHEEL) {
            uMsg = 0;
            if ((pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) == OBJECT_STYLE_VSCROLL) {
                uMsg = WM_VSCROLL;
            }
            else if ((pObj->dwStyle_ & OBJECT_STYLE_HSCROLL) == OBJECT_STYLE_HSCROLL) {
                uMsg = WM_HSCROLL;
            }
            auto zDelta = (short)HIWORD(wParam);
            if (uMsg != 0) {
                _edit_scrollmsg(hObj, pObj, uMsg, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
                return 1;
            }
        }
        else if (uMsg == WM_MOUSEHOVER) {
            _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, &nError);
        }
        else if (uMsg == WM_MOUSELEAVE) {
            _obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, &nError);
        }
        else if (uMsg == WM_SETFOCUS) {
            IME_Control(hWnd, pObj->pWnd_,
                        !((pObj->dwStyle_ & EDIT_STYLE_USEPASSWORD) == EDIT_STYLE_USEPASSWORD));
            ((ITextServices*)_edit_its(pObj))->OnTxUIActivate();
            BOOL ret = FALSE;
            _edit_sendmessage(pObj, uMsg, 0, 0, &ret);
            _obj_setuistate(pObj, STATE_FOCUS, FALSE, 0, TRUE, &nError);
            if (!((pObj->dwStyle_ & EDIT_STYLE_HIDDENCARET) == EDIT_STYLE_HIDDENCARET)) {
                SetTimer(hWnd, (size_t)pObj + TIMER_EDIT_CARET, 500, _edit_timer_caret);
            }
        }
        else if (uMsg == WM_KILLFOCUS) {
            if (!((pObj->dwStyle_ & EDIT_STYLE_HIDDENCARET) == EDIT_STYLE_HIDDENCARET)) {
                KillTimer(hWnd, (size_t)pObj + TIMER_EDIT_CARET);
                edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
                if ((pOwner->flags_ & (EDIT_FLAG_BSHOWCARET | EDIT_FLAG_BCARETCONTEXT)) ==
                    (EDIT_FLAG_BSHOWCARET | EDIT_FLAG_BCARETCONTEXT)) {
                    pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & (EDIT_FLAG_BSHOWCARET |
                                                                         EDIT_FLAG_BCARETCONTEXT));

                    _obj_invalidaterect(
                        pObj, (RECT*)((size_t)pOwner + offsetof(edit_s, rcCaret_left_)), &nError);
                }
            }
            BOOL ret = FALSE;
            _edit_sendmessage(pObj, uMsg, 0, 0, &ret);
            ((ITextServices*)_edit_its(pObj))->OnTxUIDeactivate();
            nError = 0;
            _obj_setuistate(pObj, STATE_FOCUS, TRUE, 0, TRUE, &nError);
            DestroyCaret();
            IME_Control(hWnd, pObj->pWnd_, FALSE);
        }
        else if (uMsg == WM_CHAR) {
            if ((pObj->dwStyle_ & EDIT_STYLE_NUMERICINPUT) == EDIT_STYLE_NUMERICINPUT) {
                if (!(wParam > 44 && wParam < 58 && wParam != 47)) {
                    return 1;
                }
            }
            if ((pObj->dwStyle_ & EDIT_STYLE_LETTER) == EDIT_STYLE_LETTER) {
                if (!(wParam > 64 && wParam < 91 || wParam > 96 && wParam < 123)) {
                    return 1;
                }
            }
            if ((pObj->dwStyle_ & EDIT_STYLE_NUMERIC_LETTER) == EDIT_STYLE_NUMERIC_LETTER) {
                if (!(wParam > 47 && wParam < 58 || wParam > 64 && wParam < 91 ||
                      wParam > 96 && wParam < 123)) {
                    return 1;
                }
            }
            if (wParam == VK_TAB) {
                if (!((pObj->dwStyle_ & EDIT_STYLE_ALLOWTAB) == EDIT_STYLE_ALLOWTAB)) {
                    return 1;
                }
            }
            BOOL ret = FALSE;
            _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
        }
        else if (uMsg == WM_CONTEXTMENU) {
            _edit_contextmenu(hWnd, pObj->pWnd_, hObj, pObj, wParam, GET_X_LPARAM(lParam),
                              GET_Y_LPARAM(lParam));
        }
        else if (uMsg == WM_COMMAND) {
            _edit_command(pObj, uMsg, wParam, lParam);
        }
        else if (uMsg == WM_NOTIFY) {}
        else if (uMsg == WM_ERASEBKGND) {}
        else if (uMsg == WM_SETTEXT) {
            if (lParam) {
                if (!((pObj->dwStyle_ & EDIT_STYLE_NEWLINE) ==
                      EDIT_STYLE_NEWLINE))   // 禁止回车时,删除\r\n
                {
                    LPCWSTR src = (LPCWSTR)lParam;
                    int length = lstrlenW(src);  // 获取源字符串长度

                    // 分配目标缓冲区（足够容纳原字符串）
                    wchar_t* dest = new wchar_t[length + 1];
                    int dest_index = 0;  // 目标缓冲区当前写入位置

                    // 遍历源字符串
                    for (int i = 0; i < length; ) {
                        // 检查当前是否匹配 "\r\n"
                        if (src[i] == L'\r' && (i + 1 < length) && src[i + 1] == L'\n') {
                            i += 2;  // 跳过这两个字符
                        }
                        else {
                            dest[dest_index++] = src[i++];  // 复制字符
                        }
                    }

                    // 添加字符串结束符
                    dest[dest_index] = L'\0';
                    auto ret = ((ITextServices*)_edit_its(pObj))->TxSetText(dest);
                    delete[] dest;
                    return ret;
                }
            }
            return ((ITextServices*)_edit_its(pObj))->TxSetText((LPCWSTR)lParam);
        }
        else if (uMsg == EDIT_MESSAGE_SETCUEBANNER) {
            edit_s* pOwner    = (edit_s*)_obj_pOwner(pObj);
            pOwner->crBanner_ = wParam;
            pOwner->pBanner_  = StrDupW((LPCWSTR)lParam);
            _obj_invalidaterect(pObj, 0, &nError);
            return 0;
        }
        else if (uMsg == WM_SYSCOLORCHANGE) {
            edit_s* pOwner = (edit_s*)_obj_pOwner(pObj);
            if (wParam == COLOR_EX_TEXT_NORMAL) {
                CHARFORMAT2W* pcf = (CHARFORMAT2W*)pOwner->pcf_;
                pcf->dwMask       = EDIT_SELECT_CHARFORMAT_COLOR;
                pcf->crTextColor  = ExARGB2RGB(lParam);
                ((ITextServices*)_edit_its(pObj))
                    ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
            }
            else if (wParam == COLOR_EX_EDIT_CARET) {
                pOwner->crCaret_ = lParam;
            }
            else if (wParam == COLOR_EX_EDIT_BANNER) {
                pOwner->crBanner_ = lParam;
            }
            return 0;
        }
        else if (uMsg == EDIT_MESSAGE_LOAD_RTF) {
            if (FLAGS_CHECK(pObj->dwStyle_, EDIT_STYLE_RICHTEXT)) {
                return _edit_load_rtf(pObj, (LPCWSTR)lParam, wParam);
            }
            else {
                return 0;
            }
        }
        else if (uMsg == EDIT_MESSAGE_INSERT_BITMAP) {
            BOOL          bOK;
            IRichEditOle* pRichEditOle;
            _edit_sendmessage(pObj, EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, &bOK);
            if (bOK) {
                CImageDataObject::InsertBitmap(pRichEditOle, (HBITMAP)lParam);
            }
        }
        else if (uMsg == WM_NCCREATE || uMsg == WM_NCCALCSIZE) {
            // 拦截这两个消息
        }
        else if (uMsg == WM_SETFONT) {
            edit_s*  pOwner  = (edit_s*)_obj_pOwner(pObj);
            LOGFONTW logfont = {};
            pObj->hFont_     = (HEXFONT)wParam;
            _font_getlogfont(pObj->hFont_, &logfont);
            CHARFORMAT2W* pcf    = (CHARFORMAT2W*)pOwner->pcf_;
            DWORD         dwMask = EDIT_SELECT_CHARFORMAT_SIZE | EDIT_SELECT_CHARFORMAT_COLOR |
                           EDIT_SELECT_CHARFORMAT_FACE;
            DWORD dwEffects = 0;
            if (logfont.lfWeight != 400) {
                dwEffects = dwEffects | CFE_BOLD;
                dwMask    = dwMask | EDIT_SELECT_CHARFORMAT_BOLD;
            }

            if (logfont.lfItalic != 0) {
                dwEffects = dwEffects | CFE_ITALIC;
                dwMask    = dwMask | EDIT_SELECT_CHARFORMAT_ITALIC;
            }

            if (logfont.lfUnderline != 0) {
                dwEffects = dwEffects | CFE_UNDERLINE;
                dwMask    = dwMask | EDIT_SELECT_CHARFORMAT_UNDERLINE;
            }

            if (logfont.lfStrikeOut != 0) {
                dwEffects = dwEffects | CFE_STRIKEOUT;
                dwMask    = dwMask | EDIT_SELECT_CHARFORMAT_STRIKEOUT;
            }
            pcf->dwMask          = dwMask;
            pcf->dwEffects       = dwEffects;
            pcf->yHeight         = -logfont.lfHeight * 1440 / 96;
            pcf->bCharSet        = logfont.lfCharSet;
            pcf->bPitchAndFamily = logfont.lfPitchAndFamily;
            RtlMoveMemory(pcf->szFaceName, logfont.lfFaceName, LF_FACESIZE);
            ((ITextServices*)_edit_its(pObj))
                ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
            _edit_size(hWnd, hObj, pObj);
            return 0;
        }
        else {
            if (uMsg == WM_KEYDOWN) {
                if (wParam == VK_RETURN) {
                    if ((pObj->dwStyle_ & EDIT_STYLE_NEWLINE) != EDIT_STYLE_NEWLINE)   // 拦截回车
                    {
                        return 1;
                    }
                }
                else if (wParam == VK_SPACE) {
                    // 拦截空格.Msftedit回到首行bug
                    return 1;
                }
            }
            LRESULT ret = _edit_sendmessage(pObj, uMsg, wParam, lParam, &bFree);
            if (bFree) {
                return ret;
            }
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

DWORD CALLBACK _edit_stream_proc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{

    LPCWSTR rtf   = ((EX_EDIT_STREAM*)dwCookie)->rtf;
    size_t  len   = ((EX_EDIT_STREAM*)dwCookie)->len;
    DWORD   load  = ((EX_EDIT_STREAM*)dwCookie)->flags;
    size_t  start = ((EX_EDIT_STREAM*)dwCookie)->pos;
    if (load) {
        if (len > cb) {
            if (start + cb - len < 0) {
                len = cb;
            }
            else {
                len = len - start;
            }
            if (len <= 0) {
                *pcb = 0;
            }
            else {
                RtlMoveMemory(pbBuff, (LPCWSTR)((size_t)rtf + start), len);
                ((EX_EDIT_STREAM*)dwCookie)->pos += len;
                *pcb = len;
            }
        }
        else {
            RtlMoveMemory(pbBuff, rtf, len);
            *pcb = len;
        }
    }
    else {
        ((EX_EDIT_STREAM*)dwCookie)->rtf = LPCWSTR((size_t)((EX_EDIT_STREAM*)dwCookie)->rtf + cb);
        size_t size                      = LocalSize((HLOCAL)rtf);
        if (size < len) {
            rtf = (LPCWSTR)LocalAlloc(64, 2 * len);
            RtlMoveMemory((LPVOID)rtf, ((EX_EDIT_STREAM*)dwCookie)->rtf, size);
            LocalFree((HLOCAL)((EX_EDIT_STREAM*)dwCookie)->rtf);
            ((EX_EDIT_STREAM*)dwCookie)->rtf = rtf;
        }
        RtlMoveMemory((LPVOID)((size_t)rtf + start), pbBuff, cb);
        ((EX_EDIT_STREAM*)dwCookie)->pos += cb;
    }
    return 0;
}

size_t _edit_load_rtf(obj_s* pObj, LPCWSTR rtf, size_t len)
{
    EX_EDIT_STREAM stream = {0};
    stream.s.dwCookie     = (DWORD_PTR)&stream;
    stream.s.pfnCallback  = _edit_stream_proc;
    stream.rtf            = rtf;
    stream.len            = len;
    stream.flags          = 1;
    BOOL ok               = FALSE;
    return _edit_sendmessage(pObj, EM_STREAMIN, 2, (size_t)&stream, &ok);
}


void CImageDataObject::InsertBitmap(IRichEditOle* pRichEditOle, HBITMAP hBitmap)
{
    SCODE sc;

    // Get the image data object
    //
    CImageDataObject* pods = new CImageDataObject;
    LPDATAOBJECT      lpDataObject;
    pods->QueryInterface(IID_IDataObject, (void**)&lpDataObject);

    pods->SetBitmap(hBitmap);

    // Get the RichEdit container site
    //
    IOleClientSite* pOleClientSite;
    pRichEditOle->GetClientSite(&pOleClientSite);

    // Initialize a Storage Object
    //
    IStorage* pStorage;

    LPLOCKBYTES lpLockBytes = NULL;
    sc                      = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);


    sc = ::StgCreateDocfileOnILockBytes(
        lpLockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);
    if (sc != S_OK) {
        lpLockBytes = NULL;
    }

    // The final ole object which will be inserted in the richedit control
    //
    IOleObject* pOleObject;
    pOleObject = pods->GetOleObject(pOleClientSite, pStorage);

    // all items are "contained" -- this makes our reference to this object
    //  weak -- which is needed for links to embedding silent update.
    OleSetContainedObject(pOleObject, TRUE);

    // Now Add the object to the RichEdit
    //
    REOBJECT reobject;
    ZeroMemory(&reobject, sizeof(REOBJECT));
    reobject.cbStruct = sizeof(REOBJECT);

    CLSID clsid;
    sc = pOleObject->GetUserClassID(&clsid);

    reobject.clsid    = clsid;
    reobject.cp       = REO_CP_SELECTION;
    reobject.dvaspect = DVASPECT_CONTENT;
    reobject.poleobj  = pOleObject;
    reobject.polesite = pOleClientSite;
    reobject.pstg     = pStorage;

    // Insert the bitmap at the current location in the richedit control
    //
    pRichEditOle->InsertObject(&reobject);

    // Release all unnecessary interfaces
    //
    pOleObject->Release();
    pOleClientSite->Release();
    pStorage->Release();
    lpDataObject->Release();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
    STGMEDIUM stgm;
    stgm.tymed          = TYMED_GDI;   // Storage medium = HBITMAP handle
    stgm.hBitmap        = hBitmap;
    stgm.pUnkForRelease = NULL;   // Use ReleaseStgMedium

    FORMATETC fm;
    fm.cfFormat = CF_BITMAP;          // Clipboard format = CF_BITMAP
    fm.ptd      = NULL;               // Target Device = Screen
    fm.dwAspect = DVASPECT_CONTENT;   // Level of detail = Full content
    fm.lindex   = -1;                 // Index = Not applicaple
    fm.tymed    = TYMED_GDI;          // Storage medium = HBITMAP handle

    this->SetData(&fm, &stgm, TRUE);
}

IOleObject* CImageDataObject::GetOleObject(IOleClientSite* pOleClientSite, IStorage* pStorage)
{

    SCODE       sc;
    IOleObject* pOleObject;
    sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, &m_fromat,
                                   pOleClientSite, pStorage, (void**)&pOleObject);
    if (sc != S_OK) return nullptr;
    return pOleObject;
}
