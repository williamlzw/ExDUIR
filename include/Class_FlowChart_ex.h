#pragma once

#define FLOWCHART_SLOTTYPE_INPUT 1
#define FLOWCHART_SLOTTYPE_OUTPUT 2

#define FLOWCHART_LONG_DATA               0
#define FLOWCHART_LONG_MOUSE_X            1
#define FLOWCHART_LONG_MOUSE_Y            2
#define FLOWCHART_LONG_EDIT_HANDLE        3
#define FLOWCHART_LONG_DOUBLECLICK_NODEID 4
#define FLOWCHART_LONG_DOUBLECLICK_PORTID 5
#define FLOWCHART_LONG_BACKGROUNDCOLOR    6

void _flowchart_register();
LRESULT CALLBACK _flowchart_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _flowchart_paint(HEXOBJ hObj);
void _flowchart_updatelayout(HEXOBJ hObj);
void _flowchart_calcnodesize(HEXOBJ hObj, EX_FLOWCHART_NODE* node);
void _flowchart_drawnode(HEXCANVAS hCanvas, EX_FLOWCHART_DATA* pData, EX_FLOWCHART_NODE* node, BOOL selected, FLOAT zoom, INT scrollX, INT scrollY);
void _flowchart_onmousemove(HEXOBJ hObj, INT x, INT y);
void _flowchart_onlbuttondown(HEXOBJ hObj, INT x, INT y);
void _flowchart_onlbuttonup(HEXOBJ hObj, INT x, INT y);
void _flowchart_ondoubleclick(HEXOBJ hObj, INT x, INT y);
void _flowchart_onmousewheel(HEXOBJ hObj, SHORT delta);
void _flowchart_onscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
INT _flowchart_addnode(HEXOBJ hObj, EX_FLOWCHART_NODE* pNode);
INT _flowchart_removenode(HEXOBJ hObj, INT nodeId);
INT _flowchart_addconnection(HEXOBJ hObj, EX_FLOWCHART_CONNECTION* pConn);
INT _flowchart_removeconnection(HEXOBJ hObj, INT connId);
EX_FLOWCHART_NODE* _flowchart_findnode(EX_FLOWCHART_DATA* pData, INT nodeId);
EX_FLOWCHART_CONNECTION* _flowchart_findconnection(EX_FLOWCHART_DATA* pData, INT connId);
void _flowchart_initcontrolpoint(EX_FLOWCHART_CONNECTION* conn, POINTF virtualFromPt, POINTF virtualToPt);
EXARGB _flowchart_get_port_color(INT dataType);
void _flowchart_executenode(HEXOBJ hObj, INT nodeId);
POINTF _flowchart_get_port_center(EX_FLOWCHART_NODE* node, INT slotIndex);
INT _flowchart_import_from_yaml(HEXOBJ hObj, LPCWSTR filePath);
INT _flowchart_export_to_yaml(HEXOBJ hObj, LPCWSTR filePath);
void _flowchart_clear_data(EX_FLOWCHART_DATA* pData);