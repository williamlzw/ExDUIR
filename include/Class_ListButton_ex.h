#pragma once
#include "help_ex.h"

// 列表属性_类型
#define LB_TYPE 0
// 列表属性_表项数
#define LB_CTCS 1
// 列表属性_表项信息
#define LB_ITEMINFO 2
// 列表属性_热点项目
#define LB_INDEX 3
// 列表属性_图片组句柄
#define LB_HIMAGELIST 4
// 列表属性_图片组宽度
#define LB_HIMAGWIDTH 5
// 列表属性_图片组高度
#define LB_HIMAGHEIGHT 6

void _listbuttonex_register();
LRESULT CALLBACK _listbuttonex_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _listbuttonex_arr_del(array_s *hArr, INT nIndex, EX_LISTBUTTON_ITEMINFO *pvData, INT nType);
void _listbuttonex_paint(HEXOBJ hObj);
void _listbuttonex_mousemove(HEXOBJ hObj, INT nType, LPARAM lParam);
void _listbuttonex_mousedown(HEXOBJ hObj, INT nType, LPARAM lParam);
void _listbuttonex_mouseup(HEXOBJ hObj, INT nType, LPARAM lParam);
void _listbuttonex_mouseleave(HEXOBJ hObj, INT nType);
void _listbuttonex_update(HEXOBJ hObj);
INT _listbuttonex_hittest(HEXOBJ hObj, array_s *hArr, POINT pt);
void _listbuttonex_recovery(HEXOBJ hObj, INT nType, array_s *hArr, INT Index, BOOL Update);
INT _listbuttonex_itemWidth(HEXOBJ hObj, INT nType, UINT nImage, LPCWSTR wzText);
LRESULT CALLBACK _listbuttonex_menu_proc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lpResult);