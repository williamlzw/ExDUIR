#pragma once
#include "help_ex.h"

struct listview_s
{
	INT width_item_;//项目宽度
	INT height_item_;
	INT width_split_;//间隔宽度
	INT height_split_;
	INT width_spec_;//特殊项目宽度
	INT height_spec_;
	INT width_view_;//项目总宽度
	INT height_view_;
	INT count_selects_;//选中项目数
	INT count_items_;//项目总数
	INT count_view_;//可视总数量
	INT count_view_h_;//可视横向数量
	INT count_view_v_; //可视纵向数量
	INT index_select_;//当前选中
	INT index_mouse_;//热点项目
	INT index_start_;
	INT index_end_;
	INT index_track_start_;
	INT index_track_end_;
	INT nHittest_;
	INT nOffsetX_;
	INT nOffsetY_;
	LPVOID lpItems_;
};

void _ListView_regsiter();
void _lv_updatesbvalue(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, RECT* lpRcClient);
void _lv_updateviewindex(HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, BOOL bHView, RECT* lpRcClient);
BOOL _lv_checkitem(listview_s* pOwner, INT iItem);
BOOL _lv_checkitem_view(listview_s* pOwner, INT iItem);
void _lv_rectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* rcItem);
void _lv_lprectfromiitem(obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bHView, RECT* lpRc);
INT _lv_itemfrompos(obj_s* pObj, listview_s* pOwner, INT x, INT y, INT* offsetPosX, INT* offsetPosY);
INT _lv_getitemstate(LPVOID lpItems, INT iItem);
void _lv_setitemstate(LPVOID lpItems, INT iItem, INT dwState, BOOL bRemove);
BOOL _lv_queryitemstate(LPVOID lpItems, INT iItem, INT dwState);
void _lv_getsbvalue(obj_s* pObj, listview_s* pOwner, BOOL bHSB, HEXOBJ* hSB, INT* oPos, INT* nLine, INT* nPage, INT* nView);
INT _lv_checkpos(INT nPos, INT nView, INT nPage);
void _lv_redrawitem(obj_s* pObj, listview_s* pOwner, INT iItem);
void _lv_item_changestate(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, LPVOID lpItems, INT iItem, INT state, BOOL bRemove, INT nEvent, WPARAM wParam, LPARAM lParam);
size_t _lv_setitemcount(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT nCount, LPARAM lParam);
void _lv_onvscrollbar(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, WPARAM wParam, LPARAM lParam);
size_t _lv_showitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT iItem, BOOL bCheck);
size_t _lv_reselect(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, INT iItem, BOOL bShow);
void _lv_btndown(HWND hWnd, HEXOBJ hObj, obj_s* pObj, INT uMsg, size_t wParram, LPARAM lParam);
void _lv_mouseleave(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _lv_mousemove(HWND hWnd, HEXOBJ hObj, obj_s* pObj, WPARAM wParam, LPARAM lParam);
void _lv_drawitem(HWND hWnd, HEXOBJ hObj, obj_s* pObj, listview_s* pOwner, EX_PAINTSTRUCT2 ps, INT iItem, RECT rcClip, RECT rcItem);
size_t _lv_paint(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
size_t _lv_size(HWND hWnd, HEXOBJ hObj, obj_s* pObj);
void _lv_lvm_setitemstate(obj_s* pObj, listview_s* pOwner, INT iItem, INT dwState);
LRESULT CALLBACK _lv_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);