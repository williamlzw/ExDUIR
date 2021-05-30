#include "Class_ListView_ex.h"

void _ListView_regsiter()
{
	Ex_ObjRegister(L"ListView", EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, 0, 0, _lv_proc);
}

void _lv_setitemstate(void* lpItems, int iItem, int dwState, bool bRemove)
{
	int offset = (iItem - 1) * 4;
	if (bRemove)
	{
		__del(lpItems, offset, dwState);
	}
	else {
		__add(lpItems, offset, dwState);
	}
}

void _lv_updatesbvalue(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, RECT* lpRcClient)
{
	HEXOBJ hHSB= pObj->objHScroll_;
	HEXOBJ hVSB= pObj->objVScroll_;
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, lpRcClient, 16);
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;
	bool bHView = ((pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST);
	int nCount = pOwner->count_items_;
	int iWidth = pOwner->width_item_ + pOwner->width_split_;
	int iHeight = pOwner->height_item_ + pOwner->height_split_;
	int nVS = 0;
	int nHS = 0;
	if (bHView)
	{
		nVS = height/iHeight;
		if (nVS == 0) nVS = 1;
		nHS = width / iWidth + 1;
	}
	else {
		nHS = width / iWidth;
		if (nHS == 0) nHS = 1;
		nVS = height / iHeight + 1;
	}
	int vWidth = 0;
	int vHeight = 0;
	if (bHView)//横向排列
	{
		vWidth = (nCount / nVS + (nCount % nVS == 0 ? 0 : 1)) * iWidth;
		if (hHSB != 0)
		{
			if (vWidth > width)
			{
				if (nVS > 1)
				{
					if (height < nVS * iHeight)
					{
						nVS = nVS - 1;
					}
					vWidth = (nCount / nVS + (nCount % nVS == 0 ? 0 : 1)) * iWidth;
				}
			}
		}
		if (vWidth < width)
		{
			vWidth = width;
		}
	}
	else {
		vHeight = (nCount / nHS + (nCount % nHS == 0 ? 0 : 1)) * iHeight;
		if (vHeight > height)
		{
			//横向数量大于1时，不管有没纵向滚动条都需要加上判断，超过的话强行-1
			if (nHS > 1)
			{
				if (width < nHS * iWidth)
				{
					nHS = nHS - 1;
				}
				vHeight = (nCount / nHS + (nCount % nHS == 0 ? 0 : 1)) * iHeight;
			}
		}
		vWidth = nHS * iWidth;

	}
	pOwner->count_view_h_ = nHS;
	pOwner->count_view_v_ = nVS;
	pOwner->count_view_ = nVS * nHS;
	pOwner->width_view_ = vWidth;
	pOwner->height_view_ = vHeight;
	if (hVSB != 0)
	{
		Ex_ObjScrollShow(hVSB, SB_CTL, vHeight - height > 0);
		Ex_ObjScrollSetInfo(hVSB, SB_CTL, SIF_PAGE | SIF_RANGE, 0, vHeight - height, height, 0, false);
	}
	if (hHSB != 0)
	{
		Ex_ObjScrollShow(hHSB, SB_CTL, vWidth - width > 0);
		Ex_ObjScrollSetInfo(hHSB, SB_CTL, SIF_PAGE | SIF_RANGE, 0, vWidth - width, width, 0, false);
	}
}

void _lv_updateviewindex(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, bool bHView, RECT* lpRcClient)
{
	listview_s lv;
	RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, lpRcClient, 16);
	int nWidth = lv.width_item_ + lv.width_split_;
	int nHeight = lv.height_item_ + lv.height_split_;
	int nOffset = 0;
	int iStart = 0;
	int iEnd = 0;
	if (bHView)
	{
		nOffset = Ex_ObjScrollGetPos(hObj, SB_HORZ);
		if (nOffset != lv.nOffsetX_)
		{
			pOwner->nOffsetX_ = nOffset;
		}
		iStart = (nOffset / nWidth) * lv.count_view_v_ + 1;
		iEnd = iStart + lv.count_view_ - 1;
		if (((iEnd - 1) / lv.count_view_v_) * nWidth - nOffset < rcClient.right - rcClient.left)
		{
			iEnd = iEnd + lv.count_view_v_;
		}
	}
	else {
		nOffset = Ex_ObjScrollGetPos(hObj, SB_VERT);
		if (nOffset != lv.nOffsetY_)
		{
			pOwner->nOffsetY_ = nOffset;
		}
		iStart = (nOffset / nHeight) * lv.count_view_h_ + 1;
		iEnd = iStart + lv.count_view_ - 1;
		if (((iEnd - 1) / lv.count_view_h_) * nHeight - nOffset < rcClient.bottom - rcClient.top)
		{
			iEnd = iEnd + lv.count_view_h_;
		}
	}
	pOwner->index_start_ = iStart;
	pOwner->index_end_ = iEnd;
}

bool _lv_checkitem(listview_s* pOwner, int iItem)
{
	return iItem > 0 && pOwner->count_items_ >= iItem;
}

bool _lv_checkitem_view(listview_s* pOwner, int iItem)
{
	bool ret = false;
	if (_lv_checkitem(pOwner, iItem))
	{
		ret = iItem >= pOwner->index_start_ && iItem <= pOwner->index_end_;
	}
	return ret;
}

void _lv_rectfromiitem(obj_s* pObj, listview_s* pOwner, int iItem, bool bHView, RECT* rcItem)
{
	int i = iItem - 1;
	listview_s lv;
	RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
	int nWidth = lv.width_item_ + lv.width_split_;
	int nHeight = lv.height_item_ + lv.height_split_;
	if (bHView)
	{
		(*rcItem).left = rcClient.left + (i / lv.count_view_v_) * nWidth - lv.nOffsetX_;
		(*rcItem).top = rcClient.top + (i % lv.count_view_v_) * nHeight - lv.nOffsetY_;
	}
	else {
		(*rcItem).left = rcClient.left + (i % lv.count_view_h_) * nWidth - lv.nOffsetX_;
		(*rcItem).top = rcClient.top + (i / lv.count_view_h_) * nHeight - lv.nOffsetY_;
	}
	(*rcItem).right = (*rcItem).left + lv.width_item_;
	(*rcItem).bottom = (*rcItem).top + lv.height_item_;
}

void _lv_lprectfromiitem(obj_s* pObj, listview_s* pOwner, int iItem, bool bHView, RECT* lpRc)
{
	if (lpRc == 0) return;
	RECT rcItem{ 0 };
	_lv_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
	RtlMoveMemory(lpRc, &rcItem, 16);
}

int _lv_itemfrompos(obj_s* pObj, listview_s* pOwner, int x, int y, int* offsetPosX, int* offsetPosY)
{
	*offsetPosX = 0;
	*offsetPosY = 0;
	POINT pt;
	pt.x = x;
	pt.y = y;
	int uItem = 0;
	if (PtInRect((RECT*)((size_t)pObj + offsetof(obj_s, c_left_)), pt))
	{
		listview_s lv;
		RECT rcClient{ 0 };
		RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
		RtlMoveMemory(&rcClient, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
		int nWidth = lv.width_item_ + lv.width_split_;
		int nHeight = lv.height_item_ + lv.height_split_;
		int realleft = 0;
		int offsety = 0;
		int offsetx = 0;
		int realtop = 0;
		if ((pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST)
		{
			realleft = x - rcClient.left + lv.nOffsetX_;
			offsetx = realleft % nWidth;
			if (offsetx<= lv.width_item_)
			{
				realtop = y - rcClient.top + lv.nOffsetY_;
				offsety = realtop % nHeight;
				if (offsety <= lv.height_item_)
				{
					int tmp = realtop / nHeight;
					if (tmp < lv.count_view_v_)
					{
						uItem = (realleft / nWidth) * lv.count_view_v_ + tmp +1;
						if (uItem > lv.count_items_)
						{
							uItem = 0;
						}
						else {
							*offsetPosX = offsetx;
							*offsetPosY = offsety;
						}
					}
				}
			}
		}
		else {
			realtop = y - rcClient.top + lv.nOffsetY_;
			offsety = realtop % nHeight;
			if (offsety <= lv.height_item_)
			{
				realleft = x - rcClient.left + lv.nOffsetX_;
				offsetx = realleft % nWidth;
				if (offsetx <= lv.width_item_)
				{
					int tmp = realleft / nWidth;
					if (tmp < lv.count_view_h_)
					{
						uItem = (realtop / nHeight) * lv.count_view_h_ + tmp +1;
						if (uItem > lv.count_items_)
						{
							uItem = 0;
						}
						else {
							*offsetPosX = offsetx;
							*offsetPosY = offsety;
						}
					}
				}
			}
		}
	}
	return uItem;
}

int _lv_getitemstate(void* lpItems, int iItem)
{
	return __get_int(lpItems, (iItem - 1) * 4);
}

bool _lv_queryitemstate(void* lpItems, int iItem, int dwState)
{
	return __query(lpItems, (iItem - 1) * 4, dwState);
}

void _lv_getsbvalue(obj_s* pObj, listview_s* pOwner, bool bHSB, HEXOBJ* hSB, int* oPos, int* nLine, int* nPage, int* nView)
{
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, (void*)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
	if (bHSB)
	{
		*oPos = pOwner->nOffsetX_;
		*hSB = pObj->objHScroll_;
		*nPage = rcClient.right - rcClient.left;
		*nLine = pOwner->width_item_ + pOwner->width_split_;
		*nView = pOwner->width_view_;
	}
	else {
		*oPos = pOwner->nOffsetY_;
		*hSB = pObj->objVScroll_;
		*nPage = rcClient.bottom - rcClient.top;
		*nLine = pOwner->height_item_ + pOwner->height_split_;
		*nView = pOwner->height_view_;
	}
}

int _lv_checkpos(int nPos, int nView, int nPage)
{
	if (nPos > nView - nPage)
	{
		nPos = nView - nPage;
	}
	if (nPos < 0) nPos = 0;
	return nPos;
}

void _lv_item_changestate(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, void* lpItems, int iItem, int state, bool bRemove, int nEvent, WPARAM wParam, LPARAM lParam)
{
	_lv_setitemstate(lpItems, iItem, state, bRemove);
	_lv_redrawitem(pObj, pOwner, iItem);
	if (nEvent != 0)
	{
		_obj_dispatchnotify(hWnd, pObj, hObj, 0, nEvent, wParam, lParam);
	}
}

void _lv_redrawitem(obj_s* pObj, listview_s* pOwner, int iItem)
{
	if (_lv_checkitem_view(pOwner, iItem))
	{
		RECT rcItem{ 0 };
		int nError = 0;
		_lv_rectfromiitem(pObj, pOwner, iItem, (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST, &rcItem);
		_obj_invalidaterect(pObj, &rcItem, &nError);
	}
}

size_t _lv_setitemcount(HWND hWnd, HEXOBJ hObj, obj_s* pObj, int nCount, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	bool bHView = (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST;
	void* pOld=pOwner->lpItems_;
	if (pOld != 0)
	{
		Ex_MemFree(pOld);
	}
	pOld = Ex_MemAlloc(nCount * 4);
	pOwner->lpItems_ = pOld;
	pOwner->count_items_ = nCount;
	pOwner->index_select_ = 0;
	pOwner->index_mouse_ = 0;
	pOwner->index_track_start_ = 0;
	pOwner->index_track_end_ = 0;
	pOwner->count_selects_ = 0;
	_lv_updatesbvalue(hObj, pObj, pOwner, (RECT*)&pObj->c_left_);
	int nPosX = 0;
	int nPosY = 0;
	int nPage = 0;
	int nView = 0;
	int nLine = 0;
	if ((lParam & 2) != 0)//LVSICF_NOSCROLL
	{
		HEXOBJ hSB = 0;
		_lv_getsbvalue(pObj, pOwner, true, &hSB, &nPosX, &nLine, &nPage, &nView);
		nPosX = _lv_checkpos(nPosX, nView, nPage);
		_lv_getsbvalue(pObj, pOwner, false, &hSB, &nPosY, &nLine, &nPage, &nView);
		nPosY = _lv_checkpos(nPosY, nView, nPage);
	}
	else {
		nPosX = 0;
		nPosY = 0;
	}
	pOwner->nOffsetX_ = nPosX;
	pOwner->nOffsetY_ = nPosY;
	Ex_ObjScrollSetPos(hObj, SB_HORZ, nPosX, true);
	Ex_ObjScrollSetPos(hObj, SB_VERT, nPosY, true);
	_lv_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
	int nError = 0;
	_obj_invalidaterect(pObj, 0, &nError);
	return 0;
}

void _lv_onvscrollbar(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	int nCode = LOWORD(wParam);
	bool bHView = (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST;
	bool bHScoll = uMsg == WM_HSCROLL;
	HEXOBJ hSB = 0;
	int oPos = 0;
	int nPage = 0;
	int nLine = 0;
	int nView = 0;
	_lv_getsbvalue(pObj, pOwner, bHScoll, &hSB, &oPos, &nLine, &nPage, &nView);
	int nPos = 0;
	if (nCode == SB_THUMBPOSITION)
	{
		nPos = Ex_ObjScrollGetTrackPos(hSB, SB_CTL);
	}
	else {
		if (nCode == SB_PAGEUP)
		{
			nPos = oPos - nPage;
		}
		else if (nCode == SB_PAGEDOWN)
		{
			nPos = oPos + nPage;
		}
		else if (nCode == SB_LINEUP)
		{
			nPos = oPos - nLine;
		}
		else if (nCode == SB_LINEDOWN)
		{
			nPos = oPos + nLine;
		}
		else if (nCode == SB_TOP)
		{
			nPos = 0;
		}
		else if (nCode == SB_BOTTOM)
		{
			nPos = nView - nPage;
		}
		else {
			return;
		}
	}
	nPos = _lv_checkpos(nPos, nView, nPage);
	if (nPos != oPos)
	{
		if (bHScoll)
		{
			pOwner->nOffsetX_ = nPos;
		}
		else {
			pOwner->nOffsetY_ = nPos;
		}
		if (hSB != 0)
		{
			Ex_ObjScrollSetPos(hSB, SB_CTL, nPos, true);
		}
		_lv_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
		int nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
}

void _lv_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, size_t wParram, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	void* lpItems = pOwner->lpItems_;
	int ox = 0;
	int oy = 0;
	int iCur = _lv_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
	
	bool vCur = _lv_checkitem(pOwner, iCur);
	if (vCur)
	{
		if (_lv_queryitemstate(lpItems, iCur, STATE_DISABLE))
		{
			return;
		}
	}
	int iSelect = pOwner->index_select_;
	bool vSelect = _lv_checkitem(pOwner, iSelect);
	int nCount = pOwner->count_items_;
	bool bSingleSel = !((pObj->dwStyle_ & ELS_ALLOWMULTIPLE) == ELS_ALLOWMULTIPLE);
	bool bShowAllwasy = ((pObj->dwStyle_ & ELS_SHOWSELALWAYS) == ELS_SHOWSELALWAYS);
	int iKey = _wnd_getfunctionkeys();
	if (!bSingleSel && uMsg == WM_RBUTTONDOWN)
	{
		if ((iKey & 1) != 0 || (iKey & 2) != 0)
		{
			return;
		}
	}
	if (vCur && iCur == iSelect)
	{
		if ((pObj->dwStyle_ & ELS_ITEMTRACKING) == ELS_ITEMTRACKING)
		{
			return;
		}
	}
	else {
		//取消选中
		int nSelects = pOwner->count_selects_;
		if (bSingleSel)
		{
			if (vSelect)
			{
				if (!((pObj->dwStyle_ & ELS_SHOWSELALWAYS) == ELS_SHOWSELALWAYS) || vCur)
				{
					_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, true, 0, 0, 0);
					nSelects = nSelects - 1;
				}
				else {
					return;
				}
			}
		}
		else {
			if ((iKey & 1) == 0)// ctrl没有按下,则取消所有选择
			{
				_lv_reselect(hWnd, hObj, pObj, pOwner,0,false);
			}
		}
		uMsg = 0;
		if (vCur)
		{
			if (bSingleSel && !vSelect)
			{
				_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, false, uMsg, iCur, iSelect);
				nSelects = nSelects + 1;
			}
			else {
				//判断功能键
				if ((iKey & 2) != 0 && !bSingleSel)//shift
				{
					int nStep = iCur > iSelect ? 1 : -1;
					for (int i = iSelect; i != iCur; i = i + nStep)
					{
						if (!_lv_queryitemstate(lpItems, i, STATE_SELECT))
						{
							if (!_lv_queryitemstate(lpItems, i, STATE_DISABLE))
							{
								_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, false,i == iCur ? uMsg : 0, i, iSelect);
								nSelects = nSelects + 1;
							}
						}
					}
					pOwner->count_selects_ = nSelects;
					return;
				}
				else {
					_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, false, uMsg, iCur, iSelect);
				}
			}
		}
		pOwner->index_select_ = iCur;
		pOwner->count_selects_ = nSelects;
	}
	if (iCur != iSelect)
	{
		_obj_dispatchnotify(hWnd, pObj, hObj, 0, LVN_ITEMCHANGED, iCur, iSelect); //有可能是0
	}
}

size_t _lv_reselect(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, int iItem, bool bShow)
{
	int nCount = pOwner->count_selects_;
	void* lpItems = pOwner->lpItems_;
	if (nCount > 0)
	{
		int nSelect = pOwner->count_items_;
		for (int i = 1; i < nCount + 1; i++)
		{
			if (_lv_queryitemstate(lpItems, i, STATE_SELECT))
			{
				_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, true, 0, 0, 0);
			}
		}
		pOwner->count_selects_ = 0;
	}
	if (iItem >= 0)
	{
		if (_lv_checkitem(pOwner, iItem))
		{
			nCount = pOwner->index_select_;
			pOwner->index_select_ = iItem;
			pOwner->count_selects_ = 1;
			_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iItem, STATE_SELECT, false, LVN_ITEMCHANGED, iItem, nCount);
			if (bShow)
			{
				_lv_showitem(hWnd, hObj, pObj, iItem, false);
			}
		}
	}
	return 0;
}

size_t _lv_showitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, int iItem, bool bCheck)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	if (bCheck)
	{
		if (!_lv_checkitem(pOwner, iItem))
		{
			return 0;
		}
	}
	RECT* lprc =(RECT*) ((size_t)pObj + offsetof(obj_s, c_left_));
	bool bHView = ((pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST);
	RECT rcItem{ 0 };
	_lv_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
	OffsetRect(&rcItem, -lprc->left, -lprc->top);
	HEXOBJ hSB = 0;
	int oPos = 0;
	int nPage = 0;
	int nLine = 0;
	int nView = 0;
	_lv_getsbvalue(pObj, pOwner, bHView, &hSB, &oPos, &nLine, &nPage, &nView);
	int nPos = 0;
	if (bHView)
	{
		if (rcItem.left < 0)
		{
			nPos = oPos + rcItem.left;
		}
		else {
			if (rcItem.right > nPage)
			{
				nPos = oPos + rcItem.right - nPage;
			}
			else {
				return 0;
			}
		}
	}
	else {
		if (rcItem.top < 0)
		{
			nPos = oPos + rcItem.top;
		}
		else {
			if (rcItem.bottom > nPage)
			{
				nPos = oPos + rcItem.bottom - nPage;
			}
			else {
				return 0;
			}
		}
	}
	nPos = _lv_checkpos(nPos, nView, nPage);
	if (bHView)
	{
		pOwner->nOffsetX_ = nPos;
	}
	else {
		pOwner->nOffsetY_ = nPos;
	}
	if (hSB != 0)
	{
		Ex_ObjScrollSetPos(hSB, SB_CTL, nPos, true);
	}
	_lv_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
	int nError = 0;
	_obj_invalidaterect(pObj, 0, &nError);
	return 0;
}

void _lv_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	void* lpItems = pOwner->lpItems_;
	int iLast = pOwner->index_mouse_;
	if (_lv_checkitem(pOwner, iLast))
	{
		_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, true, 0, 0, 0);
		pOwner->index_mouse_ = 0;
	}
}

void _lv_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	void* lpItems = pOwner->lpItems_;
	int iSelect = pOwner->index_select_;
	int iLast = pOwner->index_mouse_;
	bool vLast = _lv_checkitem(pOwner, iLast);
	int ox = 0;
	int oy = 0;
	int iCur = _lv_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
	bool vCur = _lv_checkitem(pOwner, iCur);
	int iHitTest = LVHT_NOWHERE;
	int tmp = 0;
	if (vCur)
	{
		tmp=_obj_baseproc(hWnd, hObj, pObj, LVM_HITTEST, iCur, MAKELONG(ox, oy));
		iHitTest = tmp == 0 ? LVHT_ONITEM : tmp;
	}
	pOwner->nHittest_ = iHitTest;
	bool bSingelSelect = !((pObj->dwStyle_ & ELS_ALLOWMULTIPLE) == ELS_ALLOWMULTIPLE);
	bool bDragdrop = ((pObj->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP);
	bool bShowAllwasy = ((pObj->dwStyle_ & ELS_SHOWSELALWAYS) == ELS_SHOWSELALWAYS);
	if (wParam != 1 || bSingelSelect)
	{
		if ((pObj->dwStyle_ & ELS_ITEMTRACKING) == ELS_ITEMTRACKING)
		{
			if (iCur != iSelect)
			{
				if (vCur || !bShowAllwasy)
				{
					if (iSelect != 0)
					{
						_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, true, 0, 0, 0);
					}
					pOwner->index_select_ = iCur;
				}
				if (vCur)
				{
					if (_lv_queryitemstate(lpItems, iCur, STATE_DISABLE))
					{
						iCur = 0;
					}
					else {
						_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, false, LVN_HOTTRACK, iCur, iSelect);
					}
				}
				_obj_dispatchnotify(hWnd, pObj, hObj, 0, LVN_ITEMCHANGED, iCur, iSelect);
			}
		}
		else
		{
			if (iCur != iLast)
			{
				if (vLast)
				{
					_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, true, 0, 0, 0);
				}
				if (vCur)
				{
					if (_lv_queryitemstate(lpItems, iCur, STATE_DISABLE))
					{
						iCur = 0;
					}
					else {
						_lv_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_HOVER, false, 0, 0, 0);
					}
				}
			}
		}
	}
	pOwner->index_mouse_ = iCur;
}

void _lv_drawitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, EX_PAINTSTRUCT2 ps, int iItem, RECT rcClip, RECT rcItem)
{
	int atomRect = 0;
	EX_CUSTOMDRAW ecd;
	ecd.hTheme = ps.hTheme;
	ecd.hCanvas = ps.hCanvas;
	ecd.dwState = _lv_getitemstate(pOwner->lpItems_, iItem);
	ecd.dwStyle = ps.dwStyle;
	ecd.rcPaint.left = rcItem.left;
	ecd.rcPaint.top = rcItem.top;
	ecd.rcPaint.right = rcItem.right;
	ecd.rcPaint.bottom = rcItem.bottom;
	ecd.iItem = iItem;
	if (_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CUSTOMDRAW, iItem, (size_t)&ecd) == 0)
	{
		if (ecd.dwState & STATE_SELECT) {
			atomRect = ATOM_SELECT;
		}
		else if (ecd.dwState & STATE_HOVER) {
			atomRect = ATOM_HOVER;
		}
		if (atomRect && pObj->crChecked_ != TRUE) {
			Ex_ThemeDrawControlEx(
				ecd.hTheme,
				ecd.hCanvas,
				ecd.rcPaint.left,
				ecd.rcPaint.top,
				ecd.rcPaint.right,
				ecd.rcPaint.bottom,
				ATOM_ITEM,
				atomRect,
				0,
				0,
				0,
				0,
				255
			);
		}
	}
}

size_t _lv_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT2 ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
		int iStart = pOwner->index_start_;
		int iEnd = pOwner->index_end_;
		bool bHView = (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST;
		for (int i = iStart; i < iEnd; i++)
		{
			if (_lv_checkitem_view(pOwner, i))
			{
				RECT rcItem{ 0 };
				_lv_rectfromiitem(pObj, pOwner, i, bHView, &rcItem);
				RECT rcClip{ 0 };
				if (IntersectRect(&rcClip, (RECT*)&ps.p_left, &rcItem))
				{
					_lv_drawitem(hWnd, hObj, pObj, pOwner, ps, i, rcClip, rcItem);
				}
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}

size_t _lv_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	bool bHView = (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST;
	RECT* lpRcClient = (RECT*)&pObj->c_left_;
	RECT rcClient{  };
	RtlMoveMemory(&rcClient, lpRcClient, sizeof(RECT));

	float iWidth = 0;
	float iHeight = 0;
	_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, L"S", 1, 0, 0, rcClient.right, rcClient.bottom, &iWidth, &iHeight);
	if (bHView)
	{
		iWidth = Ex_Scale(60);
		iHeight = Ex_Scale(60);
	}
	else {
		iWidth = (rcClient.right - rcClient.left - Ex_Scale(1)) / g_Li.DpiX;
		iHeight = iHeight / g_Li.DpiY + 8;
	}
	if (iWidth <= 0) iWidth = 1;
	if (iHeight <= 0) iHeight = 1;
	//项目尺寸
	pOwner->width_item_ = Ex_Scale(iWidth);
	pOwner->height_item_ = Ex_Scale(iHeight);
	//特殊点燃项尺寸
	pOwner->width_spec_ = pOwner->width_item_;
	pOwner->height_spec_ = pOwner->height_item_;
	//间隔尺寸
	pOwner->width_split_ = Ex_Scale(1);
	pOwner->height_split_ = Ex_Scale(1);
	_obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CALCSIZE, 0, (size_t)pOwner + offsetof(listview_s, width_item_));
	_lv_updatesbvalue(hObj, pObj, pOwner, lpRcClient);
	_lv_updateviewindex(hObj, pObj, pOwner, bHView, lpRcClient);
	return 0;
}

LRESULT CALLBACK _lv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		if (uMsg == WM_CREATE)
		{
			_struct_createfromaddr(pObj, offsetof(obj_s, dwOwnerData_), sizeof(listview_s), &nError);
			pObj->crChecked_ = FALSE;
		}
		else if (uMsg == WM_DESTROY)
		{
			_struct_destroyfromaddr(pObj, offsetof(obj_s, dwOwnerData_));
		}
		else if (uMsg == WM_SIZE)
		{
			_lv_size(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_PAINT)
		{
			return _lv_paint(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_lv_mouseleave(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_MOUSEMOVE)
		{
			_lv_mousemove(hWnd, hObj, pObj, wParam, lParam);
		}
		else if (uMsg == LVM_HITTEST)
		{
			return (((listview_s*)_obj_pOwner(pObj))->nHittest_);
		}
		else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
		{
			_lv_btndown(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP)
		{
			//_lv_btnup(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL)
		{
			_lv_onvscrollbar(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == LVM_SETITEMCOUNT)
		{
			return _lv_setitemcount(hWnd, hObj, pObj, wParam, lParam);
		}
		else if (uMsg == LVM_DELETEALLITEMS)
		{
			return _lv_setitemcount(hWnd, hObj, pObj, 0, 0);
		}
		else if (uMsg == LVM_ENSUREVISIBLE)
		{
			return _lv_showitem(hWnd, hObj, pObj, lParam, true);
		}
		else if (uMsg == LVM_GETITEMSTATE)
		{
			lParam = 0;
			if (_lv_checkitem((listview_s*)_obj_pOwner(pObj), wParam))
			{
				lParam = _lv_getitemstate(((listview_s*)_obj_pOwner(pObj))->lpItems_, wParam);
			}
			return lParam;
		}
		else if (uMsg == LVM_SETITEMSTATE)
		{
			_lv_lvm_setitemstate(pObj, (listview_s*)_obj_pOwner(pObj), wParam, lParam);
		}
		else if (uMsg == LVM_GETCOUNTPERPAGE)
		{
			return ((listview_s*)_obj_pOwner(pObj))->count_view_;
		}
		else if (uMsg == LVM_GETITEMCOUNT)
		{
			return ((listview_s*)_obj_pOwner(pObj))->count_items_;
		}
		else if (uMsg == LVM_GETITEMRECT)
		{
			if (_lv_checkitem((listview_s*)_obj_pOwner(pObj), wParam))
			{
				_lv_lprectfromiitem(pObj, (listview_s*)_obj_pOwner(pObj), wParam, (pObj->dwStyle_ & ELS_HORIZONTALLIST) == ELS_HORIZONTALLIST, (RECT*)lParam);
			}
		}
		else if (uMsg == LVM_GETSELECTEDCOUNT)
		{
			return ((listview_s*)_obj_pOwner(pObj))->count_selects_;
		}
		else if (uMsg == LVM_GETSELECTIONMARK)
		{
			return ((listview_s*)_obj_pOwner(pObj))->index_select_;
		}
		else if (uMsg == LVM_SETSELECTIONMARK)
		{
			return _lv_reselect(hWnd, hObj, pObj, (listview_s*)_obj_pOwner(pObj), lParam, wParam != 0);
		}
		else if (uMsg == LVM_GETTOPINDEX)
		{
			return ((listview_s*)_obj_pOwner(pObj))->index_start_;
		}
		else if (uMsg == LVM_GETHOTITEM)
		{
			return ((listview_s*)_obj_pOwner(pObj))->index_mouse_;
		}
		else if (uMsg == LVM_REDRAWITEMS)
		{
			for (int i = wParam; i < lParam; i++)
			{
				_lv_redrawitem(pObj, (listview_s*)_obj_pOwner(pObj), i);
			}
			return 0;
		}
		else if (uMsg == LVM_CALCITEMSIZE) {
			_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
		} 
		else if (uMsg == LVM_CANCELTHEME) {
			pObj->crChecked_ = wParam;
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _lv_lvm_setitemstate(obj_s* pObj, listview_s* pOwner, int iItem, int dwState)
{
	if (_lv_checkitem(pOwner, iItem))
	{
		if (iItem == pOwner->index_select_)
		{
			if ((dwState & STATE_SELECT) == 0)
			{
				pOwner->index_select_ = 0;
			}
		}
		void* lpItems = pOwner->lpItems_;
		__set_int(lpItems, (iItem - 1) * 4, dwState);
		_lv_redrawitem(pObj, pOwner, iItem);
	}
}