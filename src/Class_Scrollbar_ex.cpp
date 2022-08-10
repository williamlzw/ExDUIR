#include "Class_Scrollbar_ex.h"

void _scrollbar_register()
{
    Ex_ObjRegister(L"Scrollbar", ESS_RIGHTBOTTOMALIGN | ESS_CONTROLBUTTON | EOS_VISIBLE, 0, 0, 0, 0, 0, _scrollbar_proc);
}

size_t _scrollbar_parentnotify(HWND hWnd, obj_s *pObj, WPARAM wParam, LPARAM lParam, INT uMsg, BOOL bDispatch)
{
    HEXOBJ hParent = pObj->objParent_;
    obj_s *lpParent = nullptr;
    INT nError = 0;
    size_t ret = 0;
    if (_handle_validate(hParent, HT_OBJECT, (LPVOID *)&lpParent, &nError))
    {
        if (uMsg == 0)
        {
            uMsg = ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL) ? WM_VSCROLL : WM_HSCROLL;
        }
        if (bDispatch)
        {
            ret = _obj_baseproc(hWnd, hParent, lpParent, uMsg, wParam, lParam);
        }
        else
        {
            ret = Ex_ObjSendMessage(hParent, uMsg, wParam, lParam);
        }
    }
    return ret;
}

INT _scrollbar_postopoint(HWND hWnd, obj_s *pObj, si_s *psi, INT *nPos, BOOL bVert, INT *cxy)
{
    INT l = psi->rcRegion_left_;
    INT t = psi->rcRegion_top_;
    INT r = psi->rcRegion_right_;
    INT b = psi->rcRegion_bottom_;
    INT nPage = psi->nPage_;
    INT nMin = psi->nMin_;
    INT nMax = psi->nMax_;
    SHORT nMinThumbsize = HIWORD(psi->xyz_);
    auto nMinThumbsize2 = HIBYTE(nMinThumbsize);
    INT maxPos = nMax - nMin;
    INT sizeRegin = bVert ? b - t : r - l;
    INT point = 0;
    if (maxPos > 0)
    {
        if (*nPos < nMin)
        {
            *nPos = nMin;
            psi->nPos_ = *nPos;
        }
        else
        {
            if (nMax < *nPos)
            {
                *nPos = nMax;
                psi->nPos_ = *nPos;
            }
        }
        *cxy = sizeRegin * nPage / (maxPos + nPage);
        if (*cxy < nMinThumbsize2)
        {
            *cxy = nMinThumbsize2;
        }
        point = (*nPos - nMin) * (sizeRegin - *cxy) / maxPos;
    }
    point = point + (bVert ? t : l);
    return point;
}

HEXOBJ _scrollbar_getscroll(obj_s *pObj, INT nBar)
{
    HEXOBJ ret = 0;
    if (nBar == SB_HORZ)
    {
        ret = pObj->objHScroll_;
    }
    else if (nBar == SB_VERT)
    {
        ret = pObj->objVScroll_;
    }
    else if (nBar == SB_CTL)
    {
        ret = pObj->hObj_;
    }
    return ret;
}

void _scrollbar_calcthumb(HWND hWnd, obj_s *pObj, si_s *psi, BOOL bVScroll)
{
    INT cxy = 0;
    INT point = _scrollbar_postopoint(hWnd, pObj, psi, &(psi->nPos_), bVScroll, &cxy);
    if (bVScroll)
    {
        psi->rcThumb_left_ = psi->rcRegion_left_;
        psi->rcThumb_top_ = point;
        psi->rcThumb_right_ = psi->rcRegion_right_;
        psi->rcThumb_bottom_ = point + cxy;
    }
    else
    {
        psi->rcThumb_left_ = point;
        psi->rcThumb_top_ = psi->rcRegion_top_;
        psi->rcThumb_right_ = point + cxy;
        psi->rcThumb_bottom_ = psi->rcRegion_bottom_;
    }
}

INT _scrollbar_realsetinfo(HWND hWnd, HEXOBJ hObj, obj_s *pObj, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos, BOOL bRedraw)
{
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    if ((Mask & SIF_POS) != 0)
    {
        psi->nPos_ = nPos;
    }
    if ((Mask & SIF_PAGE) != 0)
    {
        psi->nPage_ = nPage;
    }
    if ((Mask & SIF_RANGE) != 0)
    {
        psi->nMin_ = nMin;
        psi->nMax_ = nMax;
    }
    INT nPosOrg = psi->nPos_;
    _scrollbar_calcthumb(hWnd, pObj, psi, ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL));
    nPos = psi->nPos_;
    if (nPos != nPosOrg)
    {
        _scrollbar_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBPOSITION, nPos), 0, 0, FALSE);
    }
    if (bRedraw)
    {
        INT nError = 0;
        _obj_invalidaterect(pObj, 0, &nError);
    }
    return nPos;
}

void _scrollbar_init(obj_s *pObj)
{
    si_s *psi = (si_s *)_struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(si_s), 0);
    BOOL bVS = FALSE;
    if (psi)
    {
        bVS = ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL);
        HEXTHEME hTheme = pObj->hTheme_;
        INT atom = bVS ? ATOM_VSCROLL : ATOM_HSCROLL;
        LPVOID pValue = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);
        if (pValue != 0)
        {
            psi->xyz_ = MAKELONG(MAKEWORD(Ex_Scale(__get_int(pValue, 0)), Ex_Scale(__get_int(pValue, 4))), MAKEWORD(Ex_Scale(__get_int(pValue, 8)), Ex_Scale(__get_int(pValue, 12))));
        }
        if (((pObj->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
        {
            psi->wArrows_ = ESB_DISABLE_BOTH;
        }
    }
    if (g_Li.hMenuVS == 0 && bVS)
    {
        HMODULE puser32 = GetModuleHandleW(L"user32.dll");
        if (puser32 != 0)
        {
            g_Li.hMenuVS = LoadMenuW(puser32, MAKEINTRESOURCE(80));
        }
    }
    if (g_Li.hMenuHS == 0 && !bVS)
    {
        HMODULE puser32 = GetModuleHandleW(L"user32.dll");
        if (puser32 != 0)
        {
            g_Li.hMenuHS = LoadMenuW(puser32, MAKEINTRESOURCE(64));
        }
    }
}

void _scrollbar_uninit(obj_s *pObj)
{
    _struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
}

void _scrollbar_nccalcsize(HWND hWnd, HEXOBJ hObj, obj_s *pObj)
{
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    BOOL bVScroll = ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL);
    INT xyz = psi->xyz_;
    WORD cxy = HIWORD(xyz);
    xyz = LOWORD(xyz);
    INT cx = 0, cy = 0;
    if (((pObj->dwStyle_ & ESS_CONTROLBUTTON) == ESS_CONTROLBUTTON))
    {
        cx = LOBYTE(xyz);
        cy = HIBYTE(xyz);
    }
    RECT rcClient{0};
    rcClient.left = pObj->c_left_;
    rcClient.top = pObj->c_top_;
    rcClient.right = pObj->c_right_;
    rcClient.bottom = pObj->c_bottom_;
    if (bVScroll)
    {
        psi->rcArrow1_left_ = rcClient.left;
        psi->rcArrow1_top_ = rcClient.top;
        psi->rcArrow1_right_ = rcClient.right;
        psi->rcArrow1_bottom_ = rcClient.top + cx;

        psi->rcArrow2_left_ = rcClient.left;
        psi->rcArrow2_top_ = rcClient.bottom - cy;
        psi->rcArrow2_right_ = rcClient.right;
        psi->rcArrow2_bottom_ = rcClient.bottom;

        psi->rcRegion_left_ = rcClient.left;
        psi->rcRegion_top_ = rcClient.top + cx;
        psi->rcRegion_right_ = rcClient.right;
        psi->rcRegion_bottom_ = rcClient.bottom - cy;
    }
    else
    {
        psi->rcArrow1_left_ = rcClient.left;
        psi->rcArrow1_top_ = rcClient.top;
        psi->rcArrow1_right_ = rcClient.left + cx;
        psi->rcArrow1_bottom_ = rcClient.bottom;

        psi->rcArrow2_left_ = rcClient.right - cy;
        psi->rcArrow2_top_ = rcClient.top;
        psi->rcArrow2_right_ = rcClient.right;
        psi->rcArrow2_bottom_ = rcClient.bottom;

        psi->rcRegion_left_ = rcClient.left + cx;
        psi->rcRegion_top_ = rcClient.top;
        psi->rcRegion_right_ = rcClient.right - cy;
        psi->rcRegion_bottom_ = rcClient.bottom;
    }
    _obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CALCSIZE, 0, (size_t)psi + offsetof(si_s, rcRegion_left_));
    _scrollbar_calcthumb(hWnd, pObj, psi, bVScroll);
}

INT _scrollbar_pointtopos(si_s *psi, INT x, INT y, BOOL bVert, BOOL bCheckPos)
{
    RECT rcRegion, rcThumb;
    rcRegion.left = psi->rcRegion_left_;
    rcRegion.top = psi->rcRegion_top_;
    rcRegion.right = psi->rcRegion_right_;
    rcRegion.bottom = psi->rcRegion_bottom_;
    rcThumb.left = psi->rcThumb_left_;
    rcThumb.top = psi->rcThumb_top_;
    rcThumb.right = psi->rcThumb_right_;
    rcThumb.bottom = psi->rcThumb_bottom_;
    INT nMin = psi->nMin_;
    INT nMax = psi->nMax_;
    INT maxpos = nMax - nMin;
    INT sizeRegion, curPoint, sizeThumb;
    if (bVert)
    {
        sizeRegion = rcRegion.bottom - rcRegion.top;
        curPoint = y - rcRegion.top - psi->nTrackPosOffset_;
        sizeThumb = rcThumb.bottom - rcThumb.top;
    }
    else
    {
        sizeRegion = rcRegion.right - rcRegion.left;
        curPoint = x - rcRegion.left - psi->nTrackPosOffset_;
        sizeThumb = rcThumb.right - rcThumb.left;
    }
    INT nPos = curPoint * maxpos / (sizeRegion - sizeThumb) + nMin;
    if (bCheckPos)
    {
        if (nPos >= nMax)
        {
            nPos = nMax;
        }
        if (nPos <= nMin)
        {
            nPos = nMin;
        }
    }
    return nPos;
}

void _scrollbar_nchittest(obj_s *pObj, INT x, INT y)
{
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    RECT rc{0};
    rc.left = psi->rcArrow1_left_;
    rc.top = psi->rcArrow1_top_;
    rc.right = psi->rcArrow1_right_;
    rc.bottom = psi->rcArrow1_bottom_;
    POINT pt;
    pt.x = x;
    pt.y = y;
    INT httype = NULL;
    if (PtInRect(&rc, pt))
    {
        if (!((psi->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT))
        {
            httype = SBCT_ADJUSTBUTTON1;
        }
    }
    else
    {
        RECT rc1{0};
        rc1.left = psi->rcArrow2_left_;
        rc1.top = psi->rcArrow2_top_;
        rc1.right = psi->rcArrow2_right_;
        rc1.bottom = psi->rcArrow2_bottom_;
        if (PtInRect(&rc1, pt))
        {
            if (!((psi->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT))
            {
                httype = SBCT_ADJUSTBUTTON2;
            }
        }
        else
        {
            if (psi->wArrows_ != ESB_DISABLE_BOTH)
            {
                RECT rcThumb{0};
                rcThumb.left = psi->rcThumb_left_;
                rcThumb.top = psi->rcThumb_top_;
                rcThumb.right = psi->rcThumb_right_;
                rcThumb.bottom = psi->rcThumb_bottom_;
                if (PtInRect(&rcThumb, pt))
                {
                    httype = SBCT_CONTROL;
                }
                else
                {
                    RECT rcRegion{0};
                    rcRegion.left = psi->rcRegion_left_;
                    rcRegion.top = psi->rcRegion_top_;
                    rcRegion.right = psi->rcRegion_right_;
                    rcRegion.bottom = psi->rcRegion_bottom_;
                    if (PtInRect(&rcRegion, pt))
                    {
                        if (((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL))
                        {
                            if (y <= psi->rcThumb_top_)
                            {
                                httype = SBCT_PAGEAREA1;
                            }
                            else
                            {
                                httype = SBCT_PAGEAREA2;
                            }
                        }
                        else
                        {
                            if (x <= psi->rcThumb_left_)
                            {
                                httype = SBCT_PAGEAREA1;
                            }
                            else
                            {
                                httype = SBCT_PAGEAREA2;
                            }
                        }
                    }
                }
            }
        }
    }
    INT oldhttype = psi->httype_;
    psi->httype_ = httype;
    if (oldhttype != httype)
    {
        INT nError = 0;
        _obj_invalidaterect(pObj, 0, &nError);
    }
}

void _scrollbar_mousemove(HWND hWnd, HEXOBJ hObj, obj_s *pObj, WPARAM wParam, INT x, INT y)
{
    if (wParam != 0)
    {
        si_s *psi = (si_s *)_obj_pOwner(pObj);
        if (psi->httype_ == SBCT_CONTROL)
        {
            INT lstPos = psi->nTrackPos_;
            INT curPos = _scrollbar_pointtopos(psi, x, y, ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL), TRUE);
            if (lstPos != curPos)
            {
                psi->nTrackPos_ = curPos;
                if (_scrollbar_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBTRACK, curPos), 0, 0, TRUE) == 0)
                {
                    psi->nPos_ = curPos;
                    _scrollbar_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBPOSITION, curPos), 0, 0, TRUE);
                }
            }
        }
    }
}

void CALLBACK _scrollbar_timer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    obj_s* pObj = (obj_s*)(idEvent - TIMER_SCROLLBAR);
    si_s *psi = (si_s *)_obj_pOwner((obj_s*)pObj);
    INT nTrack;
    if (psi->httype_ == SBCT_ADJUSTBUTTON1)
    {
        nTrack = SB_LINEUP;
    }
    else
    {
        nTrack = SB_LINEDOWN;
    }
    _scrollbar_parentnotify(hWnd, (obj_s*)pObj, MAKELONG(nTrack, 0), pObj->hObj_, 0, TRUE);
    nTrack = psi->nPos_;
    if (psi->httype_ == SBCT_ADJUSTBUTTON1)
    {
        if (nTrack <= psi->nMin_)
        {          
            KillTimer(hWnd, idEvent);
        }
    }
    else
    {
        if (nTrack >= psi->nMax_ - psi->nPage_)
        {
            KillTimer(hWnd, idEvent);
        }
    }
}

void _scrollbar_leftbuttondown(HWND hWnd, HEXOBJ hObj, obj_s *pObj, LPARAM lParam)
{
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    INT httype = psi->httype_;
    INT nError = 0;
    _obj_setuistate(pObj, STATE_DOWN, FALSE, 0, TRUE, &nError);
    if (httype != 0)
    {
        INT nTrack = -1;
        INT x, y;
        BOOL fTimer = FALSE;
        if (httype == SBCT_CONTROL)
        {
            x = GET_X_LPARAM(lParam) - psi->rcThumb_left_;
            y = GET_Y_LPARAM(lParam) - psi->rcThumb_top_;
            psi->nTrackPosOffset_ = ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL) ? y : x;
            psi->nTrackPos_ = psi->nPos_;
        }
        else if (httype == SBCT_ADJUSTBUTTON1)
        {
            nTrack = SB_LINEUP;
            fTimer = TRUE;
        }
        else if (httype == SBCT_ADJUSTBUTTON2)
        {
            nTrack = SB_LINEDOWN;
            fTimer = TRUE;
        }
        else if (httype == SBCT_PAGEAREA1)
        {
            nTrack = SB_PAGEUP;
        }
        else if (httype == SBCT_PAGEAREA2)
        {
            nTrack = SB_PAGEDOWN;
        }

        if (nTrack != -1)
        {
            _scrollbar_parentnotify(hWnd, pObj, MAKELONG(nTrack, 0), hObj, 0, TRUE);
        }
        if (fTimer)
        {
            SetTimer(hWnd, (size_t)pObj + TIMER_SCROLLBAR, 200, _scrollbar_timer);
        }
    }
}

void _scrollbar_leftbuttonup(HWND hWnd, HEXOBJ hObj, obj_s *pObj, LPARAM lParam)
{
    INT nError = 0;
    _obj_setuistate(pObj, STATE_DOWN, TRUE, 0, TRUE, &nError);
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    psi->nTrackPos_ = 0;
    KillTimer(hWnd, (size_t)pObj + TIMER_SCROLLBAR);
}

void _scrollbar_oncommand(HWND hWnd, HEXOBJ hObj, obj_s *pObj, WPARAM wParam, LPARAM lParam)
{
    BOOL fNotify = TRUE;
    wnd_s *pWnd;
    INT nPos = NULL;
    INT nCode;
    if (wParam == 4100)
    {
        pWnd = pObj->pWnd_;
        si_s *psi = (si_s *)_obj_pOwner(pObj);
        INT nTrackPosOffset = psi->nTrackPosOffset_;
        psi->nTrackPosOffset_ = 0;
        nPos = _scrollbar_pointtopos(psi, LOWORD(nTrackPosOffset) - pObj->w_left_ - pWnd->left_, HIWORD(nTrackPosOffset) - pObj->w_top_ - pWnd->top_, ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL), TRUE);
        psi->nTrackPos_ = nPos;
        nCode = SB_THUMBPOSITION;
    }
    else if (wParam == 4102)
    {
        nCode = SB_TOP;
    }
    else if (wParam == 4103)
    {
        nCode = SB_BOTTOM;
    }
    else if (wParam == 4096)
    {
        nCode = SB_LINEUP;
    }
    else if (wParam == 4097)
    {
        nCode = SB_LINEDOWN;
    }
    else if (wParam == 4098)
    {
        nCode = SB_PAGEUP;
    }
    else if (wParam == 4099)
    {
        nCode = SB_PAGEDOWN;
    }
    else
    {
        fNotify = FALSE;
    }
    if (fNotify)
    {
        _scrollbar_parentnotify(hWnd, pObj, MAKELONG(nCode, nPos), hObj, 0, TRUE);
    }
}

void _scrollbar_oncontextmenu(HEXOBJ hObj, obj_s *pObj, LPARAM lParam)
{
    HMENU hMenu = ((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL) ? g_Li.hMenuVS : g_Li.hMenuHS;
    hMenu = GetSubMenu(hMenu, 0);
    si_s *psi = (si_s *)_obj_pOwner(pObj);
    psi->nTrackPosOffset_ = lParam;
    INT wEnable = ((psi->wArrows_ & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH) ? 1026 : 1024;
    EnableMenuItem(hMenu, 0, wEnable); //滚动至此
    wEnable = (((psi->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT) || (psi->nPos_ == psi->nMin_)) ? 1026 : 1024;
    EnableMenuItem(hMenu, 2, wEnable); //顶部
    EnableMenuItem(hMenu, 5, wEnable); //上页
    EnableMenuItem(hMenu, 8, wEnable); //上行
    wEnable = (((psi->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT) || (psi->nPos_ == psi->nMax_)) ? 1026 : 1024;
    EnableMenuItem(hMenu, 3, wEnable); //底部
    EnableMenuItem(hMenu, 6, wEnable); //下页
    EnableMenuItem(hMenu, 9, wEnable); //下行
    Ex_TrackPopupMenu(hMenu, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hObj, 0, 0, 0);
}

INT _scrollbar_paint(HEXOBJ hObj, obj_s *pObj)
{
    EX_PAINTSTRUCT ps;
    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        INT httype = ((si_s *)ps.dwOwnerData)->httype_;
        BOOL bHover = (ps.dwState & STATE_HOVER) != 0;
        BOOL bDown = (ps.dwState & STATE_DOWN) != 0;
        BOOL bVScroll = (ps.dwStyle & ESS_VERTICALSCROLL) != 0;
        EXATOM atomClass = bVScroll ? ATOM_VSCROLL : ATOM_HSCROLL;
        if (bHover || (ps.dwStyle & EOS_DISABLENOSCROLL) != 0)
        {
            INT atomBtn1 = ATOM_ARROW1_NORMAL;
            if (httype == SBCT_ADJUSTBUTTON1)
            {
                if (bDown)
                {
                    atomBtn1 = ATOM_ARROW1_DOWN;
                }
                else
                {
                    if (bHover)
                    {
                        atomBtn1 = ATOM_ARROW1_HOVER;
                    }
                }
            }
            RECT rcSrc{0};
            rcSrc.left = ((si_s *)ps.dwOwnerData)->rcArrow1_left_;
            rcSrc.top = ((si_s *)ps.dwOwnerData)->rcArrow1_top_;
            rcSrc.right = ((si_s *)ps.dwOwnerData)->rcArrow1_right_;
            rcSrc.bottom = ((si_s *)ps.dwOwnerData)->rcArrow1_bottom_;
            INT alpha = ((((si_s *)ps.dwOwnerData)->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT) ? 128 : 255;
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, atomClass, atomBtn1, 0, 0, 0, 0, alpha);
        }

        if (bHover || (ps.dwStyle & EOS_DISABLENOSCROLL) != 0)
        {
            INT atomBtn2 = ATOM_ARROW2_NORMAL;
            if (httype == SBCT_ADJUSTBUTTON2)
            {
                if (bDown)
                {
                    atomBtn2 = ATOM_ARROW2_DOWN;
                }
                else
                {
                    if (bHover)
                    {
                        atomBtn2 = ATOM_ARROW2_HOVER;
                    }
                }
            }
            RECT rcSrc{0};
            rcSrc.left = ((si_s *)ps.dwOwnerData)->rcArrow2_left_;
            rcSrc.top = ((si_s *)ps.dwOwnerData)->rcArrow2_top_;
            rcSrc.right = ((si_s *)ps.dwOwnerData)->rcArrow2_right_;
            rcSrc.bottom = ((si_s *)ps.dwOwnerData)->rcArrow2_bottom_;
            INT alpha = ((((si_s *)ps.dwOwnerData)->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT) ? 128 : 255;
            Ex_ThemeDrawControlEx(ps.hTheme, ps.hCanvas, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, atomClass, atomBtn2, 0, 0, 0, 0, alpha);
        }
        if (bHover || (ps.dwStyle & EOS_DISABLENOSCROLL) != 0)
        {
            RECT rcRegion{0};
            rcRegion.left = ((si_s *)ps.dwOwnerData)->rcRegion_left_;
            rcRegion.top = ((si_s *)ps.dwOwnerData)->rcRegion_top_;
            rcRegion.right = ((si_s *)ps.dwOwnerData)->rcRegion_right_;
            rcRegion.bottom = ((si_s *)ps.dwOwnerData)->rcRegion_bottom_;
            INT alpha = ((((si_s *)ps.dwOwnerData)->wArrows_ & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH) ? 128 : 255;
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, rcRegion.left, rcRegion.top, rcRegion.right, rcRegion.bottom, atomClass, ATOM_RECT, alpha);
        }
        RECT rcThumb{0};
        rcThumb.left = ((si_s *)ps.dwOwnerData)->rcThumb_left_;
        rcThumb.top = ((si_s *)ps.dwOwnerData)->rcThumb_top_;
        rcThumb.right = ((si_s *)ps.dwOwnerData)->rcThumb_right_;
        rcThumb.bottom = ((si_s *)ps.dwOwnerData)->rcThumb_bottom_;
        if (!IsRectEmpty(&rcThumb))
        {
            INT atomThumb = ATOM_THUMB_NORMAL;
            if (httype == SBCT_CONTROL)
            {
                if (bDown)
                {
                    atomThumb = ATOM_THUMB_DOWN;
                }
                else
                {
                    if (bHover)
                    {
                        atomThumb = ATOM_THUMB_HOVER;
                    }
                }
            }
            Ex_ThemeDrawControl(ps.hTheme, ps.hCanvas, rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom, atomClass, atomThumb, 255);
        }
        Ex_ObjEndPaint(hObj, &ps);
    }
    return 0;
}

void _scrollbar_set_wArrows(HEXOBJ hSB, INT wArrows, BOOL fRedraw)
{
    obj_s *pSB = nullptr;
    INT nError = 0;
    if (_handle_validate(hSB, HT_OBJECT, (LPVOID *)&pSB, &nError))
    {
        si_s *psi = (si_s *)_obj_pOwner(pSB);
        if (psi != 0)
        {
            psi->wArrows_ = wArrows;
            if (wArrows == ESB_DISABLE_BOTH)
            {
                psi->rcThumb_left_ = 0;
                psi->rcThumb_top_ = 0;
                psi->rcThumb_right_ = 0;
                psi->rcThumb_bottom_ = 0;
            }
        }
        if (fRedraw)
        {
            nError = 0;
            _obj_invalidaterect(pSB, 0, &nError);
        }
    }
}

LRESULT CALLBACK _scrollbar_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT nError = 0;
    obj_s *pObj = nullptr;
    if (_handle_validate(hObj, HT_OBJECT, (LPVOID *)&pObj, &nError))
    {
        if (uMsg == WM_CREATE)
        {
            _scrollbar_init(pObj);
        }
        else if (uMsg == WM_DESTROY)
        {
            _scrollbar_uninit(pObj);
        }
        else if (uMsg == WM_NCHITTEST)
        {
            _scrollbar_nchittest(pObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        else if (uMsg == WM_MOUSEMOVE)
        {
            INT x = GET_X_LPARAM(lParam);
            INT y = GET_Y_LPARAM(lParam);
            si_s *psi = (si_s *)_obj_pOwner(pObj);
            if (((pObj->dwStyle_ & ESS_VERTICALSCROLL) == ESS_VERTICALSCROLL))
            {
                if (y >= psi->nMin_ && y <= psi->nMax_ + psi->rcRegion_bottom_ - psi->rcRegion_top_)
                {
                    _scrollbar_mousemove(hWnd, hObj, pObj, wParam, x, y);
                }
            }
            else if (((pObj->dwStyle_ & ESS_HORIZONTALSCROLL) == ESS_HORIZONTALSCROLL))
            {
                if (x >= psi->nMin_ && x <= psi->nMax_ + psi->rcRegion_right_ - psi->rcRegion_left_)
                {
                    _scrollbar_mousemove(hWnd, hObj, pObj, wParam, x, y);
                }
            }
        }
        else if (uMsg == WM_SIZE)
        {
            _scrollbar_nccalcsize(hWnd, hObj, pObj);
        }
        else if (uMsg == WM_PAINT)
        {
            return _scrollbar_paint(hObj, pObj);
        }
        else if (uMsg == WM_CONTEXTMENU)
        {
            _scrollbar_oncontextmenu(hObj, pObj, lParam);
        }
        else if (uMsg == WM_MOUSEHOVER)
        {
            _obj_setuistate(pObj, STATE_HOVER, FALSE, 0, TRUE, &nError);
        }
        else if (uMsg == WM_MOUSELEAVE)
        {
            _obj_setuistate(pObj, STATE_HOVER, TRUE, 0, TRUE, &nError);
        }
        else if (uMsg == WM_LBUTTONDOWN)
        {
            _scrollbar_leftbuttondown(hWnd, hObj, pObj, lParam);
        }
        else if (uMsg == WM_LBUTTONUP)
        {
            _scrollbar_leftbuttonup(hWnd, hObj, pObj, lParam);
        }
        else if (uMsg == WM_MOUSEWHEEL || uMsg == WM_HSCROLL || uMsg == WM_VSCROLL)
        {
            _scrollbar_parentnotify(hWnd, pObj, wParam, lParam, uMsg, TRUE);
        }
        else if (uMsg == SBM_SETSCROLLINFO)
        {
            return _scrollbar_realsetinfo(hWnd, hObj, pObj, __get_int((LPVOID)lParam, 4), __get_int((LPVOID)lParam, 8), __get_int((LPVOID)lParam, 12), __get_int((LPVOID)lParam, 16), __get_int((LPVOID)lParam, 20), wParam != 0);
        }
        else if (uMsg == SBM_SETPOS)
        {
            return _scrollbar_realsetinfo(hWnd, hObj, pObj, SIF_POS, 0, 0, 0, wParam, lParam != 0);
        }
        else if (uMsg == SBM_GETPOS)
        {
            return Ex_ObjScrollGetPos(hObj, SB_CTL);
        }
        else if (uMsg == SBM_SETRANGE)
        {
            return _scrollbar_realsetinfo(hWnd, hObj, pObj, SIF_RANGE, wParam, lParam, 0, 0, FALSE);
        }
        else if (uMsg == SBM_GETRANGE)
        {
            Ex_ObjScrollGetRange(hObj, SB_CTL, (INT *)&wParam, (INT *)&lParam);
        }
        else if (uMsg == WM_SETFOCUS)
        {
            return 0;
        }
        else if (uMsg == WM_COMMAND)
        {
            _scrollbar_oncommand(hWnd, hObj, pObj, wParam, lParam);
        }
    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}