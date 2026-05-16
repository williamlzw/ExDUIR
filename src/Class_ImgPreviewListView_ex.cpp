#include "stdafx.h"
// ----------------------- 数组回调 -----------------------
EX_IMGPREVIEWLISTVIEW_ITEMINFO* _imgpreviewlistview_onarrappend(array_s* pArray, INT nIndex,
	EX_IMGPREVIEWLISTVIEW_ITEMINFO* pvItem, INT nType)
{
	EX_IMGPREVIEWLISTVIEW_ITEMINFO* pItemInfo =
		(EX_IMGPREVIEWLISTVIEW_ITEMINFO*)Ex_MemAlloc(sizeof(EX_IMGPREVIEWLISTVIEW_ITEMINFO));
	if (pItemInfo) {
		pItemInfo->nIndex = pvItem->nIndex;
		pItemInfo->pwzPath = StrDupW(pvItem->pwzPath);
	}
	return pItemInfo;
}
void _imgpreviewlistview_onarrdelete(array_s* pArray, INT nIndex,
	EX_IMGPREVIEWLISTVIEW_ITEMINFO* pvItem, INT nType)
{
	if (pvItem != 0) {
		if (pvItem->pwzPath != 0) Ex_MemFree((LPVOID)pvItem->pwzPath);
		Ex_MemFree(pvItem);
	}
}
void _imgpreviewlistview_onimgarrdelete(array_s* pArray, INT nIndex, HEXIMAGE pvItem, INT nType)
{
	if (pvItem != 0) _img_destroy(pvItem);
}
void _imgpreviewlistview_updatescrollrange(HEXOBJ hObj)
{
	EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || !pData->pItemArray) return;
	INT count = Array_GetCount(pData->pItemArray);
	RECT rcScrollView;
	Ex_ObjGetClientRect(hObj, &rcScrollView);
	INT scrollViewWidth = rcScrollView.right - rcScrollView.left;
	// 布局基于选中矩形：宽 itemWidth+10，间距 10
	INT boxWidth = pData->itemWidth + 10;
	INT boxSpacing = 10;
	if (count > 0) {
		// 左边距10 + count * (boxWidth + boxSpacing) - 最后一个多加的间距10 + 右边距10
		pData->totalWidth = 10 + count * (boxWidth + boxSpacing) - boxSpacing + 10;
	}
	else {
		pData->totalWidth = 0;
	}
	INT maxOffset = pData->totalWidth - scrollViewWidth;
	if (maxOffset < 0) maxOffset = 0;
	if (pData->offset > maxOffset) pData->offset = maxOffset;
	if (pData->offset < 0) pData->offset = 0;
	BOOL needHScroll = (pData->totalWidth > scrollViewWidth);
	if (needHScroll)
	{
		Ex_ObjScrollSetInfo(hObj,
			SCROLLBAR_TYPE_HORZ,
			SIF_RANGE | SIF_PAGE | SIF_POS,
			0,
			maxOffset,
			scrollViewWidth,
			pData->offset,
			TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, TRUE);
	}
	else
	{
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, FALSE);
		pData->offset = 0;
		Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, 0, TRUE);
	}
}
INT _imgpreviewlistview_hscrolldefaultproc(HEXOBJ hObj, WPARAM wParam, INT nLine, INT nPage, BOOL fRedraw)
{
	auto hObjScroll = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
	INT  nPos = 0;
	if (hObjScroll != 0) {
		DWORD nCode = LOWORD(wParam);
		INT   nMin, nMax, oldPos, trackPos;
		Ex_ObjScrollGetInfo(hObj, SCROLLBAR_TYPE_HORZ, &nMin, &nMax, &oldPos, &trackPos);
		if (nCode == SB_PAGELEFT) nPos = oldPos - nPage;
		else if (nCode == SB_PAGERIGHT) nPos = oldPos + nPage;
		else if (nCode == SB_LINELEFT) nPos = oldPos - nLine;
		else if (nCode == SB_LINERIGHT) nPos = oldPos + nLine;
		else if (nCode == SB_LEFT) nPos = nMin;
		else if (nCode == SB_RIGHT) nPos = nMax;
		else {
			oldPos = nMin - 1;
			nPos = Ex_ObjScrollGetTrackPos(hObj, SCROLLBAR_TYPE_HORZ);
		}
		if (nPos < nMin) nPos = nMin;
		if (nPos > nMax) nPos = nMax;
		if (nPos != oldPos) {
			Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, nPos, fRedraw);
		}
	}
	return nPos;
}
// ----------------------- 绘制逻辑 -----------------------
void _imgpreviewlistview_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData || !pData->pItemArray) {
			Ex_ObjEndPaint(hObj, &ps);
			return;
		}
		INT count = Array_GetCount(pData->pItemArray);
		HEXBRUSH hBrushBg = _brush_create(ExARGB(240, 240, 240, 255));
		if (hBrushBg) {
			_canvas_fillrect(ps.hCanvas, hBrushBg, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
			_brush_destroy(hBrushBg);
		}
		if (count == 0) {
			Ex_ObjEndPaint(hObj, &ps);
			return;
		}
		// 创建所需画笔
		HEXBRUSH hBrushSel = _brush_create(ExARGB(120, 120, 120, 255)); // 选中矩形颜色
		HEXBRUSH hBrushHov = _brush_create(ExARGB(200, 200, 200, 100)); // 悬停效果
		INT boxWidth = pData->itemWidth + 10;
		INT boxHeight = pData->itemHeight + 10;
		INT boxSpacing = 10;
		_canvas_cliprect(ps.hCanvas, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
		for (INT i = 0; i < count; i++)
		{
			// 计算选中矩形(外框)坐标
			FLOAT boxLeft = 10.0f + i * (boxWidth + boxSpacing) - pData->offset;
			FLOAT boxTop = 10.0f;
			FLOAT boxRight = boxLeft + boxWidth;
			FLOAT boxBottom = boxTop + boxHeight;
			// 性能优化：不在可视范围内的跳过
			if (boxRight < 0 || boxLeft > ps.uWidth) {
				continue;
			}
			// 计算图片(内框)坐标：在选中矩形内四周缩进5px
			FLOAT imgLeft = boxLeft + 5.0f;
			FLOAT imgTop = boxTop + 5.0f;
			FLOAT imgRight = imgLeft + pData->itemWidth;
			FLOAT imgBottom = imgTop + pData->itemHeight;
			RECT rcBox = { boxLeft, boxTop, boxRight, boxBottom };
			RECT rcImg = { imgLeft, imgTop, imgRight, imgBottom };
			// 1. 绘制选中状态
			if (i == pData->selectedIndex) {
				if (hBrushSel) _canvas_fillrect(ps.hCanvas, hBrushSel, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);
			}
			// 2. 绘制悬停状态
			else if (i == pData->hoverIndex) {
				if (hBrushHov) _canvas_fillrect(ps.hCanvas, hBrushHov, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);
			}
			// 3. 绘制图片
			EX_IMGPREVIEWLISTVIEW_ITEMINFO* pItem = (EX_IMGPREVIEWLISTVIEW_ITEMINFO*)Array_GetMember(pData->pItemArray, i + 1);
			HEXIMAGE hImg = (HEXIMAGE)Array_GetMember(pData->pImageArray, i + 1);
			if (hImg) {
				INT imgW = 0, imgH = 0;
				_img_getsize(hImg, &imgW, &imgH);
				if (imgW > 0 && imgH > 0) {
					FLOAT dstLeft = (FLOAT)rcImg.left, dstTop = (FLOAT)rcImg.top, dstRight = (FLOAT)rcImg.right, dstBottom = (FLOAT)rcImg.bottom;
					FLOAT fImg = (FLOAT)imgW / imgH;
					FLOAT fCell = (FLOAT)pData->itemWidth / pData->itemHeight;
					// 保持宽高比缩放绘制
					if (fImg > fCell) {
						FLOAT nHeight = (FLOAT)pData->itemWidth / fImg;
						FLOAT nTop = rcImg.top + ((rcImg.bottom - rcImg.top) - nHeight) / 2.0f;
						dstTop = nTop; dstBottom = nTop + nHeight;
					}
					else {
						FLOAT nWidth = (FLOAT)pData->itemHeight * fImg;
						FLOAT nLeft = rcImg.left + ((rcImg.right - rcImg.left) - nWidth) / 2.0f;
						dstLeft = nLeft; dstRight = nLeft + nWidth;
					}
					_canvas_drawimagerectrect(ps.hCanvas, hImg, dstLeft, dstTop, dstRight, dstBottom, 0, 0, (FLOAT)imgW, (FLOAT)imgH, 255);
				}
			}
		}
		_canvas_resetclip(ps.hCanvas);
		if (hBrushSel) _brush_destroy(hBrushSel);
		if (hBrushHov) _brush_destroy(hBrushHov);
		Ex_ObjEndPaint(hObj, &ps);
	}
}
// ----------------------- 窗口过程 -----------------------
LRESULT CALLBACK _imgpreviewlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_MemAlloc(sizeof(EX_IMGPREVIEWLISTVIEW_DATA));
		memset(pData, 0, sizeof(EX_IMGPREVIEWLISTVIEW_DATA));
		pData->itemWidth = 120;
		pData->itemHeight = 120;
		pData->selectedIndex = -1;
		pData->hoverIndex = -1;
		pData->pItemArray = Array_Create(0);
		Array_BindEvent(pData->pItemArray, ARRAY_EVENT_ADDMEMBER, _imgpreviewlistview_onarrappend);
		Array_BindEvent(pData->pItemArray, ARRAY_EVENT_DELMEMBER, _imgpreviewlistview_onarrdelete);
		pData->pImageArray = Array_Create(0);
		Array_BindEvent(pData->pImageArray, ARRAY_EVENT_DELMEMBER, _imgpreviewlistview_onimgarrdelete);
		Ex_ObjSetLong(hObj, 0, (LONG_PTR)pData);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, FALSE);
	}
	else if (uMsg == WM_DESTROY)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (pData) {
			Array_Destroy(pData->pItemArray);
			Array_Destroy(pData->pImageArray);
			Ex_MemFree(pData);
		}
	}
	else if (uMsg == WM_PAINT)
	{
		_imgpreviewlistview_paint(hObj);
		return 0;
	}
	else if (uMsg == WM_HSCROLL)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		RECT rc;
		Ex_ObjGetClientRect(hObj, &rc);
		INT nPos = _imgpreviewlistview_hscrolldefaultproc(hObj, wParam, 50, rc.right - rc.left, TRUE);
		if (pData->offset != nPos) {
			pData->offset = nPos;
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 0;
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		SHORT delta = GET_WHEEL_DELTA_WPARAM(wParam);
		INT nScrollAmt = -(INT)((FLOAT)delta / 120.0f * 50.0f);
		INT nMin, nMax, oldPos, trackPos;
		Ex_ObjScrollGetInfo(hObj, SCROLLBAR_TYPE_HORZ, &nMin, &nMax, &oldPos, &trackPos);
		INT nPos = oldPos + nScrollAmt;
		if (nPos < nMin) nPos = nMin;
		if (nPos > nMax) nPos = nMax;
		if (nPos != oldPos) {
			pData->offset = nPos;
			Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, nPos, TRUE);
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		INT x = GET_X_LPARAM(lParam);
		INT y = GET_Y_LPARAM(lParam);
		INT prevHover = pData->hoverIndex;
		pData->hoverIndex = -1;
		INT logicX = x + pData->offset;
		INT boxWidth = pData->itemWidth + 10;
		INT boxHeight = pData->itemHeight + 10;
		INT boxSpacing = 10;
		INT count = Array_GetCount(pData->pItemArray);
		// 判断是否落在Y轴的选中矩形范围内
		if (logicX >= 10 && y >= 10 && y <= 10 + boxHeight) {
			INT relativeX = logicX - 10;
			INT idx = relativeX / (boxWidth + boxSpacing); // 计算落在哪个框
			INT posInBox = relativeX % (boxWidth + boxSpacing); // 计算在框内的位置
			// 确保点击的是矩形区域，而不是间距区域
			if (posInBox <= boxWidth && idx >= 0 && idx < count) {
				pData->hoverIndex = idx;
			}
		}
		if (prevHover != pData->hoverIndex) Ex_ObjInvalidateRect(hObj, 0);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		INT prevSel = pData->selectedIndex;
		// 点击空白区域取消选中，点击项目设置选中
		pData->selectedIndex = pData->hoverIndex;
		if (prevSel != pData->selectedIndex) {
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return 0;
	}
	else if (uMsg == WM_SIZE)
	{
		_imgpreviewlistview_updatescrollrange(hObj);
		Ex_ObjInvalidateRect(hObj, 0);
		return 0;
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_INSERTITEM)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData || lParam == 0) return 0;
		EX_IMGPREVIEWLISTVIEW_ITEMINFO* pInputItem = (EX_IMGPREVIEWLISTVIEW_ITEMINFO*)lParam;
		HEXIMAGE hImg = 0;
		if (pInputItem->pwzPath != 0) {
			_img_createfromfile(pInputItem->pwzPath, &hImg);
		}
		INT nInsertIndex = pInputItem->nIndex;
		if (nInsertIndex <= 0) nInsertIndex = Array_GetCount(pData->pItemArray) + 1;
		Array_AddMember(pData->pImageArray, (size_t)hImg, nInsertIndex);
		size_t ret = Array_AddMember(pData->pItemArray, lParam, nInsertIndex);
		if (ret != 0) {
			_imgpreviewlistview_updatescrollrange(hObj);
			if (wParam != 0) Ex_ObjInvalidateRect(hObj, 0);
		}
		return ret;
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_DELETEITEM)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		INT count = Array_GetCount(pData->pImageArray);
		if (lParam > count)
		{
			return 0;
		}
		Array_DelMember(pData->pImageArray, lParam);
		size_t ret = Array_DelMember(pData->pItemArray, lParam) ? 1 : 0;
		if (ret != 0) {
			// 如果删除的是选中项或选中项之前的项，修正选中索引
			INT delLogicalIdx = lParam - 1; // 1基转0基
			if (pData->selectedIndex == delLogicalIdx) pData->selectedIndex = -1;
			else if (pData->selectedIndex > delLogicalIdx) pData->selectedIndex--;
			_imgpreviewlistview_updatescrollrange(hObj);
			if (wParam != 0) Ex_ObjInvalidateRect(hObj, 0);
		}
		return ret;
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_DELETEALLITEMS)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		INT count = Array_GetCount(pData->pImageArray);
		if (count < 1)
		{
			return 0;
		}
		Array_Clear(pData->pImageArray);
		Array_Clear(pData->pItemArray);
		pData->hoverIndex = -1;
		pData->selectedIndex = -1;
		_imgpreviewlistview_updatescrollrange(hObj);
		if (wParam != 0) Ex_ObjInvalidateRect(hObj, 0);
		return 1;
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_SETITEMSIZE)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		INT width = Ex_Scale(LOWORD(lParam));
		if (width < 20) width = 20;
		pData->itemWidth = width;
		INT height = Ex_Scale(HIWORD(lParam));
		if (height < 20) height = 20;
		pData->itemHeight = height;
		_imgpreviewlistview_updatescrollrange(hObj);
		Ex_ObjInvalidateRect(hObj, 0);
		return 0;
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_GETITEMINFO)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData || lParam <= 0 || lParam > Array_GetCount(pData->pItemArray)) return 0;
		return (LRESULT)Array_GetMember(pData->pItemArray, lParam);
	}
	// 新增：获取选中索引消息 (返回1基索引，0表示未选中)
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_GETSELECTED)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData || pData->selectedIndex < 0) return 0;
		return pData->selectedIndex + 1; // 转换为1基数组索引返回
	}
	else if (uMsg == IMGPREVIEWLISTVIEW_MESSAGE_GETCOUNT)
	{
		EX_IMGPREVIEWLISTVIEW_DATA* pData = (EX_IMGPREVIEWLISTVIEW_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;
		return Array_GetCount(pData->pItemArray);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
void _imgpreviewlistview_register()
{
	WCHAR wzCls[] = L"ImagePreviewListView";
	Ex_ObjRegister(wzCls,
		OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL,
		OBJECT_STYLE_EX_CUSTOMDRAW | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_FOCUSABLE,
		0, 1 * sizeof(size_t), NULL, NULL, _imgpreviewlistview_proc);
}