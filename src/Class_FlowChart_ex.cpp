#include "stdafx.h"

// 注册流程图控件
void _flowchart_register()
{
	WCHAR wzCls[] = L"FlowChart";
	Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
		OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
		0, 6 * sizeof(size_t), NULL, NULL, _flowchart_proc);
}

LRESULT CALLBACK _flowchart_edit_killfocus(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
	LPARAM lParam)
{
	if (nCode == NM_KILLFOCUS) {
		Ex_ObjShow(hObj, FALSE);	
		size_t       len = Ex_ObjGetTextLength(hObj);
		std::wstring text;
		text.resize(len);
		Ex_ObjGetText(hObj, text.data(), len * 2);
		EX_FLOWCHART_NODE_DATA newData;
		auto hParent = Ex_ObjGetParent(hObj);
		Ex_ObjSetFocus(hParent);
		INT nodeId = Ex_ObjGetLong(hParent, FLOWCHART_LONG_DOUBLECLICK_NODEID);
		INT dataId = Ex_ObjGetLong(hParent, FLOWCHART_LONG_DOUBLECLICK_DATAID);
		newData.id = dataId; 
		newData.type = FLOWCHART_NODEDATA_TYPE_EDIT;
		newData.data = (LPVOID)text.data();
		Ex_ObjSendMessage(hParent, FLOWCHART_MESSAGE_UPDATE_NODEDATA,
			(WPARAM)nodeId, (LPARAM)&newData);
	}
	return 0;
}

// 流程图控件过程函数
LRESULT CALLBACK _flowchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		// 初始化流程图数据结构
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_DATA));
		memset(pData, 0, sizeof(EX_FLOWCHART_DATA));
		pData->zoom = 1.0f;
		pData->selectedNode = -1;
		pData->draggingNode = -1;
		pData->connectingSlot = -1;
		pData->connectingNode = -1;
		pData->hoverNode = -1;
		pData->hoverSlot = -1;
		pData->hoverSlotType = -1;
		pData->selectedConnection = -1;
		pData->draggingControlPoint = FALSE;

		Ex_ObjSetLong(hObj, FLOWCHART_LONG_DATA, (LONG_PTR)pData);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_X, 0);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_Y, 0);
		HEXOBJ edit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit", 0,
			OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION | OBJECT_STYLE_VSCROLL |
			EDIT_STYLE_DISABLEMENU  | EDIT_STYLE_NEWLINE,
			10, 30, 150, 30, hObj, 0, DT_LEFT | DT_TOP, 0, 0, 0);
		Ex_ObjHandleEvent(edit, NM_KILLFOCUS, _flowchart_edit_killfocus);
		
		Ex_ObjShow(edit, FALSE);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE, edit);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_NODEID, -1);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_DATAID, -1);

		// 设置滚动条范围
		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000, 0, TRUE);
		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 2000, 0, TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
	}
	else if (uMsg == WM_DESTROY)
	{
		// 清理资源
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		if (pData)
		{
			// 释放所有节点
			for (INT i = 0; i < pData->nodeCount; i++)
			{
				EX_FLOWCHART_NODE* node = &pData->nodes[i];
				Ex_MemFree((void*)node->title);

				// 添加节点数据列表的释放逻辑
				for (INT j = 0; j < node->nodeDataCount; j++)
				{
					EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[j];
					switch (dataItem->type)
					{
					case FLOWCHART_NODEDATA_TYPE_EDIT:
					case FLOWCHART_NODEDATA_TYPE_COMBO:
						if (dataItem->data != NULL) {
							Ex_MemFree((void*)dataItem->data);
						}
						break;
					case FLOWCHART_NODEDATA_TYPE_IMAGE:
						if (dataItem->data != NULL) {
							_img_destroy((HEXIMAGE)dataItem->data);
						}
						break;
					}
				}
				// 释放节点数据列表数组
				Ex_MemFree(node->nodeDataList);

				for (INT j = 0; j < node->inputCount; j++)
				{
					Ex_MemFree((void*)node->inputSlots[j]);
				}
				Ex_MemFree(node->inputSlots);
				Ex_MemFree(node->inputRects);

				for (INT j = 0; j < node->outputCount; j++)
				{
					Ex_MemFree((void*)node->outputSlots[j]);
				}
				Ex_MemFree(node->outputSlots);
				Ex_MemFree(node->outputRects);
			}
			Ex_MemFree(pData->nodes);

			// 释放所有连接
			Ex_MemFree(pData->connections);

			Ex_MemFree(pData);
		}
	}
	else if (uMsg == WM_PAINT)
	{
		_flowchart_paint(hObj);
	}
	else if (uMsg == WM_SIZE)
	{
		_flowchart_updatelayout(hObj);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		_flowchart_onmousemove(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		_flowchart_onlbuttondown(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		_flowchart_onlbuttonup(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		_flowchart_ondoubleclick(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		auto ctrl = (GetAsyncKeyState(VK_CONTROL) & 32768) != 0;
		if (ctrl)
		{
			_flowchart_onmousewheel(hObj, GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;
		}
	}
	else if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_DELETE)
		{
			EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
			if (pData && pData->selectedConnection != -1)
			{
				_flowchart_removeconnection(hObj, pData->selectedConnection);
				pData->selectedConnection = -1;
				return 1;
			}
		}
	}
	else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) {
		_flowchart_onscrollbar(hWnd, hObj, uMsg, wParam, lParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_ADD_NODE)
	{
		return _flowchart_addnode(hObj, (EX_FLOWCHART_NODE*)lParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_REMOVE_NODE)
	{
		return _flowchart_removenode(hObj, (INT)wParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_ADD_CONNECTION)
	{
		return _flowchart_addconnection(hObj, (EX_FLOWCHART_CONNECTION*)lParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_REMOVE_CONNECTION)
	{
		return _flowchart_removeconnection(hObj, (INT)wParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_GET_NODE_COUNT)
	{
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		return pData ? pData->nodeCount : 0;
	}
	else if (uMsg == FLOWCHART_MESSAGE_GET_CONNECTION_COUNT)
	{
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		return pData ? pData->connectionCount : 0;
	}
	else if (uMsg == FLOWCHART_MESSAGE_UPDATE_NODEDATA)
	{
		// wParam = 节点ID, lParam = EX_FLOWCHART_NODE_DATA 结构指针
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData) return 0;

		INT nodeId = (INT)wParam;
		EX_FLOWCHART_NODE_DATA* pNewData = (EX_FLOWCHART_NODE_DATA*)lParam;
		if (!pNewData) return 0;

		// 查找节点
		EX_FLOWCHART_NODE* pNode = _flowchart_findnode(pData, nodeId);
		if (!pNode) return 0;

		// 查找数据项
		INT dataIndex = -1;
		for (INT i = 0; i < pNode->nodeDataCount; i++)
		{
			if (pNode->nodeDataList[i].id == pNewData->id)
			{
				dataIndex = i;
				break;
			}
		}
		if (dataIndex == -1) return 0;

		EX_FLOWCHART_NODE_DATA* pOldData = &pNode->nodeDataList[dataIndex];

		// 释放旧数据资源
		switch (pOldData->type)
		{
		case FLOWCHART_NODEDATA_TYPE_EDIT:
		case FLOWCHART_NODEDATA_TYPE_COMBO:
			if (pOldData->data != NULL) {
				Ex_MemFree((void*)pOldData->data);
			}
			break;
		case FLOWCHART_NODEDATA_TYPE_IMAGE:
			if (pOldData->data != NULL) {
				_img_destroy((HEXIMAGE)pOldData->data);
			}
			break;
		}

		// 复制新数据
		pOldData->type = pNewData->type;
		pOldData->rect = pNewData->rect; // 保留原始位置

		switch (pNewData->type)
		{
		case FLOWCHART_NODEDATA_TYPE_EDIT:
			if (pNewData->data != NULL) {
				pOldData->data = (LPVOID)StrDupW((LPCWSTR)pNewData->data);
			}
			else {
				pOldData->data = NULL;
			}
			break;
		case FLOWCHART_NODEDATA_TYPE_COMBO:
			if (pNewData->data != NULL) {
				EX_FLOWCHART_NODE_COMBO_DATA* pSrcCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)pNewData->data;
				EX_FLOWCHART_NODE_COMBO_DATA* pDestCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
				pDestCombo->count = pSrcCombo->count;
				pDestCombo->current = pSrcCombo->current;
				pDestCombo->options = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * pSrcCombo->count);
				for (INT i = 0; i < pSrcCombo->count; i++) {
					pDestCombo->options[i] = StrDupW(pSrcCombo->options[i]);
				}
				pOldData->data = (LPVOID)pDestCombo;
			}
			else {
				pOldData->data = NULL;
			}
			break;
		case FLOWCHART_NODEDATA_TYPE_IMAGE:
			pOldData->data = pNewData->data; // 直接使用图片句柄
			break;
		default:
			pOldData->data = NULL;
		}

		// 重新计算节点大小
		_flowchart_calcnodesize(hObj, pNode);
		_flowchart_updatelayout(hObj);
		Ex_ObjInvalidateRect(hObj, 0);
		return 1;
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _flowchart_onscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL   bHScoll = uMsg == WM_HSCROLL;
	INT    nCode = LOWORD(wParam);
	INT    nPos = 0;
	HEXOBJ hVSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT);
	HEXOBJ hHSB = Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ);
	RECT   rc;
	Ex_ObjGetClientRect(hObj, &rc);
	INT widthRC = (INT)(rc.right - rc.left);
	INT heightRC = (INT)(rc.bottom - rc.top);
	widthRC = Ex_Scale(widthRC);
	heightRC = Ex_Scale(heightRC);
	INT oPos = 0;
	INT nView = 0;
	INT nPage = 2000;
	INT nLine = 200;
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (bHScoll) {
		oPos = pData->panOffset.x;
		nView = widthRC;
	}
	else {
		oPos = pData->panOffset.y;
		nView = heightRC;
	}

	if (nCode == SB_THUMBPOSITION) {
		if (bHScoll) {
			nPos = Ex_ObjScrollGetTrackPos(hHSB, SCROLLBAR_TYPE_CONTROL);
		}
		else {
			nPos = Ex_ObjScrollGetTrackPos(hVSB, SCROLLBAR_TYPE_CONTROL);
		}
	}
	else {
		// 箭头
		if (nCode == SB_PAGEUP) {
			nPos = oPos - nPage;
		}
		else if (nCode == SB_PAGEDOWN) {
			nPos = oPos + nPage;
		}
		else if (nCode == SB_LINEUP) {
			nPos = oPos - nLine;
		}
		else if (nCode == SB_LINEDOWN) {
			nPos = oPos + nLine;
		}
		else if (nCode == SB_TOP) {
			nPos = 0;
		}
		else {
			return;
		}
	}

	if (nPos != oPos) {
		if (bHScoll) {
			pData->panOffset.x = nPos;
			Ex_ObjScrollSetPos(hHSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
		}
		else {
			pData->panOffset.y = nPos;
			Ex_ObjScrollSetPos(hVSB, SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
		}
		Ex_ObjInvalidateRect(hObj, 0);
	}
}
// 添加节点
INT _flowchart_addnode(HEXOBJ hObj, EX_FLOWCHART_NODE* pNode)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || !pNode) return 0;
	_flowchart_calcnodesize(hObj, pNode);
	// 分配新节点数组
	INT newCount = pData->nodeCount + 1;
	EX_FLOWCHART_NODE* newNodes = (EX_FLOWCHART_NODE*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE) * newCount);

	if (pData->nodeCount > 0)
	{
		memcpy(newNodes, pData->nodes, sizeof(EX_FLOWCHART_NODE) * pData->nodeCount);
		Ex_MemFree(pData->nodes);
	}

	pData->nodes = newNodes;
	EX_FLOWCHART_NODE* newNode = &newNodes[pData->nodeCount];

	// 复制节点数据
	newNode->id = pNode->id;
	newNode->x = pNode->x;
	newNode->y = pNode->y;
	newNode->width = pNode->width;
	newNode->height = pNode->height;


	// 复制标题和描述
	newNode->title = StrDupW(pNode->title);
	// 复制节点数据列表
	newNode->nodeDataCount = pNode->nodeDataCount;
	if (pNode->nodeDataCount > 0)
	{
		newNode->nodeDataList = (EX_FLOWCHART_NODE_DATA*)Ex_MemAlloc(
			sizeof(EX_FLOWCHART_NODE_DATA) * pNode->nodeDataCount);

		for (INT i = 0; i < pNode->nodeDataCount; i++)
		{
			EX_FLOWCHART_NODE_DATA* srcData = &pNode->nodeDataList[i];
			EX_FLOWCHART_NODE_DATA* destData = &newNode->nodeDataList[i];

			// 复制基本数据
			memcpy(destData, srcData, sizeof(EX_FLOWCHART_NODE_DATA));

			// 根据类型复制数据
			switch (srcData->type)
			{
			case FLOWCHART_NODEDATA_TYPE_EDIT:
				if (srcData->data != NULL) {
					destData->data = (LPVOID)StrDupW((LPCWSTR)srcData->data);
				}
				break;
			case FLOWCHART_NODEDATA_TYPE_COMBO:
			{
				if (srcData->data != NULL)
				{
					// 源数据是 EX_FLOWCHART_NODE_COMBO_DATA 结构
					EX_FLOWCHART_NODE_COMBO_DATA* srcCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)srcData->data;

					// 为目标分配新的 COMBO 数据结构
					EX_FLOWCHART_NODE_COMBO_DATA* destCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
					destCombo->count = srcCombo->count;
					destCombo->current = srcCombo->current;

					// 复制选项数组
					destCombo->options = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * srcCombo->count);
					for (INT k = 0; k < srcCombo->count; k++) {
						destCombo->options[k] = StrDupW(srcCombo->options[k]);
					}

					destData->data = (LPVOID)destCombo;
				}
				break;
			}
			case FLOWCHART_NODEDATA_TYPE_IMAGE:
				// 图片数据直接复制句柄
				destData->data = srcData->data;
				break;
			default:
				destData->data = NULL;
			}
		}
	}
	else
	{
		newNode->nodeDataList = NULL;
	}

	// 复制输入插槽
	newNode->inputCount = pNode->inputCount;
	if (pNode->inputCount > 0)
	{
		newNode->inputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * pNode->inputCount);
		newNode->inputRects = (RECT*)Ex_MemAlloc(sizeof(RECT) * pNode->inputCount);
		memcpy(newNode->inputRects, pNode->inputRects, sizeof(RECT) * pNode->inputCount);

		for (INT i = 0; i < pNode->inputCount; i++)
		{
			newNode->inputSlots[i] = StrDupW(pNode->inputSlots[i]);
		}
	}
	else
	{
		newNode->inputSlots = NULL;
		newNode->inputRects = NULL;
	}

	// 复制输出插槽
	newNode->outputCount = pNode->outputCount;
	if (pNode->outputCount > 0)
	{
		newNode->outputSlots = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * pNode->outputCount);
		newNode->outputRects = (RECT*)Ex_MemAlloc(sizeof(RECT) * pNode->outputCount);
		memcpy(newNode->outputRects, pNode->outputRects, sizeof(RECT) * pNode->outputCount);

		for (INT i = 0; i < pNode->outputCount; i++)
		{
			newNode->outputSlots[i] = StrDupW(pNode->outputSlots[i]);
		}
	}
	else
	{
		newNode->outputSlots = NULL;
		newNode->outputRects = NULL;
	}

	pData->nodeCount++;
	_flowchart_updatelayout(hObj);
	Ex_ObjInvalidateRect(hObj, 0);
	return 1;
}

void _flowchart_measure_text(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR text,
	INT maxWidth, FLOAT* width, FLOAT* height)
{
	_canvas_calctextsize(hCanvas, hFont, text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK, maxWidth, 0, width, height);
}

void _flowchart_calcnodesize(HEXOBJ hObj, EX_FLOWCHART_NODE* node)
{
	// 1. 标题区域 (最顶部)
	FLOAT titleHeight = 30.0f;
	FLOAT currentY = titleHeight; // 标题高度

	// 2. 插槽区域 (标题下方)
	FLOAT slotHeight = 0;
	INT slotCount = max(node->inputCount, node->outputCount);
	FLOAT slotAreaHeight = slotCount * 20.0f; // 每个插槽20像素高

	// 3. 节点数据区域 (插槽下方)
	FLOAT dataHeight = 0;
	FLOAT width = 200.0f; // 默认宽度

	// 计算输入插槽位置 (在标题下方)
	if (node->inputCount > 0)
	{
		node->inputRects = (RECT*)Ex_MemAlloc(sizeof(RECT) * node->inputCount);
		for (INT i = 0; i < node->inputCount; i++)
		{
			node->inputRects[i].left = 0;
			node->inputRects[i].top = currentY + i * 20.0f; // 在标题下方
			node->inputRects[i].right = 20.0f;
			node->inputRects[i].bottom = node->inputRects[i].top + 20.0f;
		}
	}

	// 计算输出插槽位置 (在标题下方)
	if (node->outputCount > 0)
	{
		node->outputRects = (RECT*)Ex_MemAlloc(sizeof(RECT) * node->outputCount);
		for (INT i = 0; i < node->outputCount; i++)
		{
			node->outputRects[i].left = width - 20.0f;
			node->outputRects[i].top = currentY + i * 20.0f; // 在标题下方
			node->outputRects[i].right = width;
			node->outputRects[i].bottom = node->outputRects[i].top + 20.0f;
		}
	}

	currentY += slotAreaHeight; // 移动到插槽区域下方

	// 计算节点数据项高度 (在插槽下方)
	for (INT i = 0; i < node->nodeDataCount; i++)
	{
		EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[i];
		FLOAT itemHeight = 0;
		FLOAT itemWidth = 0;
		switch (dataItem->type)
		{
		case FLOWCHART_NODEDATA_TYPE_EDIT:
		{
			auto hCanvas = Ex_ObjGetLong(hObj, OBJECT_LONG_HCANVAS);
			HEXFONT hFont = _font_createfromfamily(L"Arial", 14, 0);
			_flowchart_measure_text(hCanvas, hFont, (LPCWSTR)dataItem->data, width - 5.0f, &itemWidth, &itemHeight);
			itemHeight += 10.0f; //内边距5
			_font_destroy(hFont);
			break;
		}
		case FLOWCHART_NODEDATA_TYPE_IMAGE:
			itemHeight = 185.0f;
			break;
		case FLOWCHART_NODEDATA_TYPE_COMBO:
			itemHeight = 25.0f;
			break;
		default:
			itemHeight = 25.0f;
		}

		dataItem->rect.left = 5.0f;
		dataItem->rect.top = currentY; // 在插槽下方
		dataItem->rect.right = width - 5.0f;
		dataItem->rect.bottom = currentY + itemHeight;

		currentY += itemHeight + 5.0f; // 项高度+间距
		dataHeight += itemHeight + 5.0f;
	}

	// 设置节点最终高度
	node->height = titleHeight + slotAreaHeight + dataHeight + 10.0f; // 底部内边距
	node->width = width;
}


// 移除节点
INT _flowchart_removenode(HEXOBJ hObj, INT nodeId)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || pData->nodeCount == 0) return 0;

	// 查找节点
	INT index = -1;
	for (INT i = 0; i < pData->nodeCount; i++)
	{
		if (pData->nodes[i].id == nodeId)
		{
			index = i;
			break;
		}
	}

	if (index == -1) return 0;

	// 移除与该节点相关的连接
	for (INT i = pData->connectionCount - 1; i >= 0; i--)
	{
		if (pData->connections[i].fromNode == nodeId ||
			pData->connections[i].toNode == nodeId)
		{
			_flowchart_removeconnection(hObj, pData->connections[i].id);
		}
	}

	// 释放节点资源
	EX_FLOWCHART_NODE* node = &pData->nodes[index];
	Ex_MemFree((void*)node->title);
	// 释放节点数据列表资源
	for (INT j = 0; j < node->nodeDataCount; j++)
	{
		EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[j];
		switch (dataItem->type)
		{
		case FLOWCHART_NODEDATA_TYPE_EDIT:
		case FLOWCHART_NODEDATA_TYPE_COMBO:
			if (dataItem->data != NULL) {
				EX_FLOWCHART_NODE_COMBO_DATA* comboData = (EX_FLOWCHART_NODE_COMBO_DATA*)dataItem->data;

				// 释放每个选项字符串
				for (INT k = 0; k < comboData->count; k++) {
					Ex_MemFree((void*)comboData->options[k]);
				}

				// 释放选项数组和 COMBO 结构
				Ex_MemFree(comboData->options);
				Ex_MemFree(comboData);
			}
			break;
		case FLOWCHART_NODEDATA_TYPE_IMAGE:
			if (dataItem->data != NULL)
			{
				_img_destroy((HEXIMAGE)dataItem->data);
			}
			break;
		}
	}
	Ex_MemFree(node->nodeDataList);

	for (INT j = 0; j < node->inputCount; j++)
	{
		Ex_MemFree((void*)node->inputSlots[j]);
	}
	Ex_MemFree(node->inputSlots);
	Ex_MemFree(node->inputRects);

	for (INT j = 0; j < node->outputCount; j++)
	{
		Ex_MemFree((void*)node->outputSlots[j]);
	}
	Ex_MemFree(node->outputSlots);
	Ex_MemFree(node->outputRects);

	// 从数组中移除节点
	if (pData->nodeCount > 1)
	{
		EX_FLOWCHART_NODE* newNodes = (EX_FLOWCHART_NODE*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE) * (pData->nodeCount - 1));

		if (index > 0)
		{
			memcpy(newNodes, pData->nodes, sizeof(EX_FLOWCHART_NODE) * index);
		}

		if (index < pData->nodeCount - 1)
		{
			memcpy(newNodes + index, pData->nodes + index + 1,
				sizeof(EX_FLOWCHART_NODE) * (pData->nodeCount - index - 1));
		}

		Ex_MemFree(pData->nodes);
		pData->nodes = newNodes;
	}
	else
	{
		Ex_MemFree(pData->nodes);
		pData->nodes = NULL;
	}

	pData->nodeCount--;
	_flowchart_updatelayout(hObj);
	Ex_ObjInvalidateRect(hObj, 0);
	return 1;
}

// 添加连接
INT _flowchart_addconnection(HEXOBJ hObj, EX_FLOWCHART_CONNECTION* pConn)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || !pConn) return 0;

	// 检查连接是否已存在
	for (INT i = 0; i < pData->connectionCount; i++)
	{
		if (pData->connections[i].fromNode == pConn->fromNode &&
			pData->connections[i].fromSlot == pConn->fromSlot &&
			pData->connections[i].toNode == pConn->toNode &&
			pData->connections[i].toSlot == pConn->toSlot)
		{
			return 0; // 连接已存在
		}
	}

	// 分配新连接数组
	INT newCount = pData->connectionCount + 1;
	EX_FLOWCHART_CONNECTION* newConns = (EX_FLOWCHART_CONNECTION*)Ex_MemAlloc(sizeof(EX_FLOWCHART_CONNECTION) * newCount);

	if (pData->connectionCount > 0)
	{
		memcpy(newConns, pData->connections, sizeof(EX_FLOWCHART_CONNECTION) * pData->connectionCount);
		Ex_MemFree(pData->connections);
	}

	pData->connections = newConns;
	EX_FLOWCHART_CONNECTION* newConn = &newConns[pData->connectionCount];

	// 复制连接数据
	newConn->id = pConn->id;
	newConn->fromNode = pConn->fromNode;
	newConn->fromSlot = pConn->fromSlot;
	newConn->toNode = pConn->toNode;
	newConn->toSlot = pConn->toSlot;

	pData->connectionCount++;
	// 初始化控制点
	EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, newConn->fromNode);
	EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, newConn->toNode);
	if (fromNode && toNode)
	{
		POINTF fromPt = { 0 };
		POINTF toPt = { 0 };

		// 计算起点坐标 (输出插槽中心)
		if (fromNode->outputCount > newConn->fromSlot)
		{
			RECT rcSlot = fromNode->outputRects[newConn->fromSlot];
			fromPt.x = fromNode->x + (rcSlot.left + rcSlot.right) / 2.0f;
			fromPt.y = fromNode->y + (rcSlot.top + rcSlot.bottom) / 2.0f;
		}
		else // 默认位置 (节点右侧中心)
		{
			fromPt.x = fromNode->x + fromNode->width;
			fromPt.y = fromNode->y + fromNode->height / 2.0f;
		}

		// 计算终点坐标 (输入插槽中心)
		if (toNode->inputCount > newConn->toSlot)
		{
			RECT rcSlot = toNode->inputRects[newConn->toSlot];
			toPt.x = toNode->x + (rcSlot.left + rcSlot.right) / 2.0f;
			toPt.y = toNode->y + (rcSlot.top + rcSlot.bottom) / 2.0f;
		}
		else // 默认位置 (节点左侧中心)
		{
			toPt.x = toNode->x;
			toPt.y = toNode->y + toNode->height / 2.0f;
		}

		// 初始化控制点位置
		_flowchart_initcontrolpoint(newConn, fromPt, toPt);
	}
	// 发送事件通知
	Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_CREATED, newConn->id, 0);
	Ex_ObjInvalidateRect(hObj, 0);
	return 1;
}

// 移除连接
INT _flowchart_removeconnection(HEXOBJ hObj, INT connId)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || pData->connectionCount == 0) return 0;

	// 查找连接
	INT index = -1;
	for (INT i = 0; i < pData->connectionCount; i++)
	{
		if (pData->connections[i].id == connId)
		{
			index = i;
			break;
		}
	}

	if (index == -1) return 0;

	// 从数组中移除连接
	if (pData->connectionCount > 1)
	{
		EX_FLOWCHART_CONNECTION* newConns = (EX_FLOWCHART_CONNECTION*)Ex_MemAlloc(sizeof(EX_FLOWCHART_CONNECTION) * (pData->connectionCount - 1));

		if (index > 0)
		{
			memcpy(newConns, pData->connections, sizeof(EX_FLOWCHART_CONNECTION) * index);
		}

		if (index < pData->connectionCount - 1)
		{
			memcpy(newConns + index, pData->connections + index + 1,
				sizeof(EX_FLOWCHART_CONNECTION) * (pData->connectionCount - index - 1));
		}

		Ex_MemFree(pData->connections);
		pData->connections = newConns;
	}
	else
	{
		Ex_MemFree(pData->connections);
		pData->connections = NULL;
	}

	pData->connectionCount--;
	// 如果删除的是当前选中的连接线，清除选中状态
	if (pData->selectedConnection == connId) {
		pData->selectedConnection = -1;
	}
	// 发送事件通知
	Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_REMOVED, connId, 0);

	Ex_ObjInvalidateRect(hObj, 0);
	return 1;
}

// 更新布局
void _flowchart_updatelayout(HEXOBJ hObj)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;

	// 计算内容边界（使用原始坐标）
	FLOAT minX = 0, minY = 0, maxX = 0, maxY = 0;

	for (INT i = 0; i < pData->nodeCount; i++)
	{
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		FLOAT nodeRight = node->x + node->width;
		FLOAT nodeBottom = node->y + node->height;

		if (node->x < minX) minX = node->x;
		if (node->y < minY) minY = node->y;
		if (nodeRight > maxX) maxX = nodeRight;
		if (nodeBottom > maxY) maxY = nodeBottom;
	}

	// 设置滚动范围（考虑缩放）
	INT scrollRangeX = (INT)((maxX - minX) * pData->zoom) + 100;
	INT scrollRangeY = (INT)((maxY - minY) * pData->zoom) + 100;

	Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, scrollRangeX, TRUE);
	Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, scrollRangeY, TRUE);
}

void _flowchart_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps;
	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		if (!pData)
		{
			Ex_ObjEndPaint(hObj, &ps);
			return;
		}

		// 获取滚动位置
		INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
		INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);

		// 绘制背景
		_canvas_clear(ps.hCanvas, ExARGB(33, 33, 33, 255));

		// 计算缩放后的网格间距
		FLOAT gridSpacing = 20.0f * pData->zoom;
		if (gridSpacing < 5.0f) gridSpacing = 5.0f; // 最小间距限制

		// 绘制网格
		HEXBRUSH hBrushGrid = _brush_create(ExARGB(29, 29, 29, 255));
		for (FLOAT x = fmodf(-scrollX, gridSpacing); x < ps.rcPaint.right; x += gridSpacing)
		{
			_canvas_drawline(ps.hCanvas, hBrushGrid, x, 0, x, (FLOAT)ps.rcPaint.bottom, 1, 0);
		}
		for (FLOAT y = fmodf(-scrollY, gridSpacing); y < ps.rcPaint.bottom; y += gridSpacing)
		{
			_canvas_drawline(ps.hCanvas, hBrushGrid, 0, y, (FLOAT)ps.rcPaint.right, y, 1, 0);
		}
		_brush_destroy(hBrushGrid);
		// 绘制节点
		for (INT i = 0; i < pData->nodeCount; i++)
		{
			_flowchart_drawnode(ps.hCanvas, pData, &pData->nodes[i],
				pData->selectedNode == pData->nodes[i].id,
				pData->zoom, scrollX, scrollY);
		}
		// 绘制连接线（在节点下方）
		HEXBRUSH hBrushLine = _brush_create(ExARGB(253, 212, 0, 255));
		HEXBRUSH hBrushLineSel = _brush_create(ExARGB(162, 74, 74, 255));

		for (INT i = 0; i < pData->connectionCount; i++)
		{
			EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
			BOOL isSelected = (pData->selectedConnection == conn->id);
			HEXBRUSH hBrushCurrent = isSelected ? hBrushLineSel : hBrushLine;

			EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, conn->fromNode);
			EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, conn->toNode);

			if (!fromNode || !toNode) continue;

			// 获取连接点位置（应用缩放和平移）
			POINTF fromPt, toPt;

			if (fromNode->outputCount > conn->fromSlot)
			{
				RECT rcSlot = fromNode->outputRects[conn->fromSlot];
				fromPt.x = (fromNode->x + (rcSlot.left + rcSlot.right) / 2.0f) * pData->zoom - scrollX;
				fromPt.y = (fromNode->y + (rcSlot.top + rcSlot.bottom) / 2.0f) * pData->zoom - scrollY;
			}
			else
			{
				fromPt.x = (fromNode->x + fromNode->width) * pData->zoom - scrollX;
				fromPt.y = (fromNode->y + fromNode->height / 2.0f) * pData->zoom - scrollY;
			}

			if (toNode->inputCount > conn->toSlot)
			{
				RECT rcSlot = toNode->inputRects[conn->toSlot];
				toPt.x = (toNode->x + (rcSlot.left + rcSlot.right) / 2.0f) * pData->zoom - scrollX;
				toPt.y = (toNode->y + (rcSlot.top + rcSlot.bottom) / 2.0f) * pData->zoom - scrollY;
			}
			else
			{
				toPt.x = toNode->x * pData->zoom - scrollX;
				toPt.y = (toNode->y + toNode->height / 2.0f) * pData->zoom - scrollY;
			}

			// 计算曲线控制点
			POINTF curvePoints[4] = {
			{ fromPt.x , fromPt.y  }, // 起点
			{
				conn->controlPoint1.x * pData->zoom - scrollX,
				conn->controlPoint1.y * pData->zoom - scrollY
			}, // 控制点1
			{
				conn->controlPoint2.x * pData->zoom - scrollX,
				conn->controlPoint2.y * pData->zoom - scrollY
			}, // 控制点2
			{ toPt.x, toPt.y } // 终点
			};

			// 绘制4点贝塞尔曲线
			_canvas_drawcurve(ps.hCanvas, hBrushCurrent, curvePoints, 4,
				isSelected ? 1.0f : 0.5f, isSelected ? 3.0f : 2.0f, 0);

			// 默认始终绘制控制点（绿色和蓝色）
			HEXBRUSH hBrushCtrl1 = _brush_create(ExARGB(97, 174, 237, 255));
			_canvas_fillellipse(ps.hCanvas, hBrushCtrl1,
				curvePoints[1].x, curvePoints[1].y,
				6.0f, 6.0f);
			_brush_destroy(hBrushCtrl1);

			HEXBRUSH hBrushCtrl2 = _brush_create(ExARGB(235, 144, 229, 255));
			_canvas_fillellipse(ps.hCanvas, hBrushCtrl2,
				curvePoints[2].x, curvePoints[2].y,
				6.0f, 6.0f);
			_brush_destroy(hBrushCtrl2);

			// 绘制箭头
			FLOAT angle = atan2f(toPt.y - fromPt.y, toPt.x - fromPt.x);
			FLOAT arrowSize = 8.0f;
			POINTF arrowPt1 = {
				toPt.x - cosf(angle + 0.3f) * arrowSize,
				toPt.y - sinf(angle + 0.3f) * arrowSize
			};
			POINTF arrowPt2 = {
				toPt.x - cosf(angle - 0.3f) * arrowSize,
				toPt.y - sinf(angle - 0.3f) * arrowSize
			};

			_canvas_drawline(ps.hCanvas, hBrushLine, toPt.x, toPt.y, arrowPt1.x, arrowPt1.y, 1, 0);
			_canvas_drawline(ps.hCanvas, hBrushLine, toPt.x, toPt.y, arrowPt2.x, arrowPt2.y, 1, 0);
		}

		_brush_destroy(hBrushLine);
		_brush_destroy(hBrushLineSel);

		// 绘制进行中的连接（如果有）
		if (pData->connectingSlot != -1 && pData->connectingNode != -1)
		{
			EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->connectingNode);
			if (node)
			{
				POINTF fromPt, toPt;
				FLOAT mouseX = (FLOAT)Ex_ObjGetLong(hObj, FLOWCHART_LONG_MOUSE_X);
				FLOAT mouseY = (FLOAT)Ex_ObjGetLong(hObj, FLOWCHART_LONG_MOUSE_Y);

				if (pData->connectingSlotType == FLOWCHART_SLOTTYPE_OUTPUT)
				{
					// 从输出插槽开始
					if (node->outputCount > pData->connectingSlot)
					{
						RECT rcSlot = node->outputRects[pData->connectingSlot];
						fromPt.x = (node->x + (rcSlot.left + rcSlot.right) / 2.0f) * pData->zoom - scrollX;
						fromPt.y = (node->y + (rcSlot.top + rcSlot.bottom) / 2.0f) * pData->zoom - scrollY;
					}
					else
					{
						fromPt.x = (node->x + node->width) * pData->zoom - scrollX;
						fromPt.y = (node->y + node->height / 2.0f) * pData->zoom - scrollY;
					}
					toPt.x = mouseX;
					toPt.y = mouseY;
				}
				else
				{
					// 从输入插槽开始
					if (node->inputCount > pData->connectingSlot)
					{
						RECT rcSlot = node->inputRects[pData->connectingSlot];
						fromPt.x = (node->x + (rcSlot.left + rcSlot.right) / 2.0f) * pData->zoom - scrollX;
						fromPt.y = (node->y + (rcSlot.top + rcSlot.bottom) / 2.0f) * pData->zoom - scrollY;
					}
					else
					{
						fromPt.x = node->x * pData->zoom - scrollX;
						fromPt.y = (node->y + node->height / 2.0f) * pData->zoom - scrollY;
					}
					toPt.x = mouseX;
					toPt.y = mouseY;
				}
				// 绘制临时连接线
				HEXBRUSH hBrushTemp = _brush_create(ExARGB(217, 217, 217, 255));
				_canvas_drawline(ps.hCanvas, hBrushTemp, fromPt.x, fromPt.y, toPt.x, toPt.y, 2.0f, D2D1_DASH_STYLE_DASH);

				// 绘制箭头
				FLOAT angle = atan2f(toPt.y - fromPt.y, toPt.x - fromPt.x);
				FLOAT arrowSize = 8.0f;
				POINTF arrowPt1 = {
					toPt.x - cosf(angle + 0.3f) * arrowSize,
					toPt.y - sinf(angle + 0.3f) * arrowSize
				};
				POINTF arrowPt2 = {
					toPt.x - cosf(angle - 0.3f) * arrowSize,
					toPt.y - sinf(angle - 0.3f) * arrowSize
				};

				_canvas_drawline(ps.hCanvas, hBrushTemp, toPt.x, toPt.y, arrowPt1.x, arrowPt1.y, 2.0f, 0);
				_canvas_drawline(ps.hCanvas, hBrushTemp, toPt.x, toPt.y, arrowPt2.x, arrowPt2.y, 2.0f, 0);
				_brush_destroy(hBrushTemp);
			}
		}

		Ex_ObjEndPaint(hObj, &ps);
	}
}

// 绘制三角形箭头
void _flowchart_draw_triangle_arrow(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT size, BOOL left)
{
	// 创建三角形的三个点
	POINTF points[3];

	if (left) // 向左的箭头
	{
		points[0] = { x + size, y };          // 右侧顶点
		points[1] = { x, y + size / 2.0f };   // 中间点
		points[2] = { x + size, y + size };   // 右侧底部点
	}
	else // 向右的箭头
	{
		points[0] = { x, y };                 // 左侧顶点
		points[1] = { x + size, y + size / 2.0f }; // 中间点
		points[2] = { x, y + size };          // 左侧底部点
	}
	HEXPATH hPath;
	_path_create(PATH_FLAG_DISABLESCALE, &hPath);
	_path_open(hPath);
	_path_beginfigure(hPath);
	_path_addline(hPath, points[0].x, points[0].y, points[1].x, points[1].y);
	_path_addline(hPath, points[1].x, points[1].y, points[2].x, points[2].y);
	_path_addline(hPath, points[2].x, points[2].y, points[0].x, points[0].y);
	_path_endfigure(hPath, TRUE);
	_path_close(hPath);

	_canvas_fillpath(hCanvas, hPath, hBrush);
	_path_destroy(hPath);
}

// 绘制节点
void _flowchart_drawnode(HEXCANVAS hCanvas, EX_FLOWCHART_DATA* pData, EX_FLOWCHART_NODE* node, BOOL selected,
	FLOAT zoom, INT scrollX, INT scrollY)
{
	// 计算缩放和平移后的位置
	FLOAT nodeX = node->x * zoom - scrollX;
	FLOAT nodeY = node->y * zoom - scrollY;
	FLOAT nodeWidth = node->width * zoom;
	FLOAT nodeHeight = node->height * zoom;

	// 节点主体
	EXARGB nodeColor = ExARGB(53, 53, 53, 255);

	HEXBRUSH hBrushNode = _brush_create(nodeColor);
	HEXBRUSH hBrushBorder = _brush_create(selected ? ExARGB(255, 255, 255, 255) : ExRGB2ARGB(0, 255));

	// 绘制圆角矩形（使用缩放后的坐标）
	_canvas_fillroundedrect(hCanvas, hBrushNode,
		nodeX, nodeY,
		nodeX + nodeWidth, nodeY + nodeHeight,
		5.0f * zoom, 5.0f * zoom);

	_canvas_drawroundedrect(hCanvas, hBrushBorder,
		nodeX, nodeY,
		nodeX + nodeWidth, nodeY + nodeHeight,
		5.0f * zoom, 5.0f * zoom, 2.0f * zoom, 0);

	// 绘制标题区域（高度也缩放）
	FLOAT titleHeight = 30.0f * zoom;
	EX_RECTF rcTitle = {
		nodeX + 2.0f * zoom, nodeY + 2.0f * zoom,
		nodeX + nodeWidth - 2.0f * zoom, nodeY + titleHeight - 2.0f * zoom
	};

	HEXBRUSH hBrushTitle = _brush_create(ExARGB(34, 34, 34, 255));
	_canvas_fillrect(hCanvas, hBrushTitle,
		rcTitle.left, rcTitle.top,
		rcTitle.right, rcTitle.bottom);

	// 绘制标题文本（字体大小也缩放）
	HEXFONT hFontTitle = _font_createfromfamily(L"Arial", 12 * zoom, FONT_STYLE_BOLD);
	_canvas_drawtext(hCanvas, hFontTitle, ExARGB(217, 217, 217, 255),
		node->title, -1, DT_CENTER | DT_VCENTER,
		rcTitle.left, rcTitle.top,
		rcTitle.right, rcTitle.bottom);
	_font_destroy(hFontTitle);

	// 绘制输入插槽
	for (INT i = 0; i < node->inputCount; i++)
	{
		RECT rcSlot = node->inputRects[i];
		// 转换为画布坐标 (在标题下方)
		FLOAT slotX = nodeX + rcSlot.left * zoom;
		FLOAT slotY = nodeY + rcSlot.top * zoom; // 在标题下方
		FLOAT slotWidth = (rcSlot.right - rcSlot.left) * zoom;
		FLOAT slotHeight = (rcSlot.bottom - rcSlot.top) * zoom;

		// 检查是否悬停在此插槽上
		BOOL isHover = (pData->hoverNode == node->id &&
			pData->hoverSlot == i &&
			pData->hoverSlotType == FLOWCHART_SLOTTYPE_INPUT);
		EXARGB slotColor = isHover ? ExARGB(113, 255, 107, 255) : ExARGB(200, 200, 200, 255); // 悬停时红色，普通时灰色
		HEXBRUSH hBrushSlot = _brush_create(slotColor);

		_canvas_fillellipse(hCanvas, hBrushSlot,
			slotX + slotWidth / 2,
			slotY + slotHeight / 2,
			5.0f * zoom, 5.0f * zoom);

		_canvas_drawellipse(hCanvas, hBrushBorder,
			slotX + slotWidth / 2,
			slotY + slotHeight / 2,
			5.0f * zoom, 5.0f * zoom, 1.0f * zoom, 0);

		// 绘制插槽名称
		HEXFONT hFontSlot = _font_createfromfamily(L"Arial", 10 * zoom, 0);
		_canvas_drawtext(hCanvas, hFontSlot, ExARGB(217, 217, 217, 255),
			node->inputSlots[i], -1, DT_LEFT | DT_VCENTER,
			slotX + slotWidth + 5 * zoom, slotY,
			nodeX + nodeWidth, slotY + slotHeight);
		_font_destroy(hFontSlot);
	}

	// 绘制输出插槽
	for (INT i = 0; i < node->outputCount; i++)
	{
		RECT rcSlot = node->outputRects[i];
		// 转换为画布坐标 (在标题下方)
		FLOAT slotX = nodeX + rcSlot.left * zoom;
		FLOAT slotY = nodeY + rcSlot.top * zoom; // 在标题下方
		FLOAT slotWidth = (rcSlot.right - rcSlot.left) * zoom;
		FLOAT slotHeight = (rcSlot.bottom - rcSlot.top) * zoom;

		// 检查是否悬停在此插槽上
		BOOL isHover = (pData->hoverNode == node->id &&
			pData->hoverSlot == i &&
			pData->hoverSlotType == FLOWCHART_SLOTTYPE_OUTPUT);

		// 使用不同颜色表示悬停状态
		EXARGB slotColor = isHover ? ExARGB(113, 255, 107, 255) : ExARGB(200, 200, 200, 255); // 悬停时红色，普通时灰色
		HEXBRUSH hBrushSlot = _brush_create(slotColor);

		_canvas_fillellipse(hCanvas, hBrushSlot,
			slotX + slotWidth / 2,
			slotY + slotHeight / 2,
			5.0f * zoom, 5.0f * zoom);

		_canvas_drawellipse(hCanvas, hBrushBorder,
			slotX + slotWidth / 2,
			slotY + slotHeight / 2,
			5.0f * zoom, 5.0f * zoom, 1.0f, 0);

		// 绘制插槽名称
		HEXFONT hFontSlot = _font_createfromfamily(L"Arial", 10 * zoom, 0);
		_canvas_drawtext(hCanvas, hFontSlot, ExARGB(217, 217, 217, 255),
			node->outputSlots[i], -1, DT_RIGHT | DT_VCENTER,
			nodeX, slotY,
			slotX - 5 * zoom, slotY + slotHeight);
		_font_destroy(hFontSlot);
	}

	// 绘制节点列表数据项 (插槽下方)
	FLOAT slotAreaHeight = max(node->inputCount, node->outputCount) * 20.0f * zoom;
	FLOAT dataStartY = nodeY + titleHeight + slotAreaHeight;

	for (INT i = 0; i < node->nodeDataCount; i++)
	{
		EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[i];
		RECT rcItem = dataItem->rect;

		// 计算缩放后的位置
		FLOAT itemX = nodeX + rcItem.left * zoom;
		FLOAT itemY = nodeY + rcItem.top * zoom; // 在插槽下方
		FLOAT itemWidth = (rcItem.right - rcItem.left) * zoom;
		FLOAT itemHeight = (rcItem.bottom - rcItem.top) * zoom;

		// 根据类型绘制不同的数据项
		switch (dataItem->type)
		{
		case FLOWCHART_NODEDATA_TYPE_EDIT: // 编辑框
		{
			HEXBRUSH hBrushEdit = _brush_create(ExARGB(34, 34, 34, 255));

			_canvas_fillrect(hCanvas, hBrushEdit, itemX, itemY, itemX + itemWidth, itemY + itemHeight);
			_canvas_drawrect(hCanvas, hBrushBorder, itemX, itemY, itemX + itemWidth, itemY + itemHeight, 1.0f * zoom, 0);

			// 如果有文本数据，绘制文本
			if (dataItem->data != NULL)
			{
				HEXFONT hFont = _font_createfromfamily(L"Arial", 14 * zoom, 0);
				_canvas_drawtext(hCanvas, hFont, ExARGB(217, 217, 217, 255),
					(LPCWSTR)dataItem->data, -1, DT_LEFT | DT_TOP | DT_WORDBREAK,
					itemX + 5.0f * zoom, itemY + 5.0f * zoom,
					itemX + itemWidth - 5.0f * zoom, itemY + itemHeight - 5.0f * zoom);
				_font_destroy(hFont);
			}
			_brush_destroy(hBrushEdit);

			break;
		}
		case FLOWCHART_NODEDATA_TYPE_IMAGE: // 图片框
		{
			if (dataItem->data != NULL)
			{
				HEXIMAGE hImage = (HEXIMAGE)dataItem->data;
				INT imgWidth, imgHeight;
				_img_getsize(hImage, &imgWidth, &imgHeight);
				_canvas_drawimagerectrect(hCanvas, hImage,
					itemX, itemY, itemX + itemWidth, itemY + itemHeight,
					0, 0, imgWidth, imgHeight, 255);
			}
			break;
		}
		case FLOWCHART_NODEDATA_TYPE_COMBO: // 选项卡 - 改为ComfyUI样式
		{
			HEXBRUSH hBrushCombo = _brush_create(ExARGB(34, 34, 34, 255));
			_canvas_fillrect(hCanvas, hBrushCombo, itemX, itemY, itemX + itemWidth, itemY + itemHeight);
			_canvas_drawrect(hCanvas, hBrushBorder, itemX, itemY, itemX + itemWidth, itemY + itemHeight, 1.0f * zoom, 0);

			// 获取 COMBO 数据
			EX_FLOWCHART_NODE_COMBO_DATA* comboData = (EX_FLOWCHART_NODE_COMBO_DATA*)dataItem->data;

			// 计算箭头区域大小
			FLOAT arrowWidth = itemHeight * 0.8f; // 箭头区域宽度
			FLOAT arrowHeight = arrowWidth * 0.6f; // 箭头高度（三角形高度）

			// 左侧箭头区域（左箭头）
			FLOAT leftArrowX = itemX + 5 * zoom;
			FLOAT leftArrowY = itemY + (itemHeight - arrowHeight) / 2;

			// 右侧箭头区域（右箭头）
			FLOAT rightArrowX = itemX + itemWidth - arrowWidth - 5 * zoom;
			FLOAT rightArrowY = itemY + (itemHeight - arrowHeight) / 2;

			// 文本区域（两个箭头之间）
			FLOAT textX = leftArrowX + arrowWidth + 5 * zoom;
			FLOAT textWidth = rightArrowX - textX - 5 * zoom;

			// 绘制左侧箭头（向左的三角形）
			_flowchart_draw_triangle_arrow(hCanvas, hBrushBorder, leftArrowX, leftArrowY, arrowHeight, TRUE);

			// 绘制右侧箭头（向右的三角形）
			_flowchart_draw_triangle_arrow(hCanvas, hBrushBorder, rightArrowX, rightArrowY, arrowHeight, FALSE);

			// 如果有选项，绘制当前选中的文本
			if (comboData != NULL && comboData->count > 0 && comboData->current < comboData->count) {
				LPCWSTR currentText = comboData->options[comboData->current];
				HEXFONT hFont = _font_createfromfamily(L"Arial", 10 * zoom, 0);
				_canvas_drawtext(hCanvas, hFont, ExARGB(217, 217, 217, 255),
					currentText, -1, DT_CENTER | DT_VCENTER,
					textX, itemY,
					textX + textWidth, itemY + itemHeight);
				_font_destroy(hFont);
			}
			_brush_destroy(hBrushCombo);
			break;
		}
		}
	}
	_brush_destroy(hBrushNode);
	_brush_destroy(hBrushBorder);
	_brush_destroy(hBrushTitle);
}

// 鼠标移动处理
void _flowchart_onmousemove(HEXOBJ hObj, INT x, INT y)
{
	Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_X, x);
	Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_Y, y);
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;

	// 获取滚动位置
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
	INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);

	// 转换为虚拟画布坐标
	FLOAT virtualX = (x + scrollX) / pData->zoom;
	FLOAT virtualY = (y + scrollY) / pData->zoom;

	// 记录之前的悬停状态
	INT prevHoverNode = pData->hoverNode;
	INT prevHoverSlot = pData->hoverSlot;
	INT prevHoverSlotType = pData->hoverSlotType;

	// 重置悬停状态
	pData->hoverNode = -1;
	pData->hoverSlot = -1;
	pData->hoverSlotType = -1;

	// 处理控制点拖动
	if (pData->draggingControlPoint && pData->selectedConnection != -1)
	{
		EX_FLOWCHART_CONNECTION* conn = _flowchart_findconnection(pData, pData->selectedConnection);
		if (conn) {
			// 转换为虚拟画布坐标
			FLOAT virtualX = (x + scrollX) / pData->zoom;
			FLOAT virtualY = (y + scrollY) / pData->zoom;

			FLOAT deltaX = virtualX - pData->dragStartPos.x;
			FLOAT deltaY = virtualY - pData->dragStartPos.y;

			if (pData->draggingWhichPoint == 1) { // 拖动控制点1
				conn->controlPoint1.x += deltaX;
				conn->controlPoint1.y += deltaY;
			}
			else if (pData->draggingWhichPoint == 2) { // 拖动控制点2
				conn->controlPoint2.x += deltaX;
				conn->controlPoint2.y += deltaY;
			}

			pData->dragStartPos.x = virtualX;
			pData->dragStartPos.y = virtualY;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (pData->draggingNode != -1)
	{
		// 拖动节点
		EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->draggingNode);
		if (node)
		{
			HEXOBJ edit = Ex_ObjGetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE);
			Ex_ObjShow(edit, FALSE);
			// 计算移动距离
			FLOAT deltaX = virtualX - pData->dragStartPos.x;
			FLOAT deltaY = virtualY - pData->dragStartPos.y;

			// 更新节点位置
			node->x += deltaX;
			node->y += deltaY;

			// 更新起始位置
			pData->dragStartPos.x = virtualX;
			pData->dragStartPos.y = virtualY;

			// 发送节点移动事件
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODE_MOVED, node->id, 0);

			// 重绘
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (pData->connectingSlot != -1)
	{
		// 正在创建连接，只需重绘
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else
	{
		// 检查是否悬停在插槽上

		for (INT i = 0; i < pData->nodeCount; i++)
		{
			EX_FLOWCHART_NODE* node = &pData->nodes[i];
			// 计算节点在画布上的全局坐标
			FLOAT nodeGlobalX = node->x;
			FLOAT nodeGlobalY = node->y;
			// 检查输入插槽
			for (INT j = 0; j < node->inputCount; j++)
			{
				RECT rcSlot = node->inputRects[j];
				// 转换为全局坐标 (在标题下方)
				FLOAT slotLeft = nodeGlobalX + rcSlot.left;
				FLOAT slotTop = nodeGlobalY + rcSlot.top;
				FLOAT slotRight = nodeGlobalX + rcSlot.right;
				FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
				if (virtualX >= slotLeft && virtualX <= slotRight &&
					virtualY >= slotTop && virtualY <= slotBottom)
				{
					pData->hoverNode = node->id;
					pData->hoverSlot = j;
					pData->hoverSlotType = FLOWCHART_SLOTTYPE_INPUT;
					break;
				}
			}

			// 检查输出插槽
			for (INT j = 0; j < node->outputCount; j++)
			{
				RECT rcSlot = node->outputRects[j];
				// 转换为全局坐标 (在标题下方)
				FLOAT slotLeft = nodeGlobalX + rcSlot.left;
				FLOAT slotTop = nodeGlobalY + rcSlot.top;
				FLOAT slotRight = nodeGlobalX + rcSlot.right;
				FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
				if (virtualX >= slotLeft && virtualX <= slotRight &&
					virtualY >= slotTop && virtualY <= slotBottom)
				{
					pData->hoverNode = node->id;
					pData->hoverSlot = j;
					pData->hoverSlotType = FLOWCHART_SLOTTYPE_OUTPUT;
					break;
				}
			}

			if (pData->hoverNode != -1) break;
		}
	}
	// 如果悬停状态发生变化，需要重绘
	if (prevHoverNode != pData->hoverNode ||
		prevHoverSlot != pData->hoverSlot ||
		prevHoverSlotType != pData->hoverSlotType)
	{
		Ex_ObjInvalidateRect(hObj, 0);
	}
}

// 查找连接线
EX_FLOWCHART_CONNECTION* _flowchart_findconnection(EX_FLOWCHART_DATA* pData, INT connId) {
	for (INT i = 0; i < pData->connectionCount; i++) {
		if (pData->connections[i].id == connId) {
			return &pData->connections[i];
		}
	}
	return NULL;
}

// 初始化控制点位置
void _flowchart_initcontrolpoint(EX_FLOWCHART_CONNECTION* conn,
	POINTF virtualFromPt, POINTF virtualToPt) {
	// 计算起点到终点的方向向量
	FLOAT dx = virtualToPt.x - virtualFromPt.x;
	FLOAT dy = virtualToPt.y - virtualFromPt.y;

	// 计算中点
	FLOAT midX = virtualFromPt.x + dx * 0.5f;
	FLOAT midY = virtualFromPt.y + dy * 0.5f;

	// 计算垂直向量（用于创建弯曲效果）
	// 根据起点和终点的相对位置决定弯曲方向
	FLOAT bendDirection = (virtualFromPt.y > virtualToPt.y) ? -1.0f : 1.0f;
	FLOAT perpX = -dy * 0.25f * bendDirection;
	FLOAT perpY = dx * 0.25f * bendDirection;

	// 设置控制点位置（确保在起点和终点之间）
	conn->controlPoint1.x = virtualFromPt.x + dx * 0.25f + perpX;
	conn->controlPoint1.y = virtualFromPt.y + dy * 0.25f + perpY;

	conn->controlPoint2.x = virtualToPt.x - dx * 0.25f + perpX;
	conn->controlPoint2.y = virtualToPt.y - dy * 0.25f + perpY;
}

void _flowchart_onlbuttondown(HEXOBJ hObj, INT x, INT y)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;
	if (pData->draggingControlPoint) {
		return; // 如果正在拖动控制点，不处理其他点击
	}
	// 获取滚动位置
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
	INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);

	// 转换为虚拟画布坐标
	FLOAT virtualX = (x + scrollX) / pData->zoom;
	FLOAT virtualY = (y + scrollY) / pData->zoom;

	// 检查是否点击了节点
	pData->selectedNode = -1;

	// 首先检查是否点击了控制点（优先级最高）
	for (INT i = 0; i < pData->connectionCount; i++)
	{
		EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];

		// 转换为虚拟画布坐标
		FLOAT virtualX = (x + scrollX) / pData->zoom;
		FLOAT virtualY = (y + scrollY) / pData->zoom;

		// 获取控制点位置
		POINTF ctrl1 = conn->controlPoint1;
		POINTF ctrl2 = conn->controlPoint2;

		// 检测控制点1点击
		FLOAT distCtrl1 = sqrtf(powf(virtualX - ctrl1.x, 2) + powf(virtualY - ctrl1.y, 2));
		if (distCtrl1 <= 8.0f)
		{
			pData->selectedConnection = conn->id;
			pData->draggingControlPoint = TRUE;
			pData->dragStartPos.x = virtualX;
			pData->dragStartPos.y = virtualY;
			pData->draggingWhichPoint = 1;

			// 发送连接线选中事件
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_SELECTED, conn->id, 0);
			Ex_ObjInvalidateRect(hObj, 0);
			return;
		}

		// 检测控制点2点击
		FLOAT distCtrl2 = sqrtf(powf(virtualX - ctrl2.x, 2) + powf(virtualY - ctrl2.y, 2));
		if (distCtrl2 <= 8.0f)
		{
			pData->selectedConnection = conn->id;
			pData->draggingControlPoint = TRUE;
			pData->dragStartPos.x = virtualX;
			pData->dragStartPos.y = virtualY;
			pData->draggingWhichPoint = 2;

			// 发送连接线选中事件
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_SELECTED, conn->id, 0);
			Ex_ObjInvalidateRect(hObj, 0);
			return;
		}
	}
	// 检查是否点击了插槽 (标题下方)
	BOOL clickedOnSlot = FALSE; // 标记是否点击了插槽
	for (INT i = pData->nodeCount - 1; i >= 0; i--)
	{
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		// 计算节点在画布上的全局坐标
		FLOAT nodeGlobalX = node->x;
		FLOAT nodeGlobalY = node->y;

		// 首先检查是否点击了插槽（优先级高于节点主体）
		for (INT j = 0; j < node->inputCount; j++)
		{
			RECT rcSlot = node->inputRects[j];
			// 转换为全局坐标 (在标题下方)
			FLOAT slotLeft = nodeGlobalX + rcSlot.left;
			FLOAT slotTop = nodeGlobalY + rcSlot.top;
			FLOAT slotRight = nodeGlobalX + rcSlot.right;
			FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
			if (virtualX >= slotLeft && virtualX <= slotRight &&
				virtualY >= slotTop && virtualY <= slotBottom)
			{
				pData->selectedNode = node->id;
				pData->connectingSlot = j;
				pData->connectingNode = node->id;
				pData->connectingSlotType = FLOWCHART_SLOTTYPE_INPUT;
				clickedOnSlot = TRUE; // 标记点击了插槽
				break;
			}
		}

		if (!clickedOnSlot) // 如果没有点击输入插槽，再检查输出插槽
		{
			for (INT j = 0; j < node->outputCount; j++)
			{
				RECT rcSlot = node->outputRects[j];
				// 转换为全局坐标 (在标题下方)
				FLOAT slotLeft = nodeGlobalX + rcSlot.left;
				FLOAT slotTop = nodeGlobalY + rcSlot.top;
				FLOAT slotRight = nodeGlobalX + rcSlot.right;
				FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
				if (virtualX >= slotLeft && virtualX <= slotRight &&
					virtualY >= slotTop && virtualY <= slotBottom)
				{
					pData->selectedNode = node->id;
					pData->connectingSlot = j;
					pData->connectingNode = node->id;
					pData->connectingSlotType = FLOWCHART_SLOTTYPE_OUTPUT;
					clickedOnSlot = TRUE; // 标记点击了插槽
					break;
				}
			}
		}

		// 如果点击了节点主体（且没有点击插槽）
		if (!clickedOnSlot && virtualX >= node->x && virtualX <= node->x + node->width &&
			virtualY >= node->y && virtualY <= node->y + node->height)
		{
			pData->selectedNode = node->id;
			Ex_ObjInvalidateRect(hObj, 0);

			// 发送节点点击事件
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODE_CLICKED, node->id, 0);

			// 开始拖动节点
			pData->draggingNode = node->id;
			pData->dragStartPos = { virtualX, virtualY };
			break;
		}
		// 如果点击了插槽，不再检查节点主体
		if (clickedOnSlot)
		{
			Ex_ObjInvalidateRect(hObj, 0);
			break; // 直接返回，避免触发节点拖动
		}
	}
	// ============== 新增：检查是否点击了COMBO数据项的箭头 ==============
	BOOL clickedOnComboArrow = FALSE;

	for (INT i = 0; i < pData->nodeCount; i++)
	{
		EX_FLOWCHART_NODE* node = &pData->nodes[i];

		// 检查每个数据项
		for (INT k = 0; k < node->nodeDataCount; k++)
		{
			EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[k];

			// 只处理COMBO类型
			if (dataItem->type == FLOWCHART_NODEDATA_TYPE_COMBO && dataItem->data != NULL)
			{
				// 获取数据项在画布上的位置
				FLOAT itemX = node->x + dataItem->rect.left;
				FLOAT itemY = node->y + dataItem->rect.top;
				FLOAT itemWidth = dataItem->rect.right - dataItem->rect.left;
				FLOAT itemHeight = dataItem->rect.bottom - dataItem->rect.top;

				// 检查点击是否在数据项内
				if (virtualX >= itemX && virtualX <= itemX + itemWidth &&
					virtualY >= itemY && virtualY <= itemY + itemHeight)
				{
					EX_FLOWCHART_NODE_COMBO_DATA* comboData = (EX_FLOWCHART_NODE_COMBO_DATA*)dataItem->data;

					// 计算箭头区域大小
					FLOAT arrowWidth = itemHeight * 0.8f; // 箭头区域宽度
					FLOAT arrowHeight = arrowWidth * 0.6f; // 箭头高度（三角形高度）

					// 左侧箭头区域（左箭头）
					FLOAT leftArrowX = itemX + 5 * pData->zoom;
					FLOAT leftArrowY = itemY + (itemHeight - arrowHeight) / 2;
					RECT rcLeftArrow = {
						(LONG)leftArrowX,
						(LONG)leftArrowY,
						(LONG)(leftArrowX + arrowHeight),
						(LONG)(leftArrowY + arrowHeight)
					};

					// 右侧箭头区域（右箭头）
					FLOAT rightArrowX = itemX + itemWidth - arrowWidth - 5 * pData->zoom;
					FLOAT rightArrowY = itemY + (itemHeight - arrowHeight) / 2;
					RECT rcRightArrow = {
						(LONG)rightArrowX,
						(LONG)rightArrowY,
						(LONG)(rightArrowX + arrowHeight),
						(LONG)(rightArrowY + arrowHeight)
					};

					// 检查点击左箭头
					if (virtualX >= rcLeftArrow.left && virtualX <= rcLeftArrow.right &&
						virtualY >= rcLeftArrow.top && virtualY <= rcLeftArrow.bottom)
					{
						// 切换到上一个选项
						if (comboData->count > 0)
						{
							comboData->current = (comboData->current - 1 + comboData->count) % comboData->count;
							Ex_ObjInvalidateRect(hObj, 0);

							// 发送选项改变事件
							Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODEDATA_CHANGED, node->id, (LPARAM)dataItem);
						}
						clickedOnComboArrow = TRUE;
						break;
					}
					// 检查点击右箭头
					else if (virtualX >= rcRightArrow.left && virtualX <= rcRightArrow.right &&
						virtualY >= rcRightArrow.top && virtualY <= rcRightArrow.bottom)
					{
						// 切换到下一个选项
						if (comboData->count > 0)
						{
							comboData->current = (comboData->current + 1) % comboData->count;
							Ex_ObjInvalidateRect(hObj, 0);

							// 发送选项改变事件
							Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODEDATA_CHANGED, node->id, (LPARAM)dataItem);
						}
						clickedOnComboArrow = TRUE;
						break;
					}
				}
			}
		}
		if (clickedOnComboArrow) break;
	}
}

// 鼠标左键释放
void _flowchart_onlbuttonup(HEXOBJ hObj, INT x, INT y)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;
	// 结束控制点拖动
	if (pData->draggingControlPoint) {
		pData->draggingControlPoint = FALSE;
		pData->draggingWhichPoint = 0; // 重置拖动标记
		// 发送连接线移动事件
		if (pData->selectedConnection != -1) {
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_MOVED,
				pData->selectedConnection, 0);
		}
	}
	if (pData->connectingSlot != -1 && pData->connectingNode != -1)
	{
		// 获取滚动位置
		INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
		INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);

		// 转换为虚拟画布坐标
		FLOAT virtualX = (x + scrollX) / pData->zoom;
		FLOAT virtualY = (y + scrollY) / pData->zoom;

		// 检查是否释放在另一个插槽上
		for (INT i = 0; i < pData->nodeCount; i++)
		{
			EX_FLOWCHART_NODE* node = &pData->nodes[i];
			// 计算节点在画布上的全局坐标
			FLOAT nodeGlobalX = node->x;
			FLOAT nodeGlobalY = node->y;
			if (node->id == pData->connectingNode) continue; // 跳过自身

			if (pData->connectingSlotType == FLOWCHART_SLOTTYPE_OUTPUT)
			{
				// 输出插槽 -> 寻找输入插槽
				for (INT j = 0; j < node->inputCount; j++)
				{
					RECT rcSlot = node->inputRects[j];
					// 转换为全局坐标
					FLOAT slotLeft = nodeGlobalX + rcSlot.left;
					FLOAT slotTop = nodeGlobalY + rcSlot.top;
					FLOAT slotRight = nodeGlobalX + rcSlot.right;
					FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
					if (virtualX >= slotLeft && virtualX <= slotRight &&
						virtualY >= slotTop && virtualY <= slotBottom)
					{
						// 创建连接
						EX_FLOWCHART_CONNECTION conn;
						conn.id = (INT)GetTickCount64(); // 简单生成唯一ID
						conn.fromNode = pData->connectingNode;
						conn.fromSlot = pData->connectingSlot;
						conn.toNode = node->id;
						conn.toSlot = j;

						_flowchart_addconnection(hObj, &conn);
						break;
					}
				}
			}
			else
			{
				// 输入插槽 -> 寻找输出插槽
				for (INT j = 0; j < node->outputCount; j++)
				{
					RECT rcSlot = node->outputRects[j];
					// 转换为全局坐标
					FLOAT slotLeft = nodeGlobalX + rcSlot.left;
					FLOAT slotTop = nodeGlobalY + rcSlot.top;
					FLOAT slotRight = nodeGlobalX + rcSlot.right;
					FLOAT slotBottom = nodeGlobalY + rcSlot.bottom;
					if (virtualX >= slotLeft && virtualX <= slotRight &&
						virtualY >= slotTop && virtualY <= slotBottom)
					{
						// 创建连接
						EX_FLOWCHART_CONNECTION conn;
						conn.id = (INT)GetTickCount64(); // 简单生成唯一ID
						conn.fromNode = node->id;
						conn.fromSlot = j;
						conn.toNode = pData->connectingNode;
						conn.toSlot = pData->connectingSlot;

						_flowchart_addconnection(hObj, &conn);
						break;
					}
				}
			}
		}

		// 重置连接状态
		pData->connectingSlot = -1;
		pData->connectingNode = -1;
	}

	// 结束拖动
	pData->draggingNode = -1;
	Ex_ObjInvalidateRect(hObj, 0);
}

// 鼠标双击
void _flowchart_ondoubleclick(HEXOBJ hObj, INT x, INT y)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;

	// 获取滚动位置
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ);
	INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);

	// 转换为虚拟画布坐标
	FLOAT virtualX = (x + scrollX) / pData->zoom;
	FLOAT virtualY = (y + scrollY) / pData->zoom;

	// 检查是否双击了节点
	for (INT i = 0; i < pData->nodeCount; i++)
	{
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		if (virtualX >= node->x && virtualX <= node->x + node->width &&
			virtualY >= node->y && virtualY <= node->y + node->height)
		{
			for (INT j = 0; j < node->nodeDataCount; j++)
			{
				EX_FLOWCHART_NODE_DATA* dataItem = &node->nodeDataList[j];
				if (dataItem->type == FLOWCHART_NODEDATA_TYPE_EDIT)
				{
					if (virtualX - node->x >= dataItem->rect.left && virtualX - node->x <= dataItem->rect.right && 
						virtualY - node->y >= dataItem->rect.top && virtualY - node->y <= dataItem->rect.bottom)
					{
						Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_NODEID, node->id);
						Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_DATAID, dataItem->id);
						HEXOBJ edit = Ex_ObjGetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE);
						Ex_ObjSetText(edit, (LPCWSTR)dataItem->data, FALSE);
						EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
						auto dpi = Ex_DUIGetSystemDpi();
						Ex_ObjMove(edit, (node->x + dataItem->rect.left) / dpi * pData->zoom, (node->y + dataItem->rect.top) / dpi * pData->zoom, 
							(dataItem->rect.right - dataItem->rect.left) / dpi * pData->zoom, (dataItem->rect.bottom - dataItem->rect.top) / dpi * pData->zoom, TRUE);
						Ex_ObjShow(edit, TRUE);
						break;
					}
				}
			}
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODE_DOUBLE_CLICKED, node->id, 0);
			break;
		}
	}
}

// 鼠标滚轮事件
void _flowchart_onmousewheel(HEXOBJ hObj, SHORT delta)
{
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;
	// 调整缩放比例
	FLOAT zoomFactor = delta > 0 ? 1.1f : 0.9f;
	FLOAT newZoom = pData->zoom * zoomFactor;

	// 限制缩放范围
	if (newZoom < 1.0f) newZoom = 1.0f;
	if (newZoom > 3.0f) newZoom = 3.0f;
	pData->zoom = newZoom;

	_flowchart_updatelayout(hObj);
	Ex_ObjInvalidateRect(hObj, 0);
}

// 查找节点
EX_FLOWCHART_NODE* _flowchart_findnode(EX_FLOWCHART_DATA* pData, INT nodeId)
{
	for (INT i = 0; i < pData->nodeCount; i++)
	{
		if (pData->nodes[i].id == nodeId)
		{
			return &pData->nodes[i];
		}
	}
	return NULL;
}