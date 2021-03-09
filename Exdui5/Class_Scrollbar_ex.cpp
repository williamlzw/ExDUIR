#include "Class_Scrollbar_ex.h"

int _sb_parentnotify(HWND hWnd, void* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch)
{
	EXHANDLE hParent = ((obj_s*)pObj)->objParent_;
	void* lpParent = nullptr;
	int nError = 0;
	int ret = 0;
	if (_handle_validate(hParent, HT_OBJECT, &lpParent, &nError))
	{
		if (uMsg == 0)
		{
			uMsg = ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条) ? WM_VSCROLL : WM_HSCROLL;
		}
		if (bDispatch)
		{
			ret = _obj_baseproc(hWnd, hParent, lpParent, uMsg, wParam, lParam);
		}
		else {
			ret = Ex_ObjSendMessage(hParent, uMsg, wParam, lParam);
		}
	}
	return ret;
}

int _sb_pos2point(HWND hWnd, void* pObj, void* psi, int* nPos, bool bVert, int* cxy)
{
	int l = ((si_s*)psi)->rcRegion_left_;
	int t = ((si_s*)psi)->rcRegion_top_;
	int r = ((si_s*)psi)->rcRegion_right_;
	int b = ((si_s*)psi)->rcRegion_bottom_;
	int nPage = ((si_s*)psi)->nPage_;
	int nMin = ((si_s*)psi)->nMin_;
	int nMax = ((si_s*)psi)->nMax_;
	short nMinThumbsize = HIWORD(((si_s*)psi)->xyz_);
	auto  nMinThumbsize2 = HIBYTE(nMinThumbsize);
	int maxPos = nMax - nMin;
	int sizeRegin = bVert ? b - t : r - l;
	int point;
	if (maxPos > 0)
	{
		if (*nPos < nMin)
		{
			*nPos = nMin;
			((si_s*)psi)->nPos_ = *nPos;
		}
		else {
			if (nMax < *nPos)
			{
				*nPos = nMax;
				((si_s*)psi)->nPos_ = *nPos;
			}
		}
		*cxy = sizeRegin * nPage / (maxPos + nPage);
		if (*cxy < nMinThumbsize2)
		{
			*cxy = nMinThumbsize2;
		}
		point = (*nPos - nMin) * (sizeRegin - *cxy) / maxPos;
	}
	point = point + bVert ? t : l;
	return point;
}

EXHANDLE _sb_getscroll(void* pObj, int nBar)
{
	EXHANDLE ret = 0;
	if (nBar == SB_HORZ)
	{
		ret = ((obj_s*)pObj)->objHScroll_;
	}
	else if (nBar == SB_VERT)
	{
		ret = ((obj_s*)pObj)->objVScroll_;
	}
	else if (nBar == SB_CTL)
	{
		ret = ((obj_s*)pObj)->hObj_;
	}
	return ret;
}

void _sb_calcthumb(HWND hWnd, void* pObj, void* psi, bool bVScroll)
{
	int cxy;
	int point = _sb_pos2point(hWnd, pObj, psi, &(((si_s*)psi)->nPos_), bVScroll, &cxy);
	if (bVScroll)
	{
		((si_s*)psi)->rcThumb_left_ = ((si_s*)psi)->rcRegion_left_;
		((si_s*)psi)->rcThumb_top_ = point;
		((si_s*)psi)->rcThumb_right_ = ((si_s*)psi)->rcRegion_right_;
		((si_s*)psi)->rcThumb_bottom_ = point + cxy;
	}
	else {
		((si_s*)psi)->rcThumb_left_ = point;
		((si_s*)psi)->rcThumb_top_ = ((si_s*)psi)->rcRegion_top_;
		((si_s*)psi)->rcThumb_right_ = point + cxy;
		((si_s*)psi)->rcThumb_bottom_ = ((si_s*)psi)->rcRegion_bottom_;
	}
}

int _sb_realsetinfo(HWND hWnd, EXHANDLE hObj, void* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw)
{
	void* psi = _obj_pOwner(pObj);
	if ((Mask & SIF_POS) != 0)
	{
		((si_s*)psi)->nPos_ = nPos;
	}
	if ((Mask & SIF_PAGE) != 0)
	{
		((si_s*)psi)->nPage_ = nPage;
	}
	if ((Mask & SIF_RANGE) != 0)
	{
		((si_s*)psi)->nMin_ = nMin;
		((si_s*)psi)->nMax_ = nMax;
	}
	int nPosOrg = ((si_s*)psi)->nPos_;
	_sb_calcthumb(hWnd, pObj, psi, ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条));
	nPos = ((si_s*)psi)->nPos_;
	if (nPos != nPosOrg)
	{
		_sb_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBPOSITION, nPos), 0, 0, false);
	}
	if (bRedraw)
	{
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
	return nPos;
}

void _sb_init(void* pObj)
{
	void* psi = _struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(si_s), 0);
	bool bVS = false;
	if (psi != 0)
	{
		bVS = ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条);
		void* hTheme = ((obj_s*)pObj)->hTheme_;
		int atom = bVS ? ATOM_VSCROLL : ATOM_HSCROLL;
		void* pValue = Ex_ThemeGetValuePtr(hTheme, atom, ATOM_SIZE);
		if (pValue != 0)
		{
			((si_s*)psi)->xyz_ = MAKELONG(MAKEWORD(Ex_Scale(__get_int(pValue, 0)), Ex_Scale(__get_int(pValue, 4))), MAKEWORD(Ex_Scale(__get_int(pValue, 8)), Ex_Scale(__get_int(pValue, 12))));
		}
	}
	if (((((obj_s*)pObj)->dwStyle_ & EOS_DISABLENOSCROLL) == EOS_DISABLENOSCROLL))
	{
		((si_s*)psi)->wArrows_ = ESB_DISABLE_BOTH;
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

void _sb_uninit(void* pObj)
{
	_struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
}

void _sb_nccalcsize(HWND hWnd, EXHANDLE hObj, void* pObj)
{
	void* psi = _obj_pOwner(pObj);
	bool bVScroll = ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条);
	int xyz = ((si_s*)psi)->xyz_;
	auto hcxy = HIWORD(xyz);
	auto lcxy = LOBYTE(hcxy);
	auto lcxy1 = LOWORD(lcxy);
	int cx, cy;
	if (((((obj_s*)pObj)->dwStyle_ & 滚动条风格_控制按钮) == 滚动条风格_控制按钮))
	{
		cx = LOBYTE(lcxy1);
		cy = HIBYTE(lcxy1);
	}
	RECT rcClient;
	rcClient.left = ((obj_s*)pObj)->c_left_;
	rcClient.top = ((obj_s*)pObj)->c_top_;
	rcClient.right = ((obj_s*)pObj)->c_right_;
	rcClient.bottom = ((obj_s*)pObj)->c_bottom_;
	if (bVScroll)
	{
		((si_s*)psi)->rcArrow1_left_ = rcClient.left;
		((si_s*)psi)->rcArrow1_top_ = rcClient.top;
		((si_s*)psi)->rcArrow1_right_ = rcClient.right;
		((si_s*)psi)->rcArrow1_bottom_ = rcClient.top + cx;

		((si_s*)psi)->rcArrow2_left_ = rcClient.left;
		((si_s*)psi)->rcArrow2_top_ = rcClient.bottom - cy;
		((si_s*)psi)->rcArrow2_right_ = rcClient.right;
		((si_s*)psi)->rcArrow2_bottom_ = rcClient.bottom;

		((si_s*)psi)->rcRegion_left_ = rcClient.left;
		((si_s*)psi)->rcRegion_top_ = rcClient.top + cx;
		((si_s*)psi)->rcRegion_right_ = rcClient.right;
		((si_s*)psi)->rcRegion_bottom_ = rcClient.bottom - cy;
	}
	else {
		((si_s*)psi)->rcArrow1_left_ = rcClient.left;
		((si_s*)psi)->rcArrow1_top_ = rcClient.top;
		((si_s*)psi)->rcArrow1_right_ = rcClient.left + cx;
		((si_s*)psi)->rcArrow1_bottom_ = rcClient.bottom;

		((si_s*)psi)->rcArrow2_left_ = rcClient.right - cy;
		((si_s*)psi)->rcArrow2_top_ = rcClient.top;
		((si_s*)psi)->rcArrow2_right_ = rcClient.right;
		((si_s*)psi)->rcArrow2_bottom_ = rcClient.bottom;

		((si_s*)psi)->rcRegion_left_ = rcClient.left + cx;
		((si_s*)psi)->rcRegion_top_ = rcClient.top;
		((si_s*)psi)->rcRegion_right_ = rcClient.right - cy;
		((si_s*)psi)->rcRegion_bottom_ = rcClient.bottom;
	}
	_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CALCSIZE, 0, (size_t)psi + offsetof(si_s, rcRegion_left_));
	_sb_calcthumb(hWnd, pObj, psi, bVScroll);
}

int _sb_point2pos(void* psi, int x, int y, bool bVert, bool bCheckPos)
{
	RECT rcRegion, rcThumb;
	rcRegion.left = ((si_s*)psi)->rcRegion_left_;
	rcRegion.top = ((si_s*)psi)->rcRegion_top_;
	rcRegion.right = ((si_s*)psi)->rcRegion_right_;
	rcRegion.bottom = ((si_s*)psi)->rcRegion_bottom_;
	rcThumb.left = ((si_s*)psi)->rcThumb_left_;
	rcThumb.top = ((si_s*)psi)->rcThumb_top_;
	rcThumb.right = ((si_s*)psi)->rcThumb_right_;
	rcThumb.bottom = ((si_s*)psi)->rcThumb_bottom_;
	int nMin = ((si_s*)psi)->nMin_;
	int nMax = ((si_s*)psi)->nMax_;
	int maxpos = nMax - nMin;
	int sizeRegion, curPoint, sizeThumb;
	if (bVert)
	{
		sizeRegion = rcRegion.bottom - rcRegion.top;
		curPoint = y - rcRegion.top - ((si_s*)psi)->nTrackPosOffset_;
		sizeThumb = rcThumb.bottom - rcThumb.top;
	}
	else {
		sizeRegion = rcRegion.right - rcRegion.left;
		curPoint = x - rcRegion.left - ((si_s*)psi)->nTrackPosOffset_;
		sizeThumb = rcThumb.right - rcThumb.left;
	}
	int nPos = curPoint * maxpos / (sizeRegion - sizeThumb) + nMin;
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

void _sb_nchittest(void* pObj, int x, int y)
{
	void* psi = _obj_pOwner(pObj);
	RECT rc;
	rc.left = ((si_s*)psi)->rcArrow1_left_;
	rc.top = ((si_s*)psi)->rcArrow1_top_;
	rc.right = ((si_s*)psi)->rcArrow1_right_;
	rc.bottom = ((si_s*)psi)->rcArrow1_bottom_;
	POINT pt;
	pt.x = x;
	pt.y = y;
	int httype;
	if (PtInRect(&rc, pt))
	{
		if (!((((si_s*)psi)->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT))
		{
			httype = 滚动条点击类型_调节按钮1;
		}
	}
	else {
		RECT rc1;
		rc1.left = ((si_s*)psi)->rcArrow2_left_;
		rc1.top = ((si_s*)psi)->rcArrow2_top_;
		rc1.right = ((si_s*)psi)->rcArrow2_right_;
		rc1.bottom = ((si_s*)psi)->rcArrow2_bottom_;
		if (PtInRect(&rc1, pt))
		{
			if (!((((si_s*)psi)->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT))
			{
				httype = 滚动条点击类型_调节按钮2;
			}
		}
		else {
			if (((si_s*)psi)->wArrows_ != ESB_DISABLE_BOTH)
			{
				RECT rcThumb;
				rcThumb.left = ((si_s*)psi)->rcThumb_left_;
				rcThumb.top = ((si_s*)psi)->rcThumb_top_;
				rcThumb.right = ((si_s*)psi)->rcThumb_right_;
				rcThumb.bottom = ((si_s*)psi)->rcThumb_bottom_;
				if (PtInRect(&rcThumb, pt))
				{
					httype = 滚动条点击类型_滚动条;
				}
				else {
					RECT rcRegion;
					rcRegion.left = ((si_s*)psi)->rcRegion_left_;
					rcRegion.top = ((si_s*)psi)->rcRegion_top_;
					rcRegion.right = ((si_s*)psi)->rcRegion_right_;
					rcRegion.bottom = ((si_s*)psi)->rcRegion_bottom_;
					if (PtInRect(&rcRegion, pt))
					{
						if (((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条))
						{
							if (y <= ((si_s*)psi)->rcThumb_top_)
							{
								httype = 滚动条点击类型_页面区域1;
							}
							else {
								httype = 滚动条点击类型_页面区域2;
							}
						}
						else {
							if (x <= ((si_s*)psi)->rcThumb_left_)
							{
								httype = 滚动条点击类型_页面区域1;
							}
							else {
								httype = 滚动条点击类型_页面区域2;
							}
						}
					}
				}
			}
		}
	}
	auto oldhttype = ((si_s*)psi)->httype_;
	((si_s*)psi)->httype_ = httype;
	if (oldhttype != httype)
	{
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
}

void _sb_mousemove(HWND hWnd, EXHANDLE hObj, void* pObj, size_t wParam, int x, int y)
{
	if (wParam != 0)
	{
		void* psi = _obj_pOwner(pObj);
		if (((si_s*)psi)->httype_ = 滚动条点击类型_滚动条)
		{
			int lstPos = ((si_s*)psi)->nTrackPos_;
			int curPos = _sb_point2pos(psi, x, y, ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条), true);
			if (lstPos != curPos)
			{
				((si_s*)psi)->nTrackPos_ = curPos;
				if (_sb_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBTRACK, curPos), 0, 0, true) == 0)
				{
					((si_s*)psi)->nPos_ = curPos;
					_sb_parentnotify(hWnd, pObj, MAKELONG(SB_THUMBPOSITION, curPos), 0, 0, true);
				}
			}
		}
	}
}

void _sb_timer(HWND hWnd, int uMsg, int idEvent, int dwTime)
{
	void* pObj = (void*)(idEvent - TIMER_SCROLLBAR);
	void* psi = _obj_pOwner(pObj);
	int nTrack;
	if (((si_s*)psi)->httype_ == 滚动条点击类型_调节按钮1)
	{
		nTrack = SB_LINEUP;
	}
	else {
		nTrack = SB_LINEDOWN;
	}
	_sb_parentnotify(hWnd, pObj, MAKELONG(nTrack, 0), ((obj_s*)pObj)->hObj_, 0, true);
	nTrack = ((si_s*)psi)->nPos_;
	if (((si_s*)psi)->httype_ == 滚动条点击类型_调节按钮1)
	{
		if (nTrack <= ((si_s*)psi)->nMin_)
		{
			KillTimer(hWnd, idEvent);
		}
	}
	else {
		if (nTrack >= ((si_s*)psi)->nMax_ - ((si_s*)psi)->nPage_)
		{
			KillTimer(hWnd, idEvent);
		}
	}

}

void _sb_lbuttondown(HWND hWnd, EXHANDLE hObj, void* pObj, size_t lParam)
{
	void* psi = _obj_pOwner(pObj);
	int httype = ((si_s*)psi)->httype_;
	int nError = 0;
	_obj_setuistate(pObj, 状态_按下, false, 0, true, &nError);
	if (httype != 0)
	{
		int nTrack = -1;
		int x, y;
		bool fTimer = false;
		if (httype == 滚动条点击类型_滚动条)
		{
			x = LOWORD(lParam) - ((si_s*)psi)->rcThumb_left_;
			y = HIWORD(lParam) - ((si_s*)psi)->rcThumb_top_;
			((si_s*)psi)->nTrackPosOffset_ = ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条) ? y : x;
			((si_s*)psi)->nTrackPos_ = ((si_s*)psi)->nPos_;
		}
		else if (httype == 滚动条点击类型_调节按钮1)
		{
			nTrack = SB_LINEUP;
			fTimer = true;
		}
		else if (httype == 滚动条点击类型_调节按钮2)
		{
			nTrack = SB_LINEDOWN;
			fTimer = true;
		}
		else if (httype == 滚动条点击类型_页面区域1)
		{
			nTrack = SB_PAGEUP;
		}
		else if (httype == 滚动条点击类型_页面区域2)
		{
			nTrack = SB_PAGEDOWN;
		}

		if (nTrack == -1)
		{
			_sb_parentnotify(hWnd, pObj, MAKELONG(nTrack, 0), hObj, 0, true);
		}
		if (fTimer)
		{
			SetTimer(hWnd, (size_t)pObj + TIMER_SCROLLBAR, 200, (TIMERPROC)&_sb_timer);
		}
	}
}

void _sb_lbuttonup(HWND hWnd, EXHANDLE hObj, void* pObj, size_t lParam)
{
	int nError = 0;
	_obj_setuistate(pObj, 状态_按下, true, 0, true, &nError);
	void* psi = _obj_pOwner(pObj);
	((si_s*)psi)->nTrackPos_ = 0;
	KillTimer(hWnd, (size_t)pObj + TIMER_SCROLLBAR);
}

void _sb_oncommand(HWND hWnd, EXHANDLE hObj, void* pObj, size_t wParam, size_t lParam)
{
	bool fNotify = true;
	wnd_s* pWnd;
	int nPos;
	int nCode;
	if (wParam == 4100)
	{
		pWnd = ((obj_s*)pObj)->pWnd_;
		void* psi = _obj_pOwner(pObj);
		int nTrackPosOffset = ((si_s*)psi)->nTrackPosOffset_;
		((si_s*)psi)->nTrackPosOffset_ = 0;
		nPos = _sb_point2pos(psi, LOWORD(nTrackPosOffset) - ((obj_s*)pObj)->w_left_ - pWnd->left_, HIWORD(nTrackPosOffset) - ((obj_s*)pObj)->w_top_ - pWnd->top_, ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条), true);
		((si_s*)psi)->nTrackPos_ = nPos;
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
	else {
		fNotify = false;
	}
	if (fNotify)
	{
		_sb_parentnotify(hWnd, pObj, MAKELONG(nCode, nPos), hObj, 0, true);
	}
}

void _sb_oncontextmenu(EXHANDLE hObj, void* pObj, size_t lParam)
{
	void* hMenu = ((((obj_s*)pObj)->dwStyle_ & 滚动条风格_垂直滚动条) == 滚动条风格_垂直滚动条) ? g_Li.hMenuVS : g_Li.hMenuHS;
	hMenu = GetSubMenu((HMENU)hMenu, 0);
	void* psi = _obj_pOwner(pObj);
	((si_s*)psi)->nTrackPosOffset_ = lParam;
	int wEnable = ((((si_s*)psi)->wArrows_ & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH) ? 1026 : 1024;
	EnableMenuItem((HMENU)hMenu, 0, wEnable);//滚动至此
	wEnable = (((((si_s*)psi)->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT) || (((si_s*)psi)->nPos_ == ((si_s*)psi)->nMin_)) ? 1026 : 1024;
	EnableMenuItem((HMENU)hMenu, 2, wEnable);//顶部
	EnableMenuItem((HMENU)hMenu, 5, wEnable);//上页
	EnableMenuItem((HMENU)hMenu, 8, wEnable);//上行
	wEnable = (((((si_s*)psi)->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT)  || (((si_s*)psi)->nPos_ == ((si_s*)psi)->nMax_)) ? 1026 : 1024;
	EnableMenuItem((HMENU)hMenu, 3, wEnable);//底部
	EnableMenuItem((HMENU)hMenu, 6, wEnable);//下页
	EnableMenuItem((HMENU)hMenu, 9, wEnable);//下行
	Ex_TrackPopupMenu(hMenu, 0, LOWORD(lParam), HIWORD(lParam), 0, hObj, 0, 0, 0);
}

int _sb_paint(EXHANDLE hObj, void* pObj)
{
	paintstruct_s ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		int httype = ((si_s*)ps.dwOwnerData_)->httype_;
		bool bHover = (ps.dwState_ & 状态_点燃) != 0;
		bool bDown = (ps.dwState_ & 状态_按下) != 0;
		bool bVScroll = (ps.dwStyle_ & 滚动条风格_垂直滚动条) != 0;
		int atomClass = bVScroll ? ATOM_VSCROLL : ATOM_HSCROLL;
		if (bHover || (ps.dwStyle_ & EOS_DISABLENOSCROLL) != 0)
		{
			int atomBtn1 = ATOM_ARROW1_NORMAL;
			if (httype == 滚动条点击类型_调节按钮1)
			{
				if (bDown)
				{
					atomBtn1 = ATOM_ARROW1_DOWN;
				}
				else {
					if (bHover)
					{
						atomBtn1 = ATOM_ARROW1_HOVER;
					}
				}
			}
			RECT rcSrc;
			rcSrc.left = ((si_s*)ps.dwOwnerData_)->rcArrow1_left_;
			rcSrc.top = ((si_s*)ps.dwOwnerData_)->rcArrow1_top_;
			rcSrc.right = ((si_s*)ps.dwOwnerData_)->rcArrow1_right_;
			rcSrc.bottom = ((si_s*)ps.dwOwnerData_)->rcArrow1_bottom_;
			int alpha = ((((si_s*)ps.dwOwnerData_)->wArrows_ & ESB_DISABLE_LEFT) == ESB_DISABLE_LEFT) ? 128 : 255;
			Ex_ThemeDrawControlEx(ps.hTheme_, ps.hCanvas_, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, atomClass, atomBtn1, 0, 0, 0, 0, alpha);
		}

		if (bHover || (ps.dwStyle_ & EOS_DISABLENOSCROLL) != 0)
		{
			int atomBtn2 = ATOM_ARROW2_NORMAL;
			if (httype == 滚动条点击类型_调节按钮2)
			{
				if (bDown)
				{
					atomBtn2 = ATOM_ARROW2_DOWN;
				}
				else {
					if (bHover)
					{
						atomBtn2 = ATOM_ARROW2_HOVER;
					}
				}
			}
			RECT rcSrc;
			rcSrc.left = ((si_s*)ps.dwOwnerData_)->rcArrow2_left_;
			rcSrc.top = ((si_s*)ps.dwOwnerData_)->rcArrow2_top_;
			rcSrc.right = ((si_s*)ps.dwOwnerData_)->rcArrow2_right_;
			rcSrc.bottom = ((si_s*)ps.dwOwnerData_)->rcArrow2_bottom_;
			int alpha = ((((si_s*)ps.dwOwnerData_)->wArrows_ & ESB_DISABLE_RIGHT) == ESB_DISABLE_RIGHT) ? 128 : 255;
			Ex_ThemeDrawControlEx(ps.hTheme_, ps.hCanvas_, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, atomClass, atomBtn2, 0, 0, 0, 0, alpha);
		}
		if (bHover || (ps.dwStyle_ & EOS_DISABLENOSCROLL) != 0)
		{
			RECT rcRegion;
			rcRegion.left = ((si_s*)ps.dwOwnerData_)->rcRegion_left_;
			rcRegion.top = ((si_s*)ps.dwOwnerData_)->rcRegion_top_;
			rcRegion.right = ((si_s*)ps.dwOwnerData_)->rcRegion_right_;
			rcRegion.bottom = ((si_s*)ps.dwOwnerData_)->rcRegion_bottom_;
			int alpha = ((((si_s*)ps.dwOwnerData_)->wArrows_ & ESB_DISABLE_BOTH) == ESB_DISABLE_BOTH) ? 128 : 255;
			Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, rcRegion.left, rcRegion.top, rcRegion.right, rcRegion.bottom, atomClass, ATOM_RECT, alpha);
		}
		RECT rcThumb;
		rcThumb.left = ((si_s*)ps.dwOwnerData_)->rcThumb_left_;
		rcThumb.top = ((si_s*)ps.dwOwnerData_)->rcThumb_top_;
		rcThumb.right = ((si_s*)ps.dwOwnerData_)->rcThumb_right_;
		rcThumb.bottom = ((si_s*)ps.dwOwnerData_)->rcThumb_bottom_;
		if (!IsRectEmpty(&rcThumb))
		{
			int atomThumb = ATOM_THUMB_NORMAL;
			if (httype == 滚动条点击类型_滚动条)
			{
				if (bDown)
				{
					atomThumb = ATOM_THUMB_DOWN;
				}
				else {
					if (bHover)
					{
						atomThumb = ATOM_THUMB_HOVER;
					}
				}
			}
			Ex_ThemeDrawControl(ps.hTheme_, ps.hCanvas_, rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom, atomClass, atomThumb, 255);
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}

void _sb_set_wArrows(EXHANDLE hSB, int wArrows, bool fRedraw)
{
	void* pSB = nullptr;
	int nError = 0;
	if (_handle_validate(hSB, HT_OBJECT, &pSB, &nError))
	{
		void* psi = _obj_pOwner(pSB);
		if (psi != 0)
		{
			((si_s*)psi)->wArrows_ = wArrows;
			if (wArrows == ESB_DISABLE_BOTH)
			{
				((si_s*)psi)->rcThumb_left_ = 0;
				((si_s*)psi)->rcThumb_top_ = 0;
				((si_s*)psi)->rcThumb_right_ = 0;
				((si_s*)psi)->rcThumb_bottom_ = 0;
			}
		}
		if (fRedraw)
		{
			_obj_invalidaterect(pSB, 0, &nError);
		}
	}
}

size_t _sb_proc(HWND hWnd, EXHANDLE hObj, int uMsg, size_t wParam, size_t lParam, void* pObj)
{
	int nError = 0;
	if (uMsg == WM_CREATE)
	{
		_sb_init(pObj);
	}
	else if (uMsg == WM_DESTROY)
	{
		_sb_uninit(pObj);
	}
	else if (uMsg == WM_NCHITTEST)
	{
		_sb_nchittest(pObj, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		_sb_mousemove(hWnd, hObj, pObj, wParam, LOWORD(lParam), HIWORD(lParam));
	}
	else if (uMsg == WM_SIZE)
	{
		_sb_nccalcsize(hWnd, hObj, pObj);
	}
	else if (uMsg == WM_PAINT)
	{
		return _sb_paint(hObj, pObj);
	}
	else if (uMsg == WM_CONTEXTMENU)
	{
		_sb_oncontextmenu(hObj, pObj, lParam);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		_obj_setuistate(pObj, 状态_点燃, false, 0, true, &nError);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		_obj_setuistate(pObj, 状态_点燃, true, 0, true, &nError);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		_sb_lbuttondown(hWnd, hObj, pObj, lParam);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		_sb_lbuttonup(hWnd, hObj, pObj, lParam);
	}
	else if (uMsg == WM_MOUSEWHEEL || uMsg == WM_HSCROLL || uMsg == WM_VSCROLL)
	{
		_sb_parentnotify(hWnd, pObj, wParam, lParam, uMsg, true);
	}
	else if (uMsg == SBM_SETSCROLLINFO)
	{
		return _sb_realsetinfo(hWnd, hObj, pObj, __get_int((void*)lParam, 4), __get_int((void*)lParam, 8), __get_int((void*)lParam, 12), __get_int((void*)lParam, 16), __get_int((void*)lParam, 20), wParam != 0);
	}
	else if (uMsg == SBM_SETPOS)
	{
		return _sb_realsetinfo(hWnd, hObj, pObj, SIF_POS, 0, 0, 0, wParam, lParam != 0);
	}
	else if (uMsg == SBM_GETPOS)
	{
		return Ex_ObjScrollGetPos(hObj, SB_CTL);
	}
	else if (uMsg == SBM_SETRANGE)
	{
		return _sb_realsetinfo(hWnd, hObj, pObj, SIF_RANGE, wParam, lParam, 0, 0, false);
	}
	else if (uMsg == SBM_GETRANGE)
	{
		Ex_ObjScrollGetRange(hObj, SB_CTL, &wParam, &lParam);
	}
	else if (uMsg == WM_SETFOCUS)
	{
		return 0;
	}
	else if (uMsg == WM_COMMAND)
	{
		_sb_oncommand(hWnd, hObj, pObj, wParam, lParam);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

