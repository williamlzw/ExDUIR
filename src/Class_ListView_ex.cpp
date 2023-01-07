#include "Class_ListView_ex.h"

ClsPROC m_pfnTListView;

void _tlv_array_del(array_s* hArray, int index, HEXOBJ pvData, int type)
{
	if (type == 0)
	{
		Ex_ObjSendMessage(Array_GetExtra(hArray), TLVM_ITEM_DESTROY, 0, pvData);
		Ex_ObjDestroy(pvData);
	}
}

void _tlv_refill(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, LONG_PTR iStart, LONG_PTR iStartOld, LONG_PTR iEnd, LONG_PTR iEndOld)
{
	int nIndex = 0;
	array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
	int nItemCount = pOwner->count_items_;
	int nCount = iEnd - iStart;
	int nCountOld = iEndOld - iStartOld;

	if (nCount != nCountOld)
	{
		if (nCount < nCountOld)
		{
			for (int i = nCountOld; i >= nCount; i--)
			{
				Array_DelMember(arr, i);
			}
		}
		else
		{
			for (int i = nCountOld; i <= nCount; i++)
			{
				HEXOBJ hObjItem = Ex_ObjSendMessage(hObj, TLVM_ITEM_CREATE, 0, 0);
				Array_AddMember(arr, hObjItem);
			}
		}
	}

	INT iSelect = pOwner->index_select_;
	Ex_ObjSetLong(hObj, 4, (LONG_PTR)iStart);
	Ex_ObjSetLong(hObj, 5, (LONG_PTR)iEnd);
	for (int i = 1; i <= Array_GetCount(arr); i++)
	{
		HEXOBJ hObjItem = Array_GetMember(arr, i);
		nIndex = iStart + i - 1;
		if (nIndex >= iStart && nIndex <= iEnd && nIndex <= nItemCount)
		{

			Ex_ObjSendMessage(hObj, TLVM_ITEM_FILL, nIndex, hObjItem);
			if (nIndex > 0 && nIndex == iSelect && iSelect != 0)//多选还需要修改判断
			{
				Ex_ObjSetColor(hObjItem, COLOR_EX_BACKGROUND, (EXARGB)Ex_ObjGetLong(hObj, TLVL_ITEM_SELECTCOLOR), TRUE);//新增
			}
			else
			{
				Ex_ObjSetColor(hObjItem, COLOR_EX_BACKGROUND, 0, TRUE);//新增
			}
			Ex_ObjSetLong(hObjItem, 0, nIndex);

			Ex_ObjShow(hObjItem, true);
		}
		else
		{
			Ex_ObjShow(hObjItem, false);
		}
	}
}

void _tlv_repos_items(HEXOBJ hObj, obj_s* pObj, EX_PAINTSTRUCT& ps)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	INT iStart = pOwner->index_start_;
	INT iEnd = pOwner->index_end_;
	INT nOffsetX = pOwner->nOffsetX_;
	INT nOffsetY = pOwner->nOffsetY_;
	INT nError = 0;
	bool bHView = (((pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST));
	INT iStartOld = Ex_ObjGetLong(hObj, 4);
	INT iEndOld = Ex_ObjGetLong(hObj, 5);
	INT nOffsetXOld = Ex_ObjGetLong(hObj, 6);
	INT nOffsetYOld = Ex_ObjGetLong(hObj, 7);

	_tlv_refill(hObj, pObj, pOwner, iStart, iStartOld, iEnd, iEndOld);
	
	if (nOffsetX != nOffsetXOld || nOffsetY != nOffsetYOld)
	{
		Ex_ObjSetLong(hObj, 6, nOffsetX);
		Ex_ObjSetLong(hObj, 7, nOffsetY);

		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
		int count = Array_GetCount(arr);
		//数组从1开始
		for (int i = 1; i <= count; i++)
		{
			HEXOBJ hObjItem = Array_GetMember(arr, i);
			obj_s* pObjItem = nullptr;
			if (_handle_validate(hObjItem, HT_OBJECT, (LPVOID*)&pObjItem, &nError))
			{
				int nIndex = Ex_ObjGetLong(hObjItem, 0);
				if (_listview_checkitem_view(pOwner, nIndex))
				{
					RECT rcItem = { 0 };
					_listview_rectfromiitem(pObj, pOwner, nIndex, bHView, &rcItem);
					Ex_ObjMove(hObjItem, rcItem.left, rcItem.top, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top, false);
				}
			}
		}
	}
}

void _tlv_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, size_t wParram, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	INT x = GET_X_LPARAM(lParam) / g_Li.DpiX;
	INT y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
	LPVOID lpItems = pOwner->lpItems_;
	INT ox = 0;
	INT oy = 0;
	INT iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
	BOOL vCur = _listview_checkitem(pOwner, iCur);
	if (vCur)
	{
		if (_listview_queryitemstate(lpItems, iCur, STATE_DISABLE))
		{
			return;
		}
	}
	INT iSelect = pOwner->index_select_;
	BOOL vSelect = _listview_checkitem(pOwner, iSelect);
	INT nCount = pOwner->count_items_;
	BOOL bSingleSel = !((pObj->dwStyle_ & ELVS_ALLOWMULTIPLE) == ELVS_ALLOWMULTIPLE);
	BOOL bShowAllwasy = ((pObj->dwStyle_ & ELVS_SHOWSELALWAYS) == ELVS_SHOWSELALWAYS);
	INT iKey = _wnd_getfunctionkeys();
	if (!bSingleSel && uMsg == WM_RBUTTONDOWN)
	{
		if ((iKey & 1) != 0 || (iKey & 2) != 0)
		{
			return;
		}
	}
	if (vCur && iCur == iSelect)
	{
		if ((pObj->dwStyle_ & ELVS_ITEMTRACKING) == ELVS_ITEMTRACKING)
		{
			return;
		}
	}
	else
	{
		//取消选中
		INT nSelects = pOwner->count_selects_;
		if (bSingleSel)
		{
			if (vSelect)
			{
				if (!((pObj->dwStyle_ & ELVS_SHOWSELALWAYS) == ELVS_SHOWSELALWAYS) || vCur)
				{
					_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, TRUE, 0, 0, 0);
					nSelects = nSelects - 1;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			if ((iKey & 1) == 0) // ctrl没有按下,则取消所有选择
			{
				_listview_reselect(hWnd, hObj, pObj, pOwner, 0, FALSE);
			}
		}
		uMsg = 0;
		if (vCur)
		{
			if (bSingleSel && !vSelect)
			{
				_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, uMsg, iCur, iSelect);
				nSelects = nSelects + 1;
			}
			else
			{
				//判断功能键
				if ((iKey & 2) != 0 && !bSingleSel) //shift
				{
					INT nStep = iCur > iSelect ? 1 : -1;
					for (INT i = iSelect; i != iCur; i = i + nStep)
					{
						if (!_listview_queryitemstate(lpItems, i, STATE_SELECT))
						{
							if (!_listview_queryitemstate(lpItems, i, STATE_DISABLE))
							{
								_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, FALSE, i == iCur ? uMsg : 0, i, iSelect);
								nSelects = nSelects + 1;
							}
						}
					}
					pOwner->count_selects_ = nSelects;
					return;
				}
				else
				{
					_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, uMsg, iCur, iSelect);
				}
			}
		}
		pOwner->index_select_ = iCur;
		pOwner->count_selects_ = nSelects;
	}
	array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
	if (arr)
	{
		INT iStart = pOwner->index_start_;
		auto num = Array_GetCount(arr);
		HEXOBJ hobjitem = Array_GetMember(arr, iCur - iStart + 1);
		if (iCur != iSelect)
		{
			if ((iKey & 2) != 0 && !bSingleSel)//shift
			{
				//..多选处理
			}
			else
			{
				Ex_ObjSetColor(Array_GetMember(arr, iSelect - iStart + 1), COLOR_EX_BACKGROUND, 0, TRUE);
			}
			Ex_ObjSetColor(hobjitem, COLOR_EX_BACKGROUND, (EXARGB)Ex_ObjGetLong(hObj, TLVL_ITEM_SELECTCOLOR), TRUE);
			_obj_dispatchnotify(hWnd, pObj, hObj, 0, LVN_ITEMCHANGED, iSelect, iCur); //iSelect有可能是0,为上次选中  // hobjitem
		}
		else
		{
			///....多选处理
		}
	}
}

void _tlv_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam)
{
	INT x = GET_X_LPARAM(lParam) / g_Li.DpiX;
	INT y = GET_Y_LPARAM(lParam) / g_Li.DpiY;
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	LPVOID lpItems = pOwner->lpItems_;
	INT iSelect = pOwner->index_select_;
	INT iLast = pOwner->index_mouse_;
	BOOL vLast = _listview_checkitem(pOwner, iLast);
	INT ox = 0;
	INT oy = 0;
	INT iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
	BOOL vCur = _listview_checkitem(pOwner, iCur);
	INT iHitTest = LVHT_NOWHERE;
	INT tmp = 0;
	if (vCur)
	{
		tmp = _obj_baseproc(hWnd, hObj, pObj, LVM_HITTEST, iCur, MAKELONG(ox, oy));
		iHitTest = tmp == 0 ? LVHT_ONITEM : tmp;
	}
	pOwner->nHittest_ = iHitTest;
	if (wParam != 1)
	{
		if (iCur != iLast)
		{
			if (vCur)
			{
				if (_listview_queryitemstate(lpItems, iCur, STATE_DISABLE))
				{
					iCur = 0;
				}
				else
				{
					array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
					if (arr)
					{
						listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
						INT iStart = pOwner->index_start_;
						HEXOBJ hobjitem = Array_GetMember(arr, iCur - iStart + 1);
						if (iSelect != iLast)
						{
							Ex_ObjSetColor(Array_GetMember(arr, iLast - iStart + 1), COLOR_EX_BACKGROUND, 0, TRUE);
						}
							
						if (iSelect != iCur)
						{
							Ex_ObjSetColor(hobjitem, COLOR_EX_BACKGROUND, (EXARGB)Ex_ObjGetLong(hObj, TLVL_ITEM_HOVERCOLOR), TRUE);//ExRGB2ARGB(15066083, 200)
						}
						else
						{
							Ex_ObjSetColor(hobjitem, COLOR_EX_BACKGROUND, (EXARGB)Ex_ObjGetLong(hObj, TLVL_ITEM_SELECTCOLOR), TRUE);//ExRGB2ARGB(255, 255)
						}
						_obj_dispatchnotify(hWnd, pObj, hObj, 0, LVN_HOTTRACK, iLast, iCur);
					}
				}
			}
		}
	}
	pOwner->index_mouse_ = iCur;
}

void _tlv_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	LPVOID lpItems = pOwner->lpItems_;
	INT iLast = pOwner->index_mouse_;
	if (_listview_checkitem(pOwner, iLast))
	{
		_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, TRUE, 0, 0, 0);
		array_s* arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
		if (arr)
		{
			listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
			INT iStart = pOwner->index_start_;
			INT iSelect = pOwner->index_select_;
			if (iSelect != iLast)//选中项不清除颜色  多选还需要额外判断
			{
				Ex_ObjSetColor(Array_GetMember(arr, iLast - iStart + 1), COLOR_EX_BACKGROUND, 0, TRUE);
			}
		}
		pOwner->index_mouse_ = 0;
	}
}

LRESULT CALLBACK _tlv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	array_s* arr = nullptr;
	void* ptr = nullptr;
	LONG_PTR tmp = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (uMsg == WM_CREATE)
		{
			arr = Array_Create(0);
			Array_BindEvent(arr, eae_delmember, _tlv_array_del);
			Array_SetExtra(arr, hObj);
			Ex_ObjSetLong(hObj, TLVL_ITEM_ARRAY, (LONG_PTR)arr);
			Ex_ObjSetLong(hObj, TLVL_ITEM_HOVERCOLOR, ExRGB2ARGB(15066083, 200));//表项悬浮色
			Ex_ObjSetLong(hObj, TLVL_ITEM_SELECTCOLOR, ExRGB2ARGB(255, 250));//表项选中色
		}
		else if (uMsg == WM_DESTROY)
		{
			arr = (array_s*)Ex_ObjGetLong(hObj, TLVL_ITEM_ARRAY);
			if (arr)
			{
				Array_Destroy(arr);
			}
		}
		else if (uMsg == WM_SIZE)
		{
			Ex_ObjSetLong(hObj, 6, -123321);
			Ex_ObjSetLong(hObj, 7, -321123);
		}
		else if (uMsg == WM_PAINT)
		{
			EX_PAINTSTRUCT ps{ 0 };
			if (Ex_ObjBeginPaint(hObj, &ps))
			{
				_tlv_repos_items(hObj, pObj, ps);
				Ex_ObjEndPaint(hObj, &ps);
			}
			return 0;
		}
		else if (uMsg == WM_MOUSEMOVE)
		{
			_tlv_mousemove(hWnd, hObj, pObj, wParam, lParam);
			return 0;//不执行父类的WM_MOUSEMOVE
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_tlv_mouseleave(hWnd, hObj, pObj);
			return 0;//不执行父类
		}
		else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
		{
			_tlv_btndown(hWnd, hObj, pObj, uMsg, wParam, lParam);
			return 0;//不执行父类
		}
		else if (uMsg == TLVM_ITEM_CREATE)
		{
			HEXOBJ handle = Ex_ObjCreate((LPCWSTR)Ex_Atom(L"LISTITEM"), 0, -1, 0, 0, 0, 0, hObj);
			_obj_baseproc(hWnd, hObj, pObj, TLVM_ITEM_CREATED, 0, handle);
			return handle;
		}
		else if (uMsg == TLVM_ITEM_DESTROY)
		{
			return 0;
		}
		else if (uMsg == TLVM_ITEM_FILL)
		{
			return 0;
		}
		else if (uMsg == TLVM_SETTEMPLATE)
		{
			return 0;
		}
		else if (uMsg == TLVM_SET_ITEM_HOVERCOLOR)
		{
			Ex_ObjSetLong(hObj, TLVL_ITEM_HOVERCOLOR, lParam);//表项悬浮色
		}
		else if (uMsg == TLVM_SET_ITEM_SELECTCOLOR)
		{
			Ex_ObjSetLong(hObj, TLVL_ITEM_SELECTCOLOR, lParam);//表项选中色
		}
	}
	return m_pfnTListView(hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _tlvi_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		if (uMsg == WM_PAINT)
		{
			_static_paint(hObj, pObj);
		}
		else if (uMsg == WM_MOUSEWHEEL)
		{
			return Ex_ObjDispatchMessage(Ex_ObjGetParent(hObj), uMsg, wParam, lParam);
		}
		else if (uMsg == WM_NOTIFY)
		{
			size_t tmp = 0;
			if (__get((void*)lParam, 0) != hObj)
			{
				tmp = _obj_dispatchnotify(_obj_gethwnd(pObj), pObj, hObj, 0, 1001, _obj_getextralong(pObj, 0), lParam);
			}
			return tmp;
		}
	}
	
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _listview_regsiter()
{
	Ex_ObjRegister(L"ListView", EOS_VISIBLE | EOS_HSCROLL | EOS_VSCROLL, EOS_EX_FOCUSABLE | EOS_EX_TABSTOP, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER, 0, 0, 0, _listview_proc);
	EX_CLASSINFO clsInfo{ 0 };
	Ex_ObjGetClassInfoEx(L"ListView", &clsInfo);
	m_pfnTListView = clsInfo.pfnClsProc;
	_obj_register(Ex_Atom(L"TListView"), clsInfo.dwStyle, clsInfo.dwStyleEx, clsInfo.dwTextFormat, 10 * sizeof(size_t), clsInfo.hCursor, _tlv_proc, clsInfo.dwFlags, 0);
	_obj_register(Ex_Atom(L"LISTITEM"), EOS_VISIBLE, EOS_EX_TRANSPARENT, DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_CENTER, 10 * sizeof(size_t), clsInfo.hCursor, _tlvi_proc, clsInfo.dwFlags, 0);
}

void _listview_setitemstate(LPVOID lpItems, INT iItem, INT dwState, BOOL bRemove)
{
	INT offset = (iItem - 1) * 4;
	if (bRemove)
	{
		__del(lpItems, offset, dwState);
	}
	else
	{
		__add(lpItems, offset, dwState);
	}
}

void _listview_updatesbvalue(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, RECT* lpRcClient)
{
	HEXOBJ hHSB = pObj->objHScroll_;
	HEXOBJ hVSB = pObj->objVScroll_;
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, lpRcClient, 16);
	INT width = rcClient.right - rcClient.left;
	INT height = rcClient.bottom - rcClient.top;
	BOOL bHView = ((pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST);
	INT nCount = pOwner->count_items_;
	INT iWidth = pOwner->width_item_ + pOwner->width_split_;
	INT iHeight = pOwner->height_item_ + pOwner->height_split_;
	INT nVS = 0;
	INT nHS = 0;
	if (bHView)
	{
		nVS = height / iHeight;
		if (nVS == 0)
		{
			nVS = 1;
		}
		nHS = width / iWidth + 1;
	}
	else
	{
		nHS = width / iWidth;
		if (nHS == 0)
		{
			nHS = 1;
		}
		nVS = height / iHeight + 1;
	}
	INT vWidth = 0;
	INT vHeight = 0;
	if (bHView) //横向排列
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
	else
	{
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
		Ex_ObjScrollSetInfo(hVSB, SB_CTL, SIF_PAGE | SIF_RANGE, 0, vHeight - height, height, 0, FALSE);
	}
	if (hHSB != 0)
	{
		Ex_ObjScrollShow(hHSB, SB_CTL, vWidth - width > 0);
		Ex_ObjScrollSetInfo(hHSB, SB_CTL, SIF_PAGE | SIF_RANGE, 0, vWidth - width, width, 0, FALSE);
	}
}

void _listview_updateviewindex(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, BOOL bHView, RECT* lpRcClient)
{
	listview_s lv;
	RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, lpRcClient, 16);
	INT nWidth = lv.width_item_ + lv.width_split_;
	INT nHeight = lv.height_item_ + lv.height_split_;
	INT nOffset = 0;
	INT iStart = 0;
	INT iEnd = 0;
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
	else
	{
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

BOOL _listview_checkitem(listview_s* pOwner, INT iItem)
{
	return iItem > 0 && pOwner->count_items_ >= iItem;
}

BOOL _listview_checkitem_view(listview_s* pOwner, INT iItem)
{
	BOOL ret = FALSE;
	if (_listview_checkitem(pOwner, iItem))
	{
		ret = iItem >= pOwner->index_start_ && iItem <= pOwner->index_end_;
	}
	return ret;
}

void _listview_rectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* rcItem)
{
	INT i = iItem - 1;
	listview_s lv;
	RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
	INT nWidth = lv.width_item_ + lv.width_split_;
	INT nHeight = lv.height_item_ + lv.height_split_;
	if (bHView)
	{
		(*rcItem).left = rcClient.left + (i / lv.count_view_v_) * nWidth - lv.nOffsetX_;
		(*rcItem).top = rcClient.top + (i % lv.count_view_v_) * nHeight - lv.nOffsetY_;
	}
	else
	{
		(*rcItem).left = rcClient.left + (i % lv.count_view_h_) * nWidth - lv.nOffsetX_;
		(*rcItem).top = rcClient.top + (i / lv.count_view_h_) * nHeight - lv.nOffsetY_;
	}
	(*rcItem).right = (*rcItem).left + lv.width_item_;
	(*rcItem).bottom = (*rcItem).top + lv.height_item_;
}

void _listview_lprectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* lpRc)
{
	if (lpRc == 0)
	{
		return;
	}
	RECT rcItem{ 0 };
	_listview_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
	RtlMoveMemory(lpRc, &rcItem, 16);
}

INT _listview_itemfrompos(obj_s* pObj, listview_s* pOwner, INT x, INT y, INT* offsetPosX, INT* offsetPosY)
{
	*offsetPosX = 0;
	*offsetPosY = 0;
	POINT pt;
	pt.x = x;
	pt.y = y;
	INT uItem = 0;
	if (PtInRect((RECT*)((size_t)pObj + offsetof(obj_s, c_left_)), pt))
	{
		listview_s lv;
		RECT rcClient{ 0 };
		RtlMoveMemory(&lv, pOwner, sizeof(listview_s));
		RtlMoveMemory(&rcClient, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
		INT nWidth = lv.width_item_ + lv.width_split_;
		INT nHeight = lv.height_item_ + lv.height_split_;
		INT realleft = 0;
		INT offsety = 0;
		INT offsetx = 0;
		INT realtop = 0;
		if ((pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST)
		{
			realleft = x - rcClient.left + lv.nOffsetX_;
			offsetx = realleft % nWidth;
			if (offsetx <= lv.width_item_)
			{
				realtop = y - rcClient.top + lv.nOffsetY_;
				offsety = realtop % nHeight;
				if (offsety <= lv.height_item_)
				{
					INT tmp = realtop / nHeight;
					if (tmp < lv.count_view_v_)
					{
						uItem = (realleft / nWidth) * lv.count_view_v_ + tmp + 1;
						if (uItem > lv.count_items_)
						{
							uItem = 0;
						}
						else
						{
							*offsetPosX = offsetx;
							*offsetPosY = offsety;
						}
					}
				}
			}
		}
		else
		{
			realtop = y - rcClient.top + lv.nOffsetY_;
			offsety = realtop % nHeight;
			if (offsety <= lv.height_item_)
			{
				realleft = x - rcClient.left + lv.nOffsetX_;
				offsetx = realleft % nWidth;
				if (offsetx <= lv.width_item_)
				{
					INT tmp = realleft / nWidth;
					if (tmp < lv.count_view_h_)
					{
						uItem = (realtop / nHeight) * lv.count_view_h_ + tmp + 1;
						if (uItem > lv.count_items_)
						{
							uItem = 0;
						}
						else
						{
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

INT _listview_getitemstate(LPVOID lpItems, INT iItem)
{
	return __get_int(lpItems, (iItem - 1) * 4);
}

BOOL _listview_queryitemstate(LPVOID lpItems, INT iItem, INT dwState)
{
	return __query(lpItems, (iItem - 1) * 4, dwState);
}

void _listview_getscrollbarvalue(obj_s* pObj, listview_s* pOwner, BOOL bHSB, HEXOBJ* hSB, INT* oPos, INT* nLine, INT* nPage, INT* nView)
{
	RECT rcClient{ 0 };
	RtlMoveMemory(&rcClient, (LPVOID)((size_t)pObj + offsetof(obj_s, c_left_)), 16);
	if (bHSB)
	{
		*oPos = pOwner->nOffsetX_;
		*hSB = pObj->objHScroll_;
		*nPage = rcClient.right - rcClient.left;
		*nLine = pOwner->width_item_ + pOwner->width_split_;
		*nView = pOwner->width_view_;
	}
	else
	{
		*oPos = pOwner->nOffsetY_;
		*hSB = pObj->objVScroll_;
		*nPage = rcClient.bottom - rcClient.top;
		*nLine = pOwner->height_item_ + pOwner->height_split_;
		*nView = pOwner->height_view_;
	}
}

INT _listview_checkpos(INT nPos, INT nView, INT nPage)
{
	if (nPos > nView - nPage)
	{
		nPos = nView - nPage;
	}
	if (nPos < 0)
	{
		nPos = 0;
	}
	return nPos;
}

void _listview_item_changestate(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, LPVOID lpItems, INT iItem, INT state, BOOL bRemove, INT nEvent, WPARAM wParam, LPARAM lParam)
{
	_listview_setitemstate(lpItems, iItem, state, bRemove);
	_listview_redrawitem(pObj, pOwner, iItem);
	if (nEvent != 0)
	{
		_obj_dispatchnotify(hWnd, pObj, hObj, 0, nEvent, wParam, lParam);
	}
}

void _listview_redrawitem(obj_s* pObj, listview_s* pOwner, INT iItem)
{
	if (_listview_checkitem_view(pOwner, iItem))
	{
		RECT rcItem{ 0 };
		INT nError = 0;
		_listview_rectfromiitem(pObj, pOwner, iItem, (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST, &rcItem);
		_obj_invalidaterect(pObj, &rcItem, &nError);
	}
}

size_t _listview_setitemcount(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT nCount, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	BOOL bHView = (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST;
	LPVOID pOld = pOwner->lpItems_;
	if (pOld != 0)
	{
		Ex_MemFree(pOld);
	}
	//取 提交的项目索引和当前项目索引
	int index = HIWORD(lParam);
	int index_select = pOwner->index_select_;
	if (nCount < 1)
	{
		index = 0;
		index_select = 0;
	}
	pOld = Ex_MemAlloc(nCount * 4);
	pOwner->lpItems_ = pOld;
	pOwner->count_items_ = nCount;
	pOwner->index_select_ = 0;
	pOwner->index_mouse_ = 0;
	pOwner->count_selects_ = 0;
	_listview_updatesbvalue(hObj, pObj, pOwner, (RECT*)&pObj->c_left_);
	INT nPosX = 0;
	INT nPosY = 0;
	INT nPage = 0;
	INT nView = 0;
	INT nLine = 0;
	if ((LOWORD(lParam) & LVSICF_NOSCROLL) != LVSICF_NOSCROLL)
	{
		HEXOBJ hSB = 0;
		_listview_getscrollbarvalue(pObj, pOwner, TRUE, &hSB, &nPosX, &nLine, &nPage, &nView);
		nPosX = _listview_checkpos(nPosX, nView, nPage);
		_listview_getscrollbarvalue(pObj, pOwner, FALSE, &hSB, &nPosY, &nLine, &nPage, &nView);
		nPosY = _listview_checkpos(nPosY, nView, nPage);
	}
	else
	{
		nPosX = 0;
		nPosY = 0;
	}
	pOwner->nOffsetX_ = nPosX;
	pOwner->nOffsetY_ = nPosY;
	Ex_ObjScrollSetPos(hObj, SB_HORZ, nPosX, TRUE);
	Ex_ObjScrollSetPos(hObj, SB_VERT, nPosY, TRUE);
	_listview_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
	INT nError = 0;
	_obj_invalidaterect(pObj, 0, &nError);
	//判断是否需要选中项目
	if (nCount > 0 && index > 0)
	{
		if (index >= nCount)
		{
			_listview_reselect(hWnd, hObj, pObj, pOwner, index, TRUE);
		}
	}
	return 0;
}

void _listview_onvscrollbar(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	INT nCode = LOWORD(wParam);
	BOOL bHView = (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST;
	BOOL bHScoll = uMsg == WM_HSCROLL;
	HEXOBJ hSB = 0;
	INT oPos = 0;
	INT nPage = 0;
	INT nLine = 0;
	INT nView = 0;
	_listview_getscrollbarvalue(pObj, pOwner, bHScoll, &hSB, &oPos, &nLine, &nPage, &nView);
	INT nPos = 0;
	if (nCode == SB_THUMBPOSITION)
	{
		nPos = Ex_ObjScrollGetTrackPos(hSB, SB_CTL);
	}
	else
	{
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
		else
		{
			return;
		}
	}
	nPos = _listview_checkpos(nPos, nView, nPage);
	if (nPos != oPos)
	{
		if (bHScoll)
		{
			pOwner->nOffsetX_ = nPos;
		}
		else
		{
			pOwner->nOffsetY_ = nPos;
		}
		if (hSB != 0)
		{
			Ex_ObjScrollSetPos(hSB, SB_CTL, nPos, TRUE);
		}
		_listview_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
		INT nError = 0;
		_obj_invalidaterect(pObj, 0, &nError);
	}
}

void _listview_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, size_t wParram, LPARAM lParam)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	INT x = GET_X_LPARAM(lParam);
	INT y = GET_Y_LPARAM(lParam);
	LPVOID lpItems = pOwner->lpItems_;
	INT ox = 0;
	INT oy = 0;
	INT iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);

	BOOL vCur = _listview_checkitem(pOwner, iCur);
	if (vCur)
	{
		if (_listview_queryitemstate(lpItems, iCur, STATE_DISABLE))
		{
			return;
		}
	}
	INT iSelect = pOwner->index_select_;
	BOOL vSelect = _listview_checkitem(pOwner, iSelect);
	INT nCount = pOwner->count_items_;
	BOOL bSingleSel = !((pObj->dwStyle_ & ELVS_ALLOWMULTIPLE) == ELVS_ALLOWMULTIPLE);
	BOOL bShowAllwasy = ((pObj->dwStyle_ & ELVS_SHOWSELALWAYS) == ELVS_SHOWSELALWAYS);
	INT iKey = _wnd_getfunctionkeys();
	if (!bSingleSel && uMsg == WM_RBUTTONDOWN)
	{
		if ((iKey & 1) != 0 || (iKey & 2) != 0)
		{
			return;
		}
	}
	if (vCur && iCur == iSelect)
	{
		if ((pObj->dwStyle_ & ELVS_ITEMTRACKING) == ELVS_ITEMTRACKING)
		{
			return;
		}
	}
	else
	{
		//取消选中
		INT nSelects = pOwner->count_selects_;
		if (bSingleSel)
		{
			if (vSelect)
			{
				if (!((pObj->dwStyle_ & ELVS_SHOWSELALWAYS) == ELVS_SHOWSELALWAYS) || vCur)
				{
					_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, TRUE, 0, 0, 0);
					nSelects = nSelects - 1;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			if ((iKey & 1) == 0) // ctrl没有按下,则取消所有选择
			{
				_listview_reselect(hWnd, hObj, pObj, pOwner, 0, FALSE);
			}
		}
		uMsg = 0;
		if (vCur)
		{
			if (bSingleSel && !vSelect)
			{
				_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, uMsg, iCur, iSelect);
				nSelects = nSelects + 1;
			}
			else
			{
				//判断功能键
				if ((iKey & 2) != 0 && !bSingleSel) // shift
				{
					INT nStep = iCur > iSelect ? 1 : -1;
					for (INT i = iSelect; i != iCur; i = i + nStep)
					{
						if (!_listview_queryitemstate(lpItems, i, STATE_SELECT))
						{
							if (!_listview_queryitemstate(lpItems, i, STATE_DISABLE))
							{
								_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, FALSE, i == iCur ? uMsg : 0, i, iSelect);
								nSelects = nSelects + 1;
							}
						}
					}
					pOwner->count_selects_ = nSelects;
					return;
				}
				else
				{
					_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, uMsg, iCur, iSelect);
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

size_t _listview_reselect(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bShow)
{
	INT nCount = pOwner->count_selects_;
	LPVOID lpItems = pOwner->lpItems_;
	if (nCount > 0)
	{
		INT nSelect = pOwner->count_items_;
		for (INT i = 1; i < nCount + 1; i++)
		{
			if (_listview_queryitemstate(lpItems, i, STATE_SELECT))
			{
				_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, i, STATE_SELECT, TRUE, 0, 0, 0);
			}
		}
		pOwner->count_selects_ = 0;
	}
	if (iItem >= 0)
	{
		if (_listview_checkitem(pOwner, iItem))
		{
			nCount = pOwner->index_select_;
			pOwner->index_select_ = iItem;
			pOwner->count_selects_ = 1;
			_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iItem, STATE_SELECT, FALSE, LVN_ITEMCHANGED, iItem, nCount);
			if (bShow)
			{
				_listview_showitem(hWnd, hObj, pObj, iItem, FALSE);
			}
		}
	}
	return 0;
}

size_t _listview_showitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT iItem, BOOL bCheck)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	if (bCheck)
	{
		if (!_listview_checkitem(pOwner, iItem))
		{
			return 0;
		}
	}
	RECT* lprc = (RECT*)((size_t)pObj + offsetof(obj_s, c_left_));
	BOOL bHView = ((pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST);
	RECT rcItem{ 0 };
	_listview_rectfromiitem(pObj, pOwner, iItem, bHView, &rcItem);
	OffsetRect(&rcItem, -lprc->left, -lprc->top);
	HEXOBJ hSB = 0;
	INT oPos = 0;
	INT nPage = 0;
	INT nLine = 0;
	INT nView = 0;
	_listview_getscrollbarvalue(pObj, pOwner, bHView, &hSB, &oPos, &nLine, &nPage, &nView);
	INT nPos = 0;
	if (bHView)
	{
		if (rcItem.left < 0)
		{
			nPos = oPos + rcItem.left;
		}
		else
		{
			if (rcItem.right > nPage)
			{
				nPos = oPos + rcItem.right - nPage;
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		if (rcItem.top < 0)
		{
			nPos = oPos + rcItem.top;
		}
		else
		{
			if (rcItem.bottom > nPage)
			{
				nPos = oPos + rcItem.bottom - nPage;
			}
			else
			{
				return 0;
			}
		}
	}
	nPos = _listview_checkpos(nPos, nView, nPage);
	if (bHView)
	{
		pOwner->nOffsetX_ = nPos;
	}
	else
	{
		pOwner->nOffsetY_ = nPos;
	}
	if (hSB != 0)
	{
		Ex_ObjScrollSetPos(hSB, SB_CTL, nPos, TRUE);
	}
	_listview_updateviewindex(hObj, pObj, pOwner, bHView, (RECT*)&pObj->c_left_);
	INT nError = 0;
	_obj_invalidaterect(pObj, 0, &nError);
	return 0;
}

void _listview_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	LPVOID lpItems = pOwner->lpItems_;
	INT iLast = pOwner->index_mouse_;
	if (_listview_checkitem(pOwner, iLast))
	{
		_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, TRUE, 0, 0, 0);
		pOwner->index_mouse_ = 0;
	}
}

void _listview_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam)
{
	INT x = GET_X_LPARAM(lParam);
	INT y = GET_Y_LPARAM(lParam);
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	LPVOID lpItems = pOwner->lpItems_;
	INT iSelect = pOwner->index_select_;
	INT iLast = pOwner->index_mouse_;
	BOOL vLast = _listview_checkitem(pOwner, iLast);
	INT ox = 0;
	INT oy = 0;
	INT iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
	BOOL vCur = _listview_checkitem(pOwner, iCur);
	INT iHitTest = LVHT_NOWHERE;
	INT tmp = 0;
	if (vCur)
	{
		tmp = _obj_baseproc(hWnd, hObj, pObj, LVM_HITTEST, iCur, MAKELONG(ox, oy));
		iHitTest = tmp == 0 ? LVHT_ONITEM : tmp;
	}
	pOwner->nHittest_ = iHitTest;
	BOOL bSingelSelect = !((pObj->dwStyle_ & ELVS_ALLOWMULTIPLE) == ELVS_ALLOWMULTIPLE);
	BOOL bDragdrop = ((pObj->dwStyleEx_ & EOS_EX_DRAGDROP) == EOS_EX_DRAGDROP);
	BOOL bShowAllwasy = ((pObj->dwStyle_ & ELVS_SHOWSELALWAYS) == ELVS_SHOWSELALWAYS);
	if (wParam != 1 || bSingelSelect)
	{
		if ((pObj->dwStyle_ & ELVS_ITEMTRACKING) == ELVS_ITEMTRACKING)
		{
			if (iCur != iSelect)
			{
				if (vCur || !bShowAllwasy)
				{
					if (iSelect != 0)
					{
						_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iSelect, STATE_SELECT, TRUE, 0, 0, 0);
					}
					pOwner->index_select_ = iCur;
				}
				if (vCur)
				{
					if (_listview_queryitemstate(lpItems, iCur, STATE_DISABLE))
					{
						iCur = 0;
					}
					else
					{
						_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_SELECT, FALSE, LVN_HOTTRACK, iCur, iSelect);
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
					_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iLast, STATE_HOVER, TRUE, 0, 0, 0);
				}
				if (vCur)
				{
					if (_listview_queryitemstate(lpItems, iCur, STATE_DISABLE))
					{
						iCur = 0;
					}
					else
					{
						_listview_item_changestate(hWnd, hObj, pObj, pOwner, lpItems, iCur, STATE_HOVER, FALSE, 0, 0, 0);
					}
				}
			}
		}
	}
	pOwner->index_mouse_ = iCur;
}

void _listview_drawitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, EX_PAINTSTRUCT ps, INT iItem, RECT rcClip, RECT rcItem)
{
	INT atomRect = 0;
	EX_CUSTOMDRAW ecd;
	ecd.hTheme = ps.hTheme;
	ecd.hCanvas = ps.hCanvas;
	ecd.dwState = _listview_getitemstate(pOwner->lpItems_, iItem);
	ecd.dwStyle = ps.dwStyle;
	ecd.rcPaint.left = rcItem.left;
	ecd.rcPaint.top = rcItem.top;
	ecd.rcPaint.right = rcItem.right;
	ecd.rcPaint.bottom = rcItem.bottom;
	ecd.iItem = iItem;
	if (_obj_dispatchnotify(hWnd, pObj, hObj, 0, NM_CUSTOMDRAW, iItem, (size_t)&ecd) == 0)
	{
		if (ecd.dwState & STATE_SELECT)
		{
			atomRect = ATOM_SELECT;
		}
		else if (ecd.dwState & STATE_HOVER)
		{
			atomRect = ATOM_HOVER;
		}
		if (atomRect && pObj->crChecked_ != TRUE)
		{
			Ex_ThemeDrawControlEx(ecd.hTheme, ecd.hCanvas, ecd.rcPaint.left, ecd.rcPaint.top, ecd.rcPaint.right, ecd.rcPaint.bottom, ATOM_ITEM, atomRect, 0, 0, 0, 0, 255);
		}
	}
}

size_t _listview_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
		INT iStart = pOwner->index_start_;
		INT iEnd = pOwner->index_end_;
		BOOL bHView = (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST;
		for (INT i = iStart; i < iEnd; i++)
		{
			if (_listview_checkitem_view(pOwner, i))
			{
				RECT rcItem{ 0 };
				_listview_rectfromiitem(pObj, pOwner, i, bHView, &rcItem);
				RECT rcClip{ 0 };
				if (IntersectRect(&rcClip, &ps.rcPaint, &rcItem))
				{
					_listview_drawitem(hWnd, hObj, pObj, pOwner, ps, i, rcClip, rcItem);
				}
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
	return 0;
}

size_t _listview_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj)
{
	listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
	BOOL bHView = (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST;
	RECT* lpRcClient = (RECT*)&pObj->c_left_;
	RECT rcClient{};
	RtlMoveMemory(&rcClient, lpRcClient, sizeof(RECT));

	FLOAT iWidth = 0;
	FLOAT iHeight = 0;
	_canvas_calctextsize(pObj->canvas_obj_, pObj->hFont_, L"S", 1, 0, 0, rcClient.right, rcClient.bottom, &iWidth, &iHeight);
	if (bHView)
	{
		iWidth = Ex_Scale(60);
		iHeight = Ex_Scale(60);
	}
	else
	{
		iWidth = (FLOAT)(rcClient.right - rcClient.left - Ex_Scale(1)) / g_Li.DpiX;
		iHeight = (FLOAT)iHeight / g_Li.DpiY + 8;
	}
	if (iWidth <= 0)
	{
		iWidth = 1;
	}

	if (iHeight <= 0)
	{
		iHeight = 1;
	}

	//项目尺寸
	pOwner->width_item_ = Ex_Scale(iWidth);
	pOwner->height_item_ = Ex_Scale(iHeight);
	//特殊悬浮项尺寸
	pOwner->width_spec_ = pOwner->width_item_;
	pOwner->height_spec_ = pOwner->height_item_;
	//间隔尺寸
	pOwner->width_split_ = Ex_Scale(1);
	pOwner->height_split_ = Ex_Scale(1);
	_obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, NM_CALCSIZE, 0, (size_t)pOwner + offsetof(listview_s, width_item_));
	_listview_updatesbvalue(hObj, pObj, pOwner, lpRcClient);
	_listview_updateviewindex(hObj, pObj, pOwner, bHView, lpRcClient);
	return 0;
}

LRESULT CALLBACK _listview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;
	obj_s* pObj = nullptr;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
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
			_listview_size(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_PAINT)
		{
			return _listview_paint(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			_listview_mouseleave(hWnd, hObj, pObj);
		}
		else if (uMsg == WM_MOUSEMOVE)
		{
			_listview_mousemove(hWnd, hObj, pObj, wParam, lParam);
		}
		else if (uMsg == LVM_HITTEST)
		{
			return (((listview_s*)_obj_pOwner(pObj))->nHittest_);
		}
		else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
		{
			_listview_btndown(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP)
		{
			//_lv_btnup(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL)
		{
			_listview_onvscrollbar(hWnd, hObj, pObj, uMsg, wParam, lParam);
		}
		else if (uMsg == LVM_SETITEMCOUNT)
		{
			return _listview_setitemcount(hWnd, hObj, pObj, wParam, lParam);
		}
		else if (uMsg == LVM_DELETEALLITEMS)
		{
			return _listview_setitemcount(hWnd, hObj, pObj, 0, 0);
		}
		else if (uMsg == LVM_ENSUREVISIBLE)
		{
			return _listview_showitem(hWnd, hObj, pObj, lParam, TRUE);
		}
		else if (uMsg == LVM_GETITEMSTATE)
		{
			lParam = 0;
			if (_listview_checkitem((listview_s*)_obj_pOwner(pObj), wParam))
			{
				lParam = _listview_getitemstate(((listview_s*)_obj_pOwner(pObj))->lpItems_, wParam);
			}
			return lParam;
		}
		else if (uMsg == LVM_SETITEMSTATE)
		{
			_listview_lvm_setitemstate(pObj, (listview_s*)_obj_pOwner(pObj), wParam, lParam);
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
			if (_listview_checkitem((listview_s*)_obj_pOwner(pObj), wParam))
			{
				_listview_lprectfromiitem(pObj, (listview_s*)_obj_pOwner(pObj), wParam, (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST, (RECT*)lParam);
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
			return _listview_reselect(hWnd, hObj, pObj, (listview_s*)_obj_pOwner(pObj), lParam, wParam != 0);
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
			for (INT i = wParam; i < lParam; i++)
			{
				_listview_redrawitem(pObj, (listview_s*)_obj_pOwner(pObj), i);
			}
			return 0;
		}
		else if (uMsg == LVM_CALCITEMSIZE)
		{
			_obj_sendmessage(hWnd, hObj, pObj, WM_SIZE, 0, MAKELONG(pObj->right_ - pObj->left_, pObj->bottom_ - pObj->top_), 0);
		}
		else if (uMsg == LVM_CANCELTHEME)
		{
			pObj->crChecked_ = wParam;
		}
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _listview_lvm_setitemstate(obj_s* pObj, listview_s* pOwner, INT iItem, INT dwState)
{
	if (_listview_checkitem(pOwner, iItem))
	{
		if (iItem == pOwner->index_select_)
		{
			if ((dwState & STATE_SELECT) == 0)
			{
				pOwner->index_select_ = 0;
			}
		}
		LPVOID lpItems = pOwner->lpItems_;
		__set_int(lpItems, (iItem - 1) * 4, dwState);
		_listview_redrawitem(pObj, pOwner, iItem);
	}
}