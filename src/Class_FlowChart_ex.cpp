#include "stdafx.h"
#define RYML_SINGLE_HDR_DEFINE_NOW
#include "rapidyaml/rapidyaml.hpp"

// ================= 辅助函数 =================
EXARGB _flowchart_get_port_color(INT dataType) {
	switch (dataType) {
	case FLOWCHART_DATATYPE_IMAGE: return ExARGB(180, 80, 80, 255);
	case FLOWCHART_DATATYPE_STRING: return ExARGB(80, 80, 180, 255);
	case FLOWCHART_DATATYPE_COMBO: return ExARGB(150, 50, 150, 255);
	default: return ExARGB(200, 200, 200, 255);
	}
}

// 计算点到线段距离，用于选中连线
FLOAT _flowchart_dist_to_segment(POINTF p, POINTF v, POINTF w) {
	FLOAT l2 = (w.x - v.x) * (w.x - v.x) + (w.y - v.y) * (w.y - v.y);
	if (l2 == 0.0f) return sqrtf((p.x - v.x) * (p.x - v.x) + (p.y - v.y) * (p.y - v.y));
	float t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
	t = __max(0, __min(1, t));
	POINTF projection = { v.x + t * (w.x - v.x), v.y + t * (w.y - v.y) };
	return sqrtf((p.x - projection.x) * (p.x - projection.x) + (p.y - projection.y) * (p.y - projection.y));
}

void _flowchart_measure_text(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR text, INT maxWidth, FLOAT* width, FLOAT* height) {
	_canvas_calctextsize(hCanvas, hFont, text, -1, DT_LEFT | DT_TOP | DT_WORDBREAK, maxWidth, 0, width, height);
}

void _flowchart_draw_triangle_arrow(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT size, BOOL left) {
	POINTF points[3];
	if (left) {
		points[0] = { x + size, y }; points[1] = { x, y + size / 2.0f }; points[2] = { x + size, y + size };
	}
	else {
		points[0] = { x, y }; points[1] = { x + size, y + size / 2.0f }; points[2] = { x, y + size };
	}
	HEXPATH hPath; _path_create(PATH_FLAG_DISABLESCALE, &hPath); _path_open(hPath); _path_beginfigure2(hPath, points[0].x, points[0].y);
	_path_addline(hPath, points[0].x, points[0].y, points[1].x, points[1].y);
	_path_addline(hPath, points[1].x, points[1].y, points[2].x, points[2].y);
	_path_addline(hPath, points[2].x, points[2].y, points[0].x, points[0].y);
	_path_endfigure(hPath, TRUE); _path_close(hPath);
	_canvas_fillpath(hCanvas, hPath, hBrush); _path_destroy(hPath);
}

// ================= 核心逻辑实现 =================
void _flowchart_register() {
	WCHAR wzCls[] = L"FlowChart";
	Ex_ObjRegister(wzCls, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_HSCROLL | OBJECT_STYLE_VSCROLL,
		OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED,
		0, 7 * sizeof(size_t), NULL, NULL, _flowchart_proc);
}

LRESULT CALLBACK _flowchart_edit_killfocus(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == NM_KILLFOCUS) {
		Ex_ObjShow(hObj, FALSE);
		size_t len = Ex_ObjGetTextLength(hObj);
		std::wstring text; text.resize(len);
		Ex_ObjGetText(hObj, text.data(), len * 2);
		auto hParent = Ex_ObjGetParent(hObj);
		Ex_ObjSetFocus(hParent);
		INT nodeId = Ex_ObjGetLong(hParent, FLOWCHART_LONG_DOUBLECLICK_NODEID);
		INT portId = Ex_ObjGetLong(hParent, FLOWCHART_LONG_DOUBLECLICK_PORTID);
		EX_FLOWCHART_PORT newData; newData.id = portId; newData.widgetType = FLOWCHART_NODEDATA_TYPE_EDIT; newData.widgetData = (LPVOID)text.data();
		Ex_ObjSendMessage(hParent, FLOWCHART_MESSAGE_UPDATE_NODEDATA, (WPARAM)nodeId, (LPARAM)&newData);
	}
	return 0;
}

LRESULT CALLBACK _flowchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_DATA));
		memset(pData, 0, sizeof(EX_FLOWCHART_DATA));
		pData->zoom = 1.0f; pData->selectedNode = -1; pData->draggingNode = -1;
		pData->connectingSlot = -1; pData->connectingNode = -1; pData->hoverNode = -1;
		pData->selectedConnection = -1; pData->selectedPortNode = -1; pData->selectedPortIndex = -1;
		pData->executionDepth = 0;
		pData->resizingNode = -1; pData->resizingPortIdx = -1;
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_DATA, (LONG_PTR)pData);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_X, 0); Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_Y, 0);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_BACKGROUNDCOLOR, ExARGB(80, 80, 80, 255));
		HEXOBJ edit = Ex_ObjCreateEx(OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, L"edit", 0,
			OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION | OBJECT_STYLE_VSCROLL | EDIT_STYLE_DISABLEMENU | EDIT_STYLE_NEWLINE,
			10, 30, 150, 30, hObj, 0, DT_LEFT | DT_TOP, 0, 0, 0);
		Ex_ObjHandleEvent(edit, NM_KILLFOCUS, _flowchart_edit_killfocus);
		Ex_ObjShow(edit, FALSE);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE, edit);
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_NODEID, -1); Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_PORTID, -1);
		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_HORZ, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 1000, 0, TRUE);
		Ex_ObjScrollSetInfo(hObj, SCROLLBAR_TYPE_VERT, SIF_PAGE | SIF_RANGE | SIF_POS, 0, 1, 1000, 0, TRUE);
		Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_HORZ, TRUE); Ex_ObjScrollShow(hObj, SCROLLBAR_TYPE_VERT, TRUE);
	}
	else if (uMsg == WM_DESTROY) {
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
		if (pData)
		{
			for (INT i = 0; i < pData->nodeCount; i++) {
				EX_FLOWCHART_NODE* node = &pData->nodes[i];
				Ex_MemFree((void*)node->title);
				for (INT j = 0; j < node->portCount; j++) {
					EX_FLOWCHART_PORT* port = &node->ports[j];
					Ex_MemFree((void*)port->name);
					if (port->widgetData != NULL) {
						if (port->widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
							Ex_MemFree(port->widgetData);
						}
						else if (port->widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
							EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)port->widgetData;
							for (INT k = 0; k < combo->count; k++) Ex_MemFree((void*)combo->options[k]);
							Ex_MemFree(combo->options);
							Ex_MemFree(combo);
						}
						else if (port->widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) {
							_img_destroy((HEXIMAGE)port->widgetData);
						}
					}
				}
				Ex_MemFree(node->ports);
			}
			Ex_MemFree(pData->nodes);
			Ex_MemFree(pData->connections);
			Ex_MemFree(pData);
		}
	}
	else if (uMsg == WM_PAINT) { _flowchart_paint(hObj); }
	else if (uMsg == WM_SIZE) { _flowchart_updatelayout(hObj); }
	else if (uMsg == WM_MOUSEMOVE) { _flowchart_onmousemove(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); }
	else if (uMsg == WM_LBUTTONDOWN) { _flowchart_onlbuttondown(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); }
	else if (uMsg == WM_LBUTTONUP) { _flowchart_onlbuttonup(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); }
	else if (uMsg == WM_LBUTTONDBLCLK) { _flowchart_ondoubleclick(hObj, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); }
	else if (uMsg == WM_MOUSEWHEEL) { if (GetAsyncKeyState(VK_CONTROL) & 32768) { _flowchart_onmousewheel(hObj, GET_WHEEL_DELTA_WPARAM(wParam)); return 0; } }
	else if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_DELETE) {
			EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
			if (pData) {
				if (pData->selectedConnection != -1) {
					_flowchart_removeconnection(hObj, pData->selectedConnection);
					pData->selectedConnection = -1;
				}
				else if (pData->selectedPortNode != -1 && pData->selectedPortIndex != -1) {
					EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->selectedPortNode);
					if (node && pData->selectedPortIndex < node->portCount && node->ports[pData->selectedPortIndex].isConnected) {
						for (INT i = pData->connectionCount - 1; i >= 0; i--) {
							EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
							if ((conn->fromNode == node->id && conn->fromSlot == pData->selectedPortIndex) || (conn->toNode == node->id && conn->toSlot == pData->selectedPortIndex)) {
								_flowchart_removeconnection(hObj, conn->id); break;
							}
						}
					}
					pData->selectedPortNode = -1; pData->selectedPortIndex = -1;
				}
				Ex_ObjInvalidateRect(hObj, 0); return 1;
			}
		}
	}
	else if (uMsg == WM_VSCROLL || uMsg == WM_HSCROLL) { _flowchart_onscrollbar(hWnd, hObj, uMsg, wParam, lParam); }
	else if (uMsg == FLOWCHART_MESSAGE_ADD_NODE) { return _flowchart_addnode(hObj, (EX_FLOWCHART_NODE*)lParam); }
	else if (uMsg == FLOWCHART_MESSAGE_REMOVE_NODE) { return _flowchart_removenode(hObj, (INT)wParam); }
	else if (uMsg == FLOWCHART_MESSAGE_ADD_CONNECTION) { return _flowchart_addconnection(hObj, (EX_FLOWCHART_CONNECTION*)lParam); }
	else if (uMsg == FLOWCHART_MESSAGE_REMOVE_CONNECTION) { return _flowchart_removeconnection(hObj, (INT)wParam); }
	else if (uMsg == FLOWCHART_MESSAGE_UPDATE_NODEDATA) {
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return 0;
		INT nodeId = (INT)wParam; EX_FLOWCHART_PORT* pNewData = (EX_FLOWCHART_PORT*)lParam; if (!pNewData) return 0;
		EX_FLOWCHART_NODE* pNode = _flowchart_findnode(pData, nodeId); if (!pNode) return 0;
		for (INT i = 0; i < pNode->portCount; i++) {
			if (pNode->ports[i].id == pNewData->id) {
				if (pNode->ports[i].widgetData) {
					if (pNode->ports[i].widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) Ex_MemFree(pNode->ports[i].widgetData);
					else if (pNode->ports[i].widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) _img_destroy((HEXIMAGE)pNode->ports[i].widgetData);
					else if (pNode->ports[i].widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
						EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)pNode->ports[i].widgetData;
						for (INT k = 0; k < combo->count; k++) Ex_MemFree((void*)combo->options[k]);
						Ex_MemFree(combo->options); Ex_MemFree(combo);
					}
				}
				pNode->ports[i].widgetType = pNewData->widgetType;
				pNode->ports[i].widgetData = pNewData->widgetType == FLOWCHART_NODEDATA_TYPE_EDIT ? (LPVOID)StrDupW((LPCWSTR)pNewData->widgetData) : pNewData->widgetData;
				_flowchart_calcnodesize(hObj, pNode); _flowchart_updatelayout(hObj); Ex_ObjInvalidateRect(hObj, 0);
			}
		}
		return 0;
	}
	else if (uMsg == FLOWCHART_MESSAGE_EXECUTE_NODE) {
		_flowchart_executenode(hObj, (INT)wParam);
		Ex_ObjInvalidateRect(hObj, 0); // 执行完后刷新界面
		return 1;
	}
	else if (uMsg == FLOWCHART_MESSAGE_EXPORT_YAML) {
		return _flowchart_export_to_yaml(hObj, (LPCWSTR)lParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_IMPORT_YAML) {
		return _flowchart_import_from_yaml(hObj, (LPCWSTR)lParam);
	}
	else if (uMsg == FLOWCHART_MESSAGE_SET_BACKGROUNDCOLOR)
	{
		Ex_ObjSetLong(hObj, FLOWCHART_LONG_BACKGROUNDCOLOR, lParam);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _flowchart_onscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
	BOOL bHScoll = uMsg == WM_HSCROLL;

	// 获取滚动条范围，最大<=1代表禁用，直接返回
	INT nMin, nMax;
	Ex_ObjScrollGetRange(hObj, bHScoll ? SCROLLBAR_TYPE_HORZ : SCROLLBAR_TYPE_VERT, &nMin, &nMax);
	if (nMax <= 1) return;

	INT nCode = LOWORD(wParam);
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	RECT rc; Ex_ObjGetClientRect(hObj, &rc);
	INT oPos = bHScoll ? pData->panOffset.x : pData->panOffset.y;
	INT nPos = 0, nPage = 1000, nLine = 200;

	if (nCode == SB_THUMBPOSITION) {
		HEXOBJ hSB = Ex_ObjScrollGetControl(hObj, bHScoll ? SCROLLBAR_TYPE_HORZ : SCROLLBAR_TYPE_VERT);
		nPos = Ex_ObjScrollGetTrackPos(hSB, SCROLLBAR_TYPE_CONTROL);
	}
	else {
		if (nCode == SB_PAGEUP) nPos = oPos - nPage;
		else if (nCode == SB_PAGEDOWN) nPos = oPos + nPage;
		else if (nCode == SB_LINEUP) nPos = oPos - nLine;
		else if (nCode == SB_LINEDOWN) nPos = oPos + nLine;
		else if (nCode == SB_TOP) nPos = 0;
		else return;
	}

	// 强制限制滚动位置在有效范围内
	nPos = __max(nMin, __min(nMax, nPos));
	if (nPos != oPos) {
		if (bHScoll) {
			pData->panOffset.x = nPos;
			Ex_ObjScrollSetPos(Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_HORZ), SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
		}
		else {
			pData->panOffset.y = nPos;
			Ex_ObjScrollSetPos(Ex_ObjScrollGetControl(hObj, SCROLLBAR_TYPE_VERT), SCROLLBAR_TYPE_CONTROL, nPos, TRUE);
		}
		Ex_ObjInvalidateRect(hObj, 0);
	}
}

void _flowchart_calcnodesize(HEXOBJ hObj, EX_FLOWCHART_NODE* node) {
	FLOAT titleHeight = 30.0f, width = 200.0f, currentY = titleHeight;
	INT inputCount = 0, outputCount = 0;
	for (INT i = 0; i < node->portCount; i++) {
		if (node->ports[i].portType == FLOWCHART_PORTTYPE_OUTPUT) outputCount++;
		else if (node->ports[i].portType == FLOWCHART_PORTTYPE_INPUT) inputCount++;
	}
	FLOAT slotAreaHeight = __max(inputCount, outputCount) * 25.0f;
	currentY += slotAreaHeight;

	// 提前计算最终的 width
	for (INT i = 0; i < node->portCount; i++) {
		auto& port = node->ports[i];
		BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
		if (port.widgetType != 0 && showWidget) {
			if (port.widgetWidth == 0) port.widgetWidth = 180.0f;
			width = __max(width, port.widgetWidth + 20.0f);
		}
	}

	// 使用最终的 width 计算 portRect
	INT inputIdx = 0, outputIdx = 0;
	for (INT i = 0; i < node->portCount; i++) {
		auto& port = node->ports[i];
		if (port.portType == FLOWCHART_PORTTYPE_OUTPUT) {
			port.portRect = { (LONG)(width - 15), (LONG)(titleHeight + outputIdx * 25.0f), (LONG)width, (LONG)(titleHeight + outputIdx * 25.0f + 15) };
			outputIdx++;
		}
		else if (port.portType == FLOWCHART_PORTTYPE_INPUT) {
			port.portRect = { 0, (LONG)(titleHeight + inputIdx * 25.0f), (LONG)15, (LONG)(titleHeight + inputIdx * 25.0f + 15) };
			inputIdx++;
		}
		else {
			port.portRect = { 0,0,0,0 };
		}
	}

	for (INT i = 0; i < node->portCount; i++) {
		auto& port = node->ports[i];
		BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
		if (port.widgetType != 0 && showWidget) {
			if (port.widgetHeight == 0) {
				if (port.widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) port.widgetHeight = 180.0f;
				else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) port.widgetHeight = 25.0f;
				else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) port.widgetHeight = 25.0f;
			}
			port.widgetRect = { 10, (LONG)currentY, (LONG)(10 + port.widgetWidth), (LONG)(currentY + port.widgetHeight) };
			currentY += port.widgetHeight + 5.0f;
		}
		else {
			port.widgetRect = { 0,0,0,0 };
		}
	}
	node->height = currentY + 10.0f;
	node->width = width;
}

INT _flowchart_addnode(HEXOBJ hObj, EX_FLOWCHART_NODE* pNode) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData || !pNode) return 0;
	_flowchart_calcnodesize(hObj, pNode); INT newCount = pData->nodeCount + 1;
	EX_FLOWCHART_NODE* newNodes = (EX_FLOWCHART_NODE*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE) * newCount);
	if (pData->nodeCount > 0) { memcpy(newNodes, pData->nodes, sizeof(EX_FLOWCHART_NODE) * pData->nodeCount); Ex_MemFree(pData->nodes); }
	pData->nodes = newNodes; EX_FLOWCHART_NODE* newNode = &newNodes[pData->nodeCount];
	newNode->id = pNode->id; newNode->x = pNode->x; newNode->y = pNode->y; newNode->width = pNode->width; newNode->height = pNode->height;
	newNode->title = StrDupW(pNode->title); newNode->portCount = pNode->portCount;
	newNode->ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(sizeof(EX_FLOWCHART_PORT) * pNode->portCount);
	for (INT i = 0; i < pNode->portCount; i++) {
		EX_FLOWCHART_PORT* src = &pNode->ports[i]; EX_FLOWCHART_PORT* dest = &newNode->ports[i];
		memcpy(dest, src, sizeof(EX_FLOWCHART_PORT)); dest->name = StrDupW(src->name); dest->isConnected = FALSE;
		if (src->widgetData) {
			if (src->widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
				dest->widgetData = (LPVOID)StrDupW((LPCWSTR)src->widgetData);
			}
			else if (src->widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
				// 深拷贝组合框数据
				EX_FLOWCHART_NODE_COMBO_DATA* srcCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)src->widgetData;
				EX_FLOWCHART_NODE_COMBO_DATA* destCombo = (EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
				destCombo->count = srcCombo->count;
				destCombo->current = srcCombo->current;
				destCombo->options = (LPCWSTR*)Ex_MemAlloc(sizeof(LPCWSTR) * srcCombo->count);
				for (INT k = 0; k < srcCombo->count; k++) {
					destCombo->options[k] = StrDupW(srcCombo->options[k]);
				}
				dest->widgetData = destCombo;
			}
			else {
				dest->widgetData = src->widgetData; // 如图片句柄等，直接转移
			}
		}
	}
	pData->nodeCount++; _flowchart_updatelayout(hObj); Ex_ObjInvalidateRect(hObj, 0); return 1;
}

INT _flowchart_addconnection(HEXOBJ hObj, EX_FLOWCHART_CONNECTION* pConn) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData || !pConn) return 0;
	for (INT i = 0; i < pData->connectionCount; i++) { if (pData->connections[i].toNode == pConn->toNode && pData->connections[i].toSlot == pConn->toSlot) { _flowchart_removeconnection(hObj, pData->connections[i].id); break; } }
	INT newCount = pData->connectionCount + 1; EX_FLOWCHART_CONNECTION* newConns = (EX_FLOWCHART_CONNECTION*)Ex_MemAlloc(sizeof(EX_FLOWCHART_CONNECTION) * newCount);
	if (pData->connectionCount > 0) { memcpy(newConns, pData->connections, sizeof(EX_FLOWCHART_CONNECTION) * pData->connectionCount); Ex_MemFree(pData->connections); }
	pData->connections = newConns; EX_FLOWCHART_CONNECTION* newConn = &newConns[pData->connectionCount];
	newConn->id = (pConn->id != 0) ? pConn->id : (INT)GetTickCount64() + pData->connectionCount; newConn->fromNode = pConn->fromNode; newConn->fromSlot = pConn->fromSlot; newConn->toNode = pConn->toNode; newConn->toSlot = pConn->toSlot;
	EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, newConn->toNode);
	if (toNode && newConn->toSlot < toNode->portCount) { toNode->ports[newConn->toSlot].isConnected = TRUE; _flowchart_calcnodesize(hObj, toNode); }
	pData->connectionCount++;
	EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, newConn->fromNode);
	if (fromNode && toNode) {
		POINTF fromPt = { fromNode->x + (fromNode->ports[newConn->fromSlot].portRect.left + fromNode->ports[newConn->fromSlot].portRect.right) / 2.0f, fromNode->y + (fromNode->ports[newConn->fromSlot].portRect.top + fromNode->ports[newConn->fromSlot].portRect.bottom) / 2.0f };
		POINTF toPt = { toNode->x + (toNode->ports[newConn->toSlot].portRect.left + toNode->ports[newConn->toSlot].portRect.right) / 2.0f, toNode->y + (toNode->ports[newConn->toSlot].portRect.top + toNode->ports[newConn->toSlot].portRect.bottom) / 2.0f };
		_flowchart_initcontrolpoint(newConn, fromPt, toPt);
	}
	Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_CREATED, newConn->id, 0); Ex_ObjInvalidateRect(hObj, 0); return 1;
}

INT _flowchart_removeconnection(HEXOBJ hObj, INT connId) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData || pData->connectionCount == 0) return 0;
	INT index = -1; for (INT i = 0; i < pData->connectionCount; i++) { if (pData->connections[i].id == connId) { index = i; break; } }
	if (index == -1) return 0;
	EX_FLOWCHART_CONNECTION* conn = &pData->connections[index];
	EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, conn->toNode);
	if (toNode && conn->toSlot < toNode->portCount) { toNode->ports[conn->toSlot].isConnected = FALSE; _flowchart_calcnodesize(hObj, toNode); }
	if (pData->connectionCount > 1) {
		EX_FLOWCHART_CONNECTION* newConns = (EX_FLOWCHART_CONNECTION*)Ex_MemAlloc(sizeof(EX_FLOWCHART_CONNECTION) * (pData->connectionCount - 1));
		if (index > 0) memcpy(newConns, pData->connections, sizeof(EX_FLOWCHART_CONNECTION) * index);
		if (index < pData->connectionCount - 1) memcpy(newConns + index, pData->connections + index + 1, sizeof(EX_FLOWCHART_CONNECTION) * (pData->connectionCount - index - 1));
		Ex_MemFree(pData->connections); pData->connections = newConns;
	}
	else { Ex_MemFree(pData->connections); pData->connections = NULL; }
	pData->connectionCount--; if (pData->selectedConnection == connId) pData->selectedConnection = -1;
	Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_REMOVED, connId, 0); Ex_ObjInvalidateRect(hObj, 0); return 1;
}

void _flowchart_updatelayout(HEXOBJ hObj) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return;

	// 获取流程图组件的客户区（可视区域）大小
	RECT rcClient;
	Ex_ObjGetClientRect(hObj, &rcClient);
	INT clientWidth = rcClient.right - rcClient.left;
	INT clientHeight = rcClient.bottom - rcClient.top;

	FLOAT maxX = 0.0f, maxY = 0.0f;
	// 遍历所有节点，计算内容的最大右边界、下边界
	for (INT i = 0; i < pData->nodeCount; i++) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		FLOAT nodeRight = node->x + node->width;
		FLOAT nodeBottom = node->y + node->height;
		if (nodeRight > maxX) maxX = nodeRight;
		if (nodeBottom > maxY) maxY = nodeBottom;
	}

	// 计算缩放后的内容总尺寸
	FLOAT scaledContentW = maxX * pData->zoom;
	FLOAT scaledContentH = maxY * pData->zoom;

	// ==================== 水平滚动条逻辑 ====================
	INT hScrollMax = 1; // 默认范围0~1 = 禁用滚动条
	if (scaledContentW > clientWidth) {
		// 内容超出可视区，设置实际滚动范围
		hScrollMax = (INT)scaledContentW + 50;
		// 限制偏移不超出范围
		pData->panOffset.x = __min(pData->panOffset.x, hScrollMax);
	}
	else {
		// 内容未超出，重置偏移和滚动条位置
		pData->panOffset.x = 0;
		Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_HORZ, 0, TRUE);
	}
	Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_HORZ, 0, hScrollMax, TRUE);

	// ==================== 垂直滚动条逻辑 ====================
	INT vScrollMax = 1; // 默认范围0~1 = 禁用滚动条
	if (scaledContentH > clientHeight) {
		// 内容超出可视区，设置实际滚动范围
		vScrollMax = (INT)scaledContentH + 50;
		pData->panOffset.y = __min(pData->panOffset.y, vScrollMax);
	}
	else {
		// 内容未超出，重置偏移和滚动条位置
		pData->panOffset.y = 0;
		Ex_ObjScrollSetPos(hObj, SCROLLBAR_TYPE_VERT, 0, TRUE);
	}
	Ex_ObjScrollSetRange(hObj, SCROLLBAR_TYPE_VERT, 0, vScrollMax, TRUE);
}

void _flowchart_paint(HEXOBJ hObj) {
	EX_PAINTSTRUCT ps; if (Ex_ObjBeginPaint(hObj, &ps)) {
		EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) { Ex_ObjEndPaint(hObj, &ps); return; }
		INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ); INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
		_canvas_clear(ps.hCanvas, Ex_ObjGetLong(hObj, FLOWCHART_LONG_BACKGROUNDCOLOR));
		FLOAT gridSpacing = 20.0f * pData->zoom; if (gridSpacing < 5.0f) gridSpacing = 5.0f;
		HEXBRUSH hBrushGrid = _brush_create(ExARGB(29, 29, 29, 255));
		for (FLOAT x = fmodf(-scrollX, gridSpacing); x < ps.rcPaint.right; x += gridSpacing) _canvas_drawline(ps.hCanvas, hBrushGrid, x, 0, x, (FLOAT)ps.rcPaint.bottom, 1, 0);
		for (FLOAT y = fmodf(-scrollY, gridSpacing); y < ps.rcPaint.bottom; y += gridSpacing) _canvas_drawline(ps.hCanvas, hBrushGrid, 0, y, (FLOAT)ps.rcPaint.right, y, 1, 0);
		_brush_destroy(hBrushGrid);

		// ==================== 第一步：绘制【未选中】的连线 ====================
		for (INT i = 0; i < pData->connectionCount; i++) {
			EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
			BOOL isSelected = (pData->selectedConnection == conn->id);
			if (isSelected) continue; // 跳过选中连线，最后绘制

			EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, conn->fromNode);
			EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, conn->toNode);
			if (!fromNode || !toNode) continue;

			POINTF fromPtCenter = _flowchart_get_port_center(fromNode, conn->fromSlot);
			POINTF toPtCenter = _flowchart_get_port_center(toNode, conn->toSlot);
			POINTF fromPt, toPt;
			fromPt.x = fromPtCenter.x * pData->zoom - scrollX; fromPt.y = fromPtCenter.y * pData->zoom - scrollY;
			toPt.x = toPtCenter.x * pData->zoom - scrollX; toPt.y = toPtCenter.y * pData->zoom - scrollY;
			EXARGB lineColor = _flowchart_get_port_color(fromNode->ports[conn->fromSlot].dataType);
			HEXBRUSH hBrushLine = _brush_create(lineColor);
			POINTF curvePoints[4] = { {fromPt.x, fromPt.y}, {conn->controlPoint1.x * pData->zoom - scrollX, conn->controlPoint1.y * pData->zoom - scrollY}, {conn->controlPoint2.x * pData->zoom - scrollX, conn->controlPoint2.y * pData->zoom - scrollY}, {toPt.x, toPt.y} };
			_canvas_drawcurve(ps.hCanvas, hBrushLine, curvePoints, 4, 0.5f, 2.0f, 0);
			_brush_destroy(hBrushLine);
		}

		// ==================== 第二步：绘制所有节点（未选中 → 选中，节点内部层级） ====================
		for (INT i = 0; i < pData->nodeCount; i++) {
			if (pData->selectedNode != pData->nodes[i].id) {
				_flowchart_drawnode(ps.hCanvas, pData, &pData->nodes[i], FALSE, pData->zoom, scrollX, scrollY);
			}
		}
		EX_FLOWCHART_NODE* selectedNode = _flowchart_findnode(pData, pData->selectedNode);
		if (selectedNode) {
			_flowchart_drawnode(ps.hCanvas, pData, selectedNode, TRUE, pData->zoom, scrollX, scrollY);
		}

		// ==================== 第三步：绘制【选中】的连线（最顶层，高于所有节点） ====================
		for (INT i = 0; i < pData->connectionCount; i++) {
			EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
			BOOL isSelected = (pData->selectedConnection == conn->id);
			if (!isSelected) continue; // 只绘制选中的连线

			EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, conn->fromNode);
			EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, conn->toNode);
			if (!fromNode || !toNode) continue;

			POINTF fromPtCenter = _flowchart_get_port_center(fromNode, conn->fromSlot);
			POINTF toPtCenter = _flowchart_get_port_center(toNode, conn->toSlot);
			POINTF fromPt, toPt;
			fromPt.x = fromPtCenter.x * pData->zoom - scrollX; fromPt.y = fromPtCenter.y * pData->zoom - scrollY;
			toPt.x = toPtCenter.x * pData->zoom - scrollX; toPt.y = toPtCenter.y * pData->zoom - scrollY;
			EXARGB lineColor = _flowchart_get_port_color(fromNode->ports[conn->fromSlot].dataType);
			HEXBRUSH hBrushLine = _brush_create(ExARGB(255, 165, 0, 255)); // 选中橙色
			POINTF curvePoints[4] = { {fromPt.x, fromPt.y}, {conn->controlPoint1.x * pData->zoom - scrollX, conn->controlPoint1.y * pData->zoom - scrollY}, {conn->controlPoint2.x * pData->zoom - scrollX, conn->controlPoint2.y * pData->zoom - scrollY}, {toPt.x, toPt.y} };
			_canvas_drawcurve(ps.hCanvas, hBrushLine, curvePoints, 4, 1.0f, 3.0f, 0);

			// 绘制选中连线的控制点
			HEXBRUSH hBrushCtrl = _brush_create(ExARGB(255, 255, 255, 255));
			POINTF cp1 = { conn->controlPoint1.x * pData->zoom - scrollX, conn->controlPoint1.y * pData->zoom - scrollY };
			POINTF cp2 = { conn->controlPoint2.x * pData->zoom - scrollX, conn->controlPoint2.y * pData->zoom - scrollY };
			_canvas_fillellipse(ps.hCanvas, hBrushCtrl, cp1.x, cp1.y, 4.0f * pData->zoom, 4.0f * pData->zoom);
			_canvas_fillellipse(ps.hCanvas, hBrushCtrl, cp2.x, cp2.y, 4.0f * pData->zoom, 4.0f * pData->zoom);
			_brush_destroy(hBrushCtrl);
			_brush_destroy(hBrushLine);
		}

		// ==================== 第四步：绘制拖拽中的临时连线 ====================
		if (pData->connectingSlot != -1 && pData->connectingNode != -1) {
			EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->connectingNode); if (node) {
				POINTF fromPt, toPt; FLOAT mouseX = (FLOAT)Ex_ObjGetLong(hObj, FLOWCHART_LONG_MOUSE_X); FLOAT mouseY = (FLOAT)Ex_ObjGetLong(hObj, FLOWCHART_LONG_MOUSE_Y);
				EX_FLOWCHART_PORT& port = node->ports[pData->connectingSlot];
				if (pData->connectingSlotType == FLOWCHART_SLOTTYPE_OUTPUT) {
					fromPt.x = (node->x + (port.portRect.left + port.portRect.right) / 2.0f) * pData->zoom - scrollX; fromPt.y = (node->y + (port.portRect.top + port.portRect.bottom) / 2.0f) * pData->zoom - scrollY;
					toPt.x = mouseX; toPt.y = mouseY;
				}
				else { toPt.x = (node->x + (port.portRect.left + port.portRect.right) / 2.0f) * pData->zoom - scrollX; toPt.y = (node->y + (port.portRect.top + port.portRect.bottom) / 2.0f) * pData->zoom - scrollY; fromPt.x = mouseX; fromPt.y = mouseY; }
				HEXBRUSH hBrushTemp = _brush_create(_flowchart_get_port_color(port.dataType));
				_canvas_drawline(ps.hCanvas, hBrushTemp, fromPt.x, fromPt.y, toPt.x, toPt.y, 2.0f, D2D1_DASH_STYLE_DASH);
				_brush_destroy(hBrushTemp);
			}
		}

		Ex_ObjEndPaint(hObj, &ps);
	}
}

void _flowchart_drawnode(HEXCANVAS hCanvas, EX_FLOWCHART_DATA* pData, EX_FLOWCHART_NODE* node, BOOL selected, FLOAT zoom, INT scrollX, INT scrollY) {
	FLOAT nodeX = node->x * zoom - scrollX; FLOAT nodeY = node->y * zoom - scrollY; FLOAT nodeWidth = node->width * zoom; FLOAT nodeHeight = node->height * zoom;
	HEXBRUSH hBrushNode = _brush_create(ExARGB(53, 53, 53, 255)); HEXBRUSH hBrushBorder = _brush_create(selected ? ExARGB(255, 255, 255, 255) : ExRGB2ARGB(0, 255));
	_canvas_fillroundedrect(hCanvas, hBrushNode, nodeX, nodeY, nodeX + nodeWidth, nodeY + nodeHeight, 5.0f * zoom, 5.0f * zoom);
	_canvas_drawroundedrect(hCanvas, hBrushBorder, nodeX, nodeY, nodeX + nodeWidth, nodeY + nodeHeight, 5.0f * zoom, 5.0f * zoom, 2.0f * zoom, 0);
	FLOAT titleHeight = 30.0f * zoom; EX_RECTF rcTitle = { nodeX + 2.0f * zoom, nodeY + 2.0f * zoom, nodeX + nodeWidth - 2.0f * zoom, nodeY + titleHeight - 2.0f * zoom };
	HEXBRUSH hBrushTitle = _brush_create(ExARGB(34, 34, 34, 255)); _canvas_fillrect(hCanvas, hBrushTitle, rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom);
	HEXFONT hFontTitle = _font_createfromfamily(L"Arial", 12 * zoom, FONT_STYLE_BOLD); _canvas_drawtext(hCanvas, hFontTitle, ExARGB(217, 217, 217, 255), node->title, -1, DT_CENTER | DT_VCENTER, rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom); _font_destroy(hFontTitle);

	HEXFONT hFontSlot = _font_createfromfamily(L"Arial", 10 * zoom, 0);
	for (INT i = 0; i < node->portCount; i++) {
		EX_FLOWCHART_PORT& port = node->ports[i];
		if (port.portType != FLOWCHART_PORTTYPE_INTERMEDIATE) { // 中间端口不画圆点和名称
			FLOAT cx = nodeX + (port.portRect.left + port.portRect.right) / 2.0f * zoom; FLOAT cy = nodeY + (port.portRect.top + port.portRect.bottom) / 2.0f * zoom;
			EXARGB portColor = _flowchart_get_port_color(port.dataType);
			BOOL isHover = (pData->hoverNode == node->id && pData->hoverSlot == i && pData->hoverSlotType == (port.portType == FLOWCHART_PORTTYPE_OUTPUT ? FLOWCHART_SLOTTYPE_OUTPUT : FLOWCHART_SLOTTYPE_INPUT));
			BOOL isSelectedPort = (pData->selectedPortNode == node->id && pData->selectedPortIndex == i);
			HEXBRUSH hBrushPort = _brush_create(isSelectedPort ? ExARGB(255, 255, 100, 255) : (isHover ? ExARGB(255, 255, 255, 255) : portColor));
			_canvas_fillellipse(hCanvas, hBrushPort, cx, cy, 6.0f * zoom, 6.0f * zoom); _canvas_drawellipse(hCanvas, hBrushBorder, cx, cy, 6.0f * zoom, 6.0f * zoom, 1.0f * zoom, 0);
			if (port.portType == FLOWCHART_PORTTYPE_OUTPUT) _canvas_drawtext(hCanvas, hFontSlot, ExARGB(217, 217, 217, 255), port.name, -1, DT_RIGHT | DT_VCENTER, nodeX, cy - 8.0f * zoom, cx - 8.0f * zoom, cy + 8.0f * zoom);
			else _canvas_drawtext(hCanvas, hFontSlot, ExARGB(217, 217, 217, 255), port.name, -1, DT_LEFT | DT_VCENTER, cx + 8.0f * zoom, cy - 8.0f * zoom, nodeX + nodeWidth, cy + 8.0f * zoom);
			_brush_destroy(hBrushPort);
		}

		BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
		if (port.widgetType != 0 && showWidget) {
			FLOAT wx = nodeX + port.widgetRect.left * zoom; FLOAT wy = nodeY + port.widgetRect.top * zoom; FLOAT ww = (port.widgetRect.right - port.widgetRect.left) * zoom; FLOAT wh = (port.widgetRect.bottom - port.widgetRect.top) * zoom;
			if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
				HEXBRUSH hBrushEdit = _brush_create(ExARGB(34, 34, 34, 255)); _canvas_fillrect(hCanvas, hBrushEdit, wx, wy, wx + ww, wy + wh); _canvas_drawrect(hCanvas, hBrushBorder, wx, wy, wx + ww, wy + wh, 1.0f * zoom, 0);
				if (port.widgetData) { HEXFONT hFont = _font_createfromfamily(L"Arial", 14 * zoom, 0); _canvas_drawtext(hCanvas, hFont, ExARGB(217, 217, 217, 255), (LPCWSTR)port.widgetData, -1, DT_LEFT | DT_TOP | DT_WORDBREAK, wx + 5.0f * zoom, wy + 5.0f * zoom, wx + ww - 5.0f * zoom, wy + wh - 5.0f * zoom); _font_destroy(hFont); }
				_brush_destroy(hBrushEdit);
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) {
				if (port.widgetData) { HEXIMAGE hImage = (HEXIMAGE)port.widgetData; INT imgW, imgH; _img_getsize(hImage, &imgW, &imgH); _canvas_drawimagerectrect(hCanvas, hImage, wx, wy, wx + ww, wy + wh, 0, 0, imgW, imgH, 255); }
				else { HEXBRUSH hBrushImg = _brush_create(ExARGB(20, 20, 20, 255)); _canvas_fillrect(hCanvas, hBrushImg, wx, wy, wx + ww, wy + wh); _brush_destroy(hBrushImg); }
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
				HEXBRUSH hBrushCombo = _brush_create(ExARGB(34, 34, 34, 255)); _canvas_fillrect(hCanvas, hBrushCombo, wx, wy, wx + ww, wy + wh); _canvas_drawrect(hCanvas, hBrushBorder, wx, wy, wx + ww, wy + wh, 1.0f * zoom, 0);
				EX_FLOWCHART_NODE_COMBO_DATA* comboData = (EX_FLOWCHART_NODE_COMBO_DATA*)port.widgetData; FLOAT arrowHeight = wh * 0.6f;
				_flowchart_draw_triangle_arrow(hCanvas, hBrushBorder, wx + 5 * zoom, wy + (wh - arrowHeight) / 2, arrowHeight, TRUE); _flowchart_draw_triangle_arrow(hCanvas, hBrushBorder, wx + ww - arrowHeight - 5 * zoom, wy + (wh - arrowHeight) / 2, arrowHeight, FALSE);
				if (comboData && comboData->count > 0) { HEXFONT hFont = _font_createfromfamily(L"Arial", 10 * zoom, 0); _canvas_drawtext(hCanvas, hFont, ExARGB(217, 217, 217, 255), comboData->options[comboData->current], -1, DT_CENTER | DT_VCENTER, wx + arrowHeight + 10 * zoom, wy, wx + ww - arrowHeight - 10 * zoom, wy + wh); _font_destroy(hFont); }
				_brush_destroy(hBrushCombo);
			}
			// 绘制右下角调整大小手柄
			HEXBRUSH hBrushHandle = _brush_create(ExARGB(150, 150, 150, 255));
			POINTF p1 = { wx + ww, wy + wh - 6 * zoom }, p2 = { wx + ww, wy + wh }, p3 = { wx + ww - 6 * zoom, wy + wh };
			HEXPATH hPath; _path_create(PATH_FLAG_DISABLESCALE, &hPath); _path_open(hPath); _path_beginfigure2(hPath, p1.x, p1.y);
			_path_addline(hPath, p1.x, p1.y, p2.x, p2.y); _path_addline(hPath, p2.x, p2.y, p3.x, p3.y);
			_path_endfigure(hPath, TRUE); _path_close(hPath); _canvas_fillpath(hCanvas, hPath, hBrushHandle); _path_destroy(hPath); _brush_destroy(hBrushHandle);
		}
	}
	_font_destroy(hFontSlot); _brush_destroy(hBrushNode); _brush_destroy(hBrushBorder); _brush_destroy(hBrushTitle);
}

void _flowchart_onmousemove(HEXOBJ hObj, INT x, INT y) {
	Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_X, x); Ex_ObjSetLong(hObj, FLOWCHART_LONG_MOUSE_Y, y);
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ); INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
	FLOAT virtualX = (x + scrollX) / pData->zoom; FLOAT virtualY = (y + scrollY) / pData->zoom;

	// 1. 处理组件大小调整
	if (pData->resizingNode != -1) {
		EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->resizingNode);
		if (node) {
			auto& port = node->ports[pData->resizingPortIdx];
			FLOAT dx = virtualX - pData->dragStartPos.x;
			FLOAT dy = virtualY - pData->dragStartPos.y;
			FLOAT newW = port.widgetWidth + dx;
			FLOAT newH = port.widgetHeight + dy;

			// 根据组件类型应用最小尺寸限制
			if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
				newW = __max(180.0f, newW);
				newH = __max(25.0f, newH);
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) {
				newW = __max(180.0f, newW);
				newH = __max(180.0f, newH);
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
				newW = __max(180.0f, newW);
				newH = 25.0f; // 组合框高度固定
			}

			port.widgetWidth = newW;
			port.widgetHeight = newH;
			pData->dragStartPos = { virtualX, virtualY };

			// 重新计算节点和布局，使卡片大小自适应
			_flowchart_calcnodesize(hObj, node);
			_flowchart_updatelayout(hObj);
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return;
	}
	else if (pData->draggingControlPoint && pData->selectedConnection != -1) {
		EX_FLOWCHART_CONNECTION* conn = _flowchart_findconnection(pData, pData->selectedConnection);
		if (conn) {
			FLOAT deltaX = virtualX - pData->dragStartPos.x, deltaY = virtualY - pData->dragStartPos.y;
			if (pData->draggingWhichPoint == 1) { conn->controlPoint1.x += deltaX; conn->controlPoint1.y += deltaY; }
			else if (pData->draggingWhichPoint == 2) { conn->controlPoint2.x += deltaX; conn->controlPoint2.y += deltaY; }
			pData->dragStartPos.x = virtualX; pData->dragStartPos.y = virtualY;
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (pData->draggingNode != -1) {
		EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, pData->draggingNode);
		if (node) {
			HEXOBJ edit = Ex_ObjGetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE); Ex_ObjShow(edit, FALSE);
			FLOAT dx = virtualX - pData->dragStartPos.x; FLOAT dy = virtualY - pData->dragStartPos.y;
			node->x += dx; node->y += dy;

			// ============== 限制节点坐标最小为0 ==============
			node->x = __max(0.0f, node->x);
			node->y = __max(0.0f, node->y);

			for (INT i = 0; i < pData->connectionCount; i++) {
				EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
				if (conn->fromNode == node->id) { conn->controlPoint1.x += dx; conn->controlPoint1.y += dy; }
				if (conn->toNode == node->id) { conn->controlPoint2.x += dx; conn->controlPoint2.y += dy; }
			}
			pData->dragStartPos.x = virtualX; pData->dragStartPos.y = virtualY;
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODE_MOVED, node->id, 0);
			_flowchart_updatelayout(hObj); // 实时更新滚动条范围
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (pData->connectingSlot != -1) {
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else {
		INT prevHoverNode = pData->hoverNode, prevHoverSlot = pData->hoverSlot, prevHoverSlotType = pData->hoverSlotType;
		pData->hoverNode = -1; pData->hoverSlot = -1; pData->hoverSlotType = -1;
		for (INT i = 0; i < pData->nodeCount; i++) {
			EX_FLOWCHART_NODE* node = &pData->nodes[i];
			for (INT j = 0; j < node->portCount; j++) {
				if (node->ports[j].portType == FLOWCHART_PORTTYPE_INTERMEDIATE) continue; // 中间端口无圆点，不参与连线悬停
				RECT rc = node->ports[j].portRect;
				if (virtualX >= node->x + rc.left && virtualX <= node->x + rc.right && virtualY >= node->y + rc.top && virtualY <= node->y + rc.bottom) {
					pData->hoverNode = node->id; pData->hoverSlot = j; pData->hoverSlotType = node->ports[j].portType == FLOWCHART_PORTTYPE_OUTPUT ? FLOWCHART_SLOTTYPE_OUTPUT : FLOWCHART_SLOTTYPE_INPUT; break;
				}
			}
			if (pData->hoverNode != -1) break;
		}
		if (prevHoverNode != pData->hoverNode || prevHoverSlot != pData->hoverSlot || prevHoverSlotType != pData->hoverSlotType) Ex_ObjInvalidateRect(hObj, 0);
	}
}

void _flowchart_onlbuttondown(HEXOBJ hObj, INT x, INT y) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData || pData->draggingControlPoint) return;
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ); INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
	FLOAT virtualX = (x + scrollX) / pData->zoom; FLOAT virtualY = (y + scrollY) / pData->zoom;
	pData->selectedNode = -1; pData->selectedPortNode = -1; pData->selectedPortIndex = -1;

	// 判断是否按住Shift键，按住时优先检测端口，允许从已连接的输出端口拉出新的连线
	BOOL shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	if (shiftPressed) {
		for (INT i = pData->nodeCount - 1; i >= 0; i--) {
			EX_FLOWCHART_NODE* node = &pData->nodes[i];
			for (INT j = 0; j < node->portCount; j++) {
				if (node->ports[j].portType == FLOWCHART_PORTTYPE_INTERMEDIATE) continue;
				RECT rc = node->ports[j].portRect;
				if (virtualX >= node->x + rc.left && virtualX <= node->x + rc.right && virtualY >= node->y + rc.top && virtualY <= node->y + rc.bottom) {
					pData->selectedPortNode = node->id; pData->selectedPortIndex = j; pData->selectedConnection = -1;
					pData->connectingSlot = j; pData->connectingNode = node->id; pData->connectingSlotType = node->ports[j].portType == FLOWCHART_PORTTYPE_OUTPUT ? FLOWCHART_SLOTTYPE_OUTPUT : FLOWCHART_SLOTTYPE_INPUT;
					Ex_ObjInvalidateRect(hObj, 0); return;
				}
			}
		}
	}

	// 1. 优先检测组件右下角调整大小手柄
	for (INT i = pData->nodeCount - 1; i >= 0; i--) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		for (INT j = 0; j < node->portCount; j++) {
			auto& port = node->ports[j];
			BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
			if (port.widgetType != 0 && showWidget) {
				FLOAT right = node->x + port.widgetRect.right;
				FLOAT bottom = node->y + port.widgetRect.bottom;
				// 检测右下角 8x10 的矩形热区
				if (virtualX >= right - 8 && virtualX <= right + 2 && virtualY >= bottom - 10 && virtualY <= bottom + 2) {
					pData->resizingNode = node->id;
					pData->resizingPortIdx = j;
					pData->dragStartPos = { virtualX, virtualY };
					Ex_ObjInvalidateRect(hObj, 0);
					return;
				}
			}
		}
	}

	// 2. 检测控制点拖拽
	for (INT i = 0; i < pData->connectionCount; i++) {
		EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
		FLOAT distCtrl1 = sqrtf(powf(virtualX - conn->controlPoint1.x, 2) + powf(virtualY - conn->controlPoint1.y, 2));
		FLOAT distCtrl2 = sqrtf(powf(virtualX - conn->controlPoint2.x, 2) + powf(virtualY - conn->controlPoint2.y, 2));
		if (distCtrl1 <= 8.0f || distCtrl2 <= 8.0f) {
			pData->selectedConnection = conn->id; pData->draggingControlPoint = TRUE; pData->dragStartPos = { virtualX, virtualY }; pData->draggingWhichPoint = distCtrl1 <= 8.0f ? 1 : 2;
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_SELECTED, conn->id, 0); Ex_ObjInvalidateRect(hObj, 0); return;
		}
	}

	// 3. 检测连线选中
	for (INT i = 0; i < pData->connectionCount; i++) {
		EX_FLOWCHART_CONNECTION* conn = &pData->connections[i];
		EX_FLOWCHART_NODE* fromNode = _flowchart_findnode(pData, conn->fromNode); EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, conn->toNode); if (!fromNode || !toNode) continue;
		POINTF v1 = { fromNode->x + (fromNode->ports[conn->fromSlot].portRect.left + fromNode->ports[conn->fromSlot].portRect.right) / 2.0f, fromNode->y + (fromNode->ports[conn->fromSlot].portRect.top + fromNode->ports[conn->fromSlot].portRect.bottom) / 2.0f };
		POINTF v2 = conn->controlPoint1; POINTF v3 = conn->controlPoint2;
		POINTF v4 = { toNode->x + (toNode->ports[conn->toSlot].portRect.left + toNode->ports[conn->toSlot].portRect.right) / 2.0f, toNode->y + (toNode->ports[conn->toSlot].portRect.top + toNode->ports[conn->toSlot].portRect.bottom) / 2.0f };
		POINTF p = { virtualX, virtualY };
		if (_flowchart_dist_to_segment(p, v1, v2) < 5.0f || _flowchart_dist_to_segment(p, v2, v3) < 5.0f || _flowchart_dist_to_segment(p, v3, v4) < 5.0f) {
			pData->selectedConnection = conn->id; Ex_ObjInvalidateRect(hObj, 0); return;
		}
	}

	// 4. 检测端口圆点 (排除中间端口)
	for (INT i = pData->nodeCount - 1; i >= 0; i--) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		for (INT j = 0; j < node->portCount; j++) {
			if (node->ports[j].portType == FLOWCHART_PORTTYPE_INTERMEDIATE) continue;
			RECT rc = node->ports[j].portRect;
			if (virtualX >= node->x + rc.left && virtualX <= node->x + rc.right && virtualY >= node->y + rc.top && virtualY <= node->y + rc.bottom) {
				pData->selectedPortNode = node->id; pData->selectedPortIndex = j; pData->selectedConnection = -1;
				pData->connectingSlot = j; pData->connectingNode = node->id; pData->connectingSlotType = node->ports[j].portType == FLOWCHART_PORTTYPE_OUTPUT ? FLOWCHART_SLOTTYPE_OUTPUT : FLOWCHART_SLOTTYPE_INPUT;
				Ex_ObjInvalidateRect(hObj, 0); return;
			}
		}
	}

	// 5. 检测子组件交互 (组合框左右箭头)
	for (INT i = pData->nodeCount - 1; i >= 0; i--) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		for (INT j = 0; j < node->portCount; j++) {
			EX_FLOWCHART_PORT& port = node->ports[j];
			BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
			if (showWidget && port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO && port.widgetData != NULL) {
				if (virtualX >= node->x + port.widgetRect.left && virtualX <= node->x + port.widgetRect.right && virtualY >= node->y + port.widgetRect.top && virtualY <= node->y + port.widgetRect.bottom) {
					EX_FLOWCHART_NODE_COMBO_DATA* comboData = (EX_FLOWCHART_NODE_COMBO_DATA*)port.widgetData; FLOAT wh = port.widgetRect.bottom - port.widgetRect.top; FLOAT arrowHeight = wh * 0.6f;
					FLOAT leftArrowX = node->x + port.widgetRect.left + 5; FLOAT rightArrowX = node->x + port.widgetRect.right - arrowHeight - 5;
					if (virtualX <= leftArrowX + arrowHeight) { comboData->current = (comboData->current - 1 + comboData->count) % comboData->count; Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODEDATA_COMBO_CHANGED, node->id, (LPARAM)&port); Ex_ObjInvalidateRect(hObj, 0); return; }
					else if (virtualX >= rightArrowX) { comboData->current = (comboData->current + 1) % comboData->count; Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODEDATA_COMBO_CHANGED, node->id, (LPARAM)&port); Ex_ObjInvalidateRect(hObj, 0); return; }
				}
			}
		}
	}

	// 6. 检测节点拖拽
	for (INT i = pData->nodeCount - 1; i >= 0; i--) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		if (virtualX >= node->x && virtualX <= node->x + node->width && virtualY >= node->y && virtualY <= node->y + node->height) {
			pData->selectedNode = node->id; pData->draggingNode = node->id; pData->dragStartPos = { virtualX, virtualY }; pData->selectedConnection = -1;
			Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_NODE_CLICKED, node->id, 0); Ex_ObjInvalidateRect(hObj, 0); return;
		}
	}
}

void _flowchart_onlbuttonup(HEXOBJ hObj, INT x, INT y) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;

	// 取消组件大小调整
	if (pData->resizingNode != -1) {
		pData->resizingNode = -1;
		pData->resizingPortIdx = -1;
		Ex_ObjInvalidateRect(hObj, 0);
		return;
	}

	if (pData->draggingControlPoint) {
		pData->draggingControlPoint = FALSE; pData->draggingWhichPoint = 0;
		if (pData->selectedConnection != -1) Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_CONNECTION_MOVED, pData->selectedConnection, 0);
	}

	if (pData->connectingSlot != -1 && pData->connectingNode != -1) {
		INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ); INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
		FLOAT virtualX = (x + scrollX) / pData->zoom; FLOAT virtualY = (y + scrollY) / pData->zoom;
		for (INT i = 0; i < pData->nodeCount; i++) {
			EX_FLOWCHART_NODE* node = &pData->nodes[i]; if (node->id == pData->connectingNode) continue;
			for (INT j = 0; j < node->portCount; j++) {
				if (node->ports[j].portType == FLOWCHART_PORTTYPE_INTERMEDIATE) continue; // 禁止连接到中间端口
				RECT rc = node->ports[j].portRect;
				if (virtualX >= node->x + rc.left && virtualX <= node->x + rc.right && virtualY >= node->y + rc.top && virtualY <= node->y + rc.bottom) {
					EX_FLOWCHART_NODE* srcNode = _flowchart_findnode(pData, pData->connectingNode); EX_FLOWCHART_PORT* srcPort = &srcNode->ports[pData->connectingSlot]; EX_FLOWCHART_PORT* destPort = &node->ports[j];
					// 确保连接的是一个输入一个输出
					if (srcPort->portType != destPort->portType && (srcPort->dataType == destPort->dataType || destPort->dataType == FLOWCHART_DATATYPE_ANY || srcPort->dataType == FLOWCHART_DATATYPE_ANY)) {
						EX_FLOWCHART_CONNECTION conn; conn.id = (INT)GetTickCount64();
						if (srcPort->portType == FLOWCHART_PORTTYPE_OUTPUT) { conn.fromNode = srcNode->id; conn.fromSlot = pData->connectingSlot; conn.toNode = node->id; conn.toSlot = j; }
						else { conn.fromNode = node->id; conn.fromSlot = j; conn.toNode = srcNode->id; conn.toSlot = pData->connectingSlot; }
						_flowchart_addconnection(hObj, &conn);
					}
					break;
				}
			}
		}
		pData->connectingSlot = -1; pData->connectingNode = -1;
	}
	pData->draggingNode = -1; Ex_ObjInvalidateRect(hObj, 0);
}

void _flowchart_ondoubleclick(HEXOBJ hObj, INT x, INT y) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;
	INT scrollX = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_HORZ); INT scrollY = Ex_ObjScrollGetPos(hObj, SCROLLBAR_TYPE_VERT);
	FLOAT virtualX = (x + scrollX) / pData->zoom; FLOAT virtualY = (y + scrollY) / pData->zoom;
	for (INT i = 0; i < pData->nodeCount; i++) {
		EX_FLOWCHART_NODE* node = &pData->nodes[i];
		if (virtualX >= node->x && virtualX <= node->x + node->width && virtualY >= node->y && virtualY <= node->y + node->height) {
			for (INT j = 0; j < node->portCount; j++) {
				EX_FLOWCHART_PORT& port = node->ports[j];
				BOOL showWidget = (port.portType == FLOWCHART_PORTTYPE_INTERMEDIATE) || (port.portType == FLOWCHART_PORTTYPE_INPUT && !port.isConnected);
				// 中间节点或未连线的输入节点如果是编辑框，允许双击编辑
				if (showWidget && port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
					if (virtualX - node->x >= port.widgetRect.left && virtualX - node->x <= port.widgetRect.right && virtualY - node->y >= port.widgetRect.top && virtualY - node->y <= port.widgetRect.bottom) {
						Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_NODEID, node->id); Ex_ObjSetLong(hObj, FLOWCHART_LONG_DOUBLECLICK_PORTID, port.id);
						HEXOBJ edit = Ex_ObjGetLong(hObj, FLOWCHART_LONG_EDIT_HANDLE); Ex_ObjSetText(edit, (LPCWSTR)port.widgetData, FALSE);
						Ex_ObjMove(edit, (node->x + port.widgetRect.left) * pData->zoom, (node->y + port.widgetRect.top) * pData->zoom, (port.widgetRect.right - port.widgetRect.left) * pData->zoom, (port.widgetRect.bottom - port.widgetRect.top) * pData->zoom, TRUE);
						Ex_ObjShow(edit, TRUE); break;
					}
				}
			} break;
		}
	}
}

void _flowchart_onmousewheel(HEXOBJ hObj, SHORT delta) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;
	FLOAT newZoom = pData->zoom * (delta > 0 ? 1.1f : 0.9f);
	if (newZoom < 1.0f) newZoom = 1.0f; // 限制最小缩放系数为1
	if (newZoom > 5.0f) newZoom = 5.0f;
	pData->zoom = newZoom; _flowchart_updatelayout(hObj); Ex_ObjInvalidateRect(hObj, 0);
}

INT _flowchart_removenode(HEXOBJ hObj, INT nodeId) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData || pData->nodeCount == 0) return 0;
	INT index = -1;
	for (INT i = 0; i < pData->nodeCount; i++) { if (pData->nodes[i].id == nodeId) { index = i; break; } }
	if (index == -1) return 0;

	for (INT i = pData->connectionCount - 1; i >= 0; i--) {
		if (pData->connections[i].fromNode == nodeId || pData->connections[i].toNode == nodeId)
			_flowchart_removeconnection(hObj, pData->connections[i].id);
	}

	EX_FLOWCHART_NODE* node = &pData->nodes[index];
	Ex_MemFree((void*)node->title);
	for (INT j = 0; j < node->portCount; j++) {
		Ex_MemFree((void*)node->ports[j].name);
		if (node->ports[j].widgetData) {
			if (node->ports[j].widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) Ex_MemFree(node->ports[j].widgetData);
			else if (node->ports[j].widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
				EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)node->ports[j].widgetData;
				for (INT k = 0; k < combo->count; k++) Ex_MemFree((void*)combo->options[k]);
				Ex_MemFree(combo->options); Ex_MemFree(combo);
			}
			else if (node->ports[j].widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) {
				_img_destroy((HEXIMAGE)node->ports[j].widgetData);
			}
		}
	}
	Ex_MemFree(node->ports);

	if (pData->nodeCount > 1) {
		EX_FLOWCHART_NODE* newNodes = (EX_FLOWCHART_NODE*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE) * (pData->nodeCount - 1));
		if (index > 0) memcpy(newNodes, pData->nodes, sizeof(EX_FLOWCHART_NODE) * index);
		if (index < pData->nodeCount - 1) memcpy(newNodes + index, pData->nodes + index + 1, sizeof(EX_FLOWCHART_NODE) * (pData->nodeCount - index - 1));
		Ex_MemFree(pData->nodes); pData->nodes = newNodes;
	}
	else { Ex_MemFree(pData->nodes); pData->nodes = NULL; }

	pData->nodeCount--;
	_flowchart_updatelayout(hObj);
	Ex_ObjInvalidateRect(hObj, 0);
	return 1;
}

EX_FLOWCHART_NODE* _flowchart_findnode(EX_FLOWCHART_DATA* pData, INT nodeId) {
	for (INT i = 0; i < pData->nodeCount; i++) if (pData->nodes[i].id == nodeId) return &pData->nodes[i];
	return NULL;
}

EX_FLOWCHART_CONNECTION* _flowchart_findconnection(EX_FLOWCHART_DATA* pData, INT connId) {
	for (INT i = 0; i < pData->connectionCount; i++) if (pData->connections[i].id == connId) return &pData->connections[i];
	return NULL;
}

void _flowchart_initcontrolpoint(EX_FLOWCHART_CONNECTION* conn, POINTF virtualFromPt, POINTF virtualToPt) {
	FLOAT dx = virtualToPt.x - virtualFromPt.x;
	FLOAT dy = virtualToPt.y - virtualFromPt.y;
	conn->controlPoint1.x = virtualFromPt.x + dx * 0.25f;
	conn->controlPoint1.y = virtualFromPt.y + dy * 0.25f;
	conn->controlPoint2.x = virtualToPt.x - dx * 0.25f;
	conn->controlPoint2.y = virtualToPt.y - dy * 0.25f;
}

// 将数据从输出端口推向连接的输入端口，并触发下游节点
void _flowchart_propagate_data(HEXOBJ hObj, INT fromNodeId, INT fromSlotIndex, LPVOID data, INT dataType) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;
	for (INT i = 0; i < pData->connectionCount; i++) {
		if (pData->connections[i].fromNode == fromNodeId && pData->connections[i].fromSlot == fromSlotIndex) {
			INT toNodeId = pData->connections[i].toNode; INT toSlotIndex = pData->connections[i].toSlot;
			EX_FLOWCHART_NODE* toNode = _flowchart_findnode(pData, toNodeId);
			if (toNode && toSlotIndex < toNode->portCount && toNode->ports[toSlotIndex].portType == FLOWCHART_PORTTYPE_INPUT) {
				if (toNode->ports[toSlotIndex].widgetData) {
					if (toNode->ports[toSlotIndex].dataType == FLOWCHART_DATATYPE_STRING) Ex_MemFree(toNode->ports[toSlotIndex].widgetData);
					else if (toNode->ports[toSlotIndex].dataType == FLOWCHART_DATATYPE_IMAGE) _img_destroy((HEXIMAGE)toNode->ports[toSlotIndex].widgetData);
				}
				if (dataType == FLOWCHART_DATATYPE_STRING) {
					toNode->ports[toSlotIndex].widgetData = StrDupW((LPCWSTR)data);
				}
				else if (dataType == FLOWCHART_DATATYPE_IMAGE) {
					HEXIMAGE hDst = NULL;
					if (data && _img_copy((HEXIMAGE)data, &hDst)) {
						toNode->ports[toSlotIndex].widgetData = (LPVOID)hDst;
					}
					else {
						toNode->ports[toSlotIndex].widgetData = NULL;
					}
				}
				else {
					toNode->ports[toSlotIndex].widgetData = data;
				}
				_flowchart_executenode(hObj, toNodeId);
			}
		}
	}
}

void _flowchart_executenode(HEXOBJ hObj, INT nodeId) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0); if (!pData) return;
	EX_FLOWCHART_NODE* node = _flowchart_findnode(pData, nodeId); if (!node) return;

	// 防止死循环导致栈溢出 (例如A连B，B又连A)
	if (pData->executionDepth > 100) return;
	pData->executionDepth++;

	// 1. 收集输入数据（将 INTERMEDIATE 也作为节点的本地输入参数收集）
	INT inputCount = 0;
	for (INT i = 0; i < node->portCount; i++) {
		if (node->ports[i].portType == FLOWCHART_PORTTYPE_INPUT || node->ports[i].portType == FLOWCHART_PORTTYPE_INTERMEDIATE)
			inputCount++;
	}
	EX_FLOWCHART_NODE_IO_DATA* inputs = (EX_FLOWCHART_NODE_IO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_IO_DATA) * inputCount);
	INT idx = 0;
	for (INT i = 0; i < node->portCount; i++) {
		if (node->ports[i].portType == FLOWCHART_PORTTYPE_INPUT || node->ports[i].portType == FLOWCHART_PORTTYPE_INTERMEDIATE) {
			inputs[idx].portId = node->ports[i].id;
			inputs[idx].dataType = node->ports[i].dataType;
			inputs[idx].data = node->ports[i].widgetData;
			idx++;
		}
	}

	// 2. 准备输出结构
	INT outputCount = 0;
	for (INT i = 0; i < node->portCount; i++) { if (node->ports[i].portType == FLOWCHART_PORTTYPE_OUTPUT) outputCount++; }
	EX_FLOWCHART_NODE_IO_DATA* outputs = (EX_FLOWCHART_NODE_IO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_IO_DATA) * outputCount);
	memset(outputs, 0, sizeof(EX_FLOWCHART_NODE_IO_DATA) * outputCount);
	idx = 0;
	for (INT i = 0; i < node->portCount; i++) {
		if (node->ports[i].portType == FLOWCHART_PORTTYPE_OUTPUT) {
			outputs[idx].portId = node->ports[i].id;
			outputs[idx].dataType = node->ports[i].dataType;
			idx++;
		}
	}

	// 3. 抛出事件给外部处理
	EX_FLOWCHART_EXECUTE_PARAMS params = { 0 };
	params.nodeId = nodeId;
	params.inputCount = inputCount;
	params.inputs = inputs;
	params.outputCount = outputCount;
	params.outputs = outputs;
	Ex_ObjDispatchNotify(hObj, FLOWCHART_EVENT_EXECUTE_NODE, nodeId, (LPARAM)&params);

	// 4. 将外部返回的输出数据推向下游
	idx = 0;
	for (INT i = 0; i < node->portCount; i++) {
		if (node->ports[i].portType == FLOWCHART_PORTTYPE_OUTPUT) {
			if (params.outputs[idx].data != NULL) {
				// 如果原先缓存了输出数据，需要先清理以防内存泄漏
				if (node->ports[i].widgetData) {
					if (node->ports[i].dataType == FLOWCHART_DATATYPE_STRING) {
						Ex_MemFree(node->ports[i].widgetData);
					}
					else if (node->ports[i].dataType == FLOWCHART_DATATYPE_IMAGE) {
						_img_destroy((HEXIMAGE)node->ports[i].widgetData);
					}
				}
				// 缓存到自身的 widgetData
				node->ports[i].widgetData = params.outputs[idx].data;
				// 将数据推向下游连接的输入端口
				_flowchart_propagate_data(hObj, nodeId, i, params.outputs[idx].data, params.outputs[idx].dataType);
			}
			idx++;
		}
	}

	Ex_MemFree(inputs);
	Ex_MemFree(outputs);
	pData->executionDepth--;
}

POINTF _flowchart_get_port_center(EX_FLOWCHART_NODE* node, INT slotIndex) {
	EX_FLOWCHART_PORT& port = node->ports[slotIndex];
	POINTF pt;
	// 优先使用 portRect
	if (port.portRect.left != 0 || port.portRect.top != 0 || port.portRect.right != 0 || port.portRect.bottom != 0) {
		pt.x = node->x + (port.portRect.left + port.portRect.right) / 2.0f;
		pt.y = node->y + (port.portRect.top + port.portRect.bottom) / 2.0f;
	}
	// 如果 portRect 为空，退而求其次使用 widgetRect
	else if (port.widgetRect.left != 0 || port.widgetRect.top != 0 || port.widgetRect.right != 0 || port.widgetRect.bottom != 0) {
		if (port.portType == FLOWCHART_PORTTYPE_INPUT) {
			pt.x = node->x + port.widgetRect.left;
			pt.y = node->y + (port.widgetRect.top + port.widgetRect.bottom) / 2.0f;
		}
		else if (port.portType == FLOWCHART_PORTTYPE_OUTPUT) {
			pt.x = node->x + port.widgetRect.right;
			pt.y = node->y + (port.widgetRect.top + port.widgetRect.bottom) / 2.0f;
		}
		else { // INTERMEDIATE
			pt.x = node->x + (port.widgetRect.left + port.widgetRect.right) / 2.0f;
			pt.y = node->y + (port.widgetRect.top + port.widgetRect.bottom) / 2.0f;
		}
	}
	else {
		pt.x = node->x;
		pt.y = node->y;
	}
	return pt;
}

BOOL _flowchart_export_to_yaml(HEXOBJ hObj, LPCWSTR filePath) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return FALSE;

	try {
		// 创建YAML根节点
		ryml::Tree tree;
		ryml::NodeRef root = tree.rootref();
		root |= ryml::MAP;

		// 1. 导出视图设置
		ryml::NodeRef view = root["view"];
		view |= ryml::MAP;
		view["zoom"] << pData->zoom;
		view["pan_offset_x"] << pData->panOffset.x;
		view["pan_offset_y"] << pData->panOffset.y;

		// 2. 导出节点数据
		ryml::NodeRef nodes = root["nodes"];
		nodes |= ryml::SEQ;

		for (int i = 0; i < pData->nodeCount; ++i) {
			const EX_FLOWCHART_NODE& node = pData->nodes[i];
			ryml::NodeRef node_yaml = nodes.append_child();
			node_yaml |= ryml::MAP;

			node_yaml["id"] << node.id;
			node_yaml["x"] << node.x;
			node_yaml["y"] << node.y;
			node_yaml["width"] << node.width;
			node_yaml["height"] << node.height;
			node_yaml["title"] << Ex_W2U(node.title);

			// 导出端口数据
			ryml::NodeRef ports = node_yaml["ports"];
			ports |= ryml::SEQ;

			for (int j = 0; j < node.portCount; ++j) {
				const EX_FLOWCHART_PORT& port = node.ports[j];
				ryml::NodeRef port_yaml = ports.append_child();
				port_yaml |= ryml::MAP;

				port_yaml["id"] << port.id;
				port_yaml["port_type"] << port.portType;
				port_yaml["data_type"] << port.dataType;
				port_yaml["name"] << Ex_W2U(port.name);
				port_yaml["is_connected"] << port.isConnected;
				port_yaml["widget_type"] << port.widgetType;
				port_yaml["widget_id"] << port.widgetId;
				port_yaml["widget_width"] << port.widgetWidth;
				port_yaml["widget_height"] << port.widgetHeight;

				// 导出端口矩形位置（修复插槽偏移）
				ryml::NodeRef port_rect = port_yaml["port_rect"];
				port_rect |= ryml::MAP;
				port_rect["left"] << port.portRect.left;
				port_rect["top"] << port.portRect.top;
				port_rect["right"] << port.portRect.right;
				port_rect["bottom"] << port.portRect.bottom;

				// 导出组件矩形位置（修复组件渲染位置）
				ryml::NodeRef widget_rect = port_yaml["widget_rect"];
				widget_rect |= ryml::MAP;
				widget_rect["left"] << port.widgetRect.left;
				widget_rect["top"] << port.widgetRect.top;
				widget_rect["right"] << port.widgetRect.right;
				widget_rect["bottom"] << port.widgetRect.bottom;

				// 导出EDIT数据（修复编辑框数据丢失）
				if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT && port.widgetData) {
					port_yaml["edit_text"] << Ex_W2U((LPCWSTR)port.widgetData);
				}

				// 导出COMBO数据
				if (port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO && port.widgetData) {
					const EX_FLOWCHART_NODE_COMBO_DATA* combo =
						(const EX_FLOWCHART_NODE_COMBO_DATA*)port.widgetData;

					ryml::NodeRef combo_yaml = port_yaml["combo_data"];
					combo_yaml |= ryml::MAP;
					combo_yaml["current"] << combo->current;

					ryml::NodeRef options = combo_yaml["options"];
					options |= ryml::SEQ;
					for (int k = 0; k < combo->count; ++k) {
						options.append_child() << Ex_W2U(combo->options[k]);
					}
				}
			}
		}

		// 3. 导出连接线数据
		ryml::NodeRef connections = root["connections"];
		connections |= ryml::SEQ;

		for (int i = 0; i < pData->connectionCount; ++i) {
			const EX_FLOWCHART_CONNECTION& conn = pData->connections[i];
			ryml::NodeRef conn_yaml = connections.append_child();
			conn_yaml |= ryml::MAP;

			conn_yaml["id"] << conn.id;
			conn_yaml["from_node"] << conn.fromNode;
			conn_yaml["from_slot"] << conn.fromSlot;
			conn_yaml["to_node"] << conn.toNode;
			conn_yaml["to_slot"] << conn.toSlot;

			ryml::NodeRef cp1 = conn_yaml["control_point1"];
			cp1 |= ryml::MAP;
			cp1["x"] << conn.controlPoint1.x;
			cp1["y"] << conn.controlPoint1.y;

			ryml::NodeRef cp2 = conn_yaml["control_point2"];
			cp2 |= ryml::MAP;
			cp2["x"] << conn.controlPoint2.x;
			cp2["y"] << conn.controlPoint2.y;
		}

		// 4. 写入YAML文件
		std::ofstream file(filePath);
		if (!file.is_open()) return FALSE;

		file << tree;
		file.close();
		return TRUE;
	}
	catch (...) {
		return FALSE;
	}
}

void _flowchart_clear_data(EX_FLOWCHART_DATA* pData) {
	if (!pData) return;

	// 释放节点资源
	for (int i = 0; i < pData->nodeCount; ++i) {
		EX_FLOWCHART_NODE& node = pData->nodes[i];
		Ex_MemFree((LPWSTR)node.title);

		// 释放端口资源
		for (int j = 0; j < node.portCount; ++j) {
			EX_FLOWCHART_PORT& port = node.ports[j];
			Ex_MemFree((LPWSTR)port.name);

			// 释放COMBO数据
			if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT) {
				Ex_MemFree(port.widgetData);
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO && port.widgetData) {
				EX_FLOWCHART_NODE_COMBO_DATA* combo = (EX_FLOWCHART_NODE_COMBO_DATA*)port.widgetData;
				for (int k = 0; k < combo->count; ++k) {
					Ex_MemFree((LPWSTR)combo->options[k]);
				}
				Ex_MemFree(combo->options);
				Ex_MemFree(combo);
			}
			else if (port.widgetType == FLOWCHART_NODEDATA_TYPE_IMAGE) {
				_img_destroy((HEXIMAGE)port.widgetData);
			}
		}
		Ex_MemFree(node.ports);
	}
	Ex_MemFree(pData->nodes);
	pData->nodeCount = 0;

	// 释放连接线资源
	Ex_MemFree(pData->connections);
	pData->connectionCount = 0;

	// 重置状态
	pData->selectedNode = -1;
	pData->selectedConnection = -1;
	pData->connectingNode = -1;
	pData->connectingSlot = -1;
}

BOOL _flowchart_import_from_yaml(HEXOBJ hObj, LPCWSTR filePath) {
	EX_FLOWCHART_DATA* pData = (EX_FLOWCHART_DATA*)Ex_ObjGetLong(hObj, 0);
	if (!pData) return FALSE;

	try {
		// 1. 读取YAML文件
		std::ifstream file(filePath);
		if (!file.is_open()) return FALSE;

		std::string yaml_str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		file.close();

		// 2. 解析YAML
		ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(yaml_str));
		ryml::NodeRef root = tree.rootref();
		if (!root.is_map()) return FALSE;

		// 3. 清空现有数据
		_flowchart_clear_data(pData);

		// 4. 导入视图设置
		if (root.has_child("view")) {
			ryml::NodeRef view = root["view"];
			view["zoom"] >> pData->zoom;
			view["pan_offset_x"] >> pData->panOffset.x;
			view["pan_offset_y"] >> pData->panOffset.y;
		}

		// 5. 导入节点数据
		if (root.has_child("nodes")) {
			ryml::NodeRef nodes = root["nodes"];
			if (nodes.is_seq()) {
				pData->nodeCount = (INT)nodes.num_children();
				pData->nodes = (EX_FLOWCHART_NODE*)Ex_MemAlloc(pData->nodeCount * sizeof(EX_FLOWCHART_NODE));
				memset(pData->nodes, 0, pData->nodeCount * sizeof(EX_FLOWCHART_NODE));

				for (int i = 0; i < pData->nodeCount; ++i) {
					ryml::NodeRef node_yaml = nodes[i];
					EX_FLOWCHART_NODE& node = pData->nodes[i];

					node_yaml["id"] >> node.id;
					node_yaml["x"] >> node.x;
					node_yaml["y"] >> node.y;
					node_yaml["width"] >> node.width;
					node_yaml["height"] >> node.height;

					std::string title_utf8;
					node_yaml["title"] >> title_utf8;
					node.title = StrDupW(Ex_U2W(title_utf8).c_str());

					// 导入端口数据
					if (node_yaml.has_child("ports")) {
						ryml::NodeRef ports = node_yaml["ports"];
						node.portCount = (INT)ports.num_children();
						node.ports = (EX_FLOWCHART_PORT*)Ex_MemAlloc(node.portCount * sizeof(EX_FLOWCHART_PORT));
						memset(node.ports, 0, node.portCount * sizeof(EX_FLOWCHART_PORT));

						for (int j = 0; j < node.portCount; ++j) {
							ryml::NodeRef port_yaml = ports[j];
							EX_FLOWCHART_PORT& port = node.ports[j];

							port_yaml["id"] >> port.id;
							port_yaml["port_type"] >> port.portType;
							port_yaml["data_type"] >> port.dataType;

							std::string name_utf8;
							port_yaml["name"] >> name_utf8;
							port.name = StrDupW(Ex_U2W(name_utf8).c_str());

							port_yaml["is_connected"] >> port.isConnected;
							port_yaml["widget_type"] >> port.widgetType;
							port_yaml["widget_id"] >> port.widgetId;
							port_yaml["widget_width"] >> port.widgetWidth;
							port_yaml["widget_height"] >> port.widgetHeight;

							// 导入端口矩形位置（修复插槽偏移）
							if (port_yaml.has_child("port_rect")) {
								ryml::NodeRef port_rect = port_yaml["port_rect"];
								port_rect["left"] >> port.portRect.left;
								port_rect["top"] >> port.portRect.top;
								port_rect["right"] >> port.portRect.right;
								port_rect["bottom"] >> port.portRect.bottom;
							}

							// 导入组件矩形位置（修复组件渲染位置）
							if (port_yaml.has_child("widget_rect")) {
								ryml::NodeRef widget_rect = port_yaml["widget_rect"];
								widget_rect["left"] >> port.widgetRect.left;
								widget_rect["top"] >> port.widgetRect.top;
								widget_rect["right"] >> port.widgetRect.right;
								widget_rect["bottom"] >> port.widgetRect.bottom;
							}

							// 初始化widgetData为NULL
							port.widgetData = NULL;

							// 导入EDIT数据（修复编辑框不渲染）
							if (port.widgetType == FLOWCHART_NODEDATA_TYPE_EDIT && port_yaml.has_child("edit_text")) {
								std::string edit_text_utf8;
								port_yaml["edit_text"] >> edit_text_utf8;
								port.widgetData = (LPVOID)StrDupW(Ex_U2W(edit_text_utf8).c_str());
							}

							// 导入COMBO数据
							if (port_yaml.has_child("combo_data") && port.widgetType == FLOWCHART_NODEDATA_TYPE_COMBO) {
								EX_FLOWCHART_NODE_COMBO_DATA* combo =
									(EX_FLOWCHART_NODE_COMBO_DATA*)Ex_MemAlloc(sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
								memset(combo, 0, sizeof(EX_FLOWCHART_NODE_COMBO_DATA));
								ryml::NodeRef combo_yaml = port_yaml["combo_data"];

								combo_yaml["current"] >> combo->current;

								ryml::NodeRef options = combo_yaml["options"];
								combo->count = (INT)options.num_children();
								combo->options = (LPCWSTR*)Ex_MemAlloc(combo->count * sizeof(LPCWSTR));

								for (int k = 0; k < combo->count; ++k) {
									std::string opt_utf8;
									options[k] >> opt_utf8;
									combo->options[k] = StrDupW(Ex_U2W(opt_utf8).c_str());
								}
								port.widgetData = combo;
							}
						}
					}
				}

				// 导入完成后，重新计算所有节点大小（修复布局错乱）
				for (int i = 0; i < pData->nodeCount; ++i) {
					_flowchart_calcnodesize(hObj, &pData->nodes[i]);
				}
			}
		}

		// 6. 导入连接线数据
		if (root.has_child("connections")) {
			ryml::NodeRef connections = root["connections"];
			if (connections.is_seq()) {
				pData->connectionCount = (INT)connections.num_children();
				pData->connections = (EX_FLOWCHART_CONNECTION*)Ex_MemAlloc(pData->connectionCount * sizeof(EX_FLOWCHART_CONNECTION));
				memset(pData->connections, 0, pData->connectionCount * sizeof(EX_FLOWCHART_CONNECTION));

				for (int i = 0; i < pData->connectionCount; ++i) {
					ryml::NodeRef conn_yaml = connections[i];
					EX_FLOWCHART_CONNECTION& conn = pData->connections[i];

					conn_yaml["id"] >> conn.id;
					conn_yaml["from_node"] >> conn.fromNode;
					conn_yaml["from_slot"] >> conn.fromSlot;
					conn_yaml["to_node"] >> conn.toNode;
					conn_yaml["to_slot"] >> conn.toSlot;

					ryml::NodeRef cp1 = conn_yaml["control_point1"];
					cp1["x"] >> conn.controlPoint1.x;
					cp1["y"] >> conn.controlPoint1.y;

					ryml::NodeRef cp2 = conn_yaml["control_point2"];
					cp2["x"] >> conn.controlPoint2.x;
					cp2["y"] >> conn.controlPoint2.y;
				}
			}
		}

		// 7. 更新整体布局并刷新界面
		_flowchart_updatelayout(hObj);
		Ex_ObjInvalidateRect(hObj, NULL);
		return TRUE;
	}
	catch (...) {
		_flowchart_clear_data(pData);
		return FALSE;
	}
}