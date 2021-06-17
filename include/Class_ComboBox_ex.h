#pragma once
#include "help_ex.h"

//组合框消息_置可视数量
#define CB_SETMINVISIBLE 5889
//组合框消息_取可视数量
#define CB_GETMINVISIBLE 5890 

struct EX_COMBOX_ITEM {
	LPCWSTR lpwzTitle;
	LONG_PTR data;
};
struct EX_COMBOX_ITEMLIST {
	INT size;
	EX_COMBOX_ITEM items[1];
};

void _ComboBox_regsiter();
LRESULT CALLBACK _cb_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _cb_size(HWND hWnd, obj_s* pObj, HEXOBJ hObj, INT width, INT height);
EX_COMBOX_ITEMLIST* _cb_realloc(obj_s* pObj, INT need, INT nIndex, BOOL add);
INT _cb_insertstring(obj_s* pObj, size_t nIndex, LPCWSTR lpTitle);
INT _cb_addstring(obj_s* pObj, LPCWSTR lpTitle);
INT _cb_paint(HEXOBJ hObj, obj_s* pObj);
void _cb_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
LRESULT CALLBACK _cb_wnd_proc(HWND hWnd, HEXDUI hDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
INT _cb_delstring(obj_s* pObj, size_t nIndex);
size_t _cb_getlbtext(obj_s* pObj, size_t nIndex, LPVOID buffer);
INT _cb_findstring(obj_s* pObj, size_t startIndex, LPCWSTR lpTitle);
void _cb_wnd_customdraw(obj_s* pObj, WPARAM wParam, EX_CUSTOMDRAW* lParam);
void _cb_init(obj_s* pObj, HEXOBJ hObj);
INT _cb_setcursel(obj_s* pObj, size_t nIndex);
BOOL _cb_uninit(obj_s* pObj);
INT _cb_resetcontent(obj_s* pObj);
void _cb_notify(HWND hWnd, obj_s* pObj, HEXOBJ hObj, EX_NMHDR* lParam);
INT _cb_getlbtextlen(obj_s* pObj, size_t nIndex);
LPCWSTR _cb_getitemtitle(obj_s* pObj, size_t nIndex);
INT _cb_initstorage(obj_s* pObj, size_t size);
INT _cb_setitemdata(obj_s* pObj, size_t nIndex, LONG_PTR data);
LONG_PTR _cb_getitemdata(obj_s* pObj, size_t nIndex);

