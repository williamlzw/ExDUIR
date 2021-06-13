#pragma once
#include "help_ex.h"



//图标列表属性：表项宽度
#define _ilv_nWidth 0
//图标列表属性：表项高度
#define _ilv_nHeight 1
//图标列表属性：表项数组
#define _ilv_pArray 2
//图标列表属性：图片组句柄
#define _ilv_hImageList 3




LRESULT CALLBACK _IconListView_Proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _IconListView_Register();
EX_ICONLISTVIEW_ITEMINFO* _IconListView_OnArrAppend(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType);
void _IconListView_OnArrDelete(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType);
void _IconListView_Init(HEXOBJ hObj);
void _IconListView_Uninit(HEXOBJ hObj);
BOOL _IconListView_OnNotify(HEXOBJ hObj, WPARAM wParam, LPARAM lParam);
BOOL _IconListView_OnDrawItem(HEXOBJ hObj, EX_CUSTOMDRAW* cdr);