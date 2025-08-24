#pragma once

// 属性_属性框_项目数据
#define PROPERTYGRID_LONG_ITEMARRAY 0
// 属性_属性框_纵坐标偏移
#define PROPERTYGRID_LONG_TOP_OFFSET 1
// 属性_属性框_行高
#define PROPERTYGRID_LONG_LINEHEIGHT 2
// 属性_属性框_选中项目
#define PROPERTYGRID_LONG_SELITEM 3
// 属性_属性框_热点项目
#define PROPERTYGRID_LONG_HOTITEM 4
// 属性_属性框_列宽
#define PROPERTYGRID_LONG_COLUMNWIDTH 5
// 属性_属性框_热点项目背景色
#define PROPERTYGRID_LONG_ITEMHOTCOLOR 6
// 属性_属性框_选中项目背景色
#define PROPERTYGRID_LONG_ITEMSELCOLOR 7
// 属性_属性框_编辑框句柄
#define PROPERTYGRID_LONG_HOBJEDIT 8
// 属性_属性框_组合框句柄
#define PROPERTYGRID_LONG_HOBJCOMBOBOX 9
// 属性_属性框_颜色框句柄
#define PROPERTYGRID_LONG_HOBJCOLORPICKER 10
// 属性_属性框_日期框句柄
#define PROPERTYGRID_LONG_HOBJDATEBOX 11
// 属性_属性框_按钮句柄
#define PROPERTYGRID_LONG_HOBJBUTTON 12
// 属性_属性框_线条颜色
#define PROPERTYGRID_LONG_LINECOLOR 13

void _propertygrid_register();
void _propertygrid_update_layout(HEXOBJ hObj);
LRESULT CALLBACK _propertygrid_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _propertygrid_calc_layout(HEXOBJ hObj, EX_PROPERTYGRID_ITEMINFO_SUBITEM* sub, INT widthClient);
void _propertygrid_paint(HEXOBJ hObj);
void _propertygrid_onvscrollbar(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _propertygrid_onlbuttonup(HEXOBJ hObj, INT x, INT y);
void _propertygrid_onmousemove(HEXOBJ hObj, INT x, INT y);
BOOL _propertygrid_is_item_visible(HEXOBJ hObj, INT index);
INT _propertygrid_find_group_index(HEXOBJ hObj, INT groupId);
LRESULT CALLBACK _propertygrid_oneditevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _propertygrid_oncomboboxevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _propertygrid_ondateboxevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _propertygrid_oncolorpickerevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _propertygrid_onbuttonevent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void _propertygrid_setitemtext(HEXOBJ hObj, INT index, LPCWSTR content);
LRESULT CALLBACK _propertygrid_onscrollbarmsg(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult);
void _propertygrid_onlbuttondblclk(HEXOBJ hObj, INT x, INT y);
LRESULT CALLBACK _propertygrid_oneditmsgproc(HWND hWND, HEXOBJ hObj, INT uMsg, WPARAM wParam,
	LPARAM lParam, LRESULT* lpResult);
