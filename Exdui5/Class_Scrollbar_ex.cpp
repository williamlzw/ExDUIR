#include "Class_Scrollbar_ex.h"

int _sb_parentnotify(HWND hWnd, void* pObj, size_t wParam, size_t lParam, int uMsg, bool bDispatch)
{
	size_t hParent = ((obj_s*)pObj)->objParent_;
	void* lpParent = nullptr;
	int nError = 1;
	int ret = 0;
	if (_handle_validate(hParent, HT_OBJECT, &lpParent, &nError))
	{
		if (uMsg == 0)
		{
			uMsg = __query(pObj, offsetof(obj_s, dwStyle_), 滚动条风格_垂直滚动条) ? WM_VSCROLL:WM_HSCROLL;
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
	short nMinThumbsize = 取高位(((si_s*)psi)->xyz_);
	auto  nMinThumbsize2 = 取高位(nMinThumbsize);
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

size_t _sb_getscroll(void* pObj, int nBar)
{
	size_t ret = 0;
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

int _sb_realsetinfo(HWND hWnd, size_t hObj, void* pObj, int Mask, int nMin, int nMax, int nPage, int nPos, bool bRedraw)
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
	_sb_calcthumb(hWnd, pObj, psi, __query(pObj, offsetof(obj_s, dwStyle_), 滚动条风格_垂直滚动条));
	nPos = ((si_s*)psi)->nPos_;
	if (nPos != nPosOrg)
	{
		_sb_parentnotify(hWnd, pObj, 合并整数(SB_THUMBPOSITION, nPos), 0, 0, false);
	}
	if (bRedraw)
	{
		_obj_invalidaterect(pObj, 0, 0);
	}
	return nPos;
}