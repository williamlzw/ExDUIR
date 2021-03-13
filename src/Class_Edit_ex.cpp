#include "Class_Edit_ex.h"
#include <textserv.h>
#include <WinUser.h>

class TextHost : public ITextHost {
    edit_s *m_pOwner;
public:

    TextHost(edit_s *pOwner) {
        m_pOwner = pOwner;
    };

    HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
        return E_NOINTERFACE;
    };

    ULONG STDMETHODCALLTYPE AddRef(void) {
        return E_NOINTERFACE;
    };

    ULONG STDMETHODCALLTYPE Release(void) {
        return E_NOINTERFACE;
    };

    //@cmember Get the DC for the host
    HDC TxGetDC() {
        return m_pOwner->mDc_;
    };

    //@cmember Release the DC gotten from the host
    INT TxReleaseDC(HDC hdc) {
        return ReleaseDC(0, hdc);
    };

    //@cmember Show the scroll bar
    BOOL TxShowScrollBar(INT fnBar, BOOL fShow) {
        Ex_ObjScrollShow(m_pOwner->pObj_->hObj_, fnBar, fShow);
        return TRUE;
    };

    //@cmember Enable the scroll bar
    BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags) {
        Ex_ObjScrollEnable(m_pOwner->pObj_->hObj_, fuSBFlags, fuArrowflags);
        return TRUE;
    };

    //@cmember Set the scroll range
    BOOL TxSetScrollRange(
            INT fnBar,
            LONG nMinPos,
            INT nMaxPos,
            BOOL fRedraw) {
        return TRUE;
    };

    //@cmember Set the scroll position
    BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw) {
        obj_s* pObj = m_pOwner->pObj_;
        EXHANDLE hObj = pObj->hObj_;
        Ex_ObjScrollSetPos(hObj, fnBar, nPos, fRedraw);
        return TRUE;
    };

    //@cmember InvalidateRect
    void TxInvalidateRect(LPCRECT prc, BOOL fMode) {
        _obj_invalidaterect(m_pOwner->pObj_, (void *) prc, 0);
    };

    //@cmember Send a WM_PAINT to the window
    void TxViewChange(BOOL fUpdate) {
    };

    //@cmember Create the caret
    BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) {
        EXHANDLE hCanvas = m_pOwner->hCanvasCaret_;
        if (hCanvas) {
            if (Flag_Query(EXGF_DPI_ENABLE)) {
                xWidth = 2;
            }
            _canvas_resize(hCanvas, xWidth, yHeight);
            if (_canvas_begindraw(hCanvas)) {
                _canvas_clear(hCanvas, m_pOwner->crCaret_);
                _canvas_enddraw(hCanvas);
            }
            m_pOwner->rcCaret_left_ = 0;
            m_pOwner->rcCaret_top_ = 0;
            m_pOwner->rcCaret_right_ = xWidth;
            m_pOwner->rcCaret_bottom_ = yHeight;
        }
        return CreateCaret(_obj_gethWnd(m_pOwner->pObj_), (HBITMAP)-1, xWidth, yHeight);
    };

    //@cmember Show the caret
    BOOL TxShowCaret(BOOL fShow) {
        if (fShow) {
            FLAGS_ADD(m_pOwner->flags_, eef_bShowCaret);
            ShowCaret(_obj_gethWnd(m_pOwner->pObj_));
        }
        else {
            FLAGS_DEL(m_pOwner->flags_, eef_bShowCaret);
            HideCaret(_obj_gethWnd(m_pOwner->pObj_));
        }
        return TRUE;
    };

    //@cmember Set the caret position
    BOOL TxSetCaretPos(INT x, INT y) {
        obj_s* pObj = m_pOwner->pObj_;
        OffsetRect((LPRECT)&m_pOwner->rcCaret_left_, x, y);
        if (FLAGS_CHECK(pObj->dwState_, 状态_焦点)) {
            FLAGS_ADD(m_pOwner->flags_, eef_bCaretContext | eef_bShowCaret);
            FLAGS_DEL(m_pOwner->flags_, eef_bCaretShowed);
            _obj_invalidaterect(pObj, (LPRECT)&m_pOwner->rcCaret_left_, 0);
        }
        x += pObj->w_left_;
        y += pObj->w_top_;
        SetCaretPos(x, y);
        return TRUE;
    };

    //@cmember Create a timer with the specified timeout
    BOOL TxSetTimer(UINT idTimer, UINT uTimeout) {
        return SetTimer(_obj_gethWnd(m_pOwner->pObj_), idTimer, uTimeout, NULL);
    };

    //@cmember Destroy a timer
    void TxKillTimer(UINT idTimer) {
        KillTimer(_obj_gethWnd(m_pOwner->pObj_), idTimer);
    };

    //@cmember Scroll the content of the specified window's client area
    void TxScrollWindowEx(
            INT dx,
            INT dy,
            LPCRECT lprcScroll,
            LPCRECT lprcClip,
            HRGN hrgnUpdate,
            LPRECT lprcUpdate,
            UINT fuScroll) {};

    //@cmember Get mouse capture
    void TxSetCapture(BOOL fCapture) {};

    //@cmember Set the focus to the text window
    void TxSetFocus() {

    };

    //@cmember Establish a new cursor shape
    void TxSetCursor(HCURSOR hcur, BOOL fText) {};

    //@cmember Converts screen coordinates of a specified point to the client coordinates
    BOOL TxScreenToClient(LPPOINT lppt) {
        return FALSE;
    };

    //@cmember Converts the client coordinates of a specified point to screen coordinates
    BOOL TxClientToScreen(LPPOINT lppt) {
        return FALSE;
    };

    //@cmember Request host to activate text services
    HRESULT TxActivate(LONG *plOldState) {
        return E_FAIL;
    };

    //@cmember Request host to deactivate text services
    HRESULT TxDeactivate(LONG lNewState) {
        return E_FAIL;
    };

    //@cmember Retrieves the coordinates of a window's client area
    HRESULT TxGetClientRect(LPRECT prc) {
        CopyMemory(prc, m_pOwner->prctext_, sizeof(RECT));
        return S_OK;
    };

    //@cmember Get the view rectangle relative to the inset
    HRESULT TxGetViewInset(LPRECT prc) {
        // 请求文本宿主窗口中文本周围的空白区域的尺寸。
        CopyMemory(prc, m_pOwner->prcinset_, sizeof(RECT));
        return S_OK;
    };

    //@cmember Get the default character format for the text
    HRESULT TxGetCharFormat(const CHARFORMATW **ppCF) {
        *ppCF = (CHARFORMATW*)m_pOwner->pcf_;
        return S_OK;
    };

    //@cmember Get the default paragraph format for the text
    HRESULT TxGetParaFormat(const PARAFORMAT **ppPF) {
        *ppPF = (PARAFORMAT*)m_pOwner->ppf_;
        return S_OK;
    };

    //@cmember Get the background color for the window
    COLORREF TxGetSysColor(int nIndex) {
        if (nIndex = COLOR_WINDOWTEXT) {
            return ExARGB2RGB(_obj_getcolor(m_pOwner->pObj_, COLOR_EX_TEXT_NORMAL));
        }
        else {
            return GetSysColor(nIndex);
        }
    };

    //@cmember Get the background (either opaque or transparent)
    HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle) {
        *pstyle = TXTBACK_TRANSPARENT;
        return S_OK;
    };

    //@cmember Get the maximum length for the text
    HRESULT TxGetMaxLength(DWORD *plength) {
        return S_OK;
    };

    //@cmember Get the bits representing requested scroll bars for the window
    HRESULT TxGetScrollBars(DWORD *pdwScrollBar) {
        obj_s* pObj = m_pOwner->pObj_;
        *pdwScrollBar = ES_AUTOHSCROLL | ES_AUTOVSCROLL;
        if (FLAGS_CHECK(pObj->dwStyle_, EOS_HSCROLL)) {
            *pdwScrollBar |= WS_HSCROLL;
        }
        if (FLAGS_CHECK(pObj->dwStyle_, EOS_VSCROLL)) {
            *pdwScrollBar |= WS_VSCROLL;
        }
        if (FLAGS_CHECK(pObj->dwStyle_, EOS_DISABLENOSCROLL)) {
            *pdwScrollBar |= ES_DISABLENOSCROLL;
        }
        return S_OK;
    };

    //@cmember Get the character to display for password input
    HRESULT TxGetPasswordChar(_Out_ TCHAR *pch) {
        *pch = m_pOwner->charPsw_;
        return S_OK;
    };

    //@cmember Get the accelerator character
    HRESULT TxGetAcceleratorPos(LONG *pcp) {
        return S_OK;
    };

    //@cmember Get the native size
    HRESULT TxGetExtent(LPSIZEL lpExtent) {
        lpExtent->cx = 2540;
        lpExtent->cy = 2540;
        return S_OK;
    };

    //@cmember Notify host that default character format has changed
    HRESULT OnTxCharFormatChange(const CHARFORMATW *pCF) {
        return E_FAIL;
    };

    //@cmember Notify host that default paragraph format has changed
    HRESULT OnTxParaFormatChange(const PARAFORMAT *pPF) {
        return E_FAIL;
    };

    //@cmember Bulk access to bit properties
    HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) {
        *pdwBits = dwMask & m_pOwner->dwPropBits_;
        return S_OK;
    };

    //@cmember Notify host of events
    HRESULT TxNotify(DWORD iNotify, void *pv) {
        obj_s* pObj = m_pOwner->pObj_;
        if (iNotify != EN_UPDATE) {
            _obj_dispatchnotify(_obj_gethWnd(pObj), pObj, pObj->hObj_, 0, iNotify, 0, (size_t)pv);
            if (iNotify == EN_SELCHANGE) {
                SELCHANGE* pSelChange = (SELCHANGE*)pv;
                if (pSelChange->chrg.cpMin == pSelChange->chrg.cpMax) {
                    FLAGS_DEL(m_pOwner->flags_, eef_bSelected);
                }
                else {
                    FLAGS_ADD(m_pOwner->flags_, eef_bSelected);
                }
            }
        }
        return S_OK;
    };

    // East Asia Methods for getting the Input Context
    HIMC TxImmGetContext() {
        return NULL;
    };

    void TxImmReleaseContext(HIMC himc) {};

    //@cmember Returns HIMETRIC size of the control bar.
    HRESULT TxGetSelectionBarWidth(LONG *lSelBarWidth) {
        *lSelBarWidth = 0;
        return S_OK;
    };
};

void _edit_init(HWND hWnd, EXHANDLE hObj, obj_s *pObj) {
    int nError = 0;

    edit_s *pOwner = (edit_s *) _struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(edit_s), &nError);
    if (pOwner != 0) {
        pOwner->pObj_ = pObj;
        pOwner->crBanner_ = ExRGBA(0, 0, 0, 128);
        pOwner->charPsw_ = 9679;
        pOwner->hCanvasCaret_ = _canvas_createfromobj(hObj, 0, 0, 0, &nError);
        pOwner->crCaret_ = ExRGBA(0, 0, 0, 255);

        _struct_createfromaddr(pOwner, offsetof(edit_s, prctext_), sizeof(RECT), &nError);
        _struct_createfromaddr(pOwner, offsetof(edit_s, prcinset_), sizeof(RECT), &nError);
        _edit_setppf(pObj, pOwner);
        _edit_setpropbits(pObj, pOwner);

        _edit_size(hWnd, hObj, pObj);

        TextHost * pITH = new TextHost(pOwner);

        CTSFunc CreateTextServicesL = NULL;
        CreateTextServicesL = (CTSFunc) GetProcAddress(g_Ri.hRiched20, "CreateTextServices");
        if (pITH != 0) {
            IUnknown *pIUnk = nullptr;
            if (CreateTextServicesL(NULL, pITH, &pIUnk) == 0) {
                ITextServices *pITS = nullptr;
                pIUnk->QueryInterface(IID_ITextServicesA, (void **) &pITS);
                pOwner->ith_ = pITH;
                pOwner->its_ = pITS;
                pITS->OnTxInPlaceActivate(0);
                LRESULT ret;
                pITS->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &ret);
                pITS->TxSendMessage(EM_SETEVENTMASK, 0, ENM_CHANGE | ENM_SELCHANGE | ENM_LINK | ENM_DRAGDROPDONE, &ret);
            }
        }
        if (g_Li.hMenuEdit == 0) {
            g_Li.hMenuEdit = LoadMenuW(GetModuleHandleW(L"user32.dll"), MAKEINTRESOURCEW(1));
        }
    }
}

void _edit_unint(obj_s *pObj) {
    edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
    if (pOwner != 0) {
        _canvas_destroy(pOwner->hCanvasCaret_);
        ((ITextServices *) pOwner->its_)->Release();
        ((ITextHost *) pOwner->ith_)->Release();
        delete pOwner->ith_;
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, pcf_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, ppf_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, prctext_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, prcinset_));
        _struct_destroyfromaddr(pOwner, offsetof(edit_s, pBanner_));
        _md_destroy(pOwner, offsetof(edit_s, mDc_), offsetof(edit_s, hBmp_), offsetof(edit_s, pBits_));
        _struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
    }
}

void _edit_setpcf(obj_s *pObj, edit_s *pOwner, int height) {
    //typedef struct _charformat {
    //UINT     cbSize;
    //DWORD    dwMask;
    //DWORD    dwEffects;
    //LONG     yHeight;
    //LONG     yOffset;
    //COLORREF crTextColor;
    //BYTE     bCharSet;
    //BYTE     bPitchAndFamily;
    //TCHAR    szFaceName[LF_FACESIZE];
    //} CHARFORMAT;
    int nError = 0;
    CHARFORMATW *pcf = (CHARFORMATW *) _struct_createfromaddr(pOwner, offsetof(edit_s, pcf_), sizeof(CHARFORMATW),
                                                              &nError);
    if (pcf != 0) {
        LOGFONTW *logfont = (LOGFONTW *) Ex_MemAlloc(sizeof(LOGFONTW));
        if (logfont != 0) {
            _font_getlogfont(pObj->hFont_, logfont);
            pcf->cbSize = sizeof(CHARFORMATW);
            DWORD dwMask = CFM_BOLD | CFE_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_SIZE | CFM_COLOR | CFM_FACE |
                           CFM_CHARSET | CFM_OFFSET;
            DWORD tmp = 0;
            if (logfont->lfWeight != 400) {
                tmp = tmp | CFE_BOLD;
            }
            BYTE b = logfont->lfItalic;
            if (b != 0) {
                tmp = tmp | CFE_ITALIC;
            }
            BYTE under = logfont->lfUnderline;
            if (under != 0) {
                tmp = tmp | CFE_UNDERLINE;
            }
            BYTE StrikeOut = logfont->lfStrikeOut;
            if (StrikeOut != 0) {
                tmp = tmp | CFE_STRIKEOUT;
            }
            pcf->dwEffects = tmp;
            pcf->yHeight = -logfont->lfHeight * 1440 / height;
            BYTE CharSet = logfont->lfCharSet;
            pcf->bCharSet = CharSet;
            BYTE PitchAndFamily = logfont->lfPitchAndFamily;
            pcf->bPitchAndFamily = PitchAndFamily;
            pcf->crTextColor = ExARGB2RGB(_obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL));
            RtlMoveMemory(pcf->szFaceName, logfont->lfFaceName, lstrlenW((LPCWSTR) logfont->lfFaceName) * 2);
            pcf->dwMask = dwMask;
            Ex_MemFree(logfont);
        }
    }
}

void _edit_setppf(obj_s *pObj, edit_s *pOwner) {
    //typedef struct _paraformat {
    //UINT  cbSize;
    //DWORD dwMask;
    //WORD  wNumbering;
    //WORD  wReserved;
    // LONG  dxStartIndent;
    //LONG  dxRightIndent;
    //LONG  dxOffset;
    //WORD  wAlignment;
    //SHORT cTabCount;
    //LONG  rgxTabs;
    //} PARAFORMAT;
    int nError = 0;
    PARAFORMAT *ppf = (PARAFORMAT *) _struct_createfromaddr(pOwner, offsetof(edit_s, ppf_), sizeof(PARAFORMAT),
                                                            &nError);
    if (ppf != 0) {
        ppf->cbSize = sizeof(PARAFORMAT);
        DWORD dwMask = PFM_ALIGNMENT;
        WORD tmp;
        if ((pObj->dwTextFormat_ & DT_CENTER) == DT_CENTER) {
            tmp = PFA_CENTER;
        } else {
            if ((pObj->dwTextFormat_ & DT_RIGHT) == DT_RIGHT) {
                tmp = PFA_RIGHT;
            } else {
                tmp = PFA_LEFT;
            }
        }
        ppf->wAlignment = tmp;
        ppf->cTabCount = 1;
        ppf->rgxTabs[0] = 720;
        ppf->dwMask = dwMask;
    }
}

void _edit_setpropbits(obj_s *pObj, edit_s *pOwner) {
    int dwStyle = pObj->dwStyle_;
    int dwProperty = 0;
    if (!((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE)) {
        dwProperty = dwProperty | TXTBIT_MULTILINE;
        if ((dwStyle & EOS_VSCROLL) != 0) {
            if ((dwStyle & EOS_HSCROLL) == 0) {
                dwProperty = dwProperty | TXTBIT_WORDWRAP;
            }
        }
    }
    if ((dwStyle & 编辑框风格_允许拖拽) == 0) {
        dwProperty = dwProperty | TXTBIT_DISABLEDRAG;
    }
    if ((dwStyle & 编辑框风格_只读) != 0) {
        dwProperty = dwProperty | TXTBIT_READONLY;
    }
    if ((dwStyle & 编辑框风格_密码输入) != 0) {
        dwProperty = dwProperty | TXTBIT_USEPASSWORD;
    }
    if ((dwStyle & 编辑框风格_显示选择文本) == 0) {
        dwProperty = dwProperty | TXTBIT_HIDESELECTION;
    }
    if ((dwStyle & 编辑框风格_允许鸣叫) != 0) {
        dwProperty = dwProperty | TXTBIT_ALLOWBEEP;
    }
    if ((dwStyle & 编辑框风格_丰富文本) != 0) {
        dwProperty = dwProperty | TXTBIT_RICHTEXT;
    }
    if ((dwStyle & 编辑框风格_自动选择字符) != 0) {
        dwProperty = dwProperty | TXTBIT_AUTOWORDSEL;
    }
    pOwner->dwPropBits_ = dwProperty;
}

void _edit_size(HWND hWnd, EXHANDLE hObj, obj_s *pObj) {
    edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
    void *lpRc = pOwner->prctext_;
    Ex_ObjGetTextRect(hObj, lpRc);
    RECT rcText{0};
    RtlMoveMemory(&rcText, lpRc, 16);
    int width = rcText.right - rcText.left;
    int height = rcText.bottom - rcText.top;
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    int nError = 0;
    if (_md_create(pOwner, offsetof(edit_s, mDc_), offsetof(edit_s, hBmp_), offsetof(edit_s, pBits_), width, height,
                   &nError)) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                ((int*)pOwner->pBits_)[i * height + j] = ExRGBA(0, 0, 0, 255);
            }
        }
    }
    _edit_setpcf(pObj, pOwner, height);
    DWORD tmp = TXTBIT_CLIENTRECTCHANGE | TXTBIT_EXTENTCHANGE | TXTBIT_CHARFORMATCHANGE;
    if ((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE) {
        tmp = tmp | TXTBIT_VIEWINSETCHANGE;
        TEXTMETRICW *lpRc = (TEXTMETRICW *) Ex_MemAlloc(sizeof(TEXTMETRICW));
        HDC mdc = nullptr;
        if (lpRc != 0) {
            mdc = pOwner->mDc_;
            if (mdc != nullptr) {
                LOGFONTW *logfont = (LOGFONTW *) Ex_MemAlloc(sizeof(LOGFONTW));
                if (logfont != 0) {
                    if (_font_getlogfont(pObj->hFont_, logfont)) {
                        HFONT hfont = CreateFontIndirectW(logfont);
                        HGDIOBJ hgdiobj = SelectObject(mdc, hfont);
                        GetTextMetricsW(mdc, lpRc);
                        SelectObject(mdc, hgdiobj);
                        DeleteObject(hfont);
                    }
                    Ex_MemFree(logfont);
                }
            }
            __set_int(pOwner->prcinset_, 4, DtoHimetric((height - lpRc->tmHeight) / 2, height));// top
            Ex_MemFree(lpRc);
        }
    } else {
        Ex_ObjScrollSetInfo(hObj, SB_VERT, SIF_PAGE, 0, 0, height, 0, false);
        Ex_ObjScrollSetInfo(hObj, SB_HORZ, SIF_PAGE, 0, 0, width, 0, false);
    }
    if (pOwner->its_ != nullptr) {
        ((ITextServices *) pOwner->its_)->OnTxPropertyBitsChange(tmp, tmp);
    }
}

void CALLBACK _edit_timer_caret(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    obj_s *pObj = (obj_s *) (idEvent - TIMER_EDIT_CARET);
    edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
    if ((pOwner->flags_ & eef_bCaretContext) == eef_bCaretContext) {
        pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & eef_bCaretContext);
        pOwner->flags_ = pOwner->flags_ | eef_bCaretShowed;
    } else {
        pOwner->flags_ = pOwner->flags_ | (eef_bShowCaret | eef_bCaretContext);
        pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & eef_bCaretShowed);
    }
    _obj_invalidaterect(pObj, pOwner + offsetof(edit_s, rcCaret_left_), 0);
}

void
_edit_txpaint(void *pits, DWORD dwDrawAspect, LONG lindex, void *pvAspect, void *ptd, HDC hdcDraw, HDC hicTargetDev,
              void *lprcBounds, void *lprcWBounds, void *lprcUpdate, DWORD dwContinue, LONG lViewId) {
    ((ITextServices *) pits)->TxDraw(dwDrawAspect, lindex, pvAspect, (DVTARGETDEVICE *) ptd, hdcDraw, hicTargetDev,
                                     (LPCRECTL) lprcBounds, (LPCRECTL) lprcWBounds, (LPRECT) lprcUpdate, NULL,
                                     dwContinue, lViewId);
}

size_t _edit_scrollmsg(EXHANDLE hObj, obj_s *pObj, int uMsg, size_t wParam, size_t lParam) {
    bool ret;
    _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
    return 0;
}

LRESULT _edit_sendmessage(obj_s *pObj, int uMsg, size_t wParam, size_t lParam, bool *sOK) {
    LRESULT ret = 0;
    void *pits = _edit_its(pObj);
    if (pits != nullptr) {
        *sOK = ((ITextServices *) pits)->TxSendMessage(uMsg, wParam, lParam, &ret) == 0;
    }
    return ret;
}

void *_edit_its(obj_s *pObj) {
    void *pOwner = _obj_pOwner(pObj);
    void *ret = nullptr;
    if (pOwner != nullptr) {
        ret = ((edit_s *) pOwner)->its_;
    }
    return ret;
}

void _edit_contextmenu(HWND hWnd, wnd_s *pWnd, EXHANDLE hObj, obj_s *pObj, size_t wParam, int x, int y) {
    if ((pObj->dwStyle_ & 编辑框风格_禁用右键默认菜单) == 编辑框风格_禁用右键默认菜单) {
        return;
    }
    if ((pObj->dwStyle_ & 编辑框风格_密码输入) == 编辑框风格_密码输入) {
        return;
    }
    if (_obj_setfocus(hWnd, pWnd, hObj, pObj, true)) {
        _obj_baseproc(hWnd, hObj, pObj, WM_COMMAND, EM_SETSEL, 0);
    }
    HMENU hMenu = GetSubMenu(g_Li.hMenuEdit, 0);
    bool sOK;
    LRESULT tmp = _edit_sendmessage(pObj, EM_CANUNDO, 0, 0, &sOK);//撤销
    EnableMenuItem(hMenu, 0, !tmp ? 1024 : 1026);
    SIZE sz;
    _edit_sendmessage(pObj, EM_EXGETSEL, 0, (size_t) &sz, &sOK);
    LONG index = sz.cy - sz.cx;
    index = index > 0 ? index : 0;
    EnableMenuItem(hMenu, 2, index != 0 ? 1024 : 1026);//剪切
    EnableMenuItem(hMenu, 3, index != 0 ? 1024 : 1026);//复制
    EnableMenuItem(hMenu, 5, index != 0 ? 1024 : 1026);//删除
    tmp = _edit_sendmessage(pObj, EM_CANPASTE, 0, 0, &sOK);//粘贴
    EnableMenuItem(hMenu, 4, tmp != 0 ? 1024 : 1026);//剪切
    EnableMenuItem(hMenu, 7, _edit_getlen(pObj) != 0 ? 1024 : 1026);//全选
    Ex_TrackPopupMenu(hMenu, 0, x, y, 0, hObj, 0, NULL, 0);
}

LRESULT _edit_getlen(obj_s *pObj) {
    SIZE sz;
    sz.cx = GTL_DEFAULT;
    sz.cy = CP_WINUNICODE;
    bool ret;
    return _edit_sendmessage(pObj, EM_GETTEXTLENGTHEX, (size_t) &sz, 0, &ret);
}

void _edit_command(obj_s *pObj, int uMsg, size_t wParam, size_t lParam) {
    bool sOK;
    if (wParam == WM_UNDO || wParam == WM_CUT || wParam == WM_COPY || wParam == WM_PASTE || wParam == WM_CLEAR) {
        uMsg = wParam;
        wParam = 0;

        _edit_sendmessage(pObj, uMsg, wParam, lParam, &sOK);
    } else {
        if (wParam == EM_SETSEL) {
            uMsg = EM_EXSETSEL;
            wParam = 0;
            void *lp = MemPool_Alloc(g_Li.hMemPoolMsg, true);
            __set_int(lp, sizeof(size_t), -1);
            _edit_sendmessage(pObj, uMsg, wParam, (size_t) lp, &sOK);
            MemPool_Free(g_Li.hMemPoolMsg, lp);
        }
    }
}

size_t _edit_paint(EXHANDLE hObj, obj_s *pObj) {
    paintstruct_s ps;
    if (Ex_ObjBeginPaint(hObj, &ps)) {
        int atom;
        void *pITS = (void *) __get(ps.dwOwnerData_, offsetof(edit_s, its_));
        if ((ps.dwState_ & 状态_焦点) != 0) {
            atom = ATOM_FOCUS;
        } else if ((ps.dwState_ & 状态_点燃) != 0) {
            atom = ATOM_HOVER;
        } else {
            atom = ATOM_NORMAL;
        }
        if ((ps.dwStyleEx_ & EOS_EX_CUSTOMDRAW) == 0) {
            Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, 0, 0, ps.width_, ps.height_, ATOM_EDIT, atom, 255);
        }
        void *lpBanner = (void *) __get(ps.dwOwnerData_, offsetof(edit_s, pBanner_));
        bool bDrawBanner = false;
        if (lpBanner != 0) {
            if (pITS == 0) {
                bDrawBanner = true;
            } else {
                if (_edit_getlen(pObj) == 0) {
                    bDrawBanner = true;
                }
            }
            if (bDrawBanner) {
                bDrawBanner = false;
                if (!((ps.dwState_ & 状态_焦点) != 0 && (ps.dwStyle_ & 编辑框风格_总是显示提示文本) == 0)) {
                    void *mDc = (void *) __get(ps.dwOwnerData_, offsetof(edit_s, prctext_));
                    int dt;
                    if ((pObj->dwTextFormat_ & DT_SINGLELINE) == DT_SINGLELINE) {
                        dt = DT_VCENTER;
                    }
                    _canvas_drawtext(ps.hCanvas_, pObj->hFont_, __get_int(ps.dwOwnerData_, offsetof(edit_s, crBanner_)),
                                     (LPCWSTR) lpBanner, -1, dt, __get_int(mDc, 0), __get_int(mDc, 4),
                                     __get_int(mDc, 8), __get_int(mDc, 12));
                    bDrawBanner = true;
                }
            }
        }

        if (pITS != 0) {
            RECT rcTmp;
            IntersectRect(&rcTmp, (RECT *) &ps.t_left_, (RECT *) &ps.p_left_);
            void *mDc = (void *) __get(ps.dwOwnerData_, offsetof(edit_s, mDc_));
            void *hDc = _canvas_getdc(ps.hCanvas_);
            if (hDc != 0) {
                wnd_s *pWnd = pObj->pWnd_;
                bool ismove = (pWnd->dwFlags_ & EWF_bSizeMoving) == EWF_bSizeMoving;
                _edit_txpaint(pITS, 1, 0, NULL, NULL, (HDC) hDc, NULL, NULL, NULL, &rcTmp, NULL, ismove ? -1 : 0);
                BitBlt((HDC) hDc, rcTmp.left, rcTmp.top, rcTmp.right - rcTmp.left, rcTmp.bottom - rcTmp.top, (HDC) mDc,
                       0, 0, SRCPAINT);
                _canvas_releasedc(ps.hCanvas_);
            }
            if (!((pObj->dwStyle_ & 编辑框风格_隐藏插入符) == 编辑框风格_隐藏插入符)) {
                if (!((((edit_s *) ps.dwOwnerData_)->flags_ & eef_bSelected) == eef_bSelected)) {
                    if ((((edit_s *) ps.dwOwnerData_)->flags_ & eef_bCaretContext) == eef_bCaretContext) {
                        if (!((((edit_s *) ps.dwOwnerData_)->flags_ & eef_bCaretShowed) == eef_bCaretShowed)) {
                            EXHANDLE sCanvas = ((edit_s *) ps.dwOwnerData_)->hCanvasCaret_;
                            rcTmp.left = ((edit_s *) ps.dwOwnerData_)->rcCaret_left_;
                            rcTmp.top = ((edit_s *) ps.dwOwnerData_)->rcCaret_top_;
                            rcTmp.right = ((edit_s *) ps.dwOwnerData_)->rcCaret_right_;
                            rcTmp.bottom = ((edit_s *) ps.dwOwnerData_)->rcCaret_bottom_;
                            if (rcTmp.right > 0 && rcTmp.bottom > 0) {
                                _canvas_alphablend(ps.hCanvas_, sCanvas, rcTmp.left, rcTmp.top, rcTmp.right,
                                                   rcTmp.bottom, 0, 0, rcTmp.right - rcTmp.left,
                                                   rcTmp.bottom - rcTmp.top, 255);
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

size_t _edit_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s *pObj) {
    bool bFree = false;
    if (uMsg == WM_CREATE) {
        _edit_init(hWnd, hObj, pObj);
    } else if (uMsg == WM_SETCURSOR) {
        edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
        POINT pt;
        pt.x = LOWORD(wParam);
        pt.y = HIWORD(wParam);
        if (!PtInRect((RECT *) pOwner->prctext_, pt)) {
            return 0;
        }
    } else if (uMsg == WM_DESTROY) {
        bool ret;
        _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
        _edit_unint(pObj);
    } else if (uMsg == WM_SIZE) {
        _edit_size(hWnd, hObj, pObj);
    } else if (uMsg == WM_PAINT) {
        return _edit_paint(hObj, pObj);
    } else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) {
        return _edit_scrollmsg(hObj, pObj, uMsg, wParam, lParam);
    } else if (uMsg == WM_MOUSEWHEEL) {
        uMsg = 0;
        if ((pObj->dwStyle_ & EOS_VSCROLL) == EOS_VSCROLL) {
            uMsg = WM_VSCROLL;
        } else if ((pObj->dwStyle_ & EOS_VSCROLL) == EOS_HSCROLL) {
            uMsg = WM_HSCROLL;
        }
        if (uMsg != 0) {
            _edit_scrollmsg(hObj, pObj, uMsg, wParam > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
            return 1;
        }
    } else if (uMsg == WM_MOUSEHOVER) {
        _obj_setuistate(pObj, 状态_点燃, false, 0, true, 0);
    } else if (uMsg == WM_MOUSELEAVE) {
        _obj_setuistate(pObj, 状态_点燃, true, 0, true, 0);
    } else if (uMsg == WM_SETFOCUS) {
        IME_Control(hWnd, pObj->pWnd_, !((pObj->dwStyle_ & 编辑框风格_密码输入) == 编辑框风格_密码输入));
        ((ITextServices *) _edit_its(pObj))->OnTxUIActivate();
        bool ret;
        _edit_sendmessage(pObj, uMsg, 0, 0, &ret);
        _obj_setuistate(pObj, 状态_焦点, false, 0, true, 0);
        if (!((pObj->dwStyle_ & 编辑框风格_隐藏插入符) == 编辑框风格_隐藏插入符)) {
            SetTimer(hWnd, (size_t) pObj + TIMER_EDIT_CARET, 500, &_edit_timer_caret);
        }
    } else if (uMsg == WM_KILLFOCUS) {
        if (!((pObj->dwStyle_ & 编辑框风格_隐藏插入符) == 编辑框风格_隐藏插入符)) {
            KillTimer(hWnd, (size_t) pObj + TIMER_EDIT_CARET);
            edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
            if ((pOwner->flags_ & (eef_bShowCaret | eef_bCaretContext)) == (eef_bShowCaret | eef_bCaretContext)) {
                pOwner->flags_ = pOwner->flags_ - (pOwner->flags_ & (eef_bShowCaret | eef_bCaretContext));
                _obj_invalidaterect(pObj, (void *) ((size_t) pOwner + offsetof(edit_s, rcCaret_left_)), 0);
            }
        }
        bool ret;
        _edit_sendmessage(pObj, uMsg, 0, 0, &ret);
        ((ITextServices *) _edit_its(pObj))->OnTxUIDeactivate();
        _obj_setuistate(pObj, 状态_焦点, true, 0, true, 0);
        DestroyCaret();
        IME_Control(hWnd, pObj->pWnd_, false);
    } else if (uMsg == WM_CHAR) {
        if ((pObj->dwStyle_ & 编辑框风格_数值输入) == 编辑框风格_数值输入) {
            if (!(wParam > 44 && wParam < 58 && wParam != 47)) {
                return 1;
            }
        }
        if (wParam == VK_TAB) {
            if (!((pObj->dwStyle_ & 编辑框风格_允许TAB字符) == 编辑框风格_允许TAB字符)) {
                return 1;
            }
        }
        bool ret;
        _edit_sendmessage(pObj, uMsg, wParam, lParam, &ret);
    } else if (uMsg == WM_CONTEXTMENU) {
        _edit_contextmenu(hWnd, pObj->pWnd_, hObj, pObj, wParam, LOWORD(lParam), HIWORD(lParam));
    } else if (uMsg == WM_COMMAND) {
        _edit_command(pObj, uMsg, wParam, lParam);
    } else if (uMsg == WM_SETTEXT) {
        return ((ITextServices *) _edit_its(pObj))->TxSetText((LPCWSTR) lParam);
    } else if (uMsg == EM_SETCUEBANNER) {
        edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
        pOwner->crBanner_ = wParam;
        pOwner->pBanner_ = copytstr((LPCWSTR) lParam, lstrlenW((LPCWSTR) lParam));
        _obj_invalidaterect(pObj, 0, 0);
        return 0;
    } else if (uMsg == WM_SYSCOLORCHANGE) {
        edit_s *pOwner = (edit_s *) _obj_pOwner(pObj);
        if (wParam == COLOR_EX_TEXT_NORMAL) {
            CHARFORMATW *pcf = (CHARFORMATW *) pOwner->pcf_;
            pcf->dwMask = CFM_COLOR;
            pcf->crTextColor = ExARGB2RGB(lParam);
            ((ITextServices *) _edit_its(pObj))->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,
                                                                        TXTBIT_CHARFORMATCHANGE);
        } else if (wParam == COLOR_EX_EDIT_CARET) {
            pOwner->crCaret_ = lParam;
        } else if (wParam == COLOR_EX_EDIT_BANNER) {
            pOwner->crBanner_ = lParam;
        }
        return 0;
    } else if (uMsg == WM_NCCREATE || uMsg == WM_NCCALCSIZE) {
        //拦截这两个消息
    } else {
        LRESULT ret = _edit_sendmessage(pObj, uMsg, wParam, lParam, &bFree);
        if (bFree) {
            return ret;
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}