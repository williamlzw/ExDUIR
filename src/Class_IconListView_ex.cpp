#include "Class_IconListView_ex.h"

ClsPROC m_pfnListView_icon;


LRESULT CALLBACK _iconlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		_iconlistview_init(hObj);
	}
	else if (uMsg == WM_DESTROY)
	{
		_iconlistview_uninit(hObj);
	}
	else if (uMsg == WM_NOTIFY)
	{
		if (_iconlistview_onnotify(hObj, wParam, lParam))
		{
			return 1;
		}
	}
	else if (uMsg == LVM_INSERTITEM)//插入表项 lParam 为表项信息,wParam为是否立即重画,返回索引
	{
		size_t ret = 0;
		if (lParam != 0)
		{
			ret = Array_AddMember((array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY), lParam, ((EX_ICONLISTVIEW_ITEMINFO*)lParam)->nIndex);
		}
		if (ret != 0 && wParam != 0)//当插入成功且wParam不为0时重画控件
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return ret;
	}
	else if (uMsg == LVM_DELETEITEM)//删除表项 wParam为是否立即重画，lParam为删除的索引
	{
		array_s* pArray = (array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY);
		size_t ret = 0;
		if (Array_GetCount(pArray) >= lParam)
		{
			ret= Array_DelMember(pArray, lParam)?1:0;
		}
		if (ret = !0 && wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return ret;
	}
	else if (uMsg == LVM_DELETEALLITEMS)//删除全部表项
	{
		array_s* pArray = (array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY);
		Array_Clear(pArray);
		if (wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 0;
	}
	else if (uMsg == LVM_UPDATE)//更新列表
	{
		array_s* pArray = (array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY);
		if (Ex_ObjSendMessage(hObj, LVM_GETITEMCOUNT, 0, 0) != Array_GetCount(pArray))//如果表项数不同则更新表项数
		{
			Ex_ObjSendMessage(hObj, LVM_SETITEMCOUNT, Array_GetCount(pArray), 1);
		}
	}
	else if (uMsg == LVM_SETIMAGELIST)//设置列表图片组 wParam为是否立即重画，lParam为图片组句柄
	{
		Ex_ObjSetLong(hObj, EILVL_HIMAGELIST, lParam);
		
		if (wParam != 0)
		{
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 1;
	}
	else if (uMsg == LVM_GETIMAGELIST)//获取图片组
	{
		return Ex_ObjGetLong(hObj, EILVL_HIMAGELIST);
	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)//当鼠标左键按下放开时 更新控件按下状态并重画
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & EILVS_BUTTON) != 0)//当前得是按钮列表状态
		{
			Ex_ObjSetUIState(hObj, STATE_DOWN, uMsg == WM_LBUTTONUP, 0, TRUE);
			Ex_ObjPostMessage(hObj, LVM_SETSELECTIONMARK, 0, 0);
		}
	}
	else if (uMsg == ILVM_SETITEMSIZE)//设置表项尺寸
	{
		INT width = LOWORD(lParam);
		if (width < 10) width = 10;
		Ex_ObjSetLong(hObj, EILVL_WIDTH, Ex_Scale(width));
		INT height = HIWORD(lParam);
		if (height < 10) height = 10;
		Ex_ObjSetLong(hObj, EILVL_HEIGHT, Ex_Scale(height));
		RECT rc{ 0 };
		Ex_ObjGetRect(hObj, &rc);
		Ex_ObjDispatchMessage(hObj, WM_SIZE, 0, MAKELONG(rc.right - rc.left, rc.bottom - rc.top));
		return 0;
	}
	return Ex_ObjCallProc(m_pfnListView_icon, hWnd, hObj, uMsg, wParam, lParam);
}

void _iconlistview_register()
{
	EX_CLASSINFO clsInfo{ 0 };
	Ex_ObjGetClassInfoEx(L"ListView", &clsInfo);
	m_pfnListView_icon = clsInfo.pfnClsProc;
	Ex_ObjRegister(L"IconListView", EOS_VSCROLL | EOS_HSCROLL | EOS_VISIBLE, clsInfo.dwStyleEx, clsInfo.dwTextFormat, 4 *sizeof(size_t), clsInfo.hCursor, clsInfo.dwFlags, _iconlistview_proc);
}

EX_ICONLISTVIEW_ITEMINFO* _iconlistview_onarrappend(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType)
{
	EX_ICONLISTVIEW_ITEMINFO* pItemInfo = (EX_ICONLISTVIEW_ITEMINFO*)Ex_MemAlloc(sizeof(EX_ICONLISTVIEW_ITEMINFO));
	pItemInfo->pwzText = StrDupW(pvItem->pwzText);
	pItemInfo->nIndex = pvItem->nIndex;
	pItemInfo->nImageIndex = pvItem->nImageIndex;
	return pItemInfo;
}

void _iconlistview_onarrdelete(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType)
{
	Ex_MemFree((LPVOID)pvItem->pwzText);
	Ex_MemFree(pvItem);
}

void _iconlistview_init(HEXOBJ hObj)
{
	Ex_ObjSetLong(hObj, EILVL_WIDTH, Ex_Scale(90));
	Ex_ObjSetLong(hObj, EILVL_HEIGHT, Ex_Scale(100));
	array_s* pArray = Array_Create(0);
	Array_BindEvent(pArray, eae_addmember, _iconlistview_onarrappend);
	Array_BindEvent(pArray, eae_delmember, _iconlistview_onarrdelete);
	Ex_ObjSetLong(hObj, EILVL_ITEMARRAY, (size_t)pArray);
	Ex_ObjSetLong(hObj, EILVL_HIMAGELIST, 0);
}

void _iconlistview_uninit(HEXOBJ hObj)
{
	array_s* pArray = (array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY);
	Array_Destroy(pArray);
}

BOOL _iconlistview_onnotify(HEXOBJ hObj, WPARAM wParam, LPARAM lParam)
{
	EX_NMHDR ni{ 0 };
	RtlMoveMemory(&ni, (LPVOID)lParam, sizeof(EX_NMHDR));
	if (ni.hObjFrom == hObj)
	{
		if (ni.nCode == NM_CALCSIZE)
		{
			__set_int((LPVOID)ni.lParam, 0, Ex_ObjGetLong(hObj, EILVL_WIDTH));
			__set_int((LPVOID)ni.lParam, 4, Ex_ObjGetLong(hObj, EILVL_HEIGHT));
			return TRUE;
		}
		else if (ni.nCode == NM_CUSTOMDRAW)
		{
			return _iconlistview_ondrawitem(hObj, (EX_CUSTOMDRAW*)ni.lParam);
		}
	}
	return FALSE;
}

BOOL _iconlistview_ondrawitem(HEXOBJ hObj, EX_CUSTOMDRAW* cdr)
{
	array_s* pArray = (array_s*)Ex_ObjGetLong(hObj, EILVL_ITEMARRAY);
	if (Array_GetCount(pArray) >= cdr->iItem && cdr->iItem > 0)//如果索引在范围内
	{
		LPVOID hImageList = (LPVOID)Ex_ObjGetLong(hObj, EILVL_HIMAGELIST);
		EX_ICONLISTVIEW_ITEMINFO* pItemInfo = (EX_ICONLISTVIEW_ITEMINFO*)Array_GetMember(pArray, cdr->iItem);
		INT nWidthIcon = 0;
		INT nHeightIcon =0;
		if (hImageList && pItemInfo ->nImageIndex!= 0)//列表设置了图片组且当前表象图标索引不为0
		{
			_imglist_size(hImageList, &nWidthIcon, &nHeightIcon);
		}
		FLOAT nWidthText = 0;
		FLOAT nHeightText = 0;
		if (pItemInfo->pwzText != 0)
		{
			_canvas_calctextsize(cdr->hCanvas, Ex_ObjGetFont(hObj), pItemInfo->pwzText, -1, Ex_ObjGetLong(hObj, EOL_TEXTFORMAT), 0, cdr->rcPaint.right - cdr->rcPaint.left, cdr->rcPaint.bottom - cdr->rcPaint.top, &nWidthText, &nHeightText);
		}
		FLOAT nHeightTotal = nHeightIcon + Ex_Scale(3) + nHeightText;

		//本矩形是宽度占满,高度为文本、图标高度的矩形
		RECT rcIconAndText;
		rcIconAndText.left = cdr->rcPaint.left;
		rcIconAndText.top = (cdr->rcPaint.top + cdr->rcPaint.bottom - nHeightTotal) / 2;
		rcIconAndText.right = cdr->rcPaint.right;
		rcIconAndText.bottom = (cdr->rcPaint.top + cdr->rcPaint.bottom + nHeightTotal) / 2;

		//设置剪辑区(防止图标过大、文本过长超出表项)
		_canvas_cliprect(cdr->hCanvas, cdr->rcPaint.left, cdr->rcPaint.top, cdr->rcPaint.right, cdr->rcPaint.bottom);
		LPVOID hBrush = 0;
		if ((cdr->dwStyle & EILVS_BUTTON) != 0)//如果是按钮状态,则处理背景
		{
			if ((cdr->dwState & STATE_HOVER) != 0 && (Ex_ObjGetUIState(hObj) & STATE_DOWN) != 0)//当当前表项是热点且左键按下时
			{
				hBrush = _brush_create(ExRGB2ARGB(16777215, 100));
			}
			else if ((cdr->dwState & STATE_HOVER) != 0)//当当前表项是热点时
			{
				hBrush = _brush_create(ExRGB2ARGB(16777215, 50));
			}
			if (hBrush != 0)
			{
				_canvas_fillrect(cdr->hCanvas, hBrush, cdr->rcPaint.left, cdr->rcPaint.top, cdr->rcPaint.right, cdr->rcPaint.bottom);
				_brush_destroy(hBrush);
			}
		}

		_canvas_setimageantialias(cdr->hCanvas, TRUE);
		if (hImageList != 0 && pItemInfo->nImageIndex != 0)//列表设置了图片组且当前表象图标索引不为0
		{
			_imglist_draw(hImageList, pItemInfo->nImageIndex, cdr->hCanvas, rcIconAndText.left, rcIconAndText.top, rcIconAndText.right, rcIconAndText.top + nHeightIcon, 255);
		}
		if (pItemInfo->pwzText != 0)//当前表项有文本
		{
	
			_canvas_drawtext(cdr->hCanvas, Ex_ObjGetFont(hObj), Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL), pItemInfo->pwzText, -1, Ex_ObjGetLong(hObj, EOL_TEXTFORMAT), rcIconAndText.left, rcIconAndText.bottom - nHeightText, rcIconAndText.right, rcIconAndText.bottom);
		}
		_canvas_resetclip(cdr->hCanvas);//重置剪辑区
	}
	return (cdr->dwStyle & EILVS_BUTTON) != 0;// 如果不是按钮列表状态，则表项选中、热点的状态交给列表框绘制
}