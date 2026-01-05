#include "stdafx.h"

void _object_init()
{
    _sysbutton_register();
    _page_register();
    _static_register();
    _button_regsiter();
    _item_regsiter();
    _scrollbar_register();
    _edit_register();
    _listview_regsiter();
    _combobox_regsiter();
    _groupbox_regsiter();
    _reportlistview_regsiter();
    _iconlistview_register();
    _treeview_register();
    _navbtn_register();
    _buttonex_register();
    _editex_register();
    _checkbuttonex_register();
    _radiobuttonex_register();
    _checkbox_register();
    _switch_register();
    _mosaic_loading_register();
    _win10_loading_register();
    _sliderbar_register();
    _rotateimagebox_register();
    _progressbar_register();
    _listbuttonex_register();
    _datebox_register();
    _titlebar_register();
    _color_picker_register();
    _calendar_register();
    _cefbrowser_register();
    _scorebtn_register();
    _carousel_register();
    _drawingboard_register();
    _palette_register();
    _propertygrid_register();
    _rollmenu_register();
    _taggingboard_register();
    _circleprogressbar_register();
    _waveprogressbar_register();
    _linechart_register();
    _chatbox_register();
    _flowchart_register();
    _grid_register();
    _flowscrollview_register();
    _splitter_register();
    _prototype_board_register();

#ifdef WEB_VIEW2
    _webview_register();
#endif
#ifdef VCL_PLAYER
    _vlcplayer_register();
#endif
    
}


EXATOM Ex_ObjRegister(LPCWSTR lptszClassName, INT dwStyle, INT dwStyleEx, INT dwTextFormat,
                      DWORD cbObjExtra, HCURSOR hCursor, INT dwFlags, ClsPROC pfnObjProc)
{
    EXATOM atom = Ex_Atom(lptszClassName);
    if (atom == 0) {
        return 0;
    }
    INT nError = 0;
    _obj_register(atom, dwStyle, dwStyleEx, dwTextFormat, cbObjExtra, hCursor, pfnObjProc, dwFlags,
                  &nError);
    Ex_SetLastError(nError);
    return atom;
}

void _obj_register(EXATOM atomClass, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra,
                   HCURSOR hCursor, ClsPROC pfnClsProc, DWORD dwFlags, INT* nError)
{
    EX_CLASSINFO* pCls;
    size_t        ret = 0;
    HashTable_Get(g_Li.hTableClass, atomClass, &ret);
    if (ret == 0) {
        pCls = (EX_CLASSINFO*)Ex_MemAlloc(sizeof(EX_CLASSINFO));
        if (pCls == 0) {
            *nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            pCls->dwFlags      = dwFlags;
            pCls->dwStyle      = dwStyle;
            pCls->dwStyleEx    = dwStyleEx;
            pCls->dwTextFormat = dwTextFormat;
            pCls->cbObjExtra   = cbObjExtra;
            pCls->hCursor      = hCursor;
            pCls->pfnClsProc   = pfnClsProc;
            pCls->atomName     = atomClass;
            HashTable_Set(g_Li.hTableClass, atomClass, (size_t)pCls);
        }
    }
}

BOOL Ex_ObjLayoutSet(EXHANDLE handle, HEXLAYOUT hLayout, BOOL fUpdate)
{
    HWND   hWnd = 0;
    wnd_s* pWnd = nullptr;
    obj_s* pObj = nullptr;
    if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj)) {
        HEXLAYOUT hOld = pObj->base.hLayout_;
        pObj->base.hLayout_ = hLayout;
        if (hOld != hLayout) {
            _layout_destroy(hOld);
        }
        if (fUpdate) {
            SendMessageW(hWnd, g_Li.dwMessage, hLayout, MAKELONG(EMT_LAYOUT_UPDATE, 0));
        }
    }
    return pObj != nullptr;
}

HEXLAYOUT Ex_ObjLayoutGet(EXHANDLE handle)
{
    obj_s*    pObj    = nullptr;
    HWND      hWnd    = 0;
    wnd_s*    pWnd    = nullptr;
    HEXLAYOUT hLayout = 0;
    if (_wnd_getfromhandle(handle, &hWnd, &pWnd, &pObj)) {
        hLayout = pObj->base.hLayout_;
    }
    return hLayout;
}

BOOL Ex_ObjLayoutUpdate(EXHANDLE handle)
{
    HWND   hWnd = 0;
    obj_s* pObj = nullptr;
    if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj)) {
        SendMessageW(hWnd, g_Li.dwMessage, pObj->base.hLayout_, MAKELONG(EMT_LAYOUT_UPDATE, 0));
    }
    return pObj != nullptr;
}

BOOL Ex_ObjLayoutClear(EXHANDLE handle, BOOL bChildren)
{
    HWND   hWnd = 0;
    wnd_s* pWnd = 0;
    if (_wnd_getfromhandle(handle, NULL, &pWnd)) {
        HEXLAYOUT hLayout = pWnd->base.hLayout_;
        _layout_destroy(hLayout);
        pWnd->base.hLayout_ = 0;
    }
    return pWnd != 0;
}

void _obj_z_clear(HEXOBJ hObj, obj_s* pObj, EXHANDLE* hParent, obj_base** pParent)
{
    // 逆序,顶部->底部
    if (hParent) {
        *hParent = pObj->objParent_;
    }
    INT nError = 0;

    if (!_handle_validate(pObj->objParent_, HT_OBJECT, (LPVOID*)pParent, &nError)) {
        if (hParent) {
            *hParent = 0;
        }
        if (pParent) {
            *pParent = (obj_base*)pObj->pWnd_;
        }
    }

    // 恢复并脱离原链表
    HEXOBJ objPrev = pObj->objPrev_;
    HEXOBJ objNext = pObj->objNext_;
    // 修复父层组件链表
    if (pParent) {
        if ((*pParent)->objChildFirst_ == hObj) {
            (*pParent)->objChildFirst_ = objNext;
        }
        if ((*pParent)->objChildLast_ == hObj) {
            (*pParent)->objChildLast_ = objPrev;
        }
    }

    // 修复兄弟层组件链表
    obj_s* pNext = nullptr;
    obj_s* pPrev = nullptr;
    if (_handle_validate(objNext, HT_OBJECT, (LPVOID*)&pNext, &nError)) {
        pNext->objPrev_ = objPrev;
    }
    if (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pPrev, &nError)) {
        pPrev->objNext_ = objNext;
    }
    pObj->objNext_ = 0;
    pObj->objPrev_ = 0;
}

void _obj_z_set_before_topmost(EXHANDLE objChildFirst, LPVOID pObjChildFirst, EXHANDLE objChildLast,
                               obj_s* pObjChildLast, EXHANDLE hObj, obj_s* pObj, obj_base* pParent)
{
    INT nError = 0;
    if ((pObjChildLast->dwStyleEx_ & OBJECT_STYLE_EX_TOPMOST) ==
        OBJECT_STYLE_EX_TOPMOST)   // 检查有没置顶组件
    {
        EXHANDLE objPrev = pObjChildLast->objPrev_;
        if (objPrev == 0)   // 没有置顶组件
        {
            pParent->objChildFirst_ = hObj;
            pObjChildLast->objPrev_ = hObj;
            pObj->objNext_          = objChildLast;
        }
        else {
            obj_s* pObjPrev = nullptr;
            HEXOBJ tmp      = 0;
            obj_s* pTmp     = nullptr;
            while (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pObjPrev, &nError)) {
                if ((pObjPrev->dwStyleEx_ & OBJECT_STYLE_EX_TOPMOST) == OBJECT_STYLE_EX_TOPMOST) {
                    tmp = pObjPrev->objPrev_;
                    if (tmp != 0) {
                        objPrev = tmp;
                        continue;
                    }
                    else {
                        pObjPrev->objPrev_      = hObj;
                        pObj->objNext_          = objPrev;
                        pParent->objChildFirst_ = hObj;
                    }
                }
                else {
                    tmp                = pObjPrev->objNext_;
                    pObjPrev->objNext_ = hObj;
                    pObj->objNext_     = tmp;
                    pObj->objPrev_     = objPrev;
                    if (_handle_validate(tmp, HT_OBJECT, (LPVOID*)&pTmp, &nError)) {
                        pTmp->objPrev_ = hObj;
                    }
                    else {
                        pParent->objChildFirst_ = hObj;
                    }
                }
                break;
            }
        }
    }
    else {
        pObjChildLast->objNext_ = hObj;
        pObj->objPrev_          = objChildLast;
        pParent->objChildLast_  = hObj;
    }
}
void _obj_z_set(HEXOBJ hObj, obj_s* pObj, EXHANDLE hObjInsertAfter, UINT flags, INT* nError)
{
    EXHANDLE  hParent        = 0;
    obj_base* pParent        = nullptr;
    obj_s*    pObjChildLast  = nullptr;
    obj_s*    pObjChildFirst = nullptr;
    _obj_z_clear(hObj, pObj, &hParent, &pParent);
    pObj->objParent_ = hParent;

    EXHANDLE objChildFirst = 0;
    EXHANDLE objChildLast  = 0;

    objChildFirst = pParent->objChildFirst_;
    objChildLast  = pParent->objChildLast_;

    if (objChildLast == 0 || objChildFirst == 0) {
        pParent->objChildFirst_ = hObj;
        pParent->objChildLast_  = hObj;
    }
    else {
        if (_handle_validate(objChildLast, HT_OBJECT, (LPVOID*)&pObjChildLast, nError)) {
            if (_handle_validate(objChildFirst, HT_OBJECT, (LPVOID*)&pObjChildFirst, nError)) {
                obj_s* pObjInsertAfter = nullptr;
                BOOL bTopmost =
                    (pObj->dwStyleEx_ & OBJECT_STYLE_EX_TOPMOST) == OBJECT_STYLE_EX_TOPMOST;
                if (hObjInsertAfter == (size_t)HWND_NOTOPMOST)   // 取消置顶
                {
                    pObj->dwStyleEx_ =
                        pObj->dwStyleEx_ - (pObj->dwStyleEx_ & OBJECT_STYLE_EX_TOPMOST);
                    _obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast,
                                              pObjChildLast, hObj, pObj, pParent);
                }
                else if (hObjInsertAfter == (size_t)HWND_TOPMOST)   // 置顶
                {
                    pObj->dwStyleEx_        = pObj->dwStyleEx_ | OBJECT_STYLE_EX_TOPMOST;
                    pObjChildLast->objNext_ = hObj;
                    pObj->objPrev_          = objChildLast;
                    pParent->objChildLast_  = hObj;
                }
                else if (hObjInsertAfter == (size_t)HWND_TOP)   // 顶层
                {
                    if (bTopmost) {
                        pObjChildLast->objNext_ = hObj;
                        pObj->objPrev_          = objChildLast;
                        pParent->objChildLast_  = hObj;
                    }
                    else {
                        _obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast,
                                                  pObjChildLast, hObj, pObj, pParent);
                    }
                }
                else if(hObjInsertAfter == (size_t)HWND_BOTTOM){
                    //底层
                INSERTBOTTOM:
                    if (bTopmost) {
                        _obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast,
                                                  pObjChildLast, hObj, pObj, pParent);
                    }
                    else {
                        pObjChildFirst->objPrev_ = hObj;
                        pObj->objNext_           = objChildFirst;
                        pParent->objChildFirst_  = hObj;
                    }
                }
                else if (_handle_validate(hObjInsertAfter, HT_OBJECT, (LPVOID*)&pObjInsertAfter, 0))// 插入
                {
                    //如果hObjInsertAfter具有TOPMOST属性, 那么hObj位于所有非TOPMOST窗口之上
                    if (bTopmost)
                    {
                        pObjChildLast->objNext_ = hObj;
                        pObj->objPrev_ = objChildLast;
                        pParent->objChildLast_ = hObj;
                    }
                    else if ((pObjInsertAfter->dwStyleEx_ & OBJECT_STYLE_EX_TOPMOST) == OBJECT_STYLE_EX_TOPMOST)
                    {
                        _obj_z_set_before_topmost(objChildFirst, pObjChildFirst, objChildLast, pObjChildLast, hObj, pObj, pParent);
                    }
                    else //插入在 hObjInsertAfter 下层(之前)  
                    {
                        pObj->objPrev_ = pObjInsertAfter->objPrev_;
                        pObj->objNext_ = hObjInsertAfter;
                        obj_s* pObjPrev = nullptr;
                        if (pObjInsertAfter->objPrev_ && _handle_validate(pObjInsertAfter->objPrev_, HT_OBJECT, (LPVOID*)&pObjPrev, 0))
                            pObjPrev->objNext_ = hObj;
                        else
                            pParent->objChildFirst_ = hObj;
                        pObjInsertAfter->objPrev_ = hObj;
                    }
                }
                else
                {
                    //其他值 
                    goto INSERTBOTTOM;
                }
            }
        }
    }
}

BOOL _obj_autosize(obj_s* pObj, HEXOBJ hObj, INT* width, INT* height)
{
    INT  nError = 0;
    BOOL ret    = FALSE;
    if ((pObj->dwStyleEx_ & OBJECT_STYLE_EX_AUTOSIZE) == OBJECT_STYLE_EX_AUTOSIZE &&
        (pObj->base.dwFlags_ & EOF_BAUTOSIZED) != EOF_BAUTOSIZED) {
        pObj->base.dwFlags_     = pObj->base.dwFlags_ | EOF_BAUTOSIZED;
        EXHANDLE parentObj = pObj->objParent_;
        wnd_s*   pWnd      = nullptr;
        INT      iWidth    = 0;
        INT      iHeight   = 0;
        obj_s*   ppObj     = nullptr;
        if (parentObj == 0) {
            pWnd    = pObj->pWnd_;
            iWidth  = pWnd->width_;
            iHeight = pWnd->height_;
        }
        else {
            if (_handle_validate(parentObj, HT_OBJECT, (LPVOID*)&ppObj, &nError)) {
                iWidth  = ppObj->right_ - ppObj->left_;
                iHeight = ppObj->bottom_ - ppObj->top_;
            }
        }
        iWidth  = iWidth - pObj->left_;
        iHeight = iHeight - pObj->top_;
        if (iWidth < 0) {
            iWidth = 0;
        }
        if (iHeight < 0) {
            iHeight = 0;
        }

        LPCWSTR ptitle = pObj->pstrTitle_;
        FLOAT   w      = 0;
        FLOAT   h      = 0;
        if (ptitle != 0) {
            _canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, ptitle, -1, pObj->dwTextFormat_,
                                 iWidth, iHeight, &w, &h);
        }
        *width  = pObj->t_left_ + pObj->t_right_ + w + Ex_Scale(2);
        *height = pObj->t_top_ + pObj->t_bottom_ + h + Ex_Scale(2);
        ret     = TRUE;
    }
    return ret;
}

size_t _obj_sendmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                        INT dwReserved)
{
    mempoolmsg_s* p   = (mempoolmsg_s*)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
    size_t        ret = 0;
    if (p != 0) {
        p->pObj       = pObj;
        p->uMsg       = uMsg;
        p->wParam     = wParam;
        p->lParam     = lParam;
        p->dwReserved = dwReserved;
        ret           = SendMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
    }
    return ret;
}

BOOL _obj_postmessage(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                      INT dwReserved)
{
    mempoolmsg_s* p   = (mempoolmsg_s*)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
    BOOL          ret = FALSE;
    if (p != 0) {
        p->pObj       = pObj;
        p->uMsg       = uMsg;
        p->wParam     = wParam;
        p->lParam     = lParam;
        p->dwReserved = dwReserved;
        ret           = PostMessageW(hWnd, g_Li.dwMessage, (WPARAM)p, MAKELONG(EMT_OBJECT, 0));
    }
    return ret;
}

INT _obj_wm_nchittest(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT ret = HTTRANSPARENT;
    if (!((pObj->dwStyleEx_ & OBJECT_STYLE_EX_TRANSPARENT) == OBJECT_STYLE_EX_TRANSPARENT)) {
        BOOL fHit = FALSE;

        if (((pObj->base.dwFlags_ & EOF_BPATH) == EOF_BPATH)) {
            fHit = _path_hittest(pObj->hPath_Client_, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        else {
            POINT aa = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            fHit     = PtInRect((RECT*)((size_t)pObj + offsetof(obj_s, c_left_)), aa);
        }
        if (fHit) {
            ret = pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam);
        }
    }
    return ret;
}

void _obj_killfocus(HEXOBJ hObj, obj_s* pObj, BOOL bDispatch)
{
    wnd_s* pWnd     = pObj->pWnd_;
    HWND   hWnd     = pWnd->hWnd_;
    size_t objFocus = pWnd->objFocus_;
    if (objFocus == hObj) {
        if (bDispatch) {
            _obj_baseproc(hWnd, hObj, pObj, WM_KILLFOCUS, 0, 0);
        }
        else {
            _obj_sendmessage(hWnd, hObj, pObj, WM_KILLFOCUS, 0, 0, 0);
        }
    }
}

BOOL Ex_ObjKillFocus(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_killfocus(hObj, pObj, FALSE);
    }
    return nError == 0;
}

HWND _obj_gethwnd(obj_s* pObj)
{
    wnd_s* pWnd = pObj->pWnd_;
    return pWnd->hWnd_;
}

BOOL _obj_setfocus(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, BOOL bDispatch)
{
    BOOL ret = FALSE;

    if (((pObj->dwState_ & STATE_ALLOWFOCUS) == STATE_ALLOWFOCUS)) {
        size_t objFocus = pWnd->objFocus_;
        if (bDispatch) {
            _obj_baseproc(hWnd, hObj, pObj, WM_SETFOCUS, objFocus, 0);
        }
        else {
            _obj_sendmessage(hWnd, hObj, pObj, WM_SETFOCUS, objFocus, 0, 0);
        }
        ret = TRUE;
    }
    return ret;
}

void _obj_setfocus_real(HWND hWnd, obj_s* pObj, HEXOBJ hObj, HEXOBJ lstObj)
{
    if (hObj != lstObj) {
        INT    nError = 0;
        obj_s* pLast = nullptr;
        if (_handle_validate(lstObj, HT_OBJECT, (LPVOID*)&pLast, &nError)) {
            _obj_baseproc(hWnd, lstObj, pLast, WM_KILLFOCUS, hObj, 0);
        }
        pObj->pWnd_->objFocus_ = hObj;
        pObj->dwState_ = pObj->dwState_ | STATE_FOCUS;
    }
}

BOOL Ex_ObjSetFocus(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_setfocus(_obj_gethwnd(pObj), pObj->pWnd_, hObj, pObj, FALSE);
    }
    return nError == 0;
}

LRESULT _obj_baseproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    pObj->base.dwFlags_ = pObj->base.dwFlags_ | EOF_BUSERPROCESSESED;
    if (pObj->pfnSubClass_ != 0) {
        LRESULT ret = 0;
        if ((pObj->pfnSubClass_)(hWnd, hObj, uMsg, wParam, lParam, &ret)) {
            return ret;
        }
        else if (!_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0)) //pObj->pfnSubClass_回调里可能组件已经被销毁了
        {
            return 0;
        }
    }
    if (uMsg == WM_NCHITTEST) {
        return _obj_wm_nchittest(hWnd, hObj, pObj, uMsg, wParam, lParam);
    }
    else if (uMsg == WM_SIZE)   // 布局更新
    {
        HEXLAYOUT hLayout = pObj->base.hLayout_;
        if (hLayout != 0) {
            _layout_update(hLayout);
        }
    }
    else if (uMsg == WM_SHOWWINDOW) {
        _obj_visable(hWnd, hObj, pObj, wParam != 0);
    }
    else if (uMsg == WM_ENABLE) {
        _obj_disable(hWnd, hObj, pObj, wParam == 0);
    }
    else if (uMsg == WM_SETREDRAW) {
        if (wParam == 0) {
            FLAGS_DEL(pObj->base.dwFlags_, EOF_BCANREDRAW);
        }
        else {
            FLAGS_ADD(pObj->base.dwFlags_, EOF_BCANREDRAW);
        }
    }
    else if (uMsg == WM_SETFOCUS) {
        _obj_setfocus_real(hWnd, pObj, hObj, (HEXOBJ)wParam);
        if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BIME)) {
            IME_Control(hWnd,  TRUE);
        }
    }
    else if (uMsg == WM_KILLFOCUS) {
        _wnd_popupclose(pObj->pWnd_, hWnd, 0, 0);
        _obj_killfocus_real(pObj, hObj, (HEXOBJ)wParam);
        if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BIME)) {
            IME_Control(hWnd, FALSE);
        }
    }
    return pObj->pfnClsProc_(hWnd, hObj, uMsg, wParam, lParam);
}

void _obj_killfocus_real(obj_s* pObj, HEXOBJ hObj, HEXOBJ objFocus)
{
    wnd_s* pWnd = pObj->pWnd_;
    if (pWnd->objFocus_ == hObj) {
        pWnd->objFocus_ = objFocus;
        pObj->dwState_  = pObj->dwState_ - (pObj->dwState_ & STATE_FOCUS);
        if (pWnd->objTrack_ == hObj) {
            ReleaseCapture();
        }
    }
}

void _obj_setchildrenpostion(obj_s* pObj, INT x, INT y)
{
    HEXOBJ objChild  = pObj->base.objChildFirst_;
    INT    nError    = 0;
    obj_s* pchildObj = nullptr;
    while (_handle_validate(objChild, HT_OBJECT, (LPVOID*)&pchildObj, &nError)) {
        pchildObj->w_left_   = pchildObj->left_;
        pchildObj->w_top_    = pchildObj->top_;
        pchildObj->w_right_  = pchildObj->right_;
        pchildObj->w_bottom_ = pchildObj->bottom_;
        OffsetRect((RECT*)((size_t)pchildObj + offsetof(obj_s, w_left_)), x, y);
        if (FLAGS_CHECK(pchildObj->base.dwFlags_, EOF_BPATHBYROUNDEDRECT)) {
            _obj_reset_path(pchildObj, pchildObj->w_left_, pchildObj->w_top_, pchildObj->w_right_,
                            pchildObj->w_bottom_, offsetof(obj_s, hPath_Window_));
        }
        _obj_setchildrenpostion(pchildObj, pchildObj->w_left_, pchildObj->w_top_);
        objChild = __get_int(pchildObj, offsetof(obj_s, objNext_));
    }
}

void _obj_update(obj_s* pObj, BOOL fUpdateWindow)
{
    RECT prc{ 0 };
    RtlMoveMemory(&prc, &pObj->left_, sizeof(RECT));
    //偏移到组件自身坐标系
    OffsetRect(&prc, -pObj->left_, -pObj->top_);
    //再偏移到窗口坐标系
    OffsetRect(&prc, pObj->w_left_, pObj->w_top_);
    wnd_s* pWnd = pObj->pWnd_;
    HWND   hWnd = pWnd->hWnd_;

    FLAGS_ADD(
        pObj->base.dwFlags_,
        EOF_BNEEDREDRAW);   // 这里加上是为了强制重画。最新版在_wnd_render_obj绕过了eof_bNeedRedraw检测。有可能是测试bug时不小心改了。
    if (_obj_makeupinvalidaterect(pWnd, pObj, &prc)) {
        LPRECT RCP = &pWnd->ulwi_prcDirty;
        UnionRect(RCP, RCP, &prc);
        InvalidateRect(hWnd, &prc, FALSE);
        if (fUpdateWindow) {
            UpdateWindow(hWnd);
        }
    }
}

void _obj_updatewindowpostion(obj_s* pObj, LPVOID lpWRC, BOOL fChild)
{
    RtlMoveMemory(lpWRC, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
    EXHANDLE parentObj = pObj->objParent_;
    INT      nError    = 0;
    obj_s*   ppObj     = nullptr;
    while (_handle_validate(parentObj, HT_OBJECT, (LPVOID*)&ppObj, &nError)) {
        OffsetRect((RECT*)lpWRC, ppObj->left_, ppObj->top_);
        parentObj = ppObj->objParent_;
    }
    if (fChild) {
        _obj_setchildrenpostion(pObj, pObj->w_left_, pObj->w_top_);
    }
}

size_t Ex_ObjSendMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    size_t ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _obj_sendmessage(_obj_gethwnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
    }
    else if (_handle_validate(hObj, HT_DUI, (LPVOID*)&pObj, 0)) //引擎则直接发送窗口消息
    {
        return SendMessageW(((wnd_s*)pObj)->hWnd_, uMsg, wParam, lParam);
    }
    return ret;
}

BOOL Ex_ObjPostMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _obj_postmessage(_obj_gethwnd(pObj), hObj, pObj, uMsg, wParam, lParam, 0);
    }
    else if (_handle_validate(hObj, HT_DUI, (LPVOID*)&pObj, 0)) //引擎则直接投递窗口消息
    {
        return PostMessageW(((wnd_s*)pObj)->hWnd_, uMsg, wParam, lParam);
    }
    return ret;
}

BOOL Ex_ObjDestroy(HEXOBJ hObj)
{
    return Ex_ObjSendMessage(hObj, WM_DESTROY, 0, 0);
}

BOOL _obj_z_compositedcheck(LPVOID prc, EXHANDLE objLast, EXHANDLE objStop, LPVOID lpsrcInsert)
{
    EXHANDLE objPrev = objLast;
    INT      nError  = 0;
    obj_s*   pObj    = nullptr;
    while (_handle_validate(objPrev, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (objStop == objPrev) {
            return TRUE;
        }
        objLast = pObj->base.objChildLast_;
        if (objLast != 0) {
            if (_obj_z_compositedcheck(prc, objLast, objStop, lpsrcInsert)) {
                return TRUE;
            }
        }

        if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) 
        {
            if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_COMPOSITED) == OBJECT_STYLE_EX_COMPOSITED)) {
                if (IntersectRect((LPRECT)lpsrcInsert, (RECT*)prc,
                                  (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)))) {
                    pObj->base.dwFlags_  = pObj->base.dwFlags_ | EOF_BNEEDREDRAW;
                    UnionRect((LPRECT)prc, (RECT*)prc,
                              (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)));
                }
            }
        }
        objPrev = pObj->objPrev_;
    }
    return FALSE;
}

void _obj_compostied_all(HEXOBJ objEntry)
{
    HEXOBJ objNext = objEntry;
    INT    nError  = 0;
    obj_s* pObj    = nullptr;
    HEXOBJ sObj    = 0;
    while (_handle_validate(objNext, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_COMPOSITED) == OBJECT_STYLE_EX_COMPOSITED)) {
            if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
                pObj->base.dwFlags_  = pObj->base.dwFlags_ | EOF_BNEEDREDRAW;
            }
        }
        sObj = pObj->base.objChildFirst_;
        if (sObj != 0) {
            _obj_compostied_all(sObj);
        }
        objNext = pObj->objNext_;
    }
}

BOOL Ex_ObjSetRedraw(HEXOBJ hObj, BOOL fCanbeRedraw)
{
    return Ex_ObjSendMessage(hObj, WM_SETREDRAW, fCanbeRedraw, 0);
}

BOOL Ex_ObjGetRect(HEXOBJ hObj, RECT* lpRect)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (IsBadWritePtr(lpRect, 16)) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjGetClientRect(HEXOBJ hObj, RECT* lpRect)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    wnd_s* pWnd_ = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (IsBadWritePtr(lpRect, 16)) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
        }
    }
    else if (_handle_validate(hObj, HT_DUI, (LPVOID*)&pWnd_, &nError))
    {
        lpRect->left = 0;
        lpRect->top = 0;
        lpRect->right = pWnd_->width_;
        lpRect->bottom = pWnd_->height_;
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjGetTextRect(HEXOBJ hObj, RECT* lpRect)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (IsBadWritePtr(lpRect, 16)) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            lpRect->left   = pObj->t_left_;
            lpRect->top    = pObj->t_top_;
            lpRect->right  = pObj->right_ - pObj->left_ - pObj->t_right_;
            lpRect->bottom = pObj->bottom_ - pObj->top_ - pObj->t_bottom_;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL _obj_makeupinvalidaterect(wnd_s* pWnd, obj_s* pObj, LPVOID prc)
{
    if (((pWnd->base.dwFlags_ & EWF_SIZED) == EWF_SIZED)) return FALSE;
    // 更新最终混合的窗口位置
    EXHANDLE objParent = pObj->objParent_;
    INT      nError    = 0;
    obj_s*   ppObj     = nullptr;
    while (_handle_validate(objParent, HT_OBJECT, (LPVOID*)&ppObj, &nError)) {
        if (!IntersectRect((LPRECT)prc, (RECT*)prc,
                           (RECT*)((size_t)ppObj + offsetof(obj_s, w_left_)))) {
            return FALSE;
        }
        objParent = ppObj->objParent_;
    }
    // Z序混合检测-blur

    if (((pWnd->base.dwFlags_ & EWF_BCOMPOSITEDCHECK) == EWF_BCOMPOSITEDCHECK)) {
        RECT ppObj1{ 0 };
        _obj_z_compositedcheck(prc, pWnd->base.objChildLast_, pObj->base.hObj_, &ppObj1);
    }
    return TRUE;
}

void _obj_invalidaterect(obj_s* pObj, RECT* lpRect, INT* nError)
{
    pObj->base.dwFlags_ = pObj->base.dwFlags_ | EOF_BNEEDREDRAW;
    RECT RC{ 0 };
    // 混合型组件需要全部刷新,防止背景状态不同步。

    if (lpRect == 0 ||
        ((pObj->dwStyleEx_ & OBJECT_STYLE_EX_COMPOSITED) == OBJECT_STYLE_EX_COMPOSITED)) {
        RtlMoveMemory(&RC, (RECT*)((size_t)pObj + offsetof(obj_s, left_)), 16);
        OffsetRect(&RC, (-pObj->left_), (-pObj->top_));
    }
    else {
        if (IsRectEmpty((RECT*)((size_t)pObj + offsetof(obj_s, left_))))// d_
        {
            RtlMoveMemory(&RC, lpRect, 16);
        }
        else
        {
            RtlMoveMemory(&RC, (RECT*)((size_t)pObj + offsetof(obj_s, left_)), 16);
            OffsetRect(&RC, (-pObj->left_), (-pObj->top_));
            UnionRect(&RC, &RC, lpRect);
        }
        if (IsRectEmpty(&RC))
            return;
    }
    OffsetRect((LPRECT)&RC, pObj->w_left_, pObj->w_top_);
    if (_obj_makeupinvalidaterect(pObj->pWnd_, pObj, &RC))
    {
        LPRECT RCP = &pObj->pWnd_->ulwi_prcDirty;
        UnionRect(RCP, RCP, &RC);
    }
    InvalidateRect(pObj->pWnd_->hWnd_, (RECT*)&RC, FALSE);
}

BOOL Ex_ObjInvalidateRect(HEXOBJ hObj, RECT* lprcRedraw)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_invalidaterect(pObj, lprcRedraw, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjUpdate(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_update(pObj, TRUE);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjSetPadding(HEXOBJ hObj, INT nPaddingType, INT left, INT top, INT right, INT bottom,
                      BOOL fRedraw)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pObj->t_left_   = left;
        pObj->t_top_    = top;
        pObj->t_right_  = right;
        pObj->t_bottom_ = bottom;
        if (g_Li.DpiX > 1) {
            pObj->t_left_   = pObj->t_left_ * g_Li.DpiX;
            pObj->t_top_    = pObj->t_top_ * g_Li.DpiX;
            pObj->t_right_  = pObj->t_right_ * g_Li.DpiX;
            pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiX;
        }
        if (FLAGS_CHECK(pObj->dwStyleEx_, OBJECT_STYLE_EX_AUTOSIZE)) {
            FLAGS_DEL(pObj->base.dwFlags_, EOF_BAUTOSIZED);
            INT nError = 0;
            _obj_setpos_org(pObj, pObj->base.hObj_, NULL, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER,
                            &nError);
        }
        if (fRedraw) {
            nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjClientToWindow(HEXOBJ hObj, INT* x, INT* y)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

        RECT rc = { 0 };
        RtlMoveMemory(&rc, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
        OffsetRect(&rc, pObj->w_left_, pObj->w_top_);
        if (x)
        {
            *x += rc.left;
        }
        if (y)
        {
            *y += rc.top;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjClientToScreen(HEXOBJ hObj, INT* x, INT* y)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        RECT rc = { 0 };
        wnd_s* pWnd = pObj->pWnd_;
        RtlMoveMemory(&rc, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
        RECT RC = { 0,0 };
        GetWindowRect(pWnd->hWnd_, &RC);
        OffsetRect(&rc, pObj->w_left_ + RC.left, pObj->w_top_ + RC.top);
        if (x)
        {
            *x += rc.left;
        }
        if (y)
        {
            *y += rc.top;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjEnable(HEXOBJ hObj, BOOL bEnable)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (bEnable != (!((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED))) {
            ret = Ex_ObjSendMessage(hObj, WM_ENABLE, bEnable ? 1 : 0, 0);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjIsEnable(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {

        ret = !((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED);
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjIsVisible(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = (pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE;
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjShow(HEXOBJ hObj, BOOL fShow)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (fShow != ((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE))
        {
            obj_s* pObjParent = nullptr;
            if (pObj->objParent_ && _handle_validate(pObj->objParent_, HT_OBJECT, (LPVOID*)&pObjParent, 0)
                && pObjParent->pCls_->atomName == -1110906163 // Ex_Atom(L"SysTabControl32")
                )
            {
                if (fShow)
                    pObj->dwState_ |= STATE_SUBITEM_VISIABLE;
                else
                    pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_SUBITEM_VISIABLE);
            }
            return Ex_ObjSendMessage(hObj, WM_SHOWWINDOW, fShow, 0);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjIsValidate(HEXOBJ hObj)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    return _handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError);
}

EXHANDLE Ex_ObjGetParent(HEXOBJ hObj)
{
    INT      nError = 0;
    obj_s*   pObj   = nullptr;
    EXHANDLE ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = pObj->objParent_;
    }
    Ex_SetLastError(nError);
    return ret;
}

EXHANDLE Ex_ObjGetParentEx(HEXOBJ hObj, HEXDUI* phExDUI)
{
    INT      nError = 0;
    obj_s*   pObj   = nullptr;
    EXHANDLE ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = pObj->objParent_;
        if (phExDUI != 0) {
            wnd_s* pWnd = pObj->pWnd_;
            __set_int(phExDUI, 0, pWnd->base.hexdui_);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

LONG_PTR Ex_ObjGetLong(HEXOBJ hObj, INT nIndex)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (nIndex == OBJECT_LONG_ALPHA) {
            return pObj->dwAlpha_;
        }
        else if (nIndex == OBJECT_LONG_BLUR) {
            return (LONG_PTR)pObj->fBlur_;
        }
        else if (nIndex == OBJECT_LONG_CURSOR) {
            return (LONG_PTR)pObj->hCursor_;
        }
        else if (nIndex == OBJECT_LONG_EXSTYLE) {
            return (LONG_PTR)pObj->dwStyleEx_;
        }
        else if (nIndex == OBJECT_LONG_HCANVAS) {
            return (LONG_PTR)pObj->canvas_obj_;
        }
        else if (nIndex == OBJECT_LONG_HFONT) {
            return (LONG_PTR)pObj->hFont_;
        }
        else if (nIndex == OBJECT_LONG_ID) {
            return (LONG_PTR)pObj->id_;
        }
        else if (nIndex == OBJECT_LONG_LPARAM) {
            return pObj->lParam_;
        }
        else if (nIndex == OBJECT_LONG_LPWZTITLE) {
            return (LONG_PTR)pObj->pstrTitle_;
        }
        else if (nIndex == OBJECT_LONG_NODEID) {
            return (LONG_PTR)pObj->nodeid_;
        }
        else if (nIndex == OBJECT_LONG_OBJPARENT) {
            return (LONG_PTR)pObj->objParent_;
        }
        else if (nIndex == OBJECT_LONG_OBJPROC) {
            return (LONG_PTR)pObj->pfnSubClass_;
        }
        else if (nIndex == OBJECT_LONG_OWNER) {
            return (LONG_PTR)pObj->dwOwnerData_;
        }
        else if (nIndex == OBJECT_LONG_STATE) {
            return (LONG_PTR)pObj->dwState_;
        }
        else if (nIndex == OBJECT_LONG_STYLE) {
            return (LONG_PTR)pObj->dwStyle_;
        }
        else if (nIndex == OBJECT_LONG_TEXTFORMAT) {
            return (LONG_PTR)pObj->dwTextFormat_;
        }
        else if (nIndex == OBJECT_LONG_USERDATA) {
            return (LONG_PTR)pObj->dwUserData_;
        }
        else if (nIndex >= 0) {   // 组件额外数据
            return _obj_getextralong(pObj, nIndex);
        }
        else {
            EX_ASSERT(false, L"Ex_ObjGetLong: unknown EOL index: %ld", nIndex);
        }
    }
    Ex_SetLastError(nError);
    return 0;
}

LONG_PTR Ex_ObjSetLong(HEXOBJ hObj, INT nIndex, LONG_PTR dwNewLong)
{
    INT      nError = 0;
    obj_s*   pObj   = nullptr;
    LONG_PTR ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (nIndex == OBJECT_LONG_ALPHA) {
            ret            = (LONG_PTR)pObj->dwAlpha_;
            pObj->dwAlpha_ = (INT)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_BLUR) {
            ret          = (LONG_PTR)pObj->fBlur_;
            pObj->fBlur_ = dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_CURSOR) {
            ret            = (LONG_PTR)pObj->hCursor_;
            pObj->hCursor_ = (HCURSOR)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_EXSTYLE) {
            if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, OBJECT_LONG_EXSTYLE, dwNewLong) == 0) {
                ret = pObj->dwStyleEx_;
                Ex_ObjSendMessage(hObj, WM_STYLECHANGED, OBJECT_LONG_EXSTYLE, dwNewLong);
            }
        }
        else if (nIndex == OBJECT_LONG_HCANVAS) {
            ret               = (LONG_PTR)pObj->canvas_obj_;
            pObj->canvas_obj_ = (HEXCANVAS)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_HFONT) {
            ret          = (LONG_PTR)pObj->hFont_;
            pObj->hFont_ = dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_ID) {
            ret       = (LONG_PTR)pObj->id_;
            pObj->id_ = (INT)dwNewLong;
            if (ret != 0) {
                HashTable_Remove(pObj->pWnd_->hTableObjects_, ret);
            }
            wnd_s*        pWnd          = pObj->pWnd_;
            EX_HASHTABLE* hTableObjects = pWnd->hTableObjects_;
            HashTable_Set(hTableObjects, dwNewLong, hObj);
        }
        else if (nIndex == OBJECT_LONG_LPARAM) {
            ret           = pObj->lParam_;
            pObj->lParam_ = dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_LPWZTITLE) {
            ret              = (LONG_PTR)pObj->pstrTitle_;
            pObj->pstrTitle_ = (LPCWSTR)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_NODEID) {
            ret           = (LONG_PTR)pObj->nodeid_;
            pObj->nodeid_ = (INT)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_OBJPARENT) {
            ret              = (LONG_PTR)pObj->objParent_;
            pObj->objParent_ = (EXHANDLE)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_OBJPROC) {
            ret                = (size_t)pObj->pfnSubClass_;
            pObj->pfnSubClass_ = (MsgPROC)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_OWNER) {
            ret                = (LONG_PTR)pObj->dwOwnerData_;
            pObj->dwOwnerData_ = (LPVOID)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_STATE) {
            ret            = pObj->dwState_;
            pObj->dwState_ = dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_STYLE) {
            if (Ex_ObjSendMessage(hObj, WM_STYLECHANGING, OBJECT_LONG_STYLE, dwNewLong) == 0) {
                ret            = pObj->dwStyle_;
                pObj->dwStyle_ = dwNewLong;
                Ex_ObjSendMessage(hObj, WM_STYLECHANGED, OBJECT_LONG_STYLE, dwNewLong);
            }
        }
        else if (nIndex == OBJECT_LONG_TEXTFORMAT) {
            ret                 = (LONG_PTR)pObj->dwTextFormat_;
            pObj->dwTextFormat_ = (INT)dwNewLong;
        }
        else if (nIndex == OBJECT_LONG_USERDATA) {
            ret               = (LONG_PTR)pObj->dwUserData_;
            pObj->dwUserData_ = (LPVOID)dwNewLong;
        }
        else if (nIndex >= 0) {   // 组件额外数据
            ret = _obj_setextralong(pObj, nIndex, dwNewLong);
        }
        else {
            EX_ASSERT(false, L"Ex_ObjSetLong: unknown EOL index: %ld", nIndex);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

void _obj_reset_path(obj_s* pObj, INT left, INT top, INT right, INT bottom, INT nOffset)
{
    HEXPATH path = 0;
    if (_path_create(PATH_FLAG_DISABLESCALE, &path)) {
        if (_path_open(path)) {
            _path_beginfigure2(path, left, top);
            _path_addroundedrect(path, left, top, right, bottom, pObj->radius_topleft_,
                                 pObj->radius_topright_, pObj->radius_bottomleft_,
                                 pObj->radius_bottomright_);
            _path_endfigure(path, TRUE);
            _path_close(path);
        }
        EXHANDLE old = __get_int(pObj, nOffset);
        __set_int(pObj, nOffset, path);
        _path_destroy(old);
    }
}

LPVOID _obj_pOwner(obj_s* pObj)
{
    return pObj->dwOwnerData_;
}

void _obj_scroll_updatepostion(HEXOBJ hSB, obj_s* pSB, BOOL bVScroll, INT cLeft, INT cTop,
                               INT cRight, INT cBottom, BOOL fDispatch)
{

    LPVOID own  = pSB->dwOwnerData_;
    INT    xyz  = ((si_s*)own)->xyz_;
    auto   xyz1 = HIWORD(xyz);
    auto   xyz2 = LOBYTE(xyz1);
    INT    l, t, r, b;

    if (((pSB->dwStyle_ & SCROLLBAR_STYLE_RIGHTBOTTOMALIGN) == SCROLLBAR_STYLE_RIGHTBOTTOMALIGN)) {
        if (bVScroll) {
            l = cRight;
            t = cTop;
            r = l + xyz2;
            b = cBottom;
        }
        else {
            l = cLeft;
            t = cBottom;
            r = cRight;
            b = t + xyz2;
        }
    }
    else {
        if (bVScroll) {
            l = cLeft - xyz2;
            t = cTop;
            r = cLeft;
            b = cBottom;
        }
        else {
            l = cLeft;
            t = cTop - xyz2;
            r = cRight;
            b = cTop;
        }
    }
    INT flag = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOACTIVATE | SWP_EX_NODPISCALE;
    if (!fDispatch) {
        flag = flag | SWP_ASYNCWINDOWPOS;
    }
    INT nError = 0;
    _obj_setpos_org(pSB, hSB, 0, l, t, r - l, b - t, flag, &nError);
}

void _obj_setpos_org(obj_s* pObj, EXHANDLE hObj, EXHANDLE hObjInsertAfter, INT x, INT y, INT width,
                     INT height, INT flags, INT* nError)
{
    //'
    // SWP_ASNCWINDOWPOS：[异步请求]如果调用进程不拥有窗口，系统会向拥有窗口的线程发出需求。这就防止调用线程在其他线程处理需求的时候发生死锁。
    //' SWP_DEFERERASE：防止产生WM_SYNCPAINT消息。
    //' SWP_DRAWFRAME：在窗口周围画一个边框（定义在窗口类描述中）。
    //'
    // SWP_FRAMECHANGED：给窗口发送WM_NCCALCSIZE消息，即使窗口尺寸没有改变也会发送该消息。如果未指定这个标志，只有在改变了窗口尺寸时才发送WM_NCCALCSIZE。
    //' SWP_HIDEWINDOW;隐藏窗口。
    //'
    // SWP_NOACTIVATE：不激活窗口。如果未设置标志，则窗口被激活，并被设置到其他最高级窗口或非最高级组的顶部（根据参数hWndlnsertAfter设置）。
    //'
    // SWP_NOCOPYBITS：清除客户区的所有内容。如果未设置该标志，客户区的有效内容被保存并且在窗口尺寸更新和重定位后拷贝回客户区。
    //' SWP_NOMOVE：维持当前位置（忽略X和Y参数）。
    //'
    // SWP_NOREDRAW:不重画改变的内容。适用于客户区和非客户区（包括标题栏和滚动条）和任何由于窗回移动而露出的父窗口的所有部分。如果设置了这个标志，应用程序必须明确地使窗口无效并重画窗口的任何部分和父窗口需要重画的部分。
    //' SWP_NOSENDCHANGING：防止窗口接收WM_WINDOWPOSCHANGING消息。
    //' SWP_NOSIZE：维持当前尺寸（忽略cx和Cy参数）。
    //' SWP_NOZORDER：维持当前Z序（忽略hWndlnsertAfter参数）。
    //' SWP_SHOWWINDOW：显示窗口。
    //' SWP_NOREPOSITION；与SWP_NOOWNERZORDER标志相同。
    //' SWP_NOOWNERZORDER：不改变z序中的所有者窗口的位置。
    //'
    // 注意事项：使用SetWindowPos()如果设置了SWP_SHOWWINDOWS或者SWP_HIDEWINDOW，那么窗口将不能被移动和改变大小，我使用时就是设置了SWP_SHOWWINDOW,从而导致不能重绘背景。

    if (width < 0 && width != OBJECT_POSITION_DEFAULT) {
        width = 0;
    }
    if (height < 0 && height != OBJECT_POSITION_DEFAULT) {
        height = 0;
    }

    wnd_s* pWnd    = pObj->pWnd_;
    HWND   hWnd    = pWnd->hWnd_;
    BOOL   fAsyn   = (flags & SWP_ASYNCWINDOWPOS) == SWP_ASYNCWINDOWPOS;
    BOOL   fNotify = (flags & SWP_NOSENDCHANGING) == 0;

    if ((flags & SWP_NOZORDER) == 0)   // 调整Z序
    {
        _obj_z_set(hObj, pObj, hObjInsertAfter, flags, nError);
    }
    if (_obj_autosize(pObj, hObj, &width, &height)) {
       
        flags = flags - (flags & SWP_NOSIZE);
        flags = flags | SWP_EX_NODPISCALE;
    }
    NCCALCSIZE_PARAMS np = {0};

    if ((flags & SWP_NOMOVE) == 0 || (flags & SWP_NOSIZE) == 0 || (flags & SWP_DRAWFRAME) != 0) {
        BOOL fScale = (flags & SWP_EX_NODPISCALE) == 0;

        if ((flags & SWP_NOMOVE) == 0)   // 移动
        {
            if (x == OBJECT_POSITION_DEFAULT) {
                x = pObj->left_;
            }
            else {
                if (fScale) {
                    x = (x);
                }
            }
            if (y == OBJECT_POSITION_DEFAULT) {
                y = pObj->top_;
            }
            else {
                if (fScale) {
                    y = (y);
                }
            }
        }

        if ((flags & SWP_NOSIZE) == 0)   // 修改尺寸
        {
            if (width == OBJECT_POSITION_DEFAULT) {
                width = pObj->right_ - pObj->left_;
            }
            else {
                if (fScale) {
                    width = (width);
                }
            }
            if (height == OBJECT_POSITION_DEFAULT) {
                height = pObj->bottom_ - pObj->top_;
            }
            else {
                if (fScale) {
                    height = (height);
                }
            }
        }

        WINDOWPOS wp;
        wp.hwnd            = (HWND)hObj;
        wp.hwndInsertAfter = (HWND)hObjInsertAfter;
        wp.x               = x;
        wp.y               = y;
        wp.cx              = width;
        wp.cy              = height;
        wp.flags           = flags;
        np.lppos           = &wp;
        
        if (fNotify) {
            if (fAsyn) {
                _obj_sendmessage(hWnd, hObj, pObj, WM_WINDOWPOSCHANGING, 0, (size_t)np.lppos, 0);
            }
            else {
                _obj_baseproc(hWnd, hObj, pObj, WM_WINDOWPOSCHANGING, 0, (size_t)np.lppos);
            }
        }
        
        MINMAXINFO minmaxinfo{};
        minmaxinfo.ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK);
        minmaxinfo.ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK);
       
        if (fNotify)
        {
            if (fAsyn)
            {
                _obj_sendmessage(hWnd, hObj, pObj, WM_GETMINMAXINFO, 0, (size_t)&minmaxinfo, 0);//pObj + offsetof(obj_s, minmaxinfo)
            }
            else
            {
                _obj_baseproc(hWnd, hObj, pObj, WM_GETMINMAXINFO, 0, (size_t)&minmaxinfo);
            }
        }

        if (((pObj->base.dwFlags_ & EOF_BUSERPROCESSESED) == EOF_BUSERPROCESSESED)) {
            
            np.rgrc[0].left = minmaxinfo.ptMaxPosition.x;
            np.rgrc[0].top = minmaxinfo.ptMaxPosition.y;
            np.rgrc[0].right = minmaxinfo.ptMaxPosition.x;
            np.rgrc[0].bottom = minmaxinfo.ptMaxPosition.y;
 
            if (np.rgrc[0].left != 0 && np.rgrc[0].left < width) {
                width = np.rgrc[0].left;
            }
            if (np.rgrc[0].top != 0 && np.rgrc[0].top < height) {
                height = np.rgrc[0].top;
            }
            if (np.rgrc[0].right != 0 && np.rgrc[0].right < x) {
                x = np.rgrc[0].right;
            }
            if (np.rgrc[0].bottom != 0 && np.rgrc[0].bottom < y) {
                y = np.rgrc[0].bottom;
            }
        }
        // WM_NCCALCSIZE
        np.rgrc[1].left   = pObj->left_;
        np.rgrc[1].top    = pObj->top_;
        np.rgrc[1].right  = pObj->right_;
        np.rgrc[1].bottom = pObj->bottom_;

        if ((flags & SWP_NOMOVE) != 0)   // 老位置
        {
            np.rgrc[0].left = np.rgrc[1].left;
            np.rgrc[0].top  = np.rgrc[1].top;
        }
        else {
            np.rgrc[0].left = x;
            np.rgrc[0].top  = y;
        }
        if ((flags & SWP_NOSIZE) != 0)   // 老尺寸
        {
            np.rgrc[0].right  = np.rgrc[0].left + np.rgrc[1].right - np.rgrc[1].left;
            np.rgrc[0].bottom = np.rgrc[0].top + np.rgrc[1].bottom - np.rgrc[1].top;
        }
        else {
            np.rgrc[0].right  = np.rgrc[0].left + width;
            np.rgrc[0].bottom = np.rgrc[0].top + height;
        }
        np.rgrc[2].left   = np.rgrc[0].left;
        np.rgrc[2].top    = np.rgrc[0].top;
        np.rgrc[2].right  = np.rgrc[0].right;
        np.rgrc[2].bottom = np.rgrc[0].bottom;

        pObj->left_   = np.rgrc[0].left;
        pObj->top_    = np.rgrc[0].top;
        pObj->right_  = np.rgrc[0].right;
        pObj->bottom_ = np.rgrc[0].bottom;

        OffsetRect(&np.rgrc[2], (-np.rgrc[0].left), (-np.rgrc[0].top));
        pObj->c_left_   = np.rgrc[2].left;
        pObj->c_top_    = np.rgrc[2].top;
        pObj->c_right_  = np.rgrc[2].right;
        pObj->c_bottom_ = np.rgrc[2].bottom;
        if (fNotify) {
            if (fAsyn) {
                _obj_sendmessage(hWnd, hObj, pObj, WM_NCCALCSIZE, 1, (size_t)&np, 0);
            }
            else {
                _obj_baseproc(hWnd, hObj, pObj, WM_NCCALCSIZE, 1, (size_t)&np);
            }
        }
        RECT rcOld{0};
        // 更新窗口位置
        rcOld.left   = pObj->w_left_;
        rcOld.top    = pObj->w_top_;
        rcOld.right  = pObj->w_right_;
        rcOld.bottom = pObj->w_bottom_;

        _obj_updatewindowpostion(pObj, (LPVOID)((size_t)pObj + offsetof(obj_s, w_left_)),
                                 (flags & SWP_NOMOVE) == 0);
        // 更新被修改后的位置信息
        if ((flags & SWP_NOMOVE) == 0) {
            x = np.rgrc[0].left;
            y = np.rgrc[0].top;
        }
        if ((flags & SWP_NOSIZE) == 0) {
            width  = np.rgrc[0].right - np.rgrc[0].left;
            height = np.rgrc[0].bottom - np.rgrc[0].top;
        }

        if (((pObj->base.dwFlags_ & EOF_BPATHBYROUNDEDRECT) == EOF_BPATHBYROUNDEDRECT)) {
            _obj_reset_path(pObj, np.rgrc[2].left, np.rgrc[2].top, np.rgrc[2].right,
                            np.rgrc[2].bottom, offsetof(obj_s, hPath_Client_));
            if (pObj->radius_rgn_)
                _rgn_destroy(pObj->radius_rgn_);
            pObj->radius_rgn_ = _rgn_createfrompath(pObj->hPath_Client_);
            _obj_reset_path(pObj, pObj->w_left_, pObj->w_top_, pObj->w_right_, pObj->w_bottom_,
                            offsetof(obj_s, hPath_Window_));
        }

        if ((flags & SWP_NOSIZE) == 0) {
            if (pObj->pCls_->atomName != ATOM_PAGE) {
                _canvas_resize(pObj->canvas_obj_, width, height);
            }
        }

        // WM_WINDOWPOSCHANGED
        if (fNotify) {
            if (fAsyn) {
                _obj_sendmessage(hWnd, hObj, pObj, WM_WINDOWPOSCHANGED, 0, (size_t)&np.lppos, 0);
            }
            else {
                _obj_baseproc(hWnd, hObj, pObj, WM_WINDOWPOSCHANGED, 0, (size_t)&np.lppos);
            }
        }

        // WM_WINDOWPOSCHANGED 如果用户处理掉了，则不发送 WM_SIZE / WM_MOVE

        if (((pObj->base.dwFlags_ & EOF_BUSERPROCESSESED) != EOF_BUSERPROCESSESED) && fNotify) {
            // 应该得发送客户区矩形
            if ((flags & SWP_NOMOVE) == 0) {
                if (fAsyn) {
                    _obj_sendmessage(hWnd, hObj, pObj, WM_MOVE, 0, MAKELONG(x, y), 0);
                }
                else {
                    _obj_baseproc(hWnd, hObj, pObj, WM_MOVE, 0, MAKELONG(x, y));
                }
            }

            if ((flags & SWP_NOSIZE) == 0) {
                if (fAsyn) {
                    _obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(width, height), 0);
                }
                else {
                    _obj_baseproc(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(width, height));
                }
            }
        }
        _obj_scroll_repostion(hWnd, hObj, TRUE);   // 重新更新滚动条位置，与其状态无关
        UnionRect(&np.rgrc[2], (RECT*)((size_t)pObj + offsetof(obj_s, w_left_)), &rcOld);
    }
    else {
        np.rgrc[2].left   = pObj->left_;
        np.rgrc[2].top    = pObj->top_;
        np.rgrc[2].right  = pObj->right_;
        np.rgrc[2].bottom = pObj->bottom_;
    }

    if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
        BOOL fScale = ((pObj->pWnd_->base.dwFlags_ & EWF_SIZED) == EWF_SIZED);
        if ((flags & SWP_NOREDRAW) == 0)   // 重画
        {
            if (!fScale)
            {
                LPRECT RCP = &pWnd->ulwi_prcDirty;
                UnionRect(RCP, RCP, &np.rgrc[2]);
                InvalidateRect(hWnd, &np.rgrc[2], FALSE);
            }
            _obj_invalidaterect(pObj, 0, 0);
        }
        if (!fScale) {
            if ((flags & SWP_EX_UPDATEOBJECT) == SWP_EX_UPDATEOBJECT) {
                UpdateWindow(hWnd);
            }
        }
    }
}

BOOL Ex_ObjSetPos(HEXOBJ hObj, HEXOBJ hObjInsertAfter, INT x, INT y, INT width, INT height,
                  INT flags)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_setpos_org(pObj, hObj, hObjInsertAfter, x, y, width, height, flags, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _obj_scroll_repostion(HWND hWnd, HEXOBJ hObj, BOOL fDispatch)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    si_s*  psi    = nullptr;
    obj_s* pHSB   = nullptr;
    obj_s* pVSB   = nullptr;
    HEXOBJ hVSB;
    HEXOBJ hHSB;
    WORD   xyz = NULL;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        RECT rcClient{0};
        rcClient.left   = pObj->c_left_;
        rcClient.top    = pObj->c_top_;
        rcClient.right  = pObj->c_right_;
        rcClient.bottom = pObj->c_bottom_;

        if (((pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) == OBJECT_STYLE_VSCROLL)) {
            hVSB = pObj->objVScroll_;
            if (_handle_validate(hVSB, HT_OBJECT, (LPVOID*)&pVSB, &nError)) {
                psi = (si_s*)_obj_pOwner(pVSB);
                if (psi != 0) {
                    xyz = HIWORD(psi->xyz_);
                }

                if (((pVSB->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {

                    if (((pVSB->dwStyle_ & SCROLLBAR_STYLE_RIGHTBOTTOMALIGN) ==
                         SCROLLBAR_STYLE_RIGHTBOTTOMALIGN)) {
                        rcClient.right = rcClient.right - LOBYTE(xyz);
                    }
                    else {
                        rcClient.left = rcClient.left + LOBYTE(xyz);
                    }
                }
            }
        }

        if (((pObj->dwStyle_ & OBJECT_STYLE_HSCROLL) == OBJECT_STYLE_HSCROLL)) {
            hHSB = pObj->objHScroll_;
            if (_handle_validate(hHSB, HT_OBJECT, (LPVOID*)&pHSB, &nError)) {
                psi = (si_s*)_obj_pOwner(pHSB);
                if (psi != 0) {
                    xyz = HIWORD(psi->xyz_);
                }

                if (((pHSB->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {

                    if (((pHSB->dwStyle_ & SCROLLBAR_STYLE_RIGHTBOTTOMALIGN) ==
                         SCROLLBAR_STYLE_RIGHTBOTTOMALIGN)) {
                        rcClient.bottom = rcClient.bottom - LOBYTE(xyz);
                    }
                    else {
                        rcClient.top = rcClient.top + LOBYTE(xyz);
                    }
                }
            }
        }

        if (pVSB != 0) {
            _obj_scroll_updatepostion(hVSB, pVSB, TRUE, rcClient.left, rcClient.top, rcClient.right,
                                      rcClient.bottom, fDispatch);
        }
        if (pHSB != 0) {
            _obj_scroll_updatepostion(hHSB, pHSB, FALSE, rcClient.left, rcClient.top,
                                      rcClient.right, rcClient.bottom, fDispatch);
        }
    }
}

size_t _obj_msgproc(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    if (uMsg == WM_PAINT) {
        if (!((pObj->base.dwFlags_ & EOF_BNEEDREDRAW) == EOF_BNEEDREDRAW)) {
            _obj_invalidaterect(pObj, 0, &nError);
            
            if (wParam == 0 && lParam == 0) return 0;
        }
    }
    return _obj_baseproc(hWnd, hObj, pObj, uMsg, wParam, lParam);
}

void _obj_notify_brothers(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam,
                          LPARAM lParam, BOOL bBypassSelf, BOOL bSameClass)
{
    LPVOID   pObjEntry = nullptr;
    EXHANDLE hParent   = pObj->objParent_;
    INT      nError    = 0;
    if (hParent == 0) {
        pObjEntry = pObj->pWnd_;
    }
    else {
        if (!_handle_validate(hParent, HT_OBJECT, &pObjEntry, &nError)) {
            return;
        }
    }
    EXHANDLE objEntry = ((obj_s*)pObjEntry)->base.objChildFirst_;
    while (_handle_validate(objEntry, HT_OBJECT, &pObjEntry, &nError)) {
        if (bBypassSelf) {
            if (objEntry == hObj) {
                objEntry = ((obj_s*)pObjEntry)->objNext_;
                continue;
            }
        }
        if (bSameClass) {
            EXATOM atomName  = ((obj_s*)pObjEntry)->pCls_->atomName;
            EXATOM atomName2 = pObj->pCls_->atomName;
            if (atomName != atomName2) {
                objEntry = ((obj_s*)pObjEntry)->objNext_;
                continue;
            }
        }
        HEXOBJ pNext = ((obj_s*)pObjEntry)->objNext_;
        _obj_baseproc(hWnd, objEntry, (obj_s*)pObjEntry, uMsg, wParam, lParam);
        objEntry = pNext;
    }
}

size_t Ex_ObjDispatchMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    size_t ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _obj_baseproc(_obj_gethwnd(pObj), hObj, pObj, uMsg, wParam, lParam);
    }
    Ex_SetLastError(nError);
    return ret;
}

LRESULT _obj_dispatchnotify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
                            LPARAM lParam)
{
    // 在通知处理期间对象可能被销毁
    if (!_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
    {
        return 0;
    }
    nID                                        = pObj->id_;
    LRESULT                 ret                = 0;
    wnd_s*                  pWnd               = pObj->pWnd_;
    EX_EVENT_HANDLER_TABLE* pEventHandlerTable = NULL;
    if (HashTable_Get(pWnd->hTableEvent_, nCode, (size_t*)&pEventHandlerTable)) {
        INT len = pEventHandlerTable->len;
        for (INT i = 0; i < len; i++) {
            if (pEventHandlerTable->handler[i].hObj == hObj) {
                ret = pEventHandlerTable->handler[i].pfnCallback(hObj, nID, nCode, wParam, lParam);

                if (!ret) {
                    break;
                }
                return ret;
            }
        }
    }
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))//此处pObj是局部变量,但其指向的内存地址可能已经被释放
    {
        EX_NMHDR nmhdr{ 0 };
        nmhdr.hObjFrom = hObj;
        nmhdr.idFrom = nID;
        nmhdr.nCode = nCode;
        nmhdr.wParam = wParam;
        nmhdr.lParam = lParam;
        EXHANDLE hParent = pObj->objParent_;
        auto dwStyle = pObj->dwStyle_;
        auto dwFlags = pObj->base.dwFlags_;
        auto pWnd = pObj->pWnd_;
        ret = _obj_baseproc(hWnd, hObj, pObj, WM_NOTIFY, nID, (size_t)&nmhdr); //发给自身
        if (ret == 0 && (dwStyle & OBJECT_STYLE_NOTIFYPARENT) == OBJECT_STYLE_NOTIFYPARENT)
        {
            obj_s* pParent = nullptr;
            while (_handle_validate(hParent, HT_OBJECT, (LPVOID*)&pParent, 0))
            {
                ret = _obj_baseproc(hWnd, hParent, pParent, WM_NOTIFY, nID, (size_t)&nmhdr); //逐层通知父控件
                if (ret != 0)
                {
                    break;
                }
                if ((dwFlags & EOF_BEVENTBUBBLE) != EOF_BEVENTBUBBLE)
                {
                    break;
                }
                hParent = pParent->objParent_;
            }
            if (ret == 0)
            {
                ret = _wnd_dispatch_notify(hWnd, pWnd, hObj, nID, nCode, wParam, lParam);
            }
        }
    }
    
    return ret;
}

size_t Ex_ObjDispatchNotify(HEXOBJ hObj, INT nCode, WPARAM wParam, LPARAM lParam)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    size_t ret    = 1;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, hObj, 0, nCode, wParam, lParam);
    }
    Ex_SetLastError(nError);
    return ret;
}

void _obj_backgroundimage_clear(HWND hWnd, obj_base* pObj)
{
    LPVOID dwTmp = pObj->lpBackgroundImage_;

    if (dwTmp != 0) {
        ((obj_s*)pObj)->base.uElapse_ = 0;
        KillTimer(hWnd, (UINT_PTR)((size_t)pObj + TIMER_BKG));
        _img_destroy(((EX_BACKGROUNDIMAGEINFO*)dwTmp)->hImage);
        _struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpDelay));
        _struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpGrid));
        _struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpRcSrc));//新增
        _struct_destroyfromaddr(dwTmp, offsetof(EX_BACKGROUNDIMAGEINFO, lpRCFDst));//新增
        _struct_destroyfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_));
    }
}

void _obj_destroy(HEXOBJ hObj, obj_s* pObj, INT* nError)
{
    HWND   hWnd = _obj_gethwnd(pObj);
    wnd_s* pWnd = pObj->pWnd_;
    RECT   rc{0};
    rc.left   = pObj->w_left_;
    rc.top    = pObj->w_top_;
    rc.right  = pObj->w_right_;
    rc.bottom = pObj->w_bottom_;
    KillTimer(hWnd, (size_t)pObj + TIMER_MOUSEHOVER);
    KillTimer(hWnd, (size_t)pObj + TIMER_OBJECT);

    BOOL fRelaseCaption = FALSE;
    if (pWnd->objTrack_ == hObj) {
        pWnd->objTrack_ = 0;
        fRelaseCaption  = TRUE;
    }
    if (pWnd->objFocus_ == hObj) {
        pWnd->objFocus_ = 0;
    }
    if (pWnd->objHittest_ == hObj) {
        pWnd->objHittest_ = 0;
    }
    if (pWnd->objFocusPrev_ == hObj) {
        pWnd->objFocusPrev_ = 0;
    }
    if (pWnd->base.objChildLast_ == hObj) {
        pWnd->base.objChildLast_ = pObj->objPrev_;
    }
    if (pWnd->base.objChildFirst_ == hObj) {
        pWnd->base.objChildFirst_ = pObj->objNext_;
    }

    // backgroundinfo
    _obj_backgroundimage_clear(hWnd, (obj_base*)pObj);

    // Clean EventHandler
    EX_HASHTABLE*       hTableEvent = pWnd->hTableEvent_;
    std::vector<size_t> aKey;
    std::vector<size_t> aValue;
    HashTable_GetAllKeysAndValues(hTableEvent, aKey, aValue);
    for (INT i = 0; i < aValue.size(); i++) {
        EX_EVENT_HANDLER_TABLE* pEventHandlerTable = (EX_EVENT_HANDLER_TABLE*)aValue[i];
        INT                     len                = pEventHandlerTable->len;
        for (INT j = 0; j < len; j++) {
            if (pEventHandlerTable->handler[j].hObj == hObj) {
                if (--pEventHandlerTable->len) {
                    if (len - j - 1 > 0) {
                        RtlMoveMemory(&pEventHandlerTable->handler[j],
                                      &pEventHandlerTable->handler[j + 1],
                                      (len - j - 1) * sizeof(EX_EVENT_HANDLER));
                    }
                }
                else {
                    HashTable_Remove(hTableEvent, aKey[i]);
                }
                break;
            }
        }
    }
    EXHANDLE  Parent = 0;
    obj_base* pParnet = nullptr;
    _obj_z_clear(hObj, pObj, &Parent, &pParnet);
    // 清理子组件
    HEXOBJ sObj  = pObj->base.objChildFirst_;
    obj_s* psObj = nullptr;
    if (_handle_validate(sObj, HT_OBJECT, (LPVOID*)&psObj, nError)) {
        _obj_notify_brothers(hWnd, sObj, psObj, WM_DESTROY, 0, 0, FALSE, FALSE);
    }
    // 释放资源
    _layout_destroy(pObj->base.hLayout_);
    _font_destroy(pObj->hFont_);
    _canvas_destroy(pObj->canvas_obj_);
    _path_destroy(pObj->hPath_Client_);
    _path_destroy(pObj->hPath_Window_);
    _rgn_destroy(pObj->radius_rgn_);

    _struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTips_));
    _struct_destroyfromaddr(pObj, offsetof(obj_s, pstrTitle_));
    if (pObj->nPropCount_ == -1) {
        HashTable_Destroy(pObj->pPropListEntry_);
    }
    else if (pObj->pPropListEntry_ != 0) {
        Ex_MemFree(pObj->pPropListEntry_);
    }

    if (pObj->hCursor_) {
        DestroyCursor((HCURSOR)(pObj->hCursor_));
        pObj->hCursor_ = nullptr;
    }
    Ex_MemFree((LPVOID)pObj->pstrTips_);
    Ex_MemFree((LPVOID)pObj->pstrTitle_);
    if (fRelaseCaption) {
        ReleaseCapture();
    }
    _handle_destroy(hObj, nError);
    Ex_MemFree(pObj);

    if (!((pWnd->base.dwFlags_ & EWF_BDESTROYWINDOW) == EWF_BDESTROYWINDOW)) {
        LPRECT RCP = &pWnd->ulwi_prcDirty;
        UnionRect(RCP, RCP, &rc);
        InvalidateRect(hWnd, &rc, FALSE);
    }
}

HEXOBJ _obj_create_init(HWND hWnd, wnd_s* pWnd, EXATOM atomClass, MsgPROC pfnMsgProc, obj_s** pObj,
                        INT* nError)
{
    EX_CLASSINFO* pCls = 0;
    HEXOBJ        hObj = 0;
    HashTable_Get(g_Li.hTableClass, atomClass, (size_t*)&pCls);
    if (pCls == 0) {
        *nError = ERROR_EX_INVALID_CLASS;
    }
    else {
        *pObj = (obj_s*)Ex_MemAlloc(sizeof(obj_s) + pCls->cbObjExtra);
        if (*pObj == 0) {
            *nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            hObj = _handle_create(HT_OBJECT, *pObj, nError);

            if (hObj == 0) {
                Ex_MemFree(*pObj);
                *nError = ERROR_EX_HANDLE_INVALID;
            }
        }
    }
    if (*nError != 0) return 0;

    // 初始化组件数据
    (*pObj)->base.hObj_              = hObj;
    (*pObj)->pWnd_              = pWnd;
    (*pObj)->pCls_              = pCls;
    (*pObj)->pfnClsProc_        = pCls->pfnClsProc;
    (*pObj)->pfnSubClass_       = pfnMsgProc;
    (*pObj)->dwAlpha_           = 255;
    (*pObj)->dwAlphaDisable_    = 128;
    (*pObj)->hCursor_           = pCls->hCursor;
    (*pObj)->base.lpBackgroundImage_ = 0;

    if (atomClass == ATOM_PAGE) {
       (*pObj)->base.dwFlags_ = (*pObj)->base.dwFlags_ | EOF_BPAGE;
    }
    return hObj;
}

void _obj_create_proc(INT* nError, BOOL fScale, HEXTHEME hTheme, obj_s* pObj, INT dwStyleEx,
                      EXATOM atomClass, LPCWSTR lpszName, INT dwStyle, INT x, INT y, INT width,
                      INT height, EXHANDLE hParent, INT nID, EXATOM atomName, LPARAM lParam,
                      INT dwTextFormat)
{

    HEXOBJ        hObj = pObj->base.hObj_;
    wnd_s*        pWnd = pObj->pWnd_;
    HWND          hWnd = pWnd->hWnd_;
    EX_CLASSINFO* pCls = pObj->pCls_;
    atomClass          = pCls->atomName;
    if (dwStyle == -1) {
        dwStyle = pCls->dwStyle;
    }

    if (dwStyleEx == -1) {
        dwStyleEx = pCls->dwStyleEx;
    }
    if (dwTextFormat == -1) {
        dwTextFormat = pCls->dwTextFormat;
    }

    if (hTheme == 0) {
        hTheme = pWnd->base.hTheme_;
    }
    else {
        if (!HashTable_IsExist(((EX_THEME*)hTheme)->tableClass, atomClass)) {
            hTheme = pWnd->base.hTheme_;
        }
    }

    RECT rcObj{0};
    rcObj.left   = x;
    rcObj.top    = y;
    rcObj.right  = x + width;
    rcObj.bottom = y + height;
    if (fScale) {
        rcObj.left   = Ex_Scale(rcObj.left);
        rcObj.top    = Ex_Scale(rcObj.top);
        rcObj.right  = Ex_Scale(rcObj.right);
        rcObj.bottom = Ex_Scale(rcObj.bottom);
    }

    // 初始化矩形
    pObj->left_     = rcObj.left;
    pObj->top_      = rcObj.top;
    pObj->right_    = rcObj.right;
    pObj->bottom_   = rcObj.bottom;
    pObj->c_left_   = rcObj.left;
    pObj->c_top_    = rcObj.top;
    pObj->c_right_ = rcObj.right - rcObj.left;
    pObj->c_bottom_ = rcObj.bottom - rcObj.top;

    // 初始化画布
    INT flags = 0;
    flags     = CANVAS_FLAG_GDI_COMPATIBLE;

    if (((EX_CLASSINFO*)pCls)->atomName == ATOM_PAGE) {

        pObj->canvas_obj_ = _canvas_createfrompwnd(pWnd, 1, 1, flags, nError);
    }
    else {

        pObj->canvas_obj_ = _canvas_createfrompwnd(pWnd, rcObj.right - rcObj.left,
                                                   rcObj.bottom - rcObj.top, flags, nError);
    }
    // 初始化其它数据

    LPVOID pParent = nullptr;

    if (!_handle_validate(hParent, HT_OBJECT, &pParent, nError)) hParent = 0;

    pObj->objParent_    = hParent;
    pObj->dwStyle_      = dwStyle;
    pObj->dwStyleEx_    = dwStyleEx;
    pObj->dwTextFormat_ = dwTextFormat;
    pObj->lParam_       = lParam;
    pObj->base.hTheme_       = hTheme;
    pObj->pstrTitle_    = StrDupW(lpszName);
    if ((dwStyleEx & OBJECT_STYLE_EX_BLUR) != 0) {
        pObj->fBlur_ = 15.f;
    }


    if (nID != 0) {
        pObj->id_ = nID;
        HashTable_Set(pWnd->hTableObjects_, nID, hObj);
    }


    // 初始化滚动条
    _obj_create_scrollbar(hWnd, pWnd, pObj, hObj, hTheme);
    // 消息分发,只提供参考,不提供修改
    _obj_baseproc(hWnd, hObj, pObj, WM_NCCREATE, 0, (size_t)&dwStyleEx);
    _obj_theme_load_color_font(pWnd, pObj, hTheme);
    _obj_baseproc(hWnd, hObj, pObj, WM_CREATE, 0, (size_t)&dwStyleEx);

    if ((dwStyleEx & OBJECT_STYLE_EX_DRAGDROP) == OBJECT_STYLE_EX_DRAGDROP) {
        if (!pWnd->lpIDropTarget_) {
            OleInitialize(0);   // 可能已经初始化
            pWnd->lpIDropTarget_ = new DropTarget(pWnd);
            HRESULT hr = RegisterDragDrop(pWnd->hWnd_, (DropTarget*)(pWnd->lpIDropTarget_));
        }
    }
}

void _obj_create_done(HWND hWnd, wnd_s* pWnd, HEXOBJ hObj, obj_s* pObj, HWND TOP)
{
    HEXFONT hFont = pObj->hFont_;
    pObj->hFont_  = NULL;

    _obj_baseproc(hWnd, hObj, pObj, WM_SETFONT, (size_t)hFont, 0);

    pObj->base.dwFlags_ = pObj->base.dwFlags_ | EOF_BCANREDRAW;
    pObj->base.dwFlags_ = pObj->base.dwFlags_ | EOF_OBJECT;

    if (!((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
        pObj->dwState_ = pObj->dwState_ | STATE_HIDDEN;
    }

    if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED)) {
        pObj->dwState_ = pObj->dwState_ | STATE_DISABLE;
    }

    if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_FOCUSABLE) == OBJECT_STYLE_EX_FOCUSABLE)) {
        pObj->dwState_ = pObj->dwState_ | STATE_ALLOWFOCUS;
    }

    INT flags = SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOCOPYBITS | SWP_EX_NODPISCALE;

    if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE)) {
        flags = flags | SWP_SHOWWINDOW;
    }

    if (((pObj->dwStyle_ & OBJECT_STYLE_BORDER) == OBJECT_STYLE_BORDER)) {
        flags = flags | SWP_DRAWFRAME;
    }

    INT nError = 0;
    _obj_setpos_org(pObj, hObj, (size_t)TOP, pObj->left_, pObj->top_,
                    pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_, flags, &nError);
    _obj_baseproc(hWnd, hObj, pObj, WM_SETTEXT, 0, (size_t)(pObj->pstrTitle_));
    _obj_invalidaterect(pObj, 0, &nError);
}

void _obj_create_scrollbar(HWND hWnd, wnd_s* pWnd, obj_s* pObj, HEXOBJ hObj, HEXTHEME hTheme)
{
    obj_s* pSB    = nullptr;
    INT    style  = 0;
    INT    nError = 0;

    if (((pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) == OBJECT_STYLE_VSCROLL)) {
        HEXOBJ hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
        if (hSb != 0) {
            pObj->objVScroll_ = hSb;
            style             = SCROLLBAR_STYLE_RIGHTBOTTOMALIGN | SCROLLBAR_STYLE_CONTROLBUTTON |
                    SCROLLBAR_STYLE_VERTICALSCROLL;

            if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLENOSCROLL) == OBJECT_STYLE_DISABLENOSCROLL)) {
                style = style | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_DISABLENOSCROLL;
            }
            _obj_create_proc(&nError, TRUE, hTheme, pSB, OBJECT_STYLE_EX_TOPMOST, ATOM_SCROLLBAR, 0,
                             style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
            _obj_create_done(hWnd, pWnd, hSb, pSB, HWND_TOP);
        }
    }

    if (((pObj->dwStyle_ & OBJECT_STYLE_HSCROLL) == OBJECT_STYLE_HSCROLL)) {
        nError     = 0;
        HEXOBJ hSb = _obj_create_init(hWnd, pWnd, ATOM_SCROLLBAR, 0, &pSB, &nError);
        if (hSb != 0) {
            pObj->objHScroll_ = hSb;
            style             = SCROLLBAR_STYLE_RIGHTBOTTOMALIGN | SCROLLBAR_STYLE_CONTROLBUTTON |
                    SCROLLBAR_STYLE_HORIZONTALSCROLL;

            if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLENOSCROLL) == OBJECT_STYLE_DISABLENOSCROLL)) {
                style = style | OBJECT_STYLE_VISIBLE | OBJECT_STYLE_DISABLENOSCROLL;
            }
            _obj_create_proc(&nError, TRUE, hTheme, pSB, OBJECT_STYLE_EX_TOPMOST, ATOM_SCROLLBAR, 0,
                             style, 0, 0, 0, 0, hObj, 0, 0, 0, 0);
            _obj_create_done(hWnd, pWnd, hSb, pSB, HWND_TOP);
        }
    }
}

void _obj_theme_load_color_font(wnd_s* pWnd, obj_s* pObj, HEXTHEME hTheme)
{
    pObj->dwShadowSize_ = 6;
    LPVOID pColors      = nullptr;
    if (hTheme != 0)   // 加载的主题包获取颜色信息
    {
        pColors = ((EX_THEME*)hTheme)->aryColors;
    }
    if (pColors == 0)   // 如果没有，则从默认的主题包获取颜色信息.
    {
        if (pWnd->base.hTheme_) pColors = ((EX_THEME*)pWnd->base.hTheme_)->aryColors;
    }
    if (pColors != 0)   // 复制颜色信息到本组件
    {
        RtlMoveMemory((LPVOID)((size_t)pObj + offsetof(obj_s, crBackground_)), pColors,
                      sizeof(colors_s));
    }

    if (hTheme != 0) {
        EX_HASHTABLE* pTheme = ((EX_THEME*)hTheme)->tableClass;
        classtable_s* pClass = 0;
        if (pTheme != 0) {
            EX_CLASSINFO* pCls = pObj->pCls_;
            if (HashTable_Get(pTheme, (size_t)pCls->atomName, (size_t*)&pClass)) {
                if (pClass != 0) {
                    EX_HASHTABLE* pProp = pClass->tableProps_;
                    if (pProp != 0) {
                        size_t dwTmp = 0;
                        for (size_t index = 0; index < g_Li.aryColorsAtom.size(); index++) {
                            if (HashTable_Get(pProp, g_Li.aryColorsAtom[index], &dwTmp)) {
                                __set(pObj, g_Li.aryColorsOffset[index], __get((LPVOID)dwTmp, 0));
                            }
                        }
                        if (HashTable_Get(pProp, ATOM_PADDING_TEXT, &dwTmp)) {
                            RtlMoveMemory((LPVOID)((size_t)pObj + offsetof(obj_s, t_left_)),
                                          (LPVOID)dwTmp, 16);
                            if (g_Li.DpiX > 1) {
                                pObj->t_left_   = pObj->t_left_ * g_Li.DpiX;
                                pObj->t_top_    = pObj->t_top_ * g_Li.DpiX;
                                pObj->t_right_  = pObj->t_right_ * g_Li.DpiX;
                                pObj->t_bottom_ = pObj->t_bottom_ * g_Li.DpiX;
                            }
                        }
                        size_t pFamily = -1;
                        size_t pSize   = -1;
                        size_t pStyle  = -1;
                        INT    Size    = -1;
                        INT    style   = -1;
                        HashTable_Get(pProp, ATOM_FONT_FAMILY, &pFamily);
                        if (HashTable_Get(pProp, ATOM_FONT_SIZE, &pSize)) {
                            Size = __get((LPVOID)pSize, 0);
                        }
                        if (HashTable_Get(pProp, ATOM_FONT_STYLE, &pStyle)) {
                            style = __get((LPVOID)pStyle, 0);
                        }
                        if (pFamily != -1 || Size != -1 || style != -1) {
                            pObj->hFont_ = _font_createfromfamily((LPWSTR)pFamily, Size, style);
                            return;
                        }
                    }
                }
            }
        }
    }

    pObj->hFont_ = _font_create();
}

HEXOBJ Ex_ObjCreateEx(INT dwStyleEx, LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle,
                      INT x, INT y, INT width, INT height, EXHANDLE hParent, INT nID,
                      INT dwTextFormat, LPARAM lParam, HEXTHEME hTheme, MsgPROC lpfnMsgProc)
{
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    EXATOM atom;
    HWND   hWnd;
    HEXOBJ hObj = NULL;
    if (_handle_validate(hParent, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        hParent = 0;
    }
    else {
        nError = 0;
        if (_handle_validate(hParent, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            pWnd = pObj->pWnd_;
        }
    }
    if (nError == 0) {
        if (HashTable_IsExist(g_Li.hTableClass, (size_t)lptszClassName)) {
            atom = (EXATOM)lptszClassName;
        }
        else {
            atom = Ex_Atom(lptszClassName);
        }

        if (atom == 0) {
            nError = ERROR_EX_INVALID_CLASS;
        }
        else {
            hWnd = pWnd->hWnd_;
            hObj = _obj_create_init(hWnd, pWnd, atom, lpfnMsgProc, &pObj, &nError);
            if (hObj != 0) {
                _obj_create_proc(&nError, TRUE, hTheme, pObj, dwStyleEx, atom, lptszObjTitle,
                                 dwStyle, x, y, width, height, hParent, nID, 0, lParam,
                                 dwTextFormat);

                _obj_create_done(hWnd, pWnd, hObj, pObj, HWND_TOP);
            }
        }
    }
    Ex_SetLastError(nError);
    return hObj;
}

HEXOBJ Ex_ObjCreate(LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y,
                    INT width, INT height, EXHANDLE hParent)
{
    return Ex_ObjCreateEx(-1, lptszClassName, lptszObjTitle, dwStyle, x, y, width, height, hParent,
                          0, -1, 0, 0, 0);
}

void _obj_visable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, BOOL fVisable)
{

    if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE) != fVisable) {
        _obj_killfocus_real(pObj, hObj, 0);//不再发送 WM_KILLFOCUS  防止在WM_KILLFOCUS事件下调用本函数导致死循环
        STYLESTRUCT sty{};
        sty.styleOld = pObj->dwStyle_;
        pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (STATE_HOVER | STATE_DOWN));
        if (fVisable) {
            pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_HIDDEN);
            pObj->dwStyle_ = pObj->dwStyle_ | OBJECT_STYLE_VISIBLE;
        }
        else {
            pObj->dwState_ = pObj->dwState_ | STATE_HIDDEN;
            pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & OBJECT_STYLE_VISIBLE);
        }
        sty.styleNew = pObj->dwStyle_;
        _obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, OBJECT_LONG_STYLE, (LPARAM)&sty);
    }
}

void _obj_disable(HWND hWnd, HEXOBJ hObj, obj_s* pObj, BOOL fDisable)
{

    if (((pObj->dwStyle_ & OBJECT_STYLE_DISABLED) == OBJECT_STYLE_DISABLED) != fDisable) {
        _obj_killfocus_real(pObj, hObj, 0);
        STYLESTRUCT sty{};
        sty.styleOld = pObj->dwStyle_;
        pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & (STATE_HOVER | STATE_DOWN));
        if (fDisable) {
            pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & STATE_DISABLE);
            pObj->dwStyle_ = pObj->dwStyle_ | OBJECT_STYLE_DISABLED;
        }
        else {
            pObj->dwState_ = pObj->dwState_ | STATE_DISABLE;
            pObj->dwStyle_ = pObj->dwStyle_ - (pObj->dwStyle_ & OBJECT_STYLE_DISABLED);
        }
        sty.styleNew = pObj->dwStyle_;
        _obj_baseproc(hWnd, hObj, pObj, WM_STYLECHANGED, OBJECT_LONG_STYLE, (LPARAM)&sty);
    }
}

EXARGB _obj_getcolor(obj_s* pObj, INT nIndex)
{
    EXARGB ret = 0;
    if (nIndex > -1 && nIndex < 11) {
        ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
    }
    return ret;
}

EXARGB Ex_ObjGetColor(HEXOBJ hObj, INT nIndex)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    EXARGB ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _obj_getcolor(pObj, nIndex);
    }
    Ex_SetLastError(nError);
    return ret;
}

EXARGB Ex_ObjSetColor(HEXOBJ hObj, INT nIndex, EXARGB dwColor, BOOL fRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    EXARGB ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (nIndex > -1 && nIndex < 11) {
            ret = __get_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4);
            __set_int(pObj, offsetof(obj_s, crBackground_) + (size_t)nIndex * 4, dwColor);
        }
        _obj_sendmessage(pObj->pWnd_->hWnd_, pObj->base.hObj_, pObj, WM_SYSCOLORCHANGE, nIndex, dwColor, 0);
        if (fRedraw) {
            nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

HEXFONT Ex_ObjGetFont(HEXOBJ hObj)
{
    obj_s*  pObj   = nullptr;
    INT     nError = 0;
    HEXFONT ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = pObj->hFont_;
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL _obj_setfont(obj_s* pObj, HEXFONT hFont, BOOL fredraw)
{
    BOOL result = FALSE;
    if (hFont != 0) {
        HEXFONT tmp  = pObj->hFont_;
        pObj->hFont_ = hFont;

        if (tmp != hFont) {
            _font_destroy(tmp);
            if (FLAGS_CHECK(pObj->dwStyleEx_, OBJECT_STYLE_EX_AUTOSIZE)) {
                FLAGS_DEL(pObj->base.dwFlags_, EOF_BAUTOSIZED);
                INT nError = 0;
                _obj_setpos_org(pObj, pObj->base.hObj_, NULL, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER,
                                &nError);
            }
            result = TRUE;
        }
    }
    if (fredraw) {
        INT nError = 0;
        _obj_invalidaterect(pObj, 0, &nError);
    }
    return result;
}

BOOL Ex_ObjSetFont(HEXOBJ hObj, HEXFONT hFont, BOOL fRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    BOOL   ret    = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = Ex_ObjSendMessage(hObj, WM_SETFONT, hFont,
                                fRedraw);   // 发送消息编辑框才能接收
                                            // ret = _obj_setfont(pObj, hFont, fRedraw);
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjSetText(HEXOBJ hObj, LPCWSTR lpString, BOOL fRedraw)
{
    return Ex_ObjSendMessage(hObj, WM_SETTEXT, fRedraw, (size_t)lpString) == 0;
}

size_t Ex_ObjGetText(HEXOBJ hObj, LPCWSTR lpString, size_t nMaxCount)
{
    return Ex_ObjSendMessage(hObj, WM_GETTEXT, nMaxCount, (size_t)lpString);
}

size_t Ex_ObjGetTextLength(HEXOBJ hObj)
{
    return Ex_ObjSendMessage(hObj, WM_GETTEXTLENGTH, 0, 0);
}

void _obj_drawbackground(obj_s* pObj, HEXCANVAS hCanvas, RECT rcPaint)
{
    INT    crBkg = _obj_getcolor(pObj, COLOR_EX_BACKGROUND);
    LPVOID hBrush;

    if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_COMPOSITED) == OBJECT_STYLE_EX_COMPOSITED)) {
        wnd_s*    pWnd = pObj->pWnd_;
        HEXCANVAS lpdd = pWnd->canvas_display_;
        _canvas_bitblt(hCanvas, lpdd, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom,
                       pObj->w_left_ + rcPaint.left, pObj->w_top_ + rcPaint.top);

        //if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_BLUR) == OBJECT_STYLE_EX_BLUR)) {
        //    fBlur = pObj->fBlur_;
        //    _canvas_blur(hCanvas, fBlur, &rcPaint);
        //}
        if (crBkg != 0) {
            hBrush = _brush_create(crBkg);
            if (hBrush != 0) {
                _canvas_fillrect(hCanvas, hBrush, rcPaint.left, rcPaint.top, rcPaint.right,
                                 rcPaint.bottom);
                _brush_destroy(hBrush);
            }
        }
    }
    else {
        _canvas_clear(hCanvas, crBkg);
    }
    EX_PAINTSTRUCT ps{0};
    ps.hCanvas        = hCanvas;
    ps.dwState        = pObj->dwState_;
    ps.dwStyle        = pObj->dwStyle_;
    ps.dwStyleEx      = pObj->dwStyleEx_;
    ps.uWidth         = pObj->right_ - pObj->left_;
    ps.uHeight        = pObj->bottom_ - pObj->top_;
    ps.rcPaint.left   = pObj->left_;
    ps.rcPaint.top    = pObj->top_;
    ps.rcPaint.right  = pObj->right_;
    ps.rcPaint.bottom = pObj->bottom_;
    ps.rcText.left    = pObj->t_left_;
    ps.rcText.top     = pObj->t_top_;
    ps.rcText.right   = pObj->t_right_;
    ps.rcText.bottom  = pObj->t_bottom_;
    ps.hTheme         = pObj->base.hTheme_;
    ps.dwTextFormat   = pObj->dwTextFormat_;
    ps.dwOwnerData    = pObj->dwOwnerData_;

    if (!_obj_baseproc(_obj_gethwnd(pObj), pObj->base.hObj_, pObj, WM_ERASEBKGND, hCanvas,
                       (size_t)&ps)) {
        EX_BACKGROUNDIMAGEINFO* bkgimage = pObj->base.lpBackgroundImage_;
        if (bkgimage != 0) {
            _canvas_drawimagefrombkgimg(hCanvas, bkgimage);
        }
        if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_BLUR) == OBJECT_STYLE_EX_BLUR))//新增 有效果????
        {
            _canvas_blur(hCanvas, pObj->fBlur_, 0);
        }
    }
}

BOOL Ex_ObjDrawBackgroundProc(HEXOBJ hObj, HEXCANVAS hCanvas, RECT* lprcPaint)
{
    RECT   rcPaint = {0};
    obj_s* pObj    = nullptr;
    INT    nError  = 0;
    BOOL   ret     = FALSE;
    if (lprcPaint != 0) {
        RtlMoveMemory(&rcPaint, lprcPaint, 16);
        if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            _obj_drawbackground(pObj, hCanvas, rcPaint);
            ret = TRUE;
        }
    }
    return ret;
}

BOOL Ex_ObjBeginPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS)
{
    obj_s*    pObj    = nullptr;
    INT       nError  = 0;
    HEXCANVAS hCanvas = 0;
    RECT      rcPaint = {0};
    BOOL      ret     = FALSE;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (lpPS == 0) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            hCanvas        = pObj->canvas_obj_;
            RECT rcPaint = { 0 };
            RtlMoveMemory(&rcPaint, (RECT*)((size_t)pObj + offsetof(obj_s, left_)), 16);
            OffsetRect(&rcPaint, (-pObj->left_), (-pObj->top_)); 
 
            lpPS->uWidth         = rcPaint.right - rcPaint.left;
            lpPS->uHeight        = rcPaint.bottom - rcPaint.top;

            RtlMoveMemory(&lpPS->rcPaint, &rcPaint, 16);
            lpPS->rcText.right   = lpPS->uWidth;
            lpPS->rcText.bottom  = lpPS->uHeight;
            lpPS->rcText.left    = pObj->t_left_;
            lpPS->rcText.top     = pObj->t_top_;
            lpPS->rcText.right   = lpPS->rcText.right - pObj->t_right_;
            lpPS->rcText.bottom  = lpPS->rcText.bottom - pObj->t_bottom_;
            lpPS->hCanvas        = hCanvas;
            lpPS->hTheme         = pObj->base.hTheme_;
            lpPS->dwStyle        = pObj->dwStyle_;
            lpPS->dwStyleEx      = pObj->dwStyleEx_;
            lpPS->dwTextFormat   = pObj->dwTextFormat_;
            lpPS->dwState        = pObj->dwState_;
            lpPS->dwOwnerData    = pObj->dwOwnerData_;
            if (_canvas_begindraw(hCanvas)) {
                _canvas_cliprect(hCanvas, rcPaint.left, rcPaint.top, rcPaint.right,
                                 rcPaint.bottom);   // 必须CLIP
                if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BPAINTINGMSG)) {
                    _obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING,
                                  PAINT_PROGRESS_BEGIN, (size_t)lpPS);
                }
                _obj_drawbackground(pObj, hCanvas, rcPaint);
                if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BPAINTINGMSG)) {
                    _obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING,
                                  PAINT_PROGRESS_BKG, (size_t)lpPS);
                }
                ret = TRUE;
            }
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjEndPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS)
{
    obj_s*    pObj    = nullptr;
    INT       nError  = 0;
    HEXCANVAS hCanvas = 0;
    RECT      rcPaint = {0};
    BOOL      ret     = FALSE;
    LPVOID    hBrush  = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        hCanvas = lpPS->hCanvas;

        if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_CUSTOMDRAW) == OBJECT_STYLE_EX_CUSTOMDRAW)) {
            _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, hObj, pObj->id_, NM_CUSTOMDRAW, 0, (size_t)lpPS);
            if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BPAINTINGMSG)) {
                _obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING,
                              PAINT_PROGRESS_CUSTOMDRAW, (size_t)lpPS);
            }
        }
        if ((pObj->dwStyle_ & OBJECT_STYLE_BORDER) == OBJECT_STYLE_BORDER) {
            hBrush = _brush_create(_obj_getcolor(pObj, COLOR_EX_BORDER));
            if (hBrush) {
                HEXPATH hPath = NULL;
                _path_create(PATH_FLAG_DISABLESCALE, &hPath);
                _path_open(hPath);
                _path_beginfigure2(hPath, pObj->c_left_, pObj->c_top_ + pObj->radius_topleft_);
                _path_addroundedrect(hPath, pObj->c_left_ + 0.5f, pObj->c_top_ + 0.5f, pObj->c_right_ - 0.5f,
                                     pObj->c_bottom_ - 0.5f, pObj->radius_topleft_, pObj->radius_topright_,
                                     pObj->radius_bottomleft_, pObj->radius_bottomright_);
                _path_endfigure(hPath, TRUE);
                _path_close(hPath);
                _canvas_drawpath(hCanvas, hPath, hBrush, Ex_Scale(1.0f), D2D1_DASH_STYLE_SOLID);
                _path_destroy(hPath);
                _brush_destroy(hBrush);
            }
            if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BPAINTINGMSG)) {
                _obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, PAINT_PROGRESS_BORDER,
                              (size_t)lpPS);
            }
        }
        if (Flag_Query(ENGINE_FLAG_OBJECT_SHOWPOSTION)) {
            WCHAR wzPostion[60] = L"";
            WCHAR wstrLeft[12];
            WCHAR wstrTop[12];
            WCHAR wstrRight[12];
            WCHAR wstrBottom[12];
            _itow_s(pObj->w_left_, wstrLeft, 12, 10);
            _itow_s(pObj->w_top_, wstrTop, 12, 10);
            _itow_s(pObj->w_right_, wstrRight, 12, 10);
            _itow_s(pObj->w_bottom_, wstrBottom, 12, 10);
            wcscat_s(wzPostion, wstrLeft);
            wcscat_s(wzPostion, L",");
            wcscat_s(wzPostion, wstrTop);
            wcscat_s(wzPostion, L",");
            wcscat_s(wzPostion, wstrRight);
            wcscat_s(wzPostion, L",");
            wcscat_s(wzPostion, wstrBottom);
            HEXFONT F = _font_create();
            _canvas_drawtext(hCanvas, F /*pObj->hFont*/, ExARGB(0, 0, 0, 128), wzPostion, -1,
                             DT_LEFT | DT_TOP | DT_SINGLELINE, 0, 0, pObj->right_, pObj->bottom_);
            _font_destroy(F);
        }
        if (FLAGS_CHECK(pObj->base.dwFlags_, EOF_BPAINTINGMSG)) {
            _obj_baseproc(pObj->pWnd_->hWnd_, hObj, pObj, WM_EX_PAINTING, PAINT_PROGRESS_END,
                          (size_t)lpPS);
        }
        _canvas_resetclip(hCanvas);
        FLOAT fHue = pObj->fHUE_;
        if (fHue != 0) {
            _canvas_rotate_hue(hCanvas, fHue);
        }
        return _canvas_enddraw(hCanvas);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXOBJ Ex_ObjGetObj(HEXOBJ hObj, INT nCmd)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    HEXOBJ ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (nCmd == GW_CHILD) {
            ret = pObj->base.objChildFirst_;
        }
        else if (nCmd == GW_HWNDFIRST) {
            ret = pObj->objParent_;
            if (ret == 0) {
                wnd_s* pWnd = pObj->pWnd_;
                ret         = pWnd->base.objChildFirst_;
            }
            else {
                obj_s* pObj2 = nullptr;
                if (_handle_validate(ret, HT_OBJECT, (LPVOID*)&pObj2, &nError)) {
                    ret = pObj2->base.objChildFirst_;
                }
            }
        }
        else if (nCmd == GW_HWNDLAST) {
            ret = pObj->objParent_;
            if (ret == 0) {
                wnd_s* pWnd = pObj->pWnd_;
                ret         = pWnd->base.objChildLast_;
            }
            else {
                obj_s* pObj2 = nullptr;
                if (_handle_validate(ret, HT_OBJECT, (LPVOID*)&pObj2, &nError)) {
                    ret = pObj2->base.objChildLast_;
                }
            }
        }
        else if (nCmd == GW_HWNDNEXT) {
            ret = pObj->objNext_;
        }
        else if (nCmd == GW_HWNDPREV) {
            ret = pObj->objPrev_;
        }
        else if (nCmd == GW_OWNER) {
            wnd_s* pWnd = pObj->pWnd_;
            ret         = pWnd->base.hexdui_;
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

HEXOBJ _obj_getobjfromidorname(wnd_s* pWnd, INT idorname)
{
    size_t ret = 0;
    HashTable_Get(pWnd->hTableObjects_, idorname, &ret);
    return ret;
}

HEXOBJ Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, LPCWSTR lpName)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    HEXOBJ ret    = 0;
    wnd_s* pWnd   = nullptr;
    if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pWnd = pObj->pWnd_;
    }
    else if (!_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        return 0;
    }
    EXATOM atomName = Ex_Atom(lpName);
    if (atomName != 0) {
        ret = _obj_getobjfromidorname(pWnd, atomName);
    }
    return ret;
}

HEXOBJ Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, INT nID)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    wnd_s* pWnd   = nullptr;
    if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pWnd = pObj->pWnd_;
    }
    else if (!_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        return 0;
    }
    return _obj_getobjfromidorname(pWnd, nID);
}

HEXOBJ Ex_ObjFind(EXHANDLE hObjParent, HEXOBJ hObjChildAfter, LPCWSTR lpClassName, LPCWSTR lpTitle)
{
    obj_base* pObj   = nullptr;
    INT       nError = 0;
    HEXOBJ    ret    = 0;
    if (!_handle_validate(hObjParent, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (!_handle_validate(hObjParent, HT_DUI, (LPVOID*)&pObj, &nError)) {
            return 0;
        }
    }
    HEXOBJ objEntry  = pObj->objChildFirst_;
    BOOL   bStart    = (hObjChildAfter == 0);
    EXATOM atomClass = 0;
    if (lpClassName != 0) {
        if (HashTable_Get(g_Li.hTableClass, (size_t)lpClassName, (size_t*)&atomClass)) {
            if (atomClass != 0) {
                atomClass = (EXATOM)lpClassName;
            }
        }
        else {
            atomClass = Ex_Atom(lpClassName);
        }
    }
    BOOL   bBypass = FALSE;
    obj_s* pObj2   = nullptr;
    while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj2, &nError)) {
        bBypass = FALSE;
        if (bStart) {
            if (atomClass != 0 || lpTitle != 0) {
                if (atomClass != 0) {
                    if (pObj2->pCls_->atomName != atomClass) {
                        bBypass = TRUE;
                    }
                }

                if ((lpTitle != 0) && (bBypass == TRUE ? FALSE : TRUE)) {
                    if (lstrcmpW(pObj2->pstrTitle_, lpTitle) != 0) {
                        bBypass = TRUE;
                    }
                }
            }

            if ((bBypass == TRUE ? FALSE : TRUE)) {
                return objEntry;
            }
        }
        else {
            objEntry = pObj2->objNext_;
            bStart   = (objEntry == hObjChildAfter);
        }
        HEXOBJ sObj = pObj2->base.objChildFirst_;
        if (sObj != 0) {
            ret = Ex_ObjFind(objEntry, 0, lpClassName, lpTitle);
            if (ret != 0) {
                return ret;
            }
        }
        objEntry = pObj2->objNext_;
    }
    return 0;
}

BOOL Ex_ObjEnumChild(EXHANDLE hObjParent, LPVOID lpEnumFunc, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (IsBadCodePtr((FARPROC)lpEnumFunc) || hObjParent == 0) {
        return FALSE;
    }
    obj_base* pObj   = nullptr;
    INT       nError = 0;
    if (!_handle_validate(hObjParent, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (!_handle_validate(hObjParent, HT_DUI, (LPVOID*)&pObj, &nError)) {
            return FALSE;
        }
    }
    HEXOBJ objEntry = pObj->objChildFirst_;
    obj_s* pObj2    = nullptr;
    while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj2, &nError)) {
        if (((EnumPROC)lpEnumFunc)(objEntry, lParam)) {
            return FALSE;
        }
        HEXOBJ sObj = pObj2->base.objChildFirst_;
        if (sObj != 0) {
            ret = Ex_ObjEnumChild(objEntry, lpEnumFunc, lParam);
            if (ret == FALSE) return FALSE;
        }
        objEntry = pObj2->objNext_;
    }
    return TRUE;
}

BOOL Ex_ObjGetBackgroundImage(EXHANDLE handle, EX_BACKGROUNDIMAGEINFO* lpBackgroundImage)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_wnd_getfromhandle(handle, NULL, NULL, &pObj, NULL, &nError)) {
        if (IsBadWritePtr(lpBackgroundImage, sizeof(EX_BACKGROUNDIMAGEINFO))) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            if (!IsBadReadPtr(pObj->base.lpBackgroundImage_, sizeof(EX_BACKGROUNDIMAGEINFO))) {
                RtlMoveMemory(lpBackgroundImage, pObj->base.lpBackgroundImage_,
                              sizeof(EX_BACKGROUNDIMAGEINFO));
            }
            else {
                nError = ERROR_EX_MEMORY_BADPTR;
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void CALLBACK _obj_backgroundimage_timer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    obj_base*               pObj = (obj_base*)(idEvent - TIMER_BKG);
    EX_BACKGROUNDIMAGEINFO* lpBI = pObj->lpBackgroundImage_;
    if (lpBI != 0) {
        INT* pDelay = (INT*)lpBI->lpDelay;
        if (pDelay != 0) {
            INT iCur = ((EX_BACKGROUNDIMAGEINFO*)lpBI)->curFrame + 1;
            INT iMax = ((EX_BACKGROUNDIMAGEINFO*)lpBI)->maxFrame - 1;
            if (iCur > iMax) {
                iCur = 0;
            }

            _img_selectactiveframe(((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage, iCur);
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->curFrame = iCur;

            if (((pObj->dwFlags_ & EOF_OBJECT) == EOF_OBJECT)) {
                INT nError = 0;
                _obj_invalidaterect((obj_s*)pObj, 0, &nError);
            }
            else {
                _wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
            }
            //待定注释AAAAAAAAAA
            //UpdateWindow(hWnd);
        }
    }
}

BOOL _obj_backgroundimage_set(HWND hWnd, obj_s* pObj, LPVOID lpImage, INT dwImageLen, INT x, INT y,
                              INT dwRepeat, RECT* lpGrid, INT dwFlags, INT dwAlpha, RECT* lpRcSrc, EX_RECTF* lpRCFDst, INT* nError)
{
    if (dwImageLen == 0) {
        _obj_backgroundimage_clear(hWnd, (obj_base*)pObj);
        return TRUE;
    }
    else {
        _obj_backgroundimage_clear(hWnd, (obj_base*)pObj);
        HEXIMAGE hImg = 0;
        _img_createfrommemory(lpImage, dwImageLen, &hImg);
        if (hImg != 0) {
            LPVOID lpBI = _struct_createfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_),
                                                 sizeof(EX_BACKGROUNDIMAGEINFO), nError);
            if (lpBI != 0) {
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwFlags  = dwFlags;
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage   = hImg;
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->x        = x;
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->y        = y;
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwRepeat = dwRepeat;
                ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwAlpha  = dwAlpha;
                if (lpGrid != 0) {
                    LPVOID lpDelay = _struct_createfromaddr(
                        lpBI, offsetof(EX_BACKGROUNDIMAGEINFO, lpGrid), 16, nError);
                    if (lpDelay != 0) {
                        RtlMoveMemory(lpDelay, lpGrid, 16);
                    }
                }
                if (lpRcSrc != 0)
                {
                    LPVOID lpRcSrc_ = _struct_createfromaddr(lpBI, offsetof(EX_BACKGROUNDIMAGEINFO, lpRcSrc), sizeof(RECT), nError);
                    if (lpRcSrc_ != 0)
                    {
                        RtlMoveMemory(lpRcSrc_, lpRcSrc, 16);
                    }
                }
                if (lpRCFDst != 0)
                {
                    LPVOID lpRCFDst_ = _struct_createfromaddr(lpBI, offsetof(EX_BACKGROUNDIMAGEINFO, lpRCFDst), sizeof(EX_RECTF), nError);
                    if (lpRCFDst_ != 0)
                    {
                        RtlMoveMemory(lpRCFDst_, lpRCFDst, 16);
                    }
                }
                INT nFrames = 0;
                _img_getframecount(hImg, &nFrames);

                if (nFrames > 1) {
                    INT* lpDelay2 = (INT*)Ex_MemAlloc(nFrames * sizeof(INT));
                    if (_img_getframedelay(hImg, lpDelay2, nFrames)) {
                        ((EX_BACKGROUNDIMAGEINFO*)lpBI)->lpDelay  = lpDelay2;
                        ((EX_BACKGROUNDIMAGEINFO*)lpBI)->maxFrame = nFrames;
                        if ((dwFlags & BACKGROUND_FLAG_PLAYIMAGE) == BACKGROUND_FLAG_PLAYIMAGE) {
                            pObj->base.uElapse_ = lpDelay2[0] * 10;
                            SetTimer(hWnd, ((size_t)pObj + TIMER_BKG), pObj->base.uElapse_,
                                     _obj_backgroundimage_timer);
                        }
                    }
                    else {
                        Ex_MemFree(lpDelay2);
                    }
                }
                return TRUE;
            }
            _img_destroy(hImg);
        }
        else
        {
            _svg_create(Ex_U2A3((const char*)lpImage, dwImageLen).c_str(), &hImg);
            if (hImg)
            {
                LPVOID lpBI = _struct_createfromaddr((obj_base*)pObj, offsetof(obj_base, lpBackgroundImage_), sizeof(EX_BACKGROUNDIMAGEINFO), nError);
                if (lpBI != 0)
                {
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwFlags = dwFlags;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage = hImg;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->x = x;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->y = y;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwRepeat = dwRepeat;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwAlpha = dwAlpha;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL Ex_ObjDestroyBackground(EXHANDLE handle)
{
    HWND   hWnd   = 0;
    obj_s* pObj   = nullptr;
    BOOL   bObj   = FALSE;
    INT    nError = 0;
    if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError)) {
        _obj_backgroundimage_clear(hWnd, (obj_base*)pObj);
        return TRUE;
    }
    return FALSE;
}

BOOL Ex_ObjSetBackgroundImage(EXHANDLE handle, LPVOID lpImage, size_t dwImageLen, INT x, INT y,
                              DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha,
                              BOOL fUpdate, RECT* lpRcSrc, EX_RECTF* lpRCFDst)
{
    HWND   hWnd   = 0;
    obj_s* pObj   = nullptr;
    BOOL   bObj   = FALSE;
    INT    nError = 0;
    if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError)) {
        if (_obj_backgroundimage_set(hWnd, pObj, lpImage, dwImageLen, x, y, dwRepeat, lpGrid,
                                     dwFlags, dwAlpha, lpRcSrc, lpRCFDst, &nError)) {
            if (bObj) {
                nError = 0;
                _obj_invalidaterect(pObj, 0, &nError);
            }
            else {
                ((wnd_s*)pObj)->dwStyle_ = ((wnd_s*)pObj)->dwStyle_ | WINDOW_STYLE_NOINHERITBKG;
                _wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
            }
            if (fUpdate) {
                UpdateWindow(hWnd);
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _obj_backgroundimage_frames(HWND hWnd, obj_s* pObj, BOOL bResetFrame, BOOL bPlayFrames,
                                 BOOL fUpdate, INT* nError)
{
    EX_BACKGROUNDIMAGEINFO* lpBI = pObj->base.lpBackgroundImage_;
    if (lpBI != 0) {
        HEXIMAGE hImg = lpBI->hImage;
        if (bPlayFrames) {
            lpBI->dwFlags = lpBI->dwFlags | BACKGROUND_FLAG_PLAYIMAGE;
        }
        else {
            lpBI->dwFlags = lpBI->dwFlags - (lpBI->dwFlags & BACKGROUND_FLAG_PLAYIMAGE);
        }
        INT framecount = 0;
        _img_getframecount(hImg, &framecount);
        if (framecount > 1) {
            if (bResetFrame) {
                _img_selectactiveframe(hImg, 0);
            }

            if (((pObj->base.dwFlags_ & EOF_OBJECT) == EOF_OBJECT)) {
                _obj_invalidaterect(pObj, 0, nError);
            }
            else {
                _wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
            }
            if (fUpdate) {
                UpdateWindow(hWnd);
            }
        }
    }
}

BOOL Ex_ObjSetBackgroundPlayState(EXHANDLE handle, BOOL fPlayFrames, BOOL fResetFrame, BOOL fUpdate)
{
    HWND   hWnd   = 0;
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, NULL, &nError)) {
        _obj_backgroundimage_frames(hWnd, pObj, fResetFrame, fPlayFrames, fUpdate, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void CALLBACK _obj_timer_object(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    obj_s* pObj = (obj_s*)(idEvent - TIMER_OBJECT);
    if (((pObj->dwStyle_ & OBJECT_STYLE_VISIBLE) == OBJECT_STYLE_VISIBLE))
    {
        _obj_baseproc(hWnd, pObj->base.hObj_, pObj, WM_TIMER, idEvent, dwTime);
    }
    
}

void CALLBACK _obj_timer_object2(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    obj_s* pObj = (obj_s*)(dwUser - TIMER_OBJECT);
    auto hWnd = pObj->pWnd_->hWnd_;
    pObj->uTimerID_ = uTimerID;
    _obj_baseproc(hWnd, pObj->base.hObj_, pObj, WM_EX_TIMER_EVENT, dwUser, uTimerID);
}

INT Ex_ObjSetTimer(HEXOBJ hObj, INT uElapse)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    INT    ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        wnd_s* pWnd = pObj->pWnd_;
        ret = SetTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT, uElapse, _obj_timer_object);
    }
  
    Ex_SetLastError(nError);
    return ret;
}

INT Ex_ObjSetTimer2(HEXOBJ hObj, INT uElapse)
{
    obj_s* pObj = nullptr;
    INT    nError = 0;
    INT    ret = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = timeSetEvent(
            uElapse,                    
            1,                     // 1ms 精度
            _obj_timer_object2,              // 回调函数
            (DWORD_PTR)((size_t)pObj + TIMER_OBJECT),       
            TIME_PERIODIC           // 周期定时器
        );
    }
    Ex_SetLastError(nError);
    return ret;
}

BOOL Ex_ObjKillTimer(HEXOBJ hObj)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        wnd_s* pWnd = pObj->pWnd_;
        KillTimer(pWnd->hWnd_, (size_t)pObj + TIMER_OBJECT);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjKillTimer2(HEXOBJ hObj)
{
    obj_s* pObj = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        timeKillEvent(pObj->uTimerID_);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void _obj_setradius(HEXOBJ hObj, obj_s* pObj, FLOAT topleft, FLOAT topright, FLOAT bottomright,
                    FLOAT bottomleft, BOOL fUpdate, INT* nError)
{
    if (topleft == 0 && topright == 0 && bottomleft == 0 && bottomright == 0) {
        pObj->base.dwFlags_ = pObj->base.dwFlags_ - (pObj->base.dwFlags_ & (EOF_BPATH | EOF_BPATHBYROUNDEDRECT));
        RtlZeroMemory((LPVOID)((size_t)pObj + offsetof(obj_s, radius_topleft_)), 16);
    }
    else {
        if (g_Li.DpiX > 1) {
            topleft     = topleft * g_Li.DpiX;
            topright    = topright * g_Li.DpiX;
            bottomright = bottomright * g_Li.DpiX;
            bottomleft  = bottomleft * g_Li.DpiX;
        }
        pObj->radius_topleft_     = topleft;
        pObj->radius_topright_    = topright;
        pObj->radius_bottomright_ = bottomright;
        pObj->radius_bottomleft_  = bottomleft;
        pObj->base.dwFlags_ |= (EOF_BPATH | EOF_BPATHBYROUNDEDRECT);

        INT flags = SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS;
        if (fUpdate) {
            flags |= SWP_EX_UPDATEOBJECT;
        }
        _obj_setpos_org(pObj, hObj, 0, OBJECT_POSITION_DEFAULT, OBJECT_POSITION_DEFAULT,
                        OBJECT_POSITION_DEFAULT, OBJECT_POSITION_DEFAULT, flags, nError);
    }
}

BOOL Ex_ObjSetRadius(HEXOBJ hObj, FLOAT topleft, FLOAT topright, FLOAT bottomright,
                     FLOAT bottomleft, BOOL fUpdate)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_setradius(hObj, pObj, topleft, topright, bottomright, bottomleft, fUpdate, &nError);
    }
    return nError == 0;
}

BOOL Ex_ObjSetBlur(HEXOBJ hObj, FLOAT fDeviation, BOOL bRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    wnd_s* pWnd = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pObj->dwStyleEx_ = pObj->dwStyleEx_ | (OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_BLUR);
        pObj->fBlur_     = fDeviation;
        wnd_s* pWnd      = pObj->pWnd_;
        pWnd->base.dwFlags_   = pWnd->base.dwFlags_ | EWF_BCOMPOSITEDCHECK;
        if (bRedraw) {
            nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
        }
    }
    else if (_handle_validate(hObj, HT_DUI, (LPVOID*)&pWnd, &nError))
    {
        pWnd->fBlur_ = fDeviation;
        if (bRedraw)
        {
            pWnd->base.dwFlags_ |= EWF_BREDRAWBACKGROUND;
            _wnd_redraw_bkg(pWnd->hWnd_, pWnd, 0, TRUE, FALSE);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjSetTextFormat(HEXOBJ hObj, DWORD dwTextFormat, BOOL bRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pObj->dwTextFormat_ = dwTextFormat;
        if (bRedraw) {
            nError = 0;
            _obj_invalidaterect(pObj, 0, &nError);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjTooltipsSetText(HEXOBJ hObj, LPCWSTR lpString)
{
    obj_s*  pObj   = nullptr;
    INT     nError = 0;
    LPCWSTR lpNew  = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (lpString != 0) {
            lpNew = StrDupW(lpString);
        }
        LPCWSTR lpTips  = pObj->pstrTips_;
        pObj->pstrTips_ = lpNew;
        Ex_MemFree((LPVOID)lpTips);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

void CALLBACK _obj_tooltips_pop_func(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    KillTimer(hWnd, idEvent);
    wnd_s* pWnd    = (wnd_s*)(idEvent - TIMER_TOOLTIPS_POP);
    pWnd->base.dwFlags_ = pWnd->base.dwFlags_ - (pWnd->base.dwFlags_ & EWF_BTOOLTIPSPOPUP);
    INT offset;

    if (((pWnd->base.dwFlags_ & EWF_BTOOLTIPSTRACKPOSTION) == EWF_BTOOLTIPSTRACKPOSTION)) {
        offset = offsetof(wnd_s, ti_track_);
        pWnd->ti_track_->lpszText_ = 0;
        SendMessageW(hWnd, 1081, 0, (LPARAM)pWnd->ti_track_);
    }
    else {
        offset = offsetof(wnd_s, ti_auto_);
        pWnd->ti_auto_->lpszText_ = 0;
        SendMessageW(hWnd, 1081, 0, (LPARAM)pWnd->ti_auto_);
    }
    SendMessageW(hWnd, 1041, 0, __get(pWnd, offset));   // TTM_TRACKACTIVATE
}

void CALLBACK _obj_tooltips_popup_func(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if (uMsg == WM_TIMER) {
        KillTimer(hWnd, idEvent);
    }

    wnd_s* pWnd = (wnd_s*)(idEvent - TIMER_TOOLTIPS_POPUP);
    INT    offset;

    if (!((pWnd->base.dwFlags_ & EWF_BTOOLTIPSPOPUP) == EWF_BTOOLTIPSPOPUP)) {
        pWnd->base.dwFlags_ = pWnd->base.dwFlags_ | EWF_BTOOLTIPSPOPUP;

        if (((pWnd->base.dwFlags_ & EWF_BTOOLTIPSTRACKPOSTION) == EWF_BTOOLTIPSTRACKPOSTION)) {
            offset = offsetof(wnd_s, ti_track_);
        }
        else {
            offset = offsetof(wnd_s, ti_auto_);
        }
        SendMessageW(hWnd, 1041, 1, __get(pWnd, offset));   // TTM_TRACKACTIVATE
        offset = SendMessageW(hWnd, 1045, 2, 0);
        if (offset != 0) {
            SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POP, offset, _obj_tooltips_pop_func);
        }
    }
}

void _obj_tooltips_popup(wnd_s* pWnd, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime,
                         INT nIcon, BOOL fShow)
{
    HWND  hWnd = pWnd->hWndTips_;
    ti_s* lpTI = nullptr;
    INT postion = MAKELONG(x, y);
    if (lpText == 0) {
        KillTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP);
        if (postion == -1)
            lpTI = pWnd->ti_auto_;
        else
            lpTI = pWnd->ti_track_;
        lpTI->lpszText_ = lpText;
        SendMessageW(hWnd, 1081, 0, (LPARAM)lpTI);        //TTM_UPDATETIPTEXTW
        SendMessageW(hWnd, 1041, 0, (LPARAM)lpTI); //TTM_TRACKACTIVATE
        _obj_tooltips_pop_func(hWnd, WM_TIMER, (size_t)pWnd + TIMER_TOOLTIPS_POP, 0);
    }
    else {
        if (postion == -1) {
            pWnd->base.dwFlags_ = pWnd->base.dwFlags_ - (pWnd->base.dwFlags_ & EWF_BTOOLTIPSTRACKPOSTION);
            lpTI           = pWnd->ti_auto_;
        }
        else {
            SendMessageW(hWnd, 1042, 0, postion);   // TTM_TRACKPOSITION
            lpTI           = pWnd->ti_track_;
            pWnd->base.dwFlags_ = pWnd->base.dwFlags_ | EWF_BTOOLTIPSTRACKPOSTION;
        }
        lpTI->lpszText_ = lpText;
        SendMessageW(hWnd, 1057, nIcon, (LPARAM)lpTitle);   // TTM_SETTITLEW
        SendMessageW(hWnd, 1027, 2, dwTime);                // TTM_SETDELAYTIME,TTDT_AUTOPOP
        SendMessageW(hWnd, 1081, 0, (LPARAM)lpTI);          // TTM_UPDATETIPTEXTW
        if (fShow) {
            _obj_tooltips_popup_func(hWnd, 0, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, 0);
        }
        else {
            SetTimer(hWnd, (size_t)pWnd + TIMER_TOOLTIPS_POPUP, g_Li.dwClickTime,
                     _obj_tooltips_popup_func);
        }
    }
}

BOOL Ex_ObjTooltipsPop(HEXOBJ hObj, LPCWSTR lpText)
{
    return Ex_ObjTooltipsPopEx(hObj, 0, lpText, -1, -1, -1, 0, FALSE);
}

BOOL Ex_ObjTooltipsPopEx(HEXOBJ hObj, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime,
                         INT nIcon, BOOL fShow)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_tooltips_popup(pObj->pWnd_, lpTitle, lpText, x, y, dwTime, nIcon, fShow);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

HEXOBJ Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj)
{
    obj_s* pObj   = nullptr;
    wnd_s* pWnd   = nullptr;
    INT    nError = 0;
    HEXOBJ ret    = 0;
    if (_handle_validate(hExDuiOrhObj, HT_DUI, (LPVOID*)&pWnd, &nError)) {
        ret = pWnd->objFocus_;
    }
    else {
        if (_handle_validate(hExDuiOrhObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            wnd_s* pWnd2 = pObj->pWnd_;
            ret          = pWnd2->objFocus_;
        }
    }
    return ret;
}

size_t Ex_ObjGetProp(HEXOBJ hObj, size_t dwKey)
{
    obj_s* pObj    = nullptr;
    INT    nError  = 0;
    size_t dwValue = NULL;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        INT           nList = pObj->nPropCount_;
        EX_HASHTABLE* pList = pObj->pPropListEntry_;
        if (pList != 0) {
            if (nList == -1) {
                HashTable_Get(pList, dwKey, &dwValue);
            }
            else if (dwKey >= 0 && dwKey < nList) {
                dwValue = __get(pList, dwKey * sizeof(LPVOID));
            }
        }
    }
    return dwValue;
}

size_t Ex_ObjSetProp(HEXOBJ hObj, size_t dwKey, size_t dwValue)
{
    obj_s* pObj       = NULL;
    INT    nError     = 0;
    size_t dwValueOld = NULL;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        INT           nList = pObj->nPropCount_;
        EX_HASHTABLE* pList = pObj->pPropListEntry_;
        if (pList != 0) {
            if (nList == -1) {
                HashTable_Get(pList, dwKey, &dwValueOld);
                HashTable_Set(pList, dwKey, dwValue);
            }
            else if (dwKey >= 0 && dwKey < nList) {
                dwValueOld = __get(pList, dwKey * sizeof(LPVOID));
                __set(pList, dwKey * sizeof(LPVOID), dwValue);
            }
        }
    }
    return dwValueOld;
}

size_t Ex_ObjRemoveProp(HEXOBJ hObj, size_t dwKey)
{
    obj_s* pObj    = NULL;
    INT    nError  = 0;
    size_t dwValue = NULL;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        INT           nList = pObj->nPropCount_;
        EX_HASHTABLE* pList = pObj->pPropListEntry_;
        if (pList != 0) {
            if (nList == -1) {
                if (HashTable_Get(pList, dwKey, &dwValue)) {
                    HashTable_Remove(pList, dwKey);
                }
            }
            else if (dwKey >= 0 && dwKey < nList) {
                dwValue = __get(pList, dwKey * sizeof(LPVOID));
                __set(pList, dwKey * sizeof(LPVOID), 0);
            }
        }
    }
    return dwValue;
}

BOOL Ex_ObjInitPropList(HEXOBJ hObj, INT nPropCount)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        INT           nList = pObj->nPropCount_;
        EX_HASHTABLE* pList = pObj->pPropListEntry_;
        if (nList == -1) {
            HashTable_Destroy(pList);
        }
        else if (nList > 0) {
            Ex_MemFree(pList);
        }

        if (nPropCount == -1) {
            pList = HashTable_Create(257, 0);
        }
        else if (nPropCount > 0) {
            pList = (EX_HASHTABLE*)Ex_MemAlloc(nPropCount * sizeof(LPVOID));
        }
        else {
            pList = 0;
        }
        pObj->nPropCount_     = nPropCount;
        pObj->pPropListEntry_ = pList;
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

INT Ex_ObjEnumProps(HEXOBJ hObj, EnumPropsPROC lpfnCbk, size_t param)
{
    obj_s* pObj;
    INT    nError = 0;
    INT    nList  = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        nList               = pObj->nPropCount_;
        EX_HASHTABLE* pList = pObj->pPropListEntry_;
        if (pList != 0) {
            if (nList == -1) {
                std::vector<size_t> aKey;
                std::vector<size_t> aValue;
                nList = HashTable_GetAllKeysAndValues(pList, aKey, aValue);
                if (lpfnCbk != 0) {
                    for (INT i = 0; i < aKey.size(); i++) {
                        if (lpfnCbk(hObj, aKey[i], aValue[i], param) != 0) {
                            break;
                        }
                    }
                }
            }
            else {
                for (INT i = 0; i < nList; i++) {
                    if (lpfnCbk(hObj, i, __get(pList, i * sizeof(size_t)), param) != 0) {
                        break;
                    }
                }
            }
        }
    }
    return nList;
}

BOOL Ex_ObjMove(HEXOBJ hObj, INT x, INT y, INT width, INT height, BOOL bRepaint)
{
  INT flags = SWP_NOZORDER | SWP_NOACTIVATE |
      SWP_ASYNCWINDOWPOS | SWP_EX_NODPISCALE;
    if (x == OBJECT_POSITION_DEFAULT && y == OBJECT_POSITION_DEFAULT) {
        flags = flags | SWP_NOMOVE;
    }
    if (width == OBJECT_POSITION_DEFAULT && height == OBJECT_POSITION_DEFAULT) {
        flags = flags | SWP_NOSIZE;
    }
    if (bRepaint) {
        flags = flags | SWP_EX_UPDATEOBJECT;
    }
    
    return Ex_ObjSetPos(hObj, 0, x, y, width, height, flags);
}

BOOL Ex_ObjScrollGetPAGE(HEXOBJ hObj, INT nBar, INT* pPAGE)
{
    obj_s* pObj = nullptr;
    INT nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
    {
        HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
        obj_s* pSB = nullptr;
        if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError))
        {
            si_s* psi = (si_s*)pSB->dwOwnerData_;
            if (psi == 0)
            {
                nError = ERROR_EX_INVALID_OBJECT;
            }
            else
            {
                if (pPAGE)
                {
                    *pPAGE = psi->nPage_;
                }
            }
        }
    }
    //Ex_SetLastError(nError);
    return nError == 0;
}

void _obj_setuistate(obj_s* pObj, DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw,
                     INT* nError)
{

    if (fRemove) {
        pObj->dwState_ = pObj->dwState_ - (pObj->dwState_ & dwState);
    }
    else {
        pObj->dwState_ = pObj->dwState_ | dwState;
    }
    if (fRedraw) {
        _obj_invalidaterect(pObj, lprcRedraw, nError);
    }
}

BOOL Ex_ObjSetUIState(HEXOBJ hObj, DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        _obj_setuistate(pObj, dwState, fRemove, lprcRedraw, fRedraw, &nError);
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

INT Ex_ObjGetUIState(HEXOBJ hObj)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    INT    ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = pObj->dwState_;
    }
    Ex_SetLastError(nError);
    return ret;
}

LRESULT Ex_ObjDefProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    obj_s*  pObj   = nullptr;
    INT     nError = 0;
    LRESULT ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        pObj->base.dwFlags_ = pObj->base.dwFlags_ - (pObj->base.dwFlags_ & EOF_BUSERPROCESSESED);
        if (uMsg == WM_NCHITTEST) {
            _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_NCHITTEST, wParam, lParam);
            return HTCLIENT;
        }
        else if (uMsg == WM_SETCURSOR) {
            HCURSOR tmp = pObj->hCursor_;
            if (tmp != 0) {
                wnd_s* pWnd = pObj->pWnd_;
                if (pWnd->dwHitCode_ == HTCLIENT) {
                    return (size_t)SetCursor(pObj->hCursor_);
                }
            }
        }
        else if (uMsg == WM_GETTEXT) {
            size_t tmp = NULL;
            if (!IsBadWritePtr((LPVOID)lParam, wParam)) {
                tmp = lstrlenW(pObj->pstrTitle_) * 2 + 2;
                if (tmp > wParam) {
                    tmp = wParam;
                }
                RtlMoveMemory((LPVOID)lParam, pObj->pstrTitle_, tmp);
            }
            return tmp;
        }
        else if (uMsg == WM_GETTEXTLENGTH) {
            return lstrlenW(pObj->pstrTitle_);
        }
        else if (uMsg == WM_SETTEXT) {
            if (pObj->pstrTitle_ != (LPCWSTR)lParam) {
                if (pObj->pstrTitle_) {
                    Ex_MemFree((LPVOID)pObj->pstrTitle_);
                }
                if ((LPCWSTR)lParam != L"") {
                    pObj->pstrTitle_ = StrDupW((LPCWSTR)lParam);
                }

                if (((pObj->dwStyleEx_ & OBJECT_STYLE_EX_AUTOSIZE) == OBJECT_STYLE_EX_AUTOSIZE)) {
                    pObj->base.dwFlags_ = pObj->base.dwFlags_ - (pObj->base.dwFlags_ & EOF_BAUTOSIZED);
                    _obj_setpos_org(pObj, hObj, 0, 0, 0, 1, 1, SWP_NOMOVE | SWP_NOZORDER, &nError);
                }
                else {
                    if (wParam != 0) {
                        nError = 0;
                        _obj_invalidaterect(pObj, 0, &nError);
                    }
                }
            }
            return 1;
        }
        else if (uMsg == WM_TIMER) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_TIMER, wParam, lParam);
        }
        else if (uMsg == WM_NOTIFY)
        {
            return 0;
        }
        else if (uMsg == WM_ENABLE) {
            _obj_update(pObj, FALSE);
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_ENABLE, wParam, lParam);
        }
        else if (uMsg == WM_SHOWWINDOW) {
            _obj_update(pObj, FALSE);
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_SHOW, wParam, lParam);
        }
        else if (uMsg == WM_MOUSEHOVER) {
            if (pObj->pstrTips_)
            {
                _obj_tooltips_popup(pObj->pWnd_, 0, pObj->pstrTips_, -1, -1, -1, 0, FALSE);
            }
            return 0;
        }
        else if (uMsg == WM_MOUSELEAVE) {
            _obj_tooltips_popup(pObj->pWnd_, 0, 0, -1, -1, -1, 0, FALSE);
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_LEAVE, wParam, lParam);
        }
        else if (uMsg == WM_SETFOCUS) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_SETFOCUS, wParam, lParam);
        }
        else if (uMsg == WM_KILLFOCUS) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_KILLFOCUS, wParam, lParam);
        }
        else if (uMsg == WM_LBUTTONDOWN) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_LDOWN, wParam, lParam);
        }
        else if (uMsg == WM_LBUTTONUP) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_LUP, wParam, lParam);
        }
        else if (uMsg == WM_RBUTTONDOWN) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_RDOWN, wParam, lParam);
        }
        else if (uMsg == WM_RBUTTONUP) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_RUP, wParam, lParam);
        }
        else if (uMsg == WM_EX_LCLICK) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CLICK, wParam, lParam);
        }
        else if (uMsg == WM_EX_RCLICK) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_RCLICK, wParam, lParam);
        }
        else if (uMsg == WM_LBUTTONDBLCLK) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_DBLCLK, wParam, lParam);
        }
        else if (uMsg == WM_RBUTTONDBLCLK) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_RDBLCLK, wParam, lParam);
        }
        else if (uMsg == WM_KEYDOWN) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_KEYDOWN, wParam, lParam);
        }
        else if (uMsg == WM_CHAR) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CHAR, wParam, lParam);
        }
        else if (uMsg == WM_COMMAND)
        {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_COMMAND, wParam, lParam);
        }
        else if (uMsg == WM_SIZE) {
            return  _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_SIZE, wParam, lParam);
        }
        else if (uMsg == WM_MOVE) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_MOVE, wParam, lParam);
        }
        else if (uMsg == WM_CREATE) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CREATE, wParam, lParam);
        }
        else if (uMsg == WM_DESTROY) {
            _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_DESTROY, wParam, lParam);
            _obj_destroy(hObj, pObj, 0);
            return 0;
        }
        else if (uMsg == WM_SETFONT) {
            if (_obj_setfont(pObj, wParam, lParam != 0)) {
                _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_FONTCHANGED, wParam, lParam);
            }
            else {
                _font_destroy(wParam);
            }
            return 0;
        }
        else if (uMsg == WM_GETFONT) {
            return pObj->hFont_;
        }
        else if (uMsg == WM_MOUSEWHEEL) {
            EX_CLASSINFO* pCls   = pObj->pCls_;
            SHORT         zDelta = (SHORT)HIWORD(wParam);
            if (pCls->atomName == ATOM_SCROLLBAR) {
                _scrollbar_parentnotify(hWnd, pObj, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, hObj, 0,
                                        TRUE);
                return 1;
            }
            else if (((pObj->dwStyle_ & OBJECT_STYLE_VSCROLL) == OBJECT_STYLE_VSCROLL)) {
                _obj_baseproc(hWnd, hObj, pObj, WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN,
                              0);
                return 1;
            }
            else if (((pObj->dwStyle_ & OBJECT_STYLE_HSCROLL) == OBJECT_STYLE_HSCROLL)) {
                _obj_baseproc(hWnd, hObj, pObj, WM_HSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN,
                              0);
                return 1;
            }
            return 0;
        }
        else if (uMsg == WM_EX_EASING) {
            return _obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_EASING, wParam, lParam);
        }
    }
    return ret;
}

BOOL Ex_ObjScrollGetInfo(HEXOBJ hObj, INT nBar, INT* lpnMin, INT* lpnMax, INT* lpnPos,
                         INT* lpnTrackPos)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
        obj_s* pSB = nullptr;
        if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError)) {
            si_s* psi = (si_s*)_obj_pOwner(pSB);
            if (psi == 0) {
                nError = ERROR_EX_INVALID_OBJECT;
            }
            else {
                if (lpnMin) {
                    *lpnMin = psi->nMin_;
                }
                if (lpnMax) {
                    *lpnMax = psi->nMax_;
                }
                if (lpnPos) {
                    *lpnPos = psi->nPos_;
                }
                if (lpnTrackPos) {
                    *lpnTrackPos = psi->nTrackPos_;
                }
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

INT Ex_ObjScrollGetPos(HEXOBJ hObj, INT nBar)
{
    INT ret = 0;
    Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, &ret, 0);
    return ret;
}

INT Ex_ObjScrollSetPos(HEXOBJ hObj, INT nBar, INT nPos, BOOL bRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    INT    ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
        obj_s* pSB = nullptr;
        if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError)) {
            ret = _scrollbar_realsetinfo(_obj_gethwnd(pSB), hSB, pSB, SIF_POS, 0, 0, 0, nPos,
                                         bRedraw);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

INT Ex_ObjScrollSetInfo(HEXOBJ hObj, INT nBar, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos,
                        BOOL bRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    INT    ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
        obj_s* pSB = nullptr;
        if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError)) {
            ret = _scrollbar_realsetinfo(_obj_gethwnd(pSB), hSB, pSB, Mask, nMin, nMax, nPage, nPos,
                                         bRedraw);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

INT Ex_ObjScrollSetRange(HEXOBJ hObj, INT nBar, INT nMin, INT nMax, BOOL bRedraw)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    INT    ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        HEXOBJ hSB = _scrollbar_getscroll(pObj, nBar);
        obj_s* pSB = nullptr;
        if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError)) {
            ret = _scrollbar_realsetinfo(_obj_gethwnd(pSB), hSB, pSB, SIF_RANGE, nMin, nMax, 0, 0,
                                         bRedraw);
        }
    }
    Ex_SetLastError(nError);
    return ret;
}

HEXOBJ Ex_ObjScrollGetControl(HEXOBJ hObj, INT nBar)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    HEXOBJ ret    = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        ret = _scrollbar_getscroll(pObj, nBar);
    }
    Ex_SetLastError(nError);
    return ret;
}

INT Ex_ObjScrollGetTrackPos(HEXOBJ hObj, INT nBar)
{
    INT ret = 0;
    Ex_ObjScrollGetInfo(hObj, nBar, 0, 0, 0, &ret);
    return ret;
}

BOOL Ex_ObjScrollGetRange(HEXOBJ hObj, INT nBar, INT* lpnMinPos, INT* lpnMaxPos)
{
    return Ex_ObjScrollGetInfo(hObj, nBar, lpnMinPos, lpnMaxPos, 0, 0);
}

void _sb_show(HEXOBJ hSB, BOOL fShow)
{
    obj_s* pSB    = nullptr;
    INT    nError = 0;
    if (_handle_validate(hSB, HT_OBJECT, (LPVOID*)&pSB, &nError)) {
        HWND hWnd = _obj_gethwnd(pSB);
        _obj_visable(hWnd, hSB, pSB, fShow);
        _obj_scroll_repostion(hWnd, pSB->objParent_, FALSE);
    }
}

BOOL Ex_ObjScrollShow(HEXOBJ hObj, INT nBar, BOOL fShow)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (nBar == SCROLLBAR_TYPE_BOTH) {
            _sb_show(_scrollbar_getscroll(pObj, SCROLLBAR_TYPE_VERT), fShow);
            _sb_show(_scrollbar_getscroll(pObj, SCROLLBAR_TYPE_HORZ), fShow);
        }
        else {
            _sb_show(_scrollbar_getscroll(pObj, nBar), fShow);
        }
    }
    return nError == 0;
}

BOOL Ex_ObjScrollEnable(HEXOBJ hObj, INT wSB, INT wArrows)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (wSB == SCROLLBAR_TYPE_BOTH) {
            _scrollbar_set_wArrows(_scrollbar_getscroll(pObj, SCROLLBAR_TYPE_VERT), wArrows, TRUE);
            _scrollbar_set_wArrows(_scrollbar_getscroll(pObj, SCROLLBAR_TYPE_HORZ), wArrows, TRUE);
        }
        else {
            _scrollbar_set_wArrows(_scrollbar_getscroll(pObj, wSB), wArrows, TRUE);
        }
    }
    return nError == 0;
}

BOOL Ex_ObjGetRectEx(HEXOBJ hObj, RECT* lpRect, INT nType)
{
    INT    nError = 0;
    obj_s* pObj   = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (IsBadWritePtr(lpRect, 16)) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else if (nType == 0) {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, left_)), 16);
        }
        else if (nType == 1) {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
        }
        else if (nType == 2) {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, w_left_)), 16);
        }
        else if (nType == 3) {
            RtlMoveMemory(lpRect, (LPVOID)((size_t)pObj + offsetof(obj_s, t_left_)), 16);
        }
        else {
            nError = ERROR_EX_HANDLE_BADINDEX;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjPointTransform(HEXOBJ hObjSrc, HEXOBJ hObjDst, INT* ptX, INT* ptY)
{
    INT nError = 0;
    if (hObjSrc != hObjDst) {
        obj_s* pObjSrc = nullptr;
        if (_handle_validate(hObjSrc, HT_OBJECT, (LPVOID*)&pObjSrc, &nError)) {
            INT    nOffsetX = pObjSrc->w_left_;
            INT    nOffsetY = pObjSrc->w_top_;
            obj_s* pObjDst  = nullptr;
            if (_handle_validate(hObjDst, HT_OBJECT, (LPVOID*)&pObjDst, &nError)) {
                nOffsetX = nOffsetX - pObjDst->w_left_;
                nOffsetY = nOffsetY - pObjDst->w_top_;
            }
            *ptX = *ptX - nOffsetX;
            *ptY = *ptY - nOffsetY;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjEnableEventBubble(HEXOBJ hObj, BOOL bEnable)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (bEnable) {
            pObj->base.dwFlags_ = pObj->base.dwFlags_ | EOF_BEVENTBUBBLE;
        }
        else {
            pObj->base.dwFlags_ = pObj->base.dwFlags_ - (pObj->base.dwFlags_ & EOF_BEVENTBUBBLE);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjGetClassInfo(HEXOBJ hObj, EX_CLASSINFO* lpClassInfo)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        EX_CLASSINFO* pClass = pObj->pCls_;
        if (!IsBadWritePtr(lpClassInfo, sizeof(EX_CLASSINFO))) {

            RtlMoveMemory(lpClassInfo, pClass, sizeof(EX_CLASSINFO));
        }
        else {
            nError = ERROR_EX_MEMPOOL_BADPTR;
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjHandleEvent(HEXOBJ hObj, INT nEvent, EventHandlerPROC pfnCallback)
{
    obj_s* pObj       = NULL;
    INT    nError     = 0;
    BOOL   bFind      = FALSE;
    INT    i          = 0;
    INT    len        = 0;
    INT    originSize = 0;

    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        wnd_s*                  pWnd        = pObj->pWnd_;
        EX_HASHTABLE*           hTableEvent = pWnd->hTableEvent_;
        EX_EVENT_HANDLER_TABLE* pEventHandlerTable;

        if (hTableEvent) {
            if (HashTable_Get(hTableEvent, nEvent, (size_t*)&pEventHandlerTable)) {
                len = pEventHandlerTable->len;

                for (i = 0; i < len; ++i) {
                    if (pEventHandlerTable->handler[i].hObj == hObj) {
                        bFind = TRUE;
                        break;
                    }
                }
                originSize = sizeof(EX_EVENT_HANDLER) * (len - 1) + sizeof(EX_EVENT_HANDLER_TABLE);
                if (bFind) {
                    if (pfnCallback) {
                        pEventHandlerTable->handler[i].pfnCallback = pfnCallback;
                    }
                    else if (--pEventHandlerTable->len) {
                        RtlMoveMemory(&pEventHandlerTable->handler[i],
                                      &pEventHandlerTable->handler[i + 1],
                                      (len - i - 1) * sizeof(EX_EVENT_HANDLER));
                    }
                    else {
                        HashTable_Remove(hTableEvent, nEvent);
                    }
                }
                else if (pfnCallback) {
                    EX_EVENT_HANDLER_TABLE* pNewTable =
                        (EX_EVENT_HANDLER_TABLE*)Ex_MemAlloc(originSize + sizeof(EX_EVENT_HANDLER));
                    if (pNewTable) {
                        RtlMoveMemory(pNewTable, pEventHandlerTable, originSize);
                        pNewTable->len++;
                        pNewTable->handler[pNewTable->len - 1].hObj        = hObj;
                        pNewTable->handler[pNewTable->len - 1].pfnCallback = pfnCallback;
                        HashTable_Set(hTableEvent, nEvent, (size_t)pNewTable);
                        Ex_MemFree(pEventHandlerTable);
                    }
                    else {
                        nError = ERROR_EX_MEMORY_ALLOC;
                    }
                }
            }
            else {
                pEventHandlerTable =
                    (EX_EVENT_HANDLER_TABLE*)Ex_MemAlloc(sizeof(EX_EVENT_HANDLER_TABLE));
                if (pEventHandlerTable) {
                    pEventHandlerTable->len                    = 1;
                    pEventHandlerTable->handler[0].hObj        = hObj;
                    pEventHandlerTable->handler[0].pfnCallback = pfnCallback;
                    HashTable_Set(hTableEvent, nEvent, (size_t)pEventHandlerTable);
                }
                else {
                    nError = ERROR_EX_MEMORY_ALLOC;
                }
            }
        }
        else {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
    }
    return nError == 0;
}

BOOL Ex_ObjGetClassInfoEx(LPCWSTR lptszClassName, EX_CLASSINFO* lpClassInfo)
{
    LPVOID pClass = NULL;
    EXATOM atom;
    INT    nError = 0;
    if (HashTable_IsExist(g_Li.hTableClass, (size_t)lptszClassName)) {
        atom = (EXATOM)lptszClassName;
    }
    else {
        atom = Ex_Atom(lptszClassName);
    }

    if (HashTable_Get(g_Li.hTableClass, atom, (size_t*)&pClass) && pClass) {
        if (!IsBadWritePtr(lpClassInfo, sizeof(EX_CLASSINFO))) {
            RtlMoveMemory(lpClassInfo, pClass, sizeof(EX_CLASSINFO));
            return TRUE;
        }
        else {
            nError = ERROR_EX_MEMPOOL_BADPTR;
            Ex_SetLastError(nError);
            return FALSE;
        }
    }
    return FALSE;
}

HEXOBJ Ex_ObjGetFromNodeID(EXHANDLE hExDUIOrObj, INT nNodeID)
{
    INT       nError = 0;
    HEXOBJ    hChild = NULL;
    obj_base* pObj   = NULL;

    if (_handle_validate(hExDUIOrObj, HT_DUI, (LPVOID*)&pObj, &nError)) {
        hChild = pObj->objChildFirst_;
    }
    else {
        nError = 0;
        if (_handle_validate(hExDUIOrObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            hChild = pObj->objChildFirst_;
        }
        else {
            Ex_SetLastError(ERROR_EX_HANDLE_INVALID);
        }
    }
    while (1) {
        nError = 0;
        if (!_handle_validate(hChild, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
            break;
        }

        if (((obj_s*)pObj)->nodeid_ == nNodeID) {
            break;
        }
        hChild = ((obj_s*)pObj)->objNext_;
    }
    return hChild;
}

LRESULT Ex_ObjCallProc(ClsPROC lpPrevObjProc, HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                       LPARAM lParam)
{
    return lpPrevObjProc(hWnd, hObj, uMsg, wParam, lParam);
}

BOOL Ex_ObjSetFontFromFamily(HEXOBJ hObj, LPCWSTR lpszFontfamily, INT dwFontsize, INT dwFontstyle,
                             BOOL fRedraw)
{
    INT     ret = 0;
    HEXFONT hFont;

    hFont = _font_createfromfamily(lpszFontfamily, dwFontsize, dwFontstyle);
    if (hFont) {
        ret = Ex_ObjSendMessage(hObj, WM_SETFONT, hFont, fRedraw);
    }
    else {
        ret = -1;
    }
    return ret == 0;
}

// 设置组件附加数据
LONG_PTR _obj_setextralong(obj_s* pObj, INT nIndex, LONG_PTR dwNewLong)
{
    EX_ASSERT(pObj, L"_obj_getextralong: error pObj: %ld", pObj);
    EX_ASSERT(nIndex >= 0 && nIndex * sizeof(LPVOID) < pObj->pCls_->cbObjExtra,
              L"_obj_getextralong: error index: %ld", nIndex);

    LONG_PTR origin;
    origin                   = pObj->extraData_[nIndex];
    pObj->extraData_[nIndex] = dwNewLong;
    return origin;
}

// 获取组件附加数据
LONG_PTR _obj_getextralong(obj_s* pObj, INT nIndex)
{
    EX_ASSERT(pObj, L"_obj_getextralong: error pObj: %ld", pObj);
    EX_ASSERT(nIndex >= 0 && nIndex * sizeof(LPVOID) < pObj->pCls_->cbObjExtra,
              L"_obj_getextralong: error index: %ld,%d,%d", nIndex, pObj->pCls_->cbObjExtra,
              pObj->pCls_->atomName);
    return pObj->extraData_[nIndex];
}

// 获取组件附加数据指针
LPVOID _obj_getextraptr(obj_s* pObj, INT nIndex)
{
    return &pObj->extraData_[nIndex];
}

// 查询组件附加数据flags
BOOL _obj_queryextra(obj_s* pObj, INT nIndex, size_t flags)
{
    return FLAGS_CHECK(pObj->extraData_[nIndex], flags);
}

// 添加组件附加数据flags
void _obj_addextra(obj_s* pObj, INT nIndex, size_t flags)
{
    FLAGS_ADD(pObj->extraData_[nIndex], flags);
}

void _obj_delextra(obj_s* pObj, INT nIndex, size_t flags)
{
    FLAGS_DEL(pObj->extraData_[nIndex], flags);
}

BOOL Ex_ObjEnablePaintingMsg(HEXOBJ hObj, BOOL bEnable)
{
    obj_s* pObj;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (bEnable) {
            FLAGS_ADD(pObj->base.dwFlags_, EOF_BPAINTINGMSG);
        }
        else {
            FLAGS_DEL(pObj->base.dwFlags_, EOF_BPAINTINGMSG);
        }
    }
    return nError == 0;
}

BOOL Ex_ObjEnableIME(HEXOBJ hObj, BOOL bEnable)
{
    obj_s* pObj;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (bEnable) {
            FLAGS_ADD(pObj->base.dwFlags_, EOF_BIME);
        }
        else {
            FLAGS_DEL(pObj->base.dwFlags_, EOF_BIME);
        }
    }
    return nError == 0;
}

INT CALLBACK _SetParent_EnumChild(size_t hObj, size_t lParam)
{
    std::vector<_SP_ChildhObj_private>* childhexobj = (std::vector<_SP_ChildhObj_private>*)lParam;
    int num = 0;
    HEXOBJ objEntry = hObj;
    obj_s* pObj = nullptr;
    while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj, 0))
    {
        if (pObj->objParent_)
        {
            num += 1;
            objEntry = pObj->objParent_;
        }
        else
            break;
    }
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, 0))
    {
        childhexobj->push_back({ (HEXOBJ)hObj ,pObj, pObj->objParent_, num });
    }

    return 1;
}

bool _SP_ChildhObj_private_sort(_SP_ChildhObj_private a, _SP_ChildhObj_private b)
{
    return a.parentnum > b.parentnum;
}

BOOL _obj_ChildEnum(HEXOBJ hObjParent, LPVOID lpEnumFunc, LPARAM lParam)
{
    if (IsBadCodePtr((FARPROC)lpEnumFunc) || hObjParent == 0)
    {
        return FALSE;
    }
    obj_s* pObj = nullptr;
    if (_handle_validate(hObjParent, HT_OBJECT, (LPVOID*)&pObj, 0))
    {
        HEXOBJ objEntry = pObj->base.objChildFirst_;
        obj_s* pObj2 = nullptr;
        while (_handle_validate(objEntry, HT_OBJECT, (LPVOID*)&pObj2, 0))
        {
            ((EnumPROC)lpEnumFunc)(objEntry, lParam);
            HEXOBJ sObj = pObj2->base.objChildFirst_;
            if (sObj)
                _obj_ChildEnum(objEntry, lpEnumFunc, lParam);
            objEntry = pObj2->objNext_;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL Ex_ObjSetParent(HEXOBJ hObj, EXHANDLE hParent)
{
    HWND NewhWnd = 0;
    wnd_s* OldpWnd = nullptr;
    wnd_s* NewpWnd = nullptr;
    obj_s* OldpObj = nullptr;
    obj_s* ParentpObj = nullptr;
    INT nError = 0;
    BOOL isObj;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&OldpObj, &nError)) {
        if (_wnd_getfromhandle(hParent, &NewhWnd, &NewpWnd, &ParentpObj, &isObj, &nError)) {

            OldpWnd = OldpObj->pWnd_;
            if (OldpWnd == NewpWnd) {
                if (OldpObj->objParent_ != hParent) {
                    EXHANDLE  Parent  = 0;
                    obj_base* pParnet = nullptr;
                    _obj_z_clear(hObj, OldpObj, &Parent, &pParnet);
                    if (isObj) {
                        OldpObj->objParent_ = hParent;
                        _obj_z_set(hObj, OldpObj, 0, 0, &nError);
                    }
                    else {
                        OldpObj->objParent_ = 0;
                    }
                    InvalidateRect(NewpWnd->hWnd_, 0, 1);
                    InvalidateRect(OldpWnd->hWnd_, 0, 1);
                    _obj_invalidaterect(OldpObj, 0, &nError);
                }
            }
            else   // 跨窗口置父
            {
                std::vector<_SP_ChildhObj_private> childhexobj;
                _obj_ChildEnum(hObj, (LPVOID)_SetParent_EnumChild, (LPARAM)&childhexobj);
                //---按照组件深度 重新排序所有子组件  最深组件->最浅组件
                std::sort(childhexobj.begin(), childhexobj.end(), _SP_ChildhObj_private_sort);
                _SetParent_EnumChild(hObj, (LPARAM)&childhexobj);

                //子组件 置父->根
                for (auto& childobj : childhexobj)
                {
                    _obj_baseproc(NewpWnd->hWnd_, childobj.hObj, childobj.pObj, WM_SETPARENTBEFORE, 0, NewpWnd->base.hexdui_);
                    KillTimer(childobj.pObj->pWnd_->hWnd_, (size_t)childobj.pObj + TIMER_OBJECT);
                    auto dwTmp = childobj.pObj->base.lpBackgroundImage_;
                    if (dwTmp != 0)//先停止可能存在的动图时钟,在置父完成后再恢复时钟
                    {
                        KillTimer(OldpWnd->hWnd_, (UINT_PTR)((size_t)childobj.pObj + TIMER_BKG));
                    }
                    if (childobj.pObj->pWnd_->objTrack_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->objTrack_ = 0;
                        //fRelaseCaption = TRUE;
                    }
                    if (childobj.pObj->pWnd_->objFocus_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->objFocus_ = 0;
                    }
                    if (childobj.pObj->pWnd_->objHittest_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->objHittest_ = 0;
                    }
                    if (childobj.pObj->pWnd_->objFocusPrev_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->objFocusPrev_ = 0;
                    }
                    if (childobj.pObj->pWnd_->base.objChildLast_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->base.objChildLast_ = childobj.pObj->objPrev_;
                    }
                    if (childobj.pObj->pWnd_->base.objChildFirst_ == childobj.hObj)
                    {
                        childobj.pObj->pWnd_->base.objChildFirst_ = childobj.pObj->objNext_;
                    }
               
                    EX_HASHTABLE* hTableEvent = OldpWnd->hTableEvent_;
                    std::vector<size_t> aKey;
                    std::vector<size_t> aValue;
                    HashTable_GetAllKeysAndValues(hTableEvent, aKey, aValue);
                    for (INT i = 0; i < aValue.size(); i++)
                    {
                        EX_EVENT_HANDLER_TABLE* pEventHandlerTable = (EX_EVENT_HANDLER_TABLE*)aValue[i];
                        INT len = pEventHandlerTable->len;
                        for (INT j = 0; j < len; j++)
                        {
                            if (pEventHandlerTable->handler[j].hObj == childobj.hObj)
                            {
                                if (--pEventHandlerTable->len)
                                {
                                    if (len - j - 1 > 0)
                                    {
                                        RtlMoveMemory(&pEventHandlerTable->handler[j], &pEventHandlerTable->handler[j + 1], (len - j - 1) * sizeof(EX_EVENT_HANDLER));
                                    }
                                }
                                else
                                {
                                    HashTable_Remove(hTableEvent, aKey[i]);
                                }
                                break;
                            }
                        }
                    }
                
                    EXHANDLE Parent1 = 0;
                    obj_base* pParnet1 = nullptr;
                    _obj_z_clear(childobj.hObj, childobj.pObj, &Parent1, &pParnet1);//先清除链表
                    _layout_deletechild(OldpWnd->base.hLayout_, childobj.hObj);//更换窗口,布局需删除本组件
                    childobj.pObj->pWnd_ = NewpWnd;
               
                    //需要修改本组件的*->初始化画布
                    canvas_s* pCanvas = nullptr;
                    if (_handle_validate(childobj.pObj->canvas_obj_, HT_CANVAS, (LPVOID*)&pCanvas, &nError))
                    {
                        pCanvas->pWnd_ = NewpWnd;
                        ID2D1Bitmap* pBitmap = _dx_createbitmap(NewpWnd->dx_context_, pCanvas->width_, pCanvas->height_, &nError);
                        if (pBitmap)
                        {
                            ID2D1Bitmap* oldBitmap = pCanvas->pBitmap_;
                            pCanvas->pBitmap_ = pBitmap;
                            if (oldBitmap != 0)
                            {
                                oldBitmap->Release();
                            }
                            pCanvas->pContext_ = NewpWnd->dx_context_;
                            pCanvas->pGdiInterop_ = NewpWnd->dx_gdiinterop_;
                        }
                    }
                    if (childobj.pObj->id_ != 0)
                    {
                        HashTable_Remove(OldpWnd->hTableObjects_, childobj.pObj->id_);//删除原来的键值对
                        childobj.pObj->id_ = 0;
                    }
             
                    childobj.pObj->objParent_ = 0;//先全部置0
                    _obj_z_set(childobj.hObj, childobj.pObj, 0, 0, &nError);

                    //通知组件父句柄改变了
                    _obj_baseproc(NewpWnd->hWnd_, childobj.hObj, childobj.pObj, WM_SETPARENTAFTER, 0, NewpWnd->base.hexdui_);
                   
                    if (dwTmp != 0)
                    {
                        if (dwTmp->lpDelay && dwTmp->maxFrame > 1)//恢复动图
                            SetTimer(NewpWnd->hWnd_, ((size_t)childobj.pObj + TIMER_BKG), ((INT*)dwTmp->lpDelay)[0] * 10, _obj_backgroundimage_timer);
                    }
                    SetTimer(childobj.pObj->pWnd_->hWnd_, (size_t)childobj.pObj + TIMER_OBJECT, childobj.pObj->base.uElapse_, _obj_timer_object);
          
                }
                InvalidateRect(NewpWnd->hWnd_, 0, 1);
                InvalidateRect(OldpWnd->hWnd_, 0, 1);

                _obj_invalidaterect(OldpObj, 0, &nError);
                //恢复realParent 
                for (auto& childobj : childhexobj)
                {
                    if (childobj.realParent)
                    {
                        Ex_ObjSetParent(childobj.hObj, childobj.realParent);
                    }
                }
                InvalidateRect(NewpWnd->hWnd_, 0, 1);
                InvalidateRect(OldpWnd->hWnd_, 0, 1);
                _obj_invalidaterect(OldpObj, 0, &nError);
            }
        }
        else {
            nError = ERROR_EX_INVALID_OBJECT;
        }
    }
    return nError == 0;
}

BOOL Ex_ObjSetIMEState(EXHANDLE hObjOrExDui, BOOL fOpen)
{
    HWND   hWnd   = 0;
    wnd_s* pWnd   = nullptr;
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    BOOL   ret    = FALSE;
    if (_wnd_getfromhandle(hObjOrExDui, &hWnd, &pWnd, &pObj, 0, &nError)) {
        IME_Control(hWnd, fOpen);
        ret = TRUE;
    }
    return ret;
}

BOOL Ex_ObjDisableTranslateSpaceAndEnterToClick(HEXOBJ hObj, BOOL fDisable)
{
    obj_s* pObj   = nullptr;
    INT    nError = 0;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        if (fDisable) {
            FLAGS_ADD(pObj->base.dwFlags_, EOF_BDISABLESPACEANDENTER);
        }
        else {
            FLAGS_DEL(pObj->base.dwFlags_, EOF_BDISABLESPACEANDENTER);
        }
    }
    return nError == 0;
}

BOOL Ex_ObjCheckDropFormat(HEXOBJ hObj, LPVOID pDataObject, DWORD dwFormat)
{
    FORMATETC cFmtIn;
    cFmtIn.cfFormat = dwFormat;
    cFmtIn.ptd      = 0;
    cFmtIn.dwAspect = DVASPECT_CONTENT;
    cFmtIn.lindex   = -1;
    cFmtIn.tymed    = TYMED_HGLOBAL;
    FORMATETC cFmtOUT{0};
    cFmtIn.cfFormat = dwFormat;
    cFmtIn.ptd      = 0;
    cFmtIn.dwAspect = DVASPECT_CONTENT;
    cFmtIn.lindex   = -1;
    cFmtIn.tymed    = TYMED_HGLOBAL;
    HRESULT ret     = ((IDataObject*)pDataObject)->GetCanonicalFormatEtc(&cFmtIn, &cFmtOUT);
    return ret == S_OK;
}

INT Ex_ObjGetDropString(HEXOBJ hObj, LPVOID pDataObject, LPWSTR lpwzBuffer, INT cchMaxLength)
{
    FORMATETC cFmtIn;
    cFmtIn.cfFormat   = CF_UNICODETEXT;
    cFmtIn.ptd        = 0;
    cFmtIn.dwAspect   = DVASPECT_CONTENT;
    cFmtIn.lindex     = -1;
    cFmtIn.tymed      = TYMED_HGLOBAL;
    HRESULT hr        = ((IDataObject*)pDataObject)->QueryGetData(&cFmtIn);
    BOOL    isUnicode = TRUE;
    INT     ret       = 0;
    if (hr != 0) {
        cFmtIn.cfFormat = CF_TEXT;
        hr              = ((IDataObject*)pDataObject)->QueryGetData(&cFmtIn);
        if (hr == 0) {
            isUnicode = FALSE;
        }
    }
    STGMEDIUM stgMedium = {0};
    if (((IDataObject*)pDataObject)->GetData(&cFmtIn, &stgMedium) == 0) {
        LPVOID hMem  = stgMedium.hBitmap;
        LPVOID lpMem = GlobalLock(hMem);
        if (lpMem) {
            if (isUnicode) {
                ret = lstrlenW((LPCWSTR)lpMem);
                if (lpwzBuffer) {
                    lstrcpynW(lpwzBuffer, (LPCWSTR)lpMem, cchMaxLength);
                }
            }
            else {
                ret = lstrlenA((LPCSTR)lpMem);
                if (lpwzBuffer) {
                    std::string  str = (LPCSTR)lpMem;
                    std::wstring retwStr;
                    retwStr = Ex_A2W(str);
                    lstrcpynW(lpwzBuffer, (LPCWSTR)retwStr.data(), cchMaxLength);
                }
            }
        }
        GlobalUnlock(hMem);
        if (stgMedium.hMetaFilePict == 0) {
            GlobalFree(hMem);
        }
        ReleaseStgMedium(&stgMedium);
    }
    return ret;
}

size_t Ex_ObjEditSetSelCharFormat(HEXOBJ hObj, INT dwMask, EXARGB crText, LPCWSTR wzFontFace,
                                  DWORD fontSize, INT yOffset, BOOL bBold, BOOL bItalic,
                                  BOOL bUnderLine, BOOL bStrikeOut, BOOL bLink)
{
    CHARFORMAT2W Format;
    Format.cbSize   = sizeof(CHARFORMAT2W);
    Format.dwMask   = dwMask;
    DWORD dwEffects = 0;
    if ((dwMask & EDIT_SELECT_CHARFORMAT_COLOR) == EDIT_SELECT_CHARFORMAT_COLOR) {
        Format.crTextColor = ExARGB2RGB(crText);
    }
    if ((dwMask & EDIT_SELECT_CHARFORMAT_OFFSET) == EDIT_SELECT_CHARFORMAT_OFFSET) {
        Format.yOffset = yOffset;
    }
    if ((dwMask & (EDIT_SELECT_CHARFORMAT_BOLD | EDIT_SELECT_CHARFORMAT_ITALIC |
                   EDIT_SELECT_CHARFORMAT_UNDERLINE | EDIT_SELECT_CHARFORMAT_STRIKEOUT |
                   EDIT_SELECT_CHARFORMAT_LINK)) != 0) {
        if (bBold) {
            dwEffects = dwEffects | CFE_BOLD;
        }
        if (bItalic) {
            dwEffects = dwEffects | CFE_ITALIC;
        }
        if (bUnderLine) {
            dwEffects = dwEffects | CFE_UNDERLINE;
        }
        if (bStrikeOut) {
            dwEffects = dwEffects | CFE_STRIKEOUT;
        }
        if (bLink) {
            dwEffects = dwEffects | CFE_LINK;
        }
        Format.dwEffects = dwEffects;
    }
    if ((dwMask & EDIT_SELECT_CHARFORMAT_FACE) == EDIT_SELECT_CHARFORMAT_FACE) {
        RtlMoveMemory(Format.szFaceName, wzFontFace, LF_FACESIZE);
    }
    if ((dwMask & EDIT_SELECT_CHARFORMAT_SIZE) == EDIT_SELECT_CHARFORMAT_SIZE) {
        Format.yHeight = fontSize * 1440 / 96;
    }
    return Ex_ObjSendMessage(hObj, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&Format);
}

size_t Ex_ObjEditSetSelParFormat(HEXOBJ hObj, DWORD dwMask, WORD wNumbering, INT dxStartIndent,
                                 INT dxRightIndent, INT dxOffset, WORD wAlignment)
{
    PARAFORMAT Format;
    Format.cbSize = sizeof(PARAFORMAT);
    Format.dwMask = dwMask;
    if ((dwMask & EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING) == EDIT_SELECT_PARAGRAPHFORMAT_NUMBERING) {
        Format.wNumbering = wNumbering;
    }
    if ((dwMask & EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT) ==
        EDIT_SELECT_PARAGRAPHFORMAT_STARTINDENT) {
        Format.dxStartIndent = dxStartIndent * 20;
    }
    if ((dwMask & EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT) ==
        EDIT_SELECT_PARAGRAPHFORMAT_RIGHTINDENT) {
        Format.dxRightIndent = dxRightIndent * 20;
    }
    if ((dwMask & EDIT_SELECT_PARAGRAPHFORMAT_OFFSET) == EDIT_SELECT_PARAGRAPHFORMAT_OFFSET) {
        Format.dxOffset = dxOffset;
    }
    if ((dwMask & EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT) == EDIT_SELECT_PARAGRAPHFORMAT_ALIGNMENT) {
        Format.wAlignment = wAlignment;
    }
    return Ex_ObjSendMessage(hObj, EM_SETPARAFORMAT, 0, (LPARAM)&Format);
}

BOOL _obj_backgroundimage_setsvg(HWND hWnd, obj_s* pObj, HEXSVG hSvg, INT x, INT y,
                                 INT dwRepeat, RECT* lpGrid, INT dwFlags, INT dwAlpha, INT* nError)
{
    
    _obj_backgroundimage_clear(hWnd, (obj_base*)pObj);
    HEXIMAGE hImg = 0;
    INT width = pObj->right_ - pObj->left_;
    INT height = pObj->bottom_ - pObj->top_;
    _img_createfromsvg(hSvg, width, height, &hImg);
    if (hImg != 0) {
        LPVOID lpBI = _struct_createfromaddr(pObj, offsetof(obj_base, lpBackgroundImage_),
                                                sizeof(EX_BACKGROUNDIMAGEINFO), nError);
        if (lpBI != 0) {
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwFlags  = dwFlags;
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->hImage   = hImg;
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->x        = x;
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->y        = y;
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwRepeat = dwRepeat;
            ((EX_BACKGROUNDIMAGEINFO*)lpBI)->dwAlpha  = dwAlpha;
            if (lpGrid != 0) {
                LPVOID lpDelay = _struct_createfromaddr(
                    lpBI, offsetof(EX_BACKGROUNDIMAGEINFO, lpGrid), 16, nError);
                if (lpDelay != 0) {
                    RtlMoveMemory(lpDelay, lpGrid, 16);
                }
            }

            INT nFrames = 0;
            _img_getframecount(hImg, &nFrames);

            if (nFrames > 1) {
                INT* lpDelay2 = (INT*)Ex_MemAlloc(nFrames * sizeof(INT));
                if (_img_getframedelay(hImg, lpDelay2, nFrames)) {
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->lpDelay  = lpDelay2;
                    ((EX_BACKGROUNDIMAGEINFO*)lpBI)->maxFrame = nFrames;
                    if ((dwFlags & BACKGROUND_FLAG_PLAYIMAGE) == BACKGROUND_FLAG_PLAYIMAGE) {
                        SetTimer(hWnd, ((size_t)pObj + TIMER_BKG), lpDelay2[0] * 10,
                                    _obj_backgroundimage_timer);
                    }
                }
                else {
                    Ex_MemFree(lpDelay2);
                }
            }
            return TRUE;
        }
        _img_destroy(hImg);
    }
    
    return FALSE;
}

BOOL Ex_ObjSetBackgroundImageFromSvg(EXHANDLE handle, HEXSVG hSvg, INT x, INT y,
                                        DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha,
                                        BOOL fUpdate)
{
    HWND   hWnd   = 0;
    obj_s* pObj   = nullptr;
    BOOL   bObj   = FALSE;
    INT    nError = 0;
    if (_wnd_getfromhandle(handle, &hWnd, NULL, &pObj, &bObj, &nError)) {
        if (_obj_backgroundimage_setsvg(hWnd, pObj, hSvg, x, y, dwRepeat, lpGrid, dwFlags,
                                        dwAlpha, &nError)) {
            if (bObj) {
                nError = 0;
                _obj_invalidaterect(pObj, 0, &nError);
            }
            else {
                ((wnd_s*)pObj)->dwStyle_ = ((wnd_s*)pObj)->dwStyle_ | WINDOW_STYLE_NOINHERITBKG;
                _wnd_redraw_bkg(hWnd, (wnd_s*)pObj, 0, TRUE, FALSE);
            }
            if (fUpdate) {
                UpdateWindow(hWnd);
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}

BOOL Ex_ObjScreenToClient(HEXOBJ hObj, INT* x, INT* y)
{
    INT    nError = 0;
    obj_s* pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError)) {
        LPRECT prc = (LPRECT)MemPool_Alloc(g_Li.hMemPoolMsg, TRUE);
        if (prc == 0) {
            nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            wnd_s* pWnd = pObj->pWnd_;
            RtlMoveMemory(prc, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
            OffsetRect(prc, pObj->w_left_ + pWnd->left_, pObj->w_top_ + pWnd->top_);
            if (x) {
                *x -= prc->left;
            }
            if (y) {
                *y -= prc->top;
            }
            MemPool_Free(g_Li.hMemPoolMsg, prc);
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}