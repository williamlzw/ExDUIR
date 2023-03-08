#include "Class_ReportListView_ex.h"

ClsPROC m_pfnListView;

LRESULT CALLBACK _reportlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;

	if (uMsg == WM_CREATE)
	{
		_reportlistview_init(hObj);
	}
	else if (uMsg == WM_SETPARENTAFTER)
	{
		HEXOBJ head = Ex_ObjGetLong(hObj, ERLVL_HOBJHEAD);
		Ex_ObjSetParent(head, hObj);
	}
	else if (uMsg == WM_DESTROY)
	{
		_reportlistview_uninit(hObj);
	}
	else if (uMsg == WM_NCCALCSIZE)
	{
		if (!(Ex_ObjGetLong(hObj, EOL_STYLE) & ERLS_NOHEAD))
		{
			obj_s* pObj = nullptr;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			{
				pObj->c_top_ = Ex_Scale(Ex_ObjGetLong(hObj, ERLVL_HEADHEIGHT));
				return 1;
			}
		}
	}
	else if (uMsg == WM_NOTIFY)
	{
		if (((EX_NMHDR*)lParam)->hObjFrom == hObj)
		{
			if (_reportlistview_notify_proc(hObj, (EX_NMHDR*)lParam))
			{
				return 0;
			}
		}
	}
	else if (uMsg == WM_HSCROLL)
	{
		size_t ret = Ex_ObjCallProc(m_pfnListView, hWnd, hObj, uMsg, wParam, lParam);
		Ex_ObjInvalidateRect(Ex_ObjGetLong(hObj, ERLVL_HOBJHEAD), 0);
		return ret;
	}
	else if (uMsg == WM_SIZE)
	{
		Ex_ObjMove(Ex_ObjGetLong(hObj, ERLVL_HOBJHEAD), 0, 0, LOWORD(lParam), Ex_ObjGetLong(hObj, ERLVL_HEADHEIGHT), FALSE);
		// _reportlistview_tc_update(hObj); //debug 64偶尔崩溃,暂时屏蔽
	}
	else if (uMsg == LVM_GETITEM)
	{
		return _reportlistview_li_get(hObj, (EX_REPORTLIST_ITEMINFO*)lParam);
	}
	else if (uMsg == LVM_SETITEM)
	{
		EX_REPORTLIST_ITEMINFO* iteminfo = (EX_REPORTLIST_ITEMINFO*)lParam;
		INT nIndexTR = iteminfo->iRow;
		reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), nIndexTR);
		if (pTR != 0)
		{
			pTR->nImageIndex_ = iteminfo->nImageIndex;
			pTR->dwStyle_ = iteminfo->dwStyle;
			pTR->lParam_ = iteminfo->lParam;
			pTR->rowbkcr_ = iteminfo->rowBkgCr;
		}
		if (wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return TRUE;
	}
	else if (uMsg == LVM_GETCELL)
	{
		return _reportlistview_li_getcell(hObj, (EX_REPORTLIST_CELLINFO*)lParam, FALSE);
	}
	else if (uMsg == LVM_SETCELL)
	{
		return _reportlistview_li_setcell(hObj, (EX_REPORTLIST_CELLINFO*)lParam, FALSE, wParam);
	}
	else if (uMsg == LVM_GETCELLLPARAM)
	{
		reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), wParam);
		if (pTR != 0)
		{
			reportlistview_td_s* pTD = _reportlistview_td_get(hObj, wParam, lParam);
			return pTD->lParam_;
		}
		return 0;
	}
	else if (uMsg == LVM_SETCELLLPARAM)
	{
		INT nIndexTR = LOWORD(lParam);// iRow;
		INT nIndexTC = HIWORD(lParam);//iCol;
		reportlistview_td_s* pTD = _reportlistview_td_get(hObj, nIndexTR, nIndexTC);
		if (pTD != 0)
		{
			pTD->lParam_ = wParam;
			return TRUE;
		}
		return FALSE;
	}
	else if (uMsg == LVM_SETIMAGELIST) //设置列表图片组 wParam为是否立即重画，lParam为图片组句柄
	{
		if (lParam != 0)
		{
			HEXIMAGELIST ptr = (HEXIMAGELIST)Ex_ObjSetLong(hObj, ERLVL_HIMAGELIST, lParam);
			if (ptr)
			{
				_imglist_destroy(ptr);
			}
			INT height = NULL;
			_imglist_size((HEXIMAGELIST)lParam, 0, &height);
			Ex_ObjSetLong(hObj, ERLVL_ITEMHEIGHT, height);
		}
		if (wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 1;
	}
	else if (uMsg == LVM_GETIMAGELIST) //获取图片组
	{
		return Ex_ObjGetLong(hObj, ERLVL_HIMAGELIST);
	}
	else if (uMsg == LVM_GETITEMTEXT)
	{
		if (wParam != 0)
		{
			((EX_REPORTLIST_ITEMINFO*)lParam)->iRow = wParam;
		}
		return _reportlistview_li_getcell(hObj, (EX_REPORTLIST_CELLINFO*)lParam, TRUE);
	}
	else if (uMsg == LVM_SETITEMTEXT)
	{
		if (wParam != 0)
		{
			((EX_REPORTLIST_ITEMINFO*)lParam)->iRow = wParam;
		}
		return _reportlistview_li_setcell(hObj, (EX_REPORTLIST_CELLINFO*)lParam, TRUE);
	}
	else if (uMsg == LVM_INSERTITEM)
	{
		size_t ret = _reportlistview_tr_ins(hObj, (EX_REPORTLIST_ROWINFO*)lParam);
		if (ret)
		{
			INT nCount = Array_GetCount((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO));
			obj_s* pObj = nullptr;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			{
				listview_s* pOwner = (listview_s*)_obj_pOwner(pObj);
				BOOL bHView = (pObj->dwStyle_ & ELVS_HORIZONTALLIST) == ELVS_HORIZONTALLIST;
				LPVOID pOld = pOwner->lpItems_;
				if (pOld != 0)
				{
					Ex_MemFree(pOld);
				}
				pOld = Ex_MemAlloc(nCount * 4);
				pOwner->lpItems_ = pOld;
				pOwner->count_items_ = nCount;
				pOwner->index_select_ = 0;
				pOwner->index_mouse_ = 0;
				pOwner->count_selects_ = 0;
				if (wParam != 0)//是否刷新
				{
					_listview_updatesbvalue(hObj, pObj, pOwner, (RECT*)&pObj->c_left_);
					INT nPosX = 0;
					INT nPosY = 0;
					INT nPage = 0;
					INT nView = 0;
					INT nLine = 0;
					if ((LVSICF_NOSCROLL & LVSICF_NOSCROLL) != LVSICF_NOSCROLL)
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
					//取 提交的项目索引和当前项目索引
					int index = 0;
					if (nCount < 1)
					{
						index = 0;
					}//判断是否需要选中项目
					if (nCount > 0 && index > 0)
					{
						if (index >= nCount)
						{
							_listview_reselect(hWnd, hObj, pObj, pOwner, index, TRUE);
						}
					}

					for (INT i = 1; i < nCount; i++)
					{
						_listview_redrawitem(pObj, (listview_s*)_obj_pOwner(pObj), i);
					}
				}
			}
		}
		return ret;
	}
	else if (uMsg == LVM_DELETEITEM)
	{
		size_t ret = _reportlistview_tr_del(hObj, lParam) ? 1 : 0;
		if (ret != 0 && wParam != 0)
		{
			_reportlistview_tr_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_DELETEALLITEMS)
	{
		_reportlistview_tr_clear(hObj);
		_reportlistview_tr_update(hObj);
		return 0;
	}
	else if (uMsg == LVM_INSERTCOLUMN)
	{
		size_t ret = _reportlistview_tc_ins(hObj, (EX_REPORTLIST_COLUMNINFO*)lParam);
		if (ret != 0 && wParam != 0)
		{
			_reportlistview_tc_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_DELETECOLUMN)
	{
		size_t ret = _reportlistview_tc_del(hObj, lParam) ? 1 : 0;
		if (ret != 0 && wParam != 0)
		{
			_reportlistview_tc_update(hObj);
			_reportlistview_tr_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_DELETEALLCOLUMN)
	{
		_reportlistview_tc_clear(hObj);
		_reportlistview_tc_update(hObj);
		_reportlistview_tr_update(hObj);
		return 0;
	}
	else if (uMsg == LVM_SORTITEMS)
	{
		size_t ret = 0;
		if (lParam != 0)
		{
			INT iCol = ((EX_REPORTLIST_SORTINFO*)lParam)->iCol;
			array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
			if (hArr != 0 && iCol >= 0 && iCol <= Ex_ObjGetLong(hObj, ERLVL_CTCS))
			{
				Array_SetType(hArr, lParam);
				Array_Sort(hArr, ((EX_REPORTLIST_SORTINFO*)lParam)->fDesc != FALSE);
				Array_SetType(hArr, 0);
				Ex_ObjSetLong(hObj, ERLVL_TCIDXSORTED, ((EX_REPORTLIST_SORTINFO*)lParam)->iCol);
				Ex_ObjSetLong(hObj, ERLVL_FTCSORTEDDESC, ((EX_REPORTLIST_SORTINFO*)lParam)->fDesc);
				Ex_ObjInvalidateRect(hObj, 0);
				ret = 1;
			}
		}
		return ret;
	}
	else if (uMsg == LVM_UPDATE)
	{
		_reportlistview_tc_update(hObj);
		_reportlistview_tr_update(hObj);
		return 0;
	}
	else if (uMsg == LVM_GETCOLUMNCOUNT)
	{
		return Ex_ObjGetLong(hObj, ERLVL_CTCS);
	}
	else if (uMsg == LVM_GETCOLUMN)
	{
		LPVOID ptr = __ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), wParam, sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != 0)
		{
			RtlMoveMemory((LPVOID)lParam, ptr, sizeof(EX_REPORTLIST_COLUMNINFO));
			ret = 1;
		}
		return ret;
	}
	else if (uMsg == LVM_SETCOLUMN)
	{
		EX_REPORTLIST_COLUMNINFO* info = (EX_REPORTLIST_COLUMNINFO*)lParam;
		EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)__ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), LOWORD(wParam), sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != 0)
		{
			LPCWSTR old = ptr->pwzText;
			ptr->pwzText = StrDupW(info->pwzText);
			Ex_MemFree((LPVOID)old);
			ptr->nWidth = info->nWidth;
			ptr->dwStyle = info->dwStyle;
			ptr->dwTextFormat = info->dwTextFormat;
			ptr->crText = info->crText;
			ptr->crBkg = info->crBkg;
			ret = 1;
		}
		if (ret != 0 && HIWORD(wParam) != 0)
		{
			_reportlistview_tc_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_SETCOLUMNTEXT)
	{
		EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)__ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), LOWORD(wParam), sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != 0)
		{
			LPCWSTR old = ptr->pwzText;
			ptr->pwzText = StrDupW((LPCWSTR)lParam);
			Ex_MemFree((LPVOID)old);
			ret = 1;
		}
		if (ret != 0 && HIWORD(wParam) != 0)
		{
			_reportlistview_tc_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_GETCOLUMNTEXT)
	{
		EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)__ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), wParam, sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != 0)
		{
			ret = (size_t)ptr->pwzText;
		}
		return ret;
	}
	else if (uMsg == LVM_GETCOLUMNWIDTH)
	{
		EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)__ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), wParam, sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != 0)
		{
			ret = (size_t)ptr->nWidth;
		}
		return ret;
	}
	else if (uMsg == LVM_SETCOLUMNWIDTH)
	{
		EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)__ptr_index((LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO), Ex_ObjGetLong(hObj, ERLVL_CTCS), LOWORD(wParam), sizeof(EX_REPORTLIST_COLUMNINFO));
		size_t ret = 0;
		if (ptr != nullptr)
		{
			ptr->nWidth = lParam;
			ret = ptr->nWidth;
		}
		if (lParam > 0 && wParam > 0)
		{
			_reportlistview_tc_update(hObj);
			_reportlistview_tr_update(hObj);
		}
		return ret;
	}
	else if (uMsg == LVM_GETITEMHEIGHT)
	{
		return Ex_ObjGetLong(hObj, ERLVL_ITEMHEIGHT);
	}
	else if (uMsg == LVM_SETITEMHEIGHT)
	{
		Ex_ObjSetLong(hObj, ERLVL_ITEMHEIGHT, lParam);
		_reportlistview_tc_update(hObj);
		return 0;
	}
	else if (uMsg == RLVM_GETHITCOL)
	{
		return _reportlistview_gethitcol(hObj, lParam);
	}
	else if (uMsg == WM_EX_LDCLICK)
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ERLS_EDIT) == ERLS_EDIT)//可编辑 
		{
			obj_s* pObj = nullptr;
			int nError = 0;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			{
				listview_s* pOwner = (listview_s*)Ex_ObjGetLong(hObj, EOL_OWNER);
				INT nCount = pOwner->count_items_;//项目总数
				if (nCount > 0 && lParam)//有这么巧点击了(0,0)位置?
				{
					INT x = GET_X_LPARAM(lParam);
					INT y = GET_Y_LPARAM(lParam);
					INT ox = 0;
					INT oy = 0;
					INT iCur = _listview_itemfrompos(pObj, pOwner, x, y, &ox, &oy);
					LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
					INT cTCs = Ex_ObjGetLong(hObj, ERLVL_CTCS);
					INT nOffsetX = -Ex_ObjScrollGetPos(hObj, SB_HORZ);
					INT itemhight = (Ex_ObjGetLong(hObj, ERLVL_ITEMHEIGHT) + ((listview_s*)pOwner)->height_split_) * g_Li.DpiY;
					INT ColWidth = 0;
					INT Colindex = 0;
					if (pTCs != 0 && cTCs > 0)
					{
						for (INT i = 0; i < cTCs; i++)
						{
							EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + sizeof(EX_REPORTLIST_COLUMNINFO) * i);
							INT nColWidth = ptr->nWidth;
							if (nColWidth > 0)
							{
								if (x >= nOffsetX && x < nOffsetX + nColWidth)
								{
									ColWidth = nColWidth; Colindex = i + 1;
									break;
								}
							}
							nOffsetX = nOffsetX + nColWidth;
						}
					}
					if (Colindex && iCur)
					{
						HEXOBJ edit = Ex_ObjGetLong(hObj, ERLVL_HEDIT);
						INT nIndexTC = Colindex;
						reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), iCur);
						if (pTR != 0)
						{
							LPCWSTR pwzText = ((reportlistview_td_s*)((size_t)pTR->pTDInfo_ + (nIndexTC - 1) * sizeof(reportlistview_td_s)))->wzText_;
							Ex_ObjSetText(edit, pwzText, TRUE);
						}
						Ex_ObjSetLong(edit, EOL_USERDATA, MAKELPARAM(iCur, Colindex));
						Ex_ObjSetPos(edit, 0, nOffsetX - g_Li.DpiY, y - oy - 2 * g_Li.DpiY, ColWidth + 2 * g_Li.DpiY, itemhight + 2 * g_Li.DpiY, SWP_EX_NODPISCALE);
						Ex_ObjShow(edit, TRUE);
					}
				}
			}
			return 0;//可编辑时不再执行双击判断了
		}
	}
	else if (uMsg == RLVM_SETCHECK)
	{
		array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
		size_t	ptr = Array_GetMember(hArr, wParam);
		if (ptr != 0 && __query((LPVOID)ptr, 4, ERLV_RS_CHECKBOX))
		{
			if (lParam)
			{
				__add((LPVOID)ptr, 4, ERLV_RS_CHECKBOX_CHECK);
			}
			else
			{
				__del((LPVOID)ptr, 4, ERLV_RS_CHECKBOX_CHECK);
			}
		}
		Ex_ObjInvalidateRect(hObj, 0);
		return 0;
	}
	else if (uMsg == RLVM_GETCHECK)
	{
		array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
		size_t	ptr = Array_GetMember(hArr, wParam);
		if (ptr != 0 && __query((LPVOID)ptr, 4, ERLV_RS_CHECKBOX))
		{
			return  __query((LPVOID)ptr, 4, ERLV_RS_CHECKBOX_CHECK) ? 1 : 0;
		}
	}
	return Ex_ObjCallProc(m_pfnListView, hWnd, hObj, uMsg, wParam, lParam);
}

LRESULT CALLBACK _reportlistview_head_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nError = 0;

	if (uMsg == WM_PAINT)
	{
		_reportlistview_head_paint(hObj);
	}
	else if (uMsg == WM_MOUSELEAVE) //当离开表头
	{
		if ((Ex_ObjGetUIState(hObj) & STATE_ALLOWDRAG) == 0) //并且未拖动时
		{
			Ex_ObjSetLong(hObj, ERLVHL_INDEXHIT, 0); //设置为未命中任何列
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == WM_EX_LCLICK) //当单击表头
	{
		INT nHitBlock = 0;
		size_t nIndex = _reportlistview_head_hittest(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), TRUE, &nHitBlock);
		HEXOBJ hObjList = Ex_ObjGetLong(hObj, ERLVHL_HLISTVIEW);
		if (hObjList == 0)
		{
			hObjList = Ex_ObjGetParent(hObj);
		}
		if (hObjList != 0 && nIndex != 0 && nHitBlock == 0) //若命中某一列
		{
			EX_REPORTLIST_COLUMNINFO* pTC = (EX_REPORTLIST_COLUMNINFO*)(Ex_ObjGetLong(hObjList, ERLVL_TCINFO) + (nIndex - 1) * sizeof(EX_REPORTLIST_COLUMNINFO));
			if ((pTC->dwStyle & ERLV_CS_CLICKABLE) == ERLV_CS_CLICKABLE) //如果它是可点击的
			{
				if (Ex_ObjDispatchNotify(hObjList, RLVN_COLUMNCLICK, nIndex, lParam) == 0) //分发事件
				{
					if ((pTC->dwStyle & ERLV_CS_SORTABLE) == ERLV_CS_SORTABLE) //如果是可排序的,则按文本自动排序
					{
						EX_REPORTLIST_SORTINFO* p = (EX_REPORTLIST_SORTINFO*)Ex_MemAlloc(sizeof(EX_REPORTLIST_SORTINFO));
						p->iCol = nIndex;
						if (Ex_ObjGetLong(hObjList, ERLVL_TCIDXSORTED) == nIndex)
						{
							p->fDesc = Ex_ObjGetLong(hObjList, ERLVL_FTCSORTEDDESC) != 0 ? FALSE : TRUE;
						}
						Ex_ObjSendMessage(hObjList, LVM_SORTITEMS, 0, (size_t)p);
						Ex_MemFree(p);
					}
				}
			}
		}
	}
	else if (uMsg == WM_LBUTTONDOWN) //当按下左键
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
		INT rHitBlock = 0;
		size_t nIndex = _reportlistview_head_hittest(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), FALSE, &rHitBlock);
		if (rHitBlock == 2) //如果命中位置是分割线,则开始拖动
		{
			Ex_ObjSetUIState(hObj, STATE_ALLOWDRAG, FALSE, 0, FALSE);
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
		Ex_ObjSetUIState(hObj, STATE_ALLOWDRAG, TRUE, 0, FALSE);
		INT rHitBlock = 0;
		_reportlistview_head_hittest(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), FALSE, &rHitBlock);
		HEXOBJ hObjList = Ex_ObjGetLong(hObj, ERLVHL_HLISTVIEW);
		if (hObjList == 0)
		{
			hObjList = Ex_ObjGetParent(hObj);
		}
		if (hObjList != 0)
		{
			_reportlistview_tc_update(hObjList);
			listview_s* pOwner = (listview_s*)Ex_ObjGetLong(hObjList, EOL_OWNER);
			Ex_ObjDispatchMessage(hObjList, LVM_REDRAWITEMS, pOwner->index_start_, pOwner->index_end_);
		}
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		INT rHitBlock = 0;
		INT nIndex = _reportlistview_head_hittest(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), TRUE, &rHitBlock);
		if (nIndex != 0)
		{
			if (rHitBlock == 2 || (Ex_ObjGetUIState(hObj) & STATE_ALLOWDRAG) != 0) //如果是正在拖动,则更新拖动列的宽度,并更新列表
			{
				SetCursor(LoadCursorW(0, IDC_SIZEWE));
				if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) != 0)
				{
					HEXOBJ hObjList = Ex_ObjGetLong(hObj, ERLVHL_HLISTVIEW);
					if (hObjList == 0)
					{
						hObjList = Ex_ObjGetParent(hObj);
					}
					if (hObjList != 0)
					{
						INT w = -Ex_ObjScrollGetPos(hObjList, SB_HORZ);
						LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObjList, ERLVL_TCINFO);
						for (INT i = 0; i < nIndex; i++)
						{
							EX_REPORTLIST_COLUMNINFO* p = (EX_REPORTLIST_COLUMNINFO*)__ptr_index(pTCs, nIndex, i + 1, sizeof(EX_REPORTLIST_COLUMNINFO));
							if (nIndex == i + 1)
							{
								INT x = GET_X_LPARAM(lParam);
								RECT rc;
								Ex_ObjGetRect(hObj, &rc);
								if (x > 1 && x < rc.right - 1)
								{
									w = x - w;
									if (w < 1)
									{
										w = 1;
									}
									p->nWidth = w;
									Ex_ObjInvalidateRect(hObjList, 0);
									Ex_ObjInvalidateRect(hObj, 0);
									break;
								}
							}
							w = w + p->nWidth;
						}
						return 0;
					}
				}
			}
			else
			{
				SetCursor(LoadCursorW(0, IDC_ARROW));
				INT nInexHit = Ex_ObjSetLong(hObj, ERLVHL_INDEXHIT, nIndex);
				if (nInexHit != nIndex)
				{
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
		}
		else
		{
			SetCursor(LoadCursorW(0, IDC_ARROW));
		}
	}
	else if (uMsg == WM_SETCURSOR) //禁控件自身再次设置光标产生闪烁
	{
		return 1;
	}

	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _reportlistview_head_hittest(HEXOBJ hObj, INT x, INT y, BOOL fJustHit, INT* rHitBlock)
{
	if ((Ex_ObjGetUIState(hObj) & STATE_ALLOWDRAG) == 0)
	{
		HEXOBJ hObjList = Ex_ObjGetLong(hObj, ERLVHL_HLISTVIEW);
		if (hObjList == 0)
		{
			hObjList = Ex_ObjGetParent(hObj);
		}
		if (hObjList != 0)
		{
			LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObjList, ERLVL_TCINFO);
			INT cTCs = Ex_ObjGetLong(hObjList, ERLVL_CTCS);
			if (pTCs != 0 && cTCs > 0)
			{
				RECT rc{ 0 };
				Ex_ObjGetClientRect(hObj, &rc);
				rc.left = -Ex_ObjScrollGetPos(hObjList, SB_HORZ);

				for (INT i = 0; i < cTCs; i++) //循环检测列
				{
					EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + sizeof(EX_REPORTLIST_COLUMNINFO) * i);
					INT nColWidth = ptr->nWidth;

					if (nColWidth > 0)
					{
						rc.right = rc.left + nColWidth;

						if ((ptr->dwStyle & ERLV_CS_LOCKWIDTH) != ERLV_CS_LOCKWIDTH) //如果为非锁定宽度列
						{
							rc.right = rc.right + 5;
						}

						POINT pt;
						pt.x = x;
						pt.y = y;
						if (PtInRect(&rc, pt)) //命中了某列
						{
							if (fJustHit == FALSE)
							{
								INT nIndexHit = Ex_ObjSetLong(hObj, ERLVHL_INDEXHIT, i + 1);
								if (nIndexHit != i + 1)
								{
									Ex_ObjInvalidateRect(hObj, 0);
								}
							}

							if (rHitBlock != 0)
							{
								*rHitBlock = 0;
								if (*rHitBlock == 0 && ((ptr->dwStyle & ERLV_CS_LOCKWIDTH) != ERLV_CS_LOCKWIDTH)) //线
								{
									RECT rcBlock = rc;
									rcBlock.right = rc.right;
									rcBlock.left = rcBlock.right - 10;
									POINT pt2;
									pt2.x = x;
									pt2.y = y;
									if (PtInRect(&rcBlock, pt2))
									{
										*rHitBlock = 2;
									}
								}
							}
							return i + 1;
						}
						rc.left = rc.right;
						if ((ptr->dwStyle & ERLV_CS_LOCKWIDTH) != ERLV_CS_LOCKWIDTH) //
						{
							rc.left = rc.left - 5;
						}
					}
				}
				if (fJustHit == FALSE)
				{
					INT nIndexHit = Ex_ObjSetLong(hObj, ERLVHL_INDEXHIT, 0);
					if (nIndexHit != 0)
					{
						Ex_ObjInvalidateRect(hObj, 0);
					}
				}
			}
		}
	}
	else
	{
		*rHitBlock = 2;
		return Ex_ObjGetLong(hObj, ERLVHL_INDEXHIT);
	}
	return 0;
}

void _reportlistview_head_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		HEXOBJ hObjList = Ex_ObjGetLong(hObj, ERLVHL_HLISTVIEW);
		INT nIndexHit = Ex_ObjGetLong(hObj, ERLVHL_INDEXHIT);
		if (hObjList == 0)
		{
			hObjList = Ex_ObjGetParent(hObj);
		}
		if (hObjList != 0)
		{
			LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObjList, ERLVL_TCINFO);
			INT cTCs = Ex_ObjGetLong(hObjList, ERLVL_CTCS);
			_canvas_clear(ps.hCanvas, Ex_ObjGetColor(hObjList, COLOR_EX_RLV_HEAD));
			if (pTCs != 0 && cTCs > 0)
			{
				HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObjList, COLOR_EX_BORDER));
				INT nOffsetX = -Ex_ObjScrollGetPos(hObjList, SB_HORZ);
				for (INT i = 0; i < cTCs; i++)
				{
					EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + sizeof(EX_REPORTLIST_COLUMNINFO) * i);
					INT nColWidth = ptr->nWidth;
					if (nColWidth > 0)
					{
						if ((ptr->dwStyle & ERLV_CS_COLCOR) == ERLV_CS_COLCOR)
						{
							_brush_setcolor(hBrush, ptr->crBkg);
							_canvas_fillrect(ps.hCanvas, hBrush, nOffsetX + 1.5, 0.5, nOffsetX + nColWidth + 1.0, ps.uHeight);
							_brush_setcolor(hBrush, Ex_ObjGetColor(hObjList, COLOR_EX_BORDER));
						}
						if (nIndexHit == i + 1 && (ptr->dwStyle & ERLV_CS_CLICKABLE) == ERLV_CS_CLICKABLE)
						{
							if ((ptr->dwStyle & ERLV_CS_COLCOR) == ERLV_CS_COLCOR)
							{
								_brush_setcolor(hBrush, ptr->crBkg);
							}
							else
							{
								_brush_setcolor(hBrush, Ex_ObjGetColor(hObjList, COLOR_EX_TEXT_HOVER));
							}
							_canvas_fillrect(ps.hCanvas, hBrush, nOffsetX, 0, nOffsetX + nColWidth, ps.uHeight);
							_brush_setcolor(hBrush, Ex_ObjGetColor(hObjList, COLOR_EX_BORDER));
						}
						_canvas_drawtext(ps.hCanvas, Ex_ObjGetFont(hObjList), Ex_ObjGetColor(hObjList, COLOR_EX_TEXT_NORMAL), ptr->pwzText, -1, DT_SINGLELINE | ptr->dwTextFormat, nOffsetX + 3, 0, nOffsetX + nColWidth - 3, ps.uHeight);
						_canvas_drawline(ps.hCanvas, hBrush, nOffsetX + nColWidth + 1, 0, nOffsetX + nColWidth + 1, ps.uHeight, 1.5, D2D1_DASH_STYLE_SOLID);
					}
					nOffsetX = nOffsetX + nColWidth;
				}
				RECT rc;
				Ex_ObjGetClientRect(hObjList, &rc);
				if ((Ex_ObjGetLong(hObjList, EOL_STYLE) & EOS_BORDER) == EOS_BORDER)
				{
					// uHeight +1 是为了不显示下边线，不然，表头下边线和报表上边线都出现，会出现两条边线的情况
					_canvas_drawrect(ps.hCanvas, hBrush, 0, 0, Ex_Scale(rc.right), ps.uHeight + 1, 1, 0);
				}
				_brush_destroy(hBrush);
			}
		}
		Ex_ObjEndPaint(hObj, &ps);
	}
}

INT _reportlistview_gethitcol(HEXOBJ hObj, INT x)
{
	LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
	INT cTCs = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	if (pTCs != 0 && cTCs > 0)
	{
		INT nOffsetX = -Ex_ObjScrollGetPos(hObj, SB_HORZ);
		for (INT i = 0; i < cTCs; i++)
		{
			EX_REPORTLIST_COLUMNINFO* ptr = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + sizeof(EX_REPORTLIST_COLUMNINFO) * i);
			INT nColWidth = ptr->nWidth;
			if (nColWidth > 0)
			{
				if (x >= nOffsetX && x < nOffsetX + nColWidth)
				{
					return i + 1;
				}
			}
			nOffsetX = nOffsetX + nColWidth;
		}
	}
	return 0;
}

BOOL _reportlistview_notify_proc(HEXOBJ hObj, EX_NMHDR* pNotifyInfo)
{
	INT nCode = pNotifyInfo->nCode;
	LPARAM lParam = pNotifyInfo->lParam;
	if (nCode == NM_CUSTOMDRAW)
	{
		_reportlistview_draw_tr(hObj, (EX_CUSTOMDRAW*)lParam);
	}
	else if (nCode == NM_CALCSIZE)
	{
		if (Ex_ObjGetLong(hObj, ERLVL_ITEMWIDTH) > __get_int((LPVOID)lParam, 0))
		{
			__set_int((LPVOID)lParam, 0, Ex_ObjGetLong(hObj, ERLVL_ITEMWIDTH));
		}
		if (Ex_ObjGetLong(hObj, ERLVL_ITEMHEIGHT) > __get_int((LPVOID)lParam, 4))
		{
			__set_int((LPVOID)lParam, 4, Ex_ObjGetLong(hObj, ERLVL_ITEMHEIGHT));
		}
	}
	else if (nCode == NM_CLICK)
	{
		nCode = ((listview_s*)Ex_ObjGetLong(hObj, EOL_OWNER))->index_mouse_;
		RECT rc{ 0 };
		if (nCode > 0)
		{
			reportlistview_tr_s* pData = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), nCode);
			if (pData != 0 && (pData->dwStyle_ & ERLV_RS_CHECKBOX))
			{
				Ex_ObjSendMessage(hObj, LVM_GETITEMRECT, nCode, (LPARAM)&rc);
				rc.right = rc.bottom - rc.top;
				rc.bottom += rc.bottom - rc.top;
				// 命中检查框
				if (PtInRect(&rc, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }))
				{
					if ((pData->dwStyle_ & ERLV_RS_CHECKBOX_CHECK))
					{
						__del(pData, 4, ERLV_RS_CHECKBOX_CHECK);
					}
					else
					{
						__add(pData, 4, ERLV_RS_CHECKBOX_CHECK);
					}

					Ex_ObjSendMessage(hObj, LVM_REDRAWITEMS, nCode, nCode);
					Ex_ObjDispatchNotify(hObj, RLVN_CHECK, nCode, (pData->dwStyle_ & ERLV_RS_CHECKBOX_CHECK) ? 1 : 0);
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void _reportlistview_edit_killfous_settext(HWND hWnd, HEXOBJ hObj, WPARAM wParam, LPARAM lParam)
{
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
	{
		_obj_baseproc(hWnd, hObj, pObj, WM_KILLFOCUS, wParam, lParam);
		Ex_ObjShow(hObj, FALSE);
		pObj->pWnd_->objFocus_ = pObj->objParent_;
		size_t len = Ex_ObjGetTextLength(hObj) * 2 + 2;
		std::wstring text; text.resize(len);
		Ex_ObjGetText(hObj, (LPCWSTR)text.data(), len);
		obj_s* pObjRL = nullptr;
		if (_handle_validate(pObj->objParent_, HT_OBJECT, (LPVOID*)&pObjRL, 0))
		{
			LPARAM dx = (LPARAM)pObj->dwUserData_;
			INT nIndexTR = LOWORD(dx);
			INT nIndexTC = HIWORD(dx);
			_reportlistview_td_settext(pObj->objParent_, nIndexTR, nIndexTC, text.data());
			_obj_invalidaterect(pObjRL, 0, 0);
		}
	}
}

LRESULT CALLBACK _reportlistview_edit_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	if (uMsg == WM_MOUSELEAVE)
	{
		_reportlistview_edit_killfous_settext(hWnd, hObj, wParam, lParam);
		return 1;
	}
	return 0;
}

LRESULT CALLBACK _reportlistview_edit_enter(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == NM_KEYDOWN)
	{
		if (wParam == 13)
		{
			obj_s* pObj = nullptr;
			int nError = 0;
			if (_handle_validate(hObj, HT_OBJECT, (LPVOID*)&pObj, &nError))
			{
				_reportlistview_edit_killfous_settext(pObj->pWnd_->hWnd_, hObj, wParam, lParam);
			}
		}
	}
	return 0;
}

void _reportlistview_init(HEXOBJ hObj)
{
	Ex_ObjSetLong(hObj, ERLVL_TCINFO, 0);
	Ex_ObjSetLong(hObj, ERLVL_CTCS, 0);
	Ex_ObjSetLong(hObj, ERLVL_ITEMWIDTH, 0);
	Ex_ObjSetLong(hObj, ERLVL_ITEMHEIGHT, 20);
	Ex_ObjSetLong(hObj, ERLVL_HEADHEIGHT, 25);
	HEXOBJ head = Ex_ObjCreate((LPCWSTR)ATOM_REPORTLISTVIEW_HEAD, 0, -1, 0, 0, 0, 0, hObj);
	Ex_ObjSetLong(hObj, ERLVL_HOBJHEAD, head);
	HEXOBJ edit = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, L"edit", 0,
		EOS_VISIBLE | EES_HIDESELECTION | EES_DISABLEMENU //| EOS_HSCROLL
		, 10, 30, 150, 30, hObj, 0, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, _reportlistview_edit_proc);
	Ex_ObjHandleEvent(edit, NM_KEYDOWN, _reportlistview_edit_enter);
	Ex_ObjShow(edit, FALSE);
	Ex_ObjSetLong(hObj, ERLVL_HEDIT, edit);

	Ex_ObjShow(head, (Ex_ObjGetLong(hObj, EOL_STYLE) & ERLS_NOHEAD) == 0);
	Ex_ObjSetLong(hObj, ERLVL_TCIDXSORTED, 0);
	array_s* hArr = Array_Create(0);
	if (hArr)
	{
		Array_SetExtra(hArr, hObj);
		Array_BindEvent(hArr, eae_delmember, _reportlistview_arr_del);
		Array_BindEvent(hArr, eae_comparemember, _reportlistview_arr_order);
		Ex_ObjSetLong(hObj, ERLVL_TRINFO, (size_t)hArr);
	}
}

void _reportlistview_arr_del(array_s* hArr, INT nIndex, reportlistview_tr_s* pvData, INT nType)
{
	LPVOID pTDs = pvData->pTDInfo_;
	if (pTDs != 0)
	{
		INT nCount = Ex_ObjGetLong(Array_GetExtra(hArr), ERLVL_CTCS);
		Ex_ObjDispatchNotify(Array_GetExtra(hArr), RLVN_DELETE_ITEM, nIndex, (size_t)pvData);
		for (INT i = 0; i < nCount; i++)
		{
			reportlistview_td_s* pTD = (reportlistview_td_s*)((size_t)pTDs + i * sizeof(reportlistview_td_s));
			if (pTD->font_)
			{
				_font_destroy(pTD->font_);
			}
			if (pTD->wzText_)
			{
				Ex_MemFree((LPVOID)pTD->wzText_);
			}
			
		}
		Ex_MemFree(pTDs);
	}
	Ex_MemFree(pvData);
}

size_t _reportlistview_arr_order(array_s* hArr, INT nIndex1, LPVOID pvData1, INT nIndex2, LPVOID pvData2, EX_REPORTLIST_SORTINFO* pSortInfo, INT nReason)
{
	LPVOID lpfnCmp = pSortInfo->lpfnCmp;
	INT nIndexTC = pSortInfo->iCol;
	if (nIndexTC != 0)
	{
		reportlistview_td_s* ptr1 = (reportlistview_td_s*)((size_t)(((reportlistview_tr_s*)pvData1)->pTDInfo_) + sizeof(reportlistview_td_s) * (nIndexTC - 1));
		pvData1 = (LPVOID)ptr1->wzText_;
		reportlistview_td_s* ptr2 = (reportlistview_td_s*)((size_t)(((reportlistview_tr_s*)pvData2)->pTDInfo_) + sizeof(reportlistview_td_s) * (nIndexTC - 1));
		pvData2 = (LPVOID)ptr2->wzText_;
	}
	if (lpfnCmp == 0)
	{
		if (nIndexTC == 0) //整行按lParam排序
		{
			return ((reportlistview_tr_s*)pvData2)->lParam_ - ((reportlistview_tr_s*)pvData1)->lParam_;
		}
		else if (pSortInfo->nType == 0)
		{
			return wcscmp((LPCWSTR)pvData2, (LPCWSTR)pvData1);
		}
		else
		{
			pvData1 = _fmt_int(pvData1, 0);
			pvData2 = _fmt_int(pvData2, 0);
			return (size_t)pvData2 - (size_t)pvData1;
		}
	}
	else
	{
		return ((ReportListViewOrderPROC)lpfnCmp)((HEXOBJ)Array_GetExtra(hArr), nIndex1, pvData1, nIndex2, pvData2, nIndexTC, pSortInfo->nType, (size_t)pSortInfo->lParam);
	}
	return 0;
}

void _reportlistview_uninit(HEXOBJ hObj)
{
	HEXIMAGELIST ptr = (HEXIMAGELIST)Ex_ObjSetLong(hObj, ERLVL_HIMAGELIST, 0);
	if (ptr)
	{
		_imglist_destroy(ptr);
	}
	Array_Destroy((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO));
	INT nCount = Ex_ObjSetLong(hObj, ERLVL_CTCS, 0);
	LPVOID pTCs = (LPVOID)Ex_ObjSetLong(hObj, ERLVL_TCINFO, 0);
	EX_REPORTLIST_COLUMNINFO* ptc = nullptr;
	if (pTCs != 0)
	{
		for (INT i = 0; i < nCount; i++)
		{
			ptc = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + i * sizeof(EX_REPORTLIST_COLUMNINFO));
			Ex_MemFree((LPVOID)ptc->pwzText);
		}
		Ex_MemFree(pTCs);
	}
}

void _reportlistview_draw_tr(HEXOBJ hObj, EX_CUSTOMDRAW* pDrawInfo)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), pDrawInfo->iItem);
	RECT rcTD{ 0 };

	if (pTR != 0)
	{
		rcTD.left = pDrawInfo->rcPaint.left;
		rcTD.top = pDrawInfo->rcPaint.top;
		rcTD.bottom = pDrawInfo->rcPaint.bottom;

		if (rcTD.top < Ex_ObjGetLong(hObj, ERLVL_HEADHEIGHT) && (Ex_ObjGetLong(hObj, EOL_STYLE) & ERLS_NOHEAD) == 0)
		{
			_canvas_cliprect(pDrawInfo->hCanvas, pDrawInfo->rcPaint.left, Ex_ObjGetLong(hObj, ERLVL_HEADHEIGHT), pDrawInfo->rcPaint.right, pDrawInfo->rcPaint.bottom);
		}
		if (Ex_ObjDispatchNotify(hObj, RLVN_DRAW_TR, (size_t)pTR, (size_t)pDrawInfo) == 0)
		{
			HEXIMAGELIST hImgList = (HEXIMAGELIST)Ex_ObjGetLong(hObj, ERLVL_HIMAGELIST);
			INT itemHeight = pDrawInfo->rcPaint.bottom - pDrawInfo->rcPaint.top;
			if ((pTR->dwStyle_ & ERLV_RS_ROWCOLCOR) == ERLV_RS_ROWCOLCOR)//新增
			{
				HEXBRUSH hBrush = _brush_create(pTR->rowbkcr_);
				_canvas_fillrect(pDrawInfo->hCanvas, hBrush,
					pDrawInfo->rcPaint.left + 1.5, pDrawInfo->rcPaint.top - 0.5, pDrawInfo->rcPaint.right, pDrawInfo->rcPaint.bottom + 0.5);
				_brush_destroy(hBrush);
			}
			HEXIMAGE hImage = 0;
			RECT checkrc = { rcTD.left + itemHeight / 4, rcTD.top, rcTD.left + itemHeight / 4 * 3, rcTD.bottom };
			RECT imgrc = { rcTD.left + itemHeight / 4, rcTD.top + itemHeight / 4, rcTD.left + itemHeight / 4 * 3, rcTD.top + itemHeight / 4 * 3 };

			if (pTR->dwStyle_ & ERLV_RS_CHECKBOX)
			{
				rcTD.left = rcTD.left + itemHeight / 4 * 3;
			}
			else
			{
				hImage = _imglist_get(hImgList, pTR->nImageIndex_);
				if (hImage != 0)
				{
					rcTD.left = rcTD.left + itemHeight / 4 * 3;
				}
			}

			EX_REPORTLIST_COLUMNINFO* pTC = (EX_REPORTLIST_COLUMNINFO*)Ex_ObjGetLong(hObj, ERLVL_TCINFO);

			for (INT i = 1; i <= nCount; i++)
			{
				rcTD.right = rcTD.left + pTC->nWidth;
				if (i == 1 && ((pTR->dwStyle_ & ERLV_RS_CHECKBOX) || hImage != 0))
				{
					rcTD.right = rcTD.right - itemHeight / 4 * 3; 
				}
				_reportlistview_draw_td(hObj, pDrawInfo, pDrawInfo->iItem, i, pTC, &rcTD);
				rcTD.left = rcTD.right;
				pTC = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTC + sizeof(EX_REPORTLIST_COLUMNINFO));
			}

			if ((pDrawInfo->dwStyle & ERLS_DRAWHORIZONTALLINE) == ERLS_DRAWHORIZONTALLINE)
			{
				HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BORDER));
				_canvas_drawline(pDrawInfo->hCanvas, hBrush, pDrawInfo->rcPaint.left, pDrawInfo->rcPaint.bottom, pDrawInfo->rcPaint.right, pDrawInfo->rcPaint.bottom, 1.5, D2D1_DASH_STYLE_SOLID);
				_brush_destroy(hBrush);
			}
			if (pTR->dwStyle_ & ERLV_RS_CHECKBOX)
			{
				Ex_ThemeDrawControl(pDrawInfo->hTheme, pDrawInfo->hCanvas,
					checkrc.left, checkrc.top, checkrc.right, checkrc.bottom,
					ATOM_CHECKBUTTON, (pTR->dwStyle_ & ERLV_RS_CHECKBOX_CHECK) ? ATOM_CHECK_NORMAL : ATOM_NORMAL, 255);
			}
			else
			{
				INT imgWidth;
				INT imgHeight;
				_imglist_size(hImgList, &imgWidth, &imgHeight);
				_canvas_drawimagerectrect(pDrawInfo->hCanvas, hImage,
					imgrc.left, imgrc.top, imgrc.right, imgrc.bottom,
					0, 0, imgWidth, imgHeight, 255);
			}
		}
		_canvas_resetclip(pDrawInfo->hCanvas);
	}
}

void _reportlistview_draw_td(HEXOBJ hObj, EX_CUSTOMDRAW* cd, INT nIndexTR, INT nIndexTC, EX_REPORTLIST_COLUMNINFO* pTC, RECT* rcTD)
{
	reportlistview_td_s* pTD = _reportlistview_td_get(hObj, nIndexTR, nIndexTC);
	if (pTD != 0)
	{
		if (Ex_ObjDispatchNotify(hObj, RLVN_DRAW_TD, nIndexTC, (size_t)cd) == 0)
		{
			if ((pTD->cellStyle_ & ERLV_RS_CELLCOLCOR) == ERLV_RS_CELLCOLCOR)//新增
			{
				HEXBRUSH hBrush = _brush_create(pTD->crbk_);
				_canvas_fillrect(cd->hCanvas, hBrush,
					nIndexTC == 1 ? 0 : rcTD->left + 2.0, rcTD->top, rcTD->right + 1.0, rcTD->bottom - 0.5);
				_brush_destroy(hBrush);
			}
			LPCWSTR wzText = pTD->wzText_;
			if (wzText != 0)
			{
				INT crText = 0;
				if ((pTD->cellStyle_ & ERLV_RS_CELLTEXTCOLCOR) == ERLV_RS_CELLTEXTCOLCOR)//新增
				{
					crText = pTD->crText_;
				}
				else
				{
					crText = pTC->crText;
					if (crText == 0)
						crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
				}
				HEXOBJ Font = Ex_ObjGetFont(hObj);
				if ((pTD->cellStyle_ & ERLV_RS_CELLFONT) == ERLV_RS_CELLFONT)//新增
				{
					if (pTD->font_)//防止 未设置字体时出错,使用默认字体
					{
						Font = pTD->font_;
					}
				}
				_canvas_drawtext(cd->hCanvas, Font, crText, wzText, -1, pTC->dwTextFormat, rcTD->left + 3.5, rcTD->top, rcTD->right + 1, rcTD->bottom);
			}
			if ((cd->dwStyle & ERLS_DRAWVERTICALLINE) == ERLS_DRAWVERTICALLINE)
			{
				HEXBRUSH hBrush = _brush_create(Ex_ObjGetColor(hObj, COLOR_EX_BORDER));
				_canvas_drawline(cd->hCanvas, hBrush, rcTD->right + 1.0, rcTD->top, rcTD->right + 1., rcTD->bottom, 1.0, D2D1_DASH_STYLE_SOLID);
				_brush_destroy(hBrush);
			}
		}
	}
}

INT _reportlistview_tc_ins(HEXOBJ hObj, EX_REPORTLIST_COLUMNINFO* pInsertInfo)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
	INT nIndexInsert = pInsertInfo->nInsertIndex;
	LPVOID pNew = __ptr_ins(&pTCs, nCount, &nIndexInsert, sizeof(EX_REPORTLIST_COLUMNINFO), pInsertInfo);
	EX_REPORTLIST_COLUMNINFO* ptc = (EX_REPORTLIST_COLUMNINFO*)((size_t)pNew + (nIndexInsert - 1) * sizeof(EX_REPORTLIST_COLUMNINFO));
	ptc->pwzText = StrDupW(pInsertInfo->pwzText);
	ptc->nInsertIndex = 0;
	if (ptc->dwTextFormat == 0)
	{
		ptc->dwTextFormat = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
	}
	Ex_ObjSetLong(hObj, ERLVL_TCINFO, (size_t)pNew);
	nCount = nCount + 1;
	Ex_ObjSetLong(hObj, ERLVL_CTCS, nCount);
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	for (INT i = 0; i < Array_GetCount(hArr); i++)
	{
		reportlistview_tr_s* ptr = (reportlistview_tr_s*)Array_GetMember(hArr, i + 1);
		LPVOID pTDs = ptr->pTDInfo_;
		LPVOID p2 = __ptr_ins(&pTDs, nCount - 1, &nIndexInsert, sizeof(reportlistview_td_s), 0);
		ptr->pTDInfo_ = p2;
	}
	Ex_ObjSetLong(hObj, ERLVL_TCIDXSORTED, 0);
	Ex_ObjSetLong(hObj, ERLVL_FTCSORTEDDESC, 0);
	return nIndexInsert;
}

BOOL _reportlistview_tc_del(HEXOBJ hObj, INT nIndex)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	if (nIndex <= 0 || nIndex > nCount)
	{
		return FALSE;
	}
	LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
	EX_REPORTLIST_COLUMNINFO* ptc = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + (nIndex - 1) * sizeof(EX_REPORTLIST_COLUMNINFO));
	Ex_MemFree((LPVOID)ptc->pwzText);
	__ptr_del(&pTCs, nCount, nIndex, sizeof(EX_REPORTLIST_COLUMNINFO));
	Ex_ObjSetLong(hObj, ERLVL_TCINFO, (size_t)pTCs);
	nCount = nCount - 1;
	Ex_ObjSetLong(hObj, ERLVL_CTCS, nCount);
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	for (INT i = 0; i < Array_GetCount(hArr); i++)
	{
		reportlistview_tr_s* ptr = (reportlistview_tr_s*)Array_GetMember(hArr, i + 1);
		LPVOID pTDs = ptr->pTDInfo_;
		reportlistview_td_s* pTD = ((reportlistview_td_s*)((size_t)pTDs + (nIndex - 1) * sizeof(reportlistview_td_s)));//---修改
		if (pTD)
		{
			if (pTD->font_)
			{
				_font_destroy(pTD->font_);
			}
			if (pTD->wzText_)
			{
				Ex_MemFree((LPVOID)pTD->wzText_);
			}
		}
		__ptr_del(&pTDs, nCount + 1, nIndex, sizeof(reportlistview_td_s));
		ptr->pTDInfo_ = pTDs;
	}
	Ex_ObjSetLong(hObj, ERLVL_TCIDXSORTED, 0);
	Ex_ObjSetLong(hObj, ERLVL_FTCSORTEDDESC, 0);
	return TRUE;
}

void _reportlistview_tc_clear(HEXOBJ hObj)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
	EX_REPORTLIST_COLUMNINFO* ptc = nullptr;
	for (INT i = 0; i < nCount; i++)
	{
		ptc = (EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + i * sizeof(EX_REPORTLIST_COLUMNINFO));
		Ex_MemFree((LPVOID)ptc->pwzText);
	}
	Ex_MemFree(pTCs);
	Ex_ObjSetLong(hObj, ERLVL_TCINFO, 0);
	Ex_ObjSetLong(hObj, ERLVL_CTCS, 0);
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	reportlistview_tr_s* ptr = nullptr;
	LPVOID pTDs = nullptr;
	for (INT i = 0; i < Array_GetCount(hArr); i++)
	{
		ptr = (reportlistview_tr_s*)Array_GetMember(hArr, i + 1);
		pTDs = ptr->pTDInfo_;
		for (INT j = 0; j < nCount; j++)
		{
			reportlistview_td_s* pTD = ((reportlistview_td_s*)((size_t)pTDs + j * sizeof(reportlistview_td_s)));//---修改
			if (pTD)
			{
				if (pTD->font_)
				{
					_font_destroy(pTD->font_);
				}
				if (pTD->wzText_)
				{
					Ex_MemFree((LPVOID)pTD->wzText_);
				}
			}
		}
		Ex_MemFree(pTDs);
		ptr->pTDInfo_ = 0;
	}
	Ex_ObjSetLong(hObj, ERLVL_TCIDXSORTED, 0);
	Ex_ObjSetLong(hObj, ERLVL_FTCSORTEDDESC, 0);
}

void _reportlistview_tc_update(HEXOBJ hObj)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	LPVOID pTCs = (LPVOID)Ex_ObjGetLong(hObj, ERLVL_TCINFO);
	INT nWidth = 0;
	for (INT i = 0; i < nCount; i++)
	{
		nWidth = nWidth + ((EX_REPORTLIST_COLUMNINFO*)((size_t)pTCs + i * sizeof(EX_REPORTLIST_COLUMNINFO)))->nWidth;
	}
	Ex_ObjSetLong(hObj, ERLVL_ITEMWIDTH, nWidth);
	RECT rc{ 0 };
	Ex_ObjGetRect(hObj, &rc);
	if (nWidth < Ex_Scale(rc.right - rc.left) - 1)
	{
		//列宽小于组件宽度，让横向滚动条滚动到最左边。
		Ex_ObjSendMessage(hObj, WM_HSCROLL, MAKELONG(4, 0), 0);
	}
	Ex_ObjSendMessage(hObj, WM_SIZE, 0, MAKELONG(Ex_Scale(rc.right - rc.left), Ex_Scale(rc.bottom - rc.top)));
}

INT _reportlistview_tr_ins(HEXOBJ hObj, EX_REPORTLIST_ROWINFO* pInsertInfo)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Ex_MemAlloc(sizeof(reportlistview_tr_s));
	pTR->dwStyle_ = pInsertInfo->dwStyle;
	pTR->nImageIndex_ = pInsertInfo->nImageIndex;
	pTR->lParam_ = pInsertInfo->lParam;
	pTR->rowbkcr_ = pInsertInfo->rowBkgCr;
	pTR->nInsertIndex_ = pInsertInfo->nInsertIndex;
	LPVOID pTDs = nullptr;
	if (nCount > 0)
	{
		pTDs = Ex_MemAlloc(sizeof(reportlistview_td_s) * nCount);
	}
	pTR->pTDInfo_ = pTDs;
	nCount = Array_AddMember(hArr, (size_t)pTR, (size_t)pInsertInfo->nInsertIndex);
	if (nCount == 0)
	{
		if (pTDs != 0)
		{
			Ex_MemFree(pTDs);
		}
		Ex_MemFree(pTR);
	}
	return nCount;
}

BOOL _reportlistview_tr_del(HEXOBJ hObj, INT nIndex)
{
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	BOOL ret = FALSE;
	if (Array_DelMember(hArr, nIndex))
	{
		ret = TRUE;
	}
	return ret;
}

void _reportlistview_tr_clear(HEXOBJ hObj)
{
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	Array_Clear(hArr);
}

void _reportlistview_tr_update(HEXOBJ hObj)
{
	INT nCount = Array_GetCount((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO));
	Ex_ObjSendMessage(hObj, LVM_SETITEMCOUNT, nCount, LVSICF_NOSCROLL);
	Ex_ObjSendMessage(hObj, LVM_REDRAWITEMS, 1, nCount);
}

reportlistview_td_s* _reportlistview_td_get(HEXOBJ hObj, INT nIndexTR, INT nIndexTC)
{
	INT nCount = Ex_ObjGetLong(hObj, ERLVL_CTCS);
	reportlistview_td_s* pTD = nullptr;
	if (nIndexTC <= 0 || nIndexTC > nCount)
	{
		return 0;
	}
	array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO);
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember(hArr, nIndexTR);
	if (pTR != 0)
	{
		LPVOID pTDs = pTR->pTDInfo_;
		if (pTDs != 0)
		{
			pTD = (reportlistview_td_s*)((size_t)pTDs + (nIndexTC - 1) * sizeof(reportlistview_td_s));
		}
	}
	return pTD;
}

void _reportlistview_td_settext(HEXOBJ hObj, INT nIndexTR, INT nIndexTC, LPCWSTR wzText)
{
	reportlistview_td_s* pTD = _reportlistview_td_get(hObj, nIndexTR, nIndexTC);
	if (pTD != 0)
	{
		LPCWSTR wzText_ = pTD->wzText_;
		Ex_MemFree((LPVOID)wzText_);
		pTD->wzText_ = StrDupW(wzText);
	}
}

BOOL _reportlistview_li_get(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO* lParam)
{
	INT nIndexTR = lParam->iRow;
	BOOL ret = FALSE;
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), nIndexTR);
	if (pTR != 0)
	{
		lParam->nImageIndex = pTR->nImageIndex_;
		lParam->dwStyle = pTR->dwStyle_;
		lParam->lParam = pTR->lParam_;
		LPVOID pOwner = (LPVOID)Ex_ObjGetLong(hObj, EOL_OWNER);
		LPVOID lpItems = ((listview_s*)pOwner)->lpItems_;
		lParam->dwState = _listview_getitemstate(lpItems, nIndexTR);
		ret = TRUE;
	}
	return ret;
}

BOOL _reportlistview_li_set(HEXOBJ hObj, EX_REPORTLIST_ITEMINFO* lParam)
{
	INT nIndexTR = lParam->iRow;
	BOOL ret = FALSE;
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), nIndexTR);
	if (pTR != 0)
	{
		pTR->nImageIndex_ = lParam->nImageIndex;
		pTR->dwStyle_ = lParam->dwStyle;
		pTR->lParam_ = lParam->lParam;
		pTR->rowbkcr_ = lParam->rowBkgCr;
		ret = TRUE;
	}
	return ret;
}

BOOL _reportlistview_li_getcell(HEXOBJ hObj, EX_REPORTLIST_CELLINFO* lParam, BOOL fJustText)
{
	INT nIndexTR = lParam->iRow;
	INT nIndexTC = lParam->iCol;
	reportlistview_tr_s* pTR = (reportlistview_tr_s*)Array_GetMember((array_s*)Ex_ObjGetLong(hObj, ERLVL_TRINFO), nIndexTR);
	if (pTR != 0)
	{
		reportlistview_td_s* pTD = _reportlistview_td_get(hObj, nIndexTR, nIndexTC);
		lParam->pwzText = pTD->wzText_;
		if (fJustText)
			return TRUE;
		lParam->cellStyle = pTD->cellStyle_;
		lParam->cellBkgCr = pTD->crbk_;
		lParam->cellTextCr = pTD->crText_;
		lParam->cellFont = pTD->font_;
		lParam->lParam = pTD->lParam_;
		return TRUE;
	}
	return FALSE;
}

BOOL _reportlistview_li_setcell(HEXOBJ hObj, EX_REPORTLIST_CELLINFO* lParam, BOOL fJustText, BOOL withoutText)
{
	INT nIndexTR = lParam->iRow;
	INT nIndexTC = lParam->iCol;
	reportlistview_td_s* pTD = _reportlistview_td_get(hObj, nIndexTR, nIndexTC);
	if (pTD != 0)
	{
		LPCWSTR wzText_ = pTD->wzText_;
		if (withoutText)
		{
			goto TEXT;
		}

		pTD->wzText_ = StrDupW(lParam->pwzText);
		Ex_MemFree((LPVOID)wzText_);
		if (fJustText)
		{
			return TRUE;
		}

	TEXT:
		pTD->cellStyle_ = lParam->cellStyle;
		pTD->crbk_ = lParam->cellBkgCr;
		pTD->crText_ = lParam->cellTextCr;
		if (pTD->font_)
		{
			_font_destroy(pTD->font_);
		}
		pTD->font_ = lParam->cellFont;
		pTD->lParam_ = lParam->lParam;
		return TRUE;
	}
	return FALSE;
}

void _reportlistview_regsiter()
{
	EX_CLASSINFO clsInfo{ 0 };
	Ex_ObjGetClassInfoEx(L"listview", &clsInfo);
	m_pfnListView = clsInfo.pfnClsProc;
	Ex_ObjRegister(L"ReportListView", EOS_VSCROLL | EOS_HSCROLL | EOS_VISIBLE, clsInfo.dwStyleEx, clsInfo.dwTextFormat, 11 * sizeof(size_t), clsInfo.hCursor, clsInfo.dwFlags, _reportlistview_proc);
	Ex_ObjRegister(L"ReportListView.Header", EOS_VISIBLE, EOS_EX_FOCUSABLE, 0, 2 * sizeof(size_t), 0, 0, _reportlistview_head_proc);
}