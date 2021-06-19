#pragma once
#include "help_ex.h"

// 图标列表属性_表项宽度
#define EILVL_WIDTH 0
// 图标列表属性_表项高度
#define EILVL_HEIGHT 1
// 图标列表属性_表项数组
#define EILVL_ITEMARRAY 2
// 图标列表属性_图片组句柄
#define EILVL_HIMAGELIST 3


LRESULT CALLBACK _iconlistview_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _iconlistview_register();
EX_ICONLISTVIEW_ITEMINFO* _iconlistview_onarrappend(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType);
void _iconlistview_onarrdelete(array_s* pArray, INT nIndex, EX_ICONLISTVIEW_ITEMINFO* pvItem, INT nType);
void _iconlistview_init(HEXOBJ hObj);
void _iconlistview_uninit(HEXOBJ hObj);
BOOL _iconlistview_onnotify(HEXOBJ hObj, WPARAM wParam, LPARAM lParam);
BOOL _iconlistview_ondrawitem(HEXOBJ hObj, EX_CUSTOMDRAW* cdr);