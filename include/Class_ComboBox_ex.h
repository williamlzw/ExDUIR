#pragma once
#include "help_ex.h"

// 组合框属性_按下状态
#define ECBL_STATE	0
// 组合框属性_编辑框句柄
#define ECBL_HOBJEDIT	1
// 组合框属性_矩形左边
#define ECBL_LEFT	2
// 组合框属性_矩形顶边
#define ECBL_TOP	3
// 组合框属性_矩形右边
#define ECBL_RIGHT	4
// 组合框属性_矩形底边
#define ECBL_BOTTOM	5
// 组合框属性_弹出面板窗口句柄
#define ECBL_BOXHWND	6
// 组合框属性_列表框句柄
#define ECBL_HOBJLISTVIEW	7
// 组合框属性_当前选中项
#define ECBL_CURRENTSELECTED	8
// 组合框属性_项目数量
#define ECBL_ITEMCOUNT	9
// 组合框属性_项目数据
#define ECBL_ITEMLIST	10
// 组合框属性_下拉列表宽度
#define ECBL_DROPPEDWIDTH	11
// 组合框属性_表项高度
#define ECBL_ITEMHEIGHT	12
// 组合框属性_可视项目数量
#define ECBL_MINVISIBLE	13

// 组合框消息_置可视数量
#define CB_SETMINVISIBLE 5889
// 组合框消息_取可视数量
#define CB_GETMINVISIBLE 5890

// 组合框标识_悬浮
#define ECBF_HOVER	1

struct EX_COMBOX_ITEM {
	LPCWSTR lpwzTitle;
	LONG_PTR data;
};
struct EX_COMBOX_ITEMLIST {
	INT size;
	EX_COMBOX_ITEM items[1];
};

void _combobox_regsiter();
LRESULT CALLBACK _combobox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _combobox_size(HWND hWnd, obj_s* pObj, HEXOBJ hObj, INT width, INT height);
EX_COMBOX_ITEMLIST* _combobox_realloc(obj_s* pObj, INT need, INT nIndex, BOOL add);
INT _combobox_insertstring(obj_s* pObj, size_t nIndex, LPCWSTR lpTitle);
INT _combobox_addstring(obj_s* pObj, LPCWSTR lpTitle);
void _combobox_paint(HEXOBJ hObj, obj_s* pObj);
void _combobox_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _combobox_wnd_proc(HWND hWnd, HEXDUI hDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
INT _combobox_delstring(obj_s* pObj, size_t nIndex);
size_t _combobox_getleftbuttontext(obj_s* pObj, size_t nIndex, LPVOID buffer);
INT _combobox_findstring(obj_s* pObj, size_t startIndex, LPCWSTR lpTitle);
void _combobox_wnd_customdraw(obj_s* pObj, WPARAM wParam, EX_CUSTOMDRAW* lParam);
void _combobox_init(obj_s* pObj, HEXOBJ hObj);
INT _combobox_setcursel(obj_s* pObj, size_t nIndex);
BOOL _combobox_uninit(obj_s* pObj);
INT _combobox_resetcontent(obj_s* pObj);
void _combobox_notify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, EX_NMHDR* lParam);
INT _combobox_getleftbuttontextlen(obj_s* pObj, size_t nIndex);
LPCWSTR _combobox_getitemtitle(obj_s* pObj, size_t nIndex);
INT _combobox_initstorage(obj_s* pObj, size_t size);
INT _combobox_setitemdata(obj_s* pObj, size_t nIndex, LONG_PTR data);
LONG_PTR _combobox_getitemdata(obj_s* pObj, size_t nIndex);

