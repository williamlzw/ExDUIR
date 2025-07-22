#pragma once

// 属性_流程图_数据指针
#define FLOWCHART_LONG_DATA 0
// 属性_流程图_鼠标横坐标
#define FLOWCHART_LONG_MOUSE_X 1
// 属性_流程图_鼠标纵坐标
#define FLOWCHART_LONG_MOUSE_Y 2
// 属性_流程图_编辑框
#define FLOWCHART_LONG_EDIT_HANDLE 3
// 属性_流程图_双击编辑框所处节点ID
#define FLOWCHART_LONG_DOUBLECLICK_NODEID 4
// 属性_流程图_双击编辑框所处数据ID
#define FLOWCHART_LONG_DOUBLECLICK_DATAID 5

// 流程图_连接点类型
#define FLOWCHART_SLOTTYPE_INPUT 1
#define FLOWCHART_SLOTTYPE_OUTPUT 2

void _flowchart_register();
LRESULT CALLBACK _flowchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _flowchart_addnode(HEXOBJ hObj, EX_FLOWCHART_NODE* pNode);
INT _flowchart_removenode(HEXOBJ hObj, INT nodeId);
INT _flowchart_addconnection(HEXOBJ hObj, EX_FLOWCHART_CONNECTION* pConn);
INT _flowchart_removeconnection(HEXOBJ hObj, INT connId);
void _flowchart_updatelayout(HEXOBJ hObj);
void _flowchart_paint(HEXOBJ hObj);
void _flowchart_onmousemove(HEXOBJ hObj, INT x, INT y);
void _flowchart_onlbuttondown(HEXOBJ hObj, INT x, INT y);
void _flowchart_onlbuttonup(HEXOBJ hObj, INT x, INT y);
void _flowchart_ondoubleclick(HEXOBJ hObj, INT x, INT y);
void _flowchart_onmousewheel(HEXOBJ hObj, SHORT delta);
EX_FLOWCHART_NODE* _flowchart_findnode(EX_FLOWCHART_DATA* pData, INT nodeId);
void _flowchart_drawnode(HEXCANVAS hCanvas, EX_FLOWCHART_DATA* pData, EX_FLOWCHART_NODE* node, BOOL selected,
    FLOAT zoom, INT scrollX, INT scrollY);
void _flowchart_onscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
EX_FLOWCHART_CONNECTION* _flowchart_findconnection(EX_FLOWCHART_DATA* pData, INT connId);
void _flowchart_initcontrolpoint(EX_FLOWCHART_CONNECTION* conn,
    POINTF virtualFromPt, POINTF virtualToPt);
void _flowchart_calcnodesize(HEXOBJ hObj, EX_FLOWCHART_NODE* node);
void _flowchart_measure_text(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR text,
    INT maxWidth, FLOAT* width, FLOAT* height);
LRESULT CALLBACK _flowchart_edit_killfocus(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam,
    LPARAM lParam);